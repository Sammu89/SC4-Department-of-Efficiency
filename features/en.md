**English** | [Português](pt.md) | [Français](fr.md) | [Español](es.md)

[Back to the README](../README.md)

# Department of Efficiency: Technical Reference

This guide explains how SimCity 4 works behind the scenes and how the Department of Efficiency makes its decisions, service by service. It is intended for players who want to understand the reasoning behind the Department's actions, as well as modders interested in the game's underlying mechanics.

## Quick Reference

| Service | Main reserve or setting | Default | Main protection | Chapter |
|---|---|---|---|---|
| Education | Capacity safety margin | 5% | Maintains spare capacity to accommodate fluctuations in student attendance. School bus funding is left unchanged. | [Education](#education-healthcare) |
| Healthcare | Capacity safety margin | 5% | Maintains spare capacity to accommodate fluctuations in patient numbers. Ambulance funding is left unchanged. | [Healthcare](#education-healthcare) |
| Electricity | Power reserve | 20% | Maintains spare generating capacity to accommodate city growth and reduce the risk of blackouts. Power lines are also protected against deterioration. | [Electricity](#electricity) |
| Water | Water reserve | 10% | Maintains spare water production capacity, with pipe protection enabled by default. | [Water](#water) |
| Sanitation | Garbage reserve | 2% | Ensures sufficient waste treatment and disposal capacity, aiming to clear any accumulated garbage by the end of the simulated month. | [Garbage & Landfills](#garbage-landfills) |
| Landfill advice | Savings alert threshold | Advice off; threshold 25% | Recommends landfill expansions or reductions when they could significantly lower sanitation costs. | [Landfill Size Advice](#garbage-landfills-advice) |
| Neighbor deals | Management by resource | On; dealing between neighbors  off | Manages electricity, water and garbage contracts while prioritizing the city's own needs and safety reserves. | [Neighbor Deals](#neighbor-deals) |
| Demolition | Waiting periods | Ruins: 1 month; abandoned buildings: 12 months | Protects municipal and historic buildings by default, with an optional monthly demolition spending limit. | [Automatic Demolition](#demolition) |

<a id="contents"></a>

## Contents

* [A. Education & Healthcare](#education-healthcare)
* [B. Electricity](#electricity)
* [C. Water](#water)
* [D. Garbage & Landfills](#garbage-landfills)
  * [Landfill size advice](#garbage-landfills-advice)
  * [Ghost squares](#ghost-squares)
* [E. Neighbor Deals](#neighbor-deals)
* [F. Automatic Demolition](#demolition)
* [G. Safety, Automation & Decision-Making](#safety)
  * [Advisor notices](#notices)
* [H. Option reference](#options)

<a id="education-healthcare"></a>
## A. Education & Healthcare

The Department automatically adjusts the funding of each school, college, university, library, museum, clinic and hospital it can manage.

Its goal is simple: **provide enough capacity for everyone who needs the service, with a safety margin, while spending as little as possible.**

Each building is managed individually. A school with 200 students does not need the same budget as one with 2,000 students.

The Department only adjusts the building's operating budget. **It never changes school bus or ambulance funding, building coverage areas, or the number of facilities in your city.** Those decisions remain yours.

### How funding affects capacity

In SimCity 4, each education and healthcare building has an operating budget, displayed in simoleons (§) per month. Behind the scenes, the game represents this funding as a percentage.

Funding determines both how much the building costs and how many students, patients, visitors or books it can accommodate.

The relationship is not entirely proportional:

| Funding | Available capacity | Monthly operating cost |
|---|---:|---:|
| 50% | 50% | 50% |
| 75% | 75% | 75% |
| 100% | 100% | 100% |
| 120% | 110% | 120% |

The percentages are relative to the building's capacity and operating cost at 100% funding.

**Up to 100% funding**, capacity increases at the same rate as spending. **Above 100%**, each additional percentage point of funding provides only half as much extra capacity.

For example, a school with 1,000 places at 100% funding can accommodate approximately 500 students at 50%, or 1,100 students at 120%.

<a id="education-healthcare-capacity-curve"></a>
**Capacity formula (base game)**

```math
\phi(f) = \min(f,1) + \frac{\max(0,f-1)}{2}
```

```math
C = \left\lfloor B \times \phi(f) \right\rfloor
```

Where:

- **B** is the building's capacity at 100% funding.
- **f** is the funding level (1.0 = 100%).
- **C** is the resulting capacity.
- **φ(f)** is the game's capacity multiplier.

The game uses internal rounding and may apply additional capacity modifiers. As a result, the displayed capacity can sometimes differ slightly from a straightforward calculation. The Department accounts for these differences when making decisions.

**Monthly operating cost (base game)**


```math
K = \mathrm{round}(K_0 \times f)
```

Where **K₀** is the building's monthly operating cost at 100% funding.

For example, a school costing §200 per month at 100% funding costs approximately §100 at 50%.

### How the Department chooses funding

The Department checks how many people are using each building and calculates the capacity it needs to maintain.

By default, it aims to keep **at least 5% spare capacity**, but it can maintain a larger margin when necessary.

This is particularly important for small facilities, where even a small increase in attendance can represent a large percentage change.

For example, a clinic going from 40 to 60 patients has experienced a 50% increase in demand. The Department takes this kind of fluctuation into account rather than blindly applying the same margin to every building.

It then chooses the **lowest funding level that can safely meet the required capacity**.

If demand increases, funding can be raised. If demand falls, the Department may reduce funding to avoid unnecessary spending.

### How often does it check and adjust funding?

All timings below refer to **simulated game time**, not real-world time.

| When | What the Department does |
|---|---|
| **When a city opens** | Performs its first full evaluation on the first simulated day. If some buildings do not yet have usable data, it can retry over the following days. |
| **Every 2 simulated days** | Checks for buildings that may need urgent funding increases. |
| **Every simulated day during a strike** | Checks for urgent problems so that funding can be raised as soon as possible. |
| **Once per simulated month** | Fully reviews each building and adjusts funding upwards or downwards according to current needs. |

**Funding increases do not always wait until the next month.** If a building becomes seriously overcrowded, or is on strike, the Department can raise funding during its more frequent checks.

**Funding reductions happen only during the monthly review.** This avoids cutting budgets too quickly after a temporary drop in attendance.

The Department never lowers funding while the game's own figures indicate a risk of inadequate service.

<a id="education-healthcare-strike"></a>
### How the Department helps prevent strikes

In SimCity 4, schools and healthcare facilities can go on strike when their capacity is too low compared with demand.

The game uses a **coverage ratio** to determine this risk.

```math
V = \min\left(1,\frac{C}{U}\right)
```

Where:

- **C** is the building's available capacity.
- **U** is its demand.
- **V** is the coverage ratio.

A coverage ratio of **1.0 (100%)** means the building has enough capacity for its demand.

A ratio below **0.8 (80%)** creates a risk of strikes. The lower the ratio, the greater the risk.

**Strike probability (base game)**

```math
S(V) = 100 \times \max\left(0,1-\frac{V}{0.8}\right)
```

| Coverage | Strike probability |
|---|---:|
| 100% | 0% |
| 80% | 0% |
| 60% | 25% |
| 40% | 50% |
| 0% | 100% |

A strike ends when the game's coverage ratio reaches 90%.

The Department normally keeps more capacity than the minimum needed to avoid strikes. It also monitors buildings for unexpected increases in demand.

If a strike is already underway, **the Department raises the building's funding to its configured maximum**, provided it can control that building.

The game's internal coverage calculation can sometimes differ from the demand shown in the building's query window. For that reason, the Department also uses the game's own coverage information to avoid unsafe funding reductions.

### What happens when funding is no longer enough?

Funding cannot solve every capacity problem.

At 120% funding, a building provides only about 110% of its normal capacity. If demand exceeds what the building can safely handle at its maximum funding, the Department cannot create additional places.

**You must build another facility.**

When the Department confirms that a shortage cannot be resolved through funding and persists for **30 simulated days**, it sends an in-game advisor notice.

Where supported, the notice includes a link that takes you directly to the affected building.

The Department does not construct buildings or change their coverage areas. You remain responsible for deciding where new schools, hospitals and other facilities should be placed.

For additional safeguards and special cases, see [Safety conditions](#education-healthcare-safety).
<a id="education-healthcare-problem"></a>
### 2. The management problem

Managing education and healthcare budgets manually is more complicated than it looks.

- **Wasted money:** A half-empty school does not need the same funding as a full one. Keeping every building at 100% can waste thousands of simoleons over time.
- **Risk of strikes:** Cutting funding too far reduces capacity and can cause strikes. The Department must save money without putting services at risk.
- **Changing demand:** Student and patient numbers fluctuate as the city grows. Some changes can be sudden, particularly when students move between school levels.
- **Small buildings:** A clinic going from 40 to 60 patients experiences a 50% increase in demand. Small facilities need extra protection against these fluctuations.
- **New buildings:** The game may take a few simulated days to report how many people are using a newly placed facility.
- **Limited capacity:** Even at 120% funding, a building can provide only about 110% of its normal capacity. Beyond that, the player must build another facility.
- **Too many buildings to check:** In a large city, manually reviewing every school, library, museum, clinic and hospital every month becomes tedious.

**The Department solves this by monitoring each building individually, maintaining enough capacity to keep services running, and reducing unnecessary spending whenever it is safe to do so.**

<a id="education-healthcare-algorithm"></a>
### 3. How the Department makes its decisions

<a id="education-healthcare-eligibility"></a>
#### 3.1 Which buildings can be managed?

The Department automatically manages education and healthcare buildings that use the game's standard funding system.

This includes:

| Education | Healthcare |
|---|---|
| Elementary schools | Clinics |
| High schools | Hospitals |
| Colleges and universities | Disease research centers |
| Libraries | |
| Museums | |

Compatible custom buildings can also be managed, provided they use the same funding mechanisms as the original game.

Before managing a building, the Department checks that:

1. The game identifies it as an education or healthcare facility.
2. It has its own operating budget that can actually be adjusted.
3. Its query window provides recognizable demand and capacity information.
4. Its funding and budget figures are consistent.

**If a building does not meet these conditions, the Department leaves it alone.**

For example, the game's original private schools do not have their own adjustable operating budget, so there is nothing for the Department to manage. Some custom buildings also use special funding systems or query windows that the Department cannot safely control.

Buildings are identified by how they work, **not by their names**. A compatible custom school can therefore be managed just like an original SimCity 4 school.

Police stations, fire stations and jails are not managed by these services.

<a id="education-healthcare-required"></a>
#### 3.2 How much capacity does a building need?

The Department does not simply match funding to current attendance. It keeps **extra capacity to handle unexpected increases in demand**.

The default safety margin is **5%**, but the Department may automatically maintain more when a building needs additional protection.

It considers three factors:

**1. Your chosen safety margin**

By default, a building serving 1,000 people should have capacity for at least 1,050.

You can change this margin in the Department's settings.

**2. Previous increases in demand**

The Department remembers how much a building's usage has increased from one month to the next.

It uses up to **36 monthly observations from the current game session**, looking for the largest increase. Only comparisons starting from at least 50 users are included, because very small numbers can produce misleading percentages.

For safety, the increase used in the calculation is capped at **50%**.

High schools, colleges and universities receive special protection. Student transfers between education levels can cause large annual increases, so the Department assumes a possible 50% increase until it has collected 13 monthly observations.

**Largest monthly increase**

```math
v = \min\left(0.5,\ \max\left(0,\ \max_i\left(\frac{U_i}{U_{i-1}}-1\right)\right)\right)
```

Where:

- **Uᵢ** is the number of users recorded in month *i*.
- **v** is the largest valid monthly increase, expressed as a fraction and capped at 0.5.

**3. Extra protection for small buildings**

Small facilities can experience large percentage changes even when only a few people arrive.

The Department therefore keeps additional capacity:

| Current usage | Minimum capacity margin |
|---|---|
| Fewer than 100 users | 50% extra capacity |
| 100–199 users | 25% extra capacity |
| 200 or more users | Your chosen margin, adjusted for demand fluctuations when necessary |

For example, a clinic with 60 patients keeps capacity for at least 90 patients, even if your configured safety margin is only 5%.

This additional protection is particularly useful for small buildings, where the extra operating cost is usually modest.

**How these protections work together**

The Department compares the three requirements and uses whichever calls for the most capacity:

- Your configured safety margin.
- Protection against previous increases in demand and strikes.
- Extra protection for small buildings.

**Safety margin formula**

```math
M = \max\left(1+\frac{r}{100},\ 0.85(1+v),\ s(U)\right)
```

Where:

- **r** is your configured safety margin, 5% by default.
- **v** is the largest monthly increase described above.
- **U** is the building's current usage.
- **s(U)** is 1.5 below 100 users, 1.25 from 100 to 199 users, and 0 otherwise.
- **M** is the resulting capacity multiplier.

The factor of 0.85 in the second rule aims to keep coverage above the game's 80% strike-risk threshold, even after a previously observed rise in demand.

**Required capacity**

```math
R = \left\lceil U \times M \right\rceil
```

Where **R** is the minimum capacity the Department wants the building to provide.

For example, a school with 610 students, no unusually large demand increases and a 5% safety margin needs:

```math
R = \lceil 610 \times 1.05 \rceil = 641
```

The Department therefore looks for the cheapest funding level that provides at least 641 places, with a small allowance for the game's rounding behavior.

<a id="education-healthcare-choice"></a>
#### 3.3 How does the Department choose funding?

Once the required capacity is known, the Department checks which funding level can provide it.

It chooses **the lowest whole percentage that safely meets the requirement**.

By default, it considers funding levels from **1% to 120%**.

**Funding choice**

```math
p^* = \min\left\{p\in[p_{\min},p_{\max}]: C(p)-1\ge R\right\}
```

Where:

- **p** is a possible funding percentage.
- **C(p)** is the expected capacity at that funding.
- **R** is the required capacity calculated above.
- The subtraction of one unit provides a small safeguard against the game's capacity rounding.

For example, if a school needs 641 places and 54% funding provides approximately 647, the Department selects **54%**, assuming 53% cannot provide enough capacity.

This avoids paying for hundreds of unused places.

**Additional safety rules**

The Department also follows several important rules:

- **No unnecessary changes:** If funding is already at the appropriate level, it leaves it alone.
- **Strikes:** If a building is on strike, its funding is raised directly to the configured maximum, normally 120%.
- **New buildings:** If the game has not yet reported attendance, the Department does not assume the building is empty. It keeps funding at least at 100%, or at the configured maximum if that is lower, until usable data becomes available.
- **No unsafe reductions:** Funding is never lowered while the game's reported coverage is below 85% or while a strike is in progress.
- **No zero funding:** Under the default configuration, the Department never sets a building's funding to 0%.
- **Reductions are monthly:** Funding can be increased during urgent checks, but reductions are only made during the monthly review.

If even the maximum funding cannot provide the desired capacity, the Department keeps the building at its maximum and checks whether there is a genuine shortage.

A building falling slightly below its safety margin does not necessarily need another facility. The Department only warns you when the available capacity is genuinely insufficient and the problem persists.

See [Shortages](#education-healthcare-shortage) for the conditions that trigger an advisor notice.

<a id="education-healthcare-capacity-evidence"></a>
#### 3.4 What if the game's actual capacity differs from the calculation?

SimCity 4 does not always report exactly the capacity predicted by its standard funding formula.

Small rounding differences are normal. Some cities or custom buildings may also apply additional capacity modifiers.

The Department therefore **checks the actual capacity reported by the game before changing a building's funding**.

It handles three situations:

| Situation | What the Department does |
|---|---|
| **Capacity matches the calculation** | Uses the standard formula and manages the building normally. A difference of one place is accepted as normal rounding. |
| **Capacity follows a consistent modifier** | Learns the difference and adjusts its calculations, taking precautions until the modifier is confirmed. |
| **Capacity behaves unpredictably** | Avoids automatic funding reductions rather than risking an unexpected loss of capacity. |

**Learning a building's real capacity**

For example, a custom school might consistently provide 15% more capacity than the standard formula predicts.

Instead of ignoring that extra capacity or immediately relying on it, the Department checks whether the difference is consistent.

When appropriate, it can make one small, safe funding increase during a monthly review to confirm how capacity responds. It then uses the verified relationship to choose funding more accurately.

This calibration is conservative: it is only attempted when the building already has enough capacity, and it does not assume an unconfirmed bonus.

If the capacity cannot be reliably predicted, the Department leaves the building's funding unchanged or limits itself to safe increases when the observed capacity is insufficient.

**The priority is always to avoid unsafe funding cuts.** Saving a few simoleons is not worth risking a school or hospital whose capacity the game reports inconsistently.

<a id="education-healthcare-timing"></a>
#### 3.5 When does the Department check and adjust funding?

The Department monitors education and healthcare buildings throughout the simulation. **It can increase funding when a problem appears, but only reduces funding during its monthly review.**

All timings refer to simulated game time.

| When | What happens | Funding increases | Funding reductions |
|---|---|---|---|
| **When a city opens** | Checks every managed building on the first simulated day. If some buildings have no attendance data yet, it can repeat the check over the following days, up to six times in total. | Yes | No |
| **Every 2 simulated days** | Checks for buildings that need urgent attention because of overcrowding or insufficient funding. | Yes | No |
| **Every day during a strike** | Checks for urgent funding problems while an education or healthcare strike is reported. | Yes | No |
| **Every simulated month** | Reviews every building, updates attendance history and adjusts funding to current needs. | Yes | Yes |

**What counts as an urgent problem?**

The Department intervenes between monthly reviews when a building:

- Is not already at its maximum funding.
- Needs a funding increase according to its safety rules.
- Is on strike, or its current capacity is more than 3% below its demand.

The last condition is expressed as:

```math
C < 0.97 \times U
```

Where:

- **C** is the building's available capacity.
- **U** is its current usage.

For example, a school with capacity for 900 students and attendance of 1,000 is clearly overcrowded. The Department can increase its funding without waiting for the next month.

A new or changing problem is evaluated during the regular checks. If the same problem persists without changing, the Department can repeat its evaluation every **7 simulated days**.

A building already at maximum funding cannot be helped by another increase. The Department treats that situation as a possible capacity shortage instead.

**The priority is always to restore services first and optimize spending afterwards.**

<a id="education-healthcare-buses"></a>
#### 3.6 What does the Department change, and how does it check its work?

The Department changes **only the operating budget of each eligible building**, using the same funding system as the game's own budget slider.

It never changes:

- School bus funding.
- Ambulance funding.
- Building coverage areas.
- Building locations or the number of facilities.

**Every funding change is checked before and after it happens.**

Before changing a budget, the Department reads the building's current settings again. If you have moved its slider yourself since the calculation was made, the Department skips that change and recalculates later.

After making a change, it verifies that:

1. The new funding is correctly reported by the game.
2. The building's monthly operating cost is consistent with the new funding.
3. The available capacity responds as expected.
4. School bus or ambulance funding, where present, remains unchanged.

**What happens if something goes wrong?**

If a change cannot be verified, the Department stops managing that particular building for the rest of the current city session.

If three different buildings fail these checks, it stops making education and healthcare funding changes altogether until the city is reopened.

This prevents repeated attempts to modify buildings whose funding systems behave unexpectedly.

**Can you still change a budget manually?**

Yes. You can adjust any building's funding yourself. The Department detects the change and considers it during the next evaluation, provided that building is still under automatic management.

If you want to keep complete manual control of these budgets, disable **Manage education** or **Manage health** in the Department's settings.

<a id="education-healthcare-shortage"></a>
#### 3.7 What happens when funding is not enough?

Some buildings become so overcrowded that even maximum funding cannot provide enough capacity.

In that situation, **the Department cannot solve the problem. You need another building.**

However, the Department does not immediately treat every overcrowded building as a shortage.

It first checks whether more funding could resolve the problem.

| Situation | What the Department does |
|---|---|
| **Funding is below the maximum** | Increases funding when necessary. It does not yet consider the shortage unavoidable. |
| **Funding is at maximum, with enough capacity** | Keeps the building funded and does not issue a shortage notice. |
| **Funding is at maximum, but coverage is below 85%** | Confirms that the building cannot adequately serve its current demand. |
| **The building is on strike but could normally provide sufficient capacity** | Treats the strike as a temporary problem rather than proof that a new building is needed. |

**Maximum attainable coverage**

```math
V_{\max} = \min\left(1,\frac{C_{\max}}{U}\right)
```

Where:

- **Cₘₐₓ** is the capacity available at maximum funding.
- **U** is the building's current demand.
- **Vₘₐₓ** is the highest coverage the building can achieve.

If the building is on strike and its reported capacity is temporarily affected, the Department estimates what its capacity would be without the strike.

**A shortage is confirmed when maximum attainable coverage is below 85%.**

For example, a hospital that can accommodate 1,500 patients at maximum funding but has demand for 2,000 patients can cover only 75% of that demand.

No funding increase can solve this. Another healthcare facility is needed.

The Department still waits before notifying you, to avoid reporting temporary problems that may resolve themselves.

<a id="education-healthcare-notices"></a>
#### 3.8 When and how does the Department notify you?

The Department tries to handle everyday funding problems silently.

**You do not receive messages every time it saves money, increases funding or prevents a possible strike.**

It notifies you when a building genuinely needs your intervention.

**When is a notice sent?**

A shortage must remain confirmed at every evaluation for **30 simulated days** before the Department issues a notice.

If the building recovers during that period, the countdown starts over if the shortage returns.

Buildings currently on strike are not included in shortage notices, because SimCity 4 already provides its own strike warnings.

**What does the notice show?**

The notice appears in the game's normal advisor news list.

It tells you which type of building needs more capacity and provides clickable links that take you directly to the affected locations.

For example, an overcrowded school notice might say:

> **Too Many Students, Too Little Room!**
>
> Mayor *(your name)*, this elementary school is bursting at the seams! Funding is already at maximum, but the students keep coming. Time to build another school, before recess becomes an outdoor learning program! For a closer look, visit **the school** where the desks have called for backup.

In the game, **the school** is a clickable link that moves the camera to the affected building.

**How are multiple shortages handled?**

- The Department groups shortages by building type, such as elementary schools, hospitals or libraries.
- Each notice can list up to **10 buildings**, with the most overcrowded shown first.
- Other affected buildings continue to be monitored and can appear when earlier shortages are resolved.
- If the buildings affected change, the Department updates the existing notice whenever possible rather than creating unnecessary new messages.
- If the number of listed buildings changes, the old notice is replaced. Repeated changes can trigger a **30-day pause** between replacements to avoid flooding the advisor list.
- When every building covered by a shortage notice has recovered, the notice is removed.

**Will the same warning appear every time you reopen your city?**

No. The Department remembers which shortages it has already reported for each identifiable city.

If you close and reopen a city with the same unresolved problem, it normally does not repeat the warning. Dismissing a notice also prevents the same unchanged shortage from being announced again.

This information is stored separately from your city saves.

**Where do notices appear?**

Education and healthcare shortage notices are delivered through the game's own advisors. They appear in the advisor news list without opening an intrusive pop-up window.

For the notification system shared with other services, see [Advisor Notices](#notices).

<a id="education-healthcare-safety"></a>
### 4. Safety conditions

The Department follows several safeguards to avoid reducing services incorrectly.

**Before changing funding**

- It only manages buildings whose standard funding system can be identified and verified.
- It never assumes that missing attendance data means a building is empty.
- It checks the building's funding and capacity again before making a change.
- It leaves buildings with unreliable or inconsistent readings alone.

**When reducing funding**

- Reductions are made only during the monthly review.
- Funding is never reduced while a building is on strike.
- Funding is never reduced when the game's reported coverage is below 85%.
- Buildings without usable attendance data are never underfunded.
- Buildings whose capacity cannot be reliably predicted are protected against automatic reductions.
- Under the default configuration, funding never goes below 1%.

**When increasing funding**

- A building on strike is raised to its configured maximum funding.
- Buildings with urgent capacity problems can receive increases between monthly reviews.
- If maximum funding cannot solve a genuine shortage, the Department leaves the building at maximum funding and, when the problem persists, notifies you.

**After making a change**

- The Department checks that the new budget and capacity are consistent.
- School bus and ambulance funding must remain unchanged.
- A building that fails verification is excluded from further automatic changes during the session.
- Three different verification failures stop education and healthcare funding changes for that session.

**When automatic management is disabled**

Switching the Department off does not restore previous budgets. Every building keeps its current funding until you change it manually or reactivate management.

#### The game's original advisor messages

SimCity 4 normally displays warnings when education or healthcare funding falls below certain percentages.

For example, the game may complain about education budgets below 90%, even when an individual school has more than enough capacity at a much lower funding level.

These warnings are not always relevant when the Department is managing buildings individually.

To avoid misleading messages, the plugin includes a small data file that adjusts some of the game's original education, healthcare and budget advice:

- Routine warnings based only on low funding percentages are suppressed.
- Certain warnings about overfunding or staffing levels are also suppressed.
- The relevant pre-strike warnings are restricted to situations where a strike is actually taking place.
- **Real strike alerts remain active.**
- The game's normal praise and status messages are preserved.

This adjustment is separate from the Department's automatic funding switches. It remains active while the corresponding data file is installed, even if you disable education or healthcare management.

If you want the game's original funding advice back, you can remove the corresponding advice data file while SimCity 4 is closed.

See [General Safety](#safety) for more information.

<a id="education-healthcare-settings"></a>
### 5. Settings

Education and healthcare management can be configured directly in **Play Options → Department of Efficiency**, under the **Services** page.

| Setting | Default | Range | What it does |
|---|---|---|---|
| [Department active](#opt-master) | On | On / Off | Enables or disables the entire Department. |
| [Manage education](#opt-education) | On | On / Off | Automatically manages the funding of eligible schools, colleges, universities, libraries and museums. |
| [Safety margin (education)](#opt-educationreserve) | 5% | 0–50% | Sets the minimum spare capacity the Department aims to maintain in education buildings. |
| [Manage health](#opt-health) | On | On / Off | Automatically manages the funding of eligible clinics, hospitals and disease research centers. |
| [Safety margin (health)](#opt-healthreserve) | 5% | 0–50% | Sets the minimum spare capacity the Department aims to maintain in healthcare buildings. |

**How does the safety margin work?**

A 5% safety margin means that a building with 1,000 users should have capacity for at least 1,050.

However, **5% is a minimum, not a fixed target**. The Department may keep more capacity when a building is small or has experienced large increases in demand.

Increasing the margin provides an additional buffer against sudden growth, but may also increase operating costs.

Changes made through the in-game settings window apply immediately. If you edit the configuration files manually, those changes are read at the next simulated month.

Invalid settings are not silently corrected. An out-of-range value or unknown setting can cause education and healthcare management to remain disabled until the configuration is corrected.

#### Advanced settings (INI file only)

Two additional settings are available in the configuration file. They do not appear in the game's options window.

| Setting | Default | Allowed range | What it does |
|---|---|---|---|
| `MinFundingPercent` | 1% | 0–100% | Sets the lowest funding percentage the Department may choose. |
| `MaxFundingPercent` | 120% | 1–120% | Sets the highest funding percentage the Department may choose, including during strikes. |

**Minimum funding:** The default of 1% prevents the Department from completely removing a building's operating budget, even when it has no reported users.

**Maximum funding:** The default of 120% allows buildings to operate at the highest funding level available in the original game. Lowering this limit restricts spending, but also reduces how much capacity the Department can provide during overcrowding or strikes.

The minimum must not exceed the maximum.

For most players, **the default values are recommended**.

<a id="education-healthcare-examples"></a>
### 6. Worked examples

The following examples show how the Department adjusts funding in different situations.

Building capacities and operating costs are illustrative. Your figures may differ depending on the building and the mods you use.

The game also uses internal rounding, so displayed capacities can occasionally differ by one unit from a simple percentage calculation.

#### Example 1: A school with more capacity than it needs

An elementary school has:

- **Maximum normal capacity:** 1,200 students.
- **Current attendance:** 610 students.
- **Operating cost at 100% funding:** §187/month.
- **Safety margin:** 5%.

The Department calculates the required capacity:

```math
R = \lceil 610 \times 1.05 \rceil = 641
```

It then compares funding levels:

| Funding | Available capacity | Result |
|---|---:|---|
| 53% | 635 | Not enough |
| 54% | 647 | Enough |
| 100% | 1,200 | More than necessary |

**The Department chooses 54% funding.**

The monthly operating cost falls from §187 to approximately **§101**, saving **§86 per month**.

School bus funding remains unchanged.

#### Example 2: A small clinic needs a larger safety margin

A clinic has a normal capacity of 1,400 patients but is currently serving only 60.

Although the configured safety margin is 5%, small buildings receive additional protection against sudden changes in attendance.

Because this clinic has fewer than 100 patients, the Department keeps **50% extra capacity**:

```math
R = \lceil 60 \times 1.5 \rceil = 90
```

At 6% funding, the clinic can serve only 84 patients. At 7%, it can serve approximately 98.

**The Department chooses 7% funding.**

This is slightly more than the usual 5% margin would require, but provides useful protection against sudden increases at a very small additional cost.

#### Example 3: A high school with seasonal changes in attendance

A high school has:

- **Normal capacity:** 5,000 students.
- **Current attendance:** 900 students.
- **Less than 13 months of attendance history in the current session.**

High schools can receive large numbers of students when pupils move up from elementary school.

Until enough monthly observations have been collected, the Department assumes a possible **50% increase in attendance**.

This gives a required capacity of:

```math
R = \lceil 900 \times 0.85 \times 1.5 \rceil = 1148
```

**The Department chooses 23% funding**, providing approximately 1,149 places.

After 13 monthly observations, suppose the largest increase actually recorded was only 8%.

The Department can then rely on the observed demand pattern and return to the normal 5% safety margin.

Required capacity becomes 945 students.

**Funding can fall to 19%**, providing approximately 949 places.

The Department avoids unnecessary spending while still accounting for seasonal demand.

#### Example 4: A library with unused capacity

A library has a collection capacity of 40,000 books, but only 9,000 are currently checked out.

With a 5% margin, the Department needs capacity for:

```math
R = \lceil 9000 \times 1.05 \rceil = 9450
```

At 23% funding, capacity is approximately 9,199 books, which is insufficient.

At 24%, capacity reaches approximately 9,599.

**The Department chooses 24% funding.**

The library continues serving its readers without paying for its entire unused collection capacity.

#### Example 5: A museum with few visitors

A museum can accommodate 2,800 visitors but currently receives only 150.

Because it serves fewer than 200 visitors, the Department keeps a **25% safety margin**.

```math
R = \lceil 150 \times 1.25 \rceil = 188
```

At 7% funding, the museum has capacity for approximately 196 visitors.

**The Department chooses 7% funding.**

The museum remains operational, with enough spare capacity for fluctuations in attendance.

#### Example 6: A sudden increase between monthly reviews

Consider the elementary school from Example 1.

It currently has:

- **Funding:** 54%.
- **Capacity:** 647 students.
- **Attendance:** 610 students.

In the middle of the month, attendance suddenly increases to **700 students**.

The school is now seriously overcrowded.

The Department checks for urgent problems every two simulated days. Because available capacity is more than 3% below demand, it does not wait until the next monthly review.

The new required capacity is:

```math
R = \lceil 700 \times 1.05 \rceil = 735
```

**Funding is increased from 54% to 62%**, providing approximately 744 places.

If attendance subsequently falls, the Department waits until the next monthly review before reducing the budget.

This is intentional: **funding increases protect services immediately, while reductions are handled more cautiously**.

#### Example 7: Maximum funding does not always mean a shortage

A clinic has a normal capacity of 1,400 patients.

At 120% funding, it can accommodate approximately **1,539 patients**.

Consider two situations:

| Current patients | Maximum capacity | Coverage | Department's decision |
|---|---:|---:|---|
| 1,500 | 1,539 | 100% | Keep maximum funding; no shortage notice |
| 1,900 | 1,539 | 81% | Confirm a capacity shortage |

With 1,500 patients, the clinic has enough capacity to serve everyone, although it cannot maintain its preferred safety margin.

**No new building is needed yet.**

With 1,900 patients, even maximum funding cannot provide adequate coverage.

The Department keeps funding at 120%. If the shortage remains confirmed for 30 simulated days and the clinic is not on strike, an advisor notice tells you that another clinic is needed.

There is also a financial trade-off: 120% funding costs **20% more**, but provides only **10% more capacity** than 100%.

#### Example 8: A hospital that needs additional capacity

A hospital has:

- **Normal capacity:** 3,000 patients.
- **Current demand:** 4,376 patients.
- **Maximum funding:** 120%.

Even at maximum funding, the hospital can accommodate only about **3,299 patients**.

Its maximum coverage is:

```math
V_{\max} = \frac{3299}{4376} \approx 0.754
```

That means the hospital can serve only about **75.4% of its demand**.

This is below both the Department's 85% shortage threshold and the game's 80% strike-risk threshold.

**Increasing funding cannot solve the problem.**

The Department keeps funding at 120%, but the hospital remains at risk of strikes.

If the shortage continues for 30 simulated days, the Department can issue a notice asking you to build another hospital. An ongoing strike is handled by the game's own strike alerts instead.

<a id="education-healthcare-limits"></a>
### 7. Known limitations

The Department automates funding decisions, but it cannot control every aspect of SimCity 4.

**Building capacity and coverage**

- **It cannot build new facilities.** If a school or hospital is overcrowded even at maximum funding, you must construct another building.
- **It cannot change coverage areas.** School bus and ambulance funding, and therefore the coverage decisions associated with them, remain under your control.
- **It uses each building's reported attendance.** The game may sometimes calculate strike risk using demand from the entire coverage area, which can be higher than the number of users shown in the building's query window. The Department uses the game's coverage information to prevent unsafe cuts, but may not detect every hidden increase in demand immediately.

**Monitoring and timing**

- **Attendance history is kept for the current city session.** When you close the city, the Department forgets the monthly attendance history and any capacity adjustments it learned. High schools and colleges therefore receive their initial seasonal protection again after reopening.
- **The game's coverage information can be delayed.** Some coverage figures update only about once per simulated month. The Department uses current attendance and capacity to plan funding, while also checking the game's coverage information before allowing reductions.
- **Urgent checks are not continuous.** The Department normally checks for urgent problems every two simulated days, or daily during strikes. A sudden increase in demand may still cause a strike before the next check.
- **Savings are reviewed monthly.** After a temporary increase in attendance, funding stays at the higher level until the next monthly review.
- **Small funding changes can occur between months.** If attendance fluctuates near a funding threshold, the Department may increase or decrease the budget by one percentage point in successive monthly reviews.

**Building compatibility**

The Department manages buildings that use recognizable, consistent funding mechanisms.

It does not automatically manage:

- Private schools without their own adjustable operating budget.
- Custom buildings using unsupported query windows or funding systems.
- Buildings whose funding and budget readings disagree.
- Buildings whose capacity cannot be predicted reliably from their funding.

Buildings that cannot be managed safely are left alone.

**Advisor notices**

- Shortage notices cover eight education and healthcare building types.
- Each notice can list up to **10 affected buildings**.
- Buildings currently on strike are handled by the game's original strike alerts rather than the Department's shortage notices.
- The Department remembers notices for each identifiable city. If a city cannot be identified reliably, the same warning may appear again after reopening.

**Original game advice**

The plugin includes a data file that suppresses certain misleading funding warnings from the original game.

This adjustment is independent of the education and healthcare management switches. As long as the file is installed, those original warnings remain suppressed even when you disable the Department's automatic management.

See [Safety conditions](#education-healthcare-safety) for details.

[Back to the contents](#contents) | [Back to the README](../README.md)
<a id="electricity"></a>
## B. Electricity

The Department automatically manages **each power plant individually**, adjusting its funding to provide enough electricity for the city while keeping operating costs as low as possible.

It considers:

- How much electricity your city needs.
- The safety reserve you have chosen (**20% by default**).
- Electricity bought from or sold to neighboring cities.
- Each power plant's generating capacity, condition and operating cost.

The Department reviews electricity funding **every 10 simulated days**. It also performs a quick check every simulated day and can adjust funding earlier when demand, production capacity or regional contracts change significantly.

Rather than applying the same funding percentage to every plant, it looks for **the least expensive combination of funding levels** that meets the city's needs.

The Department never builds, demolishes or replaces power plants. It only adjusts their funding, using its own permitted range and safety checks to protect electricity supply and the power-line network.

<a id="electricity-vanilla"></a>
### 1. How electricity works in SimCity 4

#### Electricity demand, production and consumption

SimCity 4 tracks several electricity values for your city:

| Term | What it means |
|---|---|
| **Demand** | The electricity requested by buildings that are currently powered. |
| **Requirement** | The game's estimate of the electricity the city needs, including buildings currently without power. It is usually slightly higher than demand. |
| **Consumption** | The electricity actually delivered to buildings. |
| **Production capacity** | The electricity your power plants can currently produce, based on their funding and condition. |
| **Imports** | Electricity purchased from neighboring cities. |
| **Exports** | Electricity sold to neighboring cities. |

All these values are measured in the game's electricity units per simulated month.

**Not every power shortage is caused by insufficient production.**

There are two possible situations:

- **Not enough electricity:** Your power plants and available imports cannot supply the city's demand. The Department can respond by increasing funding or adjusting regional contracts.
- **Electricity cannot reach some buildings:** The city has enough generating capacity, but some buildings are not properly connected to the power network. Increasing funding will not solve this. You need to fix the connections yourself.

The Department distinguishes between these situations so it does not increase electricity spending unnecessarily.

**How does imported electricity affect local production?**

In our observations, SimCity 4 uses imported electricity before drawing power from local plants. This means a power plant can sometimes show very low usage, or even zero usage, while a neighboring city supplies the electricity.

The Department takes regional contracts into account when deciding how much local production capacity is needed.

#### How funding affects a power plant's output

Each power plant has three important characteristics:

- **Maximum output:** How much electricity the plant can produce at its nominal capacity.
- **Funding:** The percentage of its operating budget allocated to the plant.
- **Condition:** The plant's current state, from 0 (completely deteriorated) to 1 (new).

A plant's actual output depends on both **its funding and its condition**.

For example, a new power plant funded at 100% can produce its full nominal capacity. As its condition deteriorates, its available output decreases.

<a id="electricity-plant-efficiency"></a>
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

<a id="electricity-efficiency-curve"></a>
#### The relationship between funding and efficiency

SimCity 4 uses a built-in curve to determine how much electricity a power plant can produce at each funding level.

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

**What does this mean for your city's electricity budget?**

At **100% funding**, a power plant provides the most generating capacity per simoleon spent.

However, that does **not** mean every plant should always be funded at 100%.

Imagine your city has two power plants but does not need all their combined output.

Funding both plants equally may cost more than running one near 100% and keeping the other at a lower funding level.

**This is exactly what the Department optimizes:** it compares the available combinations and chooses the cheapest one that still provides the required electricity and safety reserve.

See [Example 3](#electricity-example-mixed) for a calculation involving two different power plants.

#### Individual power plant funding and operating costs

Unlike the water service, **electricity funding is managed separately for each power plant**.

Every plant has its own funding setting and monthly operating cost.

This allows the Department to increase the funding of one plant while reducing another, depending on which combination is more economical.

**What funding levels can the Department use?**

The game's power configuration identifies **60%** as its *minimum acceptable funding*. However, that value should not be confused with a proven hard limit of the game's funding control.

The underlying local budget setter accepts values starting at 0%, subject to the department's maximum.

The Department's optimizer deliberately considers funding levels from **60% to 120%**, in whole percentages, and applies additional checks before changing a plant's budget.

These checks ensure that the planned changes meet the city's electricity needs and do not cause power-line deterioration.

**How is the monthly operating cost calculated?**

The operating cost changes in proportion to the funding percentage.

**Monthly plant expense (base game)**

```math
E(f) = \operatorname{round}\left(C \times \frac{f}{100}\right)
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

However, the Department only chooses 84% if the resulting production capacity is sufficient for the overall electricity plan.

The game reports a plant's nominal generation cost separately from what is actually charged at its current funding. The Department uses the latter relationship to estimate expenses, rounding to whole simoleons, and reads the plant's latest nominal cost whenever it recalculates the plan.

#### Why does the electricity department show a different funding percentage?

Although power plants are funded individually, SimCity 4 also shows an overall electricity department funding percentage in the city budget.

**This is not an additional funding slider.**

It is calculated as a **cost-weighted average** of the individual power plants' funding levels.

That means a large, expensive power plant has more influence on the department percentage than a small, inexpensive one.

**Overall electricity funding (base game)**

```math
F_{\text{dept}} = \frac{\sum_i C_i f_i}{\sum_i C_i}
```

Where:

- **Cᵢ** is the nominal monthly operating cost of plant *i*.
- **fᵢ** is that plant's funding percentage.
- **Fdept** is the overall electricity department funding percentage.

**Example**

Imagine your city's power plants have a combined nominal operating cost of **§11,660 per month**.

One plant costs §300 per month and is funded at 95%, while every other plant remains at 100%.

That plant saves §15 per month.

The overall electricity department funding becomes:

```math
F_{\text{dept}} = \frac{11660-15}{11660}\times100
\approx 99.87\%
```

Even though one plant is funded at 95%, the department as a whole still shows approximately **99.87% funding**.

This is normal.

**The Department manages each power plant individually. The overall electricity budget reflects the combined result of those decisions.**
#### Power lines: how funding affects the network

In SimCity 4, electricity funding affects more than power plants. It also influences the condition of the city's **power-line network**.

The game uses the overall electricity department funding to determine whether power lines deteriorate or are repaired over time.

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

The Department normally considers funding levels between **60% and 120% for each managed power plant**. This helps keep overall electricity funding above the level at which power lines deteriorate.

It also checks the effect of funding changes on the power-line network before applying them.

**The Department does not simply cut power plant budgets without considering the condition of the electricity network.**

<a id="electricity-aging"></a>
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

- **Funding** determines how much electricity a plant *can produce*.
- **Usage** measures how much of that available capacity the city actually uses.

For example:

| Power plant | Funding | Actual usage |
|---|---:|---:|
| Plant A | 100% | 30% |
| Plant B | 70% | 95% |

Plant A is fully funded but produces much less electricity than it could.

Plant B receives less funding, but the city uses almost all the electricity it can produce at that funding level.

**Plant usage (base game)**

```math
u = \frac{\text{Used}}{\text{Out}}
```

Where:

- **Used** is the electricity actually drawn from the plant.
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

However, reducing funding also reduces the amount of electricity the plant can produce. If the city continues drawing the same quantity, the plant's usage increases, and that can have a much larger effect on aging.

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

The Department reduces funding to save operating costs. The plant can now produce less electricity, but the city continues drawing roughly the same amount.

Its usage may increase to about 83%.

The usage-aging multiplier rises from approximately 0.60 to 1.06, while the direct funding multiplier changes only slightly.

**The plant's estimated aging rate becomes almost twice as high.**

This does not necessarily mean the funding reduction is a bad decision. The monthly saving may still be worth more than the additional wear.

The Department can compare these costs when **Consider power plant aging** is enabled.

**What does "Consider power plant aging" do?**

This option is **off by default**.

- **Off:** The Department minimizes monthly electricity operating expenses while maintaining the required supply and safety reserve.
- **On:** The Department also includes the estimated economic cost of plant wear when comparing funding combinations.

The option may favor a different funding arrangement when the reduction in wear compensates for the additional operating expense.

**It does not impose a separate minimum funding percentage, prevent aging or automatically replace old power plants.**

<a id="electricity-usage-myth"></a>
#### Is 80% plant usage a safety limit?

**No. SimCity 4 does not have a sudden aging or blackout threshold at 80% usage.**

The confusion comes from the game's aging curve.

At approximately **80.6% usage**, the aging multiplier reaches 1.0. Below that point, the multiplier is lower; above it, the multiplier increases.

But the curve is continuous.

A plant running at 81% usage does not suddenly become unsafe. Likewise, a plant running at 95% usage does not automatically cause a blackout.

**Blackouts happen when electricity supply is insufficient or cannot reach buildings, not because a particular plant exceeds 80% usage.**

The Department therefore does not enforce an 80% usage ceiling.

Instead, it uses the **Power reserve** setting to maintain spare generating capacity.

#### How does the power reserve affect usage?

The default power reserve is **20%**.

For a simple city supplied by a single local plant, with no regional electricity trade, the relationship is approximately:

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

With multiple power plants, the relationship becomes more complicated because SimCity 4 decides how electricity demand is distributed among them.

The Department does not directly control that distribution.

Its purpose is to maintain enough total capacity, with your chosen reserve, while minimizing the costs included in its optimization.

#### Regional electricity trade

The Department also considers electricity bought from or sold to neighboring cities.

- **Buying electricity** reduces how much local generating capacity the city needs.
- **Selling electricity** increases the capacity the city's own plants must provide.

For example, if your city needs 10,000 units and reliably imports 3,000, its power plants do not need to produce the entire 10,000 locally.

On the other hand, selling 2,000 units to a neighbor means those units must be available in addition to your own city's needs.

The Department includes these contracts when calculating the required capacity and choosing plant funding.

How it creates, changes, prices and cancels contracts is explained in [Neighbor Deals](#neighbor-deals).

<a id="electricity-problem"></a>
### 2. The management problem

Managing electricity manually is more complicated than adjusting a single budget slider.

The Department must balance several factors:

- **Avoiding unnecessary spending:** A power plant funded at 100% may produce far more electricity than the city needs.
- **Maintaining enough supply:** Reducing funding too far can leave buildings without electricity.
- **Protecting the power network:** Very low overall electricity funding can cause power lines to deteriorate.
- **Accounting for plant efficiency:** Funding and production are not proportional. Cutting every plant to the same percentage is not necessarily the cheapest solution.
- **Accounting for condition and aging:** Older plants produce less electricity, and plants working closer to full capacity generally age faster.
- **Considering regional trade:** Buying electricity can reduce local production requirements, while selling electricity increases them.
- **Managing several plants together:** The cheapest combination may involve funding one plant near 100% and another much lower, rather than applying the same percentage to both.

SimCity 4 does not directly show which combination of plant funding levels will provide the required electricity at the lowest overall cost.

**The Department calculates that combination automatically.**

It considers each plant's capacity, condition and operating expense, together with your chosen reserve and regional contracts.

When the optional aging setting is enabled, it also includes the estimated economic cost of plant wear.

**The result is an electricity budget designed to meet the city's needs without spending more than the Department's model considers necessary.**
<a id="electricity-algorithm"></a>
### 3. How the Department optimizes electricity funding

The Department does not simply reduce every power plant's funding by the same percentage.

Instead, it calculates how much electricity the city needs and compares different funding combinations to find **the lowest-cost solution that maintains sufficient generating capacity**.

The process has three main steps:

1. Calculate how much electricity the city's plants must provide.
2. Calculate how much each plant can produce at different funding levels.
3. Find the cheapest combination that meets the requirement.

If **Consider power plant aging** is enabled, the Department also includes the estimated economic cost of plant wear when comparing these combinations.

<a id="electricity-required-capacity"></a>
#### Step 1: How much electricity does the city need?

The Department first calculates the generating capacity that must be available locally.

It considers:

- The city's current electricity demand.
- Buildings that may still need electricity.
- Your configured **Power reserve**, 20% by default.
- Electricity purchased from neighboring cities.
- Electricity sold to neighboring cities.

**The reserve is applied only to the electricity the city needs to produce itself.**

For example, if your city needs 5,000 units and imports 2,000 from a neighbor, its power plants only need to cover the remaining 3,000 units, plus the configured reserve.

Electricity sold to neighbors must also be produced, so exports increase the local requirement.

**Required local generating capacity**

```math
N = \max\left(\left\lceil R \times \max(0,D-B)\right\rceil,\ Q-B\right)+S
```

Where:

- **D** is the city's electricity demand.
- **Q** is the game's total electricity requirement, including buildings currently without power.
- **B** is electricity purchased from neighbors that the plan can rely on.
- **S** is electricity sold to neighbors.
- **R** is the reserve multiplier (1.20 for the default 20% reserve).
- **N** is the generating capacity required from local power plants.

The Department uses whichever is greater: the reserved local demand or the game's remaining electricity requirement. Exports are then added.

**Example: a city with 5,000 units of demand**

Assume the game's total requirement is 5,050 units and the reserve is 20%.

| Situation | Required local capacity |
|---|---:|
| No electricity imports or exports | 6,000 units |
| Importing 2,000 units | 3,600 units |

Without imports:

```math
N = \max(5000 \times 1.20,\ 5050)=6000
```

With 2,000 units imported:

```math
N = \max(3000 \times 1.20,\ 3050)=3600
```

**The Department does not buy an additional reserve on top of imported electricity.** Its reserve protects the part of demand that local plants must supply.

If the Department manages electricity contracts, it plans the contracts and power plant funding together.

Otherwise, it uses the existing contracts, counting purchases only when they are reliably active. Pending, suspended or failing purchases are not treated as guaranteed supply. Existing sales are counted as obligations.

See [Neighbor Deals](#neighbor-deals) for more information.

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

If **Manage power** is disabled, all power plants keep their current funding. Electricity contracts can still be managed separately if their options remain enabled.

<a id="electricity-optimizer"></a>
#### Step 3: Finding the cheapest combination

This is where the Department makes the difference.

Imagine a city with several power plants. Each plant has its own operating cost, efficiency curve and condition.

There may be many possible funding combinations that provide enough electricity.

**The Department compares these combinations and chooses the one with the lowest total modeled cost.**

It does not simply lower the most expensive plant first or fund every plant at the same percentage.

**Electricity optimization formula**

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

**Find the cheapest combination of plant funding levels that produces enough electricity for the city and its reserve.**

For example, the most economical solution might be:

| Plant | Chosen funding |
|---|---:|
| Coal power plant | 91% |
| Natural gas power plant | 61% |

Another city with different demand, plants or operating costs might benefit from a completely different combination.

See [Example 3](#electricity-example-mixed) for a worked calculation involving two plants.

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

**What if there is not enough electricity?**

Sometimes even funding every available plant at its highest-capacity setting is insufficient.

In that case, the Department:

1. Chooses the highest available production capacity from the managed plants, normally by raising them to 120%.
2. Attempts to cover the remaining shortage through regional purchases, if electricity contract management is enabled.
3. Issues an advisor notice if the shortage is confirmed and persists.

**The Department never builds or replaces power plants.** If funding and available purchases cannot solve the problem, you must provide additional generating capacity yourself.

<a id="electricity-wear"></a>
#### Optional: Considering power plant aging

By default, the Department focuses on **minimizing the monthly electricity bill**.

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
W(f) = \operatorname{round}\left(P \times \frac{\min\left(A(f),\ \text{Cond}-T\right)}{1-T}\right)
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

The Department calculates the wear cost for each candidate using the plant's **currently observed usage**. It does not predict exactly how SimCity 4 will redistribute the electricity load after every possible funding change.

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

The city's actual monthly electricity expense would be §205, not §207. The additional §2 exists only in the economic comparison.

In practice, the two optimization modes often choose the same funding combination, especially when the wear difference is small.

See [Example 5](#electricity-example-wear) for a calculation using the game's plant data.

**What this option does not do**

Enabling **Consider power plant aging** does not:

- Prevent power plants from deteriorating.
- Require plants to operate below 80% usage.
- Introduce an additional minimum funding level.
- Automatically replace aging plants.
- Relax the city's electricity reserve or other safety requirements.

It simply allows the Department to consider **the estimated long-term economic cost of wear alongside the monthly operating bill**.

The purpose remains the same: **provide enough electricity for your city at the lowest modeled cost, using the options and safety limits you have chosen.**
<a id="electricity-cadence"></a>
#### Step 4: When does the Department adjust electricity funding?

The Department regularly monitors your city's electricity supply and adjusts power plant funding when necessary.

**It normally reviews funding every 10 simulated days, but it can react sooner if electricity demand or supply changes significantly.**

All timings refer to simulated game time.

| When | What the Department does |
|---|---|
| **When a city opens** | Performs its first evaluation as soon as the game's electricity information is available and consistent. There is no fixed waiting period. |
| **Every simulated day** | Quickly checks electricity demand, supply, consumption, generating capacity and regional contracts for significant changes. |
| **Every 10 simulated days** | Performs a regular evaluation of the power plants and their funding. |
| **When a significant change is detected** | Recalculates the electricity plan without waiting for the next regular review. |

**What triggers an early evaluation?**

During its daily checks, the Department looks for:

- **A real electricity shortage:** The city does not have enough electricity from its power plants and usable imports to meet demand.
- **A change of at least 1% in electricity demand**, compared with the last evaluation.
- **A change of at least 1% in local generating capacity.**
- **A change of at least 1% in contracted electricity imports or exports.**

An unchanged reading does not trigger another evaluation.

The Department also distinguishes between a genuine shortage of electricity and a problem with the city's power connections.

For example, if sufficient electricity is available but some buildings are disconnected from the network, the Department does not unnecessarily increase power plant funding. The connection problem is recorded in the log instead.

**How are funding changes applied?**

When the Department chooses a new funding level, it applies that level directly.

For example, a plant can move from 60% to 95% funding in a single adjustment. There is no gradual increase or artificial waiting period.

However, **increases always take priority over reductions**.

The Department raises funding where necessary and ensures that other parts of the plan, such as new electricity purchases, are in place before making reductions in a later evaluation.

This prevents temporary capacity shortages while several power plants or regional contracts are being adjusted.

Every evaluation uses the city's current information. If you manually change a plant's funding, or save and reopen the city, the Department takes the new situation into account.

<a id="electricity-safety"></a>
### 4. Safety conditions

The Department follows several safety rules to prevent its funding adjustments from disrupting the electricity supply.

#### Before calculating a plan

The Department only makes funding decisions when the game's information is reliable.

- Electricity demand and the other required readings must be available and consistent.
- A demand reading of zero is treated as potentially uninitialized, not automatically as proof that the city needs no electricity.
- At least one power plant must have readable funding and production information before plant funding can be managed.
- Contradictory readings, such as reported electricity consumption exceeding demand by more than 1% plus one unit, prevent automatic funding changes.

A city with **no power plants** can still be evaluated for electricity purchases from neighboring cities, if regional contract management is enabled.

#### Protecting electricity supply

**The Department never deliberately reduces funding below the capacity required by its safety rules.**

Before every funding reduction, it:

1. Reads the city's electricity information again.
2. Checks the updated generating capacity of every power plant, using its current condition and planned funding.
3. Counts only electricity purchases that are already active and usable.
4. Verifies that the remaining capacity will cover local demand, the configured reserve and exports.

If the reduction would leave insufficient capacity, **it is cancelled**.

A pending, suspended or failing electricity purchase is never treated as guaranteed supply.

Funding increases are not rejected merely because the city remains short of electricity. Even if maximum production cannot solve the whole shortage, increasing available capacity can still help.

#### Protecting power plants and power lines

- New funding targets normally range from **60% to 120%**, in whole percentages.
- The Department respects the funding limits accepted by the game's budget system.
- It rejects funding changes that would cause the power-line network to deteriorate.
- When the city has a genuine electricity deficit, the regional contract planner does not maintain electricity sales.

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
- Other services and regional electricity contracts continue operating independently.

**The priority is to preserve a reliable electricity supply, even if that means giving up a potential saving.**

See [General Safety](#safety) for the safeguards shared by all services.

<a id="electricity-settings"></a>
### 5. Settings

Electricity management can be configured in **Play Options → Department of Efficiency**, under the **Services** page.

| Setting | Default | Range | What it does |
|---|---|---|---|
| [Department active](#opt-master) | On | On / Off | Enables or disables the entire Department. |
| [Manage power](#opt-power) | On | On / Off | Automatically optimizes individual power plant funding. |
| [Power reserve](#opt-powerreserve) | 20% | 10–50% | Sets the minimum spare local generating capacity the Department aims to maintain. |
| [Consider power plant aging](#opt-poweraging) | Off | On / Off | Includes estimated plant wear when comparing funding costs. |
| [Manage neighbor deals](#opt-deals) | On | On / Off | Enables automatic management of regional contracts. |
| [Manage Electricity Deals](#opt-tradepower) | On | On / Off | Enables automatic management of electricity purchases and sales. |

The [Buy from one city to sell to another](#opt-arbitrage) option controls whether the Department may purchase electricity from one neighboring city for resale to another. See [Neighbor Deals](#neighbor-deals).

#### How does the power reserve work?

With a 20% reserve, the Department normally aims to maintain 20% more local generating capacity than the electricity the city must produce itself.

For example, if the city needs to produce 5,000 units locally, the Department aims for at least **6,000 units of capacity**.

This is a **minimum**, not a maximum. The Department may maintain more capacity when the available plant settings or profitable electricity sales justify it.

Increasing the reserve can provide additional protection against growth and changes in demand, but it may also increase operating costs.

If you disable **Manage power**, the Department leaves plant funding unchanged. It can still manage electricity purchases and sales if the corresponding options remain enabled.

#### Advanced settings (INI file only)

| Setting | Default | Allowed range | What it does |
|---|---|---|---|
| `PowerReservePercent` | 20 | 10–50 | Sets the electricity reserve percentage. |
| `PowerCadenceDays` | 10 | 1–30 | Sets how often regular electricity funding reviews are performed, in simulated days. |

For example, setting `PowerCadenceDays=5` makes the Department perform a regular review every five simulated days instead of ten.

**The daily checks for urgent supply changes remain separate from this interval.**

An invalid `PowerReservePercent` value makes the utility configuration file invalid. In that situation, the Department does not manage any utility until the configuration is corrected.

For most players, the default settings are a suitable starting point.

<a id="electricity-examples"></a>
### 6. Worked examples

The following examples show how the Department chooses funding for different power plants and electricity requirements.

All plant capacities, operating expenses and conditions are illustrative. The calculations use the original game's efficiency curves.

Unless otherwise stated, the power reserve is **20%**, and there are no regional electricity contracts.

The coal plant aging figures use values measured during testing: a base aging rate of 0.001111 per month and an aged threshold of 0.9. The same aging parameters are used for the natural gas plant where needed for illustration.

<a id="electricity-example-coal"></a>
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

<a id="electricity-example-gas"></a>
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

The plant continues supplying the required electricity and reserve, while saving **§23 per month**.

<a id="electricity-example-mixed"></a>
#### Example 3: Two power plants working together

This example shows why the Department does not fund every plant at the same percentage.

Imagine a city with the two plants from the previous examples:

| Plant | Maximum output | Condition | Cost at 100% |
|---|---:|---:|---:|
| Coal | 6,000 | 98% | §253 |
| Natural gas | 3,000 | 100% | §230 |
| **Total** | **9,000** | | **§483** |

The city has:

- **Electricity demand:** 5,000 units.
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
\text{Coal} = \operatorname{round}(253\times0.91)=230
```

```math
\text{Gas} = \operatorname{round}(230\times0.61)=140
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

**What if the city sells electricity?**

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

If regional contract management is enabled, it also tries to purchase enough electricity to cover the remaining shortage.

Otherwise, the player must provide additional generating capacity.

<a id="electricity-example-trade"></a>
#### Example 4: Buying electricity from a neighboring city

Consider the same two-plant city from Example 3.

Its electricity demand is 5,000 units, but it now has an active contract to buy **2,000 units** from a neighboring city.

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

Without the purchase, the plants cost §370 per month to provide the city's electricity and reserve.

With the purchase, their operating cost falls to **§295 per month**.

That is a reduction of **§75 per month in local power plant expenses**.

However, the city must also pay for the imported electricity.

**Whether the purchase actually saves money depends on the contract price.**

The Department's regional contract planner compares the purchase cost with the savings in local production before deciding whether the contract is worthwhile.

See [Neighbor Deals](#neighbor-deals).

**Why not reduce the plants further?**

The Department normally does not select funding below 60%.

Even if both plants are funded at 60%, they still provide approximately 3,440 units and cost §290 per month.

This minimum funding prevents the Department from reducing unnecessary plants to zero through its normal optimization.

<a id="electricity-example-wear"></a>
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

It does not predict exactly how the game will redistribute electricity demand after those funding changes.

As Example 1 demonstrates, reducing funding can increase real usage substantially, which may accelerate aging more than the initial economic estimate suggests.

The Department observes the new usage during subsequent evaluations.

**Consider power plant aging is therefore an economic estimate, not a guarantee of longer plant life.**

<a id="electricity-limitations"></a>
### 7. Known limitations

The Department manages electricity funding automatically, but several aspects remain controlled by SimCity 4 or by the player.

#### Power plant compatibility

- **The original game's efficiency and aging curves are built into the Department.** If another mod changes those curves, the Department does not automatically detect the new values.
- **Individual plant properties are read from the game.** Maximum output, condition, operating cost, aging parameters and construction cost are obtained from each plant, allowing compatible custom power plants to be managed.
- **Some plants cannot be managed safely.** Unsupported funding systems or unreadable information cause a plant to be excluded from automatic funding changes.
- **Waste-to-Energy plants currently burning garbage are not adjusted.** Their funding is left unchanged because the same building is involved in sanitation. Their available generating capacity still counts towards the electricity plan.

#### Funding and generating capacity

- **The Department does not build or replace plants.** A genuine shortage requires additional generating capacity or electricity purchases.
- **The normal minimum funding is 60%.** A city with far more power plants than it needs may still pay at least 60% of each managed plant's nominal operating cost.
- **Power plants are never deliberately switched off through funding.** The Department does not select new targets below its normal minimum.
- **Very large cities use capacity rounding.** When required capacity exceeds 20,000 units, the optimizer may retain slightly more generating capacity than strictly necessary. It never deliberately rounds a plan below the requirement.

#### Power plant aging

- **The aging calculation uses currently observed usage.** It does not predict exactly how a funding change will redistribute electricity generation among plants.
- **The aging time factor is based on measurements.** The factor of 1/16 was observed in testing rather than read directly from the game's internal data.
- **Wear costs and lifetime estimates are approximate.** They are intended to support economic decisions, not predict an exact replacement date.
- **The Department never replaces aged plants automatically.** The player remains responsible for constructing replacements when needed.

#### Electricity distribution and regional trade

- **The game's electricity requirement is used as reported.** The Department does not attempt to replace it with its own estimate of the city's total needs.
- **Connection problems are not production problems.** If sufficient electricity exists but cannot reach some buildings, changing power plant funding will not solve the issue.
- **Electricity contracts affect the funding plan.** Imports and exports are included when calculating local capacity requirements, but only usable purchases are treated as available supply.

**The Department's role is to manage the electricity your city can produce, not to redesign the network or construct new power plants.**

[Back to the contents](#contents) | [Back to the README](../README.md)
<a id="water"></a>
## C. Water

The Department automatically manages your city's **water supply budget**, ensuring that enough water is available for your buildings while keeping operating costs as low as possible.

It considers:

- How much water your city needs.
- Your chosen safety reserve (**10% by default**).
- The water your city buys from or sells to neighboring cities.
- The production capacity of your water facilities.
- The condition of your water pipe network.

Unlike electricity, where each power plant has its own funding setting, **the entire water supply shares a single budget percentage**.

The Department adjusts this percentage to provide enough water without unnecessary spending.

It can also protect your pipes from deterioration caused by insufficient funding.

Regional water contracts are included in the calculations. Their automatic management is explained in [Neighbor Deals](#neighbor-deals).

<a id="water-vanilla"></a>
### 1. How water works in SimCity 4

#### One budget for the entire water supply

SimCity 4 manages water funding through a single department budget.

Your city may have several water facilities, including pumps, water towers and treatment plants. Each facility has its own maintenance cost, but **all facilities receive the same funding percentage**.

For example, if you set water funding to 80%, that percentage applies to the entire water department.

You cannot fund one pump at 100% and another at 60%, as you can with power plants.

The game normally allows water funding up to **120%**, although a lower maximum may apply if reported by the game.

Unlike electricity, water has no built-in minimum acceptable funding percentage or strike mechanism.

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

Unlike electricity, it does not need to compare different funding combinations for individual facilities, because there is only one water budget percentage.

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
X(f) = \operatorname{round}\left(I\times\frac{f}{100}\right)
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

See [Neighbor Deals](#neighbor-deals).

#### Step 2: How much water can the city produce at each funding level?

Once the required capacity is known, the Department examines the city's current water production.

It then estimates how much water the existing facilities could produce at different funding percentages.

Because all water producers share the same funding setting, this calculation is much simpler than electricity optimization.

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

See [Neighbor Deals](#neighbor-deals) and [Advisor Notices](#notices).

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

The full contract pricing and management rules are explained in [Neighbor Deals](#neighbor-deals).

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
| [Department active](#opt-master) | On | On / Off | Enables or disables the entire Department. |
| [Manage water](#opt-water) | On | On / Off | Automatically adjusts the city's water department funding. |
| [Water reserve](#opt-waterreserve) | 10% | 10–50% | Sets the minimum spare local water production capacity. |
| [Protect water pipes](#opt-waterpipes) | On | On / Off | Prevents reductions that could cause pipe deterioration or interfere with the recovery of damaged pipes. |
| [Manage neighbor deals](#opt-deals) | On | On / Off | Enables automatic management of regional contracts. |
| [Manage Water Deals](#opt-tradewater) | On | On / Off | Enables automatic water purchases and sales. |
| [Buy from one city to sell to another](#opt-arbitrage) | Off | On / Off | Allows the Department to buy and sell water in the same regional plan. |

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

See [Neighbor Deals](#neighbor-deals).

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

[Back to the contents](#contents) | [Back to the README](../README.md)
<a id="garbage-landfills"></a>
## D. Garbage & Landfills

The Department helps your city manage garbage efficiently, keeping sanitation costs as low as possible without allowing waste to accumulate unnecessarily.

It provides four separate functions:

1. **Automatic sanitation funding:** Adjusts the sanitation budget to handle the city's garbage, including your chosen safety reserve.
2. **Regional garbage contracts:** Manages garbage imports and exports when neighbor deal management is enabled. See [Neighbor Deals](#neighbor-deals).
3. **Landfill size advice (optional):** Recommends expanding or reducing landfill areas when doing so could significantly lower your monthly expenses. **The Department never changes landfill zoning automatically for this purpose.**
4. **Ghost landfill square repair:** Automatically corrects a game bug where some former landfill squares continue to be counted and charged as active landfill. This uses the game's own zoning mechanism and does not have a separate option.

To understand how these features work, it is important to distinguish between **garbage production, treatment capacity, landfill intake and long-term storage**.

All garbage quantities in this chapter use SimCity 4's internal garbage units. Expenses are expressed in simoleons (§) per simulated month.

<a id="garbage-landfills-vanilla"></a>
### 1. How garbage works in SimCity 4

#### How garbage is produced and recycled

Every simulated day, SimCity 4 processes the city's buildings and calculates how much garbage they generate.

The game records this amount as **Produced garbage**.

Recycling centers remove a portion of that garbage before it enters the disposal system.

The amount recycled depends on the city's recycling facilities. **Changing sanitation funding does not change the recycling rate.**

The remaining garbage, together with any waste already left uncollected, must be handled by the city's disposal system.

#### In what order does the game dispose of garbage?

SimCity 4 processes garbage in a specific order.

| Priority | Disposal method | What happens |
|---|---|---|
| **1** | Garbage exports | Waste is sent to neighboring cities through active export contracts. |
| **2** | Incinerators and Waste-to-Energy plants | Treatment facilities process garbage, up to their available capacity. |
| **3** | Landfills | Remaining garbage is sent to landfills, subject to their intake and storage limits. |
| **4** | Hidden garbage removal | The game silently removes a small additional amount of garbage. |

**The game tries each method in this order before moving to the next.**

For example, if your city exports some garbage and treats the rest in incinerators, its landfills only need to accept what remains afterwards.

**What is hidden garbage removal?**

SimCity 4 has a built-in mechanism sometimes called *garbage abduction*.

It can silently remove up to **2,000 garbage units per simulated month**, according to the original game's settings.

This waste disappears without being counted in the normal garbage disposal statistics.

The Department must account for this behavior when interpreting the game's garbage figures.

**What happens to garbage that cannot be disposed of?**

Any garbage that remains after all four stages stays on the city grid as **uncollected garbage**.

This is the waste that can accumulate in your streets and buildings when your disposal system cannot handle the city's needs.

The game maintains a city-wide total of this uncollected garbage, which the Department can read.

#### How garbage contracts with neighboring cities work

Garbage contracts work differently from electricity and water contracts.

There are two types:

- **Garbage imports:** A neighboring city sends you its garbage and pays you to dispose of it.
- **Garbage exports:** Your city pays a neighboring city to take its garbage.

Although these contracts specify a monthly quantity, **the garbage is not delivered evenly throughout the month**.

Instead, the game normally moves approximately 10% of the contract's monthly quantity each simulated day until that month's allocation has been delivered.

**Daily garbage contract delivery (base game)**

```math
s = \operatorname{round}(0.1 \times A)
\qquad
\text{while } A-U>0
```

After each daily delivery:

```math
U \leftarrow U+s
```

Where:

- **A** is the contract's monthly garbage quantity.
- **U** is the amount already delivered during the current month.
- **s** is the quantity delivered that day.

The delivered amount resets at the beginning of each simulated month.

**Example: exporting 38,990 units per month**

```math
s = 38990 \times 0.1 = 3899
```

The game exports **3,899 units per day**, normally completing the monthly quantity in about ten days.

This means most garbage contract deliveries happen **during the first ten or so days of each month**, rather than being distributed evenly across all 30 or 31 days.

There is also a small rounding detail: the final daily delivery is not limited to the exact amount remaining. Some contracts can therefore deliver slightly more than their nominal monthly quantity.

**Why does this matter?**

A city may receive a large quantity of imported garbage during the first part of a month.

The sanitation system must be able to handle that temporary concentration, even if the total monthly quantity seems reasonable.

**What about the game's imported garbage statistics?**

The game's *Garbage imported last month* counter reports what was actually delivered **during the previous month**.

It remains unchanged throughout the current month.

It should not be confused with the quantity currently being delivered by active contracts.

#### How sanitation funding affects garbage treatment

Unlike electricity, sanitation has **one funding percentage for the entire department**.

This percentage affects the capacity of garbage treatment facilities and the amount of garbage that landfills can accept.

The original game normally allows funding from **0% to 120%**, unless the department has a different maximum or fixed funding.

However, **garbage treatment capacity does not increase in direct proportion to funding**.

SimCity 4 uses the same funding-to-efficiency curve as its power plants.

**Sanitation efficiency (base game)**

```math
e = \frac{f(p)}{100}
```

Where:

- **p** is the sanitation funding percentage.
- **f(p)** is the efficiency given by the game's curve.
- **e** is the resulting efficiency, expressed as a fraction.

The original curve has these control points:

| Sanitation funding | Treatment efficiency |
|---:|---:|
| 0% | 0% |
| 50% | 25% |
| 90% | 80% |
| 100% | 100% |
| 120% | 110% |

Intermediate values are calculated using straight-line interpolation, as explained in the previous chapters.

**Examples of sanitation efficiency**

| Funding | Treatment efficiency |
|---:|---:|
| 50% | 25% |
| 60% | 38.75% |
| 61% | 40.125% |
| 75% | 59.375% |
| 90% | 80% |
| 100% | 100% |
| 110% | 105% |
| 120% | 110% |

**What does this mean for your city?**

At **50% funding**, your sanitation department provides only **25% of its normal treatment capacity**.

At **90% funding**, it provides 80%.

At **120% funding**, it provides 110%, despite costing 20% more than normal.

The relationship is especially important between 50% and 90%, where each additional funding point increases efficiency by 1.375 percentage points.

**What does the efficiency affect?**

Sanitation efficiency changes:

- The treatment capacity of incinerators.
- The garbage-processing capacity of Waste-to-Energy plants.
- The amount of garbage landfills can accept during a month.

It does **not** change:

- The quantity delivered by regional garbage contracts.
- The amount removed by recycling centers.

**Does capacity change immediately after funding is adjusted?**

Not completely.

The funding percentage changes immediately, but the game's calculated sanitation efficiency and treatment capacities update over the following simulated days.

The Department must therefore account for these delays when evaluating whether a funding change has taken effect.

#### Incinerators and Waste-to-Energy plants

SimCity 4 calculates the available treatment capacity of incinerators and Waste-to-Energy plants every simulated day.

For each facility, this capacity already takes into account:

- The building's maximum treatment capacity.
- The current sanitation efficiency.
- The building's own condition and capacity modifiers.

The game then provides the combined treatment capacity of each facility type.

**The Department reads these values directly.**

It does not multiply them by sanitation efficiency again, because the game has already applied that adjustment.

This avoids counting the effect of funding twice.

**What about electricity generated from garbage?**

Waste-to-Energy plants also produce electricity.

Their garbage treatment function belongs to the sanitation system, while their electricity output is considered separately in the [Electricity](#electricity) chapter.

<a id="garbage-landfills-valve"></a>
#### How much garbage can a landfill accept?

This is one of the most important mechanics in SimCity 4.

**Having free space in a landfill does not mean it can accept unlimited garbage every month.**

The game applies a separate monthly intake limit, which the Department models as a **landfill valve**.

Think of it as two different restrictions:

- **Storage capacity:** How much garbage the landfill can hold in total.
- **Intake capacity:** How much new garbage can be sent into it during the current month.

A landfill may have plenty of empty storage space and still be unable to accept any more garbage that month.

**Daily landfill intake (base game)**

```math
\text{taken} =
\min\left(
G,\ F,\ \operatorname{round}(5000 \times L \times e)-S
\right)
```

The amount actually accepted cannot be negative.

Where:

- **G** is the garbage still waiting for disposal.
- **F** is the remaining free storage space.
- **L** is the number of landfill squares counted by the game.
- **e** is the current sanitation efficiency.
- **S** is the amount already sent through the landfill valve during the current month.
- **5,000** is the base monthly intake capacity per landfill square.

Every time garbage enters the landfill, it is added to the valve's monthly running total.

**In simple terms:**

The game limits landfill intake according to the number of landfill squares, sanitation funding and how much garbage has already been accepted that month.

**Example: a landfill with 48 squares**

Assume the city has:

- **48 landfill squares.**
- **50% sanitation funding.**
- **25% sanitation efficiency.**

The monthly intake limit is approximately:

```math
5000 \times 48 \times 0.25 = 60000
```

**The landfill can accept approximately 60,000 garbage units that month**, assuming it has enough free storage and no previous valve carryover.

At 100% funding, the same landfill has a theoretical monthly intake of:

```math
5000 \times 48 \times 1.00 = 240000
```

That is **240,000 units per month**.

<text strong>Comparison</text>

| Funding | Efficiency | Approximate monthly landfill intake |
|---|---:|---:|
| 50% | 25% | 60,000 units |
| 100% | 100% | 240,000 units |

**The landfill area has not changed. Only the sanitation budget has.**

This is why reducing sanitation funding too far can create uncollected garbage even when your landfills still have substantial empty space.

<a id="garbage-landfills-rollover"></a>
#### What happens to landfill garbage at the end of the month?

There is another important distinction:

**Garbage entering a landfill during a month is not the same thing as garbage permanently stored there.**

SimCity 4 processes landfill storage at each simulated month change.

Two things happen:

1. Some garbage already stored in the landfill gradually disappears through the game's decay mechanism.
2. The garbage accumulated in the monthly intake valve is transferred into landfill storage, subject to the game's limits.

This means landfill storage and monthly intake must be considered separately.

#### 1. Stored garbage gradually decays

SimCity 4 removes a small amount of stored garbage from each landfill cell every simulated month.

The amount depends on how full the cell is.

**Monthly garbage decay per landfill cell (base game)**

```math
d =
\min\left(
\text{stock},
\max\left(
1,
\min\left(
1500,
\operatorname{round}(0.07 \times \text{stock})
\right)
\right)
\right)
```

Where:

- **stock** is the garbage currently stored in one landfill cell.
- **d** is the amount that disappears during the monthly update.

An empty cell loses nothing.

For occupied cells, the game normally removes approximately **7% of the stored garbage**, subject to a minimum of one unit and a maximum of 1,500 units per cell per month.

**Example**

Imagine one landfill cell contains 10,000 garbage units.

```math
10000 \times 0.07 = 700
```

The game removes **700 units** during the monthly update.

If a cell contains 30,000 units:

```math
30000 \times 0.07 = 2100
```

But the maximum decay is 1,500 units.

The game therefore removes **1,500 units**, not 2,100.

In the cities tested, a completely full landfill cell held approximately 40,000 units and lost exactly 1,500 units each month.

**How is decay calculated for an entire landfill?**

The game stores garbage in a packed arrangement: full cells first, followed by a partially filled cell and then empty cells.

The Department can therefore calculate total decay using the amount of garbage stored and the capacity of each cell.

**Total monthly landfill decay (base game)**

```math
D(U) =
\left\lfloor\frac{U}{C}\right\rfloor d(C)
+
d(U \bmod C)
```

Where:

- **U** is the total amount of garbage stored.
- **C** is the capacity of one landfill cell.
- **d(C)** is the monthly decay of a full cell.
- **U mod C** is the garbage remaining in the last partially filled cell.

The first part calculates decay from full cells. The second calculates decay from the partially filled cell, if one exists.

This model reproduced the game's recorded decay exactly across **11 consecutive monthly updates** in a test city.

**Why does this matter?**

Landfills are not simply filled forever at a constant rate.

Some of the stored waste disappears every month.

The Department takes this into account when estimating how much landfill space is available and how quickly it may fill up.

#### 2. Monthly landfill intake becomes stored garbage

At the monthly update, SimCity 4 transfers garbage from the intake valve into the landfill's stored waste.

However, **the entire valve is not always emptied**.

The amount absorbed depends on:

- The garbage waiting in the valve.
- The landfill's available storage space.
- The number of landfill squares.
- How much sanitation funding is actually provided.

**Monthly landfill absorption (base game)**

```math
\begin{aligned}
H &= \min(S,\ 327\,680\,000) \\[4pt]
\text{if } A < B:\quad
H &= \left\lfloor\frac{H\times A}{B}\right\rfloor \\[4pt]
\text{absorbed} &=
\min\left(
H,\ 5000\times L,\ T_{\mathrm{cap}}-(U-D(U))
\right) \\[4pt]
U' &= U-D(U)+\text{absorbed} \\
S' &= S-\text{absorbed}
\end{aligned}
```

Where:

- **S** is the garbage waiting in the monthly landfill valve.
- **A** is the landfill maintenance amount actually funded.
- **B** is the landfill maintenance cost at 100% funding.
- **L** is the number of landfill squares.
- **Tcap** is the landfill's total storage capacity.
- **U** is the amount already stored before the monthly update.
- **D(U)** is the garbage removed through monthly decay.
- **absorbed** is the garbage transferred into storage.
- **U′** is the new stored garbage total.
- **S′** is the amount left in the valve for the next month.

The game's calculation also places an upper limit of **327,680,000 units** on the valve amount considered for absorption.

You do not need to calculate this manually. The important part is how funding affects the result.

**At 100% sanitation funding**, the funding-related reduction does not apply. The valve can be fully absorbed when sufficient storage and the other limits permit it.

**Below 100% funding**, the game may transfer only part of the valve into permanent storage.

The remainder stays in the valve and carries over into the next month.

#### Why is leftover garbage in the valve a problem?

Remember that the landfill has a monthly intake limit.

If part of the previous month's valve remains occupied, the landfill begins the new month with less available intake capacity.

This creates a second consequence of low sanitation funding:

1. **Lower funding reduces how much garbage can enter the landfill during the month.**
2. **Lower funding can also reduce how much garbage is absorbed into storage at the month change**, leaving part of the intake allowance occupied.

**Low sanitation funding can therefore restrict landfill throughput in two separate ways.**

This is why looking only at available landfill storage gives an incomplete picture of the sanitation system.

A landfill can be far from full and still struggle to handle the city's garbage.

#### When does the game update the landfill statistics?

The game publishes the amount of garbage absorbed into landfill storage at the following monthly update, through its *Garbage landfilled* counter.

This value is therefore linked to the monthly storage process, not simply to the garbage arriving at landfills during the current day.

**The key distinction**

| Concept | What it measures | Main limitation |
|---|---|---|
| **Garbage produced** | Waste generated by the city's buildings | City activity and recycling |
| **Treatment capacity** | Waste incinerators and Waste-to-Energy plants can process | Sanitation funding and building condition |
| **Landfill intake** | New garbage landfills can accept during the month | Landfill squares, efficiency and valve carryover |
| **Landfill storage** | Garbage accumulated inside landfill cells | Available storage space and monthly decay |
| **Monthly absorption** | Garbage transferred from the valve into storage | Funding, landfill size and remaining space |

**The Department must account for all these mechanisms rather than simply comparing garbage production with empty landfill space.**
<a id="garbage-landfills-cells"></a>
#### How landfill zones work: squares and storage cells

In SimCity 4, a landfill is more complicated than the area you draw on the map.

The game uses two different elements:

- **Landfill squares:** The individual squares you zone for garbage disposal.
- **Storage cells:** Invisible areas where the game actually stores accumulated garbage.

Each storage cell corresponds to a **2 × 2 group of landfill squares**.

A cell can store garbage only when all four squares belong to the landfill zone.

**How many storage cells does a landfill have?**

The game records each landfill inside a rectangular area, called a *bounding box*.

This rectangle also contains information identifying which squares actually belong to the landfill.

For a rectangle measuring **w squares wide and h squares high**, the possible storage cell grid contains:

```math
N_{\text{possible}}=(w-1)(h-1)
```

However, only cells whose four squares are part of the landfill are active.

<box gap={2}>
**Example: a 3 × 3 landfill**

<svg viewBox="0 0 340 155" width="100%" xmlns="http://www.w3.org/2000/svg">
  {#each [0,1,2] as r}
    {#each [0,1,2] as c}
      <rect x={22+c*34} y={16+r*34} width="32" height="32" rx="2" fill="#8fba7b" stroke="#45683e" strokeWidth="1"/>
    {/each}
  {/each}
  {#each [0,1] as r}
    {#each [0,1] as c}
      <rect x={178+c*34} y={33+r*34} width="32" height="32" rx="2" fill="#d9b477" stroke="#956b35" strokeWidth="1"/>
    {/each}
  {/each}
  <text x="73" y="136" textAnchor="middle" fill="currentColor" fontSize="12">9 landfill squares</text>
  <text x="228" y="136" textAnchor="middle" fill="currentColor" fontSize="12">4 storage cells</text>
  <path d="M138 65 L162 65 M156 60 L162 65 L156 70" fill="none" stroke="currentColor" strokeWidth="1.5"/>
</svg>
<caption>The right-hand diagram shows the possible storage-cell positions, not a second landfill area.</caption>
</box>

A 3 × 3 fully zoned landfill has **9 squares but 4 overlapping storage cells**.

A 2 × 2 landfill has only one storage cell.

This distinction matters because **landfill maintenance is calculated from squares, while garbage storage is organized into cells**.

#### How much garbage can a landfill store?

The game assigns storage capacity to each active cell.

That capacity depends on:

- The number of squares belonging to the landfill zone.
- The number of active storage cells.

**Landfill storage capacity (base game)**

```math
C = \operatorname{round}\left(\frac{T\times30000}{N}\right)
\qquad
T_{\mathrm{cap}}=N\times C
```

Where:

- **T** is the number of squares recorded in the landfill zone, including ghost tiless (explained below).
- **N** is the number of active storage cells.
- **30,000** is the game's storage capacity per landfill square.
- **C** is the capacity assigned to each active cell.
- **Tcap** is the landfill's total storage capacity.

The game rounds cell capacity to the nearest whole unit, with exact halfway values rounded to the nearest even integer.

**Examples**

| Landfill | Active cells | Capacity per cell | Total capacity |
|---|---:|---:|---:|
| 2 × 2 landfill | 1 | 120,000 | 120,000 |
| 129-square landfill | 70 | 55,286 | 3,870,020 |

In general, a landfill provides approximately **30,000 storage units per zoned square**.

The exact total can differ slightly because capacity is rounded separately for each cell.

The city's total landfill capacity is the sum of the capacities of all its landfill zones.

#### What happens when you expand or reduce a landfill?

This is where the game's internal cell structure becomes important.

**Expanding a landfill**

When you add new landfill squares, SimCity 4 rebuilds the zone's storage-cell grid.

Existing cells normally retain their stored garbage, while new valid cells begin empty.

However, **the direction in which you expand can affect the stored garbage**.

The game identifies storage cells by their positions relative to the corner of the landfill's bounding box.

- **Expanding right or down:** The original corner stays in place, so existing cells normally retain their positions.
- **Expanding left or up:** The corner moves, shifting the relative positions of existing cells. This can make the game's stored garbage no longer correspond to the intended locations.

This is the cause of a known SimCity 4 landfill problem sometimes encountered after expanding a landfill.

**The safest approach is to expand landfills towards the right or bottom of their existing area, using the game's map coordinates.**

Expansion can also change how much garbage each cell is allowed to hold.

If the new capacity of a cell becomes smaller than the amount already stored there, **the excess garbage is returned to the landfill intake valve**.

That garbage must then be processed through the landfill system again.

A zoning operation also makes the game check the zone's membership information and remove squares that are no longer actually zoned as landfill.

**Reducing a landfill**

Removing landfill squares behaves differently.

When you dezone a square, the game:

1. Removes it from the landfill's recorded area.
2. Deactivates affected storage cells.
3. Recalculates the capacity of the remaining active cells.
4. Returns any excess stored garbage to the intake valve.

Unlike expansion, **dezoning does not rebuild the entire storage-cell grid or fully reconcile the zone's membership information**.

The game also prevents certain changes:

- You cannot dezone a square containing an existing garbage pile.
- Landfill zones must respect the game's size restrictions, including the minimum width of two squares and maximum width of 48 squares.

**Why does this matter?**

Changing the shape of a landfill can affect more than its visible area.

It may also change how much garbage individual cells can hold and how much garbage is returned to the monthly intake system.

The Department considers these effects when evaluating landfill size changes.

#### How much does a landfill cost?

Landfill maintenance is calculated **square by square**.

Empty landfill squares have a fixed maintenance cost, while squares containing garbage piles cost more.

**Landfill maintenance at 100% funding (base game)**

```math
B=\sum_{\text{squares in the zone}}c_i
\qquad
c_i=
\begin{cases}
\text{pile maintenance cost}, & \text{if a garbage pile is present}\\
5, & \text{otherwise}
\end{cases}
```

Where:

- **B** is the landfill's monthly maintenance cost at 100% funding.
- **cᵢ** is the maintenance cost of an individual square.

In the cities examined, the observed costs were:

| Square type | Monthly maintenance at 100% |
|---|---:|
| Empty landfill square | §5 |
| Square with a garbage pile | §20, §30 or §40, depending on the pile |

**A landfill becomes more expensive as garbage accumulates.**

For example, a landfill with 100 empty squares costs:

```math
100\times5=500
```

That is **§500 per month at 100% funding**, even before garbage piles start appearing.

If 20 of those squares develop garbage piles costing §20 each, while the remaining 80 stay empty:

```math
(80\times5)+(20\times20)=800
```

The monthly maintenance increases to **§800**.

This is why landfill size matters economically.

Keeping a large amount of unused landfill area can generate unnecessary maintenance expenses.

The Department can identify situations where reducing unused landfill zoning would save money, provided the change is safe.

#### Ghost tiless: landfill areas that should not exist

SimCity 4 has a bug that can leave behind **ghost landfill squares**.

A ghost tiles is a location that the game still counts as part of a landfill even though it is no longer zoned as landfill.

For example, the square may now contain:

- An ordinary road.
- An unzoned piece of land.
- Another non-landfill area.

Visually, the landfill is gone from that square.

**Internally, however, the game still counts it as part of the landfill zone.**

This has two consequences:

1. **The square continues to cost §5 per month** in landfill maintenance at 100% funding.
2. **It is included in the zone's recorded square count** when landfill storage capacity is recalculated.

Ghost tiless do not count towards the active landfill square total used to calculate the monthly intake limit.

In other words, **the game can charge you for landfill squares that do not actually help dispose of garbage**.

#### Why do ghost tiless return after reopening a city?

The problem comes from how SimCity 4 saves and reloads landfill zones.

When saving a landfill, the game records its rectangular bounding box and which squares belong to the zone.

However, it does not properly save the membership of squares along the rectangle's final row and column.

When the city is reopened, the game initially assumes that every square inside the rectangle belongs to the landfill, then restores the saved membership information.

Because the final row and column have incomplete saved information, **squares on those edges can incorrectly become landfill members again**.

This creates two types of ghost tiless:

| Ghost tiles location | What happens after repair |
|---|---|
| **Inside the zone's rectangle** | A suitable zoning operation can remove it permanently because its corrected membership is saved. |
| **On the final row or column** | It can return when the city is reopened because the game does not save that edge membership correctly. |

The original game's zoning tools cannot permanently remove every edge ghost because the zone's recorded bounding box does not shrink.

**How does the Department help?**

The Department includes an automatic ghost-square repair mechanism.

When it detects eligible ghost tiless, it uses the game's own zoning system to correct them rather than directly modifying the save file.

The repair is designed to avoid disturbing legitimate landfill areas and stored garbage.

**There is no separate setting for ghost-square repair.** It operates as part of automatic sanitation management.

However, the underlying save/load bug remains in SimCity 4. Some edge ghosts may therefore return after reopening a city and require another correction.

<a id="garbage-landfills-problem"></a>
### 2. The management problem

Managing garbage in SimCity 4 is more complicated than simply providing enough landfill space.

The Department must account for several problems.

- **Garbage arrives at different times:** Your city produces garbage daily, while regional garbage contracts usually deliver most of their monthly quantities during the first ten or so days.
- **Landfill storage is not the same as disposal capacity:** A landfill may have plenty of empty space but still reach its monthly intake limit.
- **Low funding affects garbage disposal in several ways:** It reduces treatment capacity and can leave part of the landfill intake valve occupied when the next month begins.
- **The game's statistics can be delayed:** Imported garbage figures refer to the previous month, while some contract and sanitation values take time to update.
- **The funding curve is not proportional:** Reducing sanitation funding too far can remove much more treatment capacity than expected.
- **Unused landfill areas cost money:** Empty squares have monthly maintenance expenses, and ghost tiless can create additional charges.
- **Changing landfill shape can affect stored garbage:** Expanding or reducing a landfill may redistribute storage capacity and return excess garbage to the intake system.

**The Department must consider all these factors before deciding how much sanitation funding your city needs.**

It cannot safely optimize sanitation by looking only at the current garbage production or the amount of free landfill space.

<a id="garbage-landfills-algorithm"></a>
### 3. How the Department manages garbage

The Department uses the game's garbage statistics to estimate the amount of waste your city must handle.

It then considers garbage treatment facilities, landfill capacity, current waste accumulation and regional contracts when choosing a suitable sanitation budget.

The goal is to **clear the garbage the city needs to dispose of, maintain a safety reserve and avoid unnecessary monthly expenses**.

<a id="garbage-landfills-need"></a>
#### 3.1 How much garbage must the city handle?

The Department begins by calculating how much garbage your own city produces after recycling.

**Step 1: Calculate the city's own garbage**

```math
G_{\mathrm{own}}=\max(0,P-R)
```

Where:

- **P** is the garbage produced during the previous simulated month.
- **R** is the garbage recycled during that month.
- **Gown** is the garbage that remains to be disposed of.

For example, suppose your city produced 100,000 garbage units last month and recycled 20,000.

```math
G_{\mathrm{own}}=100000-20000=80000
```

The Department therefore estimates that the city generates **80,000 garbage units per month requiring disposal**.

**Imported garbage is not included in this calculation.**

The game's imported garbage statistics refer to deliveries from the previous month.

The Department treats foreign garbage separately, using the actual regional contracts rather than mixing imported waste with the city's own production.

#### Step 2: Add the safety reserve

Garbage production can increase as the city grows.

To avoid choosing a budget that is only sufficient for the previous month's exact quantity, the Department adds a safety reserve.

The default **Sanitation reserve is 2%**, adjustable from 2% to 20%.

**Protected garbage production**

```math
G_{\mathrm{prot}}=
\left\lceil
G_{\mathrm{own}}\left(1+\frac{r}{100}\right)
\right\rceil
```

Where:

- **Gown** is the city's own garbage after recycling.
- **r** is the configured sanitation reserve.
- **Gprot** is the protected garbage quantity, rounded up.

**Example: a city producing 75,464 garbage units after recycling**

With the default 2% reserve:

```math
G_{\mathrm{prot}}=
\lceil75464\times1.02\rceil
=76974
```

The Department therefore plans for **76,974 units** rather than exactly 75,464.

The reserve is a minimum, not a maximum. The Department may need to handle more garbage because of existing waste accumulation or regional contracts.

#### Step 3: Include accumulated garbage and imports

The city may already have garbage waiting to be collected.

It may also receive waste from neighboring cities.

The Department adds these amounts to obtain an overall monthly handling requirement.

**Monthly garbage handling requirement**

```math
H=G_{\mathrm{prot}}+Q+\sum\text{import amounts}
```

Where:

- **Gprot** is the city's protected monthly garbage production.
- **Q** is the uncollected garbage currently accumulated on the city grid.
- **Import amounts** are the garbage quantities specified by regional import contracts.
- **H** is the overall monthly handling requirement.

**Example**

Suppose the city has:

| Source | Garbage units |
|---|---:|
| Own garbage, including reserve | 76,974 |
| Garbage already uncollected | 5,000 |
| Garbage imported from neighbors | 20,000 |
| **Total handling requirement** | **101,974** |

The Department must therefore consider **101,974 units** of waste when planning sanitation.

However, **this total alone is not enough to decide whether a funding level will work**.

Garbage arrives and is processed at different times throughout the month.

A city might have enough total monthly treatment capacity but still experience temporary garbage accumulation because imports arrive early or landfill intake is temporarily limited.

For that reason, the Department uses this total for its monthly summary and records, but makes the final decision using a **day-by-day simulation of garbage disposal**.

That simulation considers when garbage arrives, which facilities process it, how the landfill valve behaves and whether uncollected waste remains.

The next sections explain how this prediction works.
<a id="garbage-landfills-uncollected"></a>
#### 3.2 What happens to uncollected garbage?

Sometimes your city produces more garbage than its disposal system can handle.

The excess stays on the city grid as **uncollected garbage**.

The Department reads the total amount directly from the game and treats it as a **backlog**, represented by **Q**.

**The goal is to clear this backlog by the end of the current simulated month**, in addition to handling the garbage produced during that month.

For example, if your city already has 10,000 units of uncollected garbage, the Department includes those 10,000 units in its calculations.

It does not ignore existing garbage just because the city's normal production is covered.

**What about garbage the game removes automatically?**

As explained earlier, SimCity 4 has a hidden mechanism that can silently remove a limited amount of excess garbage.

The Department does not treat this removal as a sanitation failure or use it as a reason to block funding reductions.

Only garbage that actually remains uncollected on the city grid contributes to the backlog.

<a id="garbage-landfills-contracts"></a>
#### 3.3 How garbage contracts are included

Garbage contracts are more complicated than they appear because **their monthly quantities are delivered over several days**, rather than all at once or evenly throughout the month.

The Department calculates the deliveries of **each contract separately**.

It does not simply add all contract quantities together and assume they arrive uniformly.

#### Garbage imports

When another city sends you garbage, the Department assumes the game delivers approximately **10% of that contract's monthly quantity each day**, until the monthly allocation is exhausted.

For example, a contract to import 20,000 units delivers approximately 2,000 units per day during the first ten days of the month.

The Department also accounts for rounding. Since the game's final delivery can slightly exceed the contractual amount, it does not underestimate incoming garbage.

**Imported garbage is always treated as additional waste your city must dispose of.**

A suspended import is still counted conservatively when evaluating the waste the city might need to handle.

#### Garbage exports

When you pay another city to take your garbage, the contract reduces the amount your own sanitation facilities must process.

The Department calculates this relief using the same daily delivery schedule.

However, **it never assumes that more garbage can be exported than the contract actually allows**.

For example, an export contract for 20,000 units provides approximately 2,000 units of disposal relief each day until the monthly allocation is exhausted.

Unlike imports, the final daily export credit is limited to the quantity still available.

**What if an export contract is suspended?**

The Department does not rely on a suspended export to remove garbage.

It also provides no disposal credit when the game's recorded contract usage cannot be read reliably.

The contract remains an existing obligation, but the Department does not assume it will provide usable disposal capacity.

This prevents the sanitation plan from depending on exports that may not happen.

#### A problem with the game's contract statistics

During the first few simulated days of a new month, SimCity 4 can temporarily display the amount used during the **previous month**.

For example, imagine an export contract for 7,000 units.

On the first day of a new month, the game might still report:

**Amount used: 7,000 units**

That does not necessarily mean the new month's contract has already been exhausted.

The Department detects this situation during the **first three simulated days**.

If the reported usage is greater than the contract could possibly have delivered since the start of the month, the Department treats that reading as outdated and starts the month's calculation from zero.

**Outdated reading test**

```math
U > (\text{day of month}-1)\times\operatorname{round}(0.1\times A)
```

Where:

- **A** is the contract's monthly quantity.
- **U** is the amount the game reports as already used this month.

This prevents outdated statistics from causing incorrect funding decisions.

#### When is importing garbage worthwhile?

Importing garbage earns money from the neighboring city, but it also creates more work for your sanitation system.

The Department only considers an import acceptable when the chosen sanitation funding can handle the additional garbage **without leaving a backlog at the end of the month**.

Sometimes the extra income can justify increasing sanitation funding.

For example, accepting imported garbage might require a higher sanitation budget, but still produce a net profit if the neighbor pays enough.

The contract manager compares these costs and revenues before making its decisions.

See [Neighbor Deals](#neighbor-deals) for the complete contract rules.

<a id="garbage-landfills-projection"></a>
#### 3.4 Simulating the remaining days of the month

This is the most important part of the Department's sanitation calculations.

**Instead of comparing only monthly totals, the Department simulates how garbage will be handled on each remaining day of the month.**

Why?

Because garbage arrives and is disposed of at different times.

- Your city produces garbage every day.
- Imports usually arrive during the first ten or so days.
- Exports provide disposal relief according to their own daily schedules.
- Incinerators and Waste-to-Energy plants have limited treatment capacity.
- Landfills have a monthly intake limit that becomes progressively exhausted.

A city may have enough theoretical monthly capacity and still experience temporary garbage accumulation.

The Department must determine whether that accumulated garbage can be cleared before the month ends.

#### How the daily simulation works

For each possible sanitation funding percentage, the Department performs the following calculation for every remaining day.

<box gap={2}>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="trash-2" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **1. Add incoming garbage**

      Start with any garbage left from the previous day, then add the city's estimated daily production and that day's imports.
    </box>
  </row>
  <divider color="subtle"/>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="truck" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **2. Apply garbage exports**

      Remove as much garbage as the available export contracts can take. Unused export credit can carry forward to the next day.
    </box>
  </row>
  <divider color="subtle"/>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="factory" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **3. Use treatment facilities**

      Incinerators and Waste-to-Energy plants handle garbage up to their available daily treatment capacity.
    </box>
  </row>
  <divider color="subtle"/>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="layers" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **4. Use landfill capacity**

      Any remaining garbage is sent to landfills, as long as their monthly intake valve has capacity left.
    </box>
  </row>
  <divider color="subtle"/>
  <row align="center" gap={2}>
    <box background="surface-secondary" radius="md" padding={2}>
      <icon name="calendar-check" size="lg"/>
    </box>
    <box flex="1" gap={1}>
      **5. Carry forward any uncollected garbage**

      Garbage that cannot be handled today remains in the backlog for the following day.
    </box>
  </row>
</box>

The Department repeats this process until the last day of the month.

**A funding level is considered sufficient when the projected backlog at the end of the month is zero.**

Temporary accumulation during the month is allowed, provided it can be cleared before the month ends.

The Department does not impose an additional artificial limit on the highest temporary backlog.

**Daily garbage projection**

```math
\begin{aligned}
a_t &= Q_{t-1}+\frac{G_{\mathrm{prot}}}{n}+I_t\\
X_t &= R_{t-1}+c_t\\
x_t &= \min(a_t,X_t)\\
R_t &= X_t-x_t\\
b_t &= \min\left(a_t-x_t,\frac{K(p)}{n}\right)\\
l_t &= \min(a_t-x_t-b_t,V_{t-1})\\
V_t &= V_{t-1}-l_t\\
Q_t &= a_t-x_t-b_t-l_t
\end{aligned}
```

Where:

- **n** is the number of days in the simulated month.
- **Gprot** is the city's protected monthly garbage production.
- **Q₀** is the current uncollected garbage backlog.
- **Iₜ** is the garbage imported on day *t*.
- **cₜ** is the export credit generated on that day.
- **Rₜ** is unused export credit carried forward.
- **aₜ** is the garbage requiring disposal that day.
- **xₜ** is the garbage removed through exports.
- **bₜ** is garbage processed by treatment facilities.
- **lₜ** is garbage sent to landfills.
- **K(p)** is the monthly treatment capacity at funding **p**.
- **Vₜ** is the remaining landfill intake allowance.
- **Qₜ** is the garbage still uncollected at the end of day *t*.

The Department approximates the city's own garbage production as evenly distributed across the month.

Imports and exports, however, follow the individual contracts' delivery schedules.

The calculation is performed for each candidate funding level.

#### How is treatment capacity adjusted for different funding levels?

The game already reports the combined monthly treatment capacity of incinerators and Waste-to-Energy plants.

The Department uses that value to estimate what the capacity would be at another funding percentage.

**Projected treatment capacity**

```math
K(p)=\operatorname{round}\left(K_{\mathrm{now}}\times\frac{e(p)}{e_{\mathrm{now}}}\right)
```

Where:

- **Know** is the combined treatment capacity currently reported by the game.
- **e(p)** is sanitation efficiency at the proposed funding.
- **enow** is the current efficiency.
- **K(p)** is the projected treatment capacity.

**Example**

During testing, changing sanitation funding from 61% to 50% changed efficiency from 40.125% to 25%.

The Waste-to-Energy capacity fell from approximately **34,088 to 21,238 units**, matching the model.

If current efficiency is zero while treatment buildings exist, the Department cannot reliably calculate alternative capacities and stops the funding plan rather than guessing.

#### How is available landfill intake calculated?

The Department also estimates how much more garbage the landfills can accept during the current month.

**Remaining landfill intake**

```math
V_0(p)=
\max\left(
0,\min\left(
F,\operatorname{round}(5000\times L\times e(p))-S
\right)
\right)
```

Where:

- **F** is the remaining free landfill storage.
- **L** is the number of landfill squares.
- **e(p)** is sanitation efficiency at the proposed funding.
- **S** is garbage already accumulated in the monthly landfill valve.
- **V₀(p)** is the remaining intake allowance at that funding.

For example, a landfill might have 100,000 units of free storage but only 20,000 units of monthly intake capacity remaining.

**The Department counts only the 20,000 units as available for disposal during the rest of that month.**

If no landfill exists, the available landfill intake is zero.

If the required landfill information cannot be read, the Department does not assume that the landfill can accept more garbage.

When exports and treatment facilities can handle all the remaining garbage, no landfill capacity is needed.

<a id="garbage-landfills-nextmonth"></a>
#### 3.5 Why does the Department sometimes check the next month too?

As explained earlier, low sanitation funding can leave garbage in the landfill's intake valve when a month ends.

That leftover garbage reduces the landfill's available intake during the following month.

**A funding level that clears the current month may therefore create a problem in the next one.**

To prevent this, the Department can also simulate **the entire following month**.

However, it only performs this additional check when it has enough information to predict the landfill's internal storage behavior reliably.

#### How does it learn a landfill's storage structure?

SimCity 4 does not directly provide every detail of the internal landfill-cell arrangement needed for this calculation.

The Department therefore learns the structure by observing normal monthly changes.

It compares:

- The garbage stored before the month changes.
- The garbage stored afterwards.
- The amount newly absorbed into landfill storage.

**Observed monthly landfill decay**

```math
D_{\mathrm{obs}}=
U_{\mathrm{before}}-U_{\mathrm{after}}+\text{absorbed}
```

Where:

- **Ubefore** is the amount stored before the monthly update.
- **Uafter** is the amount stored afterwards.
- **absorbed** is the amount transferred into storage during that update.
- **Dobs** is the garbage removed through decay.

The Department compares this result with the game's landfill decay formula.

It tests possible numbers of storage cells and checks whether they reproduce the observed decay **exactly**.

The possible cell count must also divide the landfill's total storage capacity into whole units.

**When exactly one arrangement matches the observations, the landfill geometry is considered calibrated.**

If several arrangements remain possible, the Department waits for more monthly observations.

If none matches, or the city's landfill structure cannot be represented reliably by one arrangement, it does not use the next-month prediction.

For example, a city with several landfills having different internal cell structures may not produce observations that fit a single geometry model.

**How long does calibration take?**

The Department learns from normal monthly updates, without requiring you to perform any special action.

In one tested city, a single ordinary month change provided enough information to identify the landfill geometry.

That meant the next-month check became available at the second monthly event of the session.

However, calibration may require more observations or may not be possible for some cities.

**The learned geometry is kept only for the current city session.**

It is not saved between sessions.

If landfill capacity changes because you zone or dezone squares, the Department starts collecting evidence again.

#### How does the next-month prediction work?

Once the landfill geometry is calibrated, the Department can estimate:

1. How much stored garbage will decay at the next month change.
2. How much garbage from the intake valve will be absorbed into storage.
3. How much garbage may remain in the valve.
4. How much landfill intake capacity will be available during the following month.

It uses the game's monthly absorption formulas described earlier.

Because the game's funded maintenance value can occasionally differ by one simoleon from the expected calculation, the Department tests both:

```math
A=\left\lfloor B\times\frac{p}{100}\right\rfloor
```

and the same value increased by one.

It then resets each garbage contract's simulated monthly usage to zero and repeats the daily garbage projection for the entire next month, starting without an uncollected backlog.

**The funding level is accepted only when both months finish without uncollected garbage.**

If the landfill geometry cannot be calibrated, the Department uses the current-month calculation only.

In that situation, the regional contract manager also behaves more conservatively when considering changes to existing garbage contracts.

See [Neighbor Deals](#neighbor-deals).

<a id="garbage-landfills-funding"></a>
#### 3.6 How does the Department choose sanitation funding?

After calculating how much garbage can be handled at each funding level, the Department looks for **the cheapest percentage that satisfies the disposal requirements**.

Sanitation expenses increase in proportion to funding.

**Monthly sanitation expense**

```math
E(p)=\operatorname{round}\left(I\times\frac{p}{100}\right)
```

Where:

- **I** is the sanitation department's monthly cost at 100% funding, as reported by the game.
- **p** is the proposed funding percentage.
- **E(p)** is the estimated monthly expense.

This cost includes landfill maintenance and the department's other budget lines.

For example, if the sanitation department costs §2,000 per month at 100% funding:

| Funding | Estimated monthly expense |
|---|---:|
| 50% | §1,000 |
| 75% | §1,500 |
| 100% | §2,000 |
| 120% | §2,400 |

But the cheapest percentage is not necessarily safe.

A low budget may reduce treatment capacity, restrict landfill intake or leave garbage uncollected at the end of the month.

**Funding optimization**

```math
p^*=
\arg\min_{p\in[0,p_{\max}]}E(p)
```

Subject to:

- The current month ending with no uncollected garbage.
- The following month also ending with no uncollected garbage, when landfill geometry is calibrated.

Here, **pmax** is the maximum sanitation funding permitted by the game.

**How does it find the lowest suitable percentage?**

Because the modeled disposal capacity normally increases with funding, the Department can efficiently search for the lowest percentage that satisfies the requirements.

It uses a binary search when the relationship is monotonic. If that assumption does not hold, it checks the available percentages individually.

If several percentages have the same rounded monthly expense, it prefers the one closest to the current funding.

This avoids unnecessary changes that do not save money.

#### What if even maximum funding cannot clear the garbage?

Sometimes the sanitation system simply does not have enough disposal capacity.

For example, a landfill may be nearly full.

In that case, increasing sanitation funding does not necessarily create more usable storage space.

The Department therefore avoids automatically choosing 120% when that would accomplish nothing.

Instead, it:

1. Calculates the smallest backlog that can be achieved at maximum funding.
2. Looks for the **cheapest funding level that produces that same backlog**.
3. Chooses that level rather than paying extra for no improvement.

**The Department does not spend more when additional funding cannot improve the result.**

If higher funding genuinely reduces the backlog, it can still choose maximum funding.

Regional garbage contracts may also help resolve a shortage, depending on the available neighboring cities and contract rules.

<a id="garbage-landfills-writing"></a>
#### 3.7 How are funding changes applied?

Unlike electricity and water, sanitation funding is **not always changed directly to its final target**.

The reason is that SimCity 4 takes several simulated days to update sanitation efficiency and treatment capacities after a budget change.

Making several large adjustments before the game finishes updating could lead to unstable decisions.

**The Department therefore changes sanitation funding gradually when reducing it.**

| Change | How it is applied |
|---|---|
| **Funding reduction** | At most **25 percentage points** at a time, with at least **7 simulated days** between sanitation funding changes. |
| **Funding increase** | The full required increase can be applied in one step, with at least **3 simulated days** since the previous sanitation funding change. |

These are percentage points, not percentages of the current budget.

**Example: reducing funding from 120% to 31%**

Instead of immediately cutting from 120% to 31%, the Department follows this sequence:

<box gap={2}>
  <row align="center" justify="between">
    <text weight="medium">Starting funding</text>
    <text weight="medium" tabularNums>120%</text>
  </row>
  <box background="surface-tertiary" height="8px" radius="full" clip>
    <box background="rgba(100,116,139,0.85)" width="100%" height="100%" radius="full"/>
  </box>
  {#each [{p:95,w:"79.2%",d:"At least 7 days later"},{p:70,w:"58.3%",d:"At least 7 more days"},{p:45,w:"37.5%",d:"At least 7 more days"},{p:31,w:"25.8%",d:"At least 7 more days"}] as s}
    <row align="center" justify="between">
      <text color="secondary" size="sm">{s.d}</text>
      <text weight="medium" tabularNums>{s.p}%</text>
    </row>
    <box background="surface-tertiary" height="8px" radius="full" clip>
      <box background="rgba(100,116,139,0.85)" width={s.w} height="100%" radius="full"/>
    </box>
  {/each}
</box>

Reaching the final target therefore takes **at least several simulated weeks**, potentially about a month.

This gives the game time to update its efficiency and disposal capacity between changes.

**Funding increases are faster** because restoring disposal capacity can be urgent.

However, even increases respect the minimum interval of three simulated days since the previous sanitation funding adjustment.

#### How does the Department verify each change?

Every funding adjustment uses SimCity 4's own sanitation budget control.

Before applying a change, the Department reads the current funding again.

If you have moved the slider manually since the plan was calculated, it cancels that pending change and recalculates from your new setting.

**After applying funding, the Department performs two checks.**

**1. Immediate funding verification**

It confirms that the game reports the new funding percentage.

If the value was not accepted, **automatic sanitation funding management stops for the rest of the current city session**.

Regional garbage contracts can continue to be managed, using the now-fixed sanitation funding.

**2. Efficiency and capacity verification**

Because sanitation capacity does not update immediately, the Department also waits for the game's calculations to catch up.

It checks that:

- The reported efficiency matches the value expected from the funding curve, within **0.001**.
- Treatment building capacities match the expected values, within **3% or 8 garbage units**, whichever tolerance is larger.

**No further sanitation funding change or garbage contract adjustment is made until these checks are confirmed.**

This prevents the Department from making new decisions based on partially updated game data.

**The objective is not just to find the cheapest sanitation budget, but to reach that budget without destabilizing the city's garbage disposal system.**
<a id="garbage-landfills-when"></a>
#### 3.8 When does the Department review sanitation funding?

The Department reviews sanitation funding **at the beginning of every simulated month**.

It can also recalculate the budget earlier when the city's garbage situation changes significantly.

An early evaluation is triggered when:

- **Garbage production** changes by at least 1%.
- **Incinerator or Waste-to-Energy treatment capacity** changes by at least 1%.
- **Sanitation efficiency** changes by at least 1%.
- **Garbage imports or exports** change by at least 1%.
- **Uncollected garbage appears or disappears** from the city.

These changes are compared with the values recorded during the previous evaluation.

**The Department does not repeat the same calculation when nothing has changed.**

For example, if your city builds a new incinerator, the increase in treatment capacity can trigger a new evaluation without waiting for the next month.

Similarly, if garbage starts accumulating on the streets, the Department can review whether additional funding is needed.

Every evaluation uses the latest available readings and respects the gradual funding adjustments described in [Section 3.7](#garbage-landfills-writing).

<a id="garbage-landfills-advice"></a>
#### 3.9 Landfill size advice: when expanding or reducing a landfill can save money

Sometimes changing the size of a landfill can reduce your city's sanitation expenses.

This may sound surprising. Adding landfill squares increases maintenance costs, but it can also allow the Department to operate the sanitation system at a lower funding percentage.

Conversely, removing unused squares can reduce maintenance without affecting the city's ability to dispose of garbage.

The Department can analyze both possibilities and recommend a change when the expected saving is significant.

**This feature is disabled by default.**

You can enable it using:

**Warn me when checking landfill size can help me save**

The Department then looks for two types of recommendations:

| Recommendation | What it means |
|---|---|
| **EXPAND** | Adding a specific area to your landfill could reduce the total monthly sanitation bill. |
| **REDUCE** | Removing a specific unused area could reduce maintenance costs without compromising garbage disposal. |

**The Department never changes landfill zoning automatically for these recommendations.**

It only displays advice. You decide whether to follow it, and you perform the zoning or dezoning yourself.

The advice does not change your sanitation funding or regional contracts either.

#### Which landfills can be analyzed?

The Department only provides size advice when it can understand the landfill's structure with confidence.

It requires:

- **Exactly one landfill zone** in the city.
- A supported game version (**SimCity 4 Deluxe 1.1.641.0**).
- Reliable access to the landfill's squares, storage cells, garbage piles and maintenance costs.
- Complete agreement between the landfill's internal structure and the totals reported by the game.

It checks that the capacity, stored garbage, maintenance, square counts and cell contents are consistent.

It also verifies that every square belongs to the expected zone.

**If any information is inconsistent, the Department does not make a recommendation.**

This prevents suggestions based on incomplete or incorrect landfill data.

#### How does the Department find safe expansion areas?

For an **EXPAND** recommendation, the Department searches for rectangular areas of unused land next to the existing landfill.

It only considers expansions towards the **right or bottom of the landfill in the game's internal map coordinates**.

This restriction is important because expanding upwards or leftwards can shift the landfill's internal storage cells and disturb their stored garbage.

The proposed area must meet several conditions:

- It must touch the existing landfill along its right or bottom edge.
- It must start from a corner of the chosen edge.
- It must be between **2 and 8 squares deep**.
- It must be at least **2 squares long**, without exceeding the adjoining edge or the game's 48-square limit.
- It must contain no more than **64 squares**.
- All squares must be available, inside the map and not already occupied or zoned.
- It must not merge with another landfill zone.
- It must not cause stored garbage to be pushed back into the landfill intake valve.

The Department calculates how the new shape would affect the landfill's storage cells and capacity.

It also performs a **dry run using the game's own zoning checks**, to confirm that SimCity 4 would actually accept the proposed area.

If the game would reject the operation or silently omit any square, that candidate is discarded.

**Only expansions verified as safe are considered.**

When several safe placements have the same number of squares, the Department keeps the one providing the greatest resulting storage capacity.

#### How does the Department find safe areas to remove?

For a **REDUCE** recommendation, the Department looks for unused squares along the outer edges of the landfill.

It considers removing **1 to 8 complete outer rows or columns**, forming a rectangular area of no more than 64 squares.

The proposed reduction must satisfy all of these conditions:

- No removed square contains a garbage pile.
- None of the affected storage cells contains garbage.
- No stored garbage would be returned to the intake valve.
- The remaining landfill must stay connected as one area.
- At least one complete **2 × 2 landfill block** must remain.
- The game's own dezoning check must accept the operation.

**The Department never recommends removing squares that would disturb stored garbage.**

For example, a large landfill may have several completely unused rows along one edge.

Removing those rows could reduce maintenance without affecting the garbage already stored or the city's disposal requirements.

But if removing them would damage the landfill's internal storage structure, the suggestion is rejected.

#### How does the Department calculate potential savings?

The Department considers two sources of savings:

1. Changes in the landfill's maintenance cost.
2. Changes in the sanitation funding needed after resizing the landfill.

**Changes in landfill maintenance**

```math
\begin{aligned}
\text{EXPAND:}\quad
\Delta B &= 5n_{\text{added}}-5g_{\text{reconciled}}
\\
L' &= L+n
\\[6pt]
\text{REDUCE:}\quad
\Delta B &= -5n
\\
L' &= L-n
\end{aligned}
```

Where:

- **n** is the number of landfill squares added or removed.
- **gᵣₑcₒₙcᵢₗₑd** is the number of ghost squares removed during expansion.
- **ΔB** is the change in monthly landfill maintenance at 100% funding.
- **L** and **L′** are the landfill square counts before and after the change.

New empty landfill squares cost **§5 each per month** at 100% funding.

Removing an unused square saves that maintenance expense.

An expansion can also remove ghost squares already costing money, which may partially offset the maintenance of the newly added area.

The Department also recalculates the landfill's storage capacity:

```math
C'=\operatorname{round}\left(\frac{T'\times30000}{N'}\right)
\qquad
T'_{\mathrm{cap}}=N'\times C'
```

Where **T′** is the resulting zone membership count and **N′** is the resulting number of active storage cells.

The sanitation department's new nominal monthly cost is:

```math
I'=I+\Delta B
```

**The important part is what happens next.**

The Department does not simply multiply the number of added or removed squares by §5.

It also recalculates **the cheapest sanitation funding required after the proposed change**, using the same garbage forecast, reserve, contracts and calendar.

**Estimated monthly savings**

```math
\begin{aligned}
E_0 &= \operatorname{round}\left(I\times\frac{p_0}{100}\right)\\
E_1 &= \operatorname{round}\left(I'\times\frac{p_1}{100}\right)\\
\text{Saving} &= E_0-E_1
\end{aligned}
```

Where:

- **E₀** is the monthly expense of the original landfill arrangement.
- **E₁** is the monthly expense after the proposed change.
- **p₀** is the funding the Department would choose for the current landfill.
- **p₁** is the funding it would choose after the proposed change.

Both funding levels are calculated by the sanitation optimizer.

**The comparison uses the funding the Department would choose today, not necessarily the funding currently shown on your budget slider.**

This is important because sanitation reductions happen gradually. An unfinished reduction must not be mistaken for a saving caused by resizing the landfill.

A proposed change is only accepted for advice when the resulting sanitation plan can still clear the required garbage, including the following month when that additional check is available.

The calculation uses the maintenance costs at the time of the proposed zoning operation. Future increases caused by new garbage piles are not included, and the one-time zoning expense is not counted as part of monthly savings.

#### How much must a change save before the Department recommends it?

You can configure the **Landfill savings alert threshold**.

The default is **25%**, adjustable from **20% to 50%**.

The Department only recommends a change when its expected monthly saving is **strictly greater than your threshold**.

**Advice threshold**

```math
\text{Saving}\times100>\theta\times E_0
```

Where **θ** is your configured savings threshold.

The percentage shown in the notice is rounded down:

```math
\text{Shown saving (\%)}=
\left\lfloor\frac{100\times\text{Saving}}{E_0}\right\rfloor
```

**Example: a sanitation budget of §1,000 per month**

With the default 25% threshold:

| Expected monthly saving | Saving percentage | Recommendation |
|---|---:|---|
| §200 | 20% | No |
| §250 | 25% | No |
| **§251** | **25.1%** | **Yes** |
| §350 | 35% | Yes |

**A saving of exactly 25% does not trigger a recommendation.**

The saving must exceed 25%, not merely reach it.

#### How often does the Department analyze landfill size?

Landfill size advice is intentionally conservative.

The Department avoids repeatedly reconsidering your zoning decisions or recommending changes while a landfill is still settling into its new configuration.

Three timing rules apply:

| Rule | What it means |
|---|---|
| **At most once per simulated month** | The Department does not repeatedly analyze landfill sizes during the same month. |
| **24-month protection period** | It does not analyze a landfill for 24 simulated months after certain changes or when its history is uncertain. |
| **12-month advice interval** | After issuing a recommendation, it waits at least 12 simulated months before analyzing the city for another one. |

**The 24-month protection period**

The Department waits 24 simulated months after:

- You expand or reduce a landfill.
- It first encounters a city that already contains a landfill, because it does not know when that landfill was created or last changed.
- You load an older save and the game's calendar moves backwards.

This prevents premature advice while landfill maintenance, garbage distribution and internal storage calculations are still changing.

**The 12-month advice interval**

After providing a recommendation, the Department waits 12 simulated months before checking again.

If the same opportunity still exceeds your threshold, it may be recommended again.

Turning the option off and back on, or changing the savings threshold, **does not reset these waiting periods**.

The Department records the necessary dates and landfill information separately for each city, so the waiting periods survive closing and reopening the game.

Automatic ghost-square repairs do not restart the 24-month protection period.

#### How does the Department keep the analysis fast?

Analyzing potential zoning changes involves checking many possible landfill shapes.

To avoid slowing down SimCity 4, the Department limits the work performed during each analysis.

- It evaluates **at most 64 candidates** per analysis.
- It targets a computation budget of **10 milliseconds**.
- It initially divides candidate evaluations between EXPAND and REDUCE.
- If one direction has fewer candidates, the remaining evaluation allowance can be used by the other.
- If repeated analyses exceed the time budget, the Department progressively reduces how many candidates it checks.

The maximum candidate count can temporarily fall through **48, 32, 16, 8, 4 and 2**, depending on performance.

After six consecutive analyses within the time budget, the limit can increase one step again.

The Department also checks smaller candidate sizes before recommending larger ones, because the savings do not necessarily increase with landfill size.

**An important consequence:** It only recommends a size after evaluating every smaller allowed size. This helps avoid overlooking a smaller, better change.

Among the safe candidates it evaluates, the Department selects:

1. The candidate with the greatest monthly saving.
2. If savings are equal, the candidate using fewer squares.
3. If still tied, EXPAND before REDUCE.

The analysis runs as part of a normal sanitation evaluation. It does not require a separate background process.

#### What does the recommendation look like?

The Department sends a message through SimCity 4's **utilities advisor**, categorized as good news.

Two types of notices are possible:

**For expansion:**

> **More Landfill, Less Spending?**

**For reduction:**

> **Our Landfill Is Taking Up Too Much Room!**

The notice explains:

- Whether to expand or reduce the landfill.
- How many squares should be added or removed.
- The proposed rectangle's length and depth.
- The estimated monthly saving in simoleons.
- The percentage of the sanitation budget that could be saved.

It also includes a clickable **show me where** link.

Selecting the link moves the camera to the center of the suggested area.

The message avoids saying "left", "right", "top" or "bottom", because the direction you see on screen depends on how you have rotated the city view.

**The Department only records that it has issued advice once the game successfully accepts the advisor notice.**

Remember: **the recommendation is informational. You decide whether to change your landfill.**

<a id="ghost-squares"></a>
#### 3.10 Ghost squares: automatic repair

Ghost landfill squares are different from an oversized or inefficient landfill.

They are **a SimCity 4 bug**, not a zoning decision.

As explained earlier, a ghost square is a location the game still counts as part of a landfill, even though it is no longer actually zoned as landfill.

This can create unnecessary maintenance expenses.

**The Department can automatically repair these ghost squares when it manages sanitation.**

Unlike landfill size advice, this repair:

- Does not require a separate setting.
- Does not depend on a savings threshold.
- Does not generate an advisor notice.
- Does not add or remove any legitimate landfill square.
- Does not demolish buildings or change the visible zoning.
- Never deliberately reduces landfill storage capacity.

Repair activity is recorded in the log.

#### How can the Department repair a ghost square?

The repair uses a particular behavior of SimCity 4's zoning system.

**Reapplying landfill zoning to squares that are already legitimately zoned as landfill causes the game to rebuild the zone's internal structure.**

This operation does not cost money or change those squares' zone types, but the rebuild also removes obsolete ghost-square membership.

In effect, the Department asks the game to rebuild a landfill using its own zoning rules, without changing the legitimate landfill area.

#### How does the automatic repair work?

The Department follows four steps.

**1. Detect ghost squares**

During the first sanitation evaluation of each simulated month, it checks the available landfill zones.

After opening a city, this check occurs no later than the first monthly sanitation evaluation, provided the required information is available.

It uses the same validated, read-only landfill information required for size advice.

If the game's reported totals and internal landfill data do not match, the Department does not attempt a repair.

**2. Verify that the repair is safe**

The Department chooses a region containing legitimate landfill squares without garbage piles.

It uses the game's supported zoning dimensions and checks the expected result before making any change.

The repair is refused if it could:

- Add new landfill squares.
- Return stored garbage to the landfill intake valve.
- Reduce the landfill's total storage capacity.
- Merge two separate landfill zones.
- Produce an uncertain capacity result because of rounding.
- Affect an area that the game's zoning checks would reject.

It also refuses the repair if no suitable pile-free landfill squares are available.

The selected repair region must fit within the game's **48 × 48 zoning limit**.

**3. Perform the repair**

Once the checks succeed, the Department uses SimCity 4's own zoning operation to reapply landfill zoning to the selected region.

The operation must not silently omit any square.

If the zoning call fails or the game rejects part of the region, the attempt is treated as unsuccessful.

**4. Verify the result**

The game processes the zoning operation after receiving the request.

The Department therefore checks the result during subsequent evaluations, allowing up to **four confirmation checks**.

It verifies that:

- The landfill's bounding box has not changed.
- The ghost squares have disappeared.
- The storage-cell arrangement and capacity match the predicted result.
- The legitimate landfill square count remains unchanged.
- The city's landfill capacity matches the expected value.
- The internal landfill information still agrees with the game's totals.

The amount of stored garbage naturally changes as the game runs, so that quantity is recorded rather than required to remain identical.

**The repair is only accepted when the resulting landfill structure matches the expected safe outcome.**

#### What happens if a repair fails?

The Department does not repeatedly attempt risky repairs.

Each distinct ghost-square state is attempted **at most once per city session**.

If the zoning operation fails or the resulting structure does not match the prediction, **automatic ghost-square repair is disabled until SimCity 4 is closed**.

Other Department functions can continue operating.

The Department also repairs **at most one landfill zone per simulated month**.

If several zones contain ghost squares, the remaining zones are considered during later months.

While a repair is awaiting confirmation, landfill size advice is postponed so that it does not analyze an area whose internal structure is still changing.

#### Why might ghost squares return?

Unfortunately, the original game's save/load behavior is not corrected permanently.

When you reopen a city, SimCity 4 can recreate ghost membership along the final row or column of a landfill's internal bounding box.

**The Department may therefore need to repair the same ghost squares again after reopening the city.**

That does not mean the previous repair failed. It means the game recreated the bug during loading.

Each ghost square removed saves **§5 per simulated month at 100% sanitation funding**.

The actual saving at a lower funding percentage is correspondingly smaller.

**Automatic ghost-square repair is a conservative correction of a game bug, not an automatic landfill redesign.**

<a id="garbage-landfills-safety"></a>
### 4. Safety conditions

The Department applies separate safety rules to sanitation funding, landfill size advice and ghost-square repair.

#### Automatic sanitation funding

- **Management must be enabled.** Both **Department active** and **Manage sanitation** must be on.
- **Only valid funding levels are used.** Funding stays within the limits accepted by the game's sanitation budget system.
- **Fixed funding is respected.** The Department does not modify a sanitation budget that the game reports as fixed.
- **The configured reserve is always included.** The sanitation plan must handle the protected garbage requirement.
- **The projected month must finish without uncollected garbage.** When next-month predictions are available, the following month must also be feasible.
- **Unreliable information is never treated as available capacity.** An unreadable landfill valve provides no assumed intake, and an unreadable garbage export provides no assumed disposal relief.
- **Funding changes are gradual and verified.** Reductions follow the pacing rules, and a new change is not attempted until the previous one has been confirmed.
- **Technical failures stop funding changes for the session.** The Department avoids continuing with an unreliable funding control.

If **Manage sanitation** is disabled, the Department leaves the sanitation budget unchanged.

Regional garbage contracts can still be managed independently, using the current funding to calculate available disposal capacity.

#### Landfill size advice

**Landfill size advice never changes zoning or funding.**

It is only available when sanitation funding is being managed and the landfill's internal information can be verified exactly.

The Department also requires:

- Exactly one landfill zone.
- A safe proposed change that preserves stored garbage.
- No risk of merging zones.
- An expansion that does not shift the landfill's storage-cell positions.
- Approval from the game's own zoning or dezoning validation.
- Savings strictly above the configured threshold.

If these conditions are not met, **no advice is issued**.

#### Automatic ghost-square repair

Automatic ghost-square repair is available only on the supported **SimCity 4 Deluxe version 1.1.641.0**, while sanitation management is active.

Every repair must:

- Use validated landfill data.
- Preserve all legitimate landfill squares.
- Avoid changing zone types or demolishing anything.
- Preserve or increase total landfill storage capacity.
- Avoid disturbing stored garbage or returning it to the intake valve.
- Avoid merging zones or relying on uncertain rounding.
- Pass the game's own zoning validation.
- Produce the exact expected result when checked afterwards.

If verification fails, automatic repairs stop for the remainder of the game session.

**The Department always prefers leaving a landfill unchanged over making a modification it cannot verify as safe.**

<a id="garbage-landfills-settings"></a>
### 5. Settings

Garbage and landfill management can be configured in **Play Options → Department of Efficiency**, under the **Services** page.

| Setting | Default | Range | What it does |
|---|---|---|---|
| [Department active](#opt-master) | On | On / Off | Enables or disables the entire Department. |
| [Manage sanitation](#opt-sanitation) | On | On / Off | Automatically manages sanitation funding. Also required for landfill size advice and ghost-square repair. |
| [Sanitation reserve](#opt-sanitationreserve) | 2% | 2–20% | Adds spare disposal capacity to handle increases in garbage production. |
| [Manage neighbor deals](#opt-deals) | On | On / Off | Enables automatic management of regional contracts. |
| [Manage Garbage Deals](#opt-tradegarbage) | On | On / Off | Enables automatic garbage imports and exports when neighbor deal management is active. |
| [Warn me when checking landfill size can help me save](#opt-landfilladvice) | Off | On / Off | Enables recommendations to expand or reduce landfill areas when significant savings are possible. |
| [Landfill savings alert threshold](#opt-landfillthreshold) | 25% | 20–50% | Sets the minimum percentage saving required before the Department recommends changing landfill size. |

#### How do these options work together?

**Manage sanitation**

When enabled, the Department calculates and adjusts the sanitation budget automatically.

If disabled, sanitation funding remains unchanged. Regional garbage contracts can still be managed independently, using the existing funding.

Landfill size advice and automatic ghost-square repair require sanitation management to be active.

**Sanitation reserve**

The default reserve is **2%**.

For example, if your city produces 100,000 garbage units per month after recycling, the Department plans to handle at least **102,000 units**, before including imports or existing uncollected garbage.

Increasing this reserve provides an additional buffer against growing garbage production, but may require higher funding.

**Landfill size advice**

This feature is **off by default**.

When enabled, the Department can recommend:

- **EXPAND:** Add landfill squares if the extra disposal capacity would allow a sufficiently large reduction in sanitation expenses.
- **REDUCE:** Remove unused landfill squares if that would lower maintenance costs without compromising disposal capacity.

**These recommendations never change landfill zoning automatically.**

The advice option is unavailable in the settings window while the Department or sanitation management is disabled, but its saved value is preserved.

**Landfill savings alert threshold**

The default threshold is **25%**.

A proposed landfill change must save **more than 25%** of the optimized monthly sanitation expense before an advisor notice is issued.

The threshold setting is unavailable while landfill size advice is disabled, but its value is preserved.

#### Advanced settings (INI file)

These options are also stored in the Department's utility configuration file:

| INI key | Purpose |
|---|---|
| `AutoGarbage` | Automatic sanitation funding |
| `GarbageReservePercent` | Sanitation safety reserve |
| `AutoTradeGarbage` | Automatic garbage contracts |
| `WarnLandfillSizeSavings` | Landfill expansion and reduction advice |
| `LandfillSavingsAlertThreshold` | Minimum saving required for advice |

**Automatic ghost-square repair has no separate setting.** It runs as part of sanitation management, on the supported game version, whenever the required safety checks pass.

<a id="garbage-landfills-examples"></a>
### 6. Worked examples

The following examples show how sanitation funding, landfill storage and the Department's safety rules work in practice.

Several examples use **Aqua**, a test city where the game's landfill behavior was measured.

#### Example 1: A landfill with plenty of storage but limited monthly intake

Consider a landfill with:

- **48 landfill squares.**
- **Sanitation funding:** 50%.
- **Sanitation efficiency:** 25%.
- **Available storage:** 900,000 garbage units.
- **Garbage already accepted this month:** 41,000 units.
- **Current date:** Day 20 of the simulated month.

**How much garbage can it still accept?**

First, the Department calculates the monthly intake limit:

```math
5000 \times 48 \times 0.25 = 60000
```

The landfill can accept approximately **60,000 units during the entire month**.

However, 41,000 units have already entered the landfill valve.

```math
60000-41000=19000
```

**Only 19,000 units of intake capacity remain for the month**, despite the landfill having 900,000 units of free storage.

Now imagine increasing sanitation funding to **61%**.

At that funding, efficiency becomes 40.125%.

```math
5000\times48\times0.40125=96300
```

The remaining monthly intake becomes:

```math
96300-41000=55300
```

| Funding | Monthly intake limit | Remaining intake |
|---|---:|---:|
| 50% | 60,000 | 19,000 |
| **61%** | **96,300** | **55,300** |

Once the game's efficiency has updated, the higher funding provides **36,300 additional units of monthly intake**.

**The important distinction:** A landfill may have enormous storage capacity but still be unable to accept more garbage because its monthly intake limit has been reached.

This is why the Department considers both storage and intake when choosing sanitation funding.

#### Example 2: Garbage contracts immediately after a month change

Imagine your city has a contract to export **7,000 garbage units per month**.

The game normally exports:

```math
7000\times0.10=700
```

That means **700 units per simulated day**, for approximately ten days.

However, SimCity 4 can temporarily display the previous month's contract usage after the calendar changes.

**On the first day of a new month**, the game might still show:

*Amount used: 7,000*

If the Department accepted that value, it would incorrectly assume the entire new month's export allocation had already been exhausted.

Instead, it checks whether the reported amount could realistically have been delivered since the month began.

On day 1:

```math
7000>(1-1)\times700
```

The reported usage is impossible for the new month, so **the Department treats it as zero**.

It then plans the export of 7,000 units over the coming days.

**What happens on day 4?**

Suppose the game reports 2,100 units used.

```math
2100\le(4-1)\times700
```

That value is consistent with three completed daily deliveries.

**The Department accepts the reading of 2,100 units.**

This protection prevents outdated contract statistics from causing incorrect sanitation calculations during the first three days of a month.

#### Example 3: How stored garbage disappears over time

This example comes from the **Aqua test city**.

The landfill has:

- **Capacity per storage cell:** 42,000 units.
- **Total stored garbage:** 985,887 units.

SimCity 4 stores garbage in a packed arrangement, filling cells before starting new ones.

The stored garbage therefore occupies:

- **23 completely full cells:** 966,000 units.
- **One partially filled cell:** 19,887 units.

At the beginning of each simulated month, the game removes some stored garbage through its decay mechanism.

**Decay of the full cells**

Each full cell loses a maximum of 1,500 units per month.

```math
23\times1500=34500
```

**Decay of the partially filled cell**

The game removes approximately 7% of its contents:

```math
19887\times0.07=1392.09
```

Rounded to the nearest whole unit, this becomes **1,392 units**.

**Total monthly decay**

```math
D(U)=34500+1392=35892
```

<text strong>Result: 35,892 garbage units disappear from storage during the monthly update.</text>

This happens through the game's normal landfill mechanics, without any intervention by the Department.

**Why is this useful?**

Landfill storage is not completely permanent. Some stored garbage disappears every month, gradually freeing capacity.

The Department uses this behavior when predicting how much garbage a landfill can hold over time.

#### Example 4: Repairing ghost squares in Aqua

In the Aqua test city, the landfill contained **seven ghost squares**.

These were squares that the game still counted as part of the landfill even though they were no longer actually zoned as landfill.

The recorded landfill structure was:

| Property | Before repair |
|---|---:|
| Bounding box | 6 × 9 squares |
| Possible storage-cell positions | 40 |
| Active storage cells | 30 |
| Recorded landfill membership | 49 squares |
| Actual landfill squares | 42 |
| **Ghost squares** | **7** |
| Storage capacity per active cell | 42,000 |
| Total storage capacity | 1,260,000 |
| Monthly maintenance at 100% funding | §1,030 |

All seven ghosts were located along the final row or column of the landfill's internal rectangle.

**What did the Department calculate?**

Removing the seven ghosts would leave:

- 42 legitimate landfill squares.
- 30 active storage cells.

The resulting capacity per cell would be:

```math
C'=\frac{42\times30000}{30}=42000
```

Total capacity:

```math
30\times42000=1260000
```

**The storage capacity would remain exactly the same.**

Nothing would be pushed back into the landfill intake valve.

Each ghost square also cost §5 per month at 100% funding.

```math
7\times5=35
```

**Expected monthly maintenance saving: §35.**

The Department performed the automatic repair using the game's zoning system.

**Observed results**

| Property | Before repair | After repair |
|---|---:|---:|
| Ghost squares | 7 | **0** |
| Monthly maintenance | §1,030 | **§995** |
| Storage capacity | 1,260,000 | **1,260,000** |

Every result matched the Department's prediction.

**But the ghosts returned after reopening the city.**

A copy of Aqua saved after the repair recreated the same seven ghost squares when loaded again.

This confirmed the game's save/load bug described earlier.

The Department was able to repair them again during the new city session.

**The repair works, but the original game can recreate the problem every time the city is loaded.**

#### Example 5: An expansion that saves money, but not enough to recommend

The Department also tested expanding Aqua's landfill.

Initially, a street ran along its right-hand edge.

The game's zoning checks rejected every expansion candidate on that side because the proposed land was occupied.

**The Department did not recommend or perform an expansion.**

After the street section was removed, a **2 × 2 expansion** became possible.

This would add four landfill squares.

The zoning operation would also remove the seven existing ghost squares.

**New landfill structure**

```math
T'=49+4-7=46
```

The expanded landfill would have 32 active storage cells.

```math
C'=\frac{46\times30000}{32}=43125
```

Total storage capacity:

```math
32\times43125=1380000
```

The landfill's capacity would increase from **1,260,000 to 1,380,000 units** without pushing any stored garbage back into the valve.

**What about maintenance?**

The four new empty squares would cost:

```math
4\times5=20
```

Removing the seven ghosts would save:

```math
7\times5=35
```

Net maintenance change:

```math
\Delta B=20-35=-15
```

The landfill's nominal monthly maintenance would fall from **§1,030 to §1,015**, even though the landfill became larger.

The expansion was performed during testing, and the game's observed figures matched the predictions.

**Would the Department recommend it?**

No.

The estimated reduction in the total sanitation bill was only about **4–5%**.

The minimum configurable alert threshold is 20%, and the default is 25%.

**The expansion was safe and could save money, but the saving was too small to justify an advisor notice.**

This illustrates why the Department does not recommend every technically beneficial zoning change.

#### Example 6: Why the Department sometimes refuses to repair ghosts

A different experiment in Aqua showed why ghost-square repair must be conservative.

The original landfill had:

- **49 recorded squares**, including seven ghosts.
- **30 active storage cells.**
- **Total capacity:** 1,260,000 units.

An empty outer row containing five legitimate landfill squares was removed while the ghosts were still present.

After that operation:

- Recorded membership fell to **44 squares**.
- Active storage cells fell to **25**.

The new capacity per cell became:

```math
C'=\frac{44\times30000}{25}=52800
```

Total storage capacity:

```math
25\times52800=1320000
```

**Surprisingly, the landfill's total capacity increased to 1,320,000 units**, despite five squares being removed.

This happened because the ghost squares still counted towards the recorded membership used in the capacity calculation.

**What would happen if the Department removed the seven ghosts next?**

Recorded membership would fall from 44 to 37 squares.

With the same 25 active cells:

```math
C'=\frac{37\times30000}{25}=44400
```

The capacity of each cell would fall from 52,800 to 44,400 units.

The Department's model predicted that **151,200 units of stored garbage would be pushed back into the landfill intake valve**.

That would disturb the sanitation system.

**The Department therefore refused the ghost repair and left the landfill unchanged.**

This is an important safety principle:

**Removing ghost squares is only allowed when the Department can prove that the operation will not reduce storage capacity or disturb existing garbage.**

Saving a few simoleons is not worth risking a landfill's stored waste.

#### Example 7: When does a landfill savings notice appear?

Suppose the Department calculates that the cheapest current sanitation budget is:

**§1,015 per month.**

It then finds a safe landfill size change that would reduce the optimized expense to:

**§760 per month.**

The expected saving is:

```math
1015-760=255
```

That means **§255 saved every simulated month**.

With the default 25% alert threshold, the Department checks:

```math
255\times100>25\times1015
```

```math
25500>25375
```

The condition is satisfied.

**The Department issues a recommendation.**

The actual saving is approximately **25.12%**, although the notice displays **25%** because the percentage is rounded down.

| Alert threshold | Result |
|---|---|
| 20% | Recommendation issued |
| **25% (default)** | **Recommendation issued** |
| 30% | No recommendation |

**An important detail:** The comparison uses the actual saving before rounding the displayed percentage.

That is why a notice can display 25% even though the rule requires a saving strictly greater than 25%.

<a id="garbage-landfills-limitations"></a>
### 7. Known limitations

The Department's sanitation model is based on detailed observations of SimCity 4, but several limitations remain.

#### Garbage production and funding

- **Future garbage production is estimated.** The Department uses the previous month's production minus recycling and assumes that the resulting quantity is produced evenly over the remaining days of the current month and the next one. It does not predict future city growth. Changes are incorporated when new information becomes available.
- **Funding can change during the month.** Garbage imports, exports and landfill intake do not follow a perfectly uniform schedule. The cheapest funding level may therefore change as the month progresses. In Aqua, observed decisions included **47%, 63% and 53%** at different times. This is expected behavior, not necessarily a fault.
- **The Department uses the original game's sanitation curves.** Its predictions assume the standard efficiency curve, landfill intake of 5,000 units per square per month, storage of approximately 30,000 units per square, standard decay rules and §5 maintenance for empty squares. Mods that change these mechanics can make the calculations inaccurate. If the game's efficiency does not match the expected value, the Department stops making funding changes rather than continuing with unreliable data.

#### Predicting the following month

- **The next-month check requires calibrated landfill geometry.** It normally needs a single landfill zone and a monthly observation with at least one full storage cell.
- **Calibration is not permanent.** The Department learns the geometry again after reopening the city and restarts calibration when landfill capacity changes.
- **The prediction assumes the selected funding remains in place.** It does not anticipate every future change in garbage production or regional contracts.
- **The landfill valve model has a limitation.** When the valve is already full, some overflowing garbage may not be included in the next-month estimate. In those situations, the Department may underestimate how much garbage remains in the valve at the month change.

If reliable calibration is unavailable, the Department uses its current-month projection only.

#### Landfill size advice

- **Only cities with exactly one landfill zone are eligible.** The Department does not provide EXPAND or REDUCE recommendations when several landfill zones exist.
- **Expansion areas are restricted.** Only safe areas along the right or bottom edges of the landfill's internal map coordinates are considered, with a maximum depth of eight squares and a maximum area of 64 squares.
- **Reductions are restricted to unused outer rows or columns.** Proposed reductions must preserve the remaining landfill's structure and stored garbage.
- **Savings are estimates.** New landfill squares are initially treated as empty, costing §5 each at 100% funding. Their maintenance may increase later when garbage piles appear.
- **One-time zoning expenses are not included.** The Department compares recurring monthly sanitation costs, not the initial cost of changing the zone.
- **Large savings are uncommon in the tested cities.** The observed opportunities generally remained well below the minimum 20% alert threshold, so landfill size advice may appear rarely.

#### Ghost-square repair

- **Some ghost squares return after every city load.** The original game's save/load bug can recreate them along the final row or column of a landfill's internal rectangle.
- **The repair cannot permanently fix the game's save format.** The Department may need to perform the same safe correction in each new session.
- **Unsafe ghost repairs are refused.** A ghost square remains untouched if removing it could reduce landfill capacity, disturb stored garbage or push waste back into the intake valve.
- **Leaving a ghost can still cost money.** Each unrepaired ghost continues to contribute §5 per month at 100% sanitation funding.
- **Multiple landfill zones are handled separately.** The Department can attempt safe ghost repairs one zone at a time, but normally cannot use the single-geometry next-month prediction or provide landfill size advice.

#### The game's hidden garbage removal

SimCity 4 can silently remove a limited amount of garbage through its built-in *abduction* mechanism.

**The Department does not treat that removal as a sanitation failure and does not attempt to prevent it.**

Its goal is to prevent genuine, persistent accumulations of uncollected garbage, not to change every hidden disposal rule of the original game.

**The Department prioritizes safe garbage disposal and reliable funding decisions. When it cannot predict a landfill operation or sanitation change with sufficient confidence, it leaves the existing situation unchanged.**

[Back to the contents](#contents) | [Back to the README](../README.md)
<a id="neighbor-deals"></a>
## E. Neighbor Deals

The Department can automatically manage **electricity, water and garbage contracts between your city and its neighbors**.

It decides when to:

- **Create a contract** when trading with a neighbor would benefit your city.
- **Increase or reduce an existing contract** as your city's needs change.
- **Cancel a contract** when it is no longer useful or safe.

The goal is to **reduce your city's overall expenses, earn money from profitable exports and maintain reliable essential services**.

The Department considers your city's own needs first. It then compares the cost of producing or disposing of a resource locally with the price offered by neighboring cities.

For example, buying electricity may be cheaper than increasing power plant funding. Conversely, selling surplus electricity may be profitable if the revenue exceeds the additional production cost.

The same principle applies to water and garbage disposal.

This chapter explains how **regional contracts work and how the Department manages them**.

For details on local production and funding, see [Electricity](#electricity), [Water](#water) and [Garbage & Landfills](#garbage-landfills).

<a id="neighbor-deals-mechanism"></a>
### 1. How neighbor deals work in SimCity 4

#### The six types of regional contracts

SimCity 4 supports six types of contracts with each neighboring city: buying and selling electricity, water and garbage disposal services.

However, **garbage contracts work in the opposite direction from electricity and water**.

When you send garbage to a neighbor, you pay that city to dispose of it. When you accept a neighbor's garbage, that city pays you.

| Resource | Your city pays | Your city receives payment |
|---|---|---|
| **Electricity** | Buy electricity from a neighbor | Sell electricity to a neighbor |
| **Water** | Buy water from a neighbor | Sell water to a neighbor |
| **Garbage** | Send your garbage to a neighbor for disposal | Accept a neighbor's garbage for disposal |

**What does this mean for your city?**

- **Buying electricity or water** reduces how much your city must produce locally.
- **Selling electricity or water** requires enough production capacity to supply your city and the neighboring city.
- **Exporting garbage** reduces the amount your sanitation system must handle, but costs money.
- **Importing garbage** earns money, but increases the amount your sanitation system must handle.

The game allows **one contract of each type with each neighboring city**.

Contract quantities use the same monthly units displayed in the game's Neighbor Deals window. Prices and payments are expressed in simoleons (§) per simulated month.

#### What connections are required?

Regional contracts require suitable connections between neighboring cities.

The connection depends on the resource:

| Resource | Required connection |
|---|---|
| **Electricity** | A power line crossing the city border |
| **Water** | A water pipe crossing the city border |
| **Garbage** | A suitable road or transport connection recognized by the game's contract system |

For electricity and water, the rule is straightforward: the relevant network must cross the regional border.

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

For example, imagine that your neighboring city once had a large electricity surplus. If you later change its power plants but do not save that city, the regional information may not reflect your latest changes.

The Department must work with the neighboring city records that SimCity 4 makes available.

**When are these records updated?**

The game refreshes the current city's regional information:

- At monthly updates.
- When regional contract proposals are requested, such as when you open the Neighbor Deals window.

For electricity, *Produced* represents generating capacity at the plants' current funding, while *Demanded* uses the game's monthly electricity requirement, which may be slightly higher than actual consumption.

**Do Imported and Exported represent measured deliveries?**

Not necessarily.

In the regional records, these figures are based on **contracted monthly quantities**, not a measurement of what physically moved that day.

For example, a contract to sell 5,000 units of electricity can count as 5,000 exported units in the regional figures as soon as the contract exists.

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

**Example: selling electricity to a neighboring city**

Imagine your city has:

- Electricity production: **14,000 units**.
- Electricity requirement: **10,300 units**.
- No existing electricity contracts.

The neighboring city has:

- Electricity production: **3,000 units**.
- Electricity requirement: **6,000 units**.
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
| Your city buys electricity | Your city |
| Your city sells electricity | Neighbor |
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

A city may be willing to buy electricity, for example, but SimCity 4 will not offer the contract until the seller has at least 1,000 units of available surplus.

A seller that could increase production through higher funding does not automatically receive an offer based on that hypothetical future capacity.

The offer becomes possible only when the game's regional figures actually show enough surplus.

Similarly, a city may have enough resources but lack the required treasury or network connection.

**The Department must respect these original game restrictions.** It cannot assume that a financially attractive contract is available before SimCity 4 considers it eligible.

<a id="neighbor-deals-pricing"></a>
#### How are regional contract prices calculated?

SimCity 4 calculates contract prices using the **seller's average cost of supplying the resource**.

It then adds a small profit margin.

This applies to electricity, water and garbage disposal services.

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

**Example: a city selling electricity**

Suppose your city produces 14,000 units of electricity and spends §1,400 per month operating its power plants.

With no imports:

```math
u=\frac{1400}{14000+1}\approx0.09999
```

The game's calculated average production cost is approximately **§0.10 per unit**.

The game uses this value as the starting point for pricing electricity contracts.

It recalculates the current city's unit cost when updating regional records, including monthly updates and requests for new contract proposals.

#### Step 2: The game's profit margin

The contract price includes a markup above the seller's average cost.

With the original game's settings, the markup ranges from approximately **10% to 13%**.

The exact value depends on the buyer's available supply and how much surplus the seller will have left after the contract.

**Monthly contract price (base game)**

```math
P(q)=q\times\operatorname{clamp}\left(
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

#### Example: Pricing electricity sales

Return to the electricity example from the previous section.

The seller has an average electricity cost of approximately **§0.09999 per unit**.

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

**However, this price does not tell us whether selling electricity is profitable for the city.**

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

This prevents it from relying on suspended electricity or water purchases simply because their quantities still appear in the region's totals.

<a id="neighbor-deals-delivery"></a>
#### When are contracted resources delivered?

Electricity, water and garbage do not follow the same delivery rules.

**Electricity and water**

These contracts are treated as monthly supply commitments.

The contracted amount counts as supply for the buyer and as an obligation for the seller while the contract remains in force, regardless of how much is actually consumed.

**Garbage**

Garbage contracts operate through daily deliveries.

**Daily garbage contract delivery (base game)**

```math
\text{daily step}=
\max\left(1,\operatorname{round}(0.1\times A)\right)
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
\operatorname{round}(25004\times0.1)=2500
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

For example, selling electricity for §400 per month is not worthwhile if it requires an additional §500 in power plant expenses.

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

Imagine your city can sell 5,000 units of electricity.

One neighbor offers to buy 3,000 units, while another offers to buy 4,000.

**Your city cannot necessarily accept both contracts**, because the total would exceed its available supply.

The Department must compare the possible combinations rather than choose each contract separately.

#### Purchases affect local production requirements

Buying electricity or water can reduce how much your own facilities need to produce.

With the Department's reserve rules, this can create savings greater than the purchased quantity alone might suggest.

#### The game's surplus calculation does not protect your reserve

SimCity 4's own contract limits do not include the Department's additional safety reserve.

The game may permit a sale that leaves too little spare electricity or water capacity for your city's needs.

**The Department must apply its own reserve and service requirements on top of the game's legal limits.**

#### Garbage contracts depend on daily timing

Two combinations of garbage contracts can have the same total monthly quantity but different delivery schedules.

That can change whether the sanitation system clears the month's garbage.

**The Department therefore evaluates complete garbage contract combinations using the daily sanitation simulation, not just monthly totals.**

<a id="neighbor-deals-algorithm"></a>
### 3. How the Department manages regional contracts

The Department combines contract selection with the local resource optimizers.

Instead of considering each neighboring city independently, it searches for **the combination of contracts that provides the greatest overall economic benefit while protecting the city's own services**.

It performs a separate optimization for electricity, water and garbage.

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
| **Electricity** | Every 10 simulated days, with earlier reviews for significant changes or confirmed supply shortages. |
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

**New electricity and water purchases**

The Department uses the quantities currently permitted by the game's proposal list.

It excludes purchases whose monthly price would not remain below the local city's available treasury.

**New electricity and water sales**

The neighboring city must already appear in the game's available offers.

However, some larger sale quantities may only become possible **after the Department increases local production capacity**.

The Department can consider these as *latent sale candidates*.

For example, imagine your city currently has only 2,000 units of electricity surplus, but could produce an additional 3,000 units by increasing plant funding.

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
\max_x\ \operatorname{Net}(x)=
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
| **Electricity** | Power plant funding needed to satisfy local requirements, reserve and exports, after imports. |
| **Water** | Water department funding needed to satisfy local requirements, reserve and exports, after imports. |
| **Garbage** | Sanitation funding needed to handle the city's garbage, the chosen imports and exports, and the configured reserve. |

The Department uses the same calculations described in the previous chapters.

When **Consider power plant aging** is enabled, electricity's local economic cost also includes estimated plant wear.

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

The Department does not buy electricity or water just to resell it.

It also does not purchase extra electricity or water solely to create its configured reserve.

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

**6. A confirmed electricity or water deficit prevents sales**

If the game's counters confirm a genuine regional shortage, the Department does not maintain sales of that resource.

**Its own city's essential supply takes priority over export income.**

#### How are electricity and water reserves included?

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
- **Rq** is the game's own monthly electricity requirement.

For electricity, the default reserve is 20%, giving **R = 1.20**.

For water, the default reserve is 10%, giving **R = 1.10**.

**For water, the Rq term is omitted**, since the water calculation does not use the separate electricity requirement counter.

**Example: Electricity with a 20% reserve**

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
\operatorname{Profit}(q)=
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
\operatorname{Saving}(q)=
\left[
C_{\mathrm{local}}(N(0))
-
C_{\mathrm{local}}(N(q))
\right]
-
P(q)
```

For example, if buying electricity costs §250 but allows your city to save §400 in power plant funding:

**Net saving: §150 per month.**

The purchase is profitable.

**This is marginal profitability:** the Department evaluates the actual economic effect of adding or changing a contract.

It does not assume that a sale is worthwhile simply because the game applies a markup to the seller's average cost.

#### Step 3: Search for the best contract combination

Once the available quantities and safety requirements are known, the Department compares the legal portfolios.

The search works differently for electricity, water and garbage.

**Electricity and water**

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
\operatorname{Net}(x^*)
-
\max_{x:\,x_k=x_k^{\mathrm{now}}}
\operatorname{Net}(x)
```

Where:

- **x\*** is the selected optimal portfolio.
- **xk** is the quantity of the contract being evaluated.
- **xk now** is its current quantity.
- **Net** is the total economic value of the portfolio.
- **Δk** is the economic benefit attributable to allowing that particular contract to change.

The Department compares the selected portfolio with the **best alternative that keeps this contract unchanged**, not merely with the current overall city state.

This matters because another neighbor might offer an equally good contract.

For example, suppose two neighbors can supply electricity for the same total economic cost.

Creating a contract with either one may be profitable compared with producing everything locally.

But if the other neighbor can provide the same benefit, **neither particular contract is indispensable**.

The Department considers that when deciding whether an individual change is justified.

**A contract creation or increase is normally written only when its marginal benefit is strictly positive.**

There is one exception: a change required to maintain service when no legal portfolio can keep that contract at its current amount.

In that case, the Department does not invent a monetary benefit. It recognizes that the change is necessary to satisfy the service requirements.

#### Cash savings versus economic savings

For electricity, the Department keeps two versions of a contract decision's benefit:

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

For electricity and water, buying and selling have their usual meanings.

For garbage, the directions are reversed:

- **BUY:** Your city pays a neighbor to dispose of its garbage.
- **SELL:** Your city receives garbage from a neighbor and gets paid for treating it.

This behavior is controlled by **Buy from one city to sell to another**, which is **off by default**.

#### What if the city already has contracts in both directions?

The Department compares the available arrangements and moves towards the one with the greatest overall economic benefit.

For example, if your city is both buying and selling electricity, the Department normally chooses whether to keep the purchases or the sales, then cancels the unnecessary contracts.

**There is one important exception: suspended contracts.**

SimCity 4 does not allow suspended contracts to be changed through its normal contract functions.

Suppose your city has a suspended electricity sale but now needs to purchase electricity to avoid a shortage.

The Department cannot immediately remove the suspended sale.

In that situation, **the city's electricity supply takes priority over the usual buy-or-sell restriction**.

The Department can purchase the electricity needed to maintain service, avoids creating additional sales and records the policy conflict in the log.

It does not allow the exclusivity rule to leave the city without an essential resource.

<a id="neighbor-deals-arbitrage"></a>
#### Optional: Buying and selling at the same time

You can enable **Buy from one city to sell to another** to allow the Department to consider contracts in both directions for the same resource.

This requires **Manage neighbor deals** to be enabled.

The Department only chooses a mixed arrangement when it produces a better economic result than every available one-direction arrangement.

**How can this be useful?**

Consider a city with two neighbors.

Neighbor A sells electricity relatively cheaply, while Neighbor B is willing to pay more for electricity.

Your city might benefit from buying electricity from A and selling electricity to B.

However, **the Department does not simply buy electricity and resell those same imported units**.

Instead, the purchase reduces the amount of electricity your city must produce for its own buildings.

That can free local generating capacity for a profitable sale.

**Example: electricity**

Imagine the following arrangement:

- Your city needs 10,000 units of electricity.
- It purchases 3,000 units from Neighbor A.
- Its own plants supply the remaining local demand and reserve.
- Available local generating capacity supports a sale to Neighbor B.

The Department evaluates whether the purchase cost, sale income and resulting power plant expenses produce a better overall result.

The normal electricity reserve and every other safety rule still apply.

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
- Imported electricity or water is never used as the basis for resale.
- The configured electricity and water reserves remain mandatory.
- Garbage imports must remain within the permitted disposal limits.
- Every contract must respect the game's eligibility, quantity and treasury rules.
- Local services must remain feasible.
- A resource with automatic deal management disabled is never traded by the Department.

**A neighboring city cannot simultaneously be your supplier and customer for the same resource.**

For example, Neighbor A may sell electricity to your city or buy electricity from it, but not both in the same plan.

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

For example, imagine it wants to buy electricity from a neighbor and reduce power plant funding.

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

For electricity and water, the local supply requirement depends on the city's own demand, sales and purchases.

In simplified form:

```math
\text{Local need}=\text{Own need}+\text{Sales}-\text{Purchases}
```

The full calculation also includes the configured reserve and, for electricity, the game's separate requirement counter.

The execution order ensures that intermediate changes do not require more local capacity than the starting or final arrangement.

#### Example: Replacing local electricity with a purchase

Suppose your city's power plants currently supply everything locally.

The Department finds that purchasing electricity from a neighbor would reduce total expenses.

It proceeds in this order:

1. Establish the new electricity purchase.
2. Verify that the game has accepted the contract.
3. Recalculate the local capacity requirement.
4. Reduce power plant funding when it is safe to do so.

**It does not cut your power plants before confirming the alternative supply.**

#### Example: Replacing an electricity purchase with local production

Now suppose the opposite becomes more economical.

Your city wants to stop buying electricity and produce it locally instead.

The Department:

1. Increases the necessary power plant funding.
2. Verifies that local generating capacity is available.
3. Reduces or cancels the electricity purchase.

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

As explained earlier, the Department can identify a potentially profitable electricity or water sale that is not yet available at the desired quantity.

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

**Local electricity, water and sanitation funding management can continue independently.**

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

For example, silently reducing a planned electricity purchase could leave the city without enough supply.

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

**Example: a new electricity sale**

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

For example, buying electricity may:

1. Add a monthly contract expense.
2. Allow the Department to reduce power plant funding.

The first effect belongs to the neighbor deal accounting.

The second belongs to electricity funding management.

**The Department records these separately to avoid counting the same saving twice.**

Similarly, a garbage contract may increase contract income while requiring additional sanitation funding. Those are separate budget changes.

**The Department's aim is to select contracts according to their complete economic effect, apply them without disrupting essential services, and distinguish expected benefits from changes actually confirmed by the game.**
<a id="neighbor-deals-reevaluation"></a>
#### When does the Department reconsider existing contracts?

The Department does not assume that an existing contract should continue forever.

**At every review, it recalculates the economic value of all contracts using the city's latest information.**

For example:

- An electricity sale may become less profitable as your city's demand increases.
- A water purchase may no longer be necessary after you build another pumping facility.
- A garbage import may become too expensive to handle if your landfills approach their limits.

When the situation changes, the Department can increase, reduce or cancel contracts to find a better arrangement.

**Your city's own needs and safety reserves always take priority.**

If electricity demand grows, for example, the Department reduces or cancels electricity sales before allowing them to consume the capacity needed for your city and its reserve.

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

Sometimes a city simply cannot obtain enough electricity or water to satisfy its full requirements.

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
\operatorname{served}(B,S)=
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

**Example: a city facing an electricity shortage**

Suppose your city needs **10,000 units**, but its power plants can produce only **6,000 units**.

Without imports, the city cannot supply 4,000 units of its demand.

If a neighboring city can legally provide 3,000 units, purchasing them increases the amount of demand served to **9,000 units**.

The city is still short of electricity, but the Department has reduced the deficit.

**During an emergency, the Department follows these priorities:**

1. **Serve as much actual demand as possible.**
2. **Restore the safety reserve where possible.**
3. **Choose the least expensive arrangement among equally effective solutions.**

The normal reserve is therefore still considered, but actual service comes first when there is not enough capacity to satisfy everything.

#### What happens to electricity and water sales?

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

See [Notices](#notices).

<a id="neighbor-deals-safety"></a>
### 4. Safety conditions

The Department follows strict rules when creating, changing or cancelling regional contracts.

#### Protecting your city's own services

**Your city's needs come first.**

Electricity and water sales must not consume the capacity required to serve the city and maintain its configured reserve.

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

An active electricity or water sale is treated as an obligation.

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

**The electricity, water and sanitation funding managers can continue operating.**

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

See [Notices](#notices).

**Important:** The original game's messages are suppressed by a resource file loaded when SimCity 4 starts. Disabling automatic contract management in the options does not immediately restore those messages. The procedure for restoring them is explained under [Known limitations](#neighbor-deals-limitations).

<a id="neighbor-deals-settings"></a>
### 5. Settings

Regional contract management can be configured in **Play Options → Department of Efficiency**, under the **Services** page.

| Setting | Default | What it does |
|---|---|---|
| [Manage neighbor deals](#opt-deals) | On | Enables automatic regional contract management. |
| [Manage Electricity Deals](#opt-tradepower) | On | Allows the Department to buy and sell electricity. |
| [Manage Water Deals](#opt-tradewater) | On | Allows the Department to buy and sell water. |
| [Manage Garbage Deals](#opt-tradegarbage) | On | Allows the Department to send garbage to neighbors or receive theirs. |
| [Buy from one city to sell to another](#opt-arbitrage) | **Off** | Allows buying and selling the same resource in one plan when economically beneficial. |
| [Power reserve](#opt-powerreserve) | 20% | Protects local electricity supply. |
| [Water reserve](#opt-waterreserve) | 10% | Protects local water supply. |
| [Sanitation reserve](#opt-sanitationreserve) | 2% | Provides extra garbage disposal capacity. |

#### How do the switches interact?

**Manage neighbor deals** is the main switch for regional contracts.

When disabled:

- The Department does not request new contract offers.
- It does not create, resize or cancel contracts.
- Existing contracts remain in force and are included in local funding calculations.

The three resource switches allow you to manage electricity, water and garbage independently.

**You can also manage contracts without managing local funding.**

For example:

| Settings | Result |
|---|---|
| **Manage Electricity Deals: On** / **Manage power: On** | The Department can coordinate electricity contracts and plant funding. |
| **Manage Electricity Deals: On** / **Manage power: Off** | Contracts can be managed, but power plant funding stays unchanged. |
| **Manage Electricity Deals: Off** / **Manage power: On** | Plant funding can be optimized, but existing electricity contracts remain unchanged. |
| **Both Off** | Neither electricity contracts nor plant funding are automatically changed. |

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

#### Example 1: Selling electricity without sacrificing the reserve

Consider two neighboring cities.

**Your city (City A)**

- Electricity demand: **10,000 units**.
- Game electricity requirement: **10,300 units**.
- Power reserve: **20%**.
- Current electricity production: **14,000 units**.
- Current power plant expenses: **§1,400 per month**.

**Neighboring city (City B)**

- Electricity production: **3,000 units**.
- Electricity requirement: **6,000 units**.
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

**SimCity 4 is willing to offer an electricity purchase from City A.**

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

| Electricity sold | Monthly income | Illustrative local cost | Extra local cost | Net profit |
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

#### Example 2: Buying electricity and saving more through the reserve

Consider another city, **City C**.

- Electricity demand: **10,000 units**.
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

Purchasing electricity reduces the amount that must be produced locally.

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

Suppose your city already has an electricity sale with:

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

[Back to the contents](#contents) | [Back to the README](../README.md)

---

<a id="demolition"></a>
## F. Automatic Demolition

The Department can automatically clear certain abandoned buildings and ruins, allowing your city's zoned land to develop again.

It handles two situations:

- **Ruins:** Buildings destroyed by fires or disasters that remain on their lots.
- **Abandoned buildings:** Buildings that have been empty for a sufficiently long time.

The Department checks eligible lots **once per simulated month**.

You control:

- How long a building must remain abandoned or destroyed before demolition.
- Which zone types can be included.
- Which abandonment reasons are eligible.

**Demolition is never free.**

The Department uses SimCity 4's normal bulldozing command and pays the same price you would pay when demolishing the lot manually.

Before each demolition, it asks the game for the actual cost and checks whether your city's treasury can afford it.

**Municipal buildings and historically protected buildings are never demolished automatically.**

The purpose is to clear neglected lots without destroying important buildings or spending more money than your city has available.

<a id="demolition-vanilla"></a>
### 1. How automatic demolition works with SimCity 4

#### How the game tracks building conditions

SimCity 4 tracks a state for every lot.

It also records **how many simulated days have passed since that state last changed**.

The Department uses these existing values to decide whether a lot is eligible for demolition.

| Game state | What you see | Department action |
|---|---|---|
| **Occupied** | A functioning building | Never demolished |
| **Constructing** | A building under construction | Never demolished |
| **Ready / Created** | A lot waiting for development or a building becoming available | Never demolished |
| **Vacant** | An abandoned building | May be demolished after the chosen waiting period |
| **Destructing** | A building already being demolished | Skipped |
| **Destroyed** | Ruins left by a fire or disaster | May be demolished after the chosen waiting period |

**Only Vacant and Destroyed lots are considered for automatic demolition.**

All other states are excluded.

#### How is the waiting period measured?

The Department uses **the game's own day counter**, not a separate timer.

For example, imagine a building has been abandoned for 200 simulated days.

If your configured waiting period is 180 days, it is already old enough to be considered during the next monthly demolition review, provided all other conditions are satisfied.

The counter is saved with the city.

**This means the Department also recognizes buildings that were abandoned before you installed the mod.**

You do not need to wait for the abandonment period to start again.

#### Why was a building abandoned?

SimCity 4 records the reason why a building became abandoned.

The Department reads that reason and uses your selected options to decide whether the lot should be eligible.

| Game abandonment reason | What it means | Controlling option |
|---|---|---|
| **Low desirability** | The area is unattractive because of factors such as pollution, crime or poor services. | **Reason: unattractive area** |
| **Commute time** | The occupants cannot reach their jobs within an acceptable travel time. | **Reason: commute too long** |
| **Low demand** | There is insufficient demand for the building's type. | **Reason: no demand** |
| **No power** | The building was abandoned because it lacks electricity. | **Reason: no power** |
| **No water** | The building was abandoned because it lacks water. | **Reason: no water** |
| **Occupants evicted** | The game reports that the occupants were removed. | Always eligible by reason; no separate option |
| **None / unknown** | The game reports no recognizable abandonment reason. | Always eligible by reason; no separate option |

**Eligibility by reason does not mean immediate demolition.** The waiting period, zone restrictions, historic protection and other safety checks still apply.

#### Why are No power and No water disabled by default?

A building abandoned because of missing electricity or water may become occupied again once the service is restored.

For example:

- An apartment building loses power and becomes abandoned.
- You repair the electricity connection.
- The building can become occupied again without being demolished.

Demolishing it too soon would be unnecessary.

**That is why abandonment caused by missing electricity or water is excluded by default.**

You can enable these reasons if you want the Department to clear such buildings after your chosen waiting period.

#### Which zone types are eligible?

The Department distinguishes between ordinary zoned development and protected municipal or special-purpose lots.

| Zone group | Included zone types |
|---|---|
| **Residential** | Low-, medium- and high-density residential zones |
| **Commercial** | Low-, medium- and high-density commercial zones |
| **Industrial** | Medium- and high-density industrial zones |
| **Agricultural** | Farms and agricultural fields |
| **Municipal / protected** | Plopped buildings, special-purpose lots and unzoned lots |

The municipal and protected group includes:

- Schools and universities.
- Hospitals.
- Police and fire stations.
- Parks and landmarks.
- Reward buildings.
- Utilities.
- Military facilities.
- Airports, seaports and spaceports.
- Landfills.
- Lots without zoning.

**The Department does not automatically demolish municipal or protected lots.**

These buildings represent deliberate player decisions or special-purpose infrastructure rather than ordinary abandoned development.

#### What about farms?

Agricultural lots require special care.

Farm fields can appear vacant as part of their normal operation, even when the farm itself is functioning.

**A vacant-looking agricultural lot is not automatically proof that a farm should be removed.**

The Department therefore distinguishes agricultural zoning from ordinary residential, commercial and industrial development, allowing it to be controlled separately.

#### Historic buildings are always protected

SimCity 4 allows you to designate certain buildings as historical.

The game records that designation through a historic flag.

**A historically designated building is protected from automatic demolition**, even if it becomes abandoned and has exceeded the configured waiting period.

The Department respects that decision.

#### How does bulldozing work?

When you manually select the bulldozer tool in SimCity 4, the game calculates the demolition price for the lot.

It also checks whether the lot can actually be demolished in its current state.

The Department uses the same underlying game functions.

**Before demolishing a lot, it obtains the game's own bulldozing cost.**

It does not assume that every building has the same demolition price.

If the game reports that a lot cannot currently be demolished, the Department skips it.

**What happens to the land afterwards?**

Demolishing a building does not remove its zoning.

For example:

- A demolished residential building leaves residential zoning behind.
- The city can later develop another residential building on that lot.

The same applies to eligible abandoned buildings and ruins.

**The Department clears the building, not the zone.**

Ruins left by fires or disasters remain on their lots until something demolishes them, either manually or through the Department's automatic demolition feature.

<a id="demolition-problem"></a>
### 2. The management problem

In large cities, abandoned buildings and ruins can become difficult to manage manually.

Several problems arise.

**1. Ruins prevent new development**

Buildings destroyed by fires or disasters can remain on zoned land.

Until they are demolished, that land cannot simply develop a replacement building.

In a large city, scattered ruins can be easy to overlook.

**2. Long-term abandonment blocks redevelopment**

A building abandoned because of low demand, poor desirability or excessive commuting time may remain empty for a long period.

Clearing the lot can allow the game to develop a different building when conditions improve.

**3. Not every apparently vacant building should be demolished**

Some buildings may recover naturally.

A building abandoned because of missing electricity or water may become occupied again when the service returns.

Agricultural fields may normally appear vacant.

Municipal buildings and historic buildings must also be protected.

**4. Demolition costs money**

Every bulldozing operation has a price determined by the game.

Demolishing many buildings during the same month could consume a significant part of a small city's treasury.

**The Department must therefore distinguish between lots that are genuinely eligible for demolition and those that should be left alone, while respecting your settings and the city's available money.**
<a id="demolition-algorithm"></a>
### 3. How the Department manages demolition

The Department checks your city **once per simulated month** for ruins and abandoned buildings that meet your demolition settings.

It does not demolish anything immediately when you open a city. The first demolition review takes place at the **first simulated month change after loading**.

#### How does the monthly demolition review work?

At each monthly review, the Department follows these steps:

1. **Find eligible lots.** It scans the city for destroyed or abandoned buildings and checks their age, zoning, abandonment reason and protected status.
2. **Decide which lots qualify.** It applies your chosen demolition settings and the game's own eligibility rules.
3. **Sort the candidates.** Ruins come first, followed by abandoned buildings. Within each group, the oldest lots come first.
4. **Check the limits.** It respects the maximum number of demolition orders, the city's treasury and any monthly demolition budget you have configured.
5. **Verify each lot again.** Before sending an order, it checks that the lot has not changed and asks the game for its actual bulldozing price.
6. **Send the demolition order.** It uses SimCity 4's normal bulldozing operation, without removing the underlying zoning.
7. **Verify the result.** At a later scan, it confirms that the building is gone and the land remains zoned.

**The Department never demolishes a lot merely because it appeared eligible during an earlier scan.**

It checks again immediately before issuing the order.

#### Which buildings are demolished first?

The priority order is:

1. **Destroyed buildings (ruins)**, oldest first.
2. **Abandoned buildings**, oldest first.

This means a recent eligible ruin is processed before an older abandoned building.

The Department can normally issue up to **150 demolition orders during one monthly review**.

However, an additional precaution applies when a city session begins.

**Before the game has confirmed at least one demolition order in that session, the Department sends at most three orders per monthly review.**

This allows it to verify that the game's demolition mechanism works correctly before processing larger numbers of buildings.

#### What happens between monthly reviews?

Every **10 simulated days**, the Department performs a lightweight demolition check.

This check:

- Confirms whether previous demolition orders were carried out.
- Records changes in the lots being monitored.
- Checks for unexpected results.

**The 10-day check never issues new demolition orders.**

New demolitions are performed only during monthly reviews.

#### How does the Department decide whether a lot qualifies?

The following rules are checked in order.

| Condition | Decision |
|---|---|
| The game is already demolishing the lot | Skip it |
| The lot is neither abandoned nor destroyed | Not eligible |
| The lot is historic and does not qualify for the special ruins exception | Keep it |
| Automatic demolition is disabled for that type of lot | Keep it |
| The lot is municipal and does not qualify for the special ruins exception | Keep it |
| An abandoned building is in a zone type you have excluded | Keep it |
| An abandoned building has an excluded abandonment reason | Keep it |
| The lot has not reached the configured waiting period | Keep it |
| The game reports that the lot cannot be demolished | Keep it |
| None of the above applies | **Eligible for demolition** |

**An important distinction:**

The zone and abandonment-reason options apply only to **abandoned buildings**.

They do not filter ruins.

For example, if **Demolish destroyed buildings** is enabled, the ruins of a residential, commercial, industrial or agricultural building can be cleared after the waiting period, even when that zone type is excluded from abandoned-building demolition.

Municipal and historic ruins have their own separate protection setting.

<a id="demolition-destroyed"></a>
#### Destroyed buildings: clearing ruins

When **Demolish destroyed buildings** is enabled, the Department can automatically remove ruins left by fires and disasters.

By default, it waits **one simulated month**, equivalent to 30 days.

For example:

- A commercial building is destroyed by a fire.
- The game marks its lot as Destroyed.
- The Department waits until at least 30 simulated days have passed.
- At the next monthly demolition review, the ruins become eligible.
- If the game allows demolition and the city can afford it, the Department removes them.

**The underlying commercial zoning remains in place**, allowing new development later.

Ruins on ordinary residential, commercial, industrial and agricultural land follow this rule.

**Municipal and historic ruins are protected by default.**

They can only be included if you explicitly enable **Demolish destroyed municipal and historic buildings**.

<a id="demolition-abandoned"></a>
#### Abandoned buildings

When **Demolish abandoned buildings** is enabled, the Department considers buildings that have remained abandoned for a sufficiently long time.

By default, it waits **12 simulated months**, equivalent to 365 days.

An abandoned building must meet all of these conditions:

- It belongs to an enabled zone type.
- Its abandonment reason is enabled, or it has one of the reasons that are always eligible.
- It has been abandoned for at least the configured waiting period.
- It is neither municipal nor historically protected.
- SimCity 4 confirms that the lot can be demolished.

**Default behavior**

The Department can demolish abandoned residential, commercial and industrial buildings when their abandonment is caused by:

- An unattractive area.
- Excessive commuting time.
- Insufficient demand.

Abandonment caused by missing electricity or water is excluded by default.

This is intentional: a building abandoned because of a temporary service problem may become occupied again when that service is restored.

<a id="demolition-delays"></a>
#### How do the waiting periods work?

The options window lets you choose waiting periods in **simulated months**.

Internally, however, SimCity 4 records how long a building has remained in its current state using **simulated days**.

The Department therefore converts the selected number of months into days.

**Waiting period conversion**

```math
D=\operatorname{round}\left(\frac{M\times365}{12}\right)
```

Where:

- **M** is the selected waiting period, in simulated months.
- **D** is the waiting period stored and used by the Department, in simulated days.

The result is rounded to the nearest whole day, with exact half-day values rounded up.

**Examples**

| Months selected | Days used |
|---:|---:|
| 1 | 30 |
| 2 | 61 |
| 3 | 91 |
| 6 | 183 |
| 12 | 365 |
| 24 | 730 |
| 36 | 1,095 |
| 60 | 1,825 |

#### Available waiting periods

| Building type | In-game range | Default |
|---|---|---|
| **Ruins** | 1–24 months | 1 month (30 days) |
| **Abandoned buildings** | 1–60 months | 12 months (365 days) |

If you edit the configuration file manually, the permitted stored ranges are wider:

- **Ruins:** 1–3,650 days.
- **Abandoned buildings:** 1–7,300 days.

#### How does the Department know when a lot is old enough?

It uses the game's own counter.

**Demolition age requirement**

```math
A\ge D
```

Where:

- **A** is the number of simulated days since the lot became abandoned or destroyed.
- **D** is the configured waiting period in days.

For example, if the waiting period is 91 days:

- At age 85 days, the lot is too recent.
- At age 91 days, it becomes eligible.
- It is actually processed at the first monthly review after reaching that age.

**Demolition can therefore happen up to approximately one simulated month after the waiting period is reached.**

#### What if I edit the waiting period directly in the configuration file?

The options window converts stored days back into an approximate number of months.

```math
M_{\mathrm{shown}}=
\min\left(
M_{\max},
\max\left(
1,\operatorname{round}\left(\frac{D\times12}{365}\right)
\right)
\right)
```

Here, **Mmax** is 24 months for ruins and 60 months for abandoned buildings.

For example, a manually configured delay of **45 days** appears as approximately **1 month** in the options window.

However, the underlying 45-day value is preserved until you actually change the waiting-period control and accept the change.

**Simply opening the options window does not rewrite your manually configured delay.**

<a id="demolition-zones"></a>
#### Which zones can the Department demolish?

The Department provides four zone options:

| Zone option | Default |
|---|---|
| **Residential zone** | On |
| **Commercial zone** | On |
| **Industrial zone** | On |
| **Agricultural zone** | Off |

These options determine which **abandoned buildings** are eligible.

For example, disabling Commercial zone prevents the Department from demolishing abandoned commercial buildings.

**These restrictions do not apply to ruins.**

If automatic ruin demolition is enabled, destroyed buildings on growable zones can still be cleared, regardless of the zone options above.

Municipal lots remain protected independently of these settings.

<a id="demolition-reasons"></a>
#### Which abandonment reasons are eligible?

You can also choose which reasons for abandonment should allow automatic demolition.

| Abandonment reason | Default |
|---|---|
| **Reason: unattractive area** | On |
| **Reason: commute too long** | On |
| **Reason: no demand** | On |
| **Reason: no power** | Off |
| **Reason: no water** | Off |

**Why are No power and No water disabled?**

Because buildings abandoned through missing services may become occupied again.

For example, if a residential building loses electricity, the Department normally leaves it alone while you restore the power supply.

You can enable these reasons if you prefer to demolish such buildings after the waiting period.

#### Reasons that are always eligible

Two game-reported situations have no separate options:

- **Occupants evicted.**
- **No reason or an unrecognized abandonment reason.**

These reasons are always considered eligible when automatic abandoned-building demolition is enabled.

The normal waiting period, zone selection, protected-building rules and financial checks still apply.

<a id="demolition-protected"></a>
#### Municipal and historic buildings

The Department protects buildings that represent deliberate player choices.

**Municipal buildings**

This group includes:

- Schools, hospitals, police and fire stations.
- Parks, landmarks, rewards and other plopped buildings.
- Utilities.
- Military facilities.
- Airports, seaports and spaceports.
- Landfills.
- Lots without ordinary zoning.

**Historic buildings**

Any lot carrying SimCity 4's historic designation is also protected.

**Standing or abandoned municipal and historic buildings are never demolished automatically.**

This protection applies regardless of your abandonment settings.

#### What about their ruins?

There is one optional exception.

**Demolish destroyed municipal and historic buildings** is **off by default**.

When disabled, ruins of municipal and historic buildings remain untouched.

When enabled, the Department may clear those ruins after the normal ruins waiting period, provided:

- The lot is actually in the Destroyed state.
- The game's demolition check accepts it.
- The treasury and budget rules permit the demolition.

**This option never authorizes demolition of a standing municipal or historic building.**

It applies only after the building has already been destroyed.

<a id="demolition-farms"></a>
#### Special care for agricultural lots

Agricultural lots behave differently from ordinary residential, commercial and industrial buildings.

In SimCity 4, some farm field lots can appear in the game's abandoned state as part of their normal behavior.

They may also be recorded as lacking water.

**This does not necessarily mean the farm is no longer functioning.**

For that reason, **Agricultural zone is disabled by default**.

With this option off, abandoned farms and agricultural fields are preserved.

If you enable it, the Department can demolish eligible abandoned agricultural lots, including fields that you may want to keep.

**Use this option carefully.**

Ruins on agricultural land are treated separately. They can still be demolished when **Demolish destroyed buildings** is enabled, even if Agricultural zone is disabled.

#### Agricultural water-warning icons

The Department also hides the game's *no water* warning icon over agricultural lots on the supported game version.

This visual adjustment is independent of automatic demolition and does not change the actual water service.

See [Water](#water-farm-icon).

<a id="demolition-money"></a>
#### How does the Department pay for demolitions?

**Every demolition uses the actual bulldozing price reported by SimCity 4.**

The Department does not estimate the price from the building type.

Before each demolition order, it asks the game for the cost of that specific lot.

If the game cannot provide a valid price, **the lot is not demolished**.

Two financial protections apply.

#### 1. The treasury protection

The Department never orders demolitions whose combined cost exceeds the city's available treasury at the beginning of the monthly review.

**Treasury rule**

```math
c_i\le T-S_i
```

Where:

- **cᵢ** is the bulldozing price of the current lot.
- **T** is the treasury at the beginning of the demolition review.
- **Sᵢ** is the combined price of demolition orders already sent during that review.

**Why does the Department subtract earlier orders?**

Because SimCity 4 does not necessarily deduct bulldozing expenses immediately when a demolition order is submitted.

Without this protection, the Department could issue several orders against the same available money.

For example, if your treasury contains §500 and the Department has already ordered §300 of demolitions, it considers only §200 available for further orders.

**What happens when one lot is too expensive?**

The Department skips that lot and continues to the next candidate.

A cheaper building may still be affordable.

**If the treasury is negative, no demolition is ordered, not even one with a price of §0.**

This treasury protection is always active, even when you disable the optional monthly demolition budget.

#### 2. The optional monthly demolition budget

You can also limit how much the Department may spend during a single monthly demolition review.

The option is:

**Limit the monthly demolition budget**

It is **off by default**.

When enabled, the Department applies an additional rule.

**Monthly budget rule**

```math
c_i=0
\quad\text{or}\quad
S_i+c_i\le B
```

Where **B** is your configured monthly demolition budget.

The default budget value is **§1,000**.

**A demolition with a price of §0 does not count against this optional budget.**

However, there is an important difference between this rule and the treasury protection.

**If a priced demolition would exceed the monthly budget, the entire demolition review stops.**

It does not skip that building and continue looking for cheaper ones.

Every remaining candidate waits until the next month, including any later lots that would have been free to demolish.

#### 3. Maximum demolition orders

The Department also limits the number of demolition orders in one monthly review.

```math
N\le
\begin{cases}
3, & \text{before any order has been confirmed in the session}\\
150, & \text{after confirmation}
\end{cases}
```

The normal limit of 150 orders is a technical default.

It can be adjusted through the configuration key:

`MaxPerRun`

Allowed range: **1–1,000 orders**.

This setting is not available in the in-game options window.

The treasury and optional budget often impose lower practical limits.

<a id="demolition-never"></a>
#### When will the Department never demolish a lot?

The Department leaves a lot untouched when any of the following applies:

- The Department is disabled, the relevant demolition feature is disabled, or the demolition configuration is missing or invalid.
- The city has just loaded and the first monthly demolition review has not yet occurred.
- The building is occupied, under construction, waiting for development or already being demolished.
- It is a municipal or historic building that has not been destroyed.
- It is a protected municipal or historic ruin and the special ruins option is disabled.
- It is abandoned in a zone type you have excluded.
- It is abandoned for a reason you have excluded.
- It has not reached the configured waiting period.
- SimCity 4 reports that the lot cannot be demolished.
- The game cannot provide a valid demolition price.
- The treasury cannot afford the demolition.
- The optional monthly demolition budget prevents the order.
- The lot has changed or its map boundaries cannot be verified.
- A demolition safety failure has stopped management for the current session.
- The game version is unsupported.

**When there is doubt about a lot's eligibility or the safety of the operation, the Department leaves it alone.**

<a id="demolition-safety"></a>
### 4. Safety conditions

Automatic demolition is a potentially destructive operation, so the Department performs several checks before and after every order.

#### Using the game's own bulldozing system

The Department uses SimCity 4's normal demolition command.

It sends the exact rectangle occupied by the eligible lot and does not invent special demolition parameters.

**It does not directly remove buildings from the game's internal data.**

#### Checking the lot immediately before demolition

Before each order, the Department reads the lot again.

It must still:

- Exist at the expected position.
- Be in the expected state.
- Meet the demolition rules.
- Have the expected dimensions.

The map rectangle must have exactly the lot's area and contain its origin cell.

If these conditions fail, the lot is skipped.

#### Making sure the price check changes nothing

The Department asks the game for the bulldozing preview price.

This question must not modify the lot.

If the lot changes unexpectedly during the price check, **automatic demolition stops for the remainder of the city session**.

#### Verifying that demolition actually happened

Demolition orders are not necessarily completed at the exact moment they are sent.

The Department therefore checks the result during the next 10-day scan or monthly review.

The lot must have disappeared.

If **three consecutive demolition orders are not carried out by the game**, automatic demolition stops for the rest of the city session.

#### Preserving zoning

After confirming that the building has disappeared, the Department checks the zoning of the lot's origin square.

**The original zone must still exist.**

If the demolition unexpectedly removes the zoning, automatic demolition stops for the rest of the session.

This protects the player's intended city layout.

#### Handling configuration and technical errors

If the demolition configuration file is missing, unreadable or invalid, the Department performs no automatic demolition.

Errors during a demolition review are caught and recorded in the log. They do not stop SimCity 4 itself.

**Demolition operates independently of the other Department features.**

A demolition safety stop does not disable electricity, water, sanitation, education, healthcare or regional contract management.

Likewise, a failure in another manager does not automatically disable demolition.

<a id="demolition-settings"></a>
### 5. Settings

Automatic demolition is configured on the **Demolition** page of the Department's options window.

The settings appear in the following order:

| Setting | Default | What it controls |
|---|---|---|
| [Demolish destroyed buildings](#opt-demolishdestroyed) | On | Automatically clears eligible ruins. |
| [Wait (ruins)](#opt-destroyedmonths) | 1 month | How long ruins must remain before demolition. |
| [Demolish destroyed municipal and historic buildings](#opt-demolishmunicipal) | Off | Allows demolition of protected buildings' ruins only. |
| [Demolish abandoned buildings](#opt-demolishabandoned) | On | Automatically clears eligible abandoned buildings. |
| [Wait (abandoned)](#opt-abandonedmonths) | 12 months | How long buildings must remain abandoned. |
| [Residential zone](#opt-zoneresidential) | On | Includes abandoned residential buildings. |
| [Commercial zone](#opt-zonecommercial) | On | Includes abandoned commercial buildings. |
| [Industrial zone](#opt-zoneindustrial) | On | Includes abandoned industrial buildings. |
| [Agricultural zone](#opt-zoneagriculture) | Off | Includes abandoned farms and fields. |
| [Reason: unattractive area](#opt-reasonlowdesirability) | On | Includes abandonment caused by low desirability. |
| [Reason: commute too long](#opt-reasoncommute) | On | Includes abandonment caused by long commutes. |
| [Reason: no demand](#opt-reasonlowdemand) | On | Includes abandonment caused by insufficient demand. |
| [Reason: no power](#opt-reasonnopower) | Off | Includes abandonment caused by missing electricity. |
| [Reason: no water](#opt-reasonnowater) | Off | Includes abandonment caused by missing water. |
| [Limit the monthly demolition budget](#opt-budgetlimit) | Off | Limits demolition spending during each monthly review. |
| [Monthly budget](#opt-demolitionbudget) | §1,000 | Sets the spending limit when the budget option is enabled. |

All automatic demolition settings depend on [Department active](#opt-master).

**Recommended precautions**

The default settings protect agricultural fields, buildings abandoned because of temporary utility failures, and municipal or historic structures.

If you want more aggressive demolition, review these protections before changing the options, especially in cities containing valuable historical buildings or large agricultural areas.

<a id="demolition-examples"></a>
### 6. Worked examples

These examples show how the Department applies the waiting periods and financial limits.

**All demolition prices below are illustrative.** Actual prices are obtained from SimCity 4 for each individual lot.

#### Example 1: A ruin reaches its waiting period

You change the ruins waiting period to **3 months**.

The Department stores this as:

**91 simulated days.**

A building is destroyed by fire.

At a monthly review, the game reports that the ruins are **85 days old**.

```math
85<91
```

The building is too recent and is left alone.

At the following monthly review, approximately 30 days later, the ruins are about **115 days old**.

```math
115\ge91
```

The lot is now eligible.

**The Department demolishes it during that review if the game's eligibility and financial checks permit it.**

The waiting period is a minimum age, not an exact demolition appointment.

#### Example 2: The monthly budget stops a demolition review

Suppose your city has:

- **Treasury:** §20,000.
- **Monthly demolition budget:** Enabled.
- **Budget limit:** §1,000.

The Department has already confirmed successful demolition in the current session, so the initial three-order precaution no longer applies.

The candidates are sorted with ruins first, followed by abandoned buildings.

| Order | Candidate | Price | Spent before | Budget result |
|---|---|---:|---:|---|
| 1 | Ruin A | §40 | §0 | Demolish |
| 2 | Ruin B | §0 | §40 | Demolish |
| 3 | Abandoned C | §300 | §40 | Demolish |
| 4 | Abandoned D | §450 | §340 | Demolish |
| 5 | Abandoned E | §700 | §790 | **Stop: exceeds budget** |
| 6 | Abandoned F | §0 | — | Waits until next month |

After the first four orders, the Department has committed:

```math
40+0+300+450=790
```

**§790 in demolition expenses.**

The next building costs §700.

```math
790+700=1490>1000
```

This would exceed the configured monthly budget.

**The Department stops the entire demolition review.**

Buildings E and F wait until the next month.

Notice that F would cost nothing to demolish, but is still postponed because the budget rule stops the review at the first priced candidate that exceeds the limit.

#### Example 3: The treasury rule skips an expensive building

Now consider a smaller city with:

- **Treasury:** §250.
- **Monthly demolition budget:** Disabled.

The Department evaluates:

| Order | Candidate | Price | Available before | Decision |
|---|---|---:|---:|---|
| 1 | Ruin A | §40 | §250 | Demolish |
| 2 | Ruin B | §60 | §210 | Demolish |
| 3 | Abandoned C | §300 | §150 | **Skip** |
| 4 | Abandoned D | §100 | §150 | Demolish |

After ordering A and B, the Department has committed §100.

Building C costs §300, which the city cannot afford.

**The Department skips C but continues checking other candidates.**

Building D costs only §100 and can still be demolished.

The resulting orders cost:

```math
40+60+100=200
```

**Three buildings are ordered for a total of §200.**

Building C remains until a later review when sufficient money is available.

**The distinction is important:**

- **Treasury failure:** Skip the unaffordable lot and continue.
- **Monthly budget failure:** Stop the review completely.

#### Example 4: Installing the Department in a city with many abandoned buildings

Imagine a city containing **400 buildings** that have already been abandoned for more than a year because of insufficient demand.

You install the Department with its default demolition settings.

These buildings are already old enough to qualify.

However, **nothing is demolished immediately when you open the city**.

At the first simulated month change, the Department sends at most **three demolition orders**.

During the following 10-day check, it confirms that the game carried out those orders.

Once at least one demolition has been confirmed, the normal limit becomes available.

At the next monthly review, the Department can order up to **150 demolitions**, provided the treasury and optional monthly budget allow them.

The oldest eligible buildings are processed first.

**This initial three-order precaution prevents the Department from immediately attempting hundreds of demolitions before confirming that the game's demolition command works correctly.**

<a id="demolition-limitations"></a>
### 7. Known limitations

Automatic demolition reduces repetitive manual work, but several restrictions are intentional.

#### Timing and eligibility

- **Demolition is monthly.** A building is processed at the first monthly review after its waiting period, potentially up to approximately one simulated month later.
- **Existing abandonment age is preserved.** Buildings abandoned before the Department was installed keep their original age. They may therefore be eligible at the first monthly review.
- **Every city session begins conservatively.** Until a demolition order has been confirmed, each monthly review sends at most three orders.
- **The Department trusts the game's abandonment reason.** It does not predict whether a building might become occupied again. The waiting period and reason options provide that protection.

#### Demolition costs

- **The game determines bulldozing prices.** The Department reads the actual price before each order but does not display a separate cost notification.
- **Demolition expenses appear as normal bulldozing costs.** Ruins are generally cheaper to clear than abandoned buildings, but actual prices depend on the lot.
- **The monthly budget applies to one demolition review.** When enabled, it stops the review at the first priced candidate that would exceed the limit.

#### Protected buildings

- **Abandoned municipal and historic buildings are never demolished.** Only their ruins may be removed, and only when the separate protection option is enabled.
- **Agricultural fields can be affected.** Enabling Agricultural zone permits demolition of eligible abandoned field lots. Use this option carefully.
- **The Department never deliberately removes the underlying zoning.** Land remains available for redevelopment.

#### Technical limits

- **Safety stops last for the current city session.** Reopening the city allows demolition management to start a new session.
- **Only the currently open city is managed.** The Department does not inspect or demolish buildings in neighboring cities.
- **Automatic demolition requires the supported game version.** Unsupported versions are not modified.

**The Department's purpose is to clear eligible ruins and long-abandoned buildings, not to make redevelopment decisions in place of the player.**

---

<a id="safety"></a>
## G. Safety, Automation & Decision-Making

The Department can change budgets, manage regional contracts, clear abandoned buildings and correct certain landfill problems.

Because these actions affect your city directly, **protecting essential services and respecting your decisions must always come before saving money**.

This chapter explains the rules shared by the Department's automatic features:

- How it decides which goals take priority.
- When it reads the city and recalculates its plans.
- How it verifies changes before continuing.
- What happens when game information is incomplete or inconsistent.
- How technical failures are contained.
- Which decisions remain under your control.
- How its automatic actions and advisor messages work.

<a id="safety-vanilla"></a>
### 1. How SimCity 4 works without the Department

In the original game, most service funding and regional trade decisions are manual.

#### Service budgets

Schools, hospitals and power plants have individual funding controls.

Water and sanitation each have a single department budget.

SimCity 4 accepts funding changes through these controls, but **does not automatically optimize them as demand changes**.

#### Game statistics

The simulation updates demand, production, consumption, capacity and regional contracts according to its own internal schedule.

These values do not necessarily change immediately after a budget adjustment.

For example:

- Water production capacity may update at the next simulated month.
- Sanitation efficiency and treatment capacities may take several simulated days to reflect new funding.
- Some statistics may initially be zero after opening a city because the simulation has not populated them yet.

**The Department must distinguish between a genuine zero and information that is not ready.**

#### Advisor messages

SimCity 4 has an advisor system that displays news and warnings.

Each message has an advisor, a mood and a priority.

Depending on the game's difficulty or advisor settings, high-priority messages may appear as pop-ups.

The game's pop-up priority thresholds are **60, 80 or 100**, depending on the selected skill level.

Messages below the threshold normally appear in the advisor news list without opening a pop-up.

Each message remains available for its configured duration.

#### Regional contracts

The original game also offers, renews and cancels regional contracts, with its own advisor messages.

The Department can automate those operations when enabled.

<a id="safety-problem"></a>
### 2. The management problem

Automatic management can save money and reduce repetitive work, but it can also create problems if decisions are made without sufficient safeguards.

For example, an automated system could:

- Reduce funding below the level needed to maintain a service.
- Sell electricity or water that the city actually needs.
- Make decisions using incomplete statistics immediately after loading.
- Apply another change before the previous one has taken effect.
- Repeat a contract or funding operation that the game has already rejected.
- Demolish buildings that should be protected.
- Flood the player with notifications about routine actions.

**The Department is designed to avoid these problems first and optimize expenses second.**

<a id="safety-principles"></a>
### 3. The Department's general rules

When several objectives conflict, the Department follows this order of priorities:

1. **Maintain essential services.** Avoid leaving the city without electricity, water, sanitation, education or healthcare.
2. **Protect the configured reserves.** Keep sufficient capacity above the city's expected needs.
3. **Respect the reserve's safety limits.** Do not bypass the rules intended to protect service continuity.
4. **Respect SimCity 4's restrictions.** Use only the funding levels, capacities and contract quantities permitted by the game.
5. **Minimize monthly expenses.** Choose the cheapest safe operating arrangement.
6. **Sell genuine surplus when profitable.** Additional revenue must justify the cost of providing it.
7. **Avoid buying and selling simultaneously by default.** Mixed trading requires the optional setting.
8. **Never invent missing information.** Unreadable or inconsistent values are not treated as valid measurements.
9. **Avoid unnecessary delays.** A decision that is safe and ready does not receive an artificial cooldown.
10. **Verify every change.** Read the new state, confirm the result and recalculate before continuing.

These principles apply to the Department's individual managers, with the resource-specific safeguards described in the earlier chapters.

<a id="safety-service-first"></a>
#### Essential services always come first

The Department coordinates funding and regional contracts to avoid interrupting supply during a transition.

#### Changes are applied in a safe order

For electricity, water and garbage, the Department uses six execution phases:

| Phase | Action |
|---|---|
| **1** | Increase local capacity |
| **2** | Reduce or cancel sales |
| **3** | Create or increase purchases |
| **4** | Create or increase sales |
| **5** | Reduce local capacity |
| **6** | Reduce or cancel purchases |

**Each resource executes only its earliest pending phase during a review.**

For example, the Department does not reduce power plant funding before the electricity purchase intended to replace that production is active.

If a capacity increase has not yet taken effect, subsequent phases for that resource wait.

There is one important distinction for sanitation: a funding reduction may be postponed because sanitation writes are deliberately gradual.

**An unfinished sanitation reduction does not necessarily prevent a later reduction in garbage-disposal purchases**, because keeping higher local disposal capacity cannot harm service continuity.

#### Funding is not reduced during a genuine shortage

The Department distinguishes between insufficient supply and problems within the city's distribution network.

For water, for example, it refuses a reduction when buildings lack water **and production capacity is below demand**.

If sufficient water is available but some buildings are disconnected from the pipe network, a funding increase would not solve the problem. Such a connection issue does not automatically prevent an otherwise safe reduction.

See [Water safety conditions](#water-no-cut).

#### Sales stop during confirmed electricity or water deficits

If the game's counters confirm that the city does not have enough electricity or water, **the Department does not maintain sales of that resource**.

Local demand takes priority over export income.

#### Education and healthcare increase funding promptly

Education and healthcare use a different timing strategy.

The Department can raise funding:

- During the first simulated day after opening the city.
- When a service-capacity risk is detected.
- During additional checks while a strike or persistent risk is present.

**Funding reductions wait for the monthly evaluation.**

This allows the Department to respond quickly to overcrowded schools or hospitals while avoiding unnecessary reductions between monthly reviews.

See [Education & Healthcare](#education-healthcare-timing).

<a id="safety-reserves"></a>
#### Safety reserves: minimum capacity, not maximum capacity

Each managed service has a configurable reserve.

**A reserve is a minimum requirement, never a maximum.**

The Department may maintain more capacity when necessary because of the available funding settings, service fluctuations or profitable regional sales.

It must not deliberately choose a plan that falls below the applicable protected requirement.

| Service | Default reserve | Allowed range | How it works |
|---|---:|---:|---|
| **Education and healthcare** | 5% | 0–50% | Adds spare capacity above usage, with additional protection for small or fluctuating facilities. |
| **Electricity** | 20% | 10–50% | Protects the portion of demand supplied by local power plants, while also respecting the game's electricity requirement. |
| **Water** | 10% | 10–50% | Protects the portion of demand supplied by local water facilities. |
| **Sanitation** | 2% | 2–20% | Adds a margin to garbage produced after recycling, alongside any existing uncollected garbage. |

See [Education & Healthcare](#education-healthcare-required), [Electricity](#electricity), [Water](#water-required-capacity) and [Garbage & Landfills](#garbage-landfills-need).

**What if a reserve value is invalid?**

If you edit a configuration file manually and enter a reserve outside its permitted range, the Department does not silently adjust the value.

**The invalid configuration is rejected, and the affected management component remains disabled until it is corrected.**

This avoids applying unexpected safety margins that you did not actually configure.

<a id="safety-economy"></a>
#### How does the Department minimize expenses?

Once the service and safety requirements are satisfied, the Department searches for the cheapest suitable arrangement.

Different services use different methods because SimCity 4 handles their funding differently.

| Service | Optimization method |
|---|---|
| **Education and healthcare** | Choose the lowest whole-number funding percentage that provides the required capacity. |
| **Electricity** | Find the cheapest combination of individual power plant funding levels. |
| **Water** | Choose the lowest safe department funding percentage. |
| **Sanitation** | Simulate garbage disposal through the remaining days of the month and find the cheapest funding that can handle the waste. |
| **Regional contracts** | Compare contract income and expenses with the actual cost of local production or disposal. |

#### How quickly are funding changes applied?

**Electricity and water**

The Department applies the selected funding percentage directly, without an artificial step limit.

The applicable safety checks and coordination with regional contracts still apply.

**Sanitation**

Funding reductions are deliberately gradual:

- At most **25 percentage points** per reduction.
- At least **7 simulated days** between sanitation funding reductions.
- At least **3 simulated days** since the previous sanitation funding write before an increase.

These delays allow the game's sanitation efficiency and treatment capacity to catch up with funding changes.

**Regional contracts**

The Department creates, changes or cancels contracts only when the resulting plan is economically justified and satisfies the relevant service requirements.

See [Neighbor Deals](#neighbor-deals-objective).

<a id="safety-physical"></a>
#### Respecting the game's physical limits

The Department does not override SimCity 4's normal operating restrictions.

It uses:

- Funding percentages accepted by the game.
- Whole-number funding choices where required.
- The configured education and healthcare range, normally **1–120%**.
- The water pipe-protection floor when that option is enabled.
- Actual landfill storage and intake limits.
- Contract quantities the game permits.
- Prices returned by the game's own contract and demolition functions.

**The Department never constructs buildings or infrastructure automatically.**

It also does not:

- Create ordinary zoning.
- Change tax rates.
- Change city ordinances.
- Adjust unrelated budgets.
- Modify neighboring cities.

The one zoning-related exception is the narrowly controlled [ghost-square repair](#ghost-squares), which uses the game's own zoning system to correct erroneous landfill membership without deliberately changing legitimate landfill zoning.

<a id="safety-reevaluation"></a>
#### When does the Department recalculate its decisions?

The Department combines regular reviews with lighter checks for significant changes.

**It does not wait for an arbitrary fixed delay after opening a city.**

Each service begins evaluating as soon as the information it needs is available, current and consistent.

Demolition is an exception: it starts at the first simulated month change after loading.

#### Checking whether game statistics are reliable

Before making decisions about electricity or water, the Department compares reported demand with actual consumption.

A small difference can result from the game's own rounding or update timing.

However, clearly contradictory readings prevent automatic funding decisions.

**Inconsistent service readings**

```math
R>D\times1.01+1
```

Where:

- **D** is the game's reported demand.
- **R** is the amount of the service reported as delivered or consumed.

If consumption exceeds demand by more than **1% plus one unit**, the readings are treated as inconsistent.

The Department waits for fresh data rather than acting on an unreliable state.

Other signs that the simulation is not ready include:

- Demand still reading zero immediately after loading.
- Watered and unwatered building counts not yet populated.
- Missing readable power plant funding or production information when plant funding management requires it.

#### Daily service checks

Every simulated day, the Department reads a small number of electricity and water counters.

These checks are deliberately lightweight.

They do not require scanning every building or rebuilding the entire regional contract market.

**An identical reading is not treated as a new observation.**

The Department only triggers an early evaluation when something relevant has actually changed.

#### Detecting a genuine regional supply shortage

A city may report insufficient consumption for two very different reasons:

1. It does not have enough electricity or water available.
2. It has enough supply, but the service cannot reach some buildings.

The Department uses the following test to distinguish them.

**Regional supply deficit**

```math
S<D
\qquad\text{and}\qquad
C+I-E<D
```

Where:

- **D** is the city's demand.
- **S** is the service actually delivered.
- **C** is local production capacity.
- **I** is usable imported supply.
- **E** is exported supply.

All quantities use the relevant resource's monthly units.

**A genuine shortage exists when consumption is below demand and the combined usable supply is also insufficient.**

For example, if a city needs 20,000 units but has only 16,000 units available after imports and exports, the Department can immediately reconsider funding and contracts.

However, suppose the city needs 20,000 units and has 25,000 units of available capacity, yet some buildings remain unserved.

The problem is likely inside the city, such as disconnected electricity lines or water pipes.

**The Department records the issue but does not unnecessarily replan production just because the buildings lack a connection.**

#### Detecting significant changes before the next regular review

The Department can also react when demand, local capacity, imports or exports change by at least **1%**.

**Significant change threshold**

```math
|x-x_0|\ge0.01\times\max(1,|x_0|)
```

Where:

- **x** is the latest reading.
- **x₀** is the value recorded at the last evaluation.

**Example**

Suppose electricity demand was 50,000 units during the previous review.

| New demand | Change | Result |
|---|---:|---|
| 50,400 | 0.8% | Wait for the normal review |
| **50,600** | **1.2%** | **Trigger an early evaluation** |

A significant new reading can bring the next evaluation forward without waiting for the regular interval.

**There is no additional cooldown for a decision that is ready and safe to apply.**

#### Regular evaluation intervals

| Service | When it is evaluated |
|---|---|
| **Electricity** | Every 10 simulated days, plus early reviews for confirmed shortages or significant changes. |
| **Water** | Every 10 simulated days since its last review, at every month change, and earlier when necessary. |
| **Sanitation** | At every month change and when monitored garbage figures change significantly. |
| **Education and healthcare** | Initial raises after opening, risk checks every two days (daily during strikes), rechecks every seven days while risks persist, and monthly reviews for increases and reductions. |
| **Automatic demolition** | At every simulated month change. |
| **Landfill size advice** | At most once per simulated month, during an ordinary sanitation evaluation. |

**What happens while an earlier change is still being verified?**

The Department does not make overlapping changes to the same resource.

For example:

- A regional contract change must be confirmed before the next contract operation.
- A sanitation funding adjustment must reach the expected efficiency and treatment capacity before another adjustment is made.

While waiting, the Department continues checking the relevant game values.

**It does not perform a complete new optimization merely because a verification check is due.**

This prevents partially completed operations from interfering with one another while keeping the system responsive to genuine changes.
<a id="safety-verification"></a>
#### How does the Department verify its changes?

The Department does not assume that an operation succeeded simply because it sent a command to SimCity 4.

**Every automatic change is checked before and after it is applied.**

The verification depends on the type of operation.

| Operation | What the Department verifies |
|---|---|
| **School and hospital funding** | The new funding matches the requested value, and the school bus or ambulance budget remains unchanged. |
| **Power plant and water funding** | The game accepts the requested funding value. |
| **Sanitation funding** | The new funding is accepted, then the game's efficiency and treatment capacities update as expected. |
| **Regional contracts** | The contract state, amount, price and regional accounting agree with the requested change. |
| **Automatic demolition** | The building has disappeared and its underlying zoning remains intact. |

#### Before applying a change

The Department reads the building, department or contract again.

The value must still match the one used when the plan was calculated.

For example, suppose the Department plans to reduce water funding from 100% to 78%.

Before applying the change, it checks that the funding is still 100%.

If you moved the slider to 90% yourself, **the old plan is cancelled rather than applied to your new setting**.

The Department can then calculate a new plan using the current information.

#### After applying a change

The Department reads the resulting value.

For funding changes, the requested percentage must be confirmed.

For schools and hospitals, it also checks that the separate bus or ambulance budget has not changed.

Regional contracts require additional confirmation because the game's contract records and financial totals may update after the operation.

Sanitation also requires a later check because its efficiency and treatment capacities do not necessarily respond immediately to a funding change.

Demolitions are verified at a subsequent scan, when the game has had time to carry out the order.

**A change is not considered successful merely because the command was sent.**

The Department distinguishes between:

- **Planned:** A change has been calculated but not applied.
- **Pending:** A change has been submitted and is awaiting confirmation.
- **Verified:** The game reports the expected result.

Savings are counted as confirmed only when the relevant result has been verified.

<a id="safety-missing-data"></a>
#### What happens when the game does not provide enough information?

The Department follows a simple rule:

**Unknown information is not the same as zero.**

For example:

- A valid list containing no power plants means there are no power plants in that list.
- A failed attempt to read the power plant list does not prove that the city has no power plants.
- A landfill with a confirmed intake capacity of zero cannot accept more garbage.
- A landfill whose intake capacity cannot be read has an unknown intake capacity, not a confirmed zero.

The Department treats these situations differently.

#### Incomplete or invalid readings

If part of the necessary information is unavailable, the Department uses only values that were actually read and verified.

When a missing or invalid value makes the calculation unreliable, **the affected service does not perform a funding or contract change during that evaluation**.

It does not replace missing information with an earlier cached value or invent a plausible estimate.

Other services can continue operating independently.

#### Unknown neighboring cities or offers

If regional offers or neighboring-city records cannot be read, the Department does not assume that no alternative supply exists.

For example, it cannot conclude that an electricity shortage is impossible to resolve if it has not been able to inspect the available regional purchases.

**An incomplete contract market cannot support a definitive shortage warning.**

#### Unknown landfill geometry

The Department can calculate sanitation funding without knowing every detail of the landfill's internal cell structure.

Until that structure has been calibrated, it uses the conservative calculations described in [Garbage & Landfills](#garbage-landfills-algorithm).

It does not invent storage capacity or make next-month predictions that require unavailable information.

#### Unknown demolition prices

If SimCity 4 cannot provide the bulldozing price of a lot, **the Department does not demolish it**.

The operation can be reconsidered during a later monthly review.

<a id="safety-player-control"></a>
#### You remain in control

The Department automates repetitive management decisions, but you can choose which features it controls.

| What you want to do | How to do it |
|---|---|
| **Disable the entire Department** | Turn off **Department active**, or repeal **Establish a Department of Efficiency** in the game's Ordinances window. |
| **Manage a service manually** | Disable that service's management option, such as **Manage power** or **Manage education**. |
| **Manage regional contracts manually** | Disable **Manage Electricity Deals**, **Manage Water Deals** or **Manage Garbage Deals** for the relevant resource. |
| **Change a funding slider yourself** | Move it normally. The Department will use your new value at its next evaluation if management remains enabled. |
| **Create or cancel a contract yourself** | Use the normal Neighbor Deals window. The Department will consider the resulting contracts during its next review. |
| **Stop automatic demolition** | Disable both **Demolish destroyed buildings** and **Demolish abandoned buildings**, or disable the entire Department. |
| **Remove the plugin** | Close SimCity 4 and remove the plugin's files. Your city saves remain usable without it. |

#### What happens when you disable the Department?

**Disabling the Department does not undo earlier changes.**

For example, if it previously reduced electricity funding to 72%, switching it off does not restore the previous funding.

Likewise, regional contracts remain in their current states.

The Department simply stops making further automatic changes.

#### What happens when you move a slider manually?

You can always move a funding slider.

However, **a manual value is not locked while its manager is enabled**.

The Department treats your new value as the starting point for its next calculation.

Depending on the service:

- **Electricity:** The funding may be recalculated within days, no later than the regular 10-day review.
- **Water:** The funding may be recalculated within 10 simulated days.
- **Education and healthcare:** A manual reduction that creates a capacity risk can be corrected within days. A manual increase is reviewed during the monthly pass.

For example, if you increase water funding to 100% but leave **Manage water** enabled, the Department may lower it again when its calculations show that less funding is sufficient.

**To keep a specific funding value under your control, disable that service's manager.**

#### Are there any effects that cannot be switched off individually?

Yes.

On the supported game version, the plugin hides the original *no water* warning icon over agricultural lots whenever the plugin is loaded.

This does not change the water service itself.

See [Water](#water-farm-icon).

The Department also includes three resource files that modify some of SimCity 4's original advisor messages.

Those files operate whenever they are installed, regardless of the corresponding management options. Their behavior is explained under [Advisor Notices](#safety-notices-game).

<a id="safety-automation"></a>
#### Which features act automatically, and which only give advice?

Some Department features modify the city directly.

Others only display information so that you can decide what to do.

| Feature | Automatic action | Advisor notice |
|---|---|---|
| **Education and healthcare** | Adjusts funding | Warns when additional funding cannot resolve a persistent capacity problem |
| **Electricity** | Adjusts power plant funding | Warns about a persistent shortage that cannot be resolved automatically |
| **Water** | Adjusts department funding | Warns about a persistent shortage that cannot be resolved automatically |
| **Sanitation** | Adjusts department funding | Warns about a persistent garbage disposal shortage |
| **Neighbor contracts** | Creates, changes and cancels contracts | No separate routine contract notice |
| **Automatic demolition** | Removes eligible ruins and abandoned buildings | No notice |
| **Ghost landfill squares** | Repairs eligible ghost squares automatically | No notice |
| **Landfill size advice** | **Never changes landfill size** | Optionally recommends a specific expansion or reduction |

**Landfill size advice is the main feature that recommends a change without carrying it out.**

It may tell you that expanding or reducing a landfill could save money, but the zoning decision remains yours.

<a id="safety-performance"></a>
#### How does the Department avoid slowing down the game?

The Department is designed to perform only the work needed at each point in the simulation.

**All simulation work runs on the game's own thread, between frames, using its normal daily and monthly events.**

It does not create a separate background simulation.

The workload is divided as follows:

| Operation | Frequency or approach |
|---|---|
| **Daily service monitoring** | Reads a small number of game counters. |
| **Full resource optimization** | Runs only when the service is due for evaluation or a meaningful change occurs. |
| **Demolition lot scan** | Reads the full lot grid once per simulated month. |
| **Demolition verification** | Performs a lightweight scan every 10 simulated days, reading one state value per lot and recording changes. |
| **Options button detection** | A 250-millisecond timer checks only whether the game's Play Options window is available. |
| **Advisor notice checks** | Once per simulated day. |

**The daily monitoring checks do not repeat the expensive full calculations unnecessarily.**

For example, an unchanged electricity demand reading does not trigger another complete power plant optimization.

Similarly, landfill size advice runs within a normal sanitation evaluation, not through a separate continuous search.

<a id="notices"></a>
### Advisor notices

The Department deliberately avoids announcing its routine work.

It does not display messages every time it:

- Adjusts funding.
- Purchases or sells a resource.
- Changes or cancels a contract.
- Demolishes an abandoned building.
- Repairs a ghost landfill square.

**There is also no greeting or activation message.**

The Department only uses advisor notices when the player may need to intervene, or when an optional landfill recommendation could offer significant savings.

<a id="safety-notices-utility"></a>
#### Electricity, water and garbage shortage notices

The Department can warn you when a shortage persists and **the available automatic solutions have been exhausted**.

These notices come from the **Utilities advisor**.

| Resource | Notice title |
|---|---|
| **Electricity** | **By Candlelight!** |
| **Water** | **The Faucets Are on Strike!** |
| **Garbage** | **Garbage Takes Over!** |

These are urgent advisor messages.

Depending on your SimCity 4 advice settings, they can appear as pop-ups as well as entries in the news list.

They do not include a camera link.

#### When does the Department consider a shortage proven?

A shortage notice is not generated simply because demand temporarily exceeds supply.

The Department must first establish that **the shortage cannot be solved using the currently available legal options**.

For electricity and water, this requires checking:

- The maximum usable capacity of local facilities.
- Available regional purchases.
- The removal of sales that consume needed capacity.
- The city's actual demand and delivered service.
- The completeness of regional records and contract offers.

If you manage contracts manually, the Department evaluates the existing contracts rather than assuming it can create new ones.

The regional information must be complete enough to support the conclusion.

For electricity and water, the game's counters must also confirm a genuine regional deficit.

**A building that lacks water because it is disconnected from the pipe network does not, by itself, prove a regional supply shortage.**

For garbage, the Department relies on its physical disposal model to determine whether the city's waste can be handled.

This is a model prediction, not a direct game measurement of every future garbage flow.

Finally, **no contract change or sanitation funding adjustment for the affected resource may still be awaiting confirmation**.

The Department does not declare a shortage impossible to solve while its own corrective action is still in progress.

#### Why does the Department wait 30 days before warning you?

Shortages may disappear after a funding adjustment, contract change or other normal game update.

The Department therefore waits until a proven shortage has remained unresolved for **30 simulated days**.

**A shortage resolved during those 30 days produces no notice.**

This avoids unnecessary warnings about problems that automatic management successfully corrects.

#### When are reminders displayed?

**Utility shortage notice schedule**

```math
t_{\mathrm{first}}=t_{\mathrm{proof}}+30
```

```math
t_k=t_{\mathrm{first}}+kP
```

Where:

- **tproof** is the simulated day when the shortage was first proven.
- **tfirst** is the first notice date.
- **k** is the reminder number, starting at 1.
- **P** is the reminder interval.

| Resource | First notice | Reminder interval |
|---|---|---|
| **Electricity** | After 30 unresolved simulated days | Every 60 simulated days |
| **Water** | After 30 unresolved simulated days | Every 60 simulated days |
| **Garbage** | After 30 unresolved simulated days | Every 120 simulated days |

The Department continues reminders only while the shortage remains unresolved.

#### When does a shortage episode end?

For electricity and water, the episode ends when a fresh game reading confirms that delivered service is at least equal to demand.

For garbage, two conditions must be met:

- The disposal model becomes feasible again.
- The game reports no uncollected garbage.

Once recovery is confirmed, the Department stops reminders.

**A later shortage is treated as a new episode**, with its own 30-day waiting period.

#### What happens if you close and reopen the city?

The Department remembers which shortages it has already reported.

This information is stored **per city** in a small notice-history file inside the Department's folder, outside your city saves.

For example, if you receive a water shortage notice and reopen the city while the same shortage remains unresolved, the Department does not immediately display the same warning again.

After the shortage is proven again in the new session and remains unresolved for the normal waiting period, it schedules the next reminder rather than repeating the first notice.

If the session reaches its **75th simulated day** without that shortage being proven again, the previous episode's memory is discarded.

A later shortage can then start a new episode.

The notice-history file can be deleted without damaging city saves.

#### Which settings are required for shortage notices?

Utility shortage notices require **Manage neighbor deals** to be enabled.

This is because the Department needs to inspect regional offers and markets before concluding that a shortage cannot be resolved.

The affected resource must also be managed through either:

- Its local funding manager.
- Its corresponding regional contract manager.

**If the Department cannot examine all relevant alternatives, it does not claim that the shortage is impossible to solve.**

<a id="safety-notices-civic"></a>
#### Education and healthcare capacity notices

The Department can also warn you when schools or healthcare facilities remain seriously overcrowded despite the maximum available funding.

These notices are issued by the **Health and Education advisor**.

They appear as **red entries in the news list**, without opening a pop-up.

There is one notice per facility type:

- Elementary schools.
- High schools.
- Colleges and universities.
- Libraries.
- Museums.
- Clinics.
- Hospitals.
- Disease research centers.

#### When is a building included?

A building must satisfy all of the following:

1. Its maximum possible funding still cannot provide sufficient capacity.
2. Its maximum capacity covers **less than 85% of its usage**.
3. It is **not currently on strike**.
4. The problem has continued for **30 simulated days without interruption**.

**Coverage below 85% means that even at maximum funding, the facility cannot serve 85% of its current usage.**

For example, if a hospital needs capacity for 1,000 patients but can serve only 800 at maximum funding, its coverage is 80%.

If that situation persists for 30 simulated days without a strike, the hospital can appear in a capacity notice.

See [Education & Healthcare](#education-healthcare-notices) for the complete calculation.

#### What information appears in the notice?

A notice lists up to **10 affected buildings**, with the most severe problems first.

Each listed building has a link that moves the camera to its location.

**The Department does not create a separate notice for every overcrowded building.**

Instead, it groups buildings by facility type to keep the advisor list manageable.

#### What happens when the affected buildings change?

The Department maintains only one active notice for each facility type.

If the number of affected buildings stays the same but their identities change, it updates the existing notice's building links.

If the number changes, the old notice is retired before a replacement is shown.

**There are never two active notices for the same facility type.**

If two replacements occur within 30 simulated days, the Department introduces a **30-day pause** before further replacements for that type.

Updates to the links of an existing notice can still continue during the pause.

#### When is the notice removed?

When every affected building has recovered, the notice is retired.

A notice you manually dismiss is not shown again while the same buildings remain affected.

This dismissal state survives reopening the city.

At the first evaluation after opening, the Department also retires any leftover notice from a previous session.

A capacity notice can remain in the news list for up to **720 simulated days** while the problem persists.

<a id="safety-notices-landfill"></a>
#### Landfill size recommendations

The Department can issue two optional notices when changing landfill size would significantly reduce sanitation expenses.

Both come from the **Utilities advisor**, with a good-news mood and urgent priority.

| Recommendation | Notice title |
|---|---|
| **EXPAND** | **More Landfill, Less Spending?** |
| **REDUCE** | **Our Landfill Is Taking Up Too Much Room!** |

Each notice describes:

- The proposed area's dimensions in squares.
- The estimated monthly saving.
- The percentage reduction in sanitation expenses.
- A **show me where** link that moves the camera to the recommended area.

These notices are available only when:

- **Manage sanitation** is enabled.
- **Warn me when checking landfill size can help me save** is enabled.
- The estimated saving is strictly greater than your configured threshold.
- The city contains exactly one landfill zone.
- The timing and safety conditions described in the landfill chapter are satisfied.

The default threshold is **25%**, and landfill size advice is disabled by default.

The Department analyzes landfill size at most once per simulated month.

It also respects:

- The **24-month protection period** after landfill size changes or when a landfill is first encountered.
- The **12-month interval** after issuing advice.

See [Garbage & Landfills](#garbage-landfills-advice).

**Ghost-square repairs never generate advisor notices.**

<a id="safety-notices-game"></a>
#### Original SimCity 4 notices modified by the Department

The Department includes **three resource files** that change some of the original game's advisor messages.

Their purpose is to avoid warnings that are misleading or redundant while automatic management is active.

**These files are loaded when SimCity 4 starts.**

As long as they remain installed, their changes continue to apply even if you disable the related Department options during play.

| Resource file | Original game messages affected | Intended use |
|---|---|---|
| **Neighbor-deal advice file** | Suppresses original contract offers, cancellation suggestions, renewal messages and default warnings. | When automatic neighbor deal management is active for at least one resource. |
| **Education and healthcare advice file** | Suppresses normal funding advice and limits pre-strike rumors to periods when a strike is actually in progress. | When education or healthcare management is active. |
| **Garbage advice file** | Restricts garbage problem warnings to situations with a significant amount of uncollected garbage. | Always. |

The education and healthcare file does **not** suppress genuine strike alerts or positive advisor messages.

The garbage file leaves good-news messages unchanged.

#### Why are some original garbage warnings suppressed?

SimCity 4 sometimes warns about garbage problems simply because landfill storage is increasing.

However, a landfill gradually filling up is not necessarily evidence that the city's sanitation system is failing.

The Department therefore adds a requirement based on the amount of garbage actually left uncollected.

**Significant uncollected garbage**

```math
U>100+0.05G
```

Where:

- **U** is the game's current uncollected garbage quantity.
- **G** is the garbage produced by the city, using the game's advisor values.

The original garbage warning can appear only when this additional condition is satisfied, alongside the game's normal warning condition.

**Example**

If the city produces 20,000 garbage units, the threshold is:

```math
100+0.05\times20000=1100
```

The game must report **more than 1,100 units of uncollected garbage** before the modified garbage-problem warning is allowed.

This prevents unnecessary alerts when garbage disposal is functioning normally but landfill storage is increasing.

#### How can I restore the original game's advisor messages?

Because SimCity 4 loads these resource files at startup, changing the Department's options does not immediately restore the original messages.

If you disable a feature but leave its advisor resource file installed, the Department records a warning in its log.

The warning identifies which file should be removed.

To restore the corresponding original messages:

1. Close SimCity 4.
2. Remove the indicated advice resource file from the Plugins folder.
3. Restart the game.

**The advisor resource files affect the game's messages independently of whether the Department is currently making automatic management changes.**

<a id="safety-isolation"></a>
### 4. Safety conditions

#### How does the Department handle technical failures?

The Department is divided into independent management components.

**A failure in one component should not disable unrelated services or stop SimCity 4 itself.**

When a safety check fails, the affected component stops making changes rather than repeatedly trying an unreliable operation.

| Failure | What the Department does |
|---|---|
| **Power plant or water funding verification fails** | Attempts to undo the changes made in that cycle and freezes that resource's funding management for the city session. Other resources and regional contracts can continue. |
| **Sanitation funding verification fails** | Stops sanitation funding changes for the session. Garbage contracts and other resource managers can continue. |
| **A contract operation fails technically** | Stops automatic contract management for the session. Local funding managers continue using existing contracts. |
| **A school or hospital funding change cannot be verified** | Excludes that building for the session. After failures involving three different buildings, education and healthcare funding writes stop for the session. |
| **A building's funding control has no effect** | Excludes that building while allowing other buildings to continue. |
| **Automatic demolition fails a safety check** | Stops demolition for the session if a price preview changes a lot, zoning disappears, or three consecutive orders are not carried out. |
| **An advisor operation produces an error** | Stops checking and retiring Department notices for the rest of the session. |
| **The Play Options window has an unexpected layout** | Leaves the original window unchanged and does not add the Department button during that session. |
| **A configuration file is invalid** | Disables the affected management component rather than using unreliable settings. |

#### What does a session safety stop mean?

A safety stop lasts for the current city session.

For example, if water funding verification fails, the Department stops making additional water funding changes while that city remains open.

It does not repeatedly send the same command.

**Reloading the city starts a new session and clears session-specific safety stops.**

The underlying problem may still exist, so the same check can fail again if it has not been resolved.

#### Can a Department error crash SimCity 4?

The Department wraps the game events it handles so that internal errors are caught and recorded in its log.

**An error in Department management is intended to stop the affected operation, not the game itself.**

This isolation is a core part of its design.

<a id="safety-version"></a>
#### Supported game version

The Department checks the game executable's version when it starts.

**The supported version is SimCity 4 Deluxe 1.1.641.0 (Steam, Windows).**

On any other version, or when the version cannot be read, the Department remains completely inactive.

That means:

- No game-event subscriptions.
- No Department options button.
- No Department ordinance.
- No funding, contract or demolition operations.
- No agricultural water-icon modification.

The same complete inactivity applies when the required **Department of Efficiency** folder or main resource file is missing beside the DLL.

**The Department does not attempt to operate on an unsupported or incomplete installation.**

#### Your city saves do not depend on the plugin

The Department is designed so that cities saved while it is installed can still be opened without it.

Its **Establish a Department of Efficiency** ordinance is temporarily removed from the city before each save and restored afterwards.

The ordinance is therefore not stored in the saved city.

**The Department never directly edits city or region save files.**

If you remove the plugin, the city's ordinary funding levels and regional contracts remain at their last established values.

You can continue playing without the Department.

<a id="safety-settings"></a>
### 5. Settings

The general safety system does not require a separate collection of options.

It follows the management settings described throughout this guide.

**Department active**

[Department active](#opt-master) is the main switch.

You can also control it through the ordinance **Establish a Department of Efficiency** in SimCity 4's Ordinances window.

Both controls represent the same setting.

**Individual management options**

Each service has its own manager and, where appropriate, a reserve or safety margin.

See the [Option Reference](#options) for all settings.

**Advisor notices**

The Department has only two notice-specific options, both related to landfill size recommendations:

- [Warn me when checking landfill size can help me save](#opt-landfilladvice).
- [Landfill savings alert threshold](#opt-landfillthreshold).

**Utility shortage notices cannot be disabled separately.**

They follow the relevant service-management settings and appear only when their conditions are satisfied.

<a id="safety-examples"></a>
### 6. Worked examples

The following examples show how the Department responds to persistent shortages, incomplete information and manual changes.

#### Example 1: An electricity shortage that cannot be resolved

Suppose your city has the following situation on **simulated day 40** after opening:

- **Electricity demand:** 10,000 units.
- **Electricity delivered:** 9,200 units.
- **Maximum local generating capacity:** 8,900 units.
- **Usable regional imports:** 200 units.
- **Sales:** None.

The Department has already considered the maximum legal plant funding and every available purchase.

Total available supply is:

```math
8900+200=9100
```

That is less than the city's demand:

```math
9100<10000
```

The game's counters also confirm that electricity delivery is below demand.

**The shortage is proven on day 40.**

However, the Department does not immediately display a warning.

It waits 30 simulated days.

**Day 70**

The shortage is still unresolved.

The Department issues:

**By Candlelight!**

**Day 130**

The shortage continues.

The first reminder appears, 60 simulated days after the original notice.

**Day 150**

You build another power plant.

A fresh reading confirms that electricity delivered now meets demand.

The Department closes the shortage episode and stops the reminders.

**Day 200**

A new electricity shortage is proven.

Because the previous episode ended, the new shortage starts its own 30-day waiting period.

If it remains unresolved, the next notice appears on **day 230**.

#### Example 2: Buildings without electricity, but enough generating capacity

Consider a city with:

- **Electricity demand:** 10,000 units.
- **Electricity delivered:** 9,600 units.
- **Local generating capacity:** 12,000 units.
- **No imports or exports.**

Although some demand is not being served, the city has enough generating capacity.

```math
12000\ge10000
```

This means the supply shortage test is not satisfied.

The problem is likely inside the electricity distribution network.

**The Department does not increase funding or issue a regional shortage notice.**

More production would not necessarily reach disconnected buildings.

The player must inspect the electricity network.

#### Example 3: Reopening a city during an existing shortage

Suppose a water shortage was first reported on **day 70** of a city session.

You save the city on day 100 while the shortage remains unresolved.

Later, you reopen the city.

The Department begins a new session and evaluates the water system again.

The shortage is proven again and remains unresolved for another 30 simulated days.

However, the Department remembers that the same shortage episode was already reported.

**It does not repeat the first warning.**

Instead, it schedules the next reminder according to the normal 60-day interval.

This prevents the same unresolved problem from producing a new initial warning every time you reopen the city.

#### Example 4: Changing water funding manually

Suppose the Department has optimized water funding to **62%**.

You manually increase it to **100%**, while **Manage water** remains enabled.

The Department accepts your change as the current starting value.

At its next water evaluation, no later than 10 simulated days afterwards, it recalculates the necessary funding.

If it finds that **64%** is sufficient, it can reduce funding from 100% to 64%.

**The Department does not preserve manual slider changes while automatic management remains enabled.**

To keep water funding at 100%, disable **Manage water**.

#### Example 5: Water readings are not ready after loading

Immediately after opening a city, the game reports:

- **Water demand:** 20,000 units.
- **Water consumption:** 20,500 units.

The Department checks whether consumption is consistent with demand.

The maximum accepted reading is:

```math
20000\times1.01+1=20201
```

However:

```math
20500>20201
```

Consumption exceeds the permitted tolerance.

**The readings are inconsistent, so the Department does not make a water funding decision yet.**

It waits for a fresh reading.

Other services are evaluated using their own information.

**A temporary water-data problem does not prevent electricity, sanitation or other independent managers from operating.**

<a id="safety-limitations"></a>
### 7. Known limitations

The Department is designed to operate conservatively, but it cannot overcome every limitation of SimCity 4.

#### Manual control

- **Manual funding changes are not locked.** When a service manager is enabled, it may revise a funding percentage you changed yourself during its next evaluation.
- **There are no individual building locks.** To keep a particular service under manual funding control, disable the corresponding manager.

#### Shortage notices

- **Utility shortage notices require Manage neighbor deals.** Without access to regional offers and markets, the Department cannot prove that all possible solutions have been exhausted.
- **The Department cannot know whether a notice was actually displayed on screen.** SimCity 4 decides whether to show a pop-up according to its advisor settings.
- **Garbage shortage notices rely on a model.** Unlike electricity and water, garbage feasibility is determined through the Department's disposal projection rather than a direct game counter proving future disposal requirements.

#### Original game advisor messages

- **The three advice resource files act whenever installed.** Disabling the corresponding Department options does not immediately undo their effects.
- **Restoring original messages may require removing the relevant resource file and restarting SimCity 4.** The Department logs configuration mismatches.

#### Technical safety

- **A technical safety stop lasts for the current city session.** The affected manager remains disabled until the city is reloaded.
- **Only the supported game version is managed.** The Department supports **Steam SimCity 4 Deluxe 1.1.641.0 on Windows**. It performs no operations on unsupported versions.

**The Department is designed to optimize the city only when it has enough reliable information to act safely. When it cannot verify an operation or prove that a change is beneficial, it prefers leaving the existing situation unchanged.**

---

<a id="options"></a>
## H. Option Reference

This chapter explains how to open the Department's settings and how configuration changes are saved.

The detailed descriptions of individual options are provided in the following sections.

<a id="options-open"></a>
### Opening the options window

To access the Department's settings:

1. Open **Play Options** in SimCity 4, either while playing a city or from the region view.
2. Click **Department of Efficiency**, the button added below the original **Credits** button.
3. The Play Options window closes, and the Department's own settings window opens in the center of the screen.

The settings window has two pages:

| Page | What it contains |
|---|---|
| **Services** | Automatic funding, reserve margins, regional contracts and landfill size advice. |
| **Demolition** | Automatic demolition rules, waiting periods, protected lot types and demolition spending limits. |

Each setting appears as a labeled row with a checkbox or a numerical control.

**Hover over a setting's label or checkbox to see its tooltip.**

#### Why are some settings greyed out?

Some settings depend on another option being enabled.

For example, **Landfill savings alert threshold** cannot be adjusted while landfill size advice is disabled.

When a parent option is off:

- Its dependent settings are greyed out.
- You cannot change those settings.
- Their existing values are preserved.
- They become available again when the parent option is enabled.

**Disabling a feature does not erase its previous settings.**

Each option currently has at most one parent dependency.

<a id="options-accept"></a>
### Accept and Cancel

The settings window provides two actions.

#### Accept

Clicking **Accept** saves your changes.

The Department:

1. Identifies which settings you changed.
2. Updates only those settings in the relevant configuration files.
3. Preserves unrelated settings and existing comments.
4. Verifies that the updated files can be read back with exactly the selected values.
5. Replaces each configuration file as a complete operation only after validation succeeds.
6. Applies the new settings immediately.

**If validation fails, no changes are written and the settings window remains open.**

This prevents an invalid configuration from being partially saved.

#### Cancel

Clicking **Cancel** closes the settings window without saving any changes.

Closing the city or exiting SimCity 4 also closes the Department's window without writing unsaved settings.

#### The Department ordinance

The game also contains the ordinance:

**Establish a Department of Efficiency**

It appears in SimCity 4's normal Ordinances window and has a monthly cost of **§0**.

This ordinance controls the same setting as **Department active**.

- **Enacting the ordinance** enables the Department.
- **Repealing the ordinance** disables it.

Both methods update the same setting, so they remain consistent.

**You can enable or disable the Department from either location without changing the individual feature preferences you have already configured.**
<a id="options-files"></a>
### Where are the settings stored?

The Department stores its settings in **three text configuration files**, located inside the `Department of Efficiency` folder in your SimCity 4 Plugins directory.

You can change most settings through the Department's in-game options window, or edit the files manually.

| Configuration file | What it controls |
|---|---|
| `DepartmentEfficiency.ini` | Education and healthcare funding, safety margins and the main Department switch. |
| `DepartmentEfficiencyUtility.ini` | Electricity, water, sanitation, landfill advice, regional contracts and their related options. |
| `DepartmentEfficiencyDemolition.ini` | Automatic demolition, waiting periods, eligible zones, abandonment reasons and demolition spending. |

Each file has its own configuration section.

#### Education and healthcare

File: `DepartmentEfficiency.ini`

Section: `[DepartmentEfficiency]`

| Setting | INI key |
|---|---|
| Manage education | `Education` |
| Manage health | `Health` |
| Education safety margin | `EducationReservePercent` |
| Healthcare safety margin | `HealthReservePercent` |
| Department active | `Mode` |

The supplied configuration uses `ReservePercent=5` for both education and healthcare safety margins.

#### Utilities and regional contracts

File: `DepartmentEfficiencyUtility.ini`

Section: `[DepartmentEfficiencyUtility]`

| Setting | INI key |
|---|---|
| Manage power | `AutoPower` |
| Power reserve | `PowerReservePercent` |
| Consider power plant aging | `ConsiderPowerAging` |
| Manage water | `AutoWater` |
| Water reserve | `WaterReservePercent` |
| Protect water pipes | `ProtectWaterPipes` |
| Manage sanitation | `AutoGarbage` |
| Sanitation reserve | `GarbageReservePercent` |
| Landfill size advice | `WarnLandfillSizeSavings` |
| Landfill savings alert threshold | `LandfillSavingsAlertThreshold` |
| Manage neighbor deals | `AutoDeals` |
| Buy from one city to sell to another | `AllowUtilityArbitrage` |
| Manage Electricity Deals | `AutoTradePower` |
| Manage Water Deals | `AutoTradeWater` |
| Manage Garbage Deals | `AutoTradeGarbage` |
| Department active | `Mode` |

#### Automatic demolition

File: `DepartmentEfficiencyDemolition.ini`

Section: `[LotDemolition]`

| Setting | INI key |
|---|---|
| Demolish destroyed buildings | `DemolishDestroyed` |
| Wait for ruins | `DestroyedAfterDays` |
| Demolish destroyed municipal and historic buildings | `DemolishMunicipalAndHistoricRuins` |
| Demolish abandoned buildings | `DemolishAbandoned` |
| Wait for abandoned buildings | `AbandonedAfterDays` |
| Eligible zone types | `AbandonedZones` |
| Eligible abandonment reasons | `AbandonedReasons` |
| Limit the monthly demolition budget | `LimitMonthlyBudget` |
| Monthly demolition budget | `MonthlyBudget` |
| Department active | `Mode` |

#### Can I edit these files manually?

Yes, but **configuration values must follow the supported format and allowed ranges**.

The Department checks each file before using it.

An unknown setting or invalid value makes the affected configuration file invalid, disabling that part of the Department rather than applying uncertain settings.

Settings omitted from the supplied files use their defaults.

For example, the default demolition configuration enables the removal of ruins and eligible abandoned buildings, while protecting agricultural lots and buildings abandoned because of missing electricity or water.

**Manual edits are normally detected and applied within one simulated month.**

For ordinary changes, the in-game options window is safer because it checks the permitted values before saving them.

---

<a id="options-services"></a>
### Services page

The **Services** page contains automatic funding, safety reserves, regional contracts and landfill advice.

The first option controls the entire Department. The remaining options allow you to decide which services it should manage.

<a id="opt-master"></a>
### Department active

<text color="secondary">Default: On | Values: On / Off | INI: `Mode`</text>

**This is the main switch for the entire Department.**

When enabled, each automatic feature follows its own individual setting.

For example, you can leave the Department active while disabling water management or automatic demolition.

When disabled, the Department stops:

- Adjusting education, healthcare and utility funding.
- Creating, changing or cancelling regional contracts.
- Demolishing buildings and ruins.
- Repairing ghost landfill squares.
- Calculating landfill size recommendations and shortage notices.

**Disabling the Department does not undo changes it has already made.**

Funding levels and regional contracts remain as they were.

Your individual preferences are also preserved, so they become active again when you re-enable the Department.

Every other setting on the Services and Demolition pages becomes unavailable while the main switch is off.

**Important:** The included advisor resource files and the modification that hides agricultural *no water* icons remain effective while installed, even when the Department is disabled.

You can also control the same switch through **Establish a Department of Efficiency** in SimCity 4's Ordinances window.

The setting is stored as `Mode=Auto` or `Mode=Off` in each of the three configuration files.

Education and healthcare operate when at least one of their management options is enabled. Demolition operates when at least one demolition category is enabled. The utility controller remains available while the Department is active, even if none of the individual utilities is selected.

See [Safety, Automation & Decision-Making](#safety-player-control).

<a id="opt-education"></a>
### Manage education

<text color="secondary">Default: On | Values: On / Off | INI: `Education`</text>

Automatically adjusts funding for eligible education facilities, including schools, colleges, universities, libraries and museums.

**The Department chooses the lowest whole-number funding percentage that can serve the facility's users while maintaining the required safety margin.**

When enabled:

- Funding can increase during the first simulated day after opening a city.
- The Department can increase funding when it detects a capacity risk.
- Funding reductions are evaluated during the monthly review.
- Persistent overcrowding that funding cannot resolve may generate an advisor notice.

When disabled, education funding remains at the values you set manually, and education capacity notices are disabled.

**Important:** Buildings that do not support the game's standard funding mechanism are not modified. The original game's education funding advice remains suppressed while the civic advice resource file is installed, even if automatic education management is disabled.

**Requires:** Department active.

**Controls:** The education [Safety margin](#opt-educationreserve).

See [Education & Healthcare](#education-healthcare).

<a id="opt-educationreserve"></a>
### Safety margin (education)

<text color="secondary">Default: 5% | Range: 0–50%, steps of 1% | INI: `EducationReservePercent`</text>

Determines how much extra capacity the Department keeps above the current usage of each education building.

For example, a school serving 1,000 students with a 5% margin has a basic protected requirement of:

```math
1000\times1.05=1050
```

**At least 1,050 places.**

A higher margin provides more room for growth and unexpected increases in attendance, but may require more funding.

A lower margin reduces expenses by allowing capacity to remain closer to current usage.

**The safety margin is a minimum, not an exact target.**

The Department may keep additional capacity for small buildings or facilities whose usage fluctuates significantly, helping to avoid strikes.

**Requires:** Manage education and Department active.

See [Education & Healthcare](#education-healthcare-required).

<a id="opt-health"></a>
### Manage health

<text color="secondary">Default: On | Values: On / Off | INI: `Health`</text>

Automatically manages funding for eligible clinics and hospitals.

The Department selects the lowest whole-number funding percentage that provides sufficient capacity for the current patients and the configured healthcare safety margin.

When enabled:

- Funding increases can happen promptly when a capacity risk is detected.
- Reductions are considered during monthly reviews.
- Persistent capacity shortages that maximum funding cannot resolve may generate advisor notices.

When disabled, healthcare funding remains under your manual control and healthcare capacity notices are not generated.

**The Department never changes the separate ambulance budget line.**

Buildings that do not support the standard funding mechanism are excluded.

The original game's ordinary healthcare funding advice remains suppressed while the civic advice resource file is installed, regardless of this setting.

**Requires:** Department active.

**Controls:** The healthcare [Safety margin](#opt-healthreserve).

See [Education & Healthcare](#education-healthcare).

<a id="opt-healthreserve"></a>
### Safety margin (health)

<text color="secondary">Default: 5% | Range: 0–50%, steps of 1% | INI: `HealthReservePercent`</text>

Determines the extra patient capacity maintained by each managed healthcare facility.

For example, a hospital currently serving 2,000 patients with a 5% safety margin requires at least:

```math
2000\times1.05=2100
```

**2,100 places of capacity.**

Increasing the margin provides additional room for patient growth and fluctuations, but can increase funding expenses.

Reducing it allows lower funding when actual usage is stable.

**As with education, this is a minimum requirement.** The Department may maintain more capacity when its safety calculations require it.

**Requires:** Manage health and Department active.

See [Education & Healthcare](#education-healthcare-required).

<a id="opt-power"></a>
### Manage power

<text color="secondary">Default: On | Values: On / Off | INI: `AutoPower`</text>

Automatically adjusts funding for individual power plants.

Instead of funding every plant at the same percentage, the Department searches for **the cheapest combination of plant funding levels** that can supply:

- Your city's electricity requirements.
- The configured power reserve.
- Any active electricity sales to neighboring cities.

It also considers available regional electricity purchases.

When enabled, a full review normally takes place every **10 simulated days**, with earlier evaluations for confirmed supply deficits or significant changes in demand or capacity.

When disabled, power plant funding stays at its current values.

**Electricity contracts are controlled separately.** They can still be managed if Manage Electricity Deals is enabled.

The Department only changes funding. It never constructs additional power plants.

If the city's electricity needs cannot be met using the available facilities and contracts, a persistent shortage may produce an advisor notice.

**Requires:** Department active.

**Controls:** [Power reserve](#opt-powerreserve) and [Consider power plant aging](#opt-poweraging).

See [Electricity](#electricity).

<a id="opt-powerreserve"></a>
### Power reserve

<text color="secondary">Default: 20% | Range: 10–50%, steps of 1% | INI: `PowerReservePercent`</text>

Determines the extra electricity production capacity the Department protects against changes in demand.

Without purchases or sales, the protected local requirement is at least the greater of:

- The game's own electricity requirement.
- Electricity demand multiplied by **1 + reserve**.

For example, at 20% reserve, a city demanding 10,000 units normally needs at least 12,000 units of generating capacity, unless the game's own requirement is higher.

A higher reserve offers better protection against sudden increases in electricity demand but can increase operating expenses.

A lower reserve reduces costs, leaving less spare capacity before a shortage becomes possible.

**The reserve is a minimum, not a limit on production.**

The Department may maintain additional capacity when profitable electricity sales justify it.

**Requires:** Manage power and Department active.

See [Electricity](#electricity).

<a id="opt-poweraging"></a>
### Consider power plant aging

<text color="secondary">Default: Off | Values: On / Off | INI: `ConsiderPowerAging`</text>

Allows the Department to consider **power plant wear and aging** when comparing electricity funding arrangements.

When disabled, the optimizer focuses on monthly operating expenses.

When enabled, the calculation also considers the economic cost of plant wear.

This can sometimes make it worthwhile to spend slightly more each month to reduce wear and extend the useful life of a power plant.

**This estimated wear is an economic cost, not an additional monthly charge to your treasury.**

The city's electricity needs and configured reserve remain protected in both modes.

If Manage power is disabled, the aging preference is preserved but has no effect until power management is enabled again.

**Requires:** Manage power and Department active.

See [Electricity](#electricity).

<a id="opt-water"></a>
### Manage water

<text color="secondary">Default: On | Values: On / Off | INI: `AutoWater`</text>

Automatically manages the water department's funding.

Unlike electricity, water uses **one department funding slider** for its pumps and treatment facilities.

The Department chooses a funding level that provides sufficient water capacity for the city's demand and configured reserve, taking regional contracts into account.

When enabled, water funding is reviewed:

- Every 10 simulated days since the previous evaluation.
- At each simulated month change.
- Earlier when a regional shortage or significant change is detected.

When disabled, water funding remains unchanged.

Regional water contracts can still be managed independently through Manage Water Deals.

**The Department does not reduce water funding while buildings are without water under the water manager's applicable safety conditions.**

**Requires:** Department active.

**Controls:** [Water reserve](#opt-waterreserve) and [Protect water pipes](#opt-waterpipes).

See [Water](#water).

<a id="opt-waterreserve"></a>
### Water reserve

<text color="secondary">Default: 10% | Range: 10–50%, steps of 1% | INI: `WaterReservePercent`</text>

Determines how much additional water capacity the Department keeps above the city's demand.

For example, with a 10% reserve, a city demanding 50,000 water units requires at least:

```math
50000\times1.10=55000
```

**55,000 units of protected capacity**, before accounting for the detailed effects of contracts.

A higher reserve provides more protection against growing water demand but may increase expenses.

A lower reserve allows cheaper operation with less spare capacity.

**The reserve is a minimum requirement.** The Department can maintain more capacity when necessary.

**Requires:** Manage water and Department active.

See [Water](#water-required-capacity).

<a id="opt-waterpipes"></a>
### Protect water pipes

<text color="secondary">Default: On | Values: On / Off | INI: `ProtectWaterPipes`</text>

Protects the water distribution network against deterioration caused by insufficient funding.

In SimCity 4, maintaining the water department below certain funding levels can contribute to pipe damage.

When this option is enabled, the Department:

- Never reduces water funding below **50%**.
- Does not reduce funding while the pipe network is damaged.
- Avoids funding levels at which the game's pipe-maintenance curve permits network deterioration.

**The goal is to save money without allowing the water network to decay through underfunding.**

When disabled, the special pipe-protection restrictions are removed.

Water funding is still subject to the normal service and reserve requirements, but the Department may choose levels below 50% when otherwise permitted.

This can save money, **at the risk of pipes deteriorating or bursting**.

The option does not independently increase funding merely to repair existing pipe damage; its protection primarily prevents unsafe reductions.

If Manage water is disabled, this preference is stored but inactive.

**Requires:** Manage water and Department active.

See [Water](#water-pipe-curve).

<a id="opt-sanitation"></a>
### Manage sanitation

<text color="secondary">Default: On | Values: On / Off | INI: `AutoGarbage`</text>

Automatically adjusts the sanitation department's funding.

Sanitation covers the city's garbage disposal system, including landfills and treatment facilities, through one department funding slider.

The Department considers:

- Garbage produced after recycling.
- The configured sanitation reserve.
- Garbage already uncollected.
- Regional garbage imports and exports.
- Incinerator and Waste-to-Energy treatment capacity.
- Available landfill storage and monthly intake.

It then searches for the cheapest funding level that can handle the projected garbage.

**Unlike electricity and water, sanitation funding reductions are gradual.**

Reductions are limited to **25 percentage points per write**, with at least **7 simulated days** between cuts.

Funding increases require at least **3 simulated days** since the previous sanitation funding write.

The Department reviews sanitation at month changes and when garbage statistics change significantly.

When enabled, it can also perform safe automatic ghost-landfill-square repairs.

When disabled:

- Sanitation funding stays unchanged.
- Ghost-square repair stops.
- Landfill size advice is unavailable.
- Garbage contracts can still be managed independently if Manage Garbage Deals is enabled.

**The Department never automatically constructs or zones additional landfill areas.** Landfill expansion and reduction are advice only.

**Requires:** Department active.

**Controls:** [Sanitation reserve](#opt-sanitationreserve) and [Landfill size advice](#opt-landfilladvice).

See [Garbage & Landfills](#garbage-landfills).

<a id="opt-sanitationreserve"></a>
### Sanitation reserve

<text color="secondary">Default: 2% | Range: 2–20%, steps of 1% | INI: `GarbageReservePercent`</text>

Adds a safety margin to the amount of garbage your city produces after recycling.

The protected garbage amount is calculated from:

```math
\text{Protected garbage}=
(\text{Produced}-\text{Recycled})
\times(1+\text{reserve})
```

The actual planning calculation also includes any uncollected garbage backlog and the effects of regional contracts.

For example, a city producing 80,000 garbage units after recycling has a basic protected requirement of **81,600 units** with the default 2% reserve.

A higher margin provides extra protection against increases in garbage production but may require higher sanitation funding.

A lower margin reduces potential expenses.

**The reserve is a minimum, not a maximum disposal target.**

**Requires:** Manage sanitation and Department active.

See [Garbage & Landfills](#garbage-landfills-need).

<a id="opt-landfilladvice"></a>
### Warn me when checking landfill size can help me save

<text color="secondary">Default: Off | Values: On / Off | INI: `WarnLandfillSizeSavings`</text>

Enables optional recommendations about changing the size of your landfill.

The Department looks for situations where either:

- **Expanding a landfill** would provide enough additional disposal capacity to reduce the required sanitation funding and total monthly expense.
- **Reducing a landfill** would remove unused squares and lower maintenance expenses without compromising garbage disposal.

If a safe change produces savings above your configured threshold, the Department issues an advisor notice.

The notice identifies the proposed area, estimates the monthly saving and includes a link that moves the camera to the location.

**This feature only gives advice.**

It does not:

- Zone or dezone land.
- Construct or demolish anything.
- Change funding because of the advice.
- Change regional contracts because of the advice.

The analysis runs **at most once per simulated month**, subject to additional protection periods described in the landfill chapter.

It only works in cities containing **exactly one landfill zone**.

When disabled, no landfill-size analysis or notice is produced.

The existing timing history remains in place, so disabling and re-enabling the option does not reset its waiting periods.

**Requires:** Manage sanitation and Department active.

**Controls:** [Landfill savings alert threshold](#opt-landfillthreshold).

See [Garbage & Landfills](#garbage-landfills-advice).

<a id="opt-landfillthreshold"></a>
### Landfill savings alert threshold

<text color="secondary">Default: 25% | Range: 20–50%, steps of 1% | INI: `LandfillSavingsAlertThreshold`</text>

Determines how much a proposed landfill-size change must save before the Department recommends it.

The comparison uses the optimized monthly sanitation expense, including maintenance.

**Advice threshold**

```math
\text{Saving}\times100>
\text{Threshold}\times\text{Monthly expense}
```

**The comparison is strict.**

For example, with a current optimized sanitation expense of §1,000 and a 25% threshold:

- Saving §200: no notice.
- Saving §250: no notice.
- Saving §251: notice may be issued.

A higher threshold means fewer recommendations, limited to larger potential savings.

A lower threshold allows smaller savings opportunities to be reported.

**The notice displays the saving actually calculated, not the threshold you selected.**

Changing this setting does not reset the advice waiting periods.

When landfill size advice is disabled, the threshold control becomes unavailable, but its stored value is preserved.

**Requires:** Landfill size advice, Manage sanitation and Department active.

See [Garbage & Landfills](#garbage-landfills-advice).

<a id="opt-deals"></a>
### Manage neighbor deals

<text color="secondary">Default: On | Values: On / Off | INI: `AutoDeals`</text>

This is the **main switch for automatic regional contract management**.

When enabled, the Department can examine neighboring-city offers and manage electricity, water and garbage contracts.

Each resource is controlled by its own option.

The Department can:

- Purchase resources when cheaper than local production.
- Sell genuinely spare capacity when profitable.
- Adjust existing contract quantities.
- Cancel contracts that no longer provide a benefit.

**Your city's own services and safety reserves always take priority.**

When disabled:

- Existing regional contracts remain in place.
- Those contracts are still included in funding calculations.
- No new contract offers are requested.
- No contract is created, resized or cancelled.
- No local capacity is reserved for hypothetical future sales.
- Utility shortage notices cannot be fully proven because regional offers are not examined.

**Important:** SimCity 4's original neighbor-deal advisor messages remain suppressed while the Department's deal advice resource file is installed.

To restore those original messages after disabling automatic contract management, remove the relevant resource file with the game closed.

**Requires:** Department active.

**Controls:** [Buy from one city to sell to another](#opt-arbitrage), [Manage Electricity Deals](#opt-tradepower), [Manage Water Deals](#opt-tradewater) and [Manage Garbage Deals](#opt-tradegarbage).

See [Neighbor Deals](#neighbor-deals).

<a id="opt-arbitrage"></a>
### Buy from one city to sell to another

<text color="secondary">Default: Off | Values: On / Off | INI: `AllowUtilityArbitrage`</text>

Allows the Department to **buy and sell the same resource during one contract plan** when that combination produces a better economic result.

With this option disabled, each resource follows one of three arrangements:

- **LOCAL:** No regional buying or selling.
- **BUY:** Purchase the resource from neighboring cities.
- **SELL:** Sell the resource to neighboring cities.

With the option enabled, the Department may combine purchases and sales.

For example, it might buy electricity from one neighbor to cover part of the city's demand, freeing locally generated electricity for a profitable sale to another neighbor.

**The Department never directly resells imported resources.**

Purchases remain limited by the city's own demand, and the normal service and reserve rules still apply.

For garbage, a mixed arrangement might involve receiving paid garbage from one neighbor while paying another neighbor to dispose of some of your own waste.

A resource whose individual deal management option is disabled is never traded by the Department, regardless of this setting.

**Requires:** Manage neighbor deals and Department active.

See [Neighbor Deals](#neighbor-deals-arbitrage).

<a id="opt-tradepower"></a>
### Manage Electricity Deals

<text color="secondary">Default: On | Values: On / Off | INI: `AutoTradePower`</text>

Allows the Department to create, resize and cancel electricity contracts with neighboring cities.

When enabled, it can:

- Purchase electricity when economically worthwhile.
- Sell surplus electricity when profitable.
- Reduce or cancel sales that threaten the city's own supply.
- Adjust existing contracts as demand, production and prices change.

When disabled, existing electricity contracts remain under your manual control.

The Department still reads them when calculating local electricity requirements, but it does not modify them or reserve capacity for future automatic sales.

**This setting controls contracts, not power plant funding.**

It operates independently of **Manage power**.

For example, you can manage electricity contracts automatically while controlling power plant funding yourself. In that case, the Department must work with the generating capacity already available.

**Requires:** Manage neighbor deals and Department active.

See [Neighbor Deals](#neighbor-deals).

<a id="opt-tradewater"></a>
### Manage Water Deals

<text color="secondary">Default: On | Values: On / Off | INI: `AutoTradeWater`</text>

Allows the Department to manage water purchases and sales with neighboring cities.

When enabled, it compares contract prices with the cost of supplying water locally.

It may create, resize or cancel contracts when doing so improves the city's economic situation without compromising water service and the configured reserve.

When disabled, existing water contracts remain under your manual control.

**This option does not control water department funding.**

It is independent of **Manage water**.

For example, you can allow the Department to optimize water funding while keeping your own regional water contracts unchanged.

**Requires:** Manage neighbor deals and Department active.

See [Neighbor Deals](#neighbor-deals).

<a id="opt-tradegarbage"></a>
### Manage Garbage Deals

<text color="secondary">Default: On | Values: On / Off | INI: `AutoTradeGarbage`</text>

Allows the Department to manage garbage contracts in both directions.

Remember that garbage contracts work differently from electricity and water:

- **Sending garbage to a neighbor:** Your city pays for disposal.
- **Receiving garbage from a neighbor:** The neighbor pays your city to handle its waste.

When enabled, the Department can create, resize and cancel these contracts according to their economic value and the city's disposal capacity.

**Your city's own garbage disposal requirements always come first.**

The Department does not accept additional waste merely because another city offers money.

It must be able to handle that garbage safely using the available sanitation facilities and funding.

When disabled, existing garbage contracts remain under your manual control.

**This option is independent of Manage sanitation.**

You can manage garbage contracts without automatically changing sanitation funding, or manage sanitation funding while leaving contracts unchanged.

**Requires:** Manage neighbor deals and Department active.

See [Neighbor Deals](#neighbor-deals-garbage).

---

<a id="options-demolition"></a>
### Demolition page

The **Demolition** page controls how the Department clears destroyed buildings and long-abandoned lots.

You can decide:

- Whether ruins should be removed automatically.
- How long buildings must remain destroyed or abandoned.
- Which types of abandoned buildings may be demolished.
- Which abandonment reasons are eligible.
- Whether municipal and historic ruins can be removed.
- How much money may be spent during each monthly demolition review.

**Automatic demolition uses the game's normal bulldozing command and costs the same as manual demolition.**

It does not remove the underlying zoning.

<a id="opt-demolishdestroyed"></a>
### Demolish destroyed buildings

<text color="secondary">Default: On | Values: On / Off | INI: `DemolishDestroyed`</text>

Automatically removes ruins left behind by fires and disasters.

When enabled, the Department can clear destroyed buildings on residential, commercial, industrial and agricultural land after the configured waiting period.

**Ruins are processed before abandoned buildings during each monthly demolition review.**

When disabled, no automatic ruin demolition takes place.

The separate option for municipal and historic ruins also becomes ineffective.

Every demolition remains subject to the game's bulldozing price, available treasury and optional monthly budget.

**Requires:** Department active.

**Controls:** [Wait (ruins)](#opt-destroyedmonths) and [Demolish destroyed municipal and historic buildings](#opt-demolishmunicipal).

See [Automatic Demolition](#demolition-destroyed).

<a id="opt-destroyedmonths"></a>
### Wait (ruins)

<text color="secondary">Default: 1 month (30 days) | Range: 1–24 months, steps of 1 | INI: `DestroyedAfterDays`</text>

Determines how long a destroyed building must remain in ruins before the Department may clear it.

The options window displays simulated months, but the Department stores the waiting period in simulated days.

The conversion uses:

```math
\text{Days}=
\operatorname{round}\left(\frac{\text{Months}\times365}{12}\right)
```

Examples:

| Selected delay | Stored days |
|---|---:|
| 1 month | 30 |
| 3 months | 91 |
| 6 months | 183 |
| 12 months | 365 |
| 24 months | 730 |

A longer waiting period gives you more time to inspect ruins and decide whether to act manually.

A shorter period allows zoned land to become available for redevelopment sooner.

**The Department checks demolition eligibility once per simulated month.**

A ruin is therefore cleared at the first monthly review after it becomes old enough, not necessarily on the exact day the waiting period expires.

The same delay applies to municipal and historic ruins if you enable their separate demolition option.

**Advanced INI range:** 1–3,650 days.

**Requires:** Demolish destroyed buildings and Department active.

See [Automatic Demolition](#demolition-delays).

<a id="opt-demolishmunicipal"></a>
### Demolish destroyed municipal and historic buildings

<text color="secondary">Default: Off | Values: On / Off | INI: `DemolishMunicipalAndHistoricRuins`</text>

Controls whether the Department may automatically clear the ruins of protected buildings.

These include municipal and special-purpose buildings, such as:

- Schools and hospitals.
- Police and fire stations.
- Landmarks and rewards.
- Other plopped buildings.
- Lots in special zones.
- Historically designated buildings.

**When disabled, these ruins remain for you to handle manually.**

When enabled, their ruins follow the same waiting period as ordinary destroyed buildings.

**This option never authorizes the demolition of standing or abandoned municipal or historic buildings.**

It applies only after the game has already marked the lot as Destroyed.

For example, if a landmark is destroyed by a disaster, enabling this option allows the Department to remove its ruins after the waiting period.

You may prefer to leave it disabled if you want to inspect destroyed landmarks or other special buildings before their remains disappear.

**Requires:** Demolish destroyed buildings and Department active.

See [Automatic Demolition](#demolition-protected).

<a id="opt-demolishabandoned"></a>
### Demolish abandoned buildings

<text color="secondary">Default: On | Values: On / Off | INI: `DemolishAbandoned`</text>

Automatically clears buildings that have remained abandoned for the configured waiting period.

The Department only considers abandoned buildings that match your selected:

- Zone types.
- Abandonment reasons.
- Waiting period.

Eligible abandoned buildings are processed during monthly reviews, **after ruins**, with the oldest abandoned lots first.

Two abandonment reasons are always considered eligible when this feature is enabled:

- Occupants evicted.
- No reason or an unrecognized reason.

All other safety conditions still apply.

**Abandoned municipal and historically protected buildings are never demolished automatically.**

When disabled, no abandoned buildings are cleared.

**Important:** Buildings that were abandoned before installing the Department keep their original age. Some may already qualify during the first monthly demolition review.

**Requires:** Department active.

**Controls:** [Wait (abandoned)](#opt-abandonedmonths), the four zone options and the five abandonment-reason options.

See [Automatic Demolition](#demolition-abandoned).

<a id="opt-abandonedmonths"></a>
### Wait (abandoned)

<text color="secondary">Default: 12 months (365 days) | Range: 1–60 months, steps of 1 | INI: `AbandonedAfterDays`</text>

Determines how long a building must remain abandoned before it becomes eligible for demolition.

For example, with the default 12-month delay, a building must have been abandoned for at least **365 simulated days**.

A longer waiting period gives buildings more time to become occupied again.

A shorter period allows abandoned land to be cleared more quickly.

**The Department uses SimCity 4's own abandonment-age counter**, which is stored in the city save.

The waiting period therefore does not restart when you install the plugin or reopen the city.

The options window shows months, while the configuration file stores days using the same 365-day conversion described above.

**Advanced INI range:** 1–7,300 days.

**Requires:** Demolish abandoned buildings and Department active.

See [Automatic Demolition](#demolition-delays).

<a id="opt-zoneresidential"></a>
### Residential zone

<text color="secondary">Default: On | Values: On / Off | INI: `AbandonedZones` (`Residential`)</text>

Includes abandoned residential buildings in automatic demolition.

This covers low-, medium- and high-density residential development, including houses and apartment buildings.

When enabled, eligible abandoned housing can be cleared after the waiting period.

When disabled, abandoned residential buildings are preserved.

**This setting only affects abandoned buildings.**

Residential ruins can still be cleared through **Demolish destroyed buildings**.

**Requires:** Demolish abandoned buildings and Department active.

See [Automatic Demolition](#demolition-zones).

<a id="opt-zonecommercial"></a>
### Commercial zone

<text color="secondary">Default: On | Values: On / Off | INI: `AbandonedZones` (`Commercial`)</text>

Includes abandoned commercial buildings in automatic demolition.

This covers low-, medium- and high-density commercial development, including shops and offices.

When enabled, eligible abandoned commercial buildings can be cleared, allowing the zoned land to develop again.

When disabled, abandoned commercial buildings remain untouched.

**Commercial ruins are controlled separately** through the destroyed-buildings setting.

**Requires:** Demolish abandoned buildings and Department active.

See [Automatic Demolition](#demolition-zones).

<a id="opt-zoneindustrial"></a>
### Industrial zone

<text color="secondary">Default: On | Values: On / Off | INI: `AbandonedZones` (`Industrial`)</text>

Includes abandoned industrial buildings in automatic demolition.

This covers medium- and high-density industrial development, such as factories and warehouses.

When enabled, eligible abandoned industrial buildings may be demolished after the waiting period.

When disabled, they are preserved.

**Agricultural lots are not included in this setting.** They have their own separate option.

Industrial ruins remain subject to the destroyed-buildings setting, regardless of this choice.

**Requires:** Demolish abandoned buildings and Department active.

See [Automatic Demolition](#demolition-zones).

<a id="opt-zoneagriculture"></a>
### Agricultural zone

<text color="secondary">Default: Off | Values: On / Off | INI: `AbandonedZones` (`Agriculture`)</text>

Controls automatic demolition of abandoned farms and agricultural fields.

**This setting is disabled by default for an important reason.**

In SimCity 4, farm fields can appear abandoned as part of their normal operation.

That does not necessarily mean the farm is no longer functioning.

When disabled, **all abandoned agricultural lots are preserved**.

When enabled, the Department may demolish eligible abandoned farms and fields after the waiting period.

**Enabling this option can remove farm fields that you wanted to keep.**

Use it carefully, especially in agricultural cities.

Ruins on farmland are a separate case. They can still be cleared through **Demolish destroyed buildings**, even when Agricultural zone is disabled.

**Requires:** Demolish abandoned buildings and Department active.

See [Automatic Demolition](#demolition-farms).

<a id="opt-reasonlowdesirability"></a>
### Reason: unattractive area

<text color="secondary">Default: On | Values: On / Off | INI: `AbandonedReasons` (`LowDesirability`)</text>

Includes buildings abandoned because their location has become unattractive.

Possible causes include:

- Pollution.
- Crime.
- Inadequate services.
- Other factors reducing local desirability.

When enabled, such buildings may be demolished after the configured waiting period.

When disabled, they are preserved.

**Demolishing an abandoned building does not fix the underlying desirability problem.**

For example, if pollution remains excessive, the replacement building may encounter similar difficulties.

Improving the surrounding area may sometimes allow the original building to become occupied again.

**Requires:** Demolish abandoned buildings and Department active.

See [Automatic Demolition](#demolition-reasons).

<a id="opt-reasoncommute"></a>
### Reason: commute too long

<text color="secondary">Default: On | Values: On / Off | INI: `AbandonedReasons` (`CommuteTime`)</text>

Includes buildings abandoned because their occupants could not reach work within an acceptable commuting time.

When enabled, eligible buildings may be demolished after the waiting period.

When disabled, they are preserved.

**Demolition does not directly resolve the commuting problem.**

For example, improving public transport or road access might allow an abandoned residential building to become occupied again.

You may prefer a longer waiting period if you regularly make improvements to your city's transport network.

**Requires:** Demolish abandoned buildings and Department active.

See [Automatic Demolition](#demolition-reasons).

<a id="opt-reasonlowdemand"></a>
### Reason: no demand

<text color="secondary">Default: On | Values: On / Off | INI: `AbandonedReasons` (`LowDemand`)</text>

Includes buildings abandoned because the city no longer has sufficient demand for that type of development.

When enabled, the Department may clear those buildings after the waiting period, leaving their zoning available for future growth.

When disabled, they remain untouched.

**Demand can change as the city develops.**

A building abandoned because of insufficient demand today may become useful again later.

The waiting period determines how much time you allow for that possibility before the Department clears the lot.

**Requires:** Demolish abandoned buildings and Department active.

See [Automatic Demolition](#demolition-reasons).

<a id="opt-reasonnopower"></a>
### Reason: no power

<text color="secondary">Default: Off | Values: On / Off | INI: `AbandonedReasons` (`NoPower`)</text>

Controls demolition of buildings abandoned because they lack electricity.

**This option is disabled by default.**

A building abandoned because of a power failure may become occupied again after electricity is restored.

When disabled, the Department preserves those buildings.

When enabled, it may demolish them after the configured waiting period.

**Check the electricity network before enabling this option.**

An abandoned building may only need a repaired connection or restored supply, rather than demolition.

**Requires:** Demolish abandoned buildings and Department active.

See [Automatic Demolition](#demolition-reasons).

<a id="opt-reasonnowater"></a>
### Reason: no water

<text color="secondary">Default: Off | Values: On / Off | INI: `AbandonedReasons` (`NoWater`)</text>

Controls demolition of buildings abandoned because they lack water.

Like the electricity equivalent, **this option is disabled by default**.

Buildings abandoned because of missing water may become occupied again when the water supply or pipe connection is restored.

When disabled, those buildings are preserved.

When enabled, the Department may clear them after the waiting period.

**Check the water network before enabling this option.**

Demolition may be unnecessary if restoring the service would allow the building to recover.

**Requires:** Demolish abandoned buildings and Department active.

See [Automatic Demolition](#demolition-reasons).

<a id="opt-budgetlimit"></a>
### Limit the monthly demolition budget

<text color="secondary">Default: Off | Values: On / Off | INI: `LimitMonthlyBudget`</text>

Adds an optional spending limit to automatic demolition.

The limit applies to **ruins and abandoned buildings together** during each monthly demolition review.

When enabled, the Department keeps track of the bulldozing costs of orders already submitted during that review.

A demolition is permitted only when its cost would not exceed the configured monthly budget.

**If the next priced building would exceed the limit, the entire monthly demolition review stops.**

The remaining buildings wait until the next month.

When disabled, there is no additional monthly spending cap.

However, **the treasury protection always remains active**.

The Department never orders more demolition expenses than the city can afford from the treasury available at the beginning of the review.

Demolitions with a price of §0 do not consume the optional monthly budget.

**Requires:** Department active.

**Controls:** [Monthly budget](#opt-demolitionbudget).

See [Automatic Demolition](#demolition-money).

<a id="opt-demolitionbudget"></a>
### Monthly budget

<text color="secondary">Default: §1,000 | In-game range: §0–§200,000, steps of §1,000 | INI: `MonthlyBudget`</text>

Sets the maximum amount the Department may spend on automatic demolition during one monthly review, when the budget limit is enabled.

For example, with a budget of **§1,000**, the Department may order eligible demolitions as long as their combined cost remains within that amount.

If a proposed demolition would exceed the limit, the review stops and the remaining candidates wait until the following month.

**What happens at different budget levels?**

| Monthly budget | Effect |
|---|---|
| **§0** | Only demolitions costing §0 are allowed. |
| **§1,000** | Up to §1,000 can be committed per review. |
| **§5,000** | Up to §5,000 can be committed per review. |
| **§20,000** | Up to §20,000 can be committed per review. |

A larger budget allows more demolition expenses in a single month.

A smaller budget spreads a large backlog of eligible buildings over more months.

**The monthly budget never overrides the treasury rule.**

For example, setting a §20,000 demolition budget does not allow the Department to spend §20,000 if the city has only §5,000 available.

#### What if the budget limit is disabled?

The Monthly budget control becomes unavailable, but its saved value is preserved.

If you enable the limit later, the previous budget becomes active again.

For example, the supplied configuration stores **§1,000** even though the limit is disabled by default.

Enabling it does not reset that amount.

#### Can I set a higher budget manually?

Yes.

The in-game options window allows a maximum of **§200,000**.

The configuration file accepts values up to **2,000,000,000**, although larger values cannot be represented fully in the options window.

**Requires:** Limit the monthly demolition budget and Department active.

See [Automatic Demolition](#demolition-money).

---

[Back to the contents](#contents) | [Back to the README](../README.md)