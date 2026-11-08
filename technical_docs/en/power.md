**English** | [Português](../pt/power.md) | [Français](../fr/power.md) | [Español](../es/power.md)

[Back to the contents](index.md#contents) | [Back to the README](../../README.md)

<a id="power"></a>
## B. Power

Reverse-engineered executable addresses related to this chapter are listed in the [game address reference](../en/game_addresses.md).

The Department automatically manages **each power plant individually**, adjusting its funding to provide enough power for the city while keeping operating costs as low as possible.

It considers:

- How much power your city needs.
- The safety reserve you have chosen (**20% by default**).
- Power bought from or sold to neighboring cities.
- Each power plant's generating capacity, condition and operating cost.

The Department reviews power funding **every 10 simulated days**. It also performs a quick check every simulated day and can adjust funding earlier when demand, production capacity or regional contracts change significantly.

Rather than applying the same funding percentage to every plant, it looks for **the least expensive combination of funding levels** that meets the city's needs.

The Department never builds, demolishes or replaces power plants. It only adjusts their funding, using its own permitted range and safety checks to protect power supply and the power-line network.

<a id="power-vanilla"></a>
### 1. How power works in SimCity 4

#### Power demand, production and consumption

SimCity 4 tracks several power values for your city:

| Term | What it means |
|---|---|
| **Demand** | The power requested by buildings that are currently powered. |
| **Requirement** | The game's estimate of the power the city needs, including buildings currently without power. It is usually slightly higher than demand. |
| **Consumption** | The power actually delivered to buildings. |
| **Production capacity** | The power your power plants can currently produce, based on their funding and condition. |
| **Imports** | Power purchased from neighboring cities. |
| **Exports** | Power sold to neighboring cities. |

All these values are measured in the game's power units per simulated month.

**Not every power shortage is caused by insufficient production.**

There are two possible situations:

- **Not enough power:** Your power plants and available imports cannot supply the city's demand. The Department can respond by increasing funding or adjusting regional contracts.
- **Power cannot reach some buildings:** The city has enough generating capacity, but some buildings are not properly connected to the power network. Increasing funding will not solve this. You need to fix the connections yourself.

The Department distinguishes between these situations so it does not increase power spending unnecessarily.

**How does imported power affect local production?**

In our observations, SimCity 4 uses imported power before drawing power from local plants. This means a power plant can sometimes show very low usage, or even zero usage, while a neighboring city supplies the power.

The Department takes regional contracts into account when deciding how much local production capacity is needed.

#### How funding affects a power plant's output

Each power plant has three important characteristics:

- **Maximum output:** How much power the plant can produce at its nominal capacity.
- **Funding:** The percentage of its operating budget allocated to the plant.
- **Condition:** The plant's current state, from 0 (completely deteriorated) to 1 (new).

A plant's actual output depends on both **its funding and its condition**.

For example, a new power plant funded at 100% can produce its full nominal capacity. As its condition deteriorates, its available output decreases.

<a id="power-plant-efficiency"></a>
**Plant efficiency (base game)**

```math
\text{Eff} = \frac{\text{Curve}(f)}{100} \times \text{Cond}
```

Where:

- **f** is the plant's funding percentage.
- **Curve(f)** is the efficiency provided by the game's funding curve.
- **Cond** is the plant's current condition, between 0 and 1.
- **Eff** is the resulting efficiency, expressed as a fraction.

**Effective output (base game)**

```math
\text{Out} = \text{MaxOut} \times \text{Eff}
```

Where:

- **MaxOut** is the plant's maximum output.
- **Out** is its effective generating capacity.

**Example: a power plant with a maximum output of 7,500 units**

Assume the plant is in almost perfect condition (0.9998).

| Funding | Efficiency after condition | Effective output |
|---|---:|---:|
| 100% | About 100% | About 7,498 units |
| 95% | About 90% | About 6,748 units |

Reducing funding by just **5 percentage points** reduces production capacity by approximately **10%**.

This happens because the game's relationship between funding and production is not proportional.

<a id="power-efficiency-curve"></a>
#### The relationship between funding and efficiency

SimCity 4 uses a built-in curve to determine how much power a power plant can produce at each funding level.

These are the original game's control points:

| Plant funding | Production efficiency |
|---:|---:|
| 0% | 0% |
| 50% | 25% |
| 90% | 80% |
| 100% | 100% |
| 120% | 110% |

**The important detail is that reducing funding also reduces efficiency.**

For example:

- At **50% funding**, a plant provides only **25% of its nominal output**.
- At **90% funding**, it provides **80% of its nominal output**.
- At **100% funding**, it provides **100% of its nominal output**.
- At **120% funding**, it provides **110% of its nominal output**.

Funding above 100% is therefore less cost-effective: you pay 20% more to obtain only 10% additional generating capacity.

**How are intermediate values calculated?**

The game uses straight-line interpolation between the control points. The Department uses the same rule.

**Curve interpolation**

```math
y(x) = y_i + \frac{x-x_i}{x_{i+1}-x_i}(y_{i+1}-y_i)
\qquad \text{for } x_i \le x \le x_{i+1}
```

This simply means that, between two known points, efficiency changes at a constant rate.

For example, between 90% and 100% funding, production efficiency increases from 80% to 100%. At 95% funding, efficiency is therefore 90%.

The curve stays at its first or last value outside the defined range.

**Efficiency at the funding levels used by the Department**

The Department normally considers funding levels from 60% to 120%.

| Funding (%) | 60 | 65 | 70 | 75 | 80 | 85 | 90 | 95 | 100 | 105 | 110 | 115 | 120 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| Efficiency (%) | 38.75 | 45.625 | 52.5 | 59.375 | 66.25 | 73.125 | 80 | 90 | 100 | 102.5 | 105 | 107.5 | 110 |
| Output per § spent (relative to 100%) | 0.65 | 0.70 | 0.75 | 0.79 | 0.83 | 0.86 | 0.89 | 0.95 | 1.00 | 0.98 | 0.95 | 0.93 | 0.92 |

The final row compares how much generating capacity each funding level provides for the money spent, relative to 100% funding.

**What does this mean for your city's power budget?**

At **100% funding**, a power plant provides the most generating capacity per simoleon spent.

However, that does **not** mean every plant should always be funded at 100%.

Imagine your city has two power plants but does not need all their combined output.

Funding both plants equally may cost more than running one near 100% and keeping the other at a lower funding level.

**This is exactly what the Department optimizes:** it compares the available combinations and chooses the cheapest one that still provides the required power and safety reserve.

See [Example 3](#power-example-mixed) for a calculation involving two different power plants.

#### Individual power plant funding and operating costs

Unlike the water service, **power funding is managed separately for each power plant**.

Every plant has its own funding setting and monthly operating cost.

This allows the Department to increase the funding of one plant while reducing another, depending on which combination is more economical.

**What funding levels can the Department use?**

The game's power configuration identifies **60%** as its *minimum acceptable funding*. However, that value should not be confused with a proven hard limit of the game's funding control.

The underlying local budget setter accepts values starting at 0%, subject to the department's maximum.

The Department's optimizer deliberately considers funding levels from **60% to 120%**, in whole percentages, and applies additional checks before changing a plant's budget.

These checks ensure that the planned changes meet the city's power needs and do not cause power-line deterioration.

**How is the monthly operating cost calculated?**

The operating cost changes in proportion to the funding percentage.

**Monthly plant expense (base game)**

```math
E(f) = \mathrm{round}\left(C \times \frac{f}{100}\right)
```

Where:

- **C** is the plant's monthly operating cost at 100% funding.
- **f** is the chosen funding percentage.
- **E(f)** is the estimated monthly expense at that funding.

For example, a power plant costing §253 per month at 100% funding would cost approximately:

| Funding | Monthly operating cost |
|---|---:|
| 60% | §152 |
| 84% | §213 |
| 100% | §253 |
| 120% | §304 |

At **84% funding**, the city saves approximately **§40 per month** compared with funding that plant at 100%.

However, the Department only chooses 84% if the resulting production capacity is sufficient for the overall power plan.

The game reports a plant's nominal generation cost separately from what is actually charged at its current funding. The Department uses the latter relationship to estimate expenses, rounding to whole simoleons, and reads the plant's latest nominal cost whenever it recalculates the plan.

#### Why does the power department show a different funding percentage?

Although power plants are funded individually, SimCity 4 also shows an overall power department funding percentage in the city budget.

**This is not an additional funding slider.**

It is calculated as a **cost-weighted average** of the individual power plants' funding levels.

That means a large, expensive power plant has more influence on the department percentage than a small, inexpensive one.

**Overall power funding (base game)**

```math
F_{\text{dept}} = \frac{\sum_i C_i f_i}{\sum_i C_i}
```

Where:

- **Cᵢ** is the nominal monthly operating cost of plant *i*.
- **fᵢ** is that plant's funding percentage.
- **Fdept** is the overall power department funding percentage.

**Example**

Imagine your city's power plants have a combined nominal operating cost of **§11,660 per month**.

One plant costs §300 per month and is funded at 95%, while every other plant remains at 100%.

That plant saves §15 per month.

The overall power department funding becomes:

```math
F_{\text{dept}} = \frac{11660-15}{11660}\times100
\approx 99.87\%
```

Even though one plant is funded at 95%, the department as a whole still shows approximately **99.87% funding**.

This is normal.

**The Department manages each power plant individually. The overall power budget reflects the combined result of those decisions.**
#### Power lines: how funding affects the network

In SimCity 4, power funding affects more than power plants. It also influences the condition of the city's **power-line network**.

The game uses the overall power department funding to determine whether power lines deteriorate or are repaired over time.

| Department funding | Effect on power lines |
|---:|---|
| 0% | Rapid deterioration (−0.020) |
| 25% | Deterioration (−0.005) |
| 50% | No deterioration or repair (0) |
| 100% | Repair (+0.025) |
| 110% | Faster repair (+0.050) |

Values between these points are calculated using the game's standard linear interpolation.

**The important threshold is 50% department funding:**

- **Below 50%:** Power lines gradually deteriorate.
- **At 50%:** Their condition remains stable.
- **Above 50%:** The network gradually repairs itself.

The Department normally considers funding levels between **60% and 120% for each managed power plant**. This helps keep overall power funding above the level at which power lines deteriorate.

It also checks the effect of funding changes on the power-line network before applying them.

**The Department does not simply cut power plant budgets without considering the condition of the power network.**

<a id="power-aging"></a>
#### Power plant condition and aging

Power plants do not last forever. As they operate, their condition gradually decreases.

In SimCity 4, **how quickly a power plant ages depends mainly on how heavily it is used**, although its funding also plays a role.

The Department reads information from each plant, including:

- **Base aging rate:** The plant's underlying aging parameter.
- **Aged threshold:** The condition at which the game considers the plant old. This was 0.9 (90%) for the plants examined.
- **Replacement cost:** The amount needed to construct another plant of the same type.

These values help the Department estimate the economic cost of operating a plant when the optional aging calculation is enabled.

**Funding and usage are not the same thing**

This distinction is essential.

- **Funding** determines how much power a plant *can produce*.
- **Usage** measures how much of that available capacity the city actually uses.

For example:

| Power plant | Funding | Actual usage |
|---|---:|---:|
| Plant A | 100% | 30% |
| Plant B | 70% | 95% |

Plant A is fully funded but produces much less power than it could.

Plant B receives less funding, but the city uses almost all the power it can produce at that funding level.

**Plant usage (base game)**

```math
u = \frac{\text{Used}}{\text{Out}}
```

Where:

- **Used** is the power actually drawn from the plant.
- **Out** is the plant's available generating capacity.
- **u** is the proportion of that capacity being used.

For example, if a plant can produce 10,000 units and the city draws 8,000, its usage is **80%**.

When a plant produces nothing, the Department treats its usage as zero.

### How quickly do power plants age?

The game's aging calculation depends on three factors:

1. The plant's base aging rate.
2. How heavily the plant is used.
3. Its funding level.

**Estimated aging per simulated month (SimCity 4, modeled by the Department)**

```math
A = \frac{B \times U(\min(\max(u,0),2)) \times G\left(\frac{f}{100}\right)}{16}
```

Where:

- **B** is the plant's base aging parameter.
- **u** is its current usage, expressed as a fraction.
- **U(u)** is the aging multiplier associated with usage.
- **f** is its funding percentage.
- **G(f/100)** is the aging multiplier associated with funding.
- **A** is the estimated condition lost per simulated month.

In simple terms:

**Plant aging = Base aging × Usage effect × Funding effect × Time factor**

The base rate and the two aging curves come from the game's mechanics.

The **1/16 time factor** was established through measurements of seven power plants over 13 simulated months. It is an experimentally measured value, not a constant directly read from the game.

The resulting aging estimates should therefore be treated as approximations rather than exact predictions of a plant's remaining lifespan.

#### 1. How funding affects aging

The game's funding-aging curve has three control points:

| Funding | Aging multiplier |
|---:|---:|
| 0% | 1.10 |
| 100% | 1.00 |
| 120% | 0.90 |

The values between these points are interpolated.

Within the Department's normal funding range:

| Funding | Aging multiplier |
|---:|---:|
| 60% | 1.04 |
| 80% | 1.02 |
| 90% | 1.01 |
| 100% | 1.00 |
| 110% | 0.95 |
| 120% | 0.90 |

**Funding has a relatively small direct effect on aging.**

For example, reducing a plant's funding from 100% to 60% increases its direct aging multiplier from 1.00 to 1.04, an increase of only **4%**.

However, reducing funding also reduces the amount of power the plant can produce. If the city continues drawing the same quantity, the plant's usage increases, and that can have a much larger effect on aging.

#### 2. How usage affects aging

The game's usage-aging curve is much more significant:

| Plant usage | Aging multiplier |
|---:|---:|
| 0% | 0.10 |
| 25% | 0.20 |
| 50% | 0.45 |
| 75% | 0.85 |
| 90% | 1.25 |
| 100% | 1.55 |
| 110% or more | 2.00 |

As with the other curves, intermediate values are calculated using linear interpolation.

**The harder a plant works, the faster it ages.**

For example:

- At **50% usage**, the aging multiplier is 0.45.
- At **90% usage**, it reaches 1.25.

A plant operating at 90% usage therefore ages approximately **2.8 times faster** than the same plant operating at 50% usage, assuming everything else remains equal.

This is why simply reducing power plant funding is not always economically ideal.

**Example: reducing funding can nearly double aging**

Imagine a plant initially funded at 100%, with the city using about 60% of its available output.

The Department reduces funding to save operating costs. The plant can now produce less power, but the city continues drawing roughly the same amount.

Its usage may increase to about 83%.

The usage-aging multiplier rises from approximately 0.60 to 1.06, while the direct funding multiplier changes only slightly.

**The plant's estimated aging rate becomes almost twice as high.**

This does not necessarily mean the funding reduction is a bad decision. The monthly saving may still be worth more than the additional wear.

The Department can compare these costs when **Consider power plant aging** is enabled.

**What does "Consider power plant aging" do?**

This option is **off by default**.

- **Off:** The Department minimizes monthly power operating expenses while maintaining the required supply and safety reserve.
- **On:** The Department also includes the estimated economic cost of plant wear when comparing funding combinations.

The option may favor a different funding arrangement when the reduction in wear compensates for the additional operating expense.

**It does not impose a separate minimum funding percentage, prevent aging or automatically replace old power plants.**

<a id="power-usage-myth"></a>
#### Is 80% plant usage a safety limit?

**No. SimCity 4 does not have a sudden aging or blackout threshold at 80% usage.**

The confusion comes from the game's aging curve.

At approximately **80.6% usage**, the aging multiplier reaches 1.0. Below that point, the multiplier is lower; above it, the multiplier increases.

But the curve is continuous.

A plant running at 81% usage does not suddenly become unsafe. Likewise, a plant running at 95% usage does not automatically cause a blackout.

**Blackouts happen when power supply is insufficient or cannot reach buildings, not because a particular plant exceeds 80% usage.**

The Department therefore does not enforce an 80% usage ceiling.

Instead, it uses the **Power reserve** setting to maintain spare generating capacity.

#### How does the power reserve affect usage?

The default power reserve is **20%**.

For a simple city supplied by a single local plant, with no regional power trade, the relationship is approximately:

```math
u \approx \frac{1}{1+r}
```

Where **r** is the configured reserve expressed as a fraction (0.20 for a 20% reserve).

This assumes the plant supplies exactly the required capacity and ignores small differences caused by rounding and the game's additional requirements.

| Power reserve | Approximate plant usage | Usage-aging multiplier |
|---|---:|---:|
| 10% | 90.9% | 1.28 |
| **20% (default)** | **83.3%** | **1.07** |
| 25% | 80.0% | 0.98 |
| 30% | 76.9% | 0.90 |
| 50% | 66.7% | 0.72 |

A higher reserve generally leaves more spare generating capacity and can reduce how heavily a plant needs to operate.

However, **more reserve may also require more funding and therefore increase monthly expenses**.

With multiple power plants, the relationship becomes more complicated because SimCity 4 decides how power demand is distributed among them.

The Department does not directly control that distribution.

Its purpose is to maintain enough total capacity, with your chosen reserve, while minimizing the costs included in its optimization.

#### Regional power trade

The Department also considers power bought from or sold to neighboring cities.

- **Buying power** reduces how much local generating capacity the city needs.
- **Selling power** increases the capacity the city's own plants must provide.

For example, if your city needs 10,000 units and reliably imports 3,000, its power plants do not need to produce the entire 10,000 locally.

On the other hand, selling 2,000 units to a neighbor means those units must be available in addition to your own city's needs.

The Department includes these contracts when calculating the required capacity and choosing plant funding.

How it creates, changes, prices and cancels contracts is explained in [Neighbor Deals](deals.md#neighbor-deals).

<a id="power-problem"></a>
### 2. The management problem

Managing power manually is more complicated than adjusting a single budget slider.

The Department must balance several factors:

- **Avoiding unnecessary spending:** A power plant funded at 100% may produce far more power than the city needs.
- **Maintaining enough supply:** Reducing funding too far can leave buildings without power.
- **Protecting the power network:** Very low overall power funding can cause power lines to deteriorate.
- **Accounting for plant efficiency:** Funding and production are not proportional. Cutting every plant to the same percentage is not necessarily the cheapest solution.
- **Accounting for condition and aging:** Older plants produce less power, and plants working closer to full capacity generally age faster.
- **Considering regional trade:** Buying power can reduce local production requirements, while selling power increases them.
- **Managing several plants together:** The cheapest combination may involve funding one plant near 100% and another much lower, rather than applying the same percentage to both.

SimCity 4 does not directly show which combination of plant funding levels will provide the required power at the lowest overall cost.

**The Department calculates that combination automatically.**

It considers each plant's capacity, condition and operating expense, together with your chosen reserve and regional contracts.

When the optional aging setting is enabled, it also includes the estimated economic cost of plant wear.

**The result is an power budget designed to meet the city's needs without spending more than the Department's model considers necessary.**
<a id="power-algorithm"></a>
### 3. How the Department optimizes power funding

The Department does not simply reduce every power plant's funding by the same percentage.

Instead, it calculates how much power the city needs and compares different funding combinations to find **the lowest-cost solution that maintains sufficient generating capacity**.

The process has three main steps:

1. Calculate how much power the city's plants must provide.
2. Calculate how much each plant can produce at different funding levels.
3. Find the cheapest combination that meets the requirement.

If **Consider power plant aging** is enabled, the Department also includes the estimated economic cost of plant wear when comparing these combinations.

<a id="power-required-capacity"></a>
#### Step 1: How much power does the city need?

The Department first calculates the generating capacity that must be available locally.

It considers:

- The city's current power demand.
- Buildings that may still need power.
- Your configured **Power reserve**, 20% by default.
- Power purchased from neighboring cities.
- Power sold to neighboring cities.

**The reserve is applied only to the power the city needs to produce itself.**

For example, if your city needs 5,000 units and imports 2,000 from a neighbor, its power plants only need to cover the remaining 3,000 units, plus the configured reserve.

Power sold to neighbors must also be produced, so exports increase the local requirement.

**Required local generating capacity**

```math
N = \max\left(\left\lceil R \times \max(0,D-B)\right\rceil,\ Q-B\right)+S
```

Where:

- **D** is the city's power demand.
- **Q** is the game's total power requirement, including buildings currently without power.
- **B** is power purchased from neighbors that the plan can rely on.
- **S** is power sold to neighbors.
- **R** is the reserve multiplier (1.20 for the default 20% reserve).
- **N** is the generating capacity required from local power plants.

The Department uses whichever is greater: the reserved local demand or the game's remaining power requirement. Exports are then added.

**Example: a city with 5,000 units of demand**

Assume the game's total requirement is 5,050 units and the reserve is 20%.

| Situation | Required local capacity |
|---|---:|
| No power imports or exports | 6,000 units |
| Importing 2,000 units | 3,600 units |

Without imports:

```math
N = \max(5000 \times 1.20,\ 5050)=6000
```

With 2,000 units imported:

```math
N = \max(3000 \times 1.20,\ 3050)=3600
```

**The Department does not buy an additional reserve on top of imported power.** Its reserve protects the part of demand that local plants must supply.

If the Department manages power contracts, it plans the contracts and power plant funding together.

Otherwise, it uses the existing contracts, counting purchases only when they are reliably active. Pending, suspended or failing purchases are not treated as guaranteed supply. Existing sales are counted as obligations.

See [Neighbor Deals](deals.md#neighbor-deals) for more information.

#### Step 2: How much can each plant produce?

Once the required capacity is known, the Department examines every power plant it can safely manage.

For each plant, it considers **whole-number funding percentages from 60% to 120%**.

It also considers the plant's current funding, even if you have manually set it outside that range.

For every possible funding level, the Department calculates:

- The plant's available generating capacity.
- Its monthly operating expense.
- Its estimated wear cost, when the aging option is enabled.

**Planned generating capacity**

```math
\text{Cap}(f) = \left\lfloor \text{MaxOut} \times \frac{\text{Curve}(f)}{100} \times \text{Cond} \right\rfloor
```

Where:

- **MaxOut** is the plant's maximum output.
- **Curve(f)** is the game's efficiency at funding level **f**.
- **Cond** is the plant's current condition, from 0 to 1.
- **Cap(f)** is the resulting generating capacity, rounded down.

The Department uses the plant's **actual condition**, not the capacity it had when new.

**Example: a power plant in good condition**

Suppose a plant has:

- Maximum output: 6,000 units.
- Current condition: 98%.
- Proposed funding: 84%.

At 84% funding, the game's efficiency curve gives 71.75%.

```math
\text{Cap}(84) = \left\lfloor 6000 \times 0.7175 \times 0.98 \right\rfloor = 4218
```

The Department therefore counts **4,218 units** of generating capacity from this plant.

It repeats this calculation for the other possible funding levels and for every eligible plant.

**What about plants the Department cannot manage?**

Some plants cannot be safely adjusted. For example:

- Plants with unsupported or unreadable funding information.
- Plants whose operating costs or production characteristics cannot be reliably determined.
- Waste-to-Energy plants currently burning garbage, because changing their funding could interfere with the city's sanitation system.

These plants are **not ignored**.

The Department includes their current generating capacity in the calculation but leaves their funding unchanged. Their costs are accounted for without counting the same expense twice.

If **Manage power** is disabled, all power plants keep their current funding. Power contracts can still be managed separately if their options remain enabled.

<a id="power-optimizer"></a>
#### Step 3: Finding the cheapest combination

This is where the Department makes the difference.

Imagine a city with several power plants. Each plant has its own operating cost, efficiency curve and condition.

There may be many possible funding combinations that provide enough power.

**The Department compares these combinations and chooses the one with the lowest total modeled cost.**

It does not simply lower the most expensive plant first or fund every plant at the same percentage.

**Power optimization formula**

```math
\min_{f_1,\ldots,f_n}\sum_{i=1}^{n}\left(E_i(f_i)+W_i(f_i)\right)
\quad\text{subject to}\quad
K+\sum_{i=1}^{n}\text{Cap}_i(f_i)\ge N
```

Where:

- **Eᵢ** is the monthly operating expense of plant *i*.
- **Wᵢ** is its estimated wear cost, or zero when the aging option is disabled.
- **Capᵢ** is its available capacity at the selected funding.
- **K** is the combined capacity of plants whose funding cannot be changed.
- **N** is the required local capacity calculated in Step 1.

In plain English:

**Find the cheapest combination of plant funding levels that produces enough power for the city and its reserve.**

For example, the most economical solution might be:

| Plant | Chosen funding |
|---|---:|
| Coal power plant | 91% |
| Natural gas power plant | 61% |

Another city with different demand, plants or operating costs might benefit from a completely different combination.

See [Example 3](#power-example-mixed) for a worked calculation involving two plants.

**How does the Department find the best combination?**

It uses an optimization algorithm that compares plant funding options together, rather than making independent decisions for each plant.

To keep the calculations fast in large cities, it organizes generating capacity into small units.

**Optimization capacity unit**

```math
\text{unit} = \max\left(1,\left\lceil\frac{N}{20000}\right\rceil\right)
```

For cities requiring up to **20,000 units**, generating capacity is compared one unit at a time.

For larger cities, the Department uses slightly larger units to reduce the amount of computation.

This can result in a plan with a little more spare capacity than the mathematical minimum, but the rounding is conservative: **the selected plan must still meet the required generating capacity**.

When comparing many possible regional contracts, the contract planner uses a coarser calculation for speed. The final power plant plan uses the more detailed calculation.

**What if several combinations cost the same?**

The Department prefers the combination that changes the fewest power plants.

For example, if keeping the current funding costs exactly the same as changing three plants, it leaves them alone.

This prevents unnecessary budget adjustments.

**What if there is not enough power?**

Sometimes even funding every available plant at its highest-capacity setting is insufficient.

In that case, the Department:

1. Chooses the highest available production capacity from the managed plants, normally by raising them to 120%.
2. Attempts to cover the remaining shortage through regional purchases, if power contract management is enabled.
3. Issues an advisor notice if the shortage is confirmed and persists.

**The Department never builds or replaces power plants.** If funding and available purchases cannot solve the problem, you must provide additional generating capacity yourself.

<a id="power-wear"></a>
#### Optional: Considering power plant aging

By default, the Department focuses on **minimizing the monthly power bill**.

However, the cheapest operating budget is not always the cheapest choice over a power plant's lifetime.

As explained earlier, reducing funding can increase how heavily a plant is used, potentially accelerating its deterioration.

The optional setting **Consider power plant aging** allows the Department to include an estimated cost for that wear.

| Setting | How funding is optimized |
|---|---|
| **Off (default)** | Minimizes actual monthly operating expenses while maintaining the required capacity and reserve. |
| **On** | Minimizes operating expenses plus the estimated economic cost of power plant wear. |

**Important:** The estimated wear cost is not an additional charge to your city's treasury. It is used only to compare funding choices.

Power plants still age normally whether this option is enabled or not.

#### How is the cost of aging calculated?

Each power plant has a construction cost and an aging threshold.

The Department estimates how much of the plant's remaining useful condition is consumed during a simulated month, then assigns a monetary value to that wear.

**Estimated monthly wear cost**

```math
W(f) = \mathrm{round}\left(P \times \frac{\min\left(A(f),\ \text{Cond}-T\right)}{1-T}\right)
```

This formula applies while the plant's condition is above its aged threshold. Otherwise, the modeled wear cost is zero.

Where:

- **P** is the plant's construction or replacement cost.
- **A(f)** is its estimated monthly aging at the funding level being considered.
- **Cond** is its current condition.
- **T** is the condition threshold at which the game considers it aged.
- **W(f)** is the estimated economic wear cost per simulated month.

For example, if a plant costs §10,000 to construct and the useful condition range is from 100% down to 90%, the Department treats that 10% condition range as having an economic value of §10,000.

A small part of that range consumed during one month represents a corresponding fraction of the replacement cost.

The result is rounded to whole simoleons.

**Construction costs of the original game's power plants**

| Power plant | Replacement cost |
|---|---:|
| Wind | §500 |
| Natural gas | §9,000 |
| Coal | §10,000 |
| Oil | §17,000 |
| Waste-to-Energy | §25,000 |
| Solar | §30,000 |
| Nuclear | §40,000 |
| Hydrogen | §100,000 |

For compatible custom power plants, the Department reads the construction cost supplied by the building.

**What if the necessary information is missing?**

If the plant's replacement cost, aging rate or aged threshold cannot be determined reliably, the Department simply leaves the wear cost out of the calculation **for that plant alone**.

The plant can still be managed normally using its operating expense.

Other plants with valid aging information continue to include their estimated wear costs.

#### What difference does this option make?

The effect is often smaller than players might expect.

Within the Department's funding range of 60% to 120%, the direct funding-aging multiplier varies only from **1.04 to 0.90**.

The Department calculates the wear cost for each candidate using the plant's **currently observed usage**. It does not predict exactly how SimCity 4 will redistribute the power load after every possible funding change.

Consequently, the estimated wear cost usually changes only slightly between funding levels.

For many original-game power plants, this represents just a few simoleons per month.

**Example**

Suppose the Department compares two funding choices:

| Option | Operating expense | Estimated wear | Total modeled cost |
|---|---:|---:|---:|
| A | §200 | §8 | §208 |
| B | §205 | §2 | §207 |

<italic>Illustrative figures, not measurements from a specific plant.</italic>

With aging disabled, **Option A** wins because its monthly operating expense is lower.

With aging enabled, **Option B** wins because its combined operating and wear cost is slightly lower.

The city's actual monthly power expense would be §205, not §207. The additional §2 exists only in the economic comparison.

In practice, the two optimization modes often choose the same funding combination, especially when the wear difference is small.

See [Example 5](#power-example-wear) for a calculation using the game's plant data.

**What this option does not do**

Enabling **Consider power plant aging** does not:

- Prevent power plants from deteriorating.
- Require plants to operate below 80% usage.
- Introduce an additional minimum funding level.
- Automatically replace aging plants.
- Relax the city's power reserve or other safety requirements.

It simply allows the Department to consider **the estimated long-term economic cost of wear alongside the monthly operating bill**.

The purpose remains the same: **provide enough power for your city at the lowest modeled cost, using the options and safety limits you have chosen.**
<a id="power-cadence"></a>
#### Step 4: When does the Department adjust power funding?

The Department regularly monitors your city's power supply and adjusts power plant funding when necessary.

**It normally reviews funding every 10 simulated days, but it can react sooner if power demand or supply changes significantly.**

All timings refer to simulated game time.

| When | What the Department does |
|---|---|
| **When a city opens** | Performs its first evaluation as soon as the game's power information is available and consistent. There is no fixed waiting period. |
| **Every simulated day** | Quickly checks power demand, supply, consumption, generating capacity and regional contracts for significant changes. |
| **Every 10 simulated days** | Performs a regular evaluation of the power plants and their funding. |
| **When a significant change is detected** | Recalculates the power plan without waiting for the next regular review. |

**What triggers an early evaluation?**

During its daily checks, the Department looks for:

- **A real power shortage:** The city does not have enough power from its power plants and usable imports to meet demand.
- **A change of at least 1% in power demand**, compared with the last evaluation.
- **A change of at least 1% in local generating capacity.**
- **A change of at least 1% in contracted power imports or exports.**

An unchanged reading does not trigger another evaluation.

The Department also distinguishes between a genuine shortage of power and a problem with the city's power connections.

For example, if sufficient power is available but some buildings are disconnected from the network, the Department does not unnecessarily increase power plant funding. The connection problem is recorded in the log instead.

**How are funding changes applied?**

When the Department chooses a new funding level, it applies that level directly.

For example, a plant can move from 60% to 95% funding in a single adjustment. There is no gradual increase or artificial waiting period.

However, **increases always take priority over reductions**.

The Department raises funding where necessary and ensures that other parts of the plan, such as new power purchases, are in place before making reductions in a later evaluation.

This prevents temporary capacity shortages while several power plants or regional contracts are being adjusted.

Every evaluation uses the city's current information. If you manually change a plant's funding, or save and reopen the city, the Department takes the new situation into account.

<a id="power-safety"></a>
### 4. Safety conditions

The Department follows several safety rules to prevent its funding adjustments from disrupting the power supply.

#### Before calculating a plan

The Department only makes funding decisions when the game's information is reliable.

- Power demand and the other required readings must be available and consistent.
- A demand reading of zero is treated as potentially uninitialized, not automatically as proof that the city needs no power.
- At least one power plant must have readable funding and production information before plant funding can be managed.
- Contradictory readings, such as reported power consumption exceeding demand by more than 1% plus one unit, prevent automatic funding changes.

A city with **no power plants** can still be evaluated for power purchases from neighboring cities, if regional contract management is enabled.

#### Protecting power supply

**The Department never deliberately reduces funding below the capacity required by its safety rules.**

Before every funding reduction, it:

1. Reads the city's power information again.
2. Checks the updated generating capacity of every power plant, using its current condition and planned funding.
3. Counts only power purchases that are already active and usable.
4. Verifies that the remaining capacity will cover local demand, the configured reserve and exports.

If the reduction would leave insufficient capacity, **it is cancelled**.

A pending, suspended or failing power purchase is never treated as guaranteed supply.

Funding increases are not rejected merely because the city remains short of power. Even if maximum production cannot solve the whole shortage, increasing available capacity can still help.

#### Protecting power plants and power lines

- New funding targets normally range from **60% to 120%**, in whole percentages.
- The Department respects the funding limits accepted by the game's budget system.
- It rejects funding changes that would cause the power-line network to deteriorate.
- When the city has a genuine power deficit, the regional contract planner does not maintain power sales.

The Department never builds, demolishes, replaces or switches off a power plant. Its actions are limited to changing eligible funding settings.

#### Checking every funding change

Before changing a plant's funding, the Department verifies that its current value still matches the one used to calculate the plan.

If you have changed that value manually, the Department skips the adjustment and recalculates during a later evaluation.

It also identifies plants by their position on the city map. If two plants cannot be distinguished reliably at the same position, it does not guess which one to modify.

**After every change, the Department checks that the game has accepted it.**

Both the plant's funding value and its budget line must reflect the new setting.

If verification fails:

- The Department attempts to undo the changes made during that review.
- Automatic power plant funding management stops for the rest of the current city session.
- Opening the city again allows management to restart.
- Other services and regional power contracts continue operating independently.

**The priority is to preserve a reliable power supply, even if that means giving up a potential saving.**

See [General Safety](safety.md#safety) for the safeguards shared by all services.

<a id="power-settings"></a>
### 5. Settings

Power management can be configured in **Play Options → Department of Efficiency**, under the **Services** page.

| Setting | Default | Range | What it does |
|---|---|---|---|
| [Department active](options.md#opt-master) | On | On / Off | Enables or disables the entire Department. |
| [Manage power](options.md#opt-power) | On | On / Off | Automatically optimizes individual power plant funding. |
| [Power reserve](options.md#opt-powerreserve) | 20% | 10–50% | Sets the minimum spare local generating capacity the Department aims to maintain. |
| [Consider power plant aging](options.md#opt-poweraging) | Off | On / Off | Includes estimated plant wear when comparing funding costs. |
| [Manage neighbor deals](options.md#opt-deals) | On | On / Off | Enables automatic management of regional contracts. |
| [Manage Power Deals](options.md#opt-tradepower) | On | On / Off | Enables automatic management of power purchases and sales. |

The [Buy from one city to sell to another](options.md#opt-arbitrage) option controls whether the Department may purchase power from one neighboring city for resale to another. See [Neighbor Deals](deals.md#neighbor-deals).

#### How does the power reserve work?

With a 20% reserve, the Department normally aims to maintain 20% more local generating capacity than the power the city must produce itself.

For example, if the city needs to produce 5,000 units locally, the Department aims for at least **6,000 units of capacity**.

This is a **minimum**, not a maximum. The Department may maintain more capacity when the available plant settings or profitable power sales justify it.

Increasing the reserve can provide additional protection against growth and changes in demand, but it may also increase operating costs.

If you disable **Manage power**, the Department leaves plant funding unchanged. It can still manage power purchases and sales if the corresponding options remain enabled.

#### Advanced settings (INI file only)

| Setting | Default | Allowed range | What it does |
|---|---|---|---|
| `PowerReservePercent` | 20 | 10–50 | Sets the power reserve percentage. |
| `PowerCadenceDays` | 10 | 1–30 | Sets how often regular power funding reviews are performed, in simulated days. |

For example, setting `PowerCadenceDays=5` makes the Department perform a regular review every five simulated days instead of ten.

**The daily checks for urgent supply changes remain separate from this interval.**

An invalid `PowerReservePercent` value makes the utility configuration file invalid. In that situation, the Department does not manage any utility until the configuration is corrected.

For most players, the default settings are a suitable starting point.

<a id="power-examples"></a>
### 6. Worked examples

The following examples show how the Department chooses funding for different power plants and power requirements.

All plant capacities, operating expenses and conditions are illustrative. The calculations use the original game's efficiency curves.

Unless otherwise stated, the power reserve is **20%**, and there are no regional power contracts.

The coal plant aging figures use values measured during testing: a base aging rate of 0.001111 per month and an aged threshold of 0.9. The same aging parameters are used for the natural gas plant where needed for illustration.

<a id="power-example-coal"></a>
#### Example 1: A coal power plant with unused capacity

Consider a coal power plant with:

- **Maximum output:** 6,000 units.
- **Monthly operating cost at 100%:** §253.
- **Current condition:** 98%.
- **City demand:** 3,500 units.
- **Game requirement:** 3,550 units.
- **Power reserve:** 20%.

**Step 1: Calculate the required capacity**

```math
N = \max(3500 \times 1.20,\ 3550)=4200
```

The city needs **4,200 units** of local generating capacity.

**Step 2: Compare funding levels**

| Funding | Available capacity | Monthly cost | Result |
|---|---:|---:|---|
| 83% | 4,138 | §210 | Not enough |
| **84%** | **4,218** | **§213** | **Enough** |
| 85% | 4,299 | §215 | Enough, but more expensive |
| 100% | 5,880 | §253 | Much more capacity than needed |

**The Department chooses 84% funding.**

The plant can produce 4,218 units, slightly more than the required 4,200.

Its monthly operating cost falls from §253 to §213.

<box gap={1}>
**Monthly saving**
<title size="xl" color="success">§40</title>
</box>

**What happens to plant aging?**

At 100% funding, the plant can produce 5,880 units, while the city draws 3,500. Its usage is approximately **59.5%**.

At 84% funding, available production falls to 4,218 units. If the city continues drawing 3,500, usage rises to approximately **83%**.

The modeled monthly condition loss changes:

| Funding | Plant usage | Estimated monthly condition loss |
|---|---:|---:|
| 100% | 59.5% | 0.0000418 |
| 84% | 83.0% | 0.0000750 |

**The estimated aging rate becomes nearly 1.8 times as high.**

Most of that difference comes from higher usage, not directly from lower funding.

This illustrates why reducing operating costs can accelerate plant aging, and why the optional aging calculation may be useful.

<a id="power-example-gas"></a>
#### Example 2: A natural gas plant

Consider a natural gas plant with:

- **Maximum output:** 3,000 units.
- **Monthly cost at 100%:** §230.
- **Condition:** 100%.
- **City demand:** 2,000 units.
- **Game requirement:** 2,020 units.

With a 20% reserve, the city needs:

```math
N = \max(2000 \times 1.20,\ 2020)=2400
```

The Department compares the available funding levels:

| Funding | Available capacity | Result |
|---|---:|---|
| 89% | 2,358 | Not enough |
| **90%** | **2,400** | **Enough** |
| 100% | 3,000 | More than necessary |

**The Department chooses 90% funding.**

The monthly operating cost becomes:

```math
E = 230 \times 0.90 = 207
```

The plant continues supplying the required power and reserve, while saving **§23 per month**.

<a id="power-example-mixed"></a>
#### Example 3: Two power plants working together

This example shows why the Department does not fund every plant at the same percentage.

Imagine a city with the two plants from the previous examples:

| Plant | Maximum output | Condition | Cost at 100% |
|---|---:|---:|---:|
| Coal | 6,000 | 98% | §253 |
| Natural gas | 3,000 | 100% | §230 |
| **Total** | **9,000** | | **§483** |

The city has:

- **Power demand:** 5,000 units.
- **Game requirement:** 5,050 units.
- **Power reserve:** 20%.

The Department calculates a required local capacity of:

```math
N = \max(5000 \times 1.20,\ 5050)=6000
```

**The city needs 6,000 units of generating capacity.**

The Department compares different funding strategies:

| Funding strategy | Combined capacity | Monthly cost |
|---|---:|---:|
| Both plants at 100% | 8,880 | §483 |
| Both plants at 81% | 6,004 | §391 |
| **Coal at 91%, gas at 61%** | **6,024** | **§370** |

**The Department chooses 91% for the coal plant and 61% for the natural gas plant.**

The resulting capacity is:

```math
\text{Coal} = \lfloor6000\times0.82\times0.98\rfloor=4821
```

```math
\text{Gas} = \lfloor3000\times0.40125\rfloor=1203
```

Together, they provide **6,024 units**, enough to satisfy the requirement.

Monthly expenses are:

```math
\text{Coal} = \mathrm{round}(253\times0.91)=230
```

```math
\text{Gas} = \mathrm{round}(230\times0.61)=140
```

<grid columns={2} gap={3}>
  <grid-item>
    <box gap={1}>
      <text color="secondary" size="xs">Compared with 100% funding</text>
      <title size="xl" color="success">§113/month saved</title>
    </box>
  </grid-item>
  <grid-item>
    <box gap={1}>
      <text color="secondary" size="xs">Compared with equal cuts</text>
      <title size="xl" color="success">§21/month saved</title>
    </box>
  </grid-item>
</grid>

This is possible because **power plant efficiency is not proportional to funding**.

In this case, keeping the coal plant near full efficiency and the gas plant near minimum funding is cheaper than reducing both equally.

**What if the city sells power?**

Suppose it signs a contract to sell **1,000 units** to a neighboring city.

The required local capacity increases from 6,000 to **7,000 units**.

The Department's cheapest combination becomes:

| Plant | Funding | Available capacity |
|---|---:|---:|
| Coal | 100% | 5,880 |
| Natural gas | 60% | 1,162 |
| **Total** | | **7,042** |

The monthly operating cost is **§391**.

The Department therefore increases production to cover the export contract without sacrificing the city's own supply.

**What if demand becomes too high?**

At 120% funding, these two plants can provide a combined maximum of **9,768 units**.

If the required local capacity exceeds that amount, the Department raises the plants to their highest-capacity settings, at a combined monthly cost of **§580**.

If regional contract management is enabled, it also tries to purchase enough power to cover the remaining shortage.

Otherwise, the player must provide additional generating capacity.

<a id="power-example-trade"></a>
#### Example 4: Buying power from a neighboring city

Consider the same two-plant city from Example 3.

Its power demand is 5,000 units, but it now has an active contract to buy **2,000 units** from a neighboring city.

The local power plants only need to supply the remaining 3,000 units, plus the 20% reserve.

```math
N = \max(3000\times1.20,\ 5050-2000)=3600
```

**Required local capacity: 3,600 units.**

The Department chooses:

| Plant | Funding | Capacity | Monthly cost |
|---|---:|---:|---:|
| Coal | 62% | 2,440 | §157 |
| Natural gas | 60% | 1,162 | §138 |
| **Total** | | **3,602** | **§295** |

Without the purchase, the plants cost §370 per month to provide the city's power and reserve.

With the purchase, their operating cost falls to **§295 per month**.

That is a reduction of **§75 per month in local power plant expenses**.

However, the city must also pay for the imported power.

**Whether the purchase actually saves money depends on the contract price.**

The Department's regional contract planner compares the purchase cost with the savings in local production before deciding whether the contract is worthwhile.

See [Neighbor Deals](deals.md#neighbor-deals).

**Why not reduce the plants further?**

The Department normally does not select funding below 60%.

Even if both plants are funded at 60%, they still provide approximately 3,440 units and cost §290 per month.

This minimum funding prevents the Department from reducing unnecessary plants to zero through its normal optimization.

<a id="power-example-wear"></a>
#### Example 5: Does considering plant aging change the result?

Return to the coal power plant from Example 1.

Its characteristics are:

- **Replacement cost:** §10,000.
- **Current condition:** 98%.
- **Aged threshold:** 90%.
- **Base aging rate:** 0.001111 per month.
- **Observed usage before optimization:** 59.5%.

Now enable **Consider power plant aging**.

The Department estimates the economic cost of wear at each possible funding level, using the plant's currently observed usage.

| Funding | Monthly operating cost | Estimated wear cost | Total modeled cost |
|---|---:|---:|---:|
| **84%** | **§213** | **§4** | **§217** |
| 85% | §215 | §4 | §219 |
| 100% | §253 | §4 | §257 |
| 120% | §304 | §4 | §308 |

**The Department still chooses 84% funding.**

Why?

Because the difference in estimated wear cost is too small to justify spending more on the plant's monthly budget.

The §4 wear cost is not taken from your treasury. It is an economic estimate used only when comparing choices.

**What about the two-plant city?**

In Example 3, with observed usage of 50% for the coal plant and 60% for the gas plant, enabling aging still produces the same result:

- Coal plant: 91%.
- Natural gas plant: 61%.
- Monthly operating cost: §370.
- Estimated wear: §7.
- Total modeled cost: §377.

Again, the cheapest economic combination is also the cheapest operating combination.

**Can aging ever make a difference?**

Yes, particularly for expensive plants.

For example, consider a plant with:

- Replacement cost: §40,000.
- Base aging rate: 0.001389 per month.
- Observed usage: 90%.

Its estimated monthly wear cost is:

| Funding | Estimated wear cost |
|---|---:|
| 60% | §45 |
| 100% | §43 |
| 120% | §39 |

The difference between 60% and 120% funding is **§6 per month**.

That can influence the choice when two funding combinations have almost identical operating expenses.

However, it does not automatically make higher funding preferable.

**How long can a power plant last?**

Using the same aging model, the coal plant from Example 1 would take approximately the following number of simulated months to deteriorate from 98% condition to its 90% aged threshold, assuming constant usage and 100% funding:

| Plant usage | Estimated time to aged threshold |
|---|---:|
| 50% | 2,560 months |
| 80% | 1,170 months |
| 90% | 920 months |
| 100% | 740 months |

These are **theoretical estimates**, not guaranteed lifespans.

The actual game may behave differently as usage, funding and condition change.

**An important limitation of the aging option**

The Department uses each plant's **currently observed usage** to estimate wear for possible funding levels.

It does not predict exactly how the game will redistribute power demand after those funding changes.

As Example 1 demonstrates, reducing funding can increase real usage substantially, which may accelerate aging more than the initial economic estimate suggests.

The Department observes the new usage during subsequent evaluations.

**Consider power plant aging is therefore an economic estimate, not a guarantee of longer plant life.**

<a id="power-limitations"></a>
### 7. Known limitations

The Department manages power funding automatically, but several aspects remain controlled by SimCity 4 or by the player.

#### Power plant compatibility

- **The original game's efficiency and aging curves are built into the Department.** If another mod changes those curves, the Department does not automatically detect the new values.
- **Individual plant properties are read from the game.** Maximum output, condition, operating cost, aging parameters and construction cost are obtained from each plant, allowing compatible custom power plants to be managed.
- **Some plants cannot be managed safely.** Unsupported funding systems or unreadable information cause a plant to be excluded from automatic funding changes.
- **Waste-to-Energy plants currently burning garbage are not adjusted.** Their funding is left unchanged because the same building is involved in sanitation. Their available generating capacity still counts towards the power plan.

#### Funding and generating capacity

- **The Department does not build or replace plants.** A genuine shortage requires additional generating capacity or power purchases.
- **The normal minimum funding is 60%.** A city with far more power plants than it needs may still pay at least 60% of each managed plant's nominal operating cost.
- **Power plants are never deliberately switched off through funding.** The Department does not select new targets below its normal minimum.
- **Very large cities use capacity rounding.** When required capacity exceeds 20,000 units, the optimizer may retain slightly more generating capacity than strictly necessary. It never deliberately rounds a plan below the requirement.

#### Power plant aging

- **The aging calculation uses currently observed usage.** It does not predict exactly how a funding change will redistribute power generation among plants.
- **The aging time factor is based on measurements.** The factor of 1/16 was observed in testing rather than read directly from the game's internal data.
- **Wear costs and lifetime estimates are approximate.** They are intended to support economic decisions, not predict an exact replacement date.
- **The Department never replaces aged plants automatically.** The player remains responsible for constructing replacements when needed.

#### Power distribution and regional trade

- **The game's power requirement is used as reported.** The Department does not attempt to replace it with its own estimate of the city's total needs.
- **Connection problems are not production problems.** If sufficient power exists but cannot reach some buildings, changing power plant funding will not solve the issue.
- **Power contracts affect the funding plan.** Imports and exports are included when calculating local capacity requirements, but only usable purchases are treated as available supply.

**The Department's role is to manage the power your city can produce, not to redesign the network or construct new power plants.**

[Back to the contents](index.md#contents) | [Back to the README](../../README.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
