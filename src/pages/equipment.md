---
layout: ../layouts/Page.astro
title: Equipment
eyebrow: What's in the millz shop
lede: The machines, software and supplies behind this build, and what each one actually does in it. Settings I've proven on them are on the settings page.
description: The CNC router, laser, 3D printer, software and materials used to build the Millz A-10C sim cockpit.
---

<div class="note">Settings for all of these are on the <a href="/settings/">settings page</a>. Nothing here is sponsored; it's just what I have.</div>

## CNC router: Onefinity Elite Foreman (Gen 2)

| Spec | Detail |
|---|---|
| Machine | Onefinity Elite Foreman (Gen 2), 4 × 4 ft bed (46.5 in of usable Y; the full 48 in is possible if I need it) |
| Controller | Onefinity Redline |
| Spindle | 80 mm air-cooled |
| CAM | Vectric VCarve Pro |
| Zeroing | Electrical XYZ touch probe, mechanical tool setter |

**What it does in this build:**

- **The cockpit structure.** The ACES II seat, both side consoles, the front console and the glare shield are all cut from birch ply and MDF on this machine. See [the structure](/posts/the-structure/).
- **Panel PCBs, isolation-milled.** Single-sided FR-4, with traces, holes, cutouts and outline all done in one job and one zero. See [milling the AUX LTS board](/posts/aux-lts-board/) and [AUX LTS PCB: milled](/posts/aux-lts-pcb-milled/).

PCB tooling:

- 0.4 mm and 0.8 mm flat end mills for isolation
- 1.0 mm carbide PCB drills
- 1/8 in end mill (downcut preferred) for standoff holes, cutouts and outlines

The touch probe won't register a fresh 0.4 mm end mill, so I zero that one with a scratch touch-off on the copper.

## Laser: Thunder Laser Bolt Pro 36

| Spec | Detail |
|---|---|
| Tube | SPT 80 W RF CO2 |
| Lens | 2.5 in (standard) |
| Max engrave speed | 2,000 mm/s (spec) |
| Air assist | Two-stage: the LightBurn air toggle switches low/high; there's no true off |
| Software | LightBurn Pro |

**What it does in this build:** all the acrylic panel work.

- Cutting the three 1/8 in cast acrylic layers
- Filling the LED pockets in the diffuser layer
- Engraving the legends through black paint on the top layer

A CO2 laser can't touch copper (it reflects 10.6 µm), so PCBs stay on the router.

- In LightBurn's Optimization Settings, I keep **Order by Layer/Priority** and **Cut inner shapes first** on. Both are saved in the `.lbrn2` file.
- Engrave layers sit above cut layers in the layer list, so they run first.

## 3D printer: Prusa XL

| Spec | Detail |
|---|---|
| Nozzle | 0.4 mm |
| Filament | Mostly PETG |

FDM only; no resin printer in the shop.

**What it does in this build:**

- Pot knobs (see [the printed dimmer knob](/posts/printed-dimmer-knob/))
- The keyed shroud for the panel header
- Small seat parts that beat stacked sheet goods: headrest pads, a lever, the ejection handles

## Software

| Tool | Used for |
|---|---|
| VCarve Pro | Router CAM: structure parts and PCB toolpaths |
| LightBurn Pro | Laser jobs: panel cuts, pockets and paint engraving |
| FreeCAD | 3D-printed parts and checking parts against the model |
| OpenSCAD | Parametric printed parts (the knob is OpenSCAD) |
| DCS World + [DCS-BIOS](https://github.com/DCS-Skunkworks/dcs-bios) | The sim, and how the pit's switches and lights talk to it |

## Electronics bench

- **Arduino Mega 2560 bench rig** with a 2x13 panel socket, used to test panels before the console controller box exists. See the [connector post](/posts/panel-connector-standard/).
- **ATX power supply** for the 12 V backlight rail.
- **Multimeter** for continuity and isolation checks on every milled PCB before it sees power.

## Materials

| Material | Source / spec | Used for |
|---|---|---|
| 1/8 in cast acrylic: black, Milky White 2447, White Matte, clear | [Houston Acrylic](https://www.houstonacrylic.com) | Panel layers (base, diffuser, painted top) |
| Single-sided copper-clad FR-4 | — | Panel PCBs |
| 3 mm green LEDs, built-in resistor | Rated 9–12 V, 12–14 mA at 12 V | Panel backlighting |
| Birch ply (0.47 in) and 1/4 in MDF | — | Cockpit structure |

## In the pit

- **Thrustmaster HOTAS Warthog throttle**, mounted in the left console
- **VR** is the plan for flying it: DCS through Steam, with the physical controls lined up to the virtual cockpit
