---
title: "Acrylic on the Bolt Pro 36: cut, pocket and engrave-through-paint settings"
date: 2026-11-10
summary: "Every laser setting I use for panel acrylic, with the test grids behind them. The banding fix, the 1500 mm/s paint engrave, a pocket-depth curve that's steeper than you'd think, and a LightBurn coordinate trap."
tags: [laser, acrylic, settings, lightburn]
hero: /img/paint-lettering-test-legend.png
files: [paint-lettering, paint-banding, paint-speed, fit-strip, aux-top-lbrn, aux-mid-lbrn]
---

<div class="bottom-line">On 1/8 in cast acrylic: cut at <b>70% @ 30 mm/s</b>. Pocket the Milky White at <b>Fill 200 mm/s, 70%, 2 passes</b>. Engrave legends through black paint on White Matte at <b>Fill 1500 mm/s, 52%, 0.05 mm interval, bi-directional</b>. Use low air for everything. The 0.05 mm interval is what killed the backlit banding.</div>

## Machine and material

- **Laser:** Thunder Laser Bolt Pro 36. SPT 80 W RF tube, standard 2.5 in lens, LightBurn 2.1.04. 2,000 mm/s max engrave speed.
- **Air:** in LightBurn, the air toggle on = high stage, off = low stage. There's no true off. "Low air" below means the toggle is off.
- **Acrylic:** Houston Acrylic 1/8 in cast, 12 x 19 in sheets, ~3.0 mm bare. Black, Clear, Milky White 2447 (their sign-grade translucent white), and White Matte.
- **No kerf compensation** anywhere. Every fit came out at the drawn size.

These numbers are for my tube and my optics. My optics also needed a clean when I ran most of this. Clean optics deliver more power, and the pocket curve is steep, so re-run one test pocket after any cleaning.

## Production settings

| Job | Mode | Speed | Power | Passes | Air | Notes |
|---|---|---|---|---|---|---|
| Cut: clear, 2447, black | Line | 30 mm/s | 70% | 1 | low | Inner shapes first |
| LED pockets, Milky White 2447 | Fill | 200 mm/s | 70% | 2 | low | 0.1 mm interval, bi-dir, overscan on |
| LED pockets, clear | Fill | 200 mm/s | 65% | 2 | low | Same |
| **Engrave through black paint, White Matte (matte face)** | Fill | **1500 mm/s** | **52%** | 1 | low | **0.05 mm interval, bi-dir** (~0.56 J/mm²) |

Min power = max power on every layer.

## Cut grid

10 mm squares on clear cast, masking on, one pass, low air. Speeds 10–35 mm/s, power 50–100%.

- Every square dropped out except 35 mm/s @ 50%. So at 50% the cut-through limit is ~30–35 mm/s.
- The 60% and 70% columns had the nicest edges at every speed.
- I picked **70% @ 30 mm/s**. It's held up since on clear, 2447 and black.

## LED pocket depth

The 3 mm LED tip sticks ~1.0 mm past the base, so the middle layer needs a 3.4 mm pocket at least ~1.0–1.4 mm deep. Fill, 200 mm/s, 0.1 mm, bi-dir, low air.

| Material | Power | Passes | Depth |
|---|---|---|---|
| Clear | 60% | 1 | 0.5 mm |
| Clear | 60% | 2 | 1.15 mm |
| Clear | 70% | 2 | 1.3 mm |
| Clear | 80% | 2 | **burned through** |
| Clear | 60% | 3 | **burned through** |
| Milky White 2447 | 65% | 2 | 0.9 mm (LED still sits fine) |

Two things to notice:

- **It's not linear.** 60% x 2 gave 1.15 mm, and 60% x 3 went through 3 mm. Creep up on it.
- **2447 pockets ~25% shallower than clear** at the same setting. My guess is the pigment changes the absorption. I run 70% x 2 on 2447 and 65% x 2 on clear.

Also watch the **Fill grouping**. My production middle ran with "Fill shapes individually" *off* (all 36 pockets scanned as one), and it came out right. Switching modes changes how much heat each pocket gets, so re-check depth if you change it.

## Engraving legends through paint

The top layer is painted matte black, and the laser removes the paint where the legends go, so light shows through. I ran three coupons on 2026-11-10.

### 1. Lettering ladder

"LAMP TEST" in MS33558 at 3.175 mm caps (1/8 in), converted to outlines. 15 copies at 10–30% x 300/450/600 mm/s, 0.1 mm interval, bi-dir.

<figure>
  <img src="/img/paint-lettering-test-legend.png" alt="Legend for the 3x5 paint lettering test grid" loading="lazy" />
  <figcaption>Lettering ladder layout. Rows are power, columns are speed. The triangle marks the top-left.</figcaption>
</figure>

- **High air blew white residue all over the coupon.** Low air left much less. Use low air for paint.
- 30% @ 600 mm/s won on both Milky White and White Matte: fully cleared, and sharper, skinnier strokes than the slower columns.
- **White Matte had much better contrast unlit.** Backlit, it's maybe 10% dimmer, but still pretty darn good. That made it my top layer.
- But backlit, the White Matte coupon showed **horizontal banding**.

### 2. Banding: line pitch or scan offset?

Ten "LAMP"s at 600 mm/s. I varied the interval (0.10 / 0.07 / 0.05 mm) and scan direction (bi vs uni), and scaled power to hold ~0.40 J/mm².

<figure>
  <img src="/img/paint-banding-test-legend.png" alt="Legend for the banding test" loading="lazy" />
</figure>

How to read it: if the uni-directional cells come out clean while their bi-directional twins band, the cause is scan offset, so fix it with LightBurn's Scanning Offset Adjustment. If the banding fades as the interval shrinks, whatever the direction, it's line pitch.

**Result: the whole 0.05 mm row was clean, in both directions. It was line pitch.** Backlit, 0.05 mm bi-dir looked the same as uni-dir. Uni-dir takes about twice as long, so I kept bi-dir.

### 3. Speed

Once 0.05 mm bi-dir was settled, I ran 600–1500 mm/s at two energy densities (~0.48 and ~0.56 J/mm²). Power % = E x v x interval / 0.8 (80 W tube, so 0.8 W per %).

<figure>
  <img src="/img/paint-speed-test-legend.png" alt="Legend for the speed test" loading="lazy" />
</figure>

**1500 mm/s at 52% (~0.56 J/mm²) won.** No banding, paint fully cleared, clean strokes, and 2.5x the speed of the control. That's now the production setting. I could push toward the machine's 2,000 mm/s max, but on 3 mm letters the scan lines are short, so I doubt it would save much time.

## LightBurn gotchas

- **Optimization settings** live in the .lbrn2 file: I use Order by Priority and Cut Inner Shapes First. In the layer list, engrave/fill layers sit *above* cut layers so they run first.
- **Layer colors mean different things per file.** C01 is a fill in one file and a line in another. Check the mode before every run.
- **Y runs down on this device.** My device profile has the Y-axis mirrored, so in the .lbrn2 file Y increases downward. If you generate .lbrn2 files from Y-up DXF geometry, use a `1 0 0 -1` transform or your text comes out upside-down. My base-layer file has this bug. It's symmetric enough to fit flipped over, but always check that text reads correctly in the preview.
- **Open shapes don't fill.** The "&" in "& INDEXER LIGHTS" in the DXF is 24 open spline segments. Select it, run *Edit → Auto-Join Selected Shapes*, and confirm it previews as filled.
- **Mirror the middle layer.** The pockets are on the back face, so mirror the job left-right and cut it back face up. Clear and 2447 have no front or back face, so mark the pocketed face "B".

## Font

The Warthog Project DXF legends are MS33558: the glyph widths match the free MS33558 TTF to ±0.01 mm. The DXF has tighter spacing than the TTF; keep the DXF spacing. I standardized on the M whose middle stops short of the baseline for every panel. The real jet mixes a few similar fonts, and I picked this one.
