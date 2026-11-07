**English** | [Português](../pt/safety.md) | [Français](../fr/safety.md) | [Español](../es/safety.md)

[Back to the contents](index.md#contents) | [Back to the README](../../README.md)

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
- Sell power or water that the city actually needs.
- Make decisions using incomplete statistics immediately after loading.
- Apply another change before the previous one has taken effect.
- Repeat a contract or funding operation that the game has already rejected.
- Demolish buildings that should be protected.
- Flood the player with notifications about routine actions.

**The Department is designed to avoid these problems first and optimize expenses second.**

<a id="safety-principles"></a>
### 3. The Department's general rules

When several objectives conflict, the Department follows this order of priorities:

1. **Maintain essential services.** Avoid leaving the city without power, water, sanitation, education or healthcare.
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

For power, water and garbage, the Department uses six execution phases:

| Phase | Action |
|---|---|
| **1** | Increase local capacity |
| **2** | Reduce or cancel sales |
| **3** | Create or increase purchases |
| **4** | Create or increase sales |
| **5** | Reduce local capacity |
| **6** | Reduce or cancel purchases |

**Each resource executes only its earliest pending phase during a review.**

For example, the Department does not reduce power plant funding before the power purchase intended to replace that production is active.

If a capacity increase has not yet taken effect, subsequent phases for that resource wait.

There is one important distinction for sanitation: a funding reduction may be postponed because sanitation writes are deliberately gradual.

**An unfinished sanitation reduction does not necessarily prevent a later reduction in garbage-disposal purchases**, because keeping higher local disposal capacity cannot harm service continuity.

#### Funding is not reduced during a genuine shortage

The Department distinguishes between insufficient supply and problems within the city's distribution network.

For water, for example, it refuses a reduction when buildings lack water **and production capacity is below demand**.

If sufficient water is available but some buildings are disconnected from the pipe network, a funding increase would not solve the problem. Such a connection issue does not automatically prevent an otherwise safe reduction.

See [Water safety conditions](water.md#water-no-cut).

#### Sales stop during confirmed power or water deficits

If the game's counters confirm that the city does not have enough power or water, **the Department does not maintain sales of that resource**.

Local demand takes priority over export income.

#### Education and healthcare increase funding promptly

Education and healthcare use a different timing strategy.

The Department can raise funding:

- During the first simulated day after opening the city.
- When a service-capacity risk is detected.
- During additional checks while a strike or persistent risk is present.

**Funding reductions wait for the monthly evaluation.**

This allows the Department to respond quickly to overcrowded schools or hospitals while avoiding unnecessary reductions between monthly reviews.

See [Education & Healthcare](education_healthcare.md#education-healthcare-timing).

<a id="safety-reserves"></a>
#### Safety reserves: minimum capacity, not maximum capacity

Each managed service has a configurable reserve.

**A reserve is a minimum requirement, never a maximum.**

The Department may maintain more capacity when necessary because of the available funding settings, service fluctuations or profitable regional sales.

It must not deliberately choose a plan that falls below the applicable protected requirement.

| Service | Default reserve | Allowed range | How it works |
|---|---:|---:|---|
| **Education and healthcare** | 5% | 0–50% | Adds spare capacity above usage, with additional protection for small or fluctuating facilities. |
| **Power** | 20% | 10–50% | Protects the portion of demand supplied by local power plants, while also respecting the game's power requirement. |
| **Water** | 10% | 10–50% | Protects the portion of demand supplied by local water facilities. |
| **Sanitation** | 2% | 2–20% | Adds a margin to garbage produced after recycling, alongside any existing uncollected garbage. |

See [Education & Healthcare](education_healthcare.md#education-healthcare-required), [Power](power.md#power), [Water](water.md#water-required-capacity) and [Garbage & Landfills](garbage.md#garbage-landfills-need).

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
| **Power** | Find the cheapest combination of individual power plant funding levels. |
| **Water** | Choose the lowest safe department funding percentage. |
| **Sanitation** | Simulate garbage disposal through the remaining days of the month and find the cheapest funding that can handle the waste. |
| **Regional contracts** | Compare contract income and expenses with the actual cost of local production or disposal. |

#### How quickly are funding changes applied?

**Power and water**

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

See [Neighbor Deals](deals.md#neighbor-deals-objective).

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

The one zoning-related exception is the narrowly controlled [ghost-tile repair](garbage.md#ghost-tiles), which uses the game's own zoning system to correct erroneous landfill membership without deliberately changing legitimate landfill zoning.

<a id="safety-reevaluation"></a>
#### When does the Department recalculate its decisions?

The Department combines regular reviews with lighter checks for significant changes.

**It does not wait for an arbitrary fixed delay after opening a city.**

Each service begins evaluating as soon as the information it needs is available, current and consistent.

Demolition is an exception: it starts at the first simulated month change after loading.

#### Checking whether game statistics are reliable

Before making decisions about power or water, the Department compares reported demand with actual consumption.

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

Every simulated day, the Department reads a small number of power and water counters.

These checks are deliberately lightweight.

They do not require scanning every building or rebuilding the entire regional contract market.

**An identical reading is not treated as a new observation.**

The Department only triggers an early evaluation when something relevant has actually changed.

#### Detecting a genuine regional supply shortage

A city may report insufficient consumption for two very different reasons:

1. It does not have enough power or water available.
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

The problem is likely inside the city, such as disconnected power lines or water pipes.

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

Suppose power demand was 50,000 units during the previous review.

| New demand | Change | Result |
|---|---:|---|
| 50,400 | 0.8% | Wait for the normal review |
| **50,600** | **1.2%** | **Trigger an early evaluation** |

A significant new reading can bring the next evaluation forward without waiting for the regular interval.

**There is no additional cooldown for a decision that is ready and safe to apply.**

#### Regular evaluation intervals

| Service | When it is evaluated |
|---|---|
| **Power** | Every 10 simulated days, plus early reviews for confirmed shortages or significant changes. |
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

For example, it cannot conclude that an power shortage is impossible to resolve if it has not been able to inspect the available regional purchases.

**An incomplete contract market cannot support a definitive shortage warning.**

#### Unknown landfill geometry

The Department can calculate sanitation funding without knowing every detail of the landfill's internal cell structure.

Until that structure has been calibrated, it uses the conservative calculations described in [Garbage & Landfills](garbage.md#garbage-landfills-algorithm).

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
| **Manage regional contracts manually** | Disable **Manage Power Deals**, **Manage Water Deals** or **Manage Garbage Deals** for the relevant resource. |
| **Change a funding slider yourself** | Move it normally. The Department will use your new value at its next evaluation if management remains enabled. |
| **Create or cancel a contract yourself** | Use the normal Neighbor Deals window. The Department will consider the resulting contracts during its next review. |
| **Stop automatic demolition** | Disable both **Demolish destroyed buildings** and **Demolish abandoned buildings**, or disable the entire Department. |
| **Remove the plugin** | Close SimCity 4 and remove the plugin's files. Your city saves remain usable without it. |

#### What happens when you disable the Department?

**Disabling the Department does not undo earlier changes.**

For example, if it previously reduced power funding to 72%, switching it off does not restore the previous funding.

Likewise, regional contracts remain in their current states.

The Department simply stops making further automatic changes.

#### What happens when you move a slider manually?

You can always move a funding slider.

However, **a manual value is not locked while its manager is enabled**.

The Department treats your new value as the starting point for its next calculation.

Depending on the service:

- **Power:** The funding may be recalculated within days, no later than the regular 10-day review.
- **Water:** The funding may be recalculated within 10 simulated days.
- **Education and healthcare:** A manual reduction that creates a capacity risk can be corrected within days. A manual increase is reviewed during the monthly pass.

For example, if you increase water funding to 100% but leave **Manage water** enabled, the Department may lower it again when its calculations show that less funding is sufficient.

**To keep a specific funding value under your control, disable that service's manager.**

#### Are there any effects that cannot be switched off individually?

Yes.

On the supported game version, the plugin hides the original *no water* warning icon over agricultural lots whenever the plugin is loaded.

This does not change the water service itself.

See [Water](water.md#water-farm-icon).

The Department also includes three resource files that modify some of SimCity 4's original advisor messages.

Those files operate whenever they are installed, regardless of the corresponding management options. Their behavior is explained under [Advisor Notices](#safety-notices-game).

<a id="safety-automation"></a>
#### Which features act automatically, and which only give advice?

Some Department features modify the city directly.

Others only display information so that you can decide what to do.

| Feature | Automatic action | Advisor notice |
|---|---|---|
| **Education and healthcare** | Adjusts funding | Warns when additional funding cannot resolve a persistent capacity problem |
| **Power** | Adjusts power plant funding | Warns about a persistent shortage that cannot be resolved automatically |
| **Water** | Adjusts department funding | Warns about a persistent shortage that cannot be resolved automatically |
| **Sanitation** | Adjusts department funding | Warns about a persistent garbage disposal shortage |
| **Neighbor contracts** | Creates, changes and cancels contracts | No separate routine contract notice |
| **Automatic demolition** | Removes eligible ruins and abandoned buildings | No notice |
| **Ghost landfill squares** | Repairs eligible ghost tiles automatically | No notice |
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

For example, an unchanged power demand reading does not trigger another complete power plant optimization.

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
#### Power, water and garbage shortage notices

The Department can warn you when a shortage persists and **the available automatic solutions have been exhausted**.

These notices come from the **Utilities advisor**.

| Resource | Notice title |
|---|---|
| **Power** | **By Candlelight!** |
| **Water** | **The Faucets Are on Strike!** |
| **Garbage** | **Garbage Takes Over!** |

These are urgent advisor messages.

Depending on your SimCity 4 advice settings, they can appear as pop-ups as well as entries in the news list.

They do not include a camera link.

#### When does the Department consider a shortage proven?

A shortage notice is not generated simply because demand temporarily exceeds supply.

The Department must first establish that **the shortage cannot be solved using the currently available legal options**.

For power and water, this requires checking:

- The maximum usable capacity of local facilities.
- Available regional purchases.
- The removal of sales that consume needed capacity.
- The city's actual demand and delivered service.
- The completeness of regional records and contract offers.

If you manage contracts manually, the Department evaluates the existing contracts rather than assuming it can create new ones.

The regional information must be complete enough to support the conclusion.

For power and water, the game's counters must also confirm a genuine regional deficit.

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
| **Power** | After 30 unresolved simulated days | Every 60 simulated days |
| **Water** | After 30 unresolved simulated days | Every 60 simulated days |
| **Garbage** | After 30 unresolved simulated days | Every 120 simulated days |

The Department continues reminders only while the shortage remains unresolved.

#### When does a shortage episode end?

For power and water, the episode ends when a fresh game reading confirms that delivered service is at least equal to demand.

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

See [Education & Healthcare](education_healthcare.md#education-healthcare-notices) for the complete calculation.

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

See [Garbage & Landfills](garbage.md#garbage-landfills-advice).

**Ghost-tile repairs never generate advisor notices.**

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

[Department active](options.md#opt-master) is the main switch.

You can also control it through the ordinance **Establish a Department of Efficiency** in SimCity 4's Ordinances window.

Both controls represent the same setting.

**Individual management options**

Each service has its own manager and, where appropriate, a reserve or safety margin.

See the [Option Reference](options.md#options) for all settings.

**Advisor notices**

The Department has only two notice-specific options, both related to landfill size recommendations:

- [Warn me when checking landfill size can help me save](options.md#opt-landfilladvice).
- [Landfill savings alert threshold](options.md#opt-landfillthreshold).

**Utility shortage notices cannot be disabled separately.**

They follow the relevant service-management settings and appear only when their conditions are satisfied.

<a id="safety-examples"></a>
### 6. Worked examples

The following examples show how the Department responds to persistent shortages, incomplete information and manual changes.

#### Example 1: An power shortage that cannot be resolved

Suppose your city has the following situation on **simulated day 40** after opening:

- **Power demand:** 10,000 units.
- **Power delivered:** 9,200 units.
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

The game's counters also confirm that power delivery is below demand.

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

A fresh reading confirms that power delivered now meets demand.

The Department closes the shortage episode and stops the reminders.

**Day 200**

A new power shortage is proven.

Because the previous episode ended, the new shortage starts its own 30-day waiting period.

If it remains unresolved, the next notice appears on **day 230**.

#### Example 2: Buildings without power, but enough generating capacity

Consider a city with:

- **Power demand:** 10,000 units.
- **Power delivered:** 9,600 units.
- **Local generating capacity:** 12,000 units.
- **No imports or exports.**

Although some demand is not being served, the city has enough generating capacity.

```math
12000\ge10000
```

This means the supply shortage test is not satisfied.

The problem is likely inside the power distribution network.

**The Department does not increase funding or issue a regional shortage notice.**

More production would not necessarily reach disconnected buildings.

The player must inspect the power network.

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

**A temporary water-data problem does not prevent power, sanitation or other independent managers from operating.**

<a id="safety-limitations"></a>
### 7. Known limitations

The Department is designed to operate conservatively, but it cannot overcome every limitation of SimCity 4.

#### Manual control

- **Manual funding changes are not locked.** When a service manager is enabled, it may revise a funding percentage you changed yourself during its next evaluation.
- **There are no individual building locks.** To keep a particular service under manual funding control, disable the corresponding manager.

#### Shortage notices

- **Utility shortage notices require Manage neighbor deals.** Without access to regional offers and markets, the Department cannot prove that all possible solutions have been exhausted.
- **The Department cannot know whether a notice was actually displayed on screen.** SimCity 4 decides whether to show a pop-up according to its advisor settings.
- **Garbage shortage notices rely on a model.** Unlike power and water, garbage feasibility is determined through the Department's disposal projection rather than a direct game counter proving future disposal requirements.

#### Original game advisor messages

- **The three advice resource files act whenever installed.** Disabling the corresponding Department options does not immediately undo their effects.
- **Restoring original messages may require removing the relevant resource file and restarting SimCity 4.** The Department logs configuration mismatches.

#### Technical safety

- **A technical safety stop lasts for the current city session.** The affected manager remains disabled until the city is reloaded.
- **Only the supported game version is managed.** The Department supports **Steam SimCity 4 Deluxe 1.1.641.0 on Windows**. It performs no operations on unsupported versions.

**The Department is designed to optimize the city only when it has enough reliable information to act safely. When it cannot verify an operation or prove that a change is beneficial, it prefers leaving the existing situation unchanged.**

---

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
