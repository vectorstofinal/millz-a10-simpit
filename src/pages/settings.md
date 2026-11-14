---
layout: ../layouts/Page.astro
title: Settings
eyebrow: Reference sheet
lede: Every setting I've proven in the shop, on one page. Each section links to the post with the test behind it. Your machine will differ, so treat these as starting points.
description: Laser, CNC router and 3D printer settings for building A-10C sim cockpit panels and boards.
---

<div class="note warn">Last updated 2026-11-11. "Proven" means it worked on a production part, not just a coupon. The rest are marked <b>starting point</b>.</div>

## Laser: 1/8 in cast acrylic

Thunder Laser Bolt Pro 36, SPT 80 W RF tube, 2.5 in lens, LightBurn 2.1.04. Houston Acrylic cast, ~3.0 mm bare. Min power = max power. **Low air** (toggle off) for everything. No kerf compensation. Test details: [laser settings post](/posts/acrylic-laser-settings/).

| Job | Mode | Speed | Power | Passes | Interval / scan | Status |
|---|---|---|---|---|---|---|
| Cut: clear, Milky White 2447, black | Line | 30 mm/s | 70% | 1 | — | Proven |
| LED pockets, 3.4 mm, Milky White 2447 | Fill | 200 mm/s | 70% | 2 | 0.1 mm, bi-dir, overscan | Proven |
| LED pockets, clear | Fill | 200 mm/s | 65% | 2 | 0.1 mm, bi-dir, overscan | Trial part (70% ran almost too deep) |
| Engrave through black paint, White Matte (matte face) | Fill | 1500 mm/s | 52% | 1 | 0.05 mm, bi-dir | Coupon, production job ready |
| Engrave through paint, Milky White 2447 | Fill | 600 mm/s | 30% | 1 | 0.1 mm, bi-dir | Coupon (superseded by White Matte) |

Pocket depth is very non-linear: on clear, 60% x 2 = 1.15 mm, but 60% x 3 or 80% x 2 burns through 3 mm.

## Laser: hole sizes that fit

From the [fit strip](/posts/aux-lts-panel/#cut-the-fit-strip-first), cut at 70% @ 30 mm/s.

| Drawn | Part | Fit |
|---|---|---|
| 3.0 mm | 3 mm LED, rim behind | Press fit |
| 7.0 mm | M7 pot bushing | Slightly loose, good |
| 12.0 mm | Toggle bushing | A little play |
| 16.0 mm | MS25089 thread | A little play; flange locates it |
| 17.0 mm | Toggle nut + washer (pocket) | Sits flat |
| 19.3 mm hex AF | MS25089 nut (pocket) | Captive |
| 2.9 mm | M3 screw | Passes |
| 1.8 x 2.6 mm | B10K pot anti-rotation tab slot | Snug (3.0 had slop) |

## CNC router: PCB isolation milling

Onefinity Elite Foreman (Gen 2), Redline controller, 80 mm air-cooled spindle, VCarve Pro. Single-sided FR-4, Z0 on the copper. Proven on the AUX LTS PCB. Details: [PCB milled post](/posts/aux-lts-pcb-milled/), [first board post](/posts/aux-lts-board/).

| Tool | RPM | Feed | Plunge | Depth | Status / notes |
|---|---|---|---|---|---|
| 0.4 mm flat end mill (isolation) | 24k | 120 mm/min | — | 0.08 mm, 1 pass | **Proven.** Z0 by scratch touch-off. Broke 3 bits at 0.10–0.12 mm with a by-eye zero |
| 1.0 mm carbide drill | 18–20k | drilling cycle | 150–200 mm/min | thickness + 0.4 mm | **Proven.** No peck. +0.2 leaves the exit too small for header pins |
| 1/8 in end mill (standoffs, cutouts, outline) | ~18k | 600–800 mm/min | — | 0.5 mm passes | **Proven.** Downcut for clean copper edges. Corners clear a 16 × 30 mm toggle case |

- **Scratch touch-off:** spindle on, step down 0.01 mm over scrap copper until the bit just marks, set Z0. My electrical probe won't trigger on a fresh 0.4 mm bit.
- **Depth:** 1 oz copper is ~0.035 mm. 0.08 mm leaves margin for flatness; don't go below ~0.06 mm.

Design rules for 0.4 mm isolation: ≥0.5 mm net-to-net, 0.8 mm traces, 1.8 mm pads on 1.0 mm holes (0.40 mm ring), traces ≥1.0 mm from the PCB edge.

## 3D printing

Prusa XL, 0.4 mm nozzle, PETG.

| Part | Layer | Perimeters | Supports | Orientation |
|---|---|---|---|---|
| [Dimmer knob](/posts/printed-dimmer-knob/) | 0.1 mm | 3 | None | Skirt down |

Shaft fit on a 6 mm knurled split-shaft pot: Ø6.15 bore with a 0.85 mm key rib, printed in PETG, is a snug fit.

Tree supports: orient the part so each tree meets it with a *small* contact patch. If the tree flares out at the top, it won't break loose.

## Electronics

| Thing | Value |
|---|---|
| Backlight LEDs | 3 mm green, built-in resistor, 12 V, 12–14 mA each |
| Panel LED supply | 12 V from ATX, high-side PWM (IRF9540N), polyfuse 1.1 A per socket |
| PWM frequency | ~3.9 kHz with the TIP42C bench dimmer; 20–30 kHz planned for P-FETs |
| Brightness curve | gamma 2.2, duty floor 6/255 |
| Panel connector | 2x13 IDC, [pinout here](/posts/panel-connector-standard/) |
