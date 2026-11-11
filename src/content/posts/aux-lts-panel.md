---
title: "AUX LTS: three layers of acrylic and 36 green LEDs"
date: 2026-11-07
summary: "My first backlit panel, built from The Warthog Project's AUX LTS file. Here's what I changed in the DXF, the fit-test results, how the stack goes together, and the cut files."
tags: [panels, acrylic, laser, backlighting]
hero: /img/aux-lts-3d.png
files: [aux-top-dxf, aux-mid-dxf, aux-base-dxf, aux-all-dxf, aux-top-lbrn, aux-mid-lbrn, aux-base-lbrn, fit-strip]
---

<div class="bottom-line">AUX LTS is a three-layer 1/8 in cast acrylic sandwich: black base, Milky White diffuser, painted White Matte top. It has 36 built-in-resistor green LEDs on a milled board behind it. The Warthog Project file needed seven edits to fit my switches and LEDs. All the edited files are below.</div>

AUX LTS sits on the left console. It has the SIGNAL LIGHTS LAMP TEST button, and it's small enough to learn the whole process on: two pots, two toggles, two pushbuttons. Every panel after this one will follow the same recipe.

<a class="teaser" href="/3d/aux-lts/">
  <img src="/img/aux-lts-3d.png" alt="3D render of the AUX LTS panel with green backlighting on" loading="lazy" />
  <div><b>Open the 3D model →</b><span>Orbit the full stack, explode the layers, toggle the backlight, and see the board wiring from the copper side.</span></div>
</a>

<figure>
  <img src="/img/aux-lts-top-preview.png" alt="AUX LTS top layer as laid out in LightBurn: engraved legends in white, cut lines in red" loading="lazy" />
  <figcaption>The top-layer production job in LightBurn. White is engraved through the paint, red is cut.</figcaption>
</figure>

## The stack

| Layer | Material | What it does |
|---|---|---|
| Top | Houston Acrylic **White Matte**, matte face painted matte black | Legends engraved through the paint. Carries the pushbutton flanges and knob skirts. |
| Middle | **Milky White 2447** cast | Diffuser. 36 back-face LED pockets. Holds the toggle and pot nuts, plus two captive hex nuts for the pushbuttons. |
| Base | **Black** cast | 36 x 3.0 mm LED press-fit holes. Switch bodies clamp to its back. |
| Standoffs | M3 F-F brass, 6 mm | The same four M3 screws clamp the stack and carry the board. |
| Board | Single-sided FR-4, copper on the back | [Milled on the router](/posts/aux-lts-board/): GND pour, LED+ tree, hook-up pads. |

Each sheet is ~3.0 mm bare (calipered without masking), so the stack is ~9.0 mm. The Warthog Project uses a 2 mm top. I use 1/8 in for all three layers, and everything still works:

- The toggles' 9 mm thread ends flush with the face.
- The toggle nut plus washer (3.0 mm) sits inside the middle layer.
- The pot's 6 mm bushing ends at the back of the top layer.
- The LED tip pokes 1.0 mm into the middle layer, hence the pockets.
- The MS25089 nut (2.33 mm) hides inside the middle layer.

I first tried painting the Milky White for the top layer. White Matte with its matte face painted gives much better contrast unlit, for maybe 10% less backlight. The [laser settings post](/posts/acrylic-laser-settings/) has the coupons.

## Edits to the original DXF

The Warthog Project's file is excellent, but it's drawn around his parts, not mine. Here's what I changed:

| Item | Original | Mine | Why |
|---|---|---|---|
| Pushbutton top holes | 12 mm | 16.0 mm | MS25089-3G has a 16 mm thread; its 17.4 mm flange (measured) sits on the face |
| Pushbutton middle holes | 16 mm round | 19.3 mm hex across flats | Captive nut, no wrench needed |
| Pushbutton base holes | 12 mm | 16.4 mm | Thread plus can clearance |
| HARS/SAS legend LEDs | 2 at (0, ±12) | 4 at (±10.4, ±12) | My 16 x 30 mm toggle case covered them |
| Pot-ring LEDs | r ≈ 9.8–10.0 mm | r 11.4 mm | The 18 mm pot body covered the LED rims |
| Middle layer | no pockets | 36 x 3.4 mm back-face pockets | LED tips protrude into it |
| Pot anti-rotation slots (base) | none | 1.8 x 2.6 mm, 7.5 mm from the pot center at 9 o'clock | Keeps the pot's tab; locks it with the lugs at 6 o'clock |

I cut the first base with 1.8 x 3.0 mm slots and had a tiny bit of slop. The files now have 2.6 mm. With the LEDs moved, the closest LED-to-LED spacing is 6.36 mm and every LED clears every hole by at least 3.15 mm.

<figure>
  <img src="/img/aux-lts-led-clearance.png" alt="Clearance check of LED rims against switch and pot bodies" loading="lazy" />
  <figcaption>Clearance check: switch and pot bodies against the LED rims, after moving the LEDs.</figcaption>
</figure>

## Cut the fit strip first

Before cutting a panel, I cut a strip with every hole at a few sizes (70% @ 30 mm/s on clear cast). No kerf compensation was needed: every fit came out right at the drawn size.

| Drawn hole | Part | Result |
|---|---|---|
| 3.0 | 3 mm LED with built-in resistor, from the back | Perfect press fit, rim seated (2.8 and 2.9 too tight) |
| 7.0 | B10K pot bushing (M7) | Fits, barely loose |
| 12.0 | Toggle bushing | Fits with a little play |
| 16.0 | MS25089 thread | Passes, a little play; the flange holds position |
| 17.0 | Toggle nut + washer | Fits inside, sits flat |
| 19.3 hex AF | MS25089 nut, no washer | Captive, excellent fit |
| 2.9 | M3 screw | Passes (2.8 doesn't) |

## Mounting, front to back

- **Toggles and pots:** the body sits against the back of the black base, and the rear nut clamps the base from inside the middle-layer hole. There's no front nut. Tighten the rear nuts *before* the middle layer goes on: switch through the base (pot tab into its slot), then nut, then wrench, then stack the middle and top.
- **MS25089 pushbuttons** are the exception. The flange sits on the face of the top layer, and the nut is captive in the middle layer's hex. Screw the switch in from the front by its collar, snug only.
- **Stack:** 4 x M3 x 12 black pan-heads through the face into 6 mm brass standoffs. Then 4 x M3 x 6 hold the board on the back.
- **LEDs:** press them into the base from behind, lower the board over the leads, then solder.

## Lighting

Green, 3 mm, water-clear, built-in resistor, rated "9–12 V". I measured 12–14 mA each at 12 V, so the panel draws ~0.43–0.50 A. All 36 are in parallel, dimmed by PWM on the high side in the console box ([connector post](/posts/panel-connector-standard/)).

Over the pocketed Milky White middle, the light is even but not blinding. I judged it in a very bright shop, so I expect it to look right in a dark sim room.

<div class="note warn"><b>Light-leak checklist.</b> Black-edge the middle and top layers at every switch hole and around the outside edges and DZUS notches. Build the paint up in several light coats and check it over a bright light. If you see hotspots over the clear LEDs, scuff the LED tips.</div>

## Controls

| Control | Part |
|---|---|
| REFUEL STATUS & INDEXER LIGHTS (DIM–BRT) | B10K pot, 16 mm, M7 bushing, knurled split shaft, with a [printed knob](/posts/printed-dimmer-knob/) |
| WPN STA STATUS LTS (OFF–BRT) | Same |
| NVIS LTS (TOP / ALL / OFF) | ON-OFF-ON hobby toggle, 16 x 30 case |
| HARS/SAS OVERRIDE / NORM | Two-position hobby toggle, same footprint |
| SIGNAL LIGHTS LAMP TEST | MS25089-3G momentary |
| FIRE DETECT BLEED AIR LEAK TEST | MS25089-3G momentary |

Status: the base and middle are cut. The top is painted and curing, with the production job ready. The board coupon is milled. Next post: [the board](/posts/aux-lts-board/).
