**English** | [Português](../pt/index.md) | [Français](../fr/index.md) | [Español](../es/index.md)

[Back to the README](../../README.md)


# Department of Efficiency: Technical Reference

This guide explains how SimCity 4 works behind the scenes and how the Department of Efficiency makes its decisions, service by service. It is intended for players who want to understand the reasoning behind the Department's actions, as well as modders interested in the game's underlying mechanics.

## Quick Reference

| Service | Main reserve or setting | Default | Main protection | Chapter |
|---|---|---|---|---|
| Education | Capacity safety margin | 5% | Maintains spare capacity to accommodate fluctuations in student attendance. School bus funding is left unchanged. | [Education](education_healthcare.md#education-healthcare) |
| Healthcare | Capacity safety margin | 5% | Maintains spare capacity to accommodate fluctuations in patient numbers. Ambulance funding is left unchanged. | [Healthcare](education_healthcare.md#education-healthcare) |
| Power | Power reserve | 20% | Maintains spare generating capacity to accommodate city growth and reduce the risk of blackouts. Power lines are also protected against deterioration. | [Power](power.md#power) |
| Water | Water reserve | 10% | Maintains spare water production capacity, with pipe protection enabled by default. | [Water](water.md#water) |
| Sanitation | Garbage reserve | 2% | Ensures sufficient waste treatment and disposal capacity, aiming to clear any accumulated garbage by the end of the simulated month. | [Garbage & Landfills](garbage.md#garbage-landfills) |
| Landfill advice | Savings alert threshold | Advice off; threshold 25% | Recommends landfill expansions or reductions when they could significantly lower sanitation costs. | [Landfill Size Advice](garbage.md#garbage-landfills-advice) |
| Neighbor deals | Management by resource | On; dealing between neighbors  off | Manages power, water and garbage contracts while prioritizing the city's own needs and safety reserves. | [Neighbor Deals](deals.md#neighbor-deals) |
| Demolition | Waiting periods | Ruins: 1 month; abandoned buildings: 12 months | Protects municipal and historic buildings by default, with an optional monthly demolition spending limit. | [Automatic Demolition](demolition.md#demolition) |

<a id="contents"></a>

## Contents

* [A. Education & Healthcare](education_healthcare.md#education-healthcare)
* [B. Power](power.md#power)
* [C. Water](water.md#water)
* [D. Garbage & Landfills](garbage.md#garbage-landfills)
  * [Landfill size advice](garbage.md#garbage-landfills-advice)
  * [Ghost tiles](garbage.md#ghost-tiles)
* [E. Neighbor Deals](deals.md#neighbor-deals)
* [F. Automatic Demolition](demolition.md#demolition)
* [G. Safety, Automation & Decision-Making](safety.md#safety)
  * [Advisor notices](safety.md#notices)
* [H. Option reference](options.md#options)

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
