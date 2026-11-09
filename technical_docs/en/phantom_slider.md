**English** | [Português](../pt/phantom_slider.md) | [Français](../fr/phantom_slider.md) | [Español](../es/phantom_slider.md)

[Back to the technical reference](index.md) · [Back to the README](../../README.md)

# Phantom Funding Slider Bug

This page explains a long-standing SimCity 4 problem, how it was reproduced for this project, what the Department of Efficiency does when it meets it, and, just as important, what it does **not** do.

## 1. What the bug is

After the **maintenance cost** of a building type has been changed (for example by a mod that edits the building's exemplar), SimCity 4 can start moving the **funding sliders** of the buildings of that type that were **already placed** in a saved city. The movement is not random noise: it can repeat at the monthly recalculations, can go on for several months, and pushes the slider in one direction until something stops it.

The community knows this as the **Phantom Funding Slider Bug** (or "phantom slider"). It was described and reproduced by several members of the SimCity 4 community, notably CorinaMarie, Kel9509 and Null 45 (see [Credits and references](#7-credits-and-references)).

**An unexpected slider movement is not proof of this bug.** A player's own change, a legitimate recalculation, another mod or a different game problem can look the same. The Department never concludes that "the bug" is present.

## 2. How it was reproduced

The Department was tested with the reproduction material shared by CorinaMarie: small disposable cities and DAT files that change the maintenance cost of a clinic. The following sequence reproduces the problem:

1. A building's funding slider is **set and confirmed** (in the tests, by the Department's own verified write; the community also reports it with a manual change of the slider).
2. The city is saved and the game closed.
3. A DAT that changes the **maintenance cost of that building type** is installed.
4. The city is opened again and simulated month by month.

In the cities used, cutting the cost of the building's staff line from **300 to 200** made the slider fall month after month (7% → 4.5% → 2.5% → 1.5%), which reduced the building's capacity until it went on strike. Raising the cost from **300 to 450** made the slider climb month after month (7% → 10% → 15% → 33% → 49% → 109% → 120%, the upper limit). Without a confirmed slider, the same change of cost did not move anything.

This happened **without the Department's automatic management** (the Department only observed): it is a behavior of the game, not a defect introduced by the plugin.

**What was measured (not an explanation of the game's internals).** After the cost change, the cost of the building's line and its stored "full funding" followed the new cost, but the budget department's own stored figure for its ideal monthly funding kept the old total. At each monthly recalculation the percentage was scaled by the ratio between the two costs, again and again, until it stopped: after three recalculations when the cost fell, at the upper limit when it rose. Why the game keeps the old figure has not been established.

## 3. What the Department does

The protection uses mechanisms the Department already had. It adds no setting to the INI file, no notice for the player, no external DLL and no new global schedule.

1. **It records every funding change it makes and has verified**: after each write it reads the value back and remembers it for that building.
2. **It watches the buildings it manages.** The civic watch already looks at schools and hospitals every second simulated day.
3. **It recognizes a suspicious change**: the funding of a building is no longer what the Department last verified, and the change shows up in the first days **after a monthly recalculation**. In the cities used, all the changes the game made on its own appeared within one day of the monthly recalculation and none in the middle of a month.
4. **It re-evaluates that building** with the same calculation as always, from the building's real usage and capacity.
5. **It corrects the funding early, in both directions**: it raises funding the game cut too far and lowers funding the game pushed too high.
6. **It applies the lowest funding that still gives the capacity and reserve the policy requires**, not simply the old value.
7. **It checks the result**: the usual safety checks apply, so the value is re-read after the change and compared with what was intended.
8. **It carries on with the other buildings** and with its normal monthly review.

Rules that keep this safe:

- A change in the **middle of a month**, or in a building the Department has not written to, is **not** treated as this problem. It waits for the normal monthly review, as before. A manual change made by the player is therefore not "corrected early", except in the few days that follow a monthly recalculation, when it cannot be told apart from the game's own change (the monthly review would have replaced it anyway).
- An early **reduction** is allowed only for a building whose change was recognized this way. Normal reductions caused by demand falling still wait for the monthly review.
- **At most one early correction per building and per monthly recalculation.** The Department never fights the game in a loop.
- A building is **not** put out of service because of a drift. If a write cannot be verified, the existing safety rules apply: that building is left alone for the session, and after three different buildings the civic writer stops.

In the tests, the longest time between a change made by the game and its correction went **from about 30 simulated days to one simulated day**.

## 4. What the protection does not do

- It does **not** fix the game's internal data behind the problem.
- It does **not** stop the game from moving the sliders again at the next recalculation.
- It does **not** necessarily repair outdated internal costs.
- It does **not** remove phantom budget entries.
- It does **not** guarantee that the money the game actually charges matches the cost the Department calculates.
- It does **not** protect buildings the Department cannot manage (for example fire stations or police stations, or buildings without a standard funding line).
- It does **not** guarantee that a building never has a strike or a capacity shortfall **between** two observations.

What was shown is narrower: **the Department keeps optimizing capacity and funding for the eligible buildings it manages automatically, instead of the sliders staying wherever the game left them.** The bug of SimCity 4 is not corrected, but its observed effects on managed buildings are substantially reduced.

## 5. Limits and safety

Detection is based on signals that can be observed, mainly an unexpected change close to the monthly recalculation. It cannot identify with certainty the cause of each change, and it does not try to. The checks that apply to every write (compare-and-set before writing, read-back afterwards, exclusion of a building whose write cannot be verified, writer stop after three different buildings) apply here too, and the Department refuses to act on readings it cannot trust.

## 6. Evidence

Four runs of the scenarios above (two with the cost going down, two with the cost going up, 10 simulated months each), with the Department managing the clinic automatically, compared with four earlier runs of the same scenarios made before this protection:

| | Before | After |
|---|---|---|
| Longest delay between a change by the game and its correction | up to 30 simulated days | 1 simulated day |
| Changes by the game / verified corrections (per run) | 4 to 10 changes, 3 to 8 corrections | 10 and 10 in every run: one verified correction per change, none without a change |
| Final funding and capacity | 7% and 35 | 7% and 35 (required capacity: 32 to 33) |
| Write failures, buildings put out of service, strikes | none | none |
| Consecutive days below the required capacity (or with funding above the minimum) | up to 31 | at most 2 |

A control run of 8 simulated months **without any change of cost** made **no** additional write. The Department's total time per month was about 2 ms before and after, below the 1 ms resolution of its own log.

These are **observed results in a few disposable test cities**, not a guarantee for every city.

## 7. Credits and references

- **CorinaMarie** explained the phenomenon and shared the reproducible cities and DAT files that made this work possible. Her tutorial on updating a lot's monthly cost while avoiding phantom budget problems describes how to avoid the problem when **creating** mods (by giving the changed building a new exemplar identity), which is prevention, not a repair of an affected city: [tutorial on Simtropolis](https://community.simtropolis.com/forums/topic/761229-tutorial-updating-lot-monthly-cost-while-avoiding-phantom-budget-predicaments/).
- **Kel9509** described a simple way to provoke the problem (change a building's capacity with [SC4PIM-X](https://community.simtropolis.com/files/file/37485-sc4pim-x-2026/), save, reopen, move the slider). That method was not used in the tests described here.
- **Null 45 (Nicholas Hayes)** wrote [Remove Phantom Budget Items](https://community.simtropolis.com/files/file/36437-remove-phantom-budget-items-dll-plugin/) ([source](https://github.com/0xC0000054/sc4-remove-phantom-budget-items)), a console command that removes **orphaned budget entries** (items of buildings whose exemplar no longer exists). It does not read or change the funding sliders, and it is **not** a fix for the behavior described here. It is not part of the Department and is not needed by it.
- The discussion took place in the [comments of the Department's page on Simtropolis](https://community.simtropolis.com/files/file/37700-department-of-efficiency-automatic-city-management-dll/?tab=comments).

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
