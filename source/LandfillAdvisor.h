// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once
// Landfill Size Savings Advisor: timing, memory and the decision glue.
// Pure: no game types, no I/O, no writes of any kind. The controller (UnifiedAuto.h) feeds it the data of a normal sanitation evaluation it already built; the plugin
// supplies two callbacks (push the notice, persist the record). Nothing here owns a thread, a timer or a second evaluation loop.
//
// Per city record (all months are simulated month indices, year * 12 + month of the year, derived with CalendarFromDayOfYear):
//   tiles = landfill tiles at the last observation, ref = month of the last observation, chg = last KNOWN change month (or the start of a protection), adv = month of the last advice.
// Observation runs on EVERY evaluation (option on or off) so the temporal rules stay intact; the economic analysis runs only once the eligibility checks below pass.
//   * first observation of a city: history unknown -> protection from now (never an invented date);
//   * tiles changed (build, removal, 0 -> n): chg = now (the 24-month protection restarts, both directions);
//   * the calendar went BACKWARDS (now < ref / chg / adv: an older save was loaded): unknown history again, chg = adv = now, no advice;
//   * the option toggling never touches the record.

#include "GarbageControl.h"
#include "LandfillSavings.h"
#include "LandfillScenario.h"
#include "LandfillSpatial.h"
#include "NativeNotice.h"
#include "NoticeMemory.h"

#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace sc4af {

constexpr int kLandfillProtectionMonths = 24;      // after the first observation / any change of the tile count
constexpr int kLandfillAdviceIntervalMonths = 12;  // after an advice (re-evaluated on the next eligible normal run, repeated only if still > 25%)

// Simulated month index = year * 12 + month of the year (0..11); -1 when the date is unknown.
inline std::int64_t LandfillSimMonth(std::uint32_t year, std::uint32_t dayOfYear) {
    if (year == 0 || dayOfYear < 1 || dayOfYear > 365) return -1;
    int d = 1;
    for (int m = 0; m < 12; ++m) {
        const int len = CalendarFromDayOfYear(d).daysInMonth;
        if (static_cast<int>(dayOfYear) < d + len) return static_cast<std::int64_t>(year) * 12 + m;
        d += len;
    }
    return -1;
}

struct LandfillAdviceRecord {
    std::int64_t tiles = 0;
    std::int64_t ref = 0;
    std::int64_t chg = 0;
    bool hasAdvice = false;
    std::int64_t adv = 0;
    bool operator==(const LandfillAdviceRecord& o) const { return tiles == o.tiles && ref == o.ref && chg == o.chg && hasAdvice == o.hasAdvice && (!hasAdvice || adv == o.adv); }
    bool operator!=(const LandfillAdviceRecord& o) const { return !(*this == o); }
};

// ---- persistence: keys of the existing notice memory (DepartmentEfficiency.told.txt), prefix "<cityKey>#L:" (never "<cityKey>#<digit>"): tiles=, ref=, chg=, adv= ----
inline std::string LandfillMemoryPrefix(const std::string& cityKey) { return cityKey.empty() ? std::string() : cityKey + "#L:"; }

inline std::vector<std::string> LandfillRecordKeys(const std::string& prefix, const LandfillAdviceRecord& r) {
    std::vector<std::string> k;
    if (prefix.empty()) return k;
    k.push_back(prefix + "tiles=" + std::to_string(r.tiles));
    k.push_back(prefix + "ref=" + std::to_string(r.ref));
    k.push_back(prefix + "chg=" + std::to_string(r.chg));
    if (r.hasAdvice) k.push_back(prefix + "adv=" + std::to_string(r.adv));
    return k;
}

// Replaces every key of the prefix by the record's. true = the memory changed (the caller saves the file). Any other key (shortage marks "<city>#<id>", board lines "<city>#<id>|", other cities) is untouched.
inline bool StoreLandfillRecord(NoticeMemory& m, const std::string& prefix, const LandfillAdviceRecord& r) {
    if (prefix.empty()) return false;
    const auto want = LandfillRecordKeys(prefix, r);
    auto have = m.KeysWithPrefix(prefix);
    bool same = have.size() == want.size();
    if (same) for (const auto& w : want) if (!m.Has(w)) { same = false; break; }
    if (same) return false;
    for (const auto& h : have) m.Erase(h);
    for (const auto& w : want) m.Add(w);
    return true;
}

// false = no complete valid record (tiles, ref and chg are required; adv optional; a negative / malformed / repeated value invalidates the whole record: the city is then treated as unknown history).
inline bool ParseLandfillRecord(const NoticeMemory& m, const std::string& prefix, LandfillAdviceRecord& out) {
    if (prefix.empty()) return false;
    bool t = false, rf = false, c = false, a = false;
    LandfillAdviceRecord r;
    for (const auto& key : m.KeysWithPrefix(prefix)) {
        const std::string rest = key.substr(prefix.size());
        const auto eq = rest.find('=');
        if (eq == std::string::npos) return false;
        const std::string name = rest.substr(0, eq), val = rest.substr(eq + 1);
        if (val.empty() || val.size() > 18) return false;
        char* end = nullptr;
        const long long v = std::strtoll(val.c_str(), &end, 10);
        if (end == nullptr || *end != '\0' || v < 0) return false;
        if (name == "tiles") { if (t) return false; t = true; r.tiles = v; }
        else if (name == "ref") { if (rf) return false; rf = true; r.ref = v; }
        else if (name == "chg") { if (c) return false; c = true; r.chg = v; }
        else if (name == "adv") { if (a) return false; a = true; r.hasAdvice = true; r.adv = v; }
        else return false;
    }
    if (!t || !rf || !c) return false;
    out = r;
    return true;
}

// ---- the temporal rules of ONE city ----
class LandfillTimeline {
public:
    bool has() const { return has_; }
    const LandfillAdviceRecord& record() const { return rec_; }
    void Clear() { has_ = false; rec_ = LandfillAdviceRecord{}; analysed_ = -1; }
    void Restore(const LandfillAdviceRecord& r) { has_ = true; rec_ = r; analysed_ = -1; }

    // Folds one observation in. true = the record changed (persist). `event` (optional) names a notable transition for the log.
    bool Observe(std::int64_t tiles, std::int64_t now, std::string* event = nullptr) {
        if (!has_) {
            rec_ = LandfillAdviceRecord{tiles, now, now, false, 0};
            has_ = true;
            if (event) *event = tiles > 0 ? "FIRST_OBSERVATION history unknown: " + std::to_string(kLandfillProtectionMonths) + "-month protection from now (tiles=" + std::to_string(tiles) + ")"
                                          : "FIRST_OBSERVATION no landfill yet";
            return true;
        }
        if (now < rec_.ref || now < rec_.chg || (rec_.hasAdvice && now < rec_.adv)) {
            rec_ = LandfillAdviceRecord{tiles, now, now, false, 0};      // history unknown: the protection starts from now (it outlasts the advice interval); no advice is invented
            if (event) *event = "CALENDAR_BACKWARDS an older save was loaded: history unknown, " + std::to_string(kLandfillProtectionMonths) + "-month protection restarts from now";
            return true;
        }
        bool changed = false;
        if (tiles != rec_.tiles) {
            if (event) *event = "TILES_CHANGED " + std::to_string(rec_.tiles) + " -> " + std::to_string(tiles) + ": " + std::to_string(kLandfillProtectionMonths) + "-month protection restarts";
            rec_.tiles = tiles;
            rec_.chg = now;
            changed = true;
        }
        if (now != rec_.ref) { rec_.ref = now; changed = true; }
        return changed;
    }
    // A zoning operation that changed the zone's structure without changing the tile count the API reports (a ghost repair): the protection restarts once.
    bool NoteStructureChange(std::int64_t now) { if (!has_ || now < 0 || rec_.chg == now) return false; rec_.chg = now; return true; }
    bool Protected(std::int64_t now) const { return !has_ || now - rec_.chg < kLandfillProtectionMonths; }
    bool InAdviceInterval(std::int64_t now) const { return has_ && rec_.hasAdvice && now - rec_.adv < kLandfillAdviceIntervalMonths; }
    void NoteAdvice(std::int64_t now) { rec_.hasAdvice = true; rec_.adv = now; }
    bool AnalysedIn(std::int64_t now) const { return analysed_ == now; }
    void MarkAnalysed(std::int64_t now) { analysed_ = now; }      // session only, never persisted

private:
    bool has_ = false;
    LandfillAdviceRecord rec_;
    std::int64_t analysed_ = -1;
};

// ---- eligibility checks: cheapest first, BEFORE any hypothetical scenario ----
enum class LandfillGate { Eligible, OptionOff, NoLandfill, NotOptimising, PlanNotComputable, NoRecord, Protected, AdviceInterval, AlreadyAnalysed, NoBudget };
inline const char* LandfillGateName(LandfillGate g) {
    switch (g) {
    case LandfillGate::Eligible: return "ELIGIBLE";
    case LandfillGate::OptionOff: return "OPTION_OFF";
    case LandfillGate::NoLandfill: return "NO_LANDFILL";
    case LandfillGate::NotOptimising: return "SANITATION_NOT_OPTIMISING";
    case LandfillGate::PlanNotComputable: return "PLAN_NOT_COMPUTABLE";
    case LandfillGate::NoRecord: return "NO_RECORD";
    case LandfillGate::Protected: return "PROTECTED_24_MONTHS";
    case LandfillGate::AdviceInterval: return "ADVICE_INTERVAL_12_MONTHS";
    case LandfillGate::AlreadyAnalysed: return "ALREADY_ANALYSED_THIS_MONTH";
    case LandfillGate::NoBudget: return "NO_BUDGET";
    }
    return "?";
}

struct LandfillCycle {
    bool option = false;              // UtilityResearchConfig::warnLandfillSizeSavings
    int thresholdPercent = kLandfillSavingThresholdPercent;   // UtilityResearchConfig::landfillSavingsAlertThreshold: only the advise / not advise decision; never the 24 / 12 month rules or the record
    bool optimising = false;          // the sanitation manager is really optimising: AutoGarbage on, the funding neither frozen nor fail-closed, the garbage readings ready, evaluated this cycle
    bool planComputable = false;      // the sanitation plan is Optimal / NoChange
    bool tilesKnown = false;
    std::int64_t tiles = 0;
    std::int64_t now = -1;            // LandfillSimMonth, -1 = unknown
    Opt<float> funding, maxAllowed;   // the department figures of the snapshot
    Opt<std::int64_t> idealMonthly;
};

inline LandfillGate EvaluateLandfillGate(const LandfillTimeline& t, const LandfillCycle& c, int budgetCap) {
    if (!c.option) return LandfillGate::OptionOff;
    if (!c.tilesKnown || c.tiles <= 0 || c.now < 0) return LandfillGate::NoLandfill;
    if (!c.optimising) return LandfillGate::NotOptimising;
    if (!c.planComputable) return LandfillGate::PlanNotComputable;
    if (!t.has()) return LandfillGate::NoRecord;
    if (t.Protected(c.now)) return LandfillGate::Protected;
    if (t.InAdviceInterval(c.now)) return LandfillGate::AdviceInterval;
    if (t.AnalysedIn(c.now)) return LandfillGate::AlreadyAnalysed;
    if (budgetCap < 1) return LandfillGate::NoBudget;
    return LandfillGate::Eligible;
}

// ---- the spatial snapshot of ONE cycle: read at most once, shared by the ghost check and the size advisor, re-read only after an operation ----
class LandfillSnapshotCache {
public:
    using ReadFn = std::function<LandfillSpatialSnapshot()>;
    explicit LandfillSnapshotCache(ReadFn read) : read_(std::move(read)) {}
    bool available() const { return static_cast<bool>(read_); }
    const LandfillSpatialSnapshot& Get() {
        if (!snap_) {
            snap_ = read_ ? read_() : LandfillSpatialSnapshot{};
            if (!read_) snap_->problem = "NO_READER";
            problem_ = ValidateLandfillSpatial(*snap_);
            ++reads_;
        }
        return *snap_;
    }
    const std::string& Problem() { Get(); return problem_; }     // "" = validated
    void Invalidate() { snap_.reset(); problem_.clear(); }
    int reads() const { return reads_; }
private:
    ReadFn read_;
    std::optional<LandfillSpatialSnapshot> snap_;
    std::string problem_;
    int reads_ = 0;
};

// The native check of a placement: PlaceZone(place = false) with the region and type the player would use (0 = accepted, else the game's error code, < 0 = the call failed).
using LandfillNativeCheckFn = std::function<int(const LandfillPlacement&)>;

inline float LandfillLinkX(const LandfillRect& r) { return (static_cast<float>(r.x1) + static_cast<float>(r.x2) + 1.0f) / 2.0f; }
inline float LandfillLinkZ(const LandfillRect& r) { return (static_cast<float>(r.z1) + static_cast<float>(r.z2) + 1.0f) / 2.0f; }

// The physically safe placements of one direction, natively checked: for each tile count n the best placement the game accepts (largest capacity after, then the generation order).
// `nativeCalls` counts the dry runs.
inline std::vector<LandfillPlacement> LandfillCheckedShortList(const std::vector<LandfillPlacement>& all, const LandfillNativeCheckFn& check, int& nativeCalls, std::vector<std::string>* log = nullptr) {
    std::map<int, std::vector<const LandfillPlacement*>> byN;
    for (const auto& p : all) if (p.refusal.empty() && p.n >= 1 && p.n <= kLandfillPlacementMaxTiles) byN[p.n].push_back(&p);
    std::vector<LandfillPlacement> out;
    for (auto& [n, v] : byN) {
        std::stable_sort(v.begin(), v.end(), [](const LandfillPlacement* a, const LandfillPlacement* b) { return a->capacityAfter > b->capacityAfter; });
        for (const auto* p : v) {
            const int err = check ? check(*p) : -1;
            ++nativeCalls;
            if (err == 0) { out.push_back(*p); break; }
            if (log && log->size() < 8) log->push_back("[LandfillAdvisor] native check refused " + LandfillPlacementLine(*p) + " error=" + std::to_string(err));
        }
    }
    return out;
}

// ---- the size advisor: observation, eligibility checks, one run, one push ----
class LandfillAdvisorEngine {
public:
    using PushFn = std::function<bool(const LandfillNotice&)>;
    using PersistFn = std::function<void(const LandfillAdviceRecord&)>;
    using BaseFn = std::function<GarbageServiceInput()>;

    PushFn push;                                                           // set by the plugin (Plugin::PushLandfillNotice); true = handed to the advisor
    PersistFn persist;                                                     // set by the plugin (told.txt); called when the record changed
    std::function<std::chrono::steady_clock::time_point()> clock;           // injectable for tests; empty = steady_clock::now
    AdaptiveBudget budget;                                                 // lives for the session: it is NOT reset with a city
    LandfillTimeline timeline;

    // Last run (diagnostics and tests).
    struct Stats {
        bool ran = false; std::string reason; int evaluations = 0; long long micros = 0; int cap = 0; bool pushed = false; bool exceeded = false;
        long long readMicros = 0, candidateMicros = 0, nativeMicros = 0, economicMicros = 0; int nativeCalls = 0, reduceCandidates = 0;
        std::optional<LandfillPlacement> placement;
    };
    const Stats& last() const { return last_; }
    int runs() const { return runs_; }

    void ResetCity() { timeline.Clear(); lastGate_ = LandfillGate::Eligible; gateLogged_ = false; }
    void Restore(const LandfillAdviceRecord& r) { timeline.Restore(r); }
    // A ghost repair changed the zone (LandfillGhostEngine): the protection restarts.
    void NoteStructureChange(std::int64_t now) { if (timeline.NoteStructureChange(now) && persist) persist(timeline.record()); }

    // `base` is called only after the eligibility checks pass (the garbage input of the plan the controller has just made); `spatial` is the cycle's snapshot (nullptr: no spatial advice) and
    // `check` the native dry run. Returns the log lines of this evaluation.
    std::vector<std::string> Evaluate(const LandfillCycle& c, const BaseFn& base, LandfillSnapshotCache* spatial = nullptr, const LandfillNativeCheckFn& check = {}) {
        std::vector<std::string> lines;
        last_ = Stats{};
        if (!c.tilesKnown || c.now < 0) return lines;      // nothing can be observed: the record is left exactly as it is
        std::string event;
        const bool hadRecord = timeline.has();
        const LandfillAdviceRecord before = timeline.record();
        const bool changed = timeline.Observe(c.tiles, c.now, &event);
        const LandfillAdviceRecord& after = timeline.record();
        // option off: only a real change of the record (first observation, tiles, protection start, advice) reaches the disk, never the monthly reference alone
        const bool significant = !hadRecord || before.tiles != after.tiles || before.chg != after.chg || before.hasAdvice != after.hasAdvice;
        if (changed && (c.option || significant) && persist) persist(after);
        if (!event.empty() && c.option) lines.push_back("[LandfillAdvisor] " + event);       // option off: observed silently, the log is exactly as before

        const LandfillGate gate = EvaluateLandfillGate(timeline, c, budget.cap);
        if (gate != LandfillGate::Eligible) {
            last_.reason = LandfillGateName(gate);
            if (c.option && (!gateLogged_ || gate != lastGate_) && gate != LandfillGate::AlreadyAnalysed) {
                lines.push_back(std::string("[LandfillAdvisor] not analysed: ") + LandfillGateName(gate));
                lastGate_ = gate;
                gateLogged_ = true;
            }
            return lines;
        }
        gateLogged_ = false;
        timeline.MarkAnalysed(c.now);
        ++runs_;
        const auto now = [&]() { return clock ? clock() : std::chrono::steady_clock::now(); };
        const auto us = [](auto a, auto b) { return static_cast<long long>(std::chrono::duration_cast<std::chrono::microseconds>(b - a).count()); };
        const auto t0 = now();
        last_.ran = true;
        last_.cap = budget.cap;

        // 1. the spatial snapshot (read once per cycle, validated against the game's totals)
        std::string problem = spatial ? spatial->Problem() : std::string("NO_READER");
        const auto t1 = now();
        last_.readMicros = us(t0, t1);
        if (problem.empty() && spatial->Get().zones.size() != 1) problem = "SEVERAL_LANDFILL_ZONES";
        if (!problem.empty()) {
            last_.reason = "SPATIAL_" + problem;
            last_.micros = us(t0, now());
            lines.push_back("[LandfillAdvisor] result=" + last_.reason + " (no spatial evidence: no advice) us=" + std::to_string(last_.micros));
            return lines;
        }
        const LandfillSpatialSnapshot& snap = spatial->Get();
        // 2. candidates (pure, exact model) and 3. the native check of the short list
        const auto rd = LandfillReduceCandidates(snap, 0);
        last_.reduceCandidates = static_cast<int>(rd.size());
        const auto t2 = now();
        last_.candidateMicros = us(t1, t2);
        std::vector<std::string> refusals;
        int calls = 0;
        const auto rdList = LandfillCheckedShortList(rd, check, calls, &refusals);
        last_.nativeCalls = calls;
        const auto t3 = now();
        last_.nativeMicros = us(t2, t3);
        for (auto& l : refusals) lines.push_back(std::move(l));
        // 4. the economic search over the existing engine: index i of a list = the i-th placement (sorted by tile count, so "fewer tiles" stays the tie-break)
        const GarbageServiceInput in = base();
        std::vector<LandfillScenario> scen;
        for (const auto& p : rdList) scen.push_back(LandfillPlacementScenario(in, c.idealMonthly, snap, p));
        LandfillSearchOptions opt;
        opt.clock = clock;
        opt.thresholdPercent = c.thresholdPercent;
        opt.ApplyAdaptive(budget);
        const auto plans0 = gGarbagePlanCalls, evals0 = gGarbageEvalCalls;
        LandfillAdvice adv = AdviseLandfillSize(in, c.funding, c.maxAllowed, c.idealMonthly, static_cast<int>(scen.size()),
                                                [&](int delta) { return scen[static_cast<size_t>(-delta - 1)]; }, opt);
        gGarbagePlanCalls = plans0;                      // the advisor's hypothetical plans are not the controller's work counters
        gGarbageEvalCalls = evals0;
        const auto t4 = now();
        last_.economicMicros = us(t3, t4);
        const long long micros = us(t0, t4);
        const bool exceeded = adv.timeBudgetExceeded || micros > opt.timeBudget.count();
        budget.Record(exceeded);
        last_.evaluations = adv.evaluations;
        last_.micros = micros;
        last_.exceeded = exceeded;
        last_.reason = adv.reason;

        std::string result = adv.reason;
        const LandfillPlacement* chosen = nullptr;
        if (adv.hasAdvice) {
            if (adv.tiles >= 1 && adv.tiles <= static_cast<int>(rdList.size())) chosen = &rdList[static_cast<size_t>(adv.tiles - 1)];
        }
        if (chosen) {
            last_.placement = *chosen;
            LandfillNotice n;
            n.kind = LandfillAdviceKind::Reduce;
            n.tiles = chosen->n; n.percent = adv.savingPercent; n.savings = adv.saving; n.across = chosen->across; n.along = chosen->along;
            n.linkX = LandfillLinkX(chosen->rect); n.linkZ = LandfillLinkZ(chosen->rect);
            result = LandfillPlacementLine(*chosen) + " percent=" + std::to_string(adv.savingPercent) + " savings=" + std::to_string(adv.saving) + " expense " +
                     std::to_string(adv.baselineExpense) + "->" + std::to_string(adv.expenseAfter) + " funding " + std::to_string(adv.baselinePercent) + "->" + std::to_string(adv.percentAfter);
            if (!LandfillNoticeValid(n)) result += " NOT_PUSHED(values not valid)";
            else if (!push) result += " NOT_PUSHED(no notice channel)";
            else if (!push(n)) result += " NOT_PUSHED(advisor refused)";
            else {
                last_.pushed = true;
                timeline.NoteAdvice(c.now);
                if (persist) persist(timeline.record());
                result += " PUSHED";
            }
        } else if (adv.hasAdvice) {
            result = "INTERNAL_PLACEMENT_INDEX";
        } else {
            // the best candidate, for the log (what was weighed and why nothing is advised)
            const LandfillCandidate* b = nullptr;
            for (const auto& cd : adv.candidates)
                if (cd.eligible && (!b || cd.saving > b->saving)) b = &cd;
            if (b) {
                const auto& pl = rdList[static_cast<size_t>(b->tiles - 1)];
                lines.push_back(std::string("[LandfillAdvisor] best REDUCE ") + LandfillPlacementLine(pl) + " saving=" + std::to_string(b->saving) + " (" +
                                std::to_string(b->savingPercent) + "% of " + std::to_string(adv.baselineExpense) + ") threshold=" + std::to_string(c.thresholdPercent) + "%");
            }
        }
        lines.push_back("[LandfillAdvisor] result=" + result + " evaluations=" + std::to_string(adv.evaluations) + " candidates reduce=" + std::to_string(last_.reduceCandidates) + "/" + std::to_string(rdList.size()) + " nativeChecks=" + std::to_string(calls) +
                        " us read=" + std::to_string(snap.readMicros) + " candidates=" + std::to_string(last_.candidateMicros) + " native=" + std::to_string(last_.nativeMicros) +
                        " economic=" + std::to_string(last_.economicMicros) + " total=" + std::to_string(micros) + " cap=" + std::to_string(last_.cap) +
                        (exceeded ? " TIME_BUDGET_EXCEEDED(next cap " + std::to_string(budget.cap) + ")" : std::string()) + (adv.interrupted ? " PARTIAL_SEARCH" : std::string()));
        return lines;
    }

private:
    Stats last_;
    int runs_ = 0;
    LandfillGate lastGate_ = LandfillGate::Eligible;
    bool gateLogged_ = false;
};

// ---- ghost squares: detection and the automatic repair under proven conditions ----
// Signature of a zone's ghost state: the same signature is never handled twice (persisted in told.txt as "<city>#G:<signature>").
inline std::string LandfillGhostSignature(const LandfillZoneView& z) {
    std::uint64_t h = 1469598103934665603ULL;
    for (const auto& [x, y] : z.ghosts) { h ^= static_cast<std::uint64_t>(x) * 4099u + static_cast<std::uint64_t>(y); h *= 1099511628211ULL; }
    const auto& m = z.model;
    return std::to_string(m.x1) + "_" + std::to_string(m.y1) + "_" + std::to_string(m.x2) + "_" + std::to_string(m.y2) + "_g" + std::to_string(z.ghosts.size()) + "_t" +
           std::to_string(z.landfillTiles.size()) + "_" + std::to_string(h % 1000000007ULL);
}

// Ghost squares are a game bug, not an economic choice: whenever the Department manages sanitation (the normal sanitation evaluation runs), a proven ghost state is
// repaired through the native re-zoning. No option, no savings threshold, no profitability test, no notice: the log says what was found, done and saved.
//   detect  -> the validated spatial view (LandfillSnapshotCache), zone by zone, once per simulated month;
//   safety  -> LandfillRepairPlacement (no new square, capacity does not fall, nothing to the valve, no other zone merged, exact rounding) and the game's own dry run of the region;
//   repair  -> PlaceZone(place = true) of the same region (pile-free landfill squares re-zoned as landfill: no zone type changes, nothing demolished);
//   confirm -> PlaceZone only POSTS the zone message: the game reconciles the zone when it handles it, after the call returns. The confirmation is therefore an
//              independent re-read at the NEXT evaluations (at most kGhostVerifyCycles of them): same box, no ghost left, T / N / C / capacity as predicted, landfill square count
//              unchanged, the view consistent with the game's totals (every stock within its cell). The stored garbage moves daily: logged, not compared. A difference disables the repair.
// A state (signature) is attempted once per SESSION: never again, repaired or refused, until the zone changes. The game restores the ghost squares when the city is loaded again
// (a city saved after a verified repair reopens with the same ghosts), so the repair is redone, and verified again, in every session.
constexpr int kGhostVerifyCycles = 4;

struct LandfillGhostCycle {
    bool optimising = false;          // the sanitation manager runs (Department active, AutoGarbage on, not frozen, plan computable)
    std::int64_t now = -1;            // simulated month (one check per month)
};

class LandfillGhostEngine {
public:
    using RepairFn = std::function<bool(const LandfillPlacement&)>;        // PlaceZone(place = true) of the region as landfill; true = the call returned success
    using RememberFn = std::function<void(const std::string& signature)>;  // persist a handled signature

    RepairFn repair;
    RememberFn remember;                                                    // optional observer (tests); the plugin keeps the states for the session only
    std::set<std::string> handled;                                          // the states handled in this session of the city

    struct Outcome { bool repaired = false; bool mismatch = false; bool pending = false; int found = 0; std::int64_t maintenanceSaved = 0; };
    const Outcome& last() const { return last_; }
    bool disabled() const { return disabled_; }
    bool verifying() const { return pending_.has_value(); }
    void ResetCity() { handled.clear(); analysed_ = -1; logged_.clear(); pending_.reset(); }

    std::vector<std::string> Evaluate(const LandfillGhostCycle& c, LandfillSnapshotCache& cache, const LandfillNativeCheckFn& check) {
        std::vector<std::string> lines;
        last_ = Outcome{};
        if (pending_ && cache.available()) { Verify(c, cache, lines); return lines; }
        if (!c.optimising || c.now < 0 || analysed_ == c.now || !cache.available()) return lines;
        analysed_ = c.now;
        const std::string problem = cache.Problem();
        if (!problem.empty()) { Once(lines, "[LandfillGhosts] not checked: spatial view " + problem); return lines; }
        const LandfillSpatialSnapshot& s = cache.Get();
        for (size_t i = 0; i < s.zones.size(); ++i) {
            const auto& z = s.zones[i];
            if (z.ghosts.empty()) continue;
            ++last_.found;
            const std::string sig = LandfillGhostSignature(z);
            if (handled.count(sig)) continue;
            std::string list;
            for (const auto& [x, y] : z.ghosts) { if (list.size() > 120) { list += " ..."; break; } list += " " + std::to_string(x) + "," + std::to_string(y); }
            lines.push_back("[LandfillGhosts] zone " + std::to_string(i) + ": " + std::to_string(z.ghosts.size()) + " ghost square(s) (zone type no longer landfill, still paid " +
                            std::to_string(kLandfillEmptyTileCost * static_cast<std::int64_t>(z.ghosts.size())) + "/month):" + list);
            const auto p = LandfillRepairPlacement(s, static_cast<int>(i));
            std::string why;
            if (disabled_) why = "DISABLED_AFTER_A_MISMATCH";
            else if (!p) why = "NO_GHOST";
            else if (!p->refusal.empty()) why = p->refusal;
            if (p) lines.push_back("[LandfillGhosts] repair plan " + LandfillPlacementLine(*p) + " region=" + std::to_string(p->tiles.size()) + " squares");
            if (why.empty()) {
                const int err = check ? check(*p) : -1;
                if (err != 0) why = "NATIVE_DRY_RUN_REFUSED_" + std::to_string(err);
            }
            if (!why.empty()) {
                lines.push_back("[LandfillGhosts] NOT repaired: " + why + " (the zone is left as it is; this state is not tried again)");
                Handle(sig);
                continue;
            }
            const bool ok = repair && repair(*p);
            Handle(sig);                                        // success or not, this state is never attempted again
            if (!ok) {
                disabled_ = true;
                last_.mismatch = true;
                lines.push_back("[LandfillGhosts] REPAIR_FAILED the native call did not succeed: automatic repair disabled for this session");
                return lines;
            }
            Pending pd;
            pd.p = *p;
            pd.x1 = z.model.x1; pd.y1 = z.model.y1; pd.x2 = z.model.x2; pd.y2 = z.model.y2;
            pd.month = c.now;
            pd.apiTiles = s.apiTiles; pd.apiCapacity = s.apiCapacity; pd.apiUsage = s.apiUsage; pd.apiMaintenance = s.apiMaintenance;
            pending_ = pd;
            last_.pending = true;
            cache.Invalidate();                                 // nothing else may use the view of before in this cycle
            lines.push_back("[LandfillGhosts] re-zoning done; the game reconciles the zone when it handles its message: verified at the next evaluation");
            return lines;                                       // one repair per cycle; another zone waits for the next month
        }
        return lines;
    }

private:
    struct Pending {
        LandfillPlacement p;
        int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
        std::int64_t month = -1, apiTiles = 0, apiCapacity = 0, apiUsage = 0, apiMaintenance = 0;
        int waits = 0;
    };
    Outcome last_;
    bool disabled_ = false;
    std::int64_t analysed_ = -1;
    std::set<std::string> logged_;
    std::optional<Pending> pending_;
    void Handle(const std::string& sig) { if (handled.insert(sig).second && remember) remember(sig); }
    void Once(std::vector<std::string>& lines, const std::string& l) { if (logged_.insert(l).second) lines.push_back(l); }

    void Verify(const LandfillGhostCycle& c, LandfillSnapshotCache& cache, std::vector<std::string>& lines) {
        auto& pd = *pending_;
        ++pd.waits;
        const std::string problem = cache.Problem();
        std::string mismatch;
        const LandfillZoneView* z2 = nullptr;
        if (problem.empty()) {
            const auto& s2 = cache.Get();
            for (const auto& v : s2.zones) if (v.model.x1 == pd.x1 && v.model.y1 == pd.y1 && v.model.x2 == pd.x2 && v.model.y2 == pd.y2) z2 = &v;
            if (!z2) mismatch = "ZONE_NOT_FOUND";
            else if (!z2->ghosts.empty()) {
                if (pd.waits < kGhostVerifyCycles) { last_.pending = true; return; }   // the message is not handled yet: look again at the next evaluation
                mismatch = "GHOSTS_LEFT";
            } else if (z2->model.T() != pd.p.tilesAfter || z2->model.N() != pd.p.cellsAfter || z2->model.CellCapacity() != pd.p.cellCapAfter || z2->model.Capacity() != pd.p.capacityAfter) {
                mismatch = "ZONE_DIFFERS";
            } else if (s2.apiTiles != pd.apiTiles || s2.apiCapacity != pd.apiCapacity - pd.p.capacityBefore + pd.p.capacityAfter) {
                mismatch = "TOTALS_DIFFER";
            }
            // the stored garbage moves every day (intake), so it is only logged: nothing went to the valve because the cells are exactly the predicted ones (N, C, capacity) and every
            // stock is within its cell's capacity (ValidateLandfillSpatial)
            if (mismatch.empty()) {
                last_.repaired = true;
                last_.maintenanceSaved = pd.apiMaintenance - s2.apiMaintenance;
                lines.push_back("[LandfillGhosts] REPAIRED and verified: " + std::to_string(pd.p.ghostsReconciled) + " ghost square(s) removed, T " + std::to_string(pd.p.tilesBefore) + " -> " +
                                std::to_string(z2->model.T()) + ", N " + std::to_string(z2->model.N()) + ", cell capacity " + std::to_string(z2->model.CellCapacity()) + ", capacity " +
                                std::to_string(pd.apiCapacity) + " -> " + std::to_string(s2.apiCapacity) + ", stored garbage " + std::to_string(pd.apiUsage) + " -> " + std::to_string(s2.apiUsage) +
                                " (daily intake)" + ", landfill squares " + std::to_string(s2.apiTiles) + " unchanged, maintenance " +
                                std::to_string(pd.apiMaintenance) + " -> " + std::to_string(s2.apiMaintenance) + " (" + std::to_string(pd.p.maintenanceDelta) + " predicted at the operation)");
            }
        } else {
            if (pd.waits < kGhostVerifyCycles) { last_.pending = true; return; }
            mismatch = "SPATIAL_" + problem;
        }
        pending_.reset();
        if (!mismatch.empty()) {
            disabled_ = true;
            last_.mismatch = true;
            lines.push_back("[LandfillGhosts] REPAIR_MISMATCH " + mismatch + ": automatic repair disabled for this session");
        }
    }
};

}  // namespace sc4af
