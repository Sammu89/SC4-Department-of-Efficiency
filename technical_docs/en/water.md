**English** | [Português](../pt/water.md) | [Français](../fr/water.md) | [Español](../es/water.md)

[Back to the contents](index.md#contents) | [Back to the README](../../README.md)

<a id="water"></a>
## C. Water

The Department automatically manages your city's **water supply budget**, ensuring that enough water is available for your buildings while keeping operating costs as low as possible.

It considers:

- How much water your city needs.
- Your chosen safety reserve (**10% by default**).
- The water your city buys from or sells to neighboring cities.
- The production capacity of your water facilities.
- The condition of your water pipe network.

Unlike power, where each power plant has its own funding setting, **the entire water supply shares a single budget percentage**.

The Department adjusts this percentage to provide enough water without unnecessary spending.

It can also protect your pipes from deterioration caused by insufficient funding.

Regional water contracts are included in the calculations. Their automatic management is explained in [Neighbor Deals](deals.md#neighbor-deals).

<a id="water-vanilla"></a>
### 1. How water works in SimCity 4

#### One budget for the entire water supply

SimCity 4 manages water funding through a single department budget.

Your city may have several water facilities, including pumps, water towers and treatment plants. Each facility has its own maintenance cost, but **all facilities receive the same funding percentage**.

For example, if you set water funding to 80%, that percentage applies to the entire water department.

You cannot fund one pump at 100% and another at 60%, as you can with power plants.

The game normally allows water funding up to **120%**, although a lower maximum may apply if reported by the game.

Unlike power, water has no built-in minimum acceptable funding percentage or strike mechanism.

#### Water demand, production and consumption

The game tracks several water-related values:

| Term | What it means |
|---|---|
| **Demand** | The amount of water your city's buildings need. |
| **Production capacity** | The amount of water your own facilities can supply at their current funding. |
| **Consumption** | The water actually delivered to buildings. |
| **Imports** | Water purchased from neighboring cities. |
| **Exports** | Water sold to neighboring cities. |
| **Watered buildings** | Buildings currently receiving water. |
| **Unwatered buildings** | Buildings without water service. |
| **Distressed pipes** | Pipes that have deteriorated. |
| **Burst pipes** | Pipes that have failed. |

**How do regional water contracts affect your supply?**

In our observations, SimCity 4 uses imported water before drawing on local production.

This means that buying water from a neighboring city can reduce how much your own facilities need to produce.

Selling water has the opposite effect: **your city must have enough capacity to supply both its own buildings and its export contracts**.

The Department takes these contracts into account when calculating the required funding.

**An important detail when opening a city**

Immediately after loading a city, the game may report zero watered buildings and zero unwatered buildings.

This does not necessarily mean that every building has water or that the city has no water demand.

It usually means the water simulation has not yet updated its information.

The Department must therefore distinguish between missing initial readings and genuine values before making funding decisions.

<a id="water-efficiency-curve"></a>
#### How funding affects water production

In SimCity 4, reducing water funding also reduces the amount of water your facilities can produce.

However, **production does not decrease at the same rate as funding**.

The game uses the following efficiency curve:

| Water funding | Production efficiency |
|---:|---:|
| 0% | 0% |
| 50% | 30% |
| 90% | 80% |
| 100% | 100% |
| 120% | 110% |

**What does this mean?**

- At **50% funding**, your city has only **30% of its normal water production capacity**.
- At **90% funding**, it has **80% of normal capacity**.
- At **100% funding**, it has full normal capacity.
- At **120% funding**, it has **110% of normal capacity**.

Funding above 100% therefore provides additional water, but at a higher cost per unit of capacity.

Likewise, reducing funding too far can remove much more production capacity than you might expect.

**How are intermediate percentages calculated?**

Between the game's control points, efficiency changes in a straight line.

For example, between 90% and 100% funding, efficiency increases from 80% to 100%.

At 95% funding, production efficiency is therefore 90%.

**Efficiency curve interpolation**

```math
y(x) = y_i + \frac{x-x_i}{x_{i+1}-x_i}(y_{i+1}-y_i)
\qquad \text{for } x_i \le x \le x_{i+1}
```

Where:

- **x** is the chosen funding percentage.
- **(xᵢ, yᵢ)** and **(xᵢ₊₁, yᵢ₊₁)** are the two surrounding points on the curve.
- **y(x)** is the resulting production efficiency.

Outside the curve's defined range, the nearest endpoint value is used.

**Examples of water production efficiency**

| Funding (%) | 30 | 50 | 60 | 70 | 75 | 78 | 80 | 90 | 95 | 100 | 105 | 110 | 120 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| Efficiency (%) | 18 | 30 | 42.5 | 55 | 61.25 | 65 | 67.5 | 80 | 90 | 100 | 102.5 | 105 | 110 |

**Why is this important for the Department?**

At 100% funding, water production is most efficient in terms of capacity obtained per simoleon spent.

But that does not mean every city needs 100% funding.

Imagine a city that produces far more water than it consumes. Reducing the water budget may still provide enough water while saving money.

For example:

| Funding | Production capacity | Cost relative to 100% |
|---|---:|---:|
| 50% | 30% | 50% |
| 80% | 67.5% | 80% |
| 100% | 100% | 100% |
| 120% | 110% | 120% |

**The Department looks for the lowest funding level that can meet the city's water needs and safety reserve**, while respecting its other protections.

Unlike power, it does not need to compare different funding combinations for individual facilities, because there is only one water budget percentage.

See [How the Department chooses funding](#water-algorithm) for the calculation.

#### Water production does not update immediately

There is an important delay in SimCity 4.

**When water funding changes, the game's reported production capacity updates at the next simulated month, not immediately.**

For example, during testing, a city had a production capacity of **241,520 units** at 100% funding.

After reducing funding to 95%, the game's reported capacity became **217,368 units** at the following monthly update.

That is exactly 90% of the original capacity, matching the efficiency curve.

This delay matters because the Department must not mistake an unchanged capacity reading immediately after a funding adjustment for proof that the adjustment had no effect.

#### How is the monthly water bill calculated?

The total water department expense depends on two things:

1. The combined maintenance costs of all water facilities.
2. The current water funding percentage.

The game charges these costs in proportion to funding.

**Monthly water expense (base game)**

```math
X(f) = \mathrm{round}\left(I\times\frac{f}{100}\right)
```

Where:

- **I** is the combined monthly maintenance cost of all water facilities at 100% funding.
- **f** is the water department funding percentage.
- **X(f)** is the estimated monthly water expense.

**Example: a city with a normal water budget of §2,000 per month**

| Funding | Monthly expense | Monthly saving |
|---|---:|---:|
| 100% | §2,000 | — |
| 90% | §1,800 | §200 |
| 78% | §1,560 | §440 |
| 50% | §1,000 | §1,000 |

At 78% funding, the city saves **§440 per month** compared with 100%.

However, that funding provides only **65% of normal production capacity**.

The Department must therefore check whether 65% capacity is enough to satisfy demand, the configured reserve and any water exports.

The formula rounds expenses to whole simoleons. Because of the game's internal calculations, the actual displayed expense may differ by one or two simoleons.

<a id="water-pipe-curve"></a>
#### How funding affects water pipes

Water funding does not only control production. It also affects the **condition of your pipe network**.

In SimCity 4, pipes can gradually deteriorate when the water department receives insufficient funding.

The game uses a separate curve to determine whether pipes deteriorate, remain stable or repair themselves.

**Pipe deterioration and repair curve (base game)**

| Water funding | Effect on pipes |
|---:|---|
| 0% | Deterioration (−0.010) |
| 20% | Deterioration (−0.005) |
| 50% | No deterioration or repair (0) |
| 100% | Repair (+0.020) |
| 110% | Faster repair (+0.050) |

The curve follows the same linear interpolation rule used for production efficiency.

At 120% funding, the repair value remains +0.050, the same as at 110%.

**The most important threshold is 50% funding.**

- **Below 50%:** Pipes gradually deteriorate.
- **At 50%:** Their condition remains stable.
- **Above 50%:** The pipe network gradually repairs itself.

This does **not** mean that pipes immediately burst when you reduce funding below 50%.

Deterioration happens over time.

As pipes become damaged, the game may report them as *distressed*. If their condition deteriorates far enough, they can become *burst pipes*.

Restoring funding to 50% stops further deterioration caused by the funding curve. Raising it above 50% allows the network to recover.

**How quickly do pipes deteriorate or recover?**

The following table shows the game's curve at several funding levels:

| Funding (%) | 0 | 10 | 20 | 30 | 35 | 40 | 50 | 60 | 75 | 78 | 90 | 100 | 105 | 110 | 120 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| Pipe curve | −0.0100 | −0.0075 | −0.0050 | −0.0033 | −0.0025 | −0.0017 | 0 | +0.0040 | +0.0100 | +0.0112 | +0.0160 | +0.0200 | +0.0350 | +0.0500 | +0.0500 |

A more negative value means a stronger deterioration effect. A larger positive value means a stronger repair effect.

For example, at 20% funding, the deterioration rate represented by the curve is approximately twice the rate at 30%.

At 110% funding, the repair value is 2.5 times the value at 100%.

**How does the Department use this information?**

The Department recognizes whether a funding level causes:

- Pipe deterioration.
- Stable pipe condition.
- Pipe repair.

When pipe protection is enabled, this information helps it avoid funding reductions that would put the network at risk.

**An important limitation:** The Department does not calculate the exact number of simulated days it would take for an individual pipe to deteriorate or burst. Its model uses the direction of the curve, not a predicted failure date.

This allows it to protect the network without relying on an unverified estimate of how quickly each pipe wears out.
<a id="water-problem"></a>
### 2. The management problem

Managing water funding manually may seem straightforward, since the entire city shares a single budget percentage. However, several factors make it more complicated.

- **Unnecessary spending:** A city may produce far more water than it needs, particularly after building a new pump or treatment plant. Keeping funding at 100% can waste hundreds of simoleons every month.
- **Risk of water shortages:** Reducing funding too far can leave buildings without water. Worse, the game's production capacity only updates at the next simulated month, so the consequences of a funding change may not be immediately visible.
- **Regional contracts:** Buying water reduces the amount your city needs to produce locally, while selling water increases it.
- **Pipe deterioration:** Funding below 50% causes water pipes to deteriorate gradually. The repair costs and consequences of damaged pipes are not reflected directly in the monthly water budget.
- **Connection problems:** Buildings may be without water even when the city has enough production capacity. In these cases, increasing funding will not help. The player must correct the pipe connections.

**The Department solves this by calculating the water capacity your city actually needs, finding the lowest suitable funding level, and protecting the pipe network when that option is enabled.**

<a id="water-algorithm"></a>
### 3. How the Department chooses water funding

The Department does not simply lower the water budget whenever production exceeds demand.

Instead, it follows four steps:

1. Calculate how much water the city must produce locally.
2. Estimate how much water can be produced at each funding percentage.
3. Identify which funding levels are safe and affordable.
4. Choose the cheapest suitable percentage and apply it in the correct order.

Every calculation uses the city's latest available information.

<a id="water-required-capacity"></a>
#### Step 1: How much water does the city need?

The Department first calculates the minimum water production capacity your city must maintain.

It considers:

- Your city's current water demand.
- The water reserve you have chosen (**10% by default**).
- Water purchased from neighboring cities.
- Water sold to neighboring cities.

**The reserve applies only to the water your city must produce itself.**

For example, if your city needs 20,000 units of water and produces everything locally, a 10% reserve means it should maintain capacity for at least **22,000 units**.

If the city purchases some of that water from a neighbor, the reserve applies only to the remaining local demand.

**Required local water capacity**

```math
L = \left\lceil R \times \max(0,D-B) \right\rceil + S
\qquad
R = 1+\frac{r}{100}
```

Where:

- **D** is the city's total water demand.
- **B** is the water purchased from neighboring cities that can reliably be used.
- **S** is the water sold to neighboring cities.
- **r** is the configured Water reserve, 10% by default.
- **R** is the reserve multiplier (1.10 for a 10% reserve).
- **L** is the required local water production capacity.

The result is rounded up to a whole water unit.

**Example: a city needing 20,000 units**

| Situation | Required local capacity |
|---|---:|
| No water imports or exports | 22,000 units |
| Importing 5,000 units | 16,500 units |
| Exporting 5,000 units | 27,000 units |

With no regional contracts:

```math
L = \lceil 20000 \times 1.10 \rceil = 22000
```

With a purchase of 5,000 units:

```math
L = \lceil (20000-5000)\times1.10\rceil = 16500
```

With a sale of 5,000 units:

```math
L = \lceil 20000\times1.10\rceil+5000=27000
```

Notice that **the reserve is not added to exported water**. Exports must be covered, but they do not receive an additional safety margin.

Likewise, the Department does not purchase extra water just to create a reserve on imported supply.

**Which water purchases can be trusted?**

Only purchases that are confirmed as active are considered reliable.

Pending, suspended or failing contracts are not counted as available water.

If the Department also manages regional water contracts, it calculates funding using the purchases and sales included in its contract plan. The two decisions are coordinated rather than made independently.

See [Neighbor Deals](deals.md#neighbor-deals).

#### Step 2: How much water can the city produce at each funding level?

Once the required capacity is known, the Department examines the city's current water production.

It then estimates how much water the existing facilities could produce at different funding percentages.

Because all water producers share the same funding setting, this calculation is much simpler than power optimization.

**Projected water production capacity**

```math
C(v)=\left\lfloor C_{\text{now}}\times\frac{\eta(v)}{\eta(f_{\text{now}})}\right\rfloor
```

Where:

- **Cnow** is the production capacity currently reported by the game.
- **fnow** is the current water funding percentage, read as stored by the game.
- **v** is a possible new funding percentage.
- **η** is the game's water production efficiency curve.
- **C(v)** is the estimated capacity at the proposed funding, rounded down.

In plain English, **the Department uses the current production capacity and the game's efficiency curve to predict how much water a different funding level would provide**.

**Example: a city producing 34,000 units**

Suppose the city currently produces 34,000 units at 100% funding.

At 78% funding, the game's efficiency curve provides 65% of normal production capacity.

```math
C(78)=\left\lfloor34000\times\frac{65}{100}\right\rfloor=22100
```

The Department therefore estimates that **78% funding would provide 22,100 units**.

If the city needs 22,000 units, including its reserve, that would be sufficient.

**An important limitation**

This calculation assumes that all water producers respond to department funding according to the standard efficiency curve.

If the current funding produces zero efficiency, such as at 0%, the Department cannot use this formula to estimate alternative capacities reliably. In that situation, it does not make an automatic water funding plan.

<a id="water-funding-choice"></a>
#### Step 3: Which funding levels are allowed?

The Department does not consider every possible funding level without restrictions.

The available choices depend on your settings and the current condition of the water network.

**Minimum funding**

| Protect water pipes | Minimum for new funding choices |
|---|---:|
| **On (default)** | **50%** |
| Off | 0% |

With pipe protection enabled, the Department normally keeps funding at or above the point where the game's pipe deterioration curve becomes neutral.

At 50% funding, pipes neither deteriorate nor repair themselves because of the department funding.

Above 50%, the network gradually repairs itself.

**Maximum funding**

The normal maximum is **120%**, or a lower value if the game reports a lower department limit.

**What if you have manually set funding below 50%?**

The Department always includes the current funding as a possible choice, even when it lies outside its normal search range.

For example, if you manually set water funding to 40%, that existing level is not automatically discarded from consideration.

However, with pipe protection enabled, the Department will not choose a **new reduction** that causes pipe deterioration.

This distinction matters: **protecting pipes does not mean the Department automatically raises every existing budget below 50%.** The funding choice still follows its capacity requirements and safety rules.

#### When does the Department refuse to reduce funding?

Even if a lower percentage would save money, the Department refuses a reduction when any of the following conditions applies:

**1. The city has a genuine water shortage**

If buildings are without water and local production capacity is below demand, the Department does not reduce funding further.

**2. Water pipes are already damaged**

When **Protect water pipes** is enabled, the Department refuses reductions if the game reports any distressed or burst pipes.

**3. The reduction would cause pipe deterioration**

With protection enabled, a funding reduction is rejected if the game's pipe curve would become negative.

These restrictions apply to **funding reductions**.

They do not prevent the Department from increasing funding to improve water supply.

#### Step 4: Choosing the cheapest funding

After removing unsafe options, the Department compares the remaining funding levels.

Its goal is to select **the lowest monthly expense that still provides the required water capacity**.

**Funding optimization**

```math
f^* = \arg\min_{v\in V,\ C(v)\ge L} X(v)
```

Where:

- **V** is the set of allowed funding percentages.
- **C(v)** is the estimated water capacity at funding **v**.
- **L** is the required local capacity from Step 1.
- **X(v)** is the monthly operating expense at funding **v**.
- **f\*** is the chosen funding percentage.

Because both water capacity and operating expense increase with funding, **the cheapest solution is normally the lowest allowed percentage that provides enough water**.

If two choices have the same cost, the Department prefers to keep the current funding.

**Example: 34,000 units at full funding**

Return to the previous example:

- Current capacity at 100%: 34,000 units.
- Required capacity, including reserve: 22,000 units.
- Pipe protection: On.

| Funding | Projected capacity | Result |
|---|---:|---|
| 77% | 21,675 | Not enough |
| **78%** | **22,100** | **Enough** |
| 80% | 22,950 | Enough, but more expensive |
| 100% | 34,000 | Unnecessary excess capacity |

**The Department chooses 78% funding**, provided none of the safety conditions prevents the reduction.

If the department normally costs §2,000 per month, the new cost is approximately **§1,560**, saving **§440 per month**.

**What if even maximum funding is insufficient?**

The Department selects the allowed funding level that provides the greatest available capacity, normally 120%.

If several choices provide the same maximum capacity, it prefers the cheaper one.

It does not reduce funding when the city already lacks sufficient capacity.

When regional water contract management is enabled, the Department can also attempt to purchase water to cover the remaining shortage.

If the problem persists, the player may receive an advisor notice.

See [Neighbor Deals](deals.md#neighbor-deals) and [Advisor Notices](safety.md#notices).

#### How are funding changes applied?

The Department applies its chosen funding percentage directly, without gradually moving the budget slider.

However, **the order of funding and contract changes matters**.

- **Funding increases happen first**, before changing water contracts.
- **Funding reductions happen only after** the required regional contract changes have been applied.

For example, suppose the Department wants to reduce local water production because it has found a cheaper purchase from a neighboring city.

It first ensures the regional contract changes are in place, then reduces the water budget.

This avoids cutting local production before the alternative supply is arranged.

Each review performs at most one coordinated stage for a resource. Any remaining changes are reconsidered during a later evaluation using fresh game readings.

**The purpose is to keep the transition safe, not just the final funding level.**

<a id="water-cadence"></a>
#### How often does the Department check water funding?

The Department regularly checks your water supply and can react earlier when significant changes occur.

All timings refer to simulated game time.

| When | What happens |
|---|---|
| **When a city opens** | Performs the first water evaluation as soon as the game's readings are ready and consistent. |
| **Every simulated day** | Checks for serious shortages and significant changes in water demand, production or regional contracts. |
| **Every 10 simulated days** | Performs a regular water funding review, counted from the last full review. |
| **At the start of every simulated month** | Performs a water review. |
| **When an important change is detected** | Recalculates the water plan during the daily check, without waiting for the next scheduled review. |

Each completed review resets the regular review interval. If an evaluation has just taken place, the Department does not immediately repeat it just because another scheduled check was due.

**What triggers an early evaluation?**

The Department's daily checks look for two situations.

**1. A genuine water supply shortage**

A shortage exists when the city consumes less water than it needs **and** the total usable supply is insufficient.

**Regional water deficit**

```math
W < D \qquad\text{and}\qquad C+U-S<D
```

Where:

- **W** is water actually consumed.
- **D** is the city's water demand.
- **C** is local production capacity.
- **U** is water actually delivered through purchase contracts.
- **S** is water sold to neighboring cities.

For example, if a city needs 20,000 units but its total usable supply is only 15,000, the Department can recalculate funding without waiting for the next regular review.

But imagine the city needs 20,000 units and has 25,000 units available, yet some buildings remain without water.

In that situation, the problem is probably within the city's pipe network rather than insufficient production.

**Increasing the budget would not necessarily help**, so the Department records the connection problem instead of triggering a funding review for that reason alone.

A shortage already evaluated does not repeatedly trigger new reviews unless the supply situation changes.

**2. A significant change in water supply or demand**

The Department also recalculates funding when any of the following changes by at least **1%** compared with the previous review:

- Water demand.
- Local production capacity.
- Contracted water imports.
- Contracted water exports.

**Significant change threshold**

```math
|x-x_0|\ge0.01\times\max(1,|x_0|)
```

Where:

- **x** is the latest reading.
- **x₀** is the value recorded at the previous review.

For example, if demand rises from 20,000 to 20,300 units, that is a 1.5% increase.

The Department can therefore recalculate funding during its next daily check.

**Unchanged readings do not trigger unnecessary evaluations.**

This keeps the system responsive without constantly recalculating the same situation.

<a id="water-regional"></a>
#### How regional water contracts affect funding

Water purchases and sales are considered when calculating how much water your own facilities must produce.

The rules are straightforward:

- **Purchases reduce local demand.** Only reliably active purchases count as available water.
- **Purchases cannot cover more than the city's total demand** in the local capacity calculation.
- **Sales increase the local production requirement.** The city must provide enough water for both its own needs and its export obligations.
- **Existing sales are treated conservatively.** When the Department is not managing contracts, it counts every listed sale as an obligation, regardless of its status, or the game's reported exports if that number is greater.
- **Sales are not maintained during a genuine water deficit.** The contract planner prioritizes the city's own supply.

#### How does the Department decide whether a contract is profitable?

A water contract is not automatically worthwhile just because another city offers to buy or sell water.

The Department compares the contract's price with the additional operating cost or saving it would create.

For example:

- **Selling water:** The Department calculates how much extra funding would be required to produce the water being sold.
- **Buying water:** It calculates how much local funding could be saved by relying on the purchased water.

**Marginal cost of water**

```math
\Delta(q)=X^*(L+q)-X^*(L)
```

Where:

- **L** is the current required local water capacity.
- **q** is the contract quantity, positive for a sale and negative for a purchase.
- **X\*(n)** is the lowest monthly water expense needed to provide at least **n** units.
- **Δ(q)** is the change in monthly operating expenses.

**For a water sale**, the Department compares the money received with the additional local production expense.

A sale is worthwhile only if the neighbor pays more than the extra cost.

**For a water purchase**, the Department compares the contract price with the savings made by reducing local production.

A purchase is worthwhile only if it costs less than those savings.

**Example**

Suppose producing an additional 5,000 units of water requires raising the department's monthly budget by §300.

A neighboring city offers to buy those 5,000 units.

| Monthly payment from neighbor | Economic result |
|---|---|
| §200 | Not worthwhile: §100 loss |
| §300 | Break-even |
| §400 | Worthwhile: §100 gain |

This is why the Department considers **the actual change in production costs**, rather than assuming that every regional contract is beneficial.

The full contract pricing and management rules are explained in [Neighbor Deals](deals.md#neighbor-deals).

#### What if automatic water or contract management is disabled?

The two functions can operate independently.

| Settings | What happens |
|---|---|
| **Manage water: On** / **Manage Water Deals: On** | The Department can optimize both water funding and regional water contracts. |
| **Manage water: On** / **Manage Water Deals: Off** | Water funding is optimized using your existing contracts, which the Department leaves unchanged. |
| **Manage water: Off** / **Manage Water Deals: On** | Water funding stays at its current level, but regional water contracts can still be managed. |
| **Both Off** | The Department does not automatically change water funding or water contracts. |

Automatic regional water contract management also requires the main **Manage neighbor deals** option to be enabled.

<a id="water-farm-icon"></a>
#### A small visual improvement: agricultural water warnings

The Department also makes a small adjustment to the game's interface.

**It hides the original "no water" warning icon over agricultural lots.**

This change affects only the warning icon displayed above farms.

It does **not**:

- Supply water to farms.
- Change their water demand.
- Change the game's decisions about agricultural buildings.
- Change the water system itself.
- Hide water warnings from residential, commercial or industrial buildings.

The adjustment is applied when SimCity 4 starts and is independent of the Department's management settings.

It only applies to the supported game version. On other versions, the Department leaves this interface behavior unchanged.

<a id="water-safety"></a>
### 4. Safety conditions

The Department follows several safety rules to avoid leaving your city without water or damaging its pipe network.

#### Before changing water funding

The Department only makes funding changes when it can trust the game's information.

It checks that:

- The water department and its current funding can be read correctly.
- Water demand and production capacity are available, with demand greater than zero.
- The game has updated its watered and unwatered building counts.
- Reported water consumption is consistent with demand.
- Every water producer uses the game's standard funding system.

**If any of these checks fails, the Department leaves water funding unchanged.**

This is particularly important immediately after opening a city, when some water statistics may not yet be available.

For additional technical accuracy, the Department accepts a small difference between consumption and demand:

```math
W \le 1.01 \times D + 1
```

Where **W** is reported water consumption and **D** is demand.

Consumption greater than this limit is treated as an inconsistent reading.

If even one water producer uses an unsupported funding mechanism, automatic water funding management is disabled for that city session rather than risking incorrect calculations.

<a id="water-no-cut"></a>
#### When does the Department refuse to reduce funding?

Even when a lower budget could save money, the Department refuses a reduction in the following situations:

| Situation | Why the reduction is refused |
|---|---|
| **Insufficient water production** | Buildings are without water and production capacity is already below demand. Cutting funding would make the shortage worse. |
| **Damaged pipes** | With **Protect water pipes** enabled, any distressed or burst pipe prevents funding reductions. |
| **Risk of pipe deterioration** | With pipe protection enabled, the Department does not reduce funding below the 50% threshold. |
| **Insufficient capacity after the cut** | The proposed funding would not provide enough water for demand, the reserve and exports. |
| **No suitable funding level** | If even the best available setting cannot meet the requirement, the Department increases funding where possible instead of reducing it. |
| **Unreliable or changed information** | The readings are incomplete, inconsistent or have changed since the calculation was made. |
| **Fixed funding** | Water funding is not under automatic management. |

**Every proposed reduction is checked twice:** once when the plan is calculated and again immediately before the new funding is applied.

This prevents the Department from using an outdated calculation if your city's situation changes.

**What if some buildings are without water, but production is sufficient?**

That is different.

For example, a city might produce 30,000 units while needing only 20,000, yet still have buildings without water because they are disconnected from the pipe network.

In that case, **increasing funding will not solve the connection problem**.

The Department can still reduce the budget, provided that the required capacity and pipe protection rules remain satisfied.

#### What happens if a funding change fails?

Before changing the budget, the Department checks that the current funding still matches the value used in its calculation.

If you have moved the slider manually, the planned change is skipped.

After applying a change, it reads the funding again to verify that the game accepted the new value.

If verification fails:

1. The Department attempts to restore the previous funding.
2. Automatic water funding changes stop for the rest of the current city session.
3. Water funding management can restart when you reopen the city.

**Regional water contracts are not affected by this safety stop.** Their management can continue independently.

<a id="water-settings"></a>
### 5. Settings

Water management can be configured in **Play Options → Department of Efficiency**, under the **Services** page.

| Setting | Default | Range | What it does |
|---|---|---|---|
| [Department active](options.md#opt-master) | On | On / Off | Enables or disables the entire Department. |
| [Manage water](options.md#opt-water) | On | On / Off | Automatically adjusts the city's water department funding. |
| [Water reserve](options.md#opt-waterreserve) | 10% | 10–50% | Sets the minimum spare local water production capacity. |
| [Protect water pipes](options.md#opt-waterpipes) | On | On / Off | Prevents reductions that could cause pipe deterioration or interfere with the recovery of damaged pipes. |
| [Manage neighbor deals](options.md#opt-deals) | On | On / Off | Enables automatic management of regional contracts. |
| [Manage Water Deals](options.md#opt-tradewater) | On | On / Off | Enables automatic water purchases and sales. |
| [Buy from one city to sell to another](options.md#opt-arbitrage) | Off | On / Off | Allows the Department to buy and sell water in the same regional plan. |

#### How does the water reserve work?

The default reserve is **10%**.

For example, if your city must produce 20,000 units locally, the Department aims to maintain capacity for at least **22,000 units**.

The reserve is a minimum, not an exact target.

The Department may keep more capacity when required by the available funding percentages or the pipe protection setting.

Increasing the reserve provides additional spare production capacity but may also increase your monthly water expenses.

#### What does Protect water pipes do?

With this option enabled, the Department:

- Normally uses **50% as the minimum for new funding choices**.
- Never reduces funding to a level that causes pipe deterioration.
- Refuses reductions while the game reports distressed or burst pipes.

**It does not automatically increase funding just to repair pipes.**

If you manually set funding below 50%, the Department may leave it there while production remains sufficient. It will raise funding when necessary to satisfy the water supply requirement, using its normal protected funding range.

If you disable pipe protection, the Department may choose funding as low as **0%**, provided the water supply requirements can still be met.

This can save more money, but allows the pipe network to deteriorate over time.

#### Advanced setting (INI file only)

| Setting | Default | Allowed range | What it does |
|---|---|---|---|
| `WaterCadenceDays` | 10 | 1–30 | Sets the regular water funding review interval, in simulated days. |

For example, setting `WaterCadenceDays=5` makes regular reviews occur every five simulated days.

The daily checks for significant changes and the review at the start of each simulated month remain separate.

<a id="water-examples"></a>
### 6. Worked examples

The following examples show how the Department chooses water funding and how its safety protections affect the result.

All examples use the default **10% water reserve**, unless stated otherwise.

The production capacities and department expenses are illustrative.

#### Example 1: Reducing water funding without creating a shortage

Consider a city with:

- **Water demand:** 20,000 units.
- **Current production capacity:** 34,000 units.
- **Current funding:** 100%.
- **Monthly water expense at 100%:** §2,000.
- **Regional contracts:** None.
- **Damaged pipes:** None.
- **Unwatered buildings:** None.

**Step 1: Calculate the required capacity**

With a 10% reserve:

```math
L = \lceil 20000 \times 1.10 \rceil = 22000
```

The city needs capacity for **22,000 units**.

**Step 2: Find the lowest sufficient funding**

| Funding | Expected capacity | Monthly expense | Result |
|---|---:|---:|---|
| 77% | 21,675 | §1,540 | Not enough |
| **78%** | **22,100** | **§1,560** | **Enough** |
| 100% | 34,000 | §2,000 | Unnecessary excess capacity |

**The Department chooses 78% funding.**

At this level, the city retains enough water for its demand and reserve.

The pipe repair curve is also positive (+0.0112), so the reduction does not cause pipe deterioration.

**Monthly saving: §440.**

#### Example 2: How much does pipe protection cost?

Imagine a city with:

- **Water demand:** 10,000 units.
- **Production capacity at 100%:** 60,000 units.
- **Monthly expense at 100%:** §3,000.

With a 10% reserve, it needs only:

```math
L = 10000 \times 1.10 = 11000
```

The city has much more water production capacity than it needs.

Let's compare the result with pipe protection enabled and disabled.

**With Protect water pipes enabled**

The Department cannot select a new funding target below 50%.

At 50% funding, production efficiency is 30%:

```math
C(50)=60000\times0.30=18000
```

This is more than enough to supply the required 11,000 units.

**The Department chooses 50% funding**, costing **§1,500 per month**.

The pipe network remains stable.

**With Protect water pipes disabled**

The Department can consider lower funding levels.

| Funding | Expected capacity | Result |
|---|---:|---|
| 30% | 10,800 | Not enough |
| **31%** | **11,160** | **Enough** |
| 50% | 18,000 | More than necessary |

Without pipe protection, **31% funding** is sufficient.

The monthly expense falls to **§930**.

However, the pipe deterioration curve becomes negative, approximately −0.0032.

The network will gradually wear down.

**The financial trade-off**

| Setting | Funding | Monthly expense | Effect on pipes |
|---|---:|---:|---|
| **Protection On** | 50% | §1,500 | Stable |
| Protection Off | 31% | §930 | Gradual deterioration |

**Pipe protection costs §570 more per month in this example.**

That additional spending prevents deterioration caused by underfunding.

Whether the saving is worth the long-term damage is a decision the player can make by enabling or disabling the protection.

#### Example 3: Buying and selling water at the same time

Return to the city from Example 1:

- **Water demand:** 20,000 units.
- **Normal production capacity:** 34,000 units.
- **Monthly cost at 100%:** §2,000.

This time, the city has regional contracts:

- **Water purchased:** 5,000 units.
- **Water sold:** 8,000 units.

The purchase is confirmed as active.

**Step 1: Calculate the required local capacity**

```math
L = \lceil(20000-5000)\times1.10\rceil+8000
```

```math
L=16500+8000=24500
```

The city needs **24,500 units** of local production capacity.

**Step 2: Choose the funding**

| Funding | Expected capacity | Result |
|---|---:|---|
| 83% | 24,225 | Not enough |
| **84%** | **24,650** | **Enough** |
| 100% | 34,000 | More than necessary |

**The Department chooses 84% funding.**

The monthly water expense becomes:

```math
X(84)=2000\times0.84=1680
```

**Monthly water department expense: §1,680**, excluding the cost and income of the regional contracts.

This example shows why the Department must consider purchases and sales together rather than looking only at the city's own demand.

#### Example 4: Is selling water worth it?

Consider the city from Example 1 again, without regional contracts.

Its cheapest funding is currently:

- **78% funding.**
- **22,100 units of production capacity.**
- **§1,560 per month.**

A neighboring city now offers to buy **5,000 units of water**.

To supply this sale, the required local capacity increases:

```math
L=22000+5000=27000
```

The Department checks how much funding is needed:

| Funding | Expected capacity | Monthly expense |
|---|---:|---:|
| 78% | 22,100 | §1,560 |
| 89% | 26,775 | §1,780 |
| **90%** | **27,200** | **§1,800** |

**Selling the additional water requires raising funding from 78% to 90%.**

The extra expense is:

```math
\Delta=1800-1560=240
```

**The sale costs an additional §240 per month in local water production.**

The Department can now compare that cost with the money offered by the neighboring city:

| Monthly payment | Result before other contract considerations |
|---|---|
| §200 | §40 loss |
| §240 | Break-even |
| §300 | §60 gain |

**The sale is economically worthwhile only if the neighbor pays more than §240 per month**, subject to the other contract safety rules.

This is how the Department evaluates regional trade using actual production costs rather than accepting every available deal.

See [Neighbor Deals](deals.md#neighbor-deals).

#### Example 5: Damaged pipes prevent a saving

Return to the city from Example 1.

Normally, the Department would reduce water funding from 100% to 78%, saving §440 per month.

However, the game now reports **three distressed pipes**.

With **Protect water pipes** enabled, the Department refuses the reduction.

| Situation | Funding | Monthly expense |
|---|---:|---:|
| No damaged pipes | 78% | §1,560 |
| **Three distressed pipes** | **100%** | **§2,000** |

**The Department keeps funding at 100% while the pipes are damaged.**

At this funding level, the pipe curve is positive (+0.020), allowing the network to recover.

When the game reports zero distressed and zero burst pipes, the Department can reduce funding to 78% at its next review.

**Does the Department increase funding to repair pipes faster?**

No.

It preserves the existing funding while the pipes are damaged, but does not increase it solely to accelerate repairs.

This is a deliberate distinction between **preventing unsafe cuts** and **actively managing pipe repairs**.

#### Example 6: Buildings without water do not always mean insufficient production

Imagine a city with:

- **Water demand:** 20,323 units.
- **Production capacity:** 34,081 units.
- **Buildings without water:** 55.

At first glance, the 55 unwatered buildings suggest a water shortage.

However, the city already has more than enough production capacity to cover its demand.

The likely problem is that some buildings are not properly connected to the water network.

**The Department does not automatically increase funding to solve a connection problem.**

It may still reduce funding, provided the new capacity satisfies the required reserve and pipe protection rules.

Now consider a different situation:

- **Water demand:** 20,323 units.
- **Production capacity:** 19,000 units.
- **Buildings without water:** 55.

Here, production capacity is actually below demand.

**The Department refuses any funding reduction until the shortage is resolved.**

The distinction matters: **missing water caused by insufficient production can be addressed through funding; missing water caused by disconnected pipes cannot.**

#### Example 7: Even maximum funding is not enough

Consider a city with:

- **Water demand:** 35,000 units.
- **Production capacity at 100%:** 34,000 units.
- **Water reserve:** 10%.
- **Regional contracts:** None.

The required capacity is:

```math
L=35000\times1.10=38500
```

The city needs **38,500 units**.

However, even at 120% funding, the water production efficiency is only 110%.

```math
C(120)=34000\times1.10=37400
```

**Maximum available capacity: 37,400 units.**

That is **1,100 units below the required capacity**, including the reserve.

The Department therefore raises water funding to **120%**, the highest available setting.

If regional water contract management is enabled, it can also try to purchase additional water from a neighboring city.

Otherwise, the player must increase local production capacity, for example by building another water pump.

**Funding cannot produce more water than the game's maximum capacity allows.**

<a id="water-limitations"></a>
### 7. Known limitations

The Department optimizes water funding automatically, but several limitations come from SimCity 4 itself.

#### Water production and funding

- **Production capacity updates with a delay.** After a funding change, SimCity 4 normally updates the reported water capacity at the next simulated month. Until then, the Department may temporarily calculate a new plan using capacity that still reflects the previous funding. The later capacity update triggers another evaluation, allowing it to correct the plan.
- **There is only one water funding percentage.** The Department cannot fund individual pumps, towers or treatment plants differently.
- **Funding cannot repair every supply problem.** A polluted or shut-down water source may produce less water. Increasing the budget does not remove the underlying problem.
- **Maximum funding has limits.** If production remains insufficient at 120%, the city needs more water production capacity or reliable regional purchases.

#### Pipe protection

- **Protection prevents cuts; it does not actively manage repairs.** The Department does not raise funding simply to repair distressed or burst pipes faster.
- **Manually underfunded networks are not automatically restored to 50%.** If you set funding below 50% yourself and production remains sufficient, the Department can leave that funding unchanged. It raises funding when more production is required, using the normal protected range.
- **Pipe deterioration is not predicted over time.** The Department uses the game's funding curve to determine whether pipes deteriorate or recover, but does not calculate how many months a particular pipe will take to fail.
- **Disabling protection allows deterioration.** With **Protect water pipes** off, the Department may choose funding below 50% if the water requirement can still be met.

#### Water distribution

- **Buildings without water may have connection problems.** Sufficient city-wide production does not guarantee that water reaches every building. The Department cannot fix missing pipe connections.
- **The Department does not construct infrastructure.** New pumps, water towers and pipe networks remain the player's responsibility.

#### Building compatibility

**Every water producer must use the standard SimCity 4 funding mechanism.**

If even one water-producing building uses an incompatible custom funding system, the Department does not automatically manage the city's water budget.

This restriction prevents the Department from making calculations based on assumptions that do not apply to every producer.

**The Department's purpose is to maintain sufficient water production at the lowest safe cost, not to replace the player's responsibility for water infrastructure.**

[Back to the contents](index.md#contents) | [Back to the README](../../README.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
