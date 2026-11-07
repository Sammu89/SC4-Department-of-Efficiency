// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

#include "UtilityModel.h"
#include "Logging.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <map>
#include <sstream>
#include <string>
#include <vector>

namespace sc4af {

// Pure, read-only correlation of snapshots collected by the runtime recorder.
// It identifies only observable effects; a missing UI or internal engine event
// remains NOT_CAPTURED/UNRESOLVED instead of being guessed from a later snapshot.
enum class Phase8CaptureStatus { NotCaptured, Captured, NotApplicable, Unresolved };
enum class Phase8Capture {
    DealCreate, DealChange, DealCancel, DealCloseRefresh, DealReopen, DealPersistence,
    DealRenewal, DealSuspension, LatentPricing, GarbageAbducted, GarbageFundingLimits,
    GarbageFundingTiming, GarbageCapacityReadback, LandfillIntake, Count
};

inline const char* Phase8CaptureName(Phase8Capture capture) {
    static constexpr const char* names[] = {
        "DEAL_CREATE", "DEAL_CHANGE", "DEAL_CANCEL", "DEAL_CLOSE_REFRESH", "DEAL_REOPEN", "DEAL_PERSISTENCE",
        "DEAL_RENEWAL", "DEAL_SUSPENSION", "LATENT_PRICING", "GARBAGE_ABDUCTED", "GARBAGE_FUNDING_LIMITS",
        "GARBAGE_FUNDING_TIMING", "GARBAGE_CAPACITY_READBACK", "LANDFILL_INTAKE"};
    return names[static_cast<std::size_t>(capture)];
}

inline const char* Phase8CaptureStatusName(Phase8CaptureStatus status) {
    switch (status) {
    case Phase8CaptureStatus::NotCaptured: return "NOT_CAPTURED";
    case Phase8CaptureStatus::Captured: return "CAPTURED";
    case Phase8CaptureStatus::NotApplicable: return "NOT_APPLICABLE";
    case Phase8CaptureStatus::Unresolved: return "UNRESOLVED";
    }
    return "UNRESOLVED";
}

// Flag meanings verified for 1.1.641.0. Proposal objects returned by
// ProposeDeals are deliberately not contracts, even when they have the same
// neighbor/type as a later contract.
enum class DealLifecycleState { Proposal, ActiveContract, CancelledPendingRemoval, Suspended, Terminal, Ambiguous };
inline const char* DealLifecycleStateName(DealLifecycleState state) {
    switch (state) {
    case DealLifecycleState::Proposal: return "PROPOSAL";
    case DealLifecycleState::ActiveContract: return "ACTIVE_CONTRACT";
    case DealLifecycleState::CancelledPendingRemoval: return "CANCELLED_PENDING_REMOVAL";
    case DealLifecycleState::Suspended: return "SUSPENDED";
    case DealLifecycleState::Terminal: return "TERMINAL";
    case DealLifecycleState::Ambiguous: return "AMBIGUOUS";
    }
    return "AMBIGUOUS";
}

inline DealLifecycleState ClassifyDealLifecycle(const ExistingDealRow& deal) {
    if (!deal.flags) return DealLifecycleState::Ambiguous;
    const auto flags = static_cast<std::uint32_t>(*deal.flags);
    if ((flags & 0x200u) != 0) return DealLifecycleState::Terminal;
    if ((flags & 0x100u) != 0) return DealLifecycleState::Suspended;
    if ((flags & 0x04u) != 0) return DealLifecycleState::CancelledPendingRemoval;
    if ((flags & 0x02u) != 0 && (flags & 0x38u) == 0) return DealLifecycleState::ActiveContract;
    if ((flags & 0x01u) != 0 && (flags & 0x02u) == 0) return DealLifecycleState::Proposal;
    return DealLifecycleState::Ambiguous;
}

// Vanilla prevents a second active deal of the same type with the same
// neighbor. Therefore type+neighbor is the minimal stable active-contract
// identity. Pointer, amount, cost and dates are deliberately excluded: all
// are mutable or ephemeral across save/reload. An unknown neighbor never
// produces a key, so collisions fail closed.
struct DealSemanticKey {
    std::int32_t type = 0;
    std::uint32_t neighbor = 0;
    bool operator<(const DealSemanticKey& other) const { return std::tie(type, neighbor) < std::tie(other.type, other.neighbor); }
    bool operator==(const DealSemanticKey&) const = default;
};
inline std::optional<DealSemanticKey> ActiveDealSemanticKey(const ExistingDealRow& deal) {
    if (ClassifyDealLifecycle(deal) != DealLifecycleState::ActiveContract || !deal.otherCitySerial) return std::nullopt;
    return DealSemanticKey{deal.type, *deal.otherCitySerial};
}

struct DealPersistenceObservation {
    enum class Result { PersistedExact, PersistedChanged, MissingAfterReload, NewAfterReload, Ambiguous };
    Result result = Result::Ambiguous;
    DealSemanticKey key{};
};
inline const char* DealPersistenceResultName(DealPersistenceObservation::Result result) {
    switch (result) {
    case DealPersistenceObservation::Result::PersistedExact: return "PERSISTED_EXACT";
    case DealPersistenceObservation::Result::PersistedChanged: return "PERSISTED_CHANGED";
    case DealPersistenceObservation::Result::MissingAfterReload: return "MISSING_AFTER_RELOAD";
    case DealPersistenceObservation::Result::NewAfterReload: return "NEW_AFTER_RELOAD";
    case DealPersistenceObservation::Result::Ambiguous: return "AMBIGUOUS";
    }
    return "AMBIGUOUS";
}

class Phase8RuntimeTracker {
public:
    Phase8RuntimeTracker() {
        // The exposed department ABI provides funding, maximum and fixed state, but no
        // version-validated minimum getter. Keep this explicitly unresolved.
        Unresolved(Phase8Capture::GarbageFundingLimits);
        Unresolved(Phase8Capture::LandfillIntake);
    }
    void ResetCity() { previous_.clear(); pendingAmountChanges_.clear(); hasBaseline_ = false; previousUiDisabled_.reset(); landfillFlowObserved_ = false; fundingChanged_ = false; lastGarbageFunding_.reset(); lastW2eCapacity_.reset(); }
    void SetReady(bool dealMessageSubscribed, bool garbageMessageSubscribed) { ready_ = dealMessageSubscribed && garbageMessageSubscribed; }
    bool ready() const { return ready_; }
    Phase8CaptureStatus Status(Phase8Capture capture) const { return statuses_[static_cast<std::size_t>(capture)]; }
    void Capture(Phase8Capture capture) { statuses_[static_cast<std::size_t>(capture)] = Phase8CaptureStatus::Captured; }
    void Unresolved(Phase8Capture capture) { statuses_[static_cast<std::size_t>(capture)] = Phase8CaptureStatus::Unresolved; }

    void ObserveGarbage(const GarbageState& garbage, bool sawScalingMessage, const char* reason) {
        const auto& a = garbage.aggregate;
        if (a.abducted) Capture(Phase8Capture::GarbageAbducted);
        if (a.toLandfill && *a.toLandfill > 0) {
            landfillFlowObserved_ = true;
            Unresolved(Phase8Capture::LandfillIntake);
        }
        if (lastGarbageFunding_ && garbage.department.funding && *lastGarbageFunding_ != *garbage.department.funding)
            fundingChanged_ = true;
        if (fundingChanged_ && lastW2eCapacity_ && a.w2eCapacityMonthly && *lastW2eCapacity_ != *a.w2eCapacityMonthly) {
            Capture(Phase8Capture::GarbageCapacityReadback);
            Capture(Phase8Capture::GarbageFundingTiming);
        }
        // Message 0x66956810 is logged when it occurs, but it is not required to
        // capture the funding -> capacity causality.
        (void)sawScalingMessage;
        (void)reason;
        lastGarbageFunding_ = garbage.department.funding;
        lastW2eCapacity_ = a.w2eCapacityMonthly;
    }

    void NoteCityShutdown(std::uint32_t serial) {
        savedCitySerial_ = serial;
        saved_ = previous_;
        if (!landfillFlowObserved_)
            statuses_[static_cast<std::size_t>(Phase8Capture::LandfillIntake)] = Phase8CaptureStatus::NotCaptured;
    }

    void ObserveDeals(const std::vector<ExistingDealRow>& deals, const DealState& dealState, const char* reason, std::uint32_t citySerial = 0) {
        const auto now = Index(deals);
        const bool accountingChanged = AccountingChanged(dealState);
        const bool dealEvent = reason && std::string(reason) == "NeighborDealEvent";
        // The initial list only establishes a baseline. It is not evidence that this recorder
        // saw a create transaction before it attached.
        if (!hasBaseline_) {
            if (reason && std::string(reason) == "PostCityInit" && savedCitySerial_ == citySerial) ComparePersistence(now, HasDuplicateActive(deals));
            previous_ = now;
            hasBaseline_ = true;
            if (!dealEvent) return;
        }
        for (const auto& [key, current] : now) {
            const auto old = previous_.find(key);
            if (old == previous_.end() && dealEvent) Capture(Phase8Capture::DealCreate);
            else if (old != previous_.end() && dealEvent && old->second.amount != current.amount) {
                pendingAmountChanges_[key] = current;
                if (accountingChanged) {
                    Capture(Phase8Capture::DealChange);
                    pendingAmountChanges_.erase(key);
                }
            }
            if (old != previous_.end() && old->second.flags && current.flags && *old->second.flags != *current.flags) {
                if ((*old->second.flags & 0x100) != 0 && (*current.flags & 0x100) == 0) Capture(Phase8Capture::DealRenewal);
                if ((*current.flags & 0x100) != 0 || (*current.flags & 0x200) != 0) Capture(Phase8Capture::DealSuspension);
            }
        }
        for (const auto& [key, old] : previous_) if (!now.contains(key) && dealEvent && accountingChanged) Capture(Phase8Capture::DealCancel);
        if (accountingChanged) {
            for (auto it = pendingAmountChanges_.begin(); it != pendingAmountChanges_.end();) {
                const auto current = now.find(it->first);
                if (current != now.end() && current->second.amount == it->second.amount) {
                    Capture(Phase8Capture::DealChange);
                    it = pendingAmountChanges_.erase(it);
                } else ++it;
            }
        }
        if (reason && std::string(reason) == "PostCityInit" && savedCitySerial_ == citySerial) ComparePersistence(now, HasDuplicateActive(deals));
        previous_ = now;
    }
    void ObserveDealUiDisabled(const Opt<bool>& disabled) {
        if (!disabled) return;
        if (previousUiDisabled_ && *previousUiDisabled_ && !*disabled) Capture(Phase8Capture::DealCloseRefresh);
        if (previousUiDisabled_ && !*previousUiDisabled_ && *disabled) Capture(Phase8Capture::DealReopen);
        previousUiDisabled_ = disabled;
    }
    const std::vector<DealPersistenceObservation>& persistence() const { return persistence_; }

private:
    struct SeenDeal {
        Opt<std::int32_t> amount, flags;
        Opt<std::int64_t> cost;
        Opt<DateStamp> start, expiration;
        bool operator==(const SeenDeal&) const = default;
    };
    static std::map<DealSemanticKey, SeenDeal> Index(const std::vector<ExistingDealRow>& deals) {
        std::map<DealSemanticKey, SeenDeal> index;
        for (const auto& deal : deals) {
            const auto key = ActiveDealSemanticKey(deal);
            if (!key || index.contains(*key)) continue; // missing/ambiguous never becomes evidence
            index[*key] = {deal.amount, deal.flags, deal.cost, deal.start, deal.expiration};
        }
        return index;
    }
    bool AccountingChanged(const DealState& state) {
        const auto now = std::make_tuple(state.monthlyIncome, state.monthlyExpense);
        const bool changed = previousAccounting_ && *previousAccounting_ != now;
        previousAccounting_ = now;
        return changed;
    }
    static bool HasDuplicateActive(const std::vector<ExistingDealRow>& deals) {
        std::map<DealSemanticKey, bool> seen;
        for (const auto& deal : deals) {
            const auto key = ActiveDealSemanticKey(deal);
            if (key && !seen.emplace(*key, true).second) return true;
        }
        return false;
    }
    void ComparePersistence(const std::map<DealSemanticKey, SeenDeal>& now, bool ambiguous) {
        persistence_.clear();
        if (ambiguous) {
            persistence_.push_back({DealPersistenceObservation::Result::Ambiguous, {}});
            return;
        }
        for (const auto& [key, old] : saved_) {
            const auto current = now.find(key);
            persistence_.push_back({current == now.end() ? DealPersistenceObservation::Result::MissingAfterReload :
                (current->second == old ? DealPersistenceObservation::Result::PersistedExact : DealPersistenceObservation::Result::PersistedChanged), key});
        }
        for (const auto& [key, current] : now) if (!saved_.contains(key)) persistence_.push_back({DealPersistenceObservation::Result::NewAfterReload, key});
        if (!persistence_.empty() && std::all_of(persistence_.begin(), persistence_.end(), [](const auto& item) { return item.result == DealPersistenceObservation::Result::PersistedExact; }))
            Capture(Phase8Capture::DealPersistence);
    }

    bool ready_ = false;
    std::array<Phase8CaptureStatus, static_cast<std::size_t>(Phase8Capture::Count)> statuses_{};
    std::map<DealSemanticKey, SeenDeal> previous_, saved_;
    std::map<DealSemanticKey, SeenDeal> pendingAmountChanges_;
    std::optional<std::tuple<Opt<std::int64_t>, Opt<std::int64_t>>> previousAccounting_;
    std::vector<DealPersistenceObservation> persistence_;
    Opt<bool> previousUiDisabled_;
    Opt<float> lastGarbageFunding_;
    Opt<std::int32_t> lastW2eCapacity_;
    bool fundingChanged_ = false;
    bool landfillFlowObserved_ = false;
    std::uint32_t savedCitySerial_ = 0;
    bool hasBaseline_ = false;
};

// A compact suitability readout. It reports only what the immutable snapshot proves;
// an absent candidate is not treated as a reason to alter the city.
inline std::string Phase8EnvironmentSummary(const UtilitySnapshot& snapshot) {
    const bool powerConnection = std::any_of(snapshot.deals.connections.begin(), snapshot.deals.connections.end(),
        [](const ConnectionRow& c) { return c.active && c.carries == Commodity::Power; });
    const bool powerDealCandidate = std::any_of(snapshot.deals.connections.begin(), snapshot.deals.connections.end(),
        [](const ConnectionRow& c) {
            return c.active && c.carries == Commodity::Power &&
                   ((c.sellMax && *c.sellMax > 0) || (c.buyMax && *c.buyMax > 0));
        });
    const bool landfill = snapshot.garbage.aggregate.landfillTiles && *snapshot.garbage.aggregate.landfillTiles > 0;
    const bool garbageInfrastructure = !snapshot.garbage.buildings.empty() || landfill;
    const bool sanitation = snapshot.garbage.department.present;
    const bool suitable = powerConnection && powerDealCandidate && garbageInfrastructure && landfill && sanitation;
    std::string missing;
    const auto addMissing = [&](const char* name, bool present) {
        if (!present) missing += (missing.empty() ? "" : ",") + std::string(name);
    };
    addMissing("PowerConnection", powerConnection);
    addMissing("PowerDealCandidate", powerDealCandidate);
    addMissing("GarbageInfrastructure", garbageInfrastructure);
    addMissing("Landfill", landfill);
    addMissing("SanitationDepartment", sanitation);
    std::ostringstream out;
    out << "PowerConnection=" << (powerConnection ? "YES" : "NO")
        << " PowerDealCandidate=" << (powerDealCandidate ? "YES" : "NO")
        << " GarbageInfrastructure=" << (garbageInfrastructure ? "YES" : "NO")
        << " Landfill=" << (landfill ? "YES" : "NO")
        << " SanitationDepartment=" << (sanitation ? "YES" : "NO")
        << " SESSION_SUITABLE=" << (suitable ? "YES" : "PARTIAL");
    if (!suitable) out << " Missing=" << missing;
    return out.str();
}

// DLL-facing recorder. Its inputs are immutable snapshots and passive message payloads;
// it owns no game pointer and contains no game call or writer path.
class Phase8RuntimeRecorder {
public:
    void ResetCity() { tracker_.ResetCity(); lastEnvironment_.clear(); lastUiDisabled_.reset(); }
    void SetReady(bool dealMessageSubscribed, bool garbageMessageSubscribed, bool requested = true) {
        tracker_.SetReady(dealMessageSubscribed, garbageMessageSubscribed);
        if (tracker_.ready() && !readyLogged_) {
            readyLogged_ = true;
            Log(LogLevel::Info, "[Phase8Runtime] READY passive-message=0xEA418D8F,0x66956810 snapshots=daily/monthly writers=DISABLED");
        } else if (requested && !tracker_.ready() && !notReadyLogged_) {
            notReadyLogged_ = true;
            Log(LogLevel::Error, std::string("[Phase8Runtime] NOT_READY reason=") +
                (!dealMessageSubscribed ? "NeighborDealMessageSubscription" : "GarbageScalingMessageSubscription"));
        }
    }
    bool ready() const { return tracker_.ready(); }
    void NoteCityShutdown(std::uint32_t serial) {
        tracker_.NoteCityShutdown(serial);
        LogStatus();
        if (tracker_.Status(Phase8Capture::LandfillIntake) == Phase8CaptureStatus::NotCaptured)
            Log(LogLevel::Info, "[Phase8Runtime][Landfill] status=NOT_CAPTURED reason=no-landfill-flow-observed");
    }
    void SimulationEvent(const char* kind) {
        Log(LogLevel::Info, std::string("[Phase8Runtime][Event] seq=") + std::to_string(++sequence_) + " kind=" + kind);
    }
    void DealMessage(std::intptr_t data1, bool hasData1, std::intptr_t data2, bool hasData2) {
        std::ostringstream out;
        out << "[Phase8Runtime][Event] seq=" << ++sequence_ << " kind=NeighborDealMessage id=0xEA418D8F"
            << " eventStatus=? payload1=" << (hasData1 ? std::to_string(data1) : "?")
            << " payload2=" << (hasData2 ? std::to_string(data2) : "?");
        Log(LogLevel::Info, out.str());
    }
    void GarbageScalingMessage() {
        sawGarbageScalingMessage_ = true;
        Log(LogLevel::Info, "[Phase8Runtime][Event] seq=" + std::to_string(++sequence_) + " kind=GarbageScalingMessage id=0x66956810");
    }
    void Observe(const UtilitySnapshot& snapshot, const char* reason) {
        ObserveDealUiDisabled(snapshot.deals.uiDisabled, reason);
        tracker_.ObserveDeals(snapshot.deals.deals, snapshot.deals, reason, snapshot.citySerial);
        tracker_.ObserveGarbage(snapshot.garbage, sawGarbageScalingMessage_, reason);
        for (const auto& connection : snapshot.deals.connections)
            if (connection.carries == Commodity::Power && connection.sellMax && !connection.sellQuotes.empty())
                tracker_.Capture(Phase8Capture::LatentPricing);
        LogGarbage(snapshot, reason);
        LogDeals(snapshot, reason);
        const auto environment = Phase8EnvironmentSummary(snapshot);
        if (environment != lastEnvironment_) {
            lastEnvironment_ = environment;
            Log(LogLevel::Info, "[Phase8Runtime][Environment] " + environment);
        }
    }
    void ObserveDealUiDisabled(const Opt<bool>& disabled, const char* reason) {
        const auto beforeClose = tracker_.Status(Phase8Capture::DealCloseRefresh);
        const auto beforeOpen = tracker_.Status(Phase8Capture::DealReopen);
        tracker_.ObserveDealUiDisabled(disabled);
        if (disabled && (!lastUiDisabled_ || *lastUiDisabled_ != *disabled)) {
            Log(LogLevel::Info, std::string("[Phase8Runtime][DealUI] seq=") + std::to_string(++sequence_) +
                " reason=" + reason + " uiDisabled=" + (*disabled ? "true" : "false"));
        }
        lastUiDisabled_ = disabled;
        if (beforeClose != tracker_.Status(Phase8Capture::DealCloseRefresh))
            Log(LogLevel::Info, "[Phase8Runtime][DealUI] transition=CLOSE_REFRESH");
        if (beforeOpen != tracker_.Status(Phase8Capture::DealReopen))
            Log(LogLevel::Info, "[Phase8Runtime][DealUI] transition=REOPEN");
    }
    void LogStatus() const {
        for (std::size_t i = 0; i < static_cast<std::size_t>(Phase8Capture::Count); ++i) {
            const auto c = static_cast<Phase8Capture>(i);
            Log(LogLevel::Info, std::string("[Phase8Runtime][Status] ") + Phase8CaptureName(c) + '=' + Phase8CaptureStatusName(tracker_.Status(c)));
        }
        for (const auto& item : tracker_.persistence()) {
            Log(LogLevel::Info, std::string("[Phase8Runtime][Persistence] result=") + DealPersistenceResultName(item.result) +
                " type=" + std::to_string(item.key.type) + " neighbor=" + std::to_string(item.key.neighbor));
        }
    }

private:
    template <class T> static std::string Field(const Opt<T>& value) { return value ? std::to_string(*value) : "?"; }
    static std::string Date(const Opt<DateStamp>& value) {
        return value ? std::to_string(value->year) + '/' + std::to_string(value->dayOfYear) : "?";
    }
    void LogGarbage(const UtilitySnapshot& snapshot, const char* reason) {
        const auto& g = snapshot.garbage;
        const auto& a = g.aggregate;
        std::ostringstream out;
        out << "[Phase8Runtime][Garbage] seq=" << ++sequence_ << " reason=" << reason << " sim=" << snapshot.simYear << '/' << snapshot.simDayOfYear
            << " produced=" << Field(a.produced) << " imported=" << Field(a.imported) << " recycled=" << Field(a.recycled)
            << " exported=" << Field(a.exported) << " incinerated=" << Field(a.incinerated) << " toEnergy=" << Field(a.toEnergy)
            << " landfilled=" << Field(a.toLandfill) << " abducted=" << Field(a.abducted)
            << " uncollectedStart=" << Field(a.uncollectedStart) << " uncollectedEnd=" << Field(a.uncollectedEnd)
            << " uncollected=" << Field(a.uncollected)
            << " garbageSupply=" << Field(a.garbageSupply) << " sanitationFunding=" << Field(g.department.funding)
            << " sanitationMin=? sanitationMax=" << Field(g.department.maxAllowed) << " sanitationFixed=" << Field(g.department.fixedFunding)
            << " uiScaling=" << Field(a.uiScalingFactor) << " fundingEfficiency=" << Field(a.fundingEfficiency)
            << " incineratorCapacity=" << Field(a.incineratorCapacityMonthly)
            << " w2eCapacity=" << Field(a.w2eCapacityMonthly) << " landfillTiles=" << Field(a.landfillTiles)
            << " landfillUsedStorage=" << Field(a.landfillUsage) << " landfillFreeStorage=";
        if (a.landfillCapacity && a.landfillUsage && *a.landfillCapacity >= *a.landfillUsage) out << (*a.landfillCapacity - *a.landfillUsage); else out << '?';
        out << " landfillEfficiency=" << Field(a.landfillEfficiency) << " landfillIntakeCapacity=" << Field(a.landfillIntakeCapacityMonthly)
            << " landfillTransportCurve=? perLandfillAllowance=? globalAllowance=? requested=? accepted=?";
        Log(LogLevel::Info, out.str());
    }
    void LogDeals(const UtilitySnapshot& snapshot, const char* reason) {
        const auto& d = snapshot.deals;
        for (const auto& deal : d.deals) {
            const ConnectionRow* connection = nullptr;
            if (deal.otherCitySerial) {
                const auto found = std::find_if(d.connections.begin(), d.connections.end(), [&](const ConnectionRow& c) {
                    return c.otherCitySerial && *c.otherCitySerial == *deal.otherCitySerial;
                });
                if (found != d.connections.end()) connection = &*found;
            }
            std::ostringstream out;
            out << "[Phase8Runtime][Deal] seq=" << ++sequence_ << " reason=" << reason << " sim=" << snapshot.simYear << '/' << snapshot.simDayOfYear
                << " pointer=" << Field(deal.pointer) << " type=" << deal.type << " neighbor=" << Field(deal.otherCitySerial)
                << " lifecycle=" << DealLifecycleStateName(ClassifyDealLifecycle(deal)) << " flags=" << Field(deal.flags) << " amount=" << Field(deal.amount) << " lastAmount=" << Field(deal.lastAmount)
                << " incAmount=" << Field(deal.incAmount) << " incAmount2=" << Field(deal.incAmount2) << " amountUsed=" << Field(deal.amountUsed)
                << " cost=" << Field(deal.cost) << " start=" << Date(deal.start) << " current=" << Date(deal.current)
                << " expiration=" << Date(deal.expiration) << " suspension=" << Date(deal.suspension)
                << " localIncome=" << Field(d.monthlyIncome) << " localExpense=" << Field(d.monthlyExpense);
            if (CommodityOfDealType(deal.type) == Commodity::Power) {
                out << " localPowerProduced=" << (d.localPowerStats ? std::to_string(d.localPowerStats->produced) : "?")
                    << " localPowerDemand=" << (d.localPowerStats ? std::to_string(d.localPowerStats->demanded) : "?")
                    << " localPowerImported=" << (d.localPowerStats ? std::to_string(d.localPowerStats->imported) : "?")
                    << " localPowerExported=" << (d.localPowerStats ? std::to_string(d.localPowerStats->exported) : "?")
                    << " neighborPowerProduced=" << (connection && connection->nbStats ? std::to_string(connection->nbStats->produced) : "?")
                    << " neighborPowerDemand=" << (connection && connection->nbStats ? std::to_string(connection->nbStats->demanded) : "?")
                    << " neighborPowerImported=" << (connection && connection->nbStats ? std::to_string(connection->nbStats->imported) : "?")
                    << " neighborPowerExported=" << (connection && connection->nbStats ? std::to_string(connection->nbStats->exported) : "?");
            }
            Log(LogLevel::Info, out.str());
        }
        for (const auto& c : d.connections) if (c.carries == Commodity::Power) {
            std::ostringstream out;
            out << "[Phase8Runtime][LatentPricing] seq=" << ++sequence_ << " reason=" << reason << " neighbor=" << Field(c.otherCitySerial)
                << " direction=sell localRequirement=" << Field(snapshot.power.aggregate.requirement)
                << " localProduced=" << (d.localPowerStats ? std::to_string(d.localPowerStats->produced) : "?")
                << " localDemand=" << (d.localPowerStats ? std::to_string(d.localPowerStats->demanded) : "?")
                << " localImported=" << (d.localPowerStats ? std::to_string(d.localPowerStats->imported) : "?")
                << " localExported=" << (d.localPowerStats ? std::to_string(d.localPowerStats->exported) : "?")
                << " neighborProduced=" << (c.nbStats ? std::to_string(c.nbStats->produced) : "?")
                << " neighborDemand=" << (c.nbStats ? std::to_string(c.nbStats->demanded) : "?")
                << " neighborImported=" << (c.nbStats ? std::to_string(c.nbStats->imported) : "?")
                << " neighborExported=" << (c.nbStats ? std::to_string(c.nbStats->exported) : "?")
                << " neighborBudget=" << Field(c.nbBudget) << " dealMax=" << Field(c.sellMax) << " legalMinimum=1000 quotes=";
            for (std::size_t i = 0; i < c.sellQuotes.size(); ++i) out << (i ? "," : "") << c.sellQuotes[i].first << ':' << c.sellQuotes[i].second;
            out << " purePrediction=UNRESOLVED(missing-vanilla-cost-function) difference=?";
            Log(LogLevel::Info, out.str());
            std::ostringstream buy;
            buy << "[Phase8Runtime][LatentPricing] seq=" << ++sequence_ << " reason=" << reason << " neighbor=" << Field(c.otherCitySerial)
                << " direction=buy dealMax=" << Field(c.buyMax) << " legalMinimum=1000 quotes=";
            for (std::size_t i = 0; i < c.buyQuotes.size(); ++i) buy << (i ? "," : "") << c.buyQuotes[i].first << ':' << c.buyQuotes[i].second;
            buy << " purePrediction=UNRESOLVED(missing-vanilla-cost-function) difference=?";
            Log(LogLevel::Info, buy.str());
        }
    }
    Phase8RuntimeTracker tracker_;
    std::uint64_t sequence_ = 0;
    bool readyLogged_ = false;
    bool notReadyLogged_ = false;
    bool sawGarbageScalingMessage_ = false;
    std::string lastEnvironment_;
    Opt<bool> lastUiDisabled_;
};

} // namespace sc4af
