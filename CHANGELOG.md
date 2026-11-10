# Changelog

## 1.0.2 — revised package (November 2026)

This package **replaces** the 1.0.2 package first uploaded on 9 November 2026. Compared with **1.0.0** it contains everything below. Items marked **New in this revision** were not in the first 1.0.2 package.

> [!IMPORTANT]
> **Installation:** Copy `DepartmentEfficiency.dll`, `DepartmentEfficiency.dat` and `DepartmentEfficiency.ini` to `Plugins`. Back up the INI to keep your personal settings.

### Installation and configuration (New in this revision)

- The DLL, DAT and INI are installed in `Documents\SimCity 4\Plugins`.
- `DepartmentEfficiency.dat` contains the plugin resources and `DepartmentEfficiency.ini` stores user preferences.
- Remove the previous Department installation before updating. Settings from version 1.0.0 must be configured again.

### Phantom Funding Slider Bug: the Department keeps managing affected buildings (New in this revision)

- SimCity 4 can move the funding sliders of already-placed buildings on its own, month after month, once the maintenance cost of their building type has been changed (for example by a mod). The Department did not cause this, and it does **not** fix the bug in the game.
- What changed: when the funding of a school, library, museum, clinic or hospital that the Department itself had set is changed by something else shortly after a monthly recalculation, the Department re-checks that building at its next regular watch and applies the **lowest funding that still gives the capacity it needs**: it raises funding the game lowered too far and lowers funding the game raised too high.
- In the tested scenarios the correction came within one simulated day, where before it could take up to about a month. No new setting, no new notice, no change to how often the Department looks at your city.
- The bug itself is not corrected, but its observed effects on managed buildings are substantially reduced. Details and limits: [Phantom Funding Slider Bug](technical_docs/en/phantom_slider.md).

### The game's own advisor messages come back when the Department is not managing (1.0.2, first package)

- **Problem in 1.0.0:** three extra DAT files replaced complete Maxis advisor scripts and kept some of the game's own warnings silenced for good, even with a feature switched off, and hid other mods' changes to those scripts.
- **Now:** the Department's own small script changes only the conditions of 49 existing advice entries (15 for education and healthcare, 34 for neighbor deals and garbage) and nothing else. The game's message comes back whenever the Department is not really managing that service.
- Education and healthcare are independent of each other. Electricity, water and garbage **deals** are independent of each other and of local sanitation. The master switch and the **Off** and **Shadow** modes are respected. If the Department cannot manage a service, for example because its funding writer is unavailable, the original messages return.
- Genuine strike alerts and the game's other advice are never changed. Other mods' changes to the rest of those scripts are left alone.
- Limits: an event that happens while its message is silenced is not shown afterwards, and a message that returns follows the game's own display frequency. See [Safety](technical_docs/en/safety.md).

### News about technical failures (New in this revision)

- If a service you enabled has to **stop for the rest of the city session** because of a technical failure, one native item appears in the **news list at the bottom of the game**: the title is shown in the list and the detail when you click it. There are no pop-ups.
- Seven kinds: electricity, water, sanitation, healthcare, education, regional neighbor contracts and automatic demolition.
- One item per service and city session. Never for something that recovers by itself, for a service in **STANDBY** (nothing to manage yet), or for a service you switched off. No recovery messages.
- Available in the ten languages of the plugin (140 texts).

### Electricity: cities that export power are no longer blocked (New in this revision)

- **Problem:** the game's monthly consumption counter includes the electricity a city exports. The Department compared it with the city's own demand only, so an exporting city could look as if it consumed more than its demand. It then reported a contradictory reading and stopped managing power ("not ready"), and its checks could treat a short city as served only because of its exports.
- **Fixed:** the export is now taken off before the comparison, in the readiness check, in the pre-check of the counters, in the shortage detection and in the confirmation that a service has recovered. If the export cannot be read, the Department still refuses to act on the reading.
- Benefit: power management continues in exporting cities, and a sale can no longer hide a real shortage.

### Water

- No change in behavior in this release. (A brief "not ready" state while a city loads is normal and is not an error.)

### Sanitation and garbage (New in this revision)

- **Cities with no garbage structure** (no landfill, incinerator, recycling center or waste-to-energy plant) are no longer treated as an error: local sanitation waits in **STANDBY** and starts by itself when a structure appears. In such a city the sanitation funding buys nothing, so the Department lowers it step by step instead of paying for it.
- **Garbage contracts do not depend on local facilities.** A city without a landfill can still export garbage under contract, and contracts and local sanitation are judged separately.
- A city whose garbage structures have just been created or are empty is read correctly; the old "garbage snapshot not valid" message for an empty city is gone.
- Cities that have only an incinerator, a recycling center or a waste-to-energy plant are managed normally.
- A landfill zoned just now is not cut back while the game has not yet reported its capacity (it does so at the next month change).
- **Landfill advice now only recommends safe reductions** (squares to remove). The "expand the landfill" advice was removed. The repair of ghost tiles is unchanged.
- The start-up lines of the log now state what is actually enabled.

### Where the log is (New in this revision)

- `DepartmentEfficiency.log` is now written to **`<SimCity 4 user folder>\Logs\DepartmentEfficiency.log`**, normally `Documents\SimCity 4\Logs`. The plugin uses the folder the game really uses, including a `-UserDir` given on the command line and a Documents folder moved to another drive or to a cloud-synchronised location.
- If that folder cannot be used, the log is written next to the DLL, in `Plugins`, and its first line says why.
- An existing log is appended to and never moved.

### Documentation

- New technical page about the Phantom Funding Slider Bug, a changelog, and updated installation, update and credits sections in every language of the README. The technical reference was moved to `technical_docs/` (the old `features/` pages are gone).

### Unchanged

- Tested game version: SimCity 4 Deluxe, Steam edition, 1.1.641.0, on Windows. No new mod is required. The Department still starts enabled; review its settings before opening a city you care about.

## 1.0.0 (November 2026)

First public release.

<!-- SPDX-License-Identifier: GPL-3.0-only · Copyright (c) 2026 Sammu89 -->
