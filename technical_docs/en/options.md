**English** | [Português](../pt/options.md) | [Français](../fr/options.md) | [Español](../es/options.md)

[Back to the contents](index.md#contents) | [Back to the README](../../README.md)

<a id="options"></a>
## H. Option Reference

This chapter explains how to open the Department's settings and how configuration changes are saved. The detailed descriptions of individual options are provided in the following sections.

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

The Department stores its settings in **one text configuration file**, `DepartmentEfficiency.ini`, placed directly in your SimCity 4 Plugins folder, next to `DepartmentEfficiency.dll` (there is no `Department of Efficiency` subfolder). It has three independent sections.

You can change most settings through the Department's in-game options window, or edit the files manually.

| Configuration file | What it controls |
|---|---|
| `DepartmentEfficiency.ini`, section `[AutoFinance]` | Education and healthcare funding, safety margins and the main Department switch. |
| `DepartmentEfficiency.ini`, section `[UtilityResearch]` | Power, water, sanitation, landfill advice, regional contracts and their related options. |
| `DepartmentEfficiency.ini`, section `[LotDemolition]` | Automatic demolition, waiting periods, eligible zones, abandonment reasons and demolition spending. |

Since version 1.0.2 there is one file with three independent sections. A missing or invalid section affects only its own module; the other two keep working.

#### Education and healthcare

File: `DepartmentEfficiency.ini`

Section: `[AutoFinance]`

| Setting | INI key |
|---|---|
| Manage education | `Education` |
| Manage health | `Health` |
| Education safety margin | `EducationReservePercent` |
| Healthcare safety margin | `HealthReservePercent` |
| Department active | `Mode` |

The supplied configuration uses `ReservePercent=5` for both education and healthcare safety margins.

#### Utilities and regional contracts

File: `DepartmentEfficiency.ini`

Section: `[UtilityResearch]`

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
| Manage Power Deals | `AutoTradePower` |
| Manage Water Deals | `AutoTradeWater` |
| Manage Garbage Deals | `AutoTradeGarbage` |
| Department active | `Mode` |

#### Automatic demolition

File: `DepartmentEfficiency.ini`

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

For example, once demolition is turned on, its default rules enable the removal of ruins and eligible abandoned buildings, while protecting agricultural lots and buildings abandoned because of missing power or water. Demolition itself is off by default.

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

**Important:** The modification that hides agricultural *no water* icons remains effective while installed, even when the Department is disabled. The game's own advisor messages are no longer silenced when the Department is disabled.

You can also control the same switch through **Establish a Department of Efficiency** in SimCity 4's Ordinances window.

The setting is stored as `Mode=Auto` or `Mode=Off` in each of the three configuration files.

Education and healthcare operate when at least one of their management options is enabled. Demolition operates when at least one demolition category is enabled. The utility controller remains available while the Department is active, even if none of the individual utilities is selected.

See [Safety, Automation & Decision-Making](safety.md#safety-player-control).

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

**Important:** Buildings that do not support the game's standard funding mechanism are not modified. The original game's education funding advice is silenced only while the Department manages education.

**Requires:** Department active.

**Controls:** The education [Safety margin](#opt-educationreserve).

See [Education & Healthcare](education_healthcare.md#education-healthcare).

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

See [Education & Healthcare](education_healthcare.md#education-healthcare-required).

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

The original game's ordinary healthcare funding advice is silenced only while the Department manages healthcare.

**Requires:** Department active.

**Controls:** The healthcare [Safety margin](#opt-healthreserve).

See [Education & Healthcare](education_healthcare.md#education-healthcare).

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

See [Education & Healthcare](education_healthcare.md#education-healthcare-required).

<a id="opt-power"></a>
### Manage power

<text color="secondary">Default: On | Values: On / Off | INI: `AutoPower`</text>

Automatically adjusts funding for individual power plants.

Instead of funding every plant at the same percentage, the Department searches for **the cheapest combination of plant funding levels** that can supply:

- Your city's power requirements.
- The configured power reserve.
- Any active power sales to neighboring cities.

It also considers available regional power purchases.

When enabled, a full review normally takes place every **10 simulated days**, with earlier evaluations for confirmed supply deficits or significant changes in demand or capacity.

When disabled, power plant funding stays at its current values.

**Power contracts are controlled separately.** They can still be managed if Manage Power Deals is enabled.

The Department only changes funding. It never constructs additional power plants.

If the city's power needs cannot be met using the available facilities and contracts, a persistent shortage may produce an advisor notice.

**Requires:** Department active.

**Controls:** [Power reserve](#opt-powerreserve) and [Consider power plant aging](#opt-poweraging).

See [Power](power.md#power).

<a id="opt-powerreserve"></a>
### Power reserve

<text color="secondary">Default: 20% | Range: 10–50%, steps of 1% | INI: `PowerReservePercent`</text>

Determines the extra power production capacity the Department protects against changes in demand.

Without purchases or sales, the protected local requirement is at least the greater of:

- The game's own power requirement.
- Power demand multiplied by **1 + reserve**.

For example, at 20% reserve, a city demanding 10,000 units normally needs at least 12,000 units of generating capacity, unless the game's own requirement is higher.

A higher reserve offers better protection against sudden increases in power demand but can increase operating expenses.

A lower reserve reduces costs, leaving less spare capacity before a shortage becomes possible.

**The reserve is a minimum, not a limit on production.**

The Department may maintain additional capacity when profitable power sales justify it.

**Requires:** Manage power and Department active.

See [Power](power.md#power).

<a id="opt-poweraging"></a>
### Consider power plant aging

<text color="secondary">Default: Off | Values: On / Off | INI: `ConsiderPowerAging`</text>

Allows the Department to consider **power plant wear and aging** when comparing power funding arrangements.

When disabled, the optimizer focuses on monthly operating expenses.

When enabled, the calculation also considers the economic cost of plant wear.

This can sometimes make it worthwhile to spend slightly more each month to reduce wear and extend the useful life of a power plant.

**This estimated wear is an economic cost, not an additional monthly charge to your treasury.**

The city's power needs and configured reserve remain protected in both modes.

If Manage power is disabled, the aging preference is preserved but has no effect until power management is enabled again.

**Requires:** Manage power and Department active.

See [Power](power.md#power).

<a id="opt-water"></a>
### Manage water

<text color="secondary">Default: On | Values: On / Off | INI: `AutoWater`</text>

Automatically manages the water department's funding.

Unlike power, water uses **one department funding slider** for its pumps and treatment facilities.

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

See [Water](water.md#water).

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

See [Water](water.md#water-required-capacity).

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

See [Water](water.md#water-pipe-curve).

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

**Unlike power and water, sanitation funding reductions are gradual.**

Reductions are limited to **25 percentage points per write**, with at least **7 simulated days** between cuts.

Funding increases require at least **3 simulated days** since the previous sanitation funding write.

The Department reviews sanitation at month changes and when garbage statistics change significantly.

When enabled, it can also perform safe automatic ghost-landfill-square repairs.

When disabled:

- Sanitation funding stays unchanged.
- Ghost-tile repair stops.
- Landfill size advice is unavailable.
- Garbage contracts can still be managed independently if Manage Garbage Deals is enabled.

**The Department never automatically constructs or zones additional landfill areas.** Landfill reduction is advice only.

**Requires:** Department active.

**Controls:** [Sanitation reserve](#opt-sanitationreserve) and [Landfill size advice](#opt-landfilladvice).

See [Garbage & Landfills](garbage.md#garbage-landfills).

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

See [Garbage & Landfills](garbage.md#garbage-landfills-need).

<a id="opt-landfilladvice"></a>
### Warn me when checking landfill size can help me save

<text color="secondary">Default: Off | Values: On / Off | INI: `WarnLandfillSizeSavings`</text>

Enables optional recommendations about changing the size of your landfill.

The Department looks for situations where:

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

See [Garbage & Landfills](garbage.md#garbage-landfills-advice).

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

See [Garbage & Landfills](garbage.md#garbage-landfills-advice).

<a id="opt-deals"></a>
### Manage neighbor deals

<text color="secondary">Default: On | Values: On / Off | INI: `AutoDeals`</text>

This is the **main switch for automatic regional contract management**.

When enabled, the Department can examine neighboring-city offers and manage power, water and garbage contracts.

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

**Important:** SimCity 4's original neighbor-deal advisor messages are silenced only while the Department manages the contracts of that resource.

If you switch the management off (or the whole Department), the original messages return at once, without restarting the game.

**Requires:** Department active.

**Controls:** [Buy from one city to sell to another](#opt-arbitrage), [Manage Power Deals](#opt-tradepower), [Manage Water Deals](#opt-tradewater) and [Manage Garbage Deals](#opt-tradegarbage).

See [Neighbor Deals](deals.md#neighbor-deals).

<a id="opt-arbitrage"></a>
### Buy from one city to sell to another

<text color="secondary">Default: Off | Values: On / Off | INI: `AllowUtilityArbitrage`</text>

Allows the Department to **buy and sell the same resource during one contract plan** when that combination produces a better economic result.

With this option disabled, each resource follows one of three arrangements:

- **LOCAL:** No regional buying or selling.
- **BUY:** Purchase the resource from neighboring cities.
- **SELL:** Sell the resource to neighboring cities.

With the option enabled, the Department may combine purchases and sales.

For example, it might buy power from one neighbor to cover part of the city's demand, freeing locally generated power for a profitable sale to another neighbor.

**The Department never directly resells imported resources.**

Purchases remain limited by the city's own demand, and the normal service and reserve rules still apply.

For garbage, a mixed arrangement might involve receiving paid garbage from one neighbor while paying another neighbor to dispose of some of your own waste.

A resource whose individual deal management option is disabled is never traded by the Department, regardless of this setting.

**Requires:** Manage neighbor deals and Department active.

See [Neighbor Deals](deals.md#neighbor-deals-arbitrage).

<a id="opt-tradepower"></a>
### Manage Power Deals

<text color="secondary">Default: On | Values: On / Off | INI: `AutoTradePower`</text>

Allows the Department to create, resize and cancel power contracts with neighboring cities.

When enabled, it can:

- Purchase power when economically worthwhile.
- Sell surplus power when profitable.
- Reduce or cancel sales that threaten the city's own supply.
- Adjust existing contracts as demand, production and prices change.

When disabled, existing power contracts remain under your manual control.

The Department still reads them when calculating local power requirements, but it does not modify them or reserve capacity for future automatic sales.

**This setting controls contracts, not power plant funding.**

It operates independently of **Manage power**.

For example, you can manage power contracts automatically while controlling power plant funding yourself. In that case, the Department must work with the generating capacity already available.

**Requires:** Manage neighbor deals and Department active.

See [Neighbor Deals](deals.md#neighbor-deals).

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

See [Neighbor Deals](deals.md#neighbor-deals).

<a id="opt-tradegarbage"></a>
### Manage Garbage Deals

<text color="secondary">Default: On | Values: On / Off | INI: `AutoTradeGarbage`</text>

Allows the Department to manage garbage contracts in both directions.

Remember that garbage contracts work differently from power and water:

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

See [Neighbor Deals](deals.md#neighbor-deals-garbage).

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

<text color="secondary">Default: Off | Values: On / Off | INI: `DemolishDestroyed`</text>

Automatically removes ruins left behind by fires and disasters.

When enabled, the Department can clear destroyed buildings on residential, commercial, industrial and agricultural land after the configured waiting period.

**Ruins are processed before abandoned buildings during each monthly demolition review.**

When disabled, no automatic ruin demolition takes place.

The separate option for municipal and historic ruins also becomes ineffective.

Every demolition remains subject to the game's bulldozing price, available treasury and optional monthly budget.

**Requires:** Department active.

**Controls:** [Wait (ruins)](#opt-destroyedmonths) and [Demolish destroyed municipal and historic buildings](#opt-demolishmunicipal).

See [Automatic Demolition](demolition.md#demolition-destroyed).

<a id="opt-destroyedmonths"></a>
### Wait (ruins)

<text color="secondary">Default: 1 month (30 days) | Range: 1–24 months, steps of 1 | INI: `DestroyedAfterDays`</text>

Determines how long a destroyed building must remain in ruins before the Department may clear it.

The options window displays simulated months, but the Department stores the waiting period in simulated days.

The conversion uses:

```math
\text{Days}=
\mathrm{round}\left(\frac{\text{Months}\times365}{12}\right)
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

See [Automatic Demolition](demolition.md#demolition-delays).

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

See [Automatic Demolition](demolition.md#demolition-protected).

<a id="opt-demolishabandoned"></a>
### Demolish abandoned buildings

<text color="secondary">Default: Off | Values: On / Off | INI: `DemolishAbandoned`</text>

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

See [Automatic Demolition](demolition.md#demolition-abandoned).

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

See [Automatic Demolition](demolition.md#demolition-delays).

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

See [Automatic Demolition](demolition.md#demolition-zones).

<a id="opt-zonecommercial"></a>
### Commercial zone

<text color="secondary">Default: On | Values: On / Off | INI: `AbandonedZones` (`Commercial`)</text>

Includes abandoned commercial buildings in automatic demolition.

This covers low-, medium- and high-density commercial development, including shops and offices.

When enabled, eligible abandoned commercial buildings can be cleared, allowing the zoned land to develop again.

When disabled, abandoned commercial buildings remain untouched.

**Commercial ruins are controlled separately** through the destroyed-buildings setting.

**Requires:** Demolish abandoned buildings and Department active.

See [Automatic Demolition](demolition.md#demolition-zones).

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

See [Automatic Demolition](demolition.md#demolition-zones).

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

See [Automatic Demolition](demolition.md#demolition-farms).

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

See [Automatic Demolition](demolition.md#demolition-reasons).

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

See [Automatic Demolition](demolition.md#demolition-reasons).

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

See [Automatic Demolition](demolition.md#demolition-reasons).

<a id="opt-reasonnopower"></a>
### Reason: no power

<text color="secondary">Default: Off | Values: On / Off | INI: `AbandonedReasons` (`NoPower`)</text>

Controls demolition of buildings abandoned because they lack power.

**This option is disabled by default.**

A building abandoned because of a power failure may become occupied again after power is restored.

When disabled, the Department preserves those buildings.

When enabled, it may demolish them after the configured waiting period.

**Check the power network before enabling this option.**

An abandoned building may only need a repaired connection or restored supply, rather than demolition.

**Requires:** Demolish abandoned buildings and Department active.

See [Automatic Demolition](demolition.md#demolition-reasons).

<a id="opt-reasonnowater"></a>
### Reason: no water

<text color="secondary">Default: Off | Values: On / Off | INI: `AbandonedReasons` (`NoWater`)</text>

Controls demolition of buildings abandoned because they lack water.

Like the power equivalent, **this option is disabled by default**.

Buildings abandoned because of missing water may become occupied again when the water supply or pipe connection is restored.

When disabled, those buildings are preserved.

When enabled, the Department may clear them after the waiting period.

**Check the water network before enabling this option.**

Demolition may be unnecessary if restoring the service would allow the building to recover.

**Requires:** Demolish abandoned buildings and Department active.

See [Automatic Demolition](demolition.md#demolition-reasons).

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

See [Automatic Demolition](demolition.md#demolition-money).

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

See [Automatic Demolition](demolition.md#demolition-money).

---

[Back to the contents](index.md#contents) | [Back to the README](../../README.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
