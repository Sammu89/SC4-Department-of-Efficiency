**English** | [Português](../pt/deals.md) | [Français](../fr/deals.md) | [Español](../es/deals.md)

[Back to the contents](index.md#contents) | [Back to the README](../../README.md)

<a id="neighbor-deals"></a>
## E. Neighbor Deals

Reverse-engineered executable addresses related to this chapter are listed in the [game address reference](../en/game_addresses.md).

The Department can automatically manage **power, water and garbage contracts between your city and its neighbors**.

It decides when to:

- **Create a contract** when trading with a neighbor would benefit your city.
- **Increase or reduce an existing contract** as your city's needs change.
- **Cancel a contract** when it is no longer useful or safe.

The goal is to **reduce your city's overall expenses, earn money from profitable exports and maintain reliable essential services**.

The Department considers your city's own needs first. It then compares the cost of producing or disposing of a resource locally with the price offered by neighboring cities.

For example, buying power may be cheaper than increasing power plant funding. Conversely, selling surplus power may be profitable if the revenue exceeds the additional production cost.

The same principle applies to water and garbage disposal.

This chapter explains how **regional contracts work and how the Department manages them**.

For details on local production and funding, see [Power](power.md#power), [Water](water.md#water) and [Garbage & Landfills](garbage.md#garbage-landfills).

<a id="neighbor-deals-mechanism"></a>
### 1. How neighbor deals work in SimCity 4

#### The six types of regional contracts

SimCity 4 supports six types of contracts with each neighboring city: buying and selling power, water and garbage disposal services.

However, **garbage contracts work in the opposite direction from power and water**.

When you send garbage to a neighbor, you pay that city to dispose of it. When you accept a neighbor's garbage, that city pays you.

| Resource | Your city pays | Your city receives payment |
|---|---|---|
| **Power** | Buy power from a neighbor | Sell power to a neighbor |
| **Water** | Buy water from a neighbor | Sell water to a neighbor |
| **Garbage** | Send your garbage to a neighbor for disposal | Accept a neighbor's garbage for disposal |

**What does this mean for your city?**

- **Buying power or water** reduces how much your city must produce locally.
- **Selling power or water** requires enough production capacity to supply your city and the neighboring city.
- **Exporting garbage** reduces the amount your sanitation system must handle, but costs money.
- **Importing garbage** earns money, but increases the amount your sanitation system must handle.

The game allows **one contract of each type with each neighboring city**.

Contract quantities use the same monthly units displayed in the game's Neighbor Deals window. Prices and payments are expressed in simoleons (§) per simulated month.

#### What connections are required?

Regional contracts require suitable connections between neighboring cities.

The connection depends on the resource:

| Resource | Required connection |
|---|---|
| **Power** | A power line crossing the city border |
| **Water** | A water pipe crossing the city border |
| **Garbage** | A suitable road or transport connection recognized by the game's contract system |

For power and water, the rule is straightforward: the relevant network must cross the regional border.

Garbage connections are more complicated because SimCity 4 performs different internal checks.

**What was confirmed about garbage connections?**

The game's regional connection-presence check recognizes:

- Roads.
- Railways.

That particular check does not recognize highways as garbage connections.

However, the game also uses a separate lookup when checking individual neighboring cities and contracts. That lookup examines several connection types, including highways, avenues, roads and dirt roads.

**These two checks are not the same.**

The presence of a particular road type in the second lookup does not, by itself, prove that it can establish an eligible garbage contract.

The Department therefore relies on the game's actual connection and contract eligibility checks rather than assuming every transport connection is valid.

**What if a connection is lost?**

For a sale, the local city's connection must remain active.

For a purchase, the corresponding connection on the neighboring city's side must exist and be active.

If a contract loses its required connection, SimCity 4 can flag the problem during its monthly contract maintenance.

**A contract cannot be treated as reliable simply because it still appears in the list.**

#### What information does the game know about neighboring cities?

SimCity 4 keeps a small regional record for each city.

For every resource, that record includes:

| Information | What it represents |
|---|---|
| **Produced** | The resource produced locally. |
| **Imported** | The amount purchased or received from neighboring cities. |
| **Demanded** | The city's own requirement for that resource. |
| **Exported** | The amount committed to neighboring cities. |
| **Unit cost** | The game's reference cost for that resource. |
| **Treasury** | The money available to the city. |

**An important limitation: neighboring cities are not simulated while you play your current city.**

SimCity 4 uses the information last saved for those neighbors.

For example, imagine that your neighboring city once had a large power surplus. If you later change its power plants but do not save that city, the regional information may not reflect your latest changes.

The Department must work with the neighboring city records that SimCity 4 makes available.

**When are these records updated?**

The game refreshes the current city's regional information:

- At monthly updates.
- When regional contract proposals are requested, such as when you open the Neighbor Deals window.

For power, *Produced* represents generating capacity at the plants' current funding, while *Demanded* uses the game's monthly power requirement, which may be slightly higher than actual consumption.

**Do Imported and Exported represent measured deliveries?**

Not necessarily.

In the regional records, these figures are based on **contracted monthly quantities**, not a measurement of what physically moved that day.

For example, a contract to sell 5,000 units of power can count as 5,000 exported units in the regional figures as soon as the contract exists.

The game normally counts the contract's full amount on both sides, unless the contract is cancelled, invalid or not yet included in the records.

This distinction matters when the Department calculates whether another contract can be offered.

<a id="neighbor-deals-max"></a>
#### How much can neighboring cities trade?

SimCity 4 does not allow unlimited trading.

The maximum quantity depends on two things:

1. **How much surplus the selling city has.**
2. **How much additional supply the buying city is allowed to receive.**

The actual maximum is whichever of those limits is lower.

**Maximum contract quantity (base game)**

```math
Q_{\max} =
\max\left(
0,\min\left(S_{\mathrm{surplus}},B_{\mathrm{need}}\right)
\right)
```

Where:

- **Qmax** is the maximum quantity the game allows for that contract.
- **Ssurplus** is the selling city's available surplus.
- **Bneed** is the buying city's permitted additional supply.

The game calculates these values using the cities' regional records.

**Buying city's additional supply allowance**

```math
B_{\mathrm{need}} =
F \times
\max\left(
B_D+B_E,\frac{M}{F}+1
\right)
-(B_P+B_I)+q_{\mathrm{cur}}
```

**Selling city's available surplus**

```math
S_{\mathrm{surplus}} =
(S_P+S_I)-(S_D+S_E)+q_{\mathrm{cur}}
```

Where:

- **P** means Produced.
- **I** means Imported.
- **D** means Demanded.
- **E** means Exported.
- **S** identifies the selling city.
- **B** identifies the buying city.
- **qcur** is the quantity of an existing contract of this type between the two cities, or zero if no contract exists.
- **F** is the game's buyer over-demand factor, **2.5** by default.
- **M** is the game's minimum contract amount, **1,000 units** by default.

All resource quantities are monthly units.

**What does the factor of 2.5 mean?**

SimCity 4 allows a buyer to have contracted and local supply amounting to as much as **2.5 times its own demand plus export commitments**, subject to the game's other limits.

This is an upper allowance, not a requirement to buy that much.

The seller, meanwhile, can offer only the surplus remaining after accounting for its own needs and existing commitments.

**Example: selling power to a neighboring city**

Imagine your city has:

- Power production: **14,000 units**.
- Power requirement: **10,300 units**.
- No existing power contracts.

The neighboring city has:

- Power production: **3,000 units**.
- Power requirement: **6,000 units**.
- No existing contracts.

**Step 1: Calculate your surplus**

```math
S_{\mathrm{surplus}}=14000-10300=3700
```

Your city has **3,700 units** available to sell.

**Step 2: Calculate the neighbor's buying allowance**

```math
B_{\mathrm{need}}=
2.5\times6000-3000=12000
```

The neighbor is allowed to receive up to **12,000 additional units** under the game's formula.

**Step 3: Determine the maximum contract**

```math
Q_{\max}=\min(3700,12000)=3700
```

**SimCity 4 allows a contract of up to 3,700 units per month.**

Even though the neighbor could accept more, your city does not currently have more surplus to offer.

| Limitation | Quantity |
|---|---:|
| Your city's surplus | 3,700 units |
| Neighbor's buying allowance | 12,000 units |
| **Maximum allowed contract** | **3,700 units** |

If your city increases generating capacity, the permitted contract quantity may also increase.

However, the game's formula alone does not guarantee that a contract will be offered. Additional conditions must be satisfied.

**What happens when a contract already exists?**

When calculating how much an existing contract can be renegotiated, the game adds its current quantity back into both sides of the calculation.

This prevents the existing contract from artificially reducing its own maximum.

For example, if you already sell 2,000 units, the game can still evaluate whether the contract may be increased or adjusted rather than treating all 2,000 units as permanently unavailable for renegotiation.

The game does not round the calculated maximum contract quantity.

Also, **the 1,000-unit minimum is used to decide whether a proposal can be offered**. It is not automatically applied as a rounding step to the maximum quantity.

<a id="neighbor-deals-offer"></a>
#### When does SimCity 4 offer a regional contract?

Even when two cities have compatible connections and enough resources to trade, the game may not offer a new contract.

SimCity 4 also checks whether **the paying city has enough money to support the agreement**.

The original game's minimum contract quantity is **1,000 units per month**.

The game requires the paying city to have:

- A treasury greater than **§5,000**.
- Enough money to pay the minimum contract's monthly price.
- Enough money to cover **more than 60 monthly payments** at that price.

The last condition corresponds to **five years of payments in simulated game time**.

**Contract proposal requirements (base game)**

```math
\text{offered}\iff
Q_{\max}\ge M
\;\land\;
T>T_{\min}
\;\land\;
T>P(M)
\;\land\;
T>60\times P(M)
```

Where:

- **Qmax** is the maximum allowed contract quantity.
- **M** is the minimum contract quantity, normally 1,000 units.
- **T** is the paying city's treasury.
- **Tmin** is the minimum required treasury, normally §5,000.
- **P(M)** is the monthly price of a minimum-sized contract.
- **60** is the number of monthly payments the game requires the buyer to be able to sustain.

The comparison is strict for treasury values: having exactly the required amount is not enough.

By contrast, a maximum contract quantity of exactly 1,000 units satisfies the minimum-quantity condition.

**Who needs the money?**

It depends on the contract.

| Contract | City whose treasury is checked |
|---|---|
| Your city buys power | Your city |
| Your city sells power | Neighbor |
| Your city buys water | Your city |
| Your city sells water | Neighbor |
| Your city exports garbage | Your city |
| Your city imports garbage | Neighbor |

Remember that garbage exports are a service you purchase, while garbage imports are a service you provide.

**Example: why a contract may not appear**

Suppose the minimum monthly contract price is **§200**.

The game requires the paying city to hold more than:

```math
60\times200=12000
```

**§12,000**, as well as satisfying the other conditions.

| Paying city's treasury | Contract offer |
|---|---|
| §5,000 | Not offered |
| §10,000 | Not offered |
| §12,000 | Not offered |
| **§12,001** | **May be offered** |

At §12,001, the treasury condition is satisfied. The contract still needs a valid connection, a sufficient trade quantity and the game's other eligibility conditions.

**Why can a potentially profitable deal remain unavailable?**

Because **economic profitability and eligibility are not the same thing**.

A city may be willing to buy power, for example, but SimCity 4 will not offer the contract until the seller has at least 1,000 units of available surplus.

A seller that could increase production through higher funding does not automatically receive an offer based on that hypothetical future capacity.

The offer becomes possible only when the game's regional figures actually show enough surplus.

Similarly, a city may have enough resources but lack the required treasury or network connection.

**The Department must respect these original game restrictions.** It cannot assume that a financially attractive contract is available before SimCity 4 considers it eligible.

<a id="neighbor-deals-pricing"></a>
#### How are regional contract prices calculated?

SimCity 4 calculates contract prices using the **seller's average cost of supplying the resource**.

It then adds a small profit margin.

This applies to power, water and garbage disposal services.

The important distinction is that **the game's contract price is based on average cost, not on how much extra it would cost to supply that particular contract**.

That difference is central to how the Department decides which contracts are worth signing.

#### Step 1: The seller's average cost

The game first calculates how much the selling city spends per unit of available supply.

**Seller's unit cost (base game)**

```math
u=\frac{E_{\mathrm{dept}}+C_{\mathrm{buy}}}{I+P+1}
```

Where:

- **Edept** is the seller's monthly expense for the relevant department.
- **Cbuy** is what the seller pays to purchase the same resource from other cities.
- **I** is the resource imported by the seller.
- **P** is the resource produced locally by the seller.
- **u** is the resulting average cost per unit.

The additional **1** in the denominator is part of the game's formula.

**Example: a city selling power**

Suppose your city produces 14,000 units of power and spends §1,400 per month operating its power plants.

With no imports:

```math
u=\frac{1400}{14000+1}\approx0.09999
```

The game's calculated average production cost is approximately **§0.10 per unit**.

The game uses this value as the starting point for pricing power contracts.

It recalculates the current city's unit cost when updating regional records, including monthly updates and requests for new contract proposals.

#### Step 2: The game's profit margin

The contract price includes a markup above the seller's average cost.

With the original game's settings, the markup ranges from approximately **10% to 13%**.

The exact value depends on the buyer's available supply and how much surplus the seller will have left after the contract.

**Monthly contract price (base game)**

```math
P(q)=q\times\mathrm{clamp}\left(
u(1+m_{\min})+
\frac{g(V/q)+g(R/S_{\mathrm{sup}})}{2}
\,u(m_{\max}-m_{\min}),
\ u(1+m_{\min}),
\ u(1+m_{\max})
\right)
```

With:

```math
\begin{aligned}
V &= 1+(B_P+B_I-q_{\mathrm{cur}})
-\max\left(B_D+B_E,\frac{M}{F}+1\right)\\
S_{\mathrm{sup}} &= \max(S_P+S_I,10^{-8})\\
R &= S_{\mathrm{sup}}-q-(S_D+S_E-q_{\mathrm{cur}})
\end{aligned}
```

Where:

- **q** is the proposed monthly contract quantity.
- **u** is the seller's average unit cost.
- **mmin = 0.10** is the game's minimum profit factor.
- **mmax = 0.50** is the game's maximum profit factor.
- **V** represents the buyer's available supply beyond its required amount.
- **R** represents the seller's remaining supply after the contract and other commitments.
- **Ssup** represents the seller's total available production and imports.
- **qcur** is the current amount of the contract being evaluated, or zero for a new contract.
- **g** is the game's pricing curve.

For a contract quantity effectively equal to zero (below \(10^{-8}\)), the calculated price is zero.

**The pricing curve used by the game**

| Input | Curve value |
|---:|---:|
| 0 | 0.00001 |
| 0.0075 | 0.0075 |
| 1 | 0.075 |

The game interpolates between these points. Below the first point, the value remains 0.00001; above the last, it remains 0.075.

Although the game's general markup settings allow a much larger range, the small values produced by this curve limit the effective contract markup.

**With the original game's tuning, a contract is priced between approximately 1.10 and 1.13 times the seller's average unit cost.**

The markup tends to be:

- **Lower** when the buyer needs more supply or the contract uses most of the seller's remaining surplus.
- **Higher** when the buyer already has spare supply and the seller retains plenty of capacity.

In practice, the difference is relatively small.

#### Step 3: The amount actually charged

Once the game calculates the contract's monthly price, it converts it to a whole number of simoleons.

**Stored monthly contract cost (base game)**

```math
\text{cost}=\max\left(1,\lfloor P(q)\rfloor\right)
```

The resulting value is stored on the contract and appears in the city's budget.

**A contract can therefore cost at least §1 per month, even when its quantity is zero.**

This is one reason the Department removes unnecessary zero-quantity contracts.

#### Example: Pricing power sales

Return to the power example from the previous section.

The seller has an average power cost of approximately **§0.09999 per unit**.

The neighboring buyer has insufficient local supply, so its pricing contribution is at the minimum.

The game's pricing formula produces the following values:

| Contract quantity | Seller's remaining supply ratio | Pricing curve value | Price relative to unit cost | Monthly contract price |
|---:|---:|---:|---:|---:|
| 1,200 | 0.1786 | 0.01913 | 1.1038 × | §132 |
| 2,400 | 0.0929 | 0.01331 | 1.1027 × | §264 |
| 3,600 | 0.0071 | 0.00714 | 1.1014 × | §396 |
| 4,800 | Below zero | 0.00001 | 1.1000 × | §527 |

<text color="secondary">The 4,800-unit row illustrates the pricing formula beyond the previously calculated 3,700-unit contract limit. It does not mean the game would allow that contract in the example's original state.</text>

Notice that the price per unit changes slightly as the contract quantity changes.

**However, this price does not tell us whether selling power is profitable for the city.**

For example, earning §396 per month may sound worthwhile, but what if supplying those additional 3,600 units requires increasing power plant expenses by §450?

The contract would lose money.

Conversely, the city might have enough unused generating capacity to supply the contract for only §100 in additional expenses.

In that case, it would be profitable.

**The Department considers the actual additional operating cost, not just the game's average selling price.**

<a id="neighbor-deals-ladder"></a>
#### What contract quantities does the game allow?

SimCity 4 calculates a maximum possible contract quantity, but the Neighbor Deals window does not allow you to select every number between zero and that maximum.

Instead, it offers a **list of permitted quantities**.

For example, if the maximum contract is 3,700 units, the window may offer only:

**1,000, 2,000 or 3,000 units.**

It does not necessarily offer 3,700.

The available amounts depend on:

- The current maximum quantity.
- The game's minimum deal amount.
- Whether the contract is new or already active.
- The increment stored on an existing contract.
- Whether the contract is suspended.

#### How is the quantity list built?

For a new contract, the game normally divides the maximum quantity into approximately ten steps, while respecting the minimum contract size.

**New contract increment (base game)**

```text
maxQ = truncate(Qmax)
d10  = maxQ / 10       (integer division)
step = max(d10, M)
```

Where **M** is the minimum deal amount, normally 1,000 units.

The offered quantities are multiples of that step, up to the permitted maximum.

**Example 1: maximum quantity of 15,442 units**

```text
step = max(1544, 1000)
     = 1544
```

The window offers:

1,544; 3,088; 4,632; 6,176; 7,720; 9,264; 10,808; 12,352; 13,896; and 15,440 units.

**Example 2: maximum quantity of 3,700 units**

```text
step = max(370, 1000)
     = 1000
```

The window offers:

1,000; 2,000; and 3,000 units.

The precise maximum of 3,700 is not available as a selectable amount.

#### Existing contracts have additional rules

When a contract already exists, SimCity 4 tries to preserve its current quantity in the available options.

It also uses the increment stored on the contract.

If the available range becomes much larger, the game can build a two-level list with smaller steps near the current quantity and larger steps above it.

**Example**

Consider an active contract with:

- Current amount: 2,000 units.
- Stored increment: 1,000 units.
- New maximum: 30,000 units.

The window offers the smaller quantities of **1,000 and 2,000**, followed by larger steps:

**3,000; 6,000; 9,000; 12,000; 15,000; 18,000; 21,000; 24,000; 27,000; and 30,000.**

This allows an existing contract to be adjusted without requiring the player to move through dozens of small steps.

**Complete quantity-list rule (base game)**

```text
maxQ = trunc(Qmax)
       (if maxQ <= 0: use the current amount)

d10  = maxQ / 10
step = max(d10, M)

New proposal:
    Inc = step

Active contract:
    if (Inc == Amount and Inc > M) or Inc == 0:
        Inc = d10

    step     = Inc
    selected = Amount / Inc

Suspended contract:
    only the current amount

Q' = max(maxQ, step * selected, step)

If active, Amount > 0, and Q' / Inc > 15:
    Inc2 = Q' / 10

    entries =
        multiples of Inc up to Amount
        plus multiples of Inc2 above Amount, up to Q'
Otherwise:
    entries = multiples of step up to Q'
```

All divisions truncate to integers.

The game's **zero or "no deal" selection cancels a contract**.

A suspended contract cannot be resized through the normal quantity list.

**The Department respects the game's legal quantity choices.** It does not invent arbitrary contract amounts simply because a different number would be more profitable.

<a id="neighbor-deals-accounting"></a>
#### Contract states: active, pending and suspended

A regional contract can be in several different states.

| State | What it means |
|---|---|
| **Pending** | A proposal exists, but the contract is not yet active. |
| **Active** | The contract is currently in force. |
| **Suspended** | The game has temporarily interrupted the contract because of a problem. |
| **Cancelled** | The contract has been ended. |
| **Invalid** | The contract fails an eligibility condition, such as a missing connection or insufficient capacity. |
| **Suspension elapsed** | The contract has reached the end of its suspension period. |

SimCity 4 checks contracts during its monthly maintenance.

For example, the game may flag a contract when:

- The paying city's treasury is no longer greater than the contract's monthly cost.
- The required regional connection has disappeared.
- The city can no longer satisfy the relevant capacity requirement.

**A failing contract is normally suspended before the game finally ends it.**

Suspension acts as a grace period.

While suspended, the contract cannot be resized or cancelled through the normal contract operations. The game manages its subsequent resolution.

#### How are contracts counted in regional supply?

The regional figures and the financial budget do not always treat contracts in the same way.

**Regional resource quantities**

The game normally counts a contract's full amount in the buyer's Imported total and the seller's Exported total, including pending proposals and suspended contracts.

Cancelled and invalid contracts are excluded.

**Monthly contract income and expenses**

The game's financial calculations include active and suspended contracts, but exclude pending, cancelled and invalid contracts.

| Contract state | Counted in regional quantities | Counted in monthly contract income or expense |
|---|---|---|
| Pending | Yes | No |
| Active | Yes | Yes |
| Suspended | Yes | Yes |
| Cancelled | No | No |
| Invalid | No | No |

**Why is this important?**

A pending proposal can affect the game's regional supply figures before it creates any financial payment.

Likewise, a suspended contract can remain represented in the regional accounting even though its service may not be reliable.

**The Department distinguishes between a contract being recorded and its supply actually being usable.**

This prevents it from relying on suspended power or water purchases simply because their quantities still appear in the region's totals.

<a id="neighbor-deals-delivery"></a>
#### When are contracted resources delivered?

Power, water and garbage do not follow the same delivery rules.

**Power and water**

These contracts are treated as monthly supply commitments.

The contracted amount counts as supply for the buyer and as an obligation for the seller while the contract remains in force, regardless of how much is actually consumed.

**Garbage**

Garbage contracts operate through daily deliveries.

**Daily garbage contract delivery (base game)**

```math
\text{daily step}=
\max\left(1,\mathrm{round}(0.1\times A)\right)
```

The daily delivery continues while:

```math
A-U>0
```

Where:

- **A** is the contracted monthly quantity.
- **U** is the amount already delivered during the month.

The game resets monthly usage at the month change.

Consequently, **most garbage contracts deliver their entire monthly amount during the first ten or so simulated days**.

**Example: a garbage contract for 25,004 units**

The daily quantity is:

```math
\mathrm{round}(25004\times0.1)=2500
```

After ten days, the game has delivered:

```math
10\times2500=25000
```

Only four units remain, but the game does not reduce the final delivery to four.

Instead, it makes another delivery of 2,500 units on the eleventh day.

**The total becomes 27,500 units**, exceeding the contract's nominal monthly amount.

This is particularly important when your city receives garbage from a neighbor.

**The Department's sanitation calculations must not underestimate imported garbage just because the contractual amount is slightly lower than what the game actually delivers.**

#### How much garbage may your city receive?

SimCity 4 also limits the total garbage that a city may accept from its neighbors.

This limit depends mainly on:

- Available landfill storage.
- The city's own garbage production.
- Garbage already imported.
- The game's contract-duration tuning.

**Maximum received garbage (base game)**

```math
\sum A_{\mathrm{received}}
\le
L_{\mathrm{free}}
\left(1-\frac{1}{60+0.000107}\right)
+G_P+G_I
```

Where:

- **Lfree** is the city's available landfill storage.
- **GP** is the city's recorded garbage production.
- **GI** is its recorded garbage imports.
- **60** represents the game's five-year contract term, measured in months.

**Example**

Suppose a city has:

- Free landfill storage: 500,000 units.
- Garbage produced: 40,000 units.
- Garbage imported: 10,000 units.

The game's maximum received-garbage allowance is approximately:

```math
500000
\left(1-\frac{1}{60+0.000107}\right)
+40000+10000
\approx541667
```

**Maximum permitted received garbage: approximately 541,667 units.**

If the total received through contracts exceeds this limit, the game flags excess contracts as having insufficient capacity.

Those contracts are then suspended, following the game's contract-list order.

**The Department must respect this limit even when accepting additional garbage would otherwise appear profitable.**

<a id="neighbor-deals-problem"></a>
### 2. The management problem

Managing regional contracts manually is difficult because several decisions affect one another.

#### A profitable-looking contract may actually lose money

SimCity 4 bases selling prices on **average cost**.

But the economic value of a contract depends on **marginal cost**: what the city must spend additionally, or what it can save, because of that specific contract.

For example, selling power for §400 per month is not worthwhile if it requires an additional §500 in power plant expenses.

The game does not show this comparison directly.

#### Contract offers constantly change

A contract that is available today may disappear when:

- Your city's demand changes.
- A neighboring city's recorded supply changes.
- A plant loses production capacity.
- A city no longer has enough money.
- Another contract uses the available surplus.

The legal quantity choices also change with the available capacity.

#### Multiple neighbors compete for the same resources

Imagine your city can sell 5,000 units of power.

One neighbor offers to buy 3,000 units, while another offers to buy 4,000.

**Your city cannot necessarily accept both contracts**, because the total would exceed its available supply.

The Department must compare the possible combinations rather than choose each contract separately.

#### Purchases affect local production requirements

Buying power or water can reduce how much your own facilities need to produce.

With the Department's reserve rules, this can create savings greater than the purchased quantity alone might suggest.

#### The game's surplus calculation does not protect your reserve

SimCity 4's own contract limits do not include the Department's additional safety reserve.

The game may permit a sale that leaves too little spare power or water capacity for your city's needs.

**The Department must apply its own reserve and service requirements on top of the game's legal limits.**

#### Garbage contracts depend on daily timing

Two combinations of garbage contracts can have the same total monthly quantity but different delivery schedules.

That can change whether the sanitation system clears the month's garbage.

**The Department therefore evaluates complete garbage contract combinations using the daily sanitation simulation, not just monthly totals.**

<a id="neighbor-deals-algorithm"></a>
### 3. How the Department manages regional contracts

The Department combines contract selection with the local resource optimizers.

Instead of considering each neighboring city independently, it searches for **the combination of contracts that provides the greatest overall economic benefit while protecting the city's own services**.

It performs a separate optimization for power, water and garbage.

#### Does the Department sign contracts automatically?

**Yes.**

When automatic neighbor deal management is enabled, the Department can:

- Create new contracts.
- Increase or reduce existing contract quantities.
- Cancel contracts that are no longer useful.

It performs these operations directly through the game's normal contract functions.

For a new contract, it asks SimCity 4 for available proposals, activates the chosen proposal and sets its quantity.

For an existing contract, it uses the game's normal operations to change the amount or cancel the contract.

**You do not need to open the Neighbor Deals window or manually accept the contracts.**

The Department does not modify neighboring cities, rewrite save files, change contract dates or force the game to accept contracts that it would not normally offer.

**What about contracts you created yourself?**

When the Department manages a resource, your manually created contracts are included in its optimization.

It may resize or cancel them if another legal combination is better.

If you want to keep full manual control over a particular resource's contracts, disable that resource's automatic deal management option.

#### How often are contracts reviewed?

Contracts are normally reviewed alongside their corresponding resource.

| Resource | Regular review |
|---|---|
| **Power** | Every 10 simulated days, with earlier reviews for significant changes or confirmed supply shortages. |
| **Water** | Every 10 simulated days since the last water review, at each month change, and earlier when necessary. |
| **Garbage** | At each month change and when monitored sanitation values change significantly. |

A lightweight daily check also looks for real shortages and significant changes in demand, production or regional contracts.

Each evaluation starts from the city's current state.

**The Department does not normally impose an additional waiting period or cooldown simply because it changed a contract recently.**

There are specific protections against unnecessary contract changes, explained below and under contract re-evaluation.

#### Step 1: Find every available contract option

The Department begins by examining neighboring cities with suitable connections and complete regional records.

For each resource and trading direction, it identifies the legal contract quantities.

**Existing contracts**

The current amount is included, along with the other quantities permitted by the game.

**New power and water purchases**

The Department uses the quantities currently permitted by the game's proposal list.

It excludes purchases whose monthly price would not remain below the local city's available treasury.

**New power and water sales**

The neighboring city must already appear in the game's available offers.

However, some larger sale quantities may only become possible **after the Department increases local production capacity**.

The Department can consider these as *latent sale candidates*.

For example, imagine your city currently has only 2,000 units of power surplus, but could produce an additional 3,000 units by increasing plant funding.

A larger contract might become possible once that capacity is actually available.

The Department can evaluate whether the additional production cost would be justified.

**A latent contract is not signed until the game's actual eligibility rules permit it.**

**Garbage received from neighbors**

The Department considers the quantities permitted by the game's current proposal list.

It must also verify that the city can handle the additional waste.

#### Why does the Department use a common pricing baseline?

When evaluating contract alternatives, the Department first removes its own adjustable contracts from the pricing calculations.

It then evaluates candidate quantities against this common baseline.

This prevents an undesirable situation where changing one contract alters the regional records, which changes the calculated price, which makes the Department immediately prefer the previous arrangement again.

**The same candidate quantity should not appear more or less attractive merely because the Department has just written an equivalent contract.**

When evaluating complete portfolios, prices are calculated using the selected combination of contracts.

Candidates without enough reliable information to calculate their price are excluded. Missing values are never silently treated as zero.

<a id="neighbor-deals-objective"></a>
#### Step 2: Find the most profitable combination

The Department does not simply maximize contract income.

It considers **income, contract expenses and the cost of supplying the city's own services together**.

**Regional contract portfolio objective**

```math
\max_x\ \mathrm{Net}(x)=
\sum_{i\in\mathrm{sell\text{-}like}}P_i(x_i\mid x)
-
\sum_{j\in\mathrm{buy\text{-}like}}P_j(x_j\mid x)
-
C_{\mathrm{local}}(x)
```

Where:

- **x** represents the chosen quantities for all candidate contracts of one resource.
- **Pi** is the income from a sell-like contract.
- **Pj** is the expense of a buy-like contract.
- **Clocal(x)** is the lowest local operating cost that can safely support the chosen contracts.
- **Net(x)** is the resulting monthly economic value.

Each contract price is calculated in the context of the full selected portfolio.

**What is included in local cost?**

It depends on the resource:

| Resource | Local cost considered |
|---|---|
| **Power** | Power plant funding needed to satisfy local requirements, reserve and exports, after imports. |
| **Water** | Water department funding needed to satisfy local requirements, reserve and exports, after imports. |
| **Garbage** | Sanitation funding needed to handle the city's garbage, the chosen imports and exports, and the configured reserve. |

The Department uses the same calculations described in the previous chapters.

When **Consider power plant aging** is enabled, power's local economic cost also includes estimated plant wear.

That wear is used for comparing options but is **not an additional treasury expense**.

#### Example: Comparing contract portfolios

Consider an illustrative city with three possible arrangements:

| Arrangement | Contract income | Contract expense | Local operating cost | Net |
|---|---:|---:|---:|---:|
| Local production only | §0 | §0 | §1,000 | −§1,000 |
| Buy from one neighbor | §0 | §300 | §600 | −§900 |
| Sell to another neighbor | §400 | §0 | §1,200 | −§800 |

In this simplified example, selling provides the best economic result, even though local operating costs increase.

The Department would prefer that arrangement **provided every legal and safety condition is satisfied**.

The figures are illustrative; the real planner uses the game's contract prices and resource-specific funding calculations.

**How are equal results handled?**

If two legal portfolios have the same economic value, the Department prefers:

1. The portfolio with **fewer contracts**.
2. If still tied, the portfolio requiring **fewer changes**.

This avoids unnecessary contracts and adjustments that do not improve the result.

#### The safety rules every portfolio must satisfy

The Department only considers contract combinations that respect the following restrictions.

**1. Your city's services come first**

The local production or sanitation plan must remain feasible with the chosen contracts.

A portfolio that the city's own services cannot safely support is rejected.

**2. Purchases cannot exceed your city's own demand**

The Department does not buy power or water just to resell it.

It also does not purchase extra power or water solely to create its configured reserve.

**3. Normally, a resource is either bought or sold**

By default, the Department chooses one of three positions for each resource:

- **LOCAL:** No purchases or sales.
- **BUY:** Purchase the resource.
- **SELL:** Sell the resource.

Buying and selling the same resource simultaneously is not allowed under the default configuration.

The optional **Buy from one city to sell to another** setting is explained in the contract settings.

**4. Garbage imports must remain within safe limits**

The total garbage received cannot exceed the game's incoming-garbage limit or the additional amount the city's sanitation system can handle.

**5. Suspended contracts are fixed**

A suspended contract cannot be changed through normal game functions.

The Department includes its existing amount in the calculations but does not attempt to resize or cancel it.

**6. A confirmed power or water deficit prevents sales**

If the game's counters confirm a genuine regional shortage, the Department does not maintain sales of that resource.

**Its own city's essential supply takes priority over export income.**

#### How are power and water reserves included?

The Department calculates how much local capacity remains necessary after imports and exports.

**Required local capacity**

```math
N(B,S)=
\max\left(
\left\lceil R\times\max(0,D-B)\right\rceil,
R_q-B
\right)+S
```

Where:

- **D** is the city's own demand.
- **B** is the total purchased quantity.
- **S** is the total sold quantity.
- **R** is the reserve multiplier.
- **Rq** is the game's own monthly power requirement.

For power, the default reserve is 20%, giving **R = 1.20**.

For water, the default reserve is 10%, giving **R = 1.10**.

**For water, the Rq term is omitted**, since the water calculation does not use the separate power requirement counter.

**Example: Power with a 20% reserve**

Suppose the city has:

- Demand: 10,000 units.
- Game requirement: 10,300 units.
- Reserve: 20%.

Without contracts:

```math
N(0,0)=12000
```

If it buys 2,000 units:

```math
N(2000,0)=
\max(9600,8300)=9600
```

If it instead sells 2,400 units:

```math
N(0,2400)=14400
```

| Situation | Required local generating capacity |
|---|---:|
| No contracts | 12,000 |
| Buy 2,000 units | 9,600 |
| Sell 2,400 units | 14,400 |

**Purchasing 2,000 units reduces the local capacity requirement by 2,400 units** in this example, because the reserve applies only to what the city must produce itself.

Selling 2,400 units increases the requirement by exactly 2,400.

The Department therefore compares contract prices with the actual change in local costs caused by these capacity requirements.

#### Why can't the Department buy more than the city needs?

The Department applies an additional purchase limit.

**Purchase cap**

```math
\sum_j x_j\le
\min\left(
D,\;
F\times\max\left(L_D+L_E,\frac{M}{F}+1\right)
-(L_P+L_I)
\right)
```

Where:

- **D** is the city's own demand.
- **F** is the game's buyer over-demand factor.
- **M** is the minimum contract amount.
- **L** values are the local city's regional records, excluding its own adjustable purchases.

For garbage, the demand limit is based on **the city's own garbage production minus recycling**.

This prevents the Department from:

- Buying more than its own needs.
- Buying resources solely to resell them.
- Buying additional resources to create a reserve on imports.
- Using purchased supply as the basis for new export commitments.

The game's own legal purchasing limits are respected as well.

#### How does the Department evaluate one contract?

The portfolio calculation can also be understood by looking at a single contract.

**For a sale**

The Department compares the payment received with the additional local operating cost.

```math
\mathrm{Profit}(q)=
P(q)-
\left[
C_{\mathrm{local}}(N+q)
-
C_{\mathrm{local}}(N)
\right]
```

For example, if a sale earns §300 but requires §200 more in production expenses:

**Net benefit: §100 per month.**

The sale is profitable.

**For a purchase**

The Department compares the contract price with the local expense it avoids.

```math
\mathrm{Saving}(q)=
\left[
C_{\mathrm{local}}(N(0))
-
C_{\mathrm{local}}(N(q))
\right]
-
P(q)
```

For example, if buying power costs §250 but allows your city to save §400 in power plant funding:

**Net saving: §150 per month.**

The purchase is profitable.

**This is marginal profitability:** the Department evaluates the actual economic effect of adding or changing a contract.

It does not assume that a sale is worthwhile simply because the game applies a markup to the seller's average cost.

#### Step 3: Search for the best contract combination

Once the available quantities and safety requirements are known, the Department compares the legal portfolios.

The search works differently for power, water and garbage.

**Power and water**

The Department uses an optimization algorithm to compare combinations of total purchases and sales.

To keep the calculation efficient, quantities are initially grouped into **100-unit blocks**.

For very large trading ranges, the block size increases to limit computation.

After choosing the best candidate, the Department checks the complete combination again using **exact quantities**, without the approximate grouping.

**Garbage**

Garbage requires a different method.

Because each contract has its own daily delivery schedule, the Department cannot safely decide using monthly totals alone.

It therefore evaluates complete contract combinations and runs the sanitation funding calculation for each one.

**Every candidate portfolio must be tested against the projected garbage disposal process.**

This search can become expensive when many neighbors and quantities are available.

If the number of combinations exceeds **5 million on one side of the search**, the Department considers the optimum unproven.

**It does not make a speculative contract change when it cannot establish a reliable result.**

#### Step 4: Remove unnecessary contracts and changes

After finding the preferred portfolio, the Department performs several final checks.

**1. Remove contracts that contribute no benefit**

If removing a contract does not make the portfolio economically worse, the Department drops that contract.

Every optional contract must contribute a strictly positive benefit.

**2. Never replace a better existing arrangement**

If keeping all current contracts is legal, feasible and strictly more valuable than the proposed result, the Department leaves the contracts unchanged.

**3. Ignore insignificant optional changes**

An advanced INI setting can specify a minimum benefit required for optional contract changes.

The default threshold is **§0**, so any strictly positive benefit can qualify.

When a higher threshold is configured, the Department avoids optional contract creations or quantity changes whose benefit is below that amount.

**Changes necessary to maintain essential services are exempt from this economic threshold.**

**4. Remove dormant zero-quantity contracts**

SimCity 4 can sometimes leave an active contract with an amount of zero.

Such a contract provides no resource and still costs §1 per month.

The Department cancels these unnecessary contracts.

#### How is the benefit of one contract decision measured?

When multiple contracts interact, looking only at one contract's income or expense may give the wrong answer.

Instead, the Department asks:

**How much worse would the best legal plan become if this particular contract had to stay at its current amount?**

**Marginal benefit of a contract decision**

```math
\Delta_k=
\mathrm{Net}(x^*)
-
\max_{x:\,x_k=x_k^{\mathrm{now}}}
\mathrm{Net}(x)
```

Where:

- **x\*** is the selected optimal portfolio.
- **xk** is the quantity of the contract being evaluated.
- **xk now** is its current quantity.
- **Net** is the total economic value of the portfolio.
- **Δk** is the economic benefit attributable to allowing that particular contract to change.

The Department compares the selected portfolio with the **best alternative that keeps this contract unchanged**, not merely with the current overall city state.

This matters because another neighbor might offer an equally good contract.

For example, suppose two neighbors can supply power for the same total economic cost.

Creating a contract with either one may be profitable compared with producing everything locally.

But if the other neighbor can provide the same benefit, **neither particular contract is indispensable**.

The Department considers that when deciding whether an individual change is justified.

**A contract creation or increase is normally written only when its marginal benefit is strictly positive.**

There is one exception: a change required to maintain service when no legal portfolio can keep that contract at its current amount.

In that case, the Department does not invent a monetary benefit. It recognizes that the change is necessary to satisfy the service requirements.

#### Cash savings versus economic savings

For power, the Department keeps two versions of a contract decision's benefit:

| Measure | What it includes |
|---|---|
| **Cash benefit** | Changes in actual operating expenses and contract income or payments. |
| **Economic benefit** | Cash benefit plus the estimated value of power plant wear, when aging is enabled. |

**The decision uses the economic benefit.**

The cash benefit describes the effect on the city's actual budget.

Estimated plant wear is never charged directly to the treasury.

This distinction allows the Department to consider long-term plant deterioration without presenting an imaginary monthly expense as money actually spent.

**The overall objective remains the same: choose the most economically beneficial combination of legal contracts without compromising your city's essential services, reserves or existing safety protections.**
<a id="neighbor-deals-exclusive"></a>
#### Buying or selling: one direction by default

By default, the Department does not buy and sell the same resource at the same time.

For each resource, it chooses one of three possible arrangements:

| Arrangement | What the Department does |
|---|---|
| **LOCAL** | Uses local production or disposal without buying or selling that resource. |
| **BUY** | Purchases the resource from one or more neighboring cities. |
| **SELL** | Sells the resource or disposal service to one or more neighboring cities. |

For power and water, buying and selling have their usual meanings.

For garbage, the directions are reversed:

- **BUY:** Your city pays a neighbor to dispose of its garbage.
- **SELL:** Your city receives garbage from a neighbor and gets paid for treating it.

This behavior is controlled by **Buy from one city to sell to another**, which is **off by default**.

#### What if the city already has contracts in both directions?

The Department compares the available arrangements and moves towards the one with the greatest overall economic benefit.

For example, if your city is both buying and selling power, the Department normally chooses whether to keep the purchases or the sales, then cancels the unnecessary contracts.

**There is one important exception: suspended contracts.**

SimCity 4 does not allow suspended contracts to be changed through its normal contract functions.

Suppose your city has a suspended power sale but now needs to purchase power to avoid a shortage.

The Department cannot immediately remove the suspended sale.

In that situation, **the city's power supply takes priority over the usual buy-or-sell restriction**.

The Department can purchase the power needed to maintain service, avoids creating additional sales and records the policy conflict in the log.

It does not allow the exclusivity rule to leave the city without an essential resource.

<a id="neighbor-deals-arbitrage"></a>
#### Optional: Buying and selling at the same time

You can enable **Buy from one city to sell to another** to allow the Department to consider contracts in both directions for the same resource.

This requires **Manage neighbor deals** to be enabled.

The Department only chooses a mixed arrangement when it produces a better economic result than every available one-direction arrangement.

**How can this be useful?**

Consider a city with two neighbors.

Neighbor A sells power relatively cheaply, while Neighbor B is willing to pay more for power.

Your city might benefit from buying power from A and selling power to B.

However, **the Department does not simply buy power and resell those same imported units**.

Instead, the purchase reduces the amount of power your city must produce for its own buildings.

That can free local generating capacity for a profitable sale.

**Example: power**

Imagine the following arrangement:

- Your city needs 10,000 units of power.
- It purchases 3,000 units from Neighbor A.
- Its own plants supply the remaining local demand and reserve.
- Available local generating capacity supports a sale to Neighbor B.

The Department evaluates whether the purchase cost, sale income and resulting power plant expenses produce a better overall result.

The normal power reserve and every other safety rule still apply.

**What about garbage?**

Garbage can also benefit from a mixed arrangement.

For example:

- Neighbor A pays your city to accept its garbage.
- Your city pays Neighbor B to dispose of some of its own garbage.

If the income from receiving garbage exceeds the additional sanitation and export costs, the combination may be worthwhile.

The Department evaluates the complete disposal schedule to ensure that the city can handle the resulting garbage.

#### What restrictions still apply?

Enabling this option does **not** remove the normal protections.

- Purchases cannot exceed your city's own demand.
- Imported power or water is never used as the basis for resale.
- The configured power and water reserves remain mandatory.
- Garbage imports must remain within the permitted disposal limits.
- Every contract must respect the game's eligibility, quantity and treasury rules.
- Local services must remain feasible.
- A resource with automatic deal management disabled is never traded by the Department.

**A neighboring city cannot simultaneously be your supplier and customer for the same resource.**

For example, Neighbor A may sell power to your city or buy power from it, but not both in the same plan.

Different neighbors may take different roles.

#### How does the Department choose each neighbor's role?

When a neighboring city could potentially trade in either direction, the Department evaluates the possible assignments.

For up to **four such neighbors**, it can evaluate every assignment exactly.

Four neighbors create at most:

```math
2^4=16
```

possible buy-or-sell assignments.

Each assignment is then evaluated using the normal contract optimization rules.

To avoid excessive computation, the Department falls back to one-direction trading when:

- More than **four neighbors** require a buy-or-sell assignment.
- The mixed garbage search would exceed **4,000 buy-by-sell combinations**.

In those situations, the Department uses the ordinary one-sided optimization for that review.

**The option expands the economic choices available to the Department without allowing unlimited trading or bypassing safety rules.**

<a id="neighbor-deals-execution"></a>
#### Step 4: Applying contract and funding changes safely

Finding the best combination of contracts is only part of the job.

The Department must also apply the changes in an order that does not interrupt your city's services.

For example, imagine it wants to buy power from a neighbor and reduce power plant funding.

**Reducing production before the purchase is available could cause a blackout.**

Likewise, selling additional water before increasing local production could leave your own buildings without enough supply.

The Department coordinates local funding and regional contracts through six execution phases.

#### The six execution phases

| Phase | Action | Why it comes in this order |
|---|---|---|
| **1** | Increase local production capacity | Make additional local supply available before it is needed. |
| **2** | Reduce or cancel sales | Free capacity currently committed to neighbors. |
| **3** | Create or increase purchases | Establish replacement supply before reducing local production. |
| **4** | Create or increase sales | Begin new exports only after the necessary supply is available. |
| **5** | Reduce local production capacity | Lower funding once the new arrangement is supported. |
| **6** | Reduce or cancel purchases | Remove imported supply only when the remaining arrangement can support the city. |

**The guiding principle is that the transition must be safe, not just the final arrangement.**

For power and water, the local supply requirement depends on the city's own demand, sales and purchases.

In simplified form:

```math
\text{Local need}=\text{Own need}+\text{Sales}-\text{Purchases}
```

The full calculation also includes the configured reserve and, for power, the game's separate requirement counter.

The execution order ensures that intermediate changes do not require more local capacity than the starting or final arrangement.

#### Example: Replacing local power with a purchase

Suppose your city's power plants currently supply everything locally.

The Department finds that purchasing power from a neighbor would reduce total expenses.

It proceeds in this order:

1. Establish the new power purchase.
2. Verify that the game has accepted the contract.
3. Recalculate the local capacity requirement.
4. Reduce power plant funding when it is safe to do so.

**It does not cut your power plants before confirming the alternative supply.**

#### Example: Replacing an power purchase with local production

Now suppose the opposite becomes more economical.

Your city wants to stop buying power and produce it locally instead.

The Department:

1. Increases the necessary power plant funding.
2. Verifies that local generating capacity is available.
3. Reduces or cancels the power purchase.

**It does not cancel imported supply before the replacement local capacity exists.**

#### Are there exceptions to the phase order?

Yes. Two changes can be brought forward to Phase 2 when the Department has confirmed that they cannot compromise local service:

- **Reducing a purchase** when the city's confirmed local capacity is already sufficient without it.
- **Creating or increasing a sale** that the game currently offers, when existing confirmed local capacity is already sufficient to support it.

In these cases, the capacity is already available. There is no need to wait for an unnecessary funding increase.

#### Does the Department apply all six phases at once?

No.

**Each review executes only the earliest pending phase for each resource, with one contract change at a time.**

The Department then reads the updated city state before continuing during a later review.

This prevents multiple unverified changes from being applied together.

#### What happens to sales that require additional production first?

As explained earlier, the Department can identify a potentially profitable power or water sale that is not yet available at the desired quantity.

This is called a *latent sale*.

For example, a sale might become legal after increasing power plant funding.

The Department must first increase generating capacity.

Only after the game recognizes that capacity and allows the intended contract can the sale be created.

**A latent sale is never signed before the game permits it.**

There is also a safeguard against unnecessary funding.

If the Department has planned a latent sale but there is no corresponding capacity increase or sale reduction that would make it possible, it recalculates the plan without that latent quantity.

**Your city is not deliberately left paying for additional production capacity to support a sale that cannot happen.**

<a id="neighbor-deals-garbage"></a>
#### Special rules for garbage contracts

Garbage contracts require additional care because waste is delivered on a daily schedule rather than as a continuous monthly supply.

#### Sending your garbage to a neighbor

Exporting garbage is treated as purchasing disposal capacity.

The Department limits the amount sent to neighbors to:

- The city's own garbage production after recycling.
- The maximum quantity permitted by the game's purchasing rules.

It does not export more garbage than the city itself produces after recycling.

#### Receiving garbage from a neighbor

Importing garbage generates revenue, but adds waste that your sanitation system must dispose of.

The Department includes incoming garbage **on top of your city's own production and sanitation reserve**.

It accepts a contract only when:

- The chosen sanitation funding can handle the additional waste.
- The projected month ends with **no uncollected garbage**.
- The revenue justifies any additional sanitation expense.
- The game's received-garbage limits are respected.
- The city's remaining disposal capacity is sufficient.

**The Department does not accept garbage simply because a neighbor is willing to pay for it.**

The city must be able to handle that garbage safely and economically.

#### Why does the Department evaluate each garbage contract separately?

Two contracts can have the same combined monthly amount but different daily delivery schedules.

For example, one large contract may deliver slightly differently from two smaller contracts because the game rounds each contract's daily quantity independently.

The Department therefore simulates every contract using its own:

- Monthly amount.
- Daily delivery quantity.
- Amount already delivered during the month.
- Remaining deliveries.

**It never replaces individual delivery schedules with a single combined monthly total.**

This is essential when predicting whether landfills and treatment facilities can clear the garbage before the month ends.

#### Why are some garbage contracts kept until the next month?

Garbage contracts usually complete their monthly deliveries during the first ten or so days.

Suppose a contract has already delivered its entire allocation.

Replacing it with a new contract during the same month may introduce a fresh delivery schedule that the current month's simulation did not anticipate.

When the Department cannot reliably predict the following month's landfill behavior, it therefore **keeps an already exhausted contract until the monthly rollover**, rather than replacing it unnecessarily.

This restriction can be relaxed when keeping the contract would prevent the city from maintaining its sanitation service.

#### Does last month's imported garbage count as local production?

No.

The game's *Garbage imported last month* counter reports past deliveries.

**The Department never counts those imports as garbage produced by your own city.**

Local production is calculated from the city's Produced and Recycled values.

Foreign garbage enters the plan through its individual contracts.

<a id="neighbor-deals-verification"></a>
#### Step 5: Checking every contract before and after changing it

The Department does not blindly apply a contract plan calculated earlier.

**Every individual contract change must be rechecked immediately before it is written.**

This protects against changes in demand, capacity, treasury, contract eligibility and the game's own regional records.

#### Before changing a contract

**1. Refresh the regional information**

The Department asks SimCity 4 to refresh its regional resource records, just as the game does during its normal monthly updates.

It then reads the current city state and calculates a new contract plan.

The intended change must still match the new plan:

- The same contract type and neighboring city.
- The same starting quantity.
- The same target quantity.
- The same earliest pending execution phase.
- The required economic justification.

**If the proposed change no longer passes these checks, nothing is written.**

The Department does not force an outdated contract decision onto a changed city.

**2. Check the game's actual price**

For a new contract or a change in quantity, the Department asks SimCity 4 for its authoritative price.

It compares that price with the amount predicted by its own pricing formula.

**Contract price tolerance**

```math
\left|P_{\mathrm{game}}-P_{\mathrm{model}}\right|
\le
\max\left(
2,\left\lfloor\frac{P_{\mathrm{model}}}{100}\right\rfloor
\right)
```

Where:

- **Pgame** is the monthly price returned by SimCity 4.
- **Pmodel** is the price calculated by the Department.

The permitted difference is at least **§2**, increasing to approximately **1%** for larger contracts.

**Example**

Suppose the Department predicts a monthly price of **§1,874**.

The tolerance is:

```math
\max(2,\lfloor1874/100\rfloor)=18
```

The game must return a price between **§1,856 and §1,892**, inclusive.

| Price reported by the game | Result |
|---|---|
| §1,855 | Rejected |
| §1,856 | Accepted |
| §1,874 | Accepted |
| §1,892 | Accepted |
| §1,893 | Rejected |

**What if the difference is too large?**

The Department treats its pricing model as unreliable for the current city.

It stops automatic neighbor contract management for the rest of that city session.

**Local power, water and sanitation funding management can continue independently.**

This prevents the Department from making financial decisions using prices that the game does not confirm.

**3. Verify the exact contract**

The Department finds the live contract using its **type and neighboring city**, rather than relying on an earlier remembered position in the contract list.

It refuses the change when:

- More than one contract matches the same identity.
- The contract is suspended.
- The current quantity no longer matches the quantity used in the plan.
- The requested target quantity is no longer legal.

A target must be one of the game's permitted quantities, or an allowed multiple of the proposal increment within the game's current maximum.

#### A special rule for received garbage

There is one conservative adjustment for garbage received from neighbors.

Suppose the Department planned to accept 10,000 units, but the game now allows only 8,000.

If 8,000 is a legal amount, the Department may reduce the planned received-garbage quantity to that lower amount.

It chooses the **largest legal amount not exceeding the original target**.

This is safe because accepting less foreign garbage cannot increase the city's disposal burden.

**The same adjustment is not applied to purchases.**

For example, silently reducing a planned power purchase could leave the city without enough supply.

The Department therefore does not substitute a smaller purchase quantity without recalculating the plan.

#### After changing a contract

A successful contract operation does not mean that every game counter has already updated.

The Department waits for confirmation before considering the change complete.

It checks that:

1. **The contract has the correct state:** Active after creation or resizing, or cancelled at zero quantity or removed after cancellation.
2. **The quantity is correct:** The amount matches the requested value.
3. **The stored monthly price is updated.**
4. **Regional quantities are consistent:** Imported and Exported totals agree with the contract list.
5. **Financial totals are consistent:** Monthly neighbor deal income and expenses agree with the active and suspended contracts, following the game's accounting rules.

**The game may update these values slightly after the contract operation.**

If the new values have not appeared yet, the Department waits.

A delayed update is not immediately treated as a failed transaction.

While a contract change is awaiting confirmation:

- No additional contract change is made for that resource.
- The pending transaction is checked again every simulated day.
- Other resources can continue their own independent management.

**The Department only proceeds once the previous contract change has been confirmed.**

<a id="neighbor-deals-revenue"></a>
#### How does the Department measure contract savings?

The Department distinguishes between three stages of a contract decision.

| Stage | What it means |
|---|---|
| **Projected** | The expected monthly benefit of a contract plan, calculated before applying changes. |
| **Pending** | The expected cash benefit of a contract change that has been written but is still awaiting verification. |
| **Realized** | The change in monthly neighbor deal income and expenses confirmed by the game's own budget counters. |

These distinctions prevent the Department from presenting a planned or unverified contract as money already earned.

#### Projected savings

Before changing contracts, the Department estimates how much better the proposed arrangement would be than keeping the existing contracts.

It calculates two values:

- **Cash benefit:** The expected change in actual income and expenses.
- **Economic benefit:** The cash benefit including estimated power plant wear, when that option is enabled.

This comparison is made when keeping the existing contracts is a feasible alternative.

A planned contract or latent sale is **not counted as realized income** merely because it appears in an optimized portfolio.

#### Pending savings

Once a contract change has been written, but before the game confirms it, the Department tracks its expected cash margin as **pending**.

This is still an estimate.

It does not become a realized result until the game's contract state and financial counters have been verified.

#### Realized contract income

After confirmation, the Department compares the game's monthly neighbor deal income and expenses before and after the change.

**Realized contract result**

```math
\Delta_{\mathrm{realized}}=
(I_{\mathrm{after}}-X_{\mathrm{after}})
-
(I_{\mathrm{before}}-X_{\mathrm{before}})
```

Where:

- **Ibefore** and **Iafter** are the game's total monthly neighbor deal income before and after the change.
- **Xbefore** and **Xafter** are the corresponding monthly neighbor deal expenses.
- **Δrealized** is the verified change in monthly net contract income.

**Example: a new power sale**

Suppose the city's monthly neighbor deal figures were:

| | Before | After |
|---|---:|---:|
| Contract income | §200 | §500 |
| Contract expense | §100 | §100 |
| **Net contract income** | **§100** | **§400** |

The realized change is:

```math
\Delta_{\mathrm{realized}}=
(500-100)-(200-100)=300
```

**Verified improvement in monthly contract income: §300.**

This is the change in contract income alone.

It does not yet subtract any extra power plant expense required to support the sale.

Those changes are recorded separately when the Department adjusts local funding.

#### What if the financial counters cannot be read?

If SimCity 4 does not provide reliable before-and-after contract income and expense totals, the Department uses the planned cash margin as a fallback.

**The log explicitly records that an estimate was used instead of a verified financial measurement.**

The two are not silently presented as equivalent.

#### What about savings from reduced local funding?

A regional purchase can create two separate financial effects.

For example, buying power may:

1. Add a monthly contract expense.
2. Allow the Department to reduce power plant funding.

The first effect belongs to the neighbor deal accounting.

The second belongs to power funding management.

**The Department records these separately to avoid counting the same saving twice.**

Similarly, a garbage contract may increase contract income while requiring additional sanitation funding. Those are separate budget changes.

**The Department's aim is to select contracts according to their complete economic effect, apply them without disrupting essential services, and distinguish expected benefits from changes actually confirmed by the game.**
<a id="neighbor-deals-reevaluation"></a>
#### When does the Department reconsider existing contracts?

The Department does not assume that an existing contract should continue forever.

**At every review, it recalculates the economic value of all contracts using the city's latest information.**

For example:

- An power sale may become less profitable as your city's demand increases.
- A water purchase may no longer be necessary after you build another pumping facility.
- A garbage import may become too expensive to handle if your landfills approach their limits.

When the situation changes, the Department can increase, reduce or cancel contracts to find a better arrangement.

**Your city's own needs and safety reserves always take priority.**

If power demand grows, for example, the Department reduces or cancels power sales before allowing them to consume the capacity needed for your city and its reserve.

#### Does the Department remember previous failures?

Normally, every review starts fresh. There is no general contract holding period or cooldown.

However, two exceptions prevent repeated actions that the game has already refused.

**1. A new contract was rejected by the game**

If SimCity 4 refuses to create a contract, the Department does not attempt that same creation again until the **third simulated month change after the refusal**.

This gives the game's eligibility conditions, such as treasury, demand and connections, time to change.

**2. The game refuses to cancel a dormant contract**

A dormant contract is an active contract with a quantity of zero.

If the game refuses its cancellation, the Department leaves it alone for the rest of the current city session rather than repeatedly attempting the same operation.

<a id="neighbor-deals-emergency"></a>
#### What happens during a serious supply shortage?

Sometimes a city simply cannot obtain enough power or water to satisfy its full requirements.

For example:

- A major power plant is demolished.
- Several water facilities stop producing.
- A regional connection is lost.
- A neighboring city can no longer supply a contracted resource.

Normally, the Department searches for contracts that satisfy both your demand and your configured safety reserve.

**But what if no legal combination of local production and regional purchases can achieve that?**

In that situation, the Department changes priorities.

Instead of rejecting every possible arrangement because it cannot satisfy the full requirement, **it tries to supply as much of the city's actual demand as possible**.

#### How much demand can still be served?

**Served demand during an emergency**

```math
\mathrm{served}(B,S)=
D-\max\left(
0,\max(0,D-B)+S-C_{\max}
\right)
```

Where:

- **D** is the city's actual demand.
- **B** is the amount purchased from neighboring cities.
- **S** is the amount sold to neighboring cities.
- **Cmax** is the greatest local generating or pumping capacity that the city's facilities can legally provide.

The formula determines how much demand remains covered after accounting for imports, exports and the maximum available local capacity.

**Example: a city facing an power shortage**

Suppose your city needs **10,000 units**, but its power plants can produce only **6,000 units**.

Without imports, the city cannot supply 4,000 units of its demand.

If a neighboring city can legally provide 3,000 units, purchasing them increases the amount of demand served to **9,000 units**.

The city is still short of power, but the Department has reduced the deficit.

**During an emergency, the Department follows these priorities:**

1. **Serve as much actual demand as possible.**
2. **Restore the safety reserve where possible.**
3. **Choose the least expensive arrangement among equally effective solutions.**

The normal reserve is therefore still considered, but actual service comes first when there is not enough capacity to satisfy everything.

#### What happens to power and water sales?

The Department cancels every sale it is legally able to change.

This frees capacity for your own city.

It then considers the legal purchase quantities available from neighboring cities.

Normally, purchases cannot exceed the city's own demand.

**During an emergency, one additional quantity step beyond demand may be allowed**, but only if that step increases the amount of demand actually served by more than 1%.

This is a limited exception intended to deal with the game's discrete contract quantity choices.

Suspended contracts are still included in the calculations, but the Department does not attempt to modify them.

#### What about a garbage disposal emergency?

For garbage, the Department follows the equivalent principle.

It attempts to:

- Cancel contracts that bring additional garbage into your city.
- Increase garbage exports to neighboring cities.
- Use the least expensive available disposal arrangements first.

Garbage exports remain subject to the game's legal quantities and the amount of garbage your city produces after recycling.

**The Department cannot guarantee that every shortage will be solved.**

If the available facilities and contracts are insufficient, the player must provide additional infrastructure or resolve the underlying problem.

The Department only issues a shortage notice when the problem is **confirmed and persistent**, rather than warning you about every temporary fluctuation.

See [Notices](safety.md#notices).

<a id="neighbor-deals-safety"></a>
### 4. Safety conditions

The Department follows strict rules when creating, changing or cancelling regional contracts.

#### Protecting your city's own services

**Your city's needs come first.**

Power and water sales must not consume the capacity required to serve the city and maintain its configured reserve.

SimCity 4's own definition of available surplus does not include the Department's safety margins.

The Department therefore performs its own capacity checks before accepting a sale.

#### Only relying on dependable purchases

A purchase is treated as reliable supply only while it is **properly active**.

The Department does not rely on contracts that are:

- Pending.
- Suspended.
- Failing because of a missing connection.
- Failing because the paying city cannot afford them.

**Local production is never reduced before the replacement purchase is in force.**

There is no artificial rule requiring an active purchase to have a particular number of days remaining.

The Department considers it usable until the game changes its status.

#### Honoring existing sales

An active power or water sale is treated as an obligation.

The Department includes its full quantity when calculating how much capacity your city needs.

The same principle applies to garbage contracts and the sanitation capacity required to support them.

A sale may be reduced or cancelled during a later review, but **while it remains in force, the local service plan must account for it**.

#### Respecting the game's contract rules

The Department only uses:

- Contract offers that SimCity 4 actually makes available.
- Quantities permitted by the game's contract system.
- The game's own functions for creating, resizing and cancelling contracts.

It never directly modifies neighboring cities or forces a contract that the game would reject.

**Suspended contracts are never modified.**

#### Stopping safely when something goes wrong

If a technical error occurs during a contract operation, the Department stops automatic contract management for the rest of the current city session.

This can happen when:

- The game refuses a contract operation unexpectedly.
- A contract enters an unexpected state after being changed.
- The price reported by the game differs too much from the Department's calculation.
- The resulting contract information cannot be verified reliably.

**The power, water and sanitation funding managers can continue operating.**

They simply use the existing regional contracts as fixed commitments.

If you reload the city or disable contract management while a transaction is unfinished, the Department abandons further actions for that transaction.

A transaction that only awaits the game's accounting updates may still be observed until those values settle, without making additional contract calls.

#### One contract change at a time

For each resource, the Department applies **one contract change at a time**.

It then verifies the resulting contract state and the game's regional and financial totals before making another change.

This prevents several unverified contract operations from interfering with one another.

#### What happens to the original game's advisor messages?

While the Department manages regional contracts, the original game's neighbor-deal advisor messages are suppressed by an included resource file.

These include:

- New contract offers.
- Contract renewal messages.
- Contract default warnings.

The Department handles the corresponding situations through its own management system.

When a genuine shortage cannot be resolved automatically, it can issue its own notice.

See [Notices](safety.md#notices).

**Important:** The original game's messages are suppressed by a resource file loaded when SimCity 4 starts. Disabling automatic contract management in the options does not immediately restore those messages. The procedure for restoring them is explained under [Known limitations](#neighbor-deals-limitations).

<a id="neighbor-deals-settings"></a>
### 5. Settings

Regional contract management can be configured in **Play Options → Department of Efficiency**, under the **Services** page.

| Setting | Default | What it does |
|---|---|---|
| [Manage neighbor deals](options.md#opt-deals) | On | Enables automatic regional contract management. |
| [Manage Power Deals](options.md#opt-tradepower) | On | Allows the Department to buy and sell power. |
| [Manage Water Deals](options.md#opt-tradewater) | On | Allows the Department to buy and sell water. |
| [Manage Garbage Deals](options.md#opt-tradegarbage) | On | Allows the Department to send garbage to neighbors or receive theirs. |
| [Buy from one city to sell to another](options.md#opt-arbitrage) | **Off** | Allows buying and selling the same resource in one plan when economically beneficial. |
| [Power reserve](options.md#opt-powerreserve) | 20% | Protects local power supply. |
| [Water reserve](options.md#opt-waterreserve) | 10% | Protects local water supply. |
| [Sanitation reserve](options.md#opt-sanitationreserve) | 2% | Provides extra garbage disposal capacity. |

#### How do the switches interact?

**Manage neighbor deals** is the main switch for regional contracts.

When disabled:

- The Department does not request new contract offers.
- It does not create, resize or cancel contracts.
- Existing contracts remain in force and are included in local funding calculations.

The three resource switches allow you to manage power, water and garbage independently.

**You can also manage contracts without managing local funding.**

For example:

| Settings | Result |
|---|---|
| **Manage Power Deals: On** / **Manage power: On** | The Department can coordinate power contracts and plant funding. |
| **Manage Power Deals: On** / **Manage power: Off** | Contracts can be managed, but power plant funding stays unchanged. |
| **Manage Power Deals: Off** / **Manage power: On** | Plant funding can be optimized, but existing power contracts remain unchanged. |
| **Both Off** | Neither power contracts nor plant funding are automatically changed. |

The same independence applies to water and sanitation.

**If contract management is enabled but local funding management is disabled**, the Department can only use the production or disposal capacity already available at the current funding.

It does not increase funding to support a new contract.

**If local funding management is enabled but contract management is disabled**, existing purchases and sales are treated as fixed commitments.

The Department can still monitor the resource and warn you about persistent shortages.

Disabling **Manage neighbor deals** preserves the individual resource switches and arbitrage setting. Their values apply again when the main switch is re-enabled.

#### Advanced setting: minimum contract benefit

You can configure a minimum monthly benefit for optional contract decisions.

This setting is available through the INI file, not the in-game options window.

| INI setting | Default | Allowed range |
|---|---:|---:|
| `MinContractBenefit` | §0/month | §0–§100,000/month |

File: `DepartmentEfficiencyUtility.ini`

When set to zero, **any strictly positive economic benefit is sufficient**.

For example, a contract that improves the monthly economic result by §1 may qualify.

If you set:

```ini
MinContractBenefit=20
```

Optional contract creations and quantity changes must produce at least **§20 per month** of additional benefit.

This setting can be useful if you prefer fewer contract changes in exchange for ignoring small potential savings.

**The threshold never blocks a change required to maintain essential services.**

SimCity 4 does not charge a separate fee simply for creating, changing or cancelling a regional contract.

<a id="neighbor-deals-examples"></a>
### 6. Worked examples

The following examples demonstrate how the Department evaluates regional contracts.

The **game's contract formulas and prices are calculated using its actual rules**.

However, the local operating costs shown in these examples are illustrative values representing what the appropriate funding optimizer could calculate for a particular city.

#### Example 1: Selling power without sacrificing the reserve

Consider two neighboring cities.

**Your city (City A)**

- Power demand: **10,000 units**.
- Game power requirement: **10,300 units**.
- Power reserve: **20%**.
- Current power production: **14,000 units**.
- Current power plant expenses: **§1,400 per month**.

**Neighboring city (City B)**

- Power production: **3,000 units**.
- Power requirement: **6,000 units**.
- Treasury: **§250,000**.

#### Step 1: What does SimCity 4 allow?

City A has a surplus of:

```math
14000-10300=3700
```

City B's buying allowance is:

```math
2.5\times6000-3000=12000
```

The game's maximum contract quantity is therefore:

```math
Q_{\max}=\min(3700,12000)=3700
```

City B meets the game's proposal requirements.

The minimum contract price is approximately **§110 per month** for 1,000 units, and City B's treasury comfortably exceeds the required five years of payments.

**SimCity 4 is willing to offer an power purchase from City A.**

#### Step 2: What does the Department require?

The Department applies its own 20% power reserve.

Without a sale:

```math
N(0,0)=\max(12000,10300)=12000
```

City A must maintain at least **12,000 units of local generating capacity**.

Every unit sold increases that requirement by one unit.

The Department evaluates candidate sale quantities using the proposal increment derived from the neighbor's limit:

```math
\text{step}=\max(12000/10,1000)=1200
```

It therefore considers quantities such as 1,200, 2,400 and 3,600 units.

Larger quantities can also be considered as *latent sales*, meaning they would require additional local capacity before the game could legally accept them.

#### Step 3: Which quantity is most profitable?

| Power sold | Monthly income | Illustrative local cost | Extra local cost | Net profit |
|---:|---:|---:|---:|---:|
| 0 | §0 | §1,250 | — | §0 |
| 1,200 | §132 | §1,330 | §80 | +§52 |
| **2,400** | **§264** | **§1,420** | **§170** | **+§94** |
| 3,600 | §396 | §1,560 | §310 | +§86 |
| 4,800 (latent) | §527 | §1,720 | §470 | +§57 |

**The most profitable choice is to sell 2,400 units.**

This provides an estimated improvement of **§94 per month**.

However, the sale requires:

```math
12000+2400=14400
```

**14,400 units of local generating capacity.**

City A currently produces only 14,000.

#### Step 4: How is the plan applied?

The Department does not immediately sign the sale.

First, it increases power plant funding to reach the required capacity of 14,400 units.

After the game recognizes the additional production, the contract is reconsidered during a later review.

With 14,400 units of production, the game's available surplus becomes:

```math
14400-10300=4100
```

The planned 2,400-unit sale is now within the permitted maximum, and it can be written if the refreshed plan still selects it.

**The Department increases production before signing the sale, protecting the city's reserve throughout the transition.**

Without the Department's reserve protection, the player could have sold the full 3,700 units permitted by the game's original calculations, leaving no reserve margin.

#### Example 2: Buying power and saving more through the reserve

Consider another city, **City C**.

- Power demand: **10,000 units**.
- Game requirement: **10,300 units**.
- Current production: **12,500 units**.
- Power reserve: **20%**.

Its neighboring supplier has:

- Production: **30,000 units**.
- Requirement: **20,000 units**.
- Average unit cost: **§0.08**.

The supplier has 10,000 units of surplus.

The buyer's allowance is:

```math
2.5\times10300-12500=13250
```

The game's maximum is therefore **10,000 units**, and the proposal quantities increase in steps of 1,000.

The Department's own purchase limit is also 10,000 units.

#### Step 1: How much local capacity is needed?

Without purchases, the city's 20% reserve requires:

```math
N(0,0)=12000
```

Purchasing power reduces the amount that must be produced locally.

**Each unit purchased can reduce the local capacity requirement by 1.2 units**, while the reserve-based requirement remains binding.

#### Step 2: Compare possible purchases

| Units bought | Required local capacity | Illustrative local cost | Local saving | Contract price | Net saving |
|---:|---:|---:|---:|---:|---:|
| 0 | 12,000 | §1,250 | — | — | §0 |
| 1,000 | 10,800 | §1,160 | §90 | §89 | +§1 |
| **2,000** | **9,600** | **§1,060** | **§190** | **§179** | **+§11** |
| 3,000 | 8,400 | §975 | §275 | §267 | +§8 |
| 4,000 | 7,200 | §900 | §350 | §356 | −§6 |

**The cheapest overall arrangement is to buy 2,000 units.**

The purchase costs **§179 per month**, but allows a reduction of **§190 in local power plant expenses**.

Estimated net saving:

```math
190-179=11
```

**§11 per month.**

#### Step 3: Apply the changes safely

The Department first creates and verifies the purchase.

It then reduces power plant funding during the appropriate later execution phase.

**What if the minimum benefit is set to §20?**

With:

```ini
MinContractBenefit=20
```

The optional contract is not created.

Its §11 monthly benefit is below the configured threshold.

This setting allows you to avoid small economic adjustments if you prefer a more stable set of contracts.

#### Example 3: Why a contract cannot always be resized to any amount

Suppose your city already has an power sale with:

- Current amount: **3,088 units**.
- Contract increment: **1,544 units**.
- Game maximum: **15,442 units**.

The game's selectable quantities follow the increment:

1,544; 3,088; 4,632; 6,176; and so on, up to 15,440.

**A contract quantity of 6,000 units is not available.**

The nearest permitted quantity above it is **6,176 units**.

The Department respects those legal quantity choices rather than submitting an arbitrary number to the game.

#### Example 4: Accepting garbage from another city

A neighboring city offers to pay you to dispose of **20,000 garbage units per month**.

The game normally delivers:

```math
20000\times0.10=2000
```

**2,000 units per simulated day**, for approximately ten days.

The Department evaluates whether your sanitation facilities can handle these additional 20,000 units.

It considers:

- Your city's own garbage production after recycling.
- Your configured sanitation reserve.
- Garbage already uncollected.
- Existing imports and exports.
- Incinerator and Waste-to-Energy treatment capacity.
- Available landfill intake.
- The daily delivery schedule of every contract.

**The contract is accepted only if the projected month ends without uncollected garbage.**

The revenue must also justify any additional sanitation funding required to handle the incoming waste.

Finally, the total garbage received must remain below the game's allowed limit.

In the example from Section 1, that limit was approximately **541,667 units**.

**A contract that pays well is still rejected if your city cannot safely handle the additional garbage.**

<a id="neighbor-deals-limitations"></a>
### 7. Known limitations

The Department can manage regional contracts automatically, but it remains subject to SimCity 4's original rules and available information.

#### Neighboring cities and regional information

- **Neighbor records are snapshots.** SimCity 4 does not simulate neighboring cities while you play the current city. The Department uses the regional figures last saved for those neighbors, which may be outdated.
- **The Department never changes neighboring cities.** It does not load them, modify their buildings or update their saved information.
- **Contract dates remain controlled by SimCity 4.** The game handles contract start dates, expiration and renewal. The Department does not attempt to predict or modify them. When a contract ends or becomes suspended, its new state is considered at the next review.
- **New sales require an existing offer.** The Department may increase your city's production to support a larger sale to a neighbor already offering that resource. It cannot make a completely new buyer appear when the game has no offer from that neighbor.

#### Applying contract changes

- **Only one step is applied per review.** Each resource performs at most one contract change or local funding step during a review. A major reorganization can therefore take several reviews.
- **Every contract price is verified.** If the game's authoritative price differs from the Department's model beyond the permitted tolerance, contract management stops for the rest of the city session.
- **Suspended contracts cannot be changed.** The game controls their resolution, and the Department must work around them.
- **Large garbage contract searches have limits.** If the Department cannot exhaustively evaluate the legal combinations because there are more than five million on one side, it does not make a speculative contract change. Mixed buy-and-sell searches also have their own limits, described earlier.

#### Original neighbor-deal advisor messages

The Department includes a resource file that suppresses SimCity 4's original neighbor-deal advisor messages.

This file is:

`DepartmentEfficiencyDealAdvice.dat`

**SimCity 4 loads resource files when the game starts.**

Therefore, disabling the Department's contract management options does not immediately restore the original advisor messages.

If you want to manage contracts manually and restore the original messages:

1. Close SimCity 4.
2. Remove `DepartmentEfficiencyDealAdvice.dat` from the game's Plugins folder.
3. Restart the game.

This also restores original messages such as *Deal Defaulted*.

The Department records a warning in its log when the resource file and current management settings do not match.

#### Pending proposals

When the Department requests contract offers, it uses the same game operation as opening the Neighbor Deals window.

This may cause pending proposals for other neighboring cities to appear in the game's contract lists.

As in the original game, **pending proposals do not generate income or expenses** until they become active contracts.

**The Department works within the game's contract system. It can optimize the available choices, but it cannot create resources, connections, eligibility or reliable information that SimCity 4 does not provide.**

[Back to the contents](index.md#contents) | [Back to the README](../../README.md)

---

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
