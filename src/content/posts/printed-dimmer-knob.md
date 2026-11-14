---
title: "A printed dimmer knob for knurled B10K pots"
date: 2026-11-09
summary: "Six versions to get an MS-style panel knob that keys onto a cheap split-shaft pot, points at the right tick, and prints without supports. STL and OpenSCAD source included."
tags: [3d-printing, knobs, openscad]
hero: /img/dimmer-knob-preview.png
files: [knob-stl, knob-scad]
---

<div class="bottom-line">The v6 knob has a 26 mm conical skirt, a knurled Ø17 grip and a low gable roof in line with a painted pointer. A key rib drops into the pot's shaft slot. Bore Ø6.15 with a 0.85 mm rib was a snug fit on the first print. Print it skirt-down in PETG, no supports.</div>

<stl-viewer src="/files/dimmer-knob/dimmer-knob-26mm.stl" label="Dimmer knob v6" color="#c8ccc8"></stl-viewer>

I have a big pile of B10K pots, so every dimmer in the pit will use them: AUX LTS, the Lighting Control Panel, the fuel panel's RCVR LT. They have 6 mm knurled split shafts (18 teeth, 1 mm slot). I wanted a knob that keys to the slot so the pointer can't drift.

## What the real knob looks like

From reference photos of the CONSOLE and AUX INST knobs:

- A slight **roof** on top, with the ridge in line with the pointer
- A **triangle** on the skirt is the only pointer; no channel across the top
- A **shallow sloped skirt**
- A small smooth **band** at the top, at a smaller radius than the knurl (not a proud lip)

## Geometry (v6)

| Feature | Size |
|---|---|
| Skirt | Ø26, 0.8 mm vertical rim, then a cone rising 2.0 mm to the grip (~24°) |
| Grip | Ø17 at the crests, 36 fine straight V-knurls, 11.5 mm to the eaves |
| Top band | Ø15.4, 1.5 mm tall, smooth, no shoulder |
| Roof | 8° gable each side, ridge in line with the pointer, ~15.4 mm overall |
| Pointer | Triangle recessed 0.45 mm into the skirt slope; fill it with white paint |
| Fit | Bore Ø6.15, key rib 0.85 mm, bore depth 5.0 mm |

**Height logic:** my pot's shaft sticks 8.5 mm past the bushing. The bushing ends at the back of the 3 mm top layer, so 5.5 mm of shaft stands proud of the panel face. The shaft tip bottoms on the bore ceiling at 5.0 mm, which leaves the skirt floating 0.5 mm above the paint.

**Pointer logic:** with the pot fully counter-clockwise and its lugs at 6 o'clock, the slot runs 10 to 4 o'clock. On AUX LTS, DIM/OFF is at 7:30 and BRT at 4:30. So the pointer sits at 225° (clockwise from 12) while the rib sits at 300°/120°. If your pot's slot is clocked differently, change `slot_clock` in the .scad and nothing else.

## Version history

- **v1:** flat skirt, painted groove. The fit (Ø6.15 bore, 0.85 rib) was snug on the first try and hasn't changed since.
- **v2:** no top rim.
- **v3:** top rim proud of the knurl.
- **v4:** band at full radius with a 45° shoulder. Still read as a lip.
- **v5:** recessed band. Printed it and liked it.
- **v6:** 2 mm more knurled grip.

The OpenSCAD file also prints a set of six fit-test rings (bore 6.00–6.30 x rib 0.75/0.90) if your pots or printer differ from mine. Set `part = "rings"`.

## Printing

Prusa XL, 0.4 mm nozzle, PETG. Skirt down, no supports, 0.1 mm layers, 3 perimeters. Light grey filament, or paint it. Then fill the pointer triangle with white.

The pot turns a little more than the ~270° arc marked on the panel. I handle that in firmware: the DCS-BIOS `Potentiometer` takes calibrated min/max values, so the marked DIM and BRT positions map to 0 and 100% and anything past them clamps.
