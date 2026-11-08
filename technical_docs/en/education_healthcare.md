**English** | [Português](../pt/education_healthcare.md) | [Français](../fr/education_healthcare.md) | [Español](../es/education_healthcare.md)

[Back to the contents](index.md#contents) | [Back to the README](../../README.md)

<a id="education-healthcare"></a>
## A. Education & Healthcare

The Department automatically adjusts the funding of each school, college, university, library, museum, clinic and hospital it can manage. Its goal is simple: **provide enough capacity for everyone who needs the service, with a safety margin, while spending as little as possible.**

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

For the notification system shared with other services, see [Advisor Notices](safety.md#notices).

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

See [General Safety](safety.md#safety) for more information.

<a id="education-healthcare-settings"></a>
### 5. Settings

Education and healthcare management can be configured directly in **Play Options → Department of Efficiency**, under the **Services** page.

| Setting | Default | Range | What it does |
|---|---|---|---|
| [Department active](options.md#opt-master) | On | On / Off | Enables or disables the entire Department. |
| [Manage education](options.md#opt-education) | On | On / Off | Automatically manages the funding of eligible schools, colleges, universities, libraries and museums. |
| [Safety margin (education)](options.md#opt-educationreserve) | 5% | 0–50% | Sets the minimum spare capacity the Department aims to maintain in education buildings. |
| [Manage health](options.md#opt-health) | On | On / Off | Automatically manages the funding of eligible clinics, hospitals and disease research centers. |
| [Safety margin (health)](options.md#opt-healthreserve) | 5% | 0–50% | Sets the minimum spare capacity the Department aims to maintain in healthcare buildings. |

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

[Back to the contents](index.md#contents) | [Back to the README](../../README.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
