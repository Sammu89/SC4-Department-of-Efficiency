**English** | [Português](../pt/garbage.md) | [Français](../fr/garbage.md) | [Español](../es/garbage.md)

[Back to the contents](index.md#contents) | [Back to the README](../../README.md)

<a id="garbage-landfills"></a>
## D. Garbage & Landfills

The Department helps your city manage garbage efficiently, keeping sanitation costs as low as possible without allowing waste to accumulate unnecessarily.

It provides four separate functions:

1. **Automatic sanitation funding:** Adjusts the sanitation budget to handle the city's garbage, including your chosen safety reserve.

2. **Regional garbage contracts:** Manages garbage imports and exports when neighbor deal management is enabled. See [Neighbor Deals](deals.md#neighbor-deals).

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

Garbage contracts work differently from power and water contracts.

There are two types:

- **Garbage imports:** A neighboring city sends you its garbage and pays you to dispose of it.
- **Garbage exports:** Your city pays a neighboring city to take its garbage.

Although these contracts specify a monthly quantity, **the garbage is not delivered evenly throughout the month**.

Instead, the game normally moves approximately 10% of the contract's monthly quantity each simulated day until that month's allocation has been delivered.

**Daily garbage contract delivery (base game)**

```math
s = \mathrm{round}(0.1 \times A)
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

Unlike power, sanitation has **one funding percentage for the entire department**.

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

**What about power generated from garbage?**

Waste-to-Energy plants also produce power.

Their garbage treatment function belongs to the sanitation system, while their power output is considered separately in the [Power](power.md#power) chapter.

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
G,\ F,\ \mathrm{round}(5000 \times L \times e)-S
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
\mathrm{round}(0.07 \times \text{stock})
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
C = \mathrm{round}\left(\frac{T\times30000}{N}\right)
\qquad
T_{\mathrm{cap}}=N\times C
```

Where:

- **T** is the number of squares recorded in the landfill zone, including ghost tiles (explained below).
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

#### Ghost tiles: landfill areas that should not exist

SimCity 4 has a bug that can leave behind **ghost landfill squares**.

A ghost tile is a location that the game still counts as part of a landfill even though it is no longer zoned as landfill.

For example, the square may now contain:

- An ordinary road.
- An unzoned piece of land.
- Another non-landfill area.

Visually, the landfill is gone from that square.

**Internally, however, the game still counts it as part of the landfill zone.**

This has two consequences:

1. **The square continues to cost §5 per month** in landfill maintenance at 100% funding.
2. **It is included in the zone's recorded square count** when landfill storage capacity is recalculated.

Ghost tiles do not count towards the active landfill square total used to calculate the monthly intake limit.

In other words, **the game can charge you for landfill squares that do not actually help dispose of garbage**.

#### Why do ghost tiles return after reopening a city?

The problem comes from how SimCity 4 saves and reloads landfill zones.

When saving a landfill, the game records its rectangular bounding box and which squares belong to the zone.

However, it does not properly save the membership of squares along the rectangle's final row and column.

When the city is reopened, the game initially assumes that every square inside the rectangle belongs to the landfill, then restores the saved membership information.

Because the final row and column have incomplete saved information, **squares on those edges can incorrectly become landfill members again**.

This creates two types of ghost tiles:

| Ghost tiles location | What happens after repair |
|---|---|
| **Inside the zone's rectangle** | A suitable zoning operation can remove it permanently because its corrected membership is saved. |
| **On the final row or column** | It can return when the city is reopened because the game does not save that edge membership correctly. |

The original game's zoning tools cannot permanently remove every edge ghost because the zone's recorded bounding box does not shrink.

**How does the Department help?**

The Department includes an automatic ghost-tile repair mechanism.

When it detects eligible ghost tiles, it uses the game's own zoning system to correct them rather than directly modifying the save file.

The repair is designed to avoid disturbing legitimate landfill areas and stored garbage.

**There is no separate setting for ghost-tile repair.** It operates as part of automatic sanitation management.

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
- **Unused landfill areas cost money:** Empty squares have monthly maintenance expenses, and ghost tiles can create additional charges.
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
U > (\text{day of month}-1)\times\mathrm{round}(0.1\times A)
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

See [Neighbor Deals](deals.md#neighbor-deals) for the complete contract rules.

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
K(p)=\mathrm{round}\left(K_{\mathrm{now}}\times\frac{e(p)}{e_{\mathrm{now}}}\right)
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
F,\mathrm{round}(5000\times L\times e(p))-S
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

See [Neighbor Deals](deals.md#neighbor-deals).

<a id="garbage-landfills-funding"></a>
#### 3.6 How does the Department choose sanitation funding?

After calculating how much garbage can be handled at each funding level, the Department looks for **the cheapest percentage that satisfies the disposal requirements**.

Sanitation expenses increase in proportion to funding.

**Monthly sanitation expense**

```math
E(p)=\mathrm{round}\left(I\times\frac{p}{100}\right)
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

Unlike power and water, sanitation funding is **not always changed directly to its final target**.

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
- **gᵣₑcₒₙcᵢₗₑd** is the number of ghost tiles removed during expansion.
- **ΔB** is the change in monthly landfill maintenance at 100% funding.
- **L** and **L′** are the landfill square counts before and after the change.

New empty landfill squares cost **§5 each per month** at 100% funding.

Removing an unused square saves that maintenance expense.

An expansion can also remove ghost tiles already costing money, which may partially offset the maintenance of the newly added area.

The Department also recalculates the landfill's storage capacity:

```math
C'=\mathrm{round}\left(\frac{T'\times30000}{N'}\right)
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
E_0 &= \mathrm{round}\left(I\times\frac{p_0}{100}\right)\\
E_1 &= \mathrm{round}\left(I'\times\frac{p_1}{100}\right)\\
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

Automatic ghost-tile repairs do not restart the 24-month protection period.

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

<a id="ghost-tiles"></a>
#### 3.10 Ghost tiles: automatic repair

Ghost landfill squares are different from an oversized or inefficient landfill.

They are **a SimCity 4 bug**, not a zoning decision.

As explained earlier, a ghost tile is a location the game still counts as part of a landfill, even though it is no longer actually zoned as landfill.

This can create unnecessary maintenance expenses.

**The Department can automatically repair these ghost tiles when it manages sanitation.**

Unlike landfill size advice, this repair:

- Does not require a separate setting.
- Does not depend on a savings threshold.
- Does not generate an advisor notice.
- Does not add or remove any legitimate landfill square.
- Does not demolish buildings or change the visible zoning.
- Never deliberately reduces landfill storage capacity.

Repair activity is recorded in the log.

#### How can the Department repair a ghost tile?

The repair uses a particular behavior of SimCity 4's zoning system.

**Reapplying landfill zoning to squares that are already legitimately zoned as landfill causes the game to rebuild the zone's internal structure.**

This operation does not cost money or change those squares' zone types, but the rebuild also removes obsolete ghost-tile membership.

In effect, the Department asks the game to rebuild a landfill using its own zoning rules, without changing the legitimate landfill area.

#### How does the automatic repair work?

The Department follows four steps.

**1. Detect ghost tiles**

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
- The ghost tiles have disappeared.
- The storage-cell arrangement and capacity match the predicted result.
- The legitimate landfill square count remains unchanged.
- The city's landfill capacity matches the expected value.
- The internal landfill information still agrees with the game's totals.

The amount of stored garbage naturally changes as the game runs, so that quantity is recorded rather than required to remain identical.

**The repair is only accepted when the resulting landfill structure matches the expected safe outcome.**

#### What happens if a repair fails?

The Department does not repeatedly attempt risky repairs.

Each distinct ghost-tile state is attempted **at most once per city session**.

If the zoning operation fails or the resulting structure does not match the prediction, **automatic ghost-tile repair is disabled until SimCity 4 is closed**.

Other Department functions can continue operating.

The Department also repairs **at most one landfill zone per simulated month**.

If several zones contain ghost tiles, the remaining zones are considered during later months.

While a repair is awaiting confirmation, landfill size advice is postponed so that it does not analyze an area whose internal structure is still changing.

#### Why might ghost tiles return?

Unfortunately, the original game's save/load behavior is not corrected permanently.

When you reopen a city, SimCity 4 can recreate ghost membership along the final row or column of a landfill's internal bounding box.

**The Department may therefore need to repair the same ghost tiles again after reopening the city.**

That does not mean the previous repair failed. It means the game recreated the bug during loading.

Each ghost tile removed saves **§5 per simulated month at 100% sanitation funding**.

The actual saving at a lower funding percentage is correspondingly smaller.

**Automatic ghost-tile repair is a conservative correction of a game bug, not an automatic landfill redesign.**

<a id="garbage-landfills-safety"></a>
### 4. Safety conditions

The Department applies separate safety rules to sanitation funding, landfill size advice and ghost-tile repair.

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

#### Automatic ghost-tile repair

Automatic ghost-tile repair is available only on the supported **SimCity 4 Deluxe version 1.1.641.0**, while sanitation management is active.

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
| [Department active](options.md#opt-master) | On | On / Off | Enables or disables the entire Department. |
| [Manage sanitation](options.md#opt-sanitation) | On | On / Off | Automatically manages sanitation funding. Also required for landfill size advice and ghost-tile repair. |
| [Sanitation reserve](options.md#opt-sanitationreserve) | 2% | 2–20% | Adds spare disposal capacity to handle increases in garbage production. |
| [Manage neighbor deals](options.md#opt-deals) | On | On / Off | Enables automatic management of regional contracts. |
| [Manage Garbage Deals](options.md#opt-tradegarbage) | On | On / Off | Enables automatic garbage imports and exports when neighbor deal management is active. |
| [Warn me when checking landfill size can help me save](options.md#opt-landfilladvice) | Off | On / Off | Enables recommendations to expand or reduce landfill areas when significant savings are possible. |
| [Landfill savings alert threshold](options.md#opt-landfillthreshold) | 25% | 20–50% | Sets the minimum percentage saving required before the Department recommends changing landfill size. |

#### How do these options work together?

**Manage sanitation**

When enabled, the Department calculates and adjusts the sanitation budget automatically.

If disabled, sanitation funding remains unchanged. Regional garbage contracts can still be managed independently, using the existing funding.

Landfill size advice and automatic ghost-tile repair require sanitation management to be active.

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

**Automatic ghost-tile repair has no separate setting.** It runs as part of sanitation management, on the supported game version, whenever the required safety checks pass.

<a id="garbage-landfills-examples"></a>
### 6. Worked examples

The following examples show how sanitation funding, landfill storage and the Department's safety rules work in practice.

Several examples use a test city where the game's landfill behavior was measured.

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

This example comes from the **test city**.

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

#### Example 4: Repairing ghost tiles in a test city

In the test city, the landfill contained **seven ghost tiles**.

These were squares that the game still counted as part of the landfill even though they were no longer actually zoned as landfill.

The recorded landfill structure was:

| Property | Before repair |
|---|---:|
| Bounding box | 6 × 9 squares |
| Possible storage-cell positions | 40 |
| Active storage cells | 30 |
| Recorded landfill membership | 49 squares |
| Actual landfill squares | 42 |
| **Ghost tiles** | **7** |
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

Each ghost tile also cost §5 per month at 100% funding.

```math
7\times5=35
```

**Expected monthly maintenance saving: §35.**

The Department performed the automatic repair using the game's zoning system.

**Observed results**

| Property | Before repair | After repair |
|---|---:|---:|
| Ghost tiles | 7 | **0** |
| Monthly maintenance | §1,030 | **§995** |
| Storage capacity | 1,260,000 | **1,260,000** |

Every result matched the Department's prediction.

**But the ghosts returned after reopening the city.**

A copy of the test city saved after the repair recreated the same seven ghost tiles when loaded again.

This confirmed the game's save/load bug described earlier.

The Department was able to repair them again during the new city session.

**The repair works, but the original game can recreate the problem every time the city is loaded.**

#### Example 5: An expansion that saves money, but not enough to recommend

The Department also tested expanding the test city's landfill.

Initially, a street ran along its right-hand edge.

The game's zoning checks rejected every expansion candidate on that side because the proposed land was occupied.

**The Department did not recommend or perform an expansion.**

After the street section was removed, a **2 × 2 expansion** became possible.

This would add four landfill squares.

The zoning operation would also remove the seven existing ghost tiles.

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

A different experiment in the test city showed why ghost-tile repair must be conservative.

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

This happened because the ghost tiles still counted towards the recorded membership used in the capacity calculation.

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

**Removing ghost tiles is only allowed when the Department can prove that the operation will not reduce storage capacity or disturb existing garbage.**

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
- **Funding can change during the month.** Garbage imports, exports and landfill intake do not follow a perfectly uniform schedule. The cheapest funding level may therefore change as the month progresses. In the test city, observed decisions included **47%, 63% and 53%** at different times. This is expected behavior, not necessarily a fault.
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

#### Ghost-tile repair

- **Some ghost tiles return after every city load.** The original game's save/load bug can recreate them along the final row or column of a landfill's internal rectangle.
- **The repair cannot permanently fix the game's save format.** The Department may need to perform the same safe correction in each new session.
- **Unsafe ghost repairs are refused.** A ghost tile remains untouched if removing it could reduce landfill capacity, disturb stored garbage or push waste back into the intake valve.
- **Leaving a ghost can still cost money.** Each unrepaired ghost continues to contribute §5 per month at 100% sanitation funding.
- **Multiple landfill zones are handled separately.** The Department can attempt safe ghost repairs one zone at a time, but normally cannot use the single-geometry next-month prediction or provide landfill size advice.

#### The game's hidden garbage removal

SimCity 4 can silently remove a limited amount of garbage through its built-in *abduction* mechanism.

**The Department does not treat that removal as a sanitation failure and does not attempt to prevent it.**

Its goal is to prevent genuine, persistent accumulations of uncollected garbage, not to change every hidden disposal rule of the original game.

**The Department prioritizes safe garbage disposal and reliable funding decisions. When it cannot predict a landfill operation or sanitation change with sufficient confidence, it leaves the existing situation unchanged.**

[Back to the contents](index.md#contents) | [Back to the README](../../README.md)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
