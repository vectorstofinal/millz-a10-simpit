---
title: "Milling the AUX LTS board on the CNC router"
date: 2026-11-11
summary: "A single-sided panel board, isolation-milled on the same router that cut the cockpit. Design rules, the VCarve job with a test coupon, starting feeds and speeds, and why my touch probe wouldn't zero a 0.4 mm end mill."
tags: [pcb, cnc, feeds-and-speeds, electronics]
hero: /img/aux-lts-pcb-v2-copper.png
files: [pcb-vcarve, pcb-cnc, shroud-stl]
---

<div class="bottom-line">The board is single-sided FR-4, milled on my router with one VCarve setup: no etching, no mask. A 0.4 mm end mill isolates 0.8 mm traces in a ground pour, about 0.12 mm deep. The first coupon isolated cleanly. Use a scratch touch-off to zero the 0.4 mm bit; my touch probe won't trigger on it.</div>

## Why mill it

I looked at three ways to make panel boards: a fiber MOPA laser, CO2 laser masking plus a ferric chloride etch, and plain isolation milling. I went with milling. The router already exists, it already drills, and every hole, trace and cutout comes from one job with one zero. The trade-off is single-sided boards with wider rules than a fab house would give you. For a panel full of through-hole LEDs and hand-wired switches, that's fine.

## The board

<figure>
  <img src="/img/aux-lts-pcb-v2-copper.png" alt="AUX LTS board v2, copper side as milled, showing the GND pour, LED+ tree, signal traces and hook-up wires" loading="lazy" />
  <figcaption>Copper side, as milled (mirrored relative to the panel front). Yellow: LED+ tree to all 36 anodes. Green: switch and pot signals. Pink: +5 V pot reference. Blue dashes: hook-up wires from switch lugs to pads.</figcaption>
</figure>

- **143 x 120 mm**, single-sided, copper on the back. LEDs and the header's plastic strip go on the front.
- **The whole pour is ground:** LED cathodes, switch commons, pot low ends, standoff pads. That's possible because the LEDs are dimmed on the *high* side in the console box. More on that in the [connector post](/posts/panel-connector-standard/).
- **LED+** is one branching 0.8 mm trace to all 36 anodes and header pins 1–2.
- **Signals and +5 V** are 0.8 mm traces, with a pad next to each control. Short wires run from the switch lugs to those pads.
- **Two GND link wires** jump the LED+ trace where it splits the pour into islands. With both fitted, every ground terminal is one net. I checked that with the 0.4 mm groove simulated.
- **Cutouts** let the switch and pot bodies pass through: toggles 17 x 31 mm, pots Ø19 with a lug notch, MS25089 Ø16.
- **Connector:** a 2x13 extra-long pin header pushed in from the front and soldered on the copper side, with a printed keyed shroud glued over the pins.

### Design rules I held

- ≥0.5 mm between nets (worst case 0.475 mm, between a signal and an unused header pin, which is harmless).
- Traces ≥0.8 mm from cutouts, ≥1.0 mm from the board edge.
- Annular rings: 0.40 mm minimum on the header (1.8 mm pad, 1.0 mm hole).
- Signal traces ≥4 mm from LED anodes, so the LED+ tree can reach every anode.

## The VCarve job

One DXF lays out the board *and* a test coupon on a 250 x 130 mm blank. X0 Y0 is the blank's lower-left corner; Z0 is the copper surface.

<figure>
  <img src="/img/aux-lts-pcb-v2-vcarve-job.png" alt="VCarve job layout: board, test coupon and Z-check marks on a 250 x 130 blank" loading="lazy" />
</figure>

Layers, in run order:

1. **Isolate:** profile outside, 0.4 mm end mill, ~0.10–0.15 mm deep
2. **Drill 1.0 mm:** 117 holes (LED leads, header, pads, GND links)
3. **Drill 3.2 mm:** 4 standoff holes (a 1/8 in tool is fine)
4. **Cutouts:** profile inside, through
5. **Outline:** profile outside, through, **with tabs**

The **coupon** is a 39 x 16 mm window of the real header area. That's the tightest isolation on the board: 0.4 mm rings at 2.54 mm pitch. It also has a standoff hole and one pot and one toggle cutout for test-fitting real parts. Four **Z-check rings** in the blank corners show if the stock or spoilboard isn't flat: each should cut cleanly through the copper.

## Starting feeds and speeds

| Tool | RPM | Feed | Plunge | Depth / passes |
|---|---|---|---|---|
| 0.4 mm flat end mill (isolation) | ~20k | 250–300 mm/min | 60 mm/min | one 0.12 mm pass |
| 1.0 mm carbide drill | 18–20k | VCarve drilling cycle | 150–200 mm/min (250–300 once proven) | full depth, no peck, **thickness + 0.4 mm** |
| 1/8 in end mill (cutouts, outline) | ~18k | 600–800 mm/min | — | 0.5 mm passes, downcut preferred |

- **Drill depth matters.** The 130° point is 0.23 mm long. At thickness + 0.2 mm, the exit hole is only ~0.86 mm, which is too tight for header pins (0.64 mm square, 0.9 mm across the diagonal). Go +0.4 mm. A burned ring or fuzz at the exit means the feed is too slow or the bit is dull.
- **Err deep on isolation.** A flat end mill's groove width doesn't change with depth, so 0.15 mm costs nothing over 0.10 mm, and a shallow spot leaves a short.
- **Toggle cutout corners.** A 1/8 in end mill leaves r 1.59 mm corners, which clears a sharp-cornered 16 x 30 mm toggle case by only ~0.05 mm. Use a 1/16 in end mill for the cutouts if you have one, or test on the coupon.

## Zeroing a 0.4 mm end mill

My touch probe wouldn't register with the 0.4 mm bit. Likely culprits: an insulating coating on the tip (TiAlN/DLC), or too little contact area. I set Z by eye for the first coupon and had to drop it a hair.

The better way is a **scratch touch-off** on copper you'll throw away: spindle on, jog down in 0.01 mm steps until the bit just leaves a bright mark, and set Z0 there. It's repeatable to ~0.01 mm. Paper or eyeballing is ~0.1 mm, the same as the whole cut depth. The drill and 1/8 in end mill have depth margin, so the probe or paper is fine for them.

## Results so far

Coupon C1 (isolation) is done. It **isolated the traces pretty darn well**. Next: the Z-check rings, continuity checks on the tightest gap, then the drill, standoff and cutout coupons. Then the real board.

Shopping note: the header needs to be extra-long. I'm after an L15 mm part: 3 mm short side, 2.5 mm plastic, ~7.9 mm through past the copper. A 19 mm header trimmed down is the fallback.
