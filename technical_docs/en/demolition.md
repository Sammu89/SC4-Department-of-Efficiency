**English** | [Português](../pt/demolition.md) | [Français](../fr/demolition.md) | [Español](../es/demolition.md)

[Back to the contents](index.md#contents) | [Back to the README](../../README.md)

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
| **No power** | The building was abandoned because it lacks power. | **Reason: no power** |
| **No water** | The building was abandoned because it lacks water. | **Reason: no water** |
| **Occupants evicted** | The game reports that the occupants were removed. | Always eligible by reason; no separate option |
| **None / unknown** | The game reports no recognizable abandonment reason. | Always eligible by reason; no separate option |

**Eligibility by reason does not mean immediate demolition.** The waiting period, zone restrictions, historic protection and other safety checks still apply.

#### Why are No power and No water disabled by default?

A building abandoned because of missing power or water may become occupied again once the service is restored.

For example:

- An apartment building loses power and becomes abandoned.
- You repair the power connection.
- The building can become occupied again without being demolished.

Demolishing it too soon would be unnecessary.

**That is why abandonment caused by missing power or water is excluded by default.**

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

A building abandoned because of missing power or water may become occupied again when the service returns.

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

Abandonment caused by missing power or water is excluded by default.

This is intentional: a building abandoned because of a temporary service problem may become occupied again when that service is restored.

<a id="demolition-delays"></a>
#### How do the waiting periods work?

The options window lets you choose waiting periods in **simulated months**.

Internally, however, SimCity 4 records how long a building has remained in its current state using **simulated days**.

The Department therefore converts the selected number of months into days.

**Waiting period conversion**

```math
D=\mathrm{round}\left(\frac{M\times365}{12}\right)
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
1,\mathrm{round}\left(\frac{D\times12}{365}\right)
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

For example, if a residential building loses power, the Department normally leaves it alone while you restore the power supply.

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

See [Water](water.md#water-farm-icon).

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

A demolition safety stop does not disable power, water, sanitation, education, healthcare or regional contract management.

Likewise, a failure in another manager does not automatically disable demolition.

<a id="demolition-settings"></a>
### 5. Settings

Automatic demolition is configured on the **Demolition** page of the Department's options window.

The settings appear in the following order:

| Setting | Default | What it controls |
|---|---|---|
| [Demolish destroyed buildings](options.md#opt-demolishdestroyed) | On | Automatically clears eligible ruins. |
| [Wait (ruins)](options.md#opt-destroyedmonths) | 1 month | How long ruins must remain before demolition. |
| [Demolish destroyed municipal and historic buildings](options.md#opt-demolishmunicipal) | Off | Allows demolition of protected buildings' ruins only. |
| [Demolish abandoned buildings](options.md#opt-demolishabandoned) | On | Automatically clears eligible abandoned buildings. |
| [Wait (abandoned)](options.md#opt-abandonedmonths) | 12 months | How long buildings must remain abandoned. |
| [Residential zone](options.md#opt-zoneresidential) | On | Includes abandoned residential buildings. |
| [Commercial zone](options.md#opt-zonecommercial) | On | Includes abandoned commercial buildings. |
| [Industrial zone](options.md#opt-zoneindustrial) | On | Includes abandoned industrial buildings. |
| [Agricultural zone](options.md#opt-zoneagriculture) | Off | Includes abandoned farms and fields. |
| [Reason: unattractive area](options.md#opt-reasonlowdesirability) | On | Includes abandonment caused by low desirability. |
| [Reason: commute too long](options.md#opt-reasoncommute) | On | Includes abandonment caused by long commutes. |
| [Reason: no demand](options.md#opt-reasonlowdemand) | On | Includes abandonment caused by insufficient demand. |
| [Reason: no power](options.md#opt-reasonnopower) | Off | Includes abandonment caused by missing power. |
| [Reason: no water](options.md#opt-reasonnowater) | Off | Includes abandonment caused by missing water. |
| [Limit the monthly demolition budget](options.md#opt-budgetlimit) | Off | Limits demolition spending during each monthly review. |
| [Monthly budget](options.md#opt-demolitionbudget) | §1,000 | Sets the spending limit when the budget option is enabled. |

All automatic demolition settings depend on [Department active](options.md#opt-master).

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

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
