**English** | [Português](README.pt.md) | [Français](README.fr.md) | [Deutsch](README.de.md) | [Español](README.es.md) | [Italiano](README.it.md) | [Nederlands](README.nl.md) | [Dansk](README.da.md) | [Svenska](README.sv.md) | [Polski](README.pl.md)

# Department of Efficiency

**The Department of Efficiency takes care of the small details of city management that make a big difference.**

Department of Efficiency is a plugin for **SimCity 4 Deluxe** that automates routine city management tasks, so you can focus on building, planning, and growing your city.

**You build the city. The Department optimizes the budget!**

It manages individual education and healthcare budgets, power plants, water and sanitation funding, deals with neighboring cities, and the optional demolition of ruins and abandoned buildings. It aims to meet the city's needs without spending more than necessary, while leaving building placement, coverage and major planning decisions to you.

Everything is controlled from within the game at **Play Options → Department of Efficiency**. Choose the services you want to automate, adjust their safety margins, configure demolition, or switch the entire Department off. You can also use the **Establish a Department of Efficiency** city ordinance as the master on/off switch. No separate application is needed.

> [!IMPORTANT]
> **The Department starts enabled, including automatic demolition of destroyed and abandoned buildings.** Review its settings before opening a city you care about. It makes real in-game changes: funding, contracts and demolition will be saved if you save the city. It does not directly edit save files.

## Contents

- [Download and installation](#download-and-installation)

- [What the Department does](#features)

- [Setup](#first-time-setup)

- [Settings](#settings-explained)

- [Screenshots](#screenshots)

- [Requirements and compatibility](#requirements-and-compatibility)

- [Updating and uninstalling](#updating-and-uninstalling)

- [Known limitations](#known-limitations)

- [Source code](#source-code)

- [License](#license)

- [Credits](#credits)

<a id="download-and-installation"></a>

## Download and installation

### [⬇ Download Department of Efficiency 1.0.0](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases/download/v1.0.0/Departement_of_Efficiency_1.00.zip)

1\. Open the project’s [Releases page](https://github.com/Sammu89/SC4-Department-of-Efficiency/releases) and choose the latest release.

2\. Under **Assets**, download the **Department of Efficiency distribution ZIP**.

3\. **Close SimCity 4.** Extract the ZIP to a temporary folder.

4\. Copy **`DepartmentEfficiency.dll`** and the entire **`Department of Efficiency`** folder into the root of the `Documents\SimCity 4\Plugins`.

5\. Start the game and follow the [first-time setup](#first-time-setup) instructions before playing a saved city.

The distribution ZIP contains the DLL and a folder with five DAT and three INI files. After installation, you should have:

```text
Documents\SimCity 4\Plugins\
    DepartmentEfficiency.dll
    Department of Efficiency\
        DepartmentEfficiency.dat
        DepartmentEfficiencyUI.dat
        DepartmentEfficiencyGarbageAdvice.dat
        DepartmentEfficiencyDealAdvice.dat
        DepartmentEfficiencyCivicAdvice.dat
        DepartmentEfficiency.ini
        DepartmentEfficiencyUtility.ini
        DepartmentEfficiencyDemolition.ini
```

The public repository's Markdown files and screenshots are **not** required by the game. Keep the DLL **directly in Plugins**, with its accompanying folder alongside it.

**Changing settings later:** open **Play Options → Department of Efficiency**, use the **Services** and **Demolition** pages, and click **Accept**. For the master switch you can also use the **Establish a Department of Efficiency** ordinance at no monthly cost. Switching the Department off stops future actions; it does not undo funding, contracts or demolition already applied.

<a id="features"></a>

## What the Department does

The table below describes what the Department does. For formulas, examples, safeguards and the mechanics behind its decisions, see the **[detailed feature reference](technical_docs/en/index.md)**.

| Area | What it does |
|---|---|
| [Education and healthcare](technical_docs/en/education_healthcare.md#education-healthcare) | Adjusts funding **building by building** for eligible schools, colleges, universities, libraries, museums, clinics and hospitals. It follows actual attendance and capacity, with a safety margin. It **does not change coverage radii** or school bus and ambulance funding. If a building is still overcrowded at maximum funding, an advisor tells you that another facility is needed; **you choose where to build it**. |
| [Electricity](technical_docs/en/power.md#power) | Manages **each power plant individually**, choosing funding that covers the city's needs and safety reserve at the lowest modeled cost. It considers plant efficiency and condition; an optional setting also includes the economic cost of plant aging. |
| [Water](technical_docs/en/water.md#water) | Adjusts the city's water funding to meet demand plus a safety reserve. Pipe protection is enabled by default to avoid cuts that would let the network deteriorate. It also hides the game's misleading “no water” icon over farms. |
| [Sanitation and garbage](technical_docs/en/garbage.md#garbage-landfills) | Adjusts sanitation funding using garbage production, recycling, treatment, landfill storage and regional contracts, while protecting the city's own disposal needs. |
| [Neighbor deals](technical_docs/en/deals.md#neighbor-deals) | Can **buy or sell electricity and water**, **pay neighbors to take your garbage**, or **accept theirs for payment**. It creates, adjusts and cancels eligible contracts when the overall result benefits the city and its own needs and reserves remain protected. |
| [Automatic demolition](technical_docs/en/demolition.md#demolition) | Can clear ruins after roughly **30 simulated days** and eligible buildings abandoned for **one simulated year**, by default. You decide which zones and abandonment reasons count, whether protected ruins may be cleared, and whether to set a monthly spending limit. |
| [Advisor notices](technical_docs/en/safety.md#notices) | Uses the game's own advisors to flag proven, persistent shortages that funding or available deals cannot fix. Where appropriate, messages include links that take you to the affected buildings. |
| [Landfill size advice](technical_docs/en/garbage.md#garbage-landfills-advice) | Optional advice identifies a specific landfill expansion or safe reduction that could cut the sanitation bill by more than your chosen savings threshold. **The Department advises; you decide whether to change the zoning.** |
| [Ghost-tile repair](technical_docs/en/garbage.md#ghost-tiles) | When sanitation management is enabled, safely repairs eligible invisible landfill squares that the game still counts toward maintenance. Unsafe or unverified cases are left alone; there is no separate switch for this repair. |

You remain in control of construction, zoning, taxes, roads and transport, police, fire services and any other service the Department does not manage. If you enable the management of a type of neighbor deal, the Department may also change or cancel a contract you previously made manually. Turn that deal option off to manage those contracts yourself.

<a id="first-time-setup"></a>

## Setup

The Department uses the settings supplied with the plugin and **starts working automatically**. You do not have to turn it on, but you should review what it is allowed to do before using an important saved city.

1\. Start SimCity 4 and, from the **region view**, open **Play Options → Department of Efficiency**. If you prefer, use a new or disposable test city for your first look.

2\. On the **Services** page, choose which budgets and neighbor deals the Department may manage and how much spare capacity it should keep.

3\. On the **Demolition** page, check the rules for clearing ruins and abandoned buildings. **Disable automatic demolition if you want to bulldoze everything yourself.**

4\. Click **Accept** to save your choices. **Cancel** closes the window without saving changes.

5\. Open the city you want to play. The Department follows your saved preferences and manages the city as the simulation runs.

Your preferences are stored in the plugin's INI files and are reused when you open another city. The Department does **not** build schools, hospitals, power plants or water facilities for you. When funding and available deals cannot solve a proven, persistent shortage, an in-game advisor tells you where more capacity is needed.

You can also activate or deactivate the Department through the city ordinances.

<a id="settings-explained"></a>

## Settings

Open **Play Options → Department of Efficiency**. The window has two pages, **Services** and **Demolition**. The tables below cover **every setting shown in the menu**, without the technical formulas. Click an option name to read its full explanation in the [feature reference](technical_docs/en/options.md#options).

**How the controls work:** “On” means the Department is allowed to manage that feature. A setting that depends on another feature is greyed out while its parent is off. Percentage settings control safety margins and thresholds, helping the Department maintain spare capacity and accommodate fluctuations in demand.

### Services

| Option | Default | What it means |
|---|---|---|
| [Department active](technical_docs/en/options.md#opt-master) | **On** | Master switch for funding, neighbor contracts, demolition and Department notices. Switching it off does not undo changes already made. |
| [Manage education](technical_docs/en/options.md#opt-education) | **On** | Automatically adjusts eligible schools, universities, libraries and museums individually. Does not change their coverage or school bus funding. |
| [Safety margin (education)](technical_docs/en/options.md#opt-educationreserve) | **5%** (0–50%) | Extra capacity above current education usage. More capacity costs more. For buildings with low attendance, the Department may keep an additional safety margin. For example, this may allow a school to grow from 10 to 20 students (a 100% increase) without a strike, particularly early on, when the extra cost is small. |
| [Manage health](technical_docs/en/options.md#opt-health) | **On** | Automatically adjusts eligible clinics, hospitals and other supported health facilities individually. Does not change ambulance funding or coverage. |
| [Safety margin (health)](technical_docs/en/options.md#opt-healthreserve) | **5%** (0–50%) | Works in the same way as the education safety margin. |
| [Manage power](technical_docs/en/options.md#opt-power) | **On** | Adjusts the funding of individual power plants to provide sufficient electricity at the lowest modeled cost. |
| [Power reserve](technical_docs/en/options.md#opt-powerreserve) | **20%** (10–50%) | Spare generating capacity for the part of demand supplied locally; higher reserves provide more spare capacity to accommodate rapid growth and reduce the risk of blackouts, but may cost more. |
| [Consider power plant aging](technical_docs/en/options.md#opt-poweraging) | **Off** | Includes the estimated cost of power plant wear and aging when optimizing funding. This may favor higher funding when the long-term benefit in terms of power plant conservation outweighs the additional operating cost. |
| [Manage water](technical_docs/en/options.md#opt-water) | **On** | Adjusts the water department's funding to supply the city without unnecessary spending. |
| [Water reserve](technical_docs/en/options.md#opt-waterreserve) | **10%** (10–50%) | Works in the same way as the power reserve. |
| [Protect water pipes](technical_docs/en/options.md#opt-waterpipes) | **On** | Keeps funding at or above the game's 50% to avoid damage to the pipe network. |
| [Manage sanitation](technical_docs/en/options.md#opt-sanitation) | **On** | Adjusts the garbage department's funding to cover collection and disposal needs. Also repairs ghost tiles (a bug in the game that charges maintenance costs for dezoned landfill tiles). |
| [Sanitation reserve](technical_docs/en/options.md#opt-sanitationreserve) | **2%** (2–20%) | Extra treatment capacity for the city's garbage, on top of what it produces after recycling and any existing backlog. |
| [Warn me when checking landfill size can help me save](technical_docs/en/options.md#opt-landfilladvice) | **Off** | Enables optional advice about changes to landfill zoning if that saves money while still handling the city’s garbage. Requires sanitation management. |
| [Landfill savings alert threshold](technical_docs/en/options.md#opt-landfillthreshold) | **25%** (20–50%) | Only asks you to consider a landfill size change if the modeled monthly saving is at least this amount of the sanitation budget. |
| [Manage neighbor deals](technical_docs/en/options.md#opt-deals) | **On** | Enables automatic regional contracts; the three individual deal switches below decide which resources may be traded. |
| [Buy from one city to sell to another](technical_docs/en/options.md#opt-arbitrage) | **Off** | Allows profitable buy-and-sell combinations for the same resource across neighbors, while keeping the city needs first. |
| [Manage Electricity Deals](technical_docs/en/options.md#opt-tradepower) | **On** | Allows the Department to create, change or cancel electricity **imports and exports**. Independent of power-plant funding. |
| [Manage Water Deals](technical_docs/en/options.md#opt-tradewater) | **On** | Allows the Department to create, change or cancel water **imports and exports**. Independent of water funding. |
| [Manage Garbage Deals](technical_docs/en/options.md#opt-tradegarbage) | **On** | Allows the Department to arrange garbage treatment **in either direction**. Independent of sanitation funding. |

### Demolition

| Option | Default | What it means |
|---|---|---|
| [Demolish destroyed buildings](technical_docs/en/options.md#opt-demolishdestroyed) | **On** | Demolishes ruins after a certain period. Does not automatically clear protected municipal or historic ruins unless allowed below. |
| [Wait (destroyed buildings)](technical_docs/en/options.md#opt-destroyedmonths) | **1 month** (1–24) | How long ruins must remain before they can be cleared; approximately **30 simulated days** by default. |
| [Demolish destroyed municipal and historic buildings](technical_docs/en/options.md#opt-demolishmunicipal) | **Off** | If enabled, also clears eligible **ruins** of municipal, special and historic buildings after the wait. |
| [Demolish abandoned buildings](technical_docs/en/options.md#opt-demolishabandoned) | **On** | Clears eligible abandoned buildings after the waiting period, in the selected zone types and for the selected reasons. |
| [Wait (abandoned buildings)](technical_docs/en/options.md#opt-abandonedmonths) | **12 months** (1–60) | How long a building must remain abandoned before it may be demolished; one simulated year by default. |
| [Residential zone](technical_docs/en/options.md#opt-zoneresidential) | **On** | Includes abandoned houses and apartment buildings. |
| [Commercial zone](technical_docs/en/options.md#opt-zonecommercial) | **On** | Includes abandoned shops and offices. |
| [Industrial zone](technical_docs/en/options.md#opt-zoneindustrial) | **On** | Includes abandoned factories and other industrial buildings. |
| [Agricultural zone](technical_docs/en/options.md#opt-zoneagriculture) | **Off** | Includes abandoned farms. Off by default because empty / abandoned tiles on a farm can be normal. |
| [Reason: unattractive area](technical_docs/en/options.md#opt-reasonlowdesirability) | **On** | Includes abandonment caused by poor desirability, such as pollution or lack of services. |
| [Reason: commute too long](technical_docs/en/options.md#opt-reasoncommute) | **On** | Includes abandonment caused by excessively long journeys to work. |
| [Reason: no demand](technical_docs/en/options.md#opt-reasonlowdemand) | **On** | Includes buildings abandoned because demand for that type has disappeared. |
| [Reason: no power](technical_docs/en/options.md#opt-reasonnopower) | **Off** | Optionally includes buildings abandoned because they lack electricity. Usually better to restore power first to check if the building is repopulated. |
| [Reason: no water](technical_docs/en/options.md#opt-reasonnowater) | **Off** | Optionally includes buildings abandoned because they lack water. Usually better to restore the water first to check if the building is repopulated. |
| [Limit the monthly demolition budget](technical_docs/en/options.md#opt-budgetlimit) | **Off** | Sets an optional spending cap for automatic demolition, in addition to the city's available treasury. |
| [Monthly budget](technical_docs/en/options.md#opt-demolitionbudget) | **§1,000** (§0–§200,000) | Maximum demolition spending per simulated month **when the budget limit is enabled**. |

<a id="screenshots"></a>

## Screenshots

All screenshots show the English version of the game.

| | |
|---|---|
| ![The Services page of the Department's settings](screenshots/Options-1.png) | ![The Demolition page of the Department's settings](screenshots/Options-2.png) |
| *The Services page: each service, its reserve and neighbor deals.* | *The Demolition page: ruins, abandoned buildings and the monthly budget.* |
| ![An advisor notice about overcrowded high schools](screenshots/high_school.png) | ![An advisor notice about overcrowded museums](screenshots/museum.png) |
| *Schools at maximum funding and still overcrowded: the notice links to the buildings.* | *The same for museums.* |
| ![An advisor notice about a water shortage](screenshots/water.png) | ![An advisor notice about a garbage shortage](screenshots/real-garbage-paused.png) |
| *Water: the Department cannot resolve the shortage with the available supply.* | *Garbage: the city needs more disposal capacity.* |

<a id="requirements-and-compatibility"></a>

## Requirements and compatibility

- **SimCity 4 Deluxe, Steam edition, game version 1.1.641.0, on Windows.** This is the tested version; the Department is untested on other versions.

- **No other mod, installer or external application is required.**

- **Tested and compatible with these mods**:

  - Network Addon Mod (NAM) — 1.3.1
  - Colossus Addon Mod (CAM) — 3.1.1
  - SC4 System Integration Module — 4.0.1
  - CAM Ordinances — 3.3.0
  - Industrial Revolution Mod (IRM) — 2.0.0
  - Colossus Farming
  - Advanced Water Management — 1.0
  - SC4Fix — 1.0.0.7
  - More Building Styles — 3.6.1
  - More Demand Info — 1.3.1
  - Custom Budget Departments — 1.1
  - Custom Ordinance Host — 1.0.1
  - Legalize Gambling Ordinance Upgrade — 1.0.0
  - Lua Sandbox — 1.0.0
  - Lua Extensions — 1.5
  - Query Tool UI Extensions — 2.7.1
  - Resource Loading Hooks — 2.0.1
  - Transparent Texture Fix — 1.0.0
  - I-R Fix — 1.0.0
  - Universal Commercial Cap Fix — 2.0.0
  - Save Warning — 1.0
  - Zoning Mod Maxis — 2.0.0
  - Maxis Prop Names and Query Fix — 2.0
  - Seasonal Flora Patch — 2.1
  - No Maxis I-D, I-M e I-HT — 2.0

- **Recommended:** add `-CPUCount:1` in **Steam → Library → SimCity 4 Deluxe → Properties → Launch Options**. This helped stability in long sessions on my computer.

- **Ten in-game languages:** English, Portuguese, French, German, Spanish, Italian, Dutch, Danish, Swedish and Polish. The Department follows the language of your game automatically.

<a id="updating-and-uninstalling"></a>

## Updating and uninstalling

**Keep your preferences.** The three INI files in the `Department of Efficiency` folder store your settings. Before replacing the mod, back up `DepartmentEfficiency.ini`, `DepartmentEfficiencyUtility.ini` and `DepartmentEfficiencyDemolition.ini` outside your Plugins folder.

**To replace an installation:** close the game, remove the existing `DepartmentEfficiency.dll` and `Department of Efficiency` folder, install the new package, and restore your backed-up INI files **only if their format is compatible** with the new release. Otherwise, set your preferences again in the game.

**To uninstall:** close the game, delete `DepartmentEfficiency.dll` and the `Department of Efficiency` folder. Your saved cities remain readable without the mod. Funding levels, existing contracts and any demolitions already saved in a city are not automatically reversed.

<a id="known-limitations"></a>

## Known limitations

- **Compatibility:** I tested the Windows Steam build of SimCity 4 Deluxe **1.1.641.0**. Other game versions have not been tested.

- **Landfill advice:** requires exactly one landfill zone. In cities with multiple landfills, the Department can still repair eligible bugged 'ghost tiles' but does not recommend size changes.

<a id="source-code"></a>

## Source code

The plugin's source code is in the [`source/`](source/) folder. It builds with CMake and Visual Studio into a 32-bit (Win32) DLL. You need Visual Studio 2026 with the C++ desktop workload (other versions are untested), CMake 3.20 or newer and Git:

```bat
cmake -S . -B build -G "Visual Studio 18 2026" -A Win32
cmake --build build --config Release
```

The build downloads [gzcom-dll](https://github.com/nsgomez/gzcom-dll) at a fixed revision (`779b669bf88112146d58fba6c7ed44f13ac6ce7d`). To use a modified copy of that library, or to build without an Internet connection, add `-DFETCHCONTENT_SOURCE_DIR_GZCOM_DLL=<folder>` to the first command. The DLL is written to `build\Release\DepartmentEfficiency.dll`. A rebuilt DLL is not byte-for-byte identical to the published one.

<a id="license"></a>

## License

- The original code of Department of Efficiency (the files in [`source/`](source/)) was written by me, **Sammu89**, and is released under the **GNU General Public License, version 3 only** (GPL-3.0-only). The full text is in [LICENSE](LICENSE). You may study, modify and redistribute it under the GPLv3: keep the copyright notices and credits, mark your changes, and release your version under the same license. Please do not present modified versions as my official versions.

- The documentation (these READMEs and the files in [`technical_docs/`](technical_docs/)) and the texts written for this project (including those in the three INI files and the ones compiled into `DepartmentEfficiency.dat`), together with their original translations, are released under the same license, GPL-3.0-only.

- **gzcom-dll** is not covered by this license: it keeps its own license, LGPL-2.1-or-later ([licenses/LGPL-2.1.txt](licenses/LGPL-2.1.txt)), and is downloaded from its original repository when you build.

- **SimCity 4** and its original resources belong to Electronic Arts / Maxis. **Department of Efficiency** is an unofficial fan-made mod, independent of EA and not endorsed by it. Four of the files in `release/Department of Efficiency/` contain scripts and interface definitions derived from the game's own resources, and the screenshots show the game: these are not my property, they are not covered by the GPL, and all rights to them remain with their owners. The same applies to any other third-party material.

<a id="credits"></a>

## Credits

**Author**

- Created by **Sammu89**.

**Library used**

- **gzcom-dll**, by Nelson Gomez (simmaster07), with contributions from Nicholas Hayes (Null 45), Casper Van Gheluwe, memo and others (see its [repository](https://github.com/nsgomez/gzcom-dll)), used under the LGPL-2.1-or-later license and statically linked into the DLL. The library is not bundled: the build downloads the exact revision from its original repository (see [Source code](#source-code)); its copyright notices and the license text are in [licenses/gzcom-dll.txt](licenses/gzcom-dll.txt) and [licenses/LGPL-2.1.txt](licenses/LGPL-2.1.txt). You may modify the library and rebuild the plugin with your version.

**Techniques, documentation and examples consulted** (no code was copied from them; their authors are not co-authors of Department of Efficiency):

- Nicholas Hayes (0xC0000054 / Null 45), for SimCity 4 DLL research, documentation and open examples: sc4-dll-utilities, sc4-custom-budget-departments, sc4-query-ui-hooks, sc4-lua-extensions, sc4-lua-extension-example. The options window, the ordinance and the notice file follow mechanisms published in his projects sc4-region-census, sc4-humane-society-ordinance and sc4-lua-extension-example (MIT license); game commands follow sc4-auto-run-cheats and sc4-game-command-documentation.

- CorinaMarie, for documenting the SimCity 4 Urgent Advice technique that the mod's advisor notices follow.

- The SimCity 4 modding community, for two decades of research, tools and shared knowledge, including the exemplar property documentation of SC4Devotion.

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
