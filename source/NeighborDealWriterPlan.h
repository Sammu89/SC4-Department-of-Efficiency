// SPDX-License-Identifier: GPL-3.0-only
// Copyright (c) 2026 Sammu89
#pragma once

// Pure, game-free transaction layer for Neighbor Deals: CREATE, CHANGE_AMOUNT and CANCEL as the vanilla window performs
// them. Nothing here touches the game; the game-facing executor lives in NeighborDealWriter.cpp.
//
// The machine is driven by SNAPSHOTS, not by time. The host loop is
//
//     step = machine.Next(freshObservation);        // decides ONE primitive, or reports a state
//     if (step.action != None) { ok = executeOnePrimitive(step.action); machine.Executed(ok); }
//     ...later, on every snapshot/event:  machine.Next(freshObservation)  -> PENDING_READBACK ... -> APPLIED
//
// Rules that keep it faithful to the vanilla mechanism:
//  * The deal is identified semantically by (type, neighbor serial). Amount is mutable and never part of the identity; a
//    pointer is never kept. Two rows with the same key => TARGET_AMBIGUOUS and no write.
//  * Success is never inferred from a primitive's return value. APPLIED needs authoritative evidence: the row's flags and
//    amount, a refreshed stored cost, and RegionalCity import/export plus the simulator money totals reconciled with what
//    the vanilla accounting rules (DealAccounting.h) say the deal list must produce. Those update LATER than SetAmount
//    (message 0xEA418D8F is posted), so "not yet" is PENDING_READBACK, never a failure.
//  * No wall clock, no sleep, no retry counter: the machine only changes state on new evidence. A city reload (lifecycle
//    epoch change) makes the whole transaction obsolete (PRECONDITION_CHANGED).

#include "DealAccounting.h"
#include "DealQuantity.h"
#include "UtilityModel.h"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace sc4af {

enum class DealWriteOp { Create, ChangeAmount, Cancel };

inline const char* DealWriteOpName(DealWriteOp o) {
    switch (o) {
    case DealWriteOp::Create: return "CREATE";
    case DealWriteOp::ChangeAmount: return "CHANGE_AMOUNT";
    case DealWriteOp::Cancel: return "CANCEL";
    }
    return "?";
}

enum class DealWriteState {
    Applied,
    PendingReadback,
    AlreadyInState,
    PreconditionChanged,
    TargetMissing,
    TargetAmbiguous,
    TargetSuspended,
    QuantityNotLegal,
    VanillaRejected,
    VerifyFailed,
    Unsupported,
    InvalidSnapshot,
    InProgress,       // a primitive has been issued and the machine waits for the host to execute it
};

inline const char* DealWriteStateName(DealWriteState s) {
    switch (s) {
    case DealWriteState::Applied: return "APPLIED";
    case DealWriteState::PendingReadback: return "PENDING_READBACK";
    case DealWriteState::AlreadyInState: return "ALREADY_IN_STATE";
    case DealWriteState::PreconditionChanged: return "PRECONDITION_CHANGED";
    case DealWriteState::TargetMissing: return "TARGET_MISSING";
    case DealWriteState::TargetAmbiguous: return "TARGET_AMBIGUOUS";
    case DealWriteState::TargetSuspended: return "TARGET_SUSPENDED";
    case DealWriteState::QuantityNotLegal: return "QUANTITY_NOT_LEGAL";
    case DealWriteState::VanillaRejected: return "VANILLA_REJECTED";
    case DealWriteState::VerifyFailed: return "VERIFY_FAILED";
    case DealWriteState::Unsupported: return "UNSUPPORTED";
    case DealWriteState::InvalidSnapshot: return "INVALID_SNAPSHOT";
    case DealWriteState::InProgress: return "IN_PROGRESS";
    }
    return "?";
}

inline bool IsTerminal(DealWriteState s) {
    return s != DealWriteState::PendingReadback && s != DealWriteState::InProgress;
}

struct DealKey {
    std::int32_t type = 0;
    std::uint32_t neighbor = 0;
    bool operator==(const DealKey& o) const { return type == o.type && neighbor == o.neighbor; }
};

// The primitive vanilla calls, one at a time. The executor performs EXACTLY the named call.
enum class DealPrimitive {
    None,
    ProposeDeals,    // NeighborsSimulator::ProposeDeals(list, true): what the window does when it opens
    Activate,        // cISC4NeighborDeal::Activate()
    SetAmount,       // cISC4NeighborDeal::SetAmount(amount)  (UpdateCost happens inside; message 0xEA418D8F follows)
    Cancel,          // cISC4NeighborDeal::Cancel()
};

inline const char* DealPrimitiveName(DealPrimitive p) {
    switch (p) {
    case DealPrimitive::None: return "NONE";
    case DealPrimitive::ProposeDeals: return "ProposeDeals(list,true)";
    case DealPrimitive::Activate: return "Activate()";
    case DealPrimitive::SetAmount: return "SetAmount(q)";
    case DealPrimitive::Cancel: return "Cancel()";
    }
    return "?";
}

// ---- what the planner hands to the writer: expected state, target, reason, expected delta, required evidence ----
struct RequiredEvidence {
    bool activeFlag = true;               // flags has 0x02 and none of 0x04/0x38/0x300 (create/change)
    bool amountEquals = true;
    bool storedCostRefreshed = true;      // cost >= 1 and moved when the amount moved
    bool regionalFlowReconciled = true;   // RegionalCity Imported/Exported(commodity) == reconstruction from the deal list
    bool moneyReconciled = true;          // simulator monthly income/expense == reconstruction from the deal list
};

struct DealWritePlan {
    DealWriteOp op = DealWriteOp::ChangeAmount;
    DealKey key;
    std::int32_t expectedAmount = 0;      // semantic state the plan was made against (0 for CREATE: no live contract)
    std::int32_t targetAmount = 0;        // 0 for CANCEL
    std::int64_t expectedMonthlyDelta = 0;   // net benefit the optimizer attributed to this action (informational; 0 when it had no monetary marginal: required / restores feasibility)
    std::int32_t ladderStep = 0;          // canonical ladder the planner chose the target from (Power/Water SELL): 0 = only the vanilla ladders of the live row are legal
    std::int32_t ladderMax = 0;
    std::string reason;
    RequiredEvidence evidence;
};

struct DealWriteRequest {
    DealWriteOp op = DealWriteOp::ChangeAmount;
    DealKey key;
    std::uint32_t lifecycleEpoch = 0;
    std::int32_t expectedAmount = 0;
    std::int32_t targetAmount = 0;
    std::int32_t ladderStep = 0, ladderMax = 0;
    std::string reason;
};

inline DealWriteRequest MakeRequest(const DealWritePlan& plan, std::uint32_t lifecycleEpoch) {
    DealWriteRequest r;
    r.op = plan.op;
    r.key = plan.key;
    r.lifecycleEpoch = lifecycleEpoch;
    r.expectedAmount = plan.expectedAmount;
    r.targetAmount = plan.targetAmount;
    r.ladderStep = plan.ladderStep;
    r.ladderMax = plan.ladderMax;
    r.reason = plan.reason;
    return r;
}

// ---- one fresh, semantic reading of everything the machine needs (no pointers) --------------------------------------------
struct DealObservation {
    bool valid = false;                              // false: the read layer could not produce a trustworthy reading
    std::uint32_t lifecycleEpoch = 0;
    std::vector<ExistingDealRow> deals;              // RegionalCity::GetLocalDeals, semantic rows
    Opt<std::int64_t> regionalImported[3];           // RegionalCity::GetImported(commodity) truncated; index = power, water, garbage
    Opt<std::int64_t> regionalExported[3];
    Opt<std::int64_t> monthlyIncome, monthlyExpense; // NeighborsSimulator::GetTotalMonthlyNeighborDealIncome/Expense
    Opt<std::int64_t> maxAmountTruncated;            // trunc(GetDealMaxAmount(key.type, neighbor)), read after the last write
    Opt<std::int32_t> minAmount;                     // tuned minimum for the key's commodity
};

inline int CommodityIndex(std::int32_t dealType) {
    const auto c = CommodityOfDealType(dealType);
    return c == Commodity::Power ? 0 : c == Commodity::Water ? 1 : c == Commodity::Garbage ? 2 : -1;
}

inline bool RowMatches(const ExistingDealRow& r, const DealKey& k) {
    return r.type == k.type && r.otherCitySerial && *r.otherCitySerial == k.neighbor;
}

inline std::vector<const ExistingDealRow*> RowsForKey(const DealObservation& o, const DealKey& k) {
    std::vector<const ExistingDealRow*> out;
    for (const auto& r : o.deals)
        if (RowMatches(r, k)) out.push_back(&r);
    return out;
}

// Reconciliation of the delayed accounting with the deal list the same observation shows.
struct AccountingReconciliation {
    bool flowReady = false, flowReconciled = false;
    bool moneyReady = false, moneyReconciled = false;
    std::string detail;
};

inline AccountingReconciliation ReconcileAccounting(const DealObservation& o, const DealKey& k) {
    AccountingReconciliation r;
    const int ci = CommodityIndex(k.type);
    if (ci < 0) return r;
    const auto rec = ReconstructAccounting(o.deals);
    const auto& imported = o.regionalImported[ci];
    const auto& exported = o.regionalExported[ci];
    r.flowReady = imported && exported;
    r.flowReconciled = r.flowReady && *imported == rec.imported[ci] && *exported == rec.exported[ci];
    r.moneyReady = o.monthlyIncome && o.monthlyExpense;
    r.moneyReconciled = r.moneyReady && *o.monthlyIncome == rec.income && *o.monthlyExpense == rec.expense;
    if (!r.flowReconciled || !r.moneyReconciled) {
        r.detail = "regional imported/exported " + (imported ? std::to_string(*imported) : std::string("?")) + "/" +
                   (exported ? std::to_string(*exported) : std::string("?")) + " vs list " + std::to_string(rec.imported[ci]) + "/" +
                   std::to_string(rec.exported[ci]) + "; money income/expense " + (o.monthlyIncome ? std::to_string(*o.monthlyIncome) : std::string("?")) +
                   "/" + (o.monthlyExpense ? std::to_string(*o.monthlyExpense) : std::string("?")) + " vs list " + std::to_string(rec.income) + "/" +
                   std::to_string(rec.expense);
    }
    return r;
}

struct DealStep {
    DealPrimitive primitive = DealPrimitive::None;
    std::int32_t amount = 0;           // for SetAmount
    DealWriteState state = DealWriteState::InvalidSnapshot;
    std::string reason;
};

class DealWriteMachine {
public:
    explicit DealWriteMachine(DealWriteRequest request) : req_(std::move(request)) {}

    const DealWriteRequest& request() const { return req_; }
    DealWriteState state() const { return state_; }
    bool terminal() const { return IsTerminal(state_); }
    const std::string& reason() const { return reason_; }
    // the quantity actually requested (a new garbage import may have been lowered to the largest legal one); snappedFrom() > 0 says it was
    std::int32_t effectiveTarget() const { return req_.targetAmount; }
    std::int32_t snappedFrom() const { return snappedFrom_; }
    std::size_t primitivesIssued() const { return issued_; }
    bool started() const { return issued_ > 0; }
    // Every primitive this transaction will ever issue has been executed: Next() can only observe from here (the verification phase never issues one).
    // The host may keep pumping such a machine with a deal manager that has been switched off; any other machine could still create, change or cancel a contract.
    bool readbackOnly() const { return phase_ == Phase::Verify && awaiting_ == DealPrimitive::None; }

    // The host reports that the primitive returned by the last Next() was executed. `ok` is the primitive's own boolean
    // (Activate), true for primitives that return nothing. A false Activate is the vanilla refusal.
    void Executed(bool ok) {
        if (awaiting_ == DealPrimitive::None) return;
        const auto p = awaiting_;
        awaiting_ = DealPrimitive::None;
        state_ = DealWriteState::PendingReadback;
        if (p == DealPrimitive::Activate && !ok) { Finish(DealWriteState::VanillaRejected, "Activate() returned false (proposal not active-able: invalid flag set or not pending)"); return; }
        if (p == DealPrimitive::ProposeDeals) phase_ = Phase::ProposalIssued;
        else if (p == DealPrimitive::Activate) phase_ = Phase::Activated;
        else if (p == DealPrimitive::SetAmount || p == DealPrimitive::Cancel) phase_ = Phase::Verify;
    }

    // A technical fault in the executor (a guarded primitive raised): the transaction is over and nothing is assumed.
    void Abort(std::string why) {
        awaiting_ = DealPrimitive::None;
        state_ = DealWriteState::VerifyFailed;
        reason_ = std::move(why);
    }

    DealStep Next(const DealObservation& obs) {
        if (terminal()) return Report();
        if (awaiting_ != DealPrimitive::None) return Out(DealPrimitive::None, 0, DealWriteState::InProgress, "primitive issued but not reported as executed");
        if (!obs.valid) return Out(DealPrimitive::None, 0, issued_ > 0 ? DealWriteState::PendingReadback : DealWriteState::InvalidSnapshot,
                                   "observation is not valid (nothing assumed)");
        if (obs.lifecycleEpoch != req_.lifecycleEpoch) return Finish(DealWriteState::PreconditionChanged, "city lifecycle changed");
        if (CommodityIndex(req_.key.type) < 0) return Finish(DealWriteState::Unsupported, "unknown deal type");

        switch (phase_) {
        case Phase::Start: return Start(obs);
        case Phase::ProposalIssued: return AfterProposal(obs);
        case Phase::Activated: return AfterActivate(obs);
        case Phase::Verify: return Verify(obs);
        }
        return Finish(DealWriteState::Unsupported, "unreachable");
    }

private:
    enum class Phase { Start, ProposalIssued, Activated, Verify };

    struct Baseline {
        std::int32_t amount = 0;
        std::int64_t cost = 0;
        bool captured = false;
    };

    DealStep Report() const { return Out(DealPrimitive::None, 0, state_, reason_); }
    static DealStep Out(DealPrimitive p, std::int32_t amount, DealWriteState s, std::string why) {
        DealStep d;
        d.primitive = p;
        d.amount = amount;
        d.state = s;
        d.reason = std::move(why);
        return d;
    }
    DealStep Finish(DealWriteState s, std::string why) {
        state_ = s;
        reason_ = std::move(why);
        awaiting_ = DealPrimitive::None;
        return Out(DealPrimitive::None, 0, s, reason_);
    }
    DealStep Issue(DealPrimitive p, std::int32_t amount, std::string why) {
        awaiting_ = p;
        state_ = DealWriteState::InProgress;
        reason_ = std::move(why);
        ++issued_;
        return Out(p, amount, DealWriteState::InProgress, reason_);
    }
    DealStep Pending(std::string why) {
        state_ = DealWriteState::PendingReadback;
        reason_ = std::move(why);
        return Out(DealPrimitive::None, 0, state_, reason_);
    }

    static bool IsActiveRow(const ExistingDealRow& r) { return r.flags && (*r.flags & deal_flag::kActive); }
    static bool IsPendingProposal(const ExistingDealRow& r) {
        // PENDING (0x01), or CANCELLED+PENDING (0x05, the window re-flags a cancelled object as a proposal and
        // Activate() clears 0x04 again), with none of the invalid reasons and not suspended.
        if (!r.flags) return false;
        const auto f = *r.flags;
        return (f & deal_flag::kPending) && !(f & deal_flag::kActive) && !(f & deal_flag::kInvalidMask) && !(f & deal_flag::kSuspensionMask);
    }

    DealQuantityInput QuantityInputFor(const DealObservation& obs, const ExistingDealRow* row, bool active) const {
        DealQuantityInput in;
        in.maxAmountTruncated = obs.maxAmountTruncated ? *obs.maxAmountTruncated : 0;
        in.minAmount = obs.minAmount ? *obs.minAmount : 0;
        in.active = active;
        if (row) {
            in.amount = row->amount ? *row->amount : 0;
            in.incAmount = row->incAmount ? *row->incAmount : 0;
            in.incAmount2 = row->incAmount2 ? *row->incAmount2 : 0;
            in.suspended = row->flags && (*row->flags & deal_flag::kSuspensionMask);
        }
        return in;
    }

    // Legal = an entry of the vanilla ladder the window would show, OR an entry of the canonical proposal ladder the planner chose it from
    // (multiples of ladderStep up to ladderMax), never above what GetDealMaxAmount allows right now (nor above the current amount, for decreases).
    bool LegalTarget(const DealQuantityGrid& grid, const DealObservation& obs, std::int64_t currentAmount) const {
        if (IsLegalDealQuantity(grid, req_.targetAmount)) return true;
        if (req_.ladderStep <= 0 || req_.targetAmount <= 0 || req_.targetAmount % req_.ladderStep != 0 || req_.targetAmount > req_.ladderMax) return false;
        const std::int64_t ceiling = std::max<std::int64_t>(obs.maxAmountTruncated ? *obs.maxAmountTruncated : 0, currentAmount);
        return req_.targetAmount <= ceiling;
    }

    // A new garbage IMPORT only adds foreign load: a smaller quantity is always safe for the own city. The planned amount can sit above what
    // GetDealMaxAmount allows when the contract is written (the planner judges on the base books, the game on the live ones). Instead of giving up
    // with QUANTITY_NOT_LEGAL, the largest legal quantity not above the planned one is used. Buy-like deals (the own supply depends on them) are never reduced here.
    void SnapImportDown(const DealObservation& obs, const ExistingDealRow* row) {
        if (req_.op != DealWriteOp::Create || req_.key.type != utility_id::kDealImportGarbage || !QuantityInputsKnown(obs)) return;
        const auto proposal = EnumerateDealQuantities(QuantityInputFor(obs, row, false));
        if (LegalTarget(proposal, obs, 0)) return;
        std::vector<std::int64_t> legal(proposal.amounts.begin(), proposal.amounts.end());
        if (row) { const auto active = EnumerateDealQuantities(QuantityInputFor(obs, row, true)); legal.insert(legal.end(), active.amounts.begin(), active.amounts.end()); }
        if (req_.ladderStep > 0) {
            const std::int64_t ceiling = obs.maxAmountTruncated ? *obs.maxAmountTruncated : 0;
            for (std::int64_t q = req_.ladderStep; q <= req_.ladderMax && q <= ceiling; q += req_.ladderStep) legal.push_back(q);
        }
        std::int64_t best = 0;
        for (const auto q : legal) if (q <= req_.targetAmount && q > best) best = q;
        if (best >= *obs.minAmount && best > 0) {
            snappedFrom_ = req_.targetAmount;
            req_.targetAmount = static_cast<std::int32_t>(best);
        }
    }

    // The same for CHANGING an active garbage IMPORT: the largest legal quantity not above the planned one. Decreases
    // are also taken down to a legal entry (less foreign load is always safe for the own city). If the legal entry is the current amount there is nothing to write.
    void SnapImportChange(const DealObservation& obs, const ExistingDealRow& row) {
        if (req_.op != DealWriteOp::ChangeAmount || req_.key.type != utility_id::kDealImportGarbage || !QuantityInputsKnown(obs)) return;
        const auto grid = EnumerateDealQuantities(QuantityInputFor(obs, &row, true));
        const std::int64_t current = row.amount ? *row.amount : 0;
        if (LegalTarget(grid, obs, current)) return;
        std::vector<std::int64_t> legal(grid.amounts.begin(), grid.amounts.end());
        if (req_.ladderStep > 0) {
            const std::int64_t ceiling = std::max<std::int64_t>(obs.maxAmountTruncated ? *obs.maxAmountTruncated : 0, current);
            for (std::int64_t q = req_.ladderStep; q <= req_.ladderMax && q <= ceiling; q += req_.ladderStep) legal.push_back(q);
        }
        std::int64_t best = 0;
        for (const auto q : legal) if (q <= req_.targetAmount && q > best) best = q;
        if (best >= *obs.minAmount && best > 0) {
            snappedFrom_ = req_.targetAmount;
            req_.targetAmount = static_cast<std::int32_t>(best);
        }
    }

    bool QuantityInputsKnown(const DealObservation& obs) const { return obs.maxAmountTruncated && obs.minAmount; }

    DealStep Start(const DealObservation& obs) {
        const auto rows = RowsForKey(obs, req_.key);
        if (rows.size() > 1) return Finish(DealWriteState::TargetAmbiguous, "several deals share (type, neighbor); no write");
        const ExistingDealRow* row = rows.empty() ? nullptr : rows.front();

        switch (req_.op) {
        case DealWriteOp::Create: return StartCreate(obs, row);
        case DealWriteOp::ChangeAmount: return StartChange(obs, row);
        case DealWriteOp::Cancel: return StartCancel(obs, row);
        }
        return Finish(DealWriteState::Unsupported, "unknown operation");
    }

    DealStep StartCreate(const DealObservation& obs, const ExistingDealRow* row) {
        if (req_.targetAmount <= 0) return Finish(DealWriteState::QuantityNotLegal, "create needs a positive quantity");
        if (!QuantityInputsKnown(obs)) return Finish(DealWriteState::InvalidSnapshot, "GetDealMaxAmount or the tuned minimum is unknown");
        if (!row) {
            SnapImportDown(obs, nullptr);
            const auto grid = EnumerateDealQuantities(QuantityInputFor(obs, nullptr, false));
            if (!LegalTarget(grid, obs, 0)) return Finish(DealWriteState::QuantityNotLegal, "target is not an entry of the vanilla ladder for a new proposal");
            return Issue(DealPrimitive::ProposeDeals, 0, "no proposal for (type, neighbor): ask the simulator exactly as the window does");
        }
        if (IsActiveRow(*row)) {
            if (row->amount && *row->amount == req_.targetAmount) return Finish(DealWriteState::AlreadyInState, "an active contract with that amount already exists");
            return Finish(DealWriteState::PreconditionChanged, "an active contract already exists: use CHANGE_AMOUNT");
        }
        if (!IsPendingProposal(*row)) return Finish(DealWriteState::PreconditionChanged, "the existing row is cancelled, invalid or suspended; vanilla must remove it first");
        return PrepareActivate(obs, *row);
    }

    DealStep PrepareActivate(const DealObservation& obs, const ExistingDealRow& row) {
        SnapImportDown(obs, &row);
        const auto grid = EnumerateDealQuantities(QuantityInputFor(obs, &row, false));
        if (!LegalTarget(grid, obs, 0)) return Finish(DealWriteState::QuantityNotLegal, "target is not an entry of the vanilla ladder for this proposal");
        baseline_ = {row.amount ? *row.amount : 0, row.cost ? *row.cost : 0, true};
        return Issue(DealPrimitive::Activate, 0, "pending proposal found by (type, neighbor)");
    }

    DealStep AfterProposal(const DealObservation& obs) {
        const auto rows = RowsForKey(obs, req_.key);
        if (rows.size() > 1) return Finish(DealWriteState::TargetAmbiguous, "several deals share (type, neighbor) after ProposeDeals");
        if (rows.empty()) return Finish(DealWriteState::VanillaRejected, "the simulator did not propose a deal for this (type, neighbor): a vanilla gate failed");
        const auto& proposed = *rows.front();
        if (IsActiveRow(proposed) && proposed.amount && *proposed.amount == 0 && !(proposed.flags && (*proposed.flags & (deal_flag::kInvalidMask | deal_flag::kCancelled | deal_flag::kSuspensionMask)))) {
            // ProposeDeals(list, true) may hand back a row that is already ACTIVE at amount 0 (a dormant contract): it only needs SetAmount
            if (!QuantityInputsKnown(obs)) return Finish(DealWriteState::InvalidSnapshot, "GetDealMaxAmount or the tuned minimum is unknown");
            // the CREATE was planned on the ladder of a fresh proposal; the row now shows the ladder of an active contract: either is a quantity the window offers
            SnapImportDown(obs, &proposed);
            const auto grid = EnumerateDealQuantities(QuantityInputFor(obs, &proposed, true));
            const auto proposalGrid = EnumerateDealQuantities(QuantityInputFor(obs, &proposed, false));
            if (!LegalTarget(grid, obs, 0) && !LegalTarget(proposalGrid, obs, 0))
                return Finish(DealWriteState::QuantityNotLegal, "target is not an entry of the vanilla ladder for this dormant contract");
            baseline_ = {0, proposed.cost ? *proposed.cost : 0, true};
            return Issue(DealPrimitive::SetAmount, req_.targetAmount, "the simulator returned an active contract at amount 0: request the target quantity");
        }
        if (!IsPendingProposal(proposed)) return Finish(DealWriteState::VanillaRejected, "the proposal is not pending");
        if (!QuantityInputsKnown(obs)) return Finish(DealWriteState::InvalidSnapshot, "GetDealMaxAmount or the tuned minimum is unknown");
        return PrepareActivate(obs, proposed);
    }

    DealStep AfterActivate(const DealObservation& obs) {
        const auto rows = RowsForKey(obs, req_.key);
        if (rows.size() > 1) return Finish(DealWriteState::TargetAmbiguous, "several deals share (type, neighbor) after Activate");
        if (rows.empty()) return Finish(DealWriteState::TargetMissing, "the deal disappeared after Activate");
        const auto& row = *rows.front();
        if (!IsActiveRow(row)) return Finish(DealWriteState::VerifyFailed, "Activate() reported success but the row is not active");
        if (row.flags && (*row.flags & (deal_flag::kInvalidMask | deal_flag::kCancelled | deal_flag::kSuspensionMask)))
            return Finish(DealWriteState::VanillaRejected, "the activated row carries an invalid or cancelled flag");
        return Issue(DealPrimitive::SetAmount, req_.targetAmount, "activated; request the target quantity");
    }

    DealStep StartChange(const DealObservation& obs, const ExistingDealRow* row) {
        if (!row) return Finish(DealWriteState::TargetMissing, "no deal for (type, neighbor)");
        if (row->flags && (*row->flags & deal_flag::kSuspensionMask)) return Finish(DealWriteState::TargetSuspended, "SetAmount refuses suspended deals");
        if (!IsActiveRow(*row)) return Finish(DealWriteState::PreconditionChanged, "the deal is not an active contract");
        if (!row->amount || *row->amount != req_.expectedAmount) return Finish(DealWriteState::PreconditionChanged, "the contract amount differs from the planned state");
        if (req_.targetAmount <= 0) return Finish(DealWriteState::QuantityNotLegal, "use CANCEL to end a contract");
        if (*row->amount == req_.targetAmount) return Finish(DealWriteState::AlreadyInState, "the contract is already at the target amount");
        if (!QuantityInputsKnown(obs)) return Finish(DealWriteState::InvalidSnapshot, "GetDealMaxAmount or the tuned minimum is unknown");
        SnapImportChange(obs, *row);
        if (*row->amount == req_.targetAmount) return Finish(DealWriteState::AlreadyInState, "the largest legal quantity not above the plan is the current amount");
        const auto grid = EnumerateDealQuantities(QuantityInputFor(obs, row, true));
        if (!LegalTarget(grid, obs, *row->amount)) return Finish(DealWriteState::QuantityNotLegal, "target is not an entry of the vanilla ladder for this contract");
        baseline_ = {*row->amount, row->cost ? *row->cost : 0, true};
        return Issue(DealPrimitive::SetAmount, req_.targetAmount, "in-place SetAmount on the live contract");
    }

    DealStep StartCancel(const DealObservation&, const ExistingDealRow* row) {
        if (!row) return Finish(DealWriteState::AlreadyInState, "no deal for (type, neighbor)");
        if (row->flags && (*row->flags & deal_flag::kSuspensionMask)) return Finish(DealWriteState::TargetSuspended, "a suspended deal cannot be cancelled through Cancel() (SetAmount refuses it)");
        if (!IsActiveRow(*row)) return Finish(DealWriteState::AlreadyInState, "the deal is not an active contract");
        if (!row->amount || *row->amount != req_.expectedAmount) return Finish(DealWriteState::PreconditionChanged, "the contract amount differs from the planned state");
        baseline_ = {*row->amount, row->cost ? *row->cost : 0, true};
        return Issue(DealPrimitive::Cancel, 0, "Cancel() on the live contract");
    }

    // Authoritative evidence. Returns the state to report; PENDING_READBACK means "not yet", never a failure.
    DealStep Verify(const DealObservation& obs) {
        const auto rows = RowsForKey(obs, req_.key);
        if (rows.size() > 1) return Finish(DealWriteState::TargetAmbiguous, "several deals share (type, neighbor) while verifying");
        const auto acct = ReconcileAccounting(obs, req_.key);

        if (req_.op == DealWriteOp::Cancel) {
            if (!rows.empty()) {
                const auto& row = *rows.front();
                const auto f = row.flags ? *row.flags : 0;
                if (f & deal_flag::kActive) return Finish(DealWriteState::VerifyFailed, "the cancelled contract is active again");
                if (!(f & deal_flag::kCancelled)) return Pending("cancel not visible in the flags yet");
                if (!row.amount || *row.amount != 0) return Pending("cancelled flag set but the amount is not zero yet");
            }
            // the row may also be already gone (the simulator drops flag-0x04 deals on its next update): both are a cancelled contract
            if (!acct.flowReady || !acct.moneyReady) return Pending("accounting totals unavailable");
            if (!acct.flowReconciled || !acct.moneyReconciled) return Pending("accounting not reconciled yet: " + acct.detail);
            return Finish(DealWriteState::Applied, rows.empty() ? "contract removed and accounting reconciled" : "contract cancelled (flags 0x04, amount 0) and accounting reconciled");
        }

        if (rows.empty()) return Finish(DealWriteState::TargetMissing, "the deal disappeared while verifying");
        const auto& row = *rows.front();
        const auto f = row.flags ? *row.flags : 0;
        if (f & deal_flag::kCancelled) return Finish(DealWriteState::VerifyFailed, "the deal was cancelled while verifying");
        if (f & deal_flag::kInvalidMask) return Finish(DealWriteState::VanillaRejected, "vanilla marked the deal invalid (0x08 no connection / 0x10 buyer cannot pay / 0x20 no capacity)");
        if (f & deal_flag::kSuspensionMask) return Finish(DealWriteState::VerifyFailed, "the deal became suspended while verifying");
        if (!(f & deal_flag::kActive)) return Finish(DealWriteState::VerifyFailed, "the deal is not active");
        if (!row.amount || *row.amount != req_.targetAmount) return Finish(DealWriteState::VerifyFailed, "the contract amount is not the requested one");
        if (!row.cost || *row.cost < 1) return Pending("stored cost not available yet");
        if (baseline_.captured && req_.targetAmount != baseline_.amount && *row.cost == baseline_.cost) return Pending("stored cost has not moved with the amount yet");
        if (!acct.flowReady || !acct.moneyReady) return Pending("accounting totals unavailable");
        if (!acct.flowReconciled || !acct.moneyReconciled) return Pending("accounting not reconciled yet: " + acct.detail);
        return Finish(DealWriteState::Applied, "active contract at the requested amount, cost refreshed, regional flow and money reconciled");
    }

    DealWriteRequest req_;
    DealWriteState state_ = DealWriteState::PendingReadback;
    std::string reason_ = "not started";
    Phase phase_ = Phase::Start;
    DealPrimitive awaiting_ = DealPrimitive::None;
    Baseline baseline_;
    std::int32_t snappedFrom_ = 0;
    std::size_t issued_ = 0;
};

} // namespace sc4af
