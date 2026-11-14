---
title: "AUX LTS PCB: milled, and how 36 LEDs get in"
date: 2026-11-11
summary: "The AUX LTS PCB came off the router today. The isolation recipe that finally stopped breaking 0.4 mm end mills, what the test coupon proved, a screw-length trap in the standoffs, and a way to load 36 LEDs without threading 72 leads at once."
tags: [pcb, cnc, feeds-and-speeds, backlighting]
hero: /img/aux-lts-pcb-over-base.png
heroAlt: "AUX LTS PCB outline over the black base layer, back view, with the 36 LED positions and 8 test LEDs marked"
files: [pcb-vcarve, pcb-cnc]
---

<div class="bottom-line">The AUX LTS PCB is fully milled. The 0.4 mm isolation pass works at 24,000 rpm, 120 mm/min and only 0.08 mm deep, with Z0 set by a scratch touch-off. Deeper than that, I broke three end mills. On the dry fit, check your screw lengths: M3×12 through the panel plus M3×6 through the PCB is 7.4 mm of thread in a 6 mm standoff. For the LEDs, load them into the PCB one at a time and let the PCB carry them into the base.</div>

This picks up from [milling the AUX LTS board](/posts/aux-lts-board/), where only the first coupon was done. Since then I've settled on calling it the **PCB**, so that's what it is from here on.

## What the coupon proved

The coupon sits next to the PCB on the same blank, so it runs with the same zero and the same tools. All five layers passed:

- **Isolation (C1):** clean. Z0 needed to go a hair lower than I first set it by eye.
- **1.0 mm drill (C2):** every hole through. The extra-long header pins fit perfectly, and continuity checked good.
- **Standoff hole, pot and toggle cutouts (C3–C5):** all cut with the 1/8 in end mill in one file. The toggle body and the pot body both drop in easily.

That last one corrects my earlier post. I worried that the 1/8 in end mill's 1.59 mm corner radius would catch the sharp corners of the 16 × 30 mm toggle case and that I'd need a 1/16 in tool. It doesn't. The 1/8 in end mill is fine for the cutouts.

## Three broken end mills, then the recipe that works

The first PCB isolation pass didn't go smoothly. I broke three 0.4 mm end mills: one right after the coupon, then two mid-trace on the PCB itself.

| Attempt | RPM | Feed | Depth | Z0 | Result |
|---|---|---|---|---|---|
| 1 | 20k | 200 mm/min | 0.12 mm | by eye | broke mid-trace |
| 2 | 24k | 120 mm/min | 0.10 mm | by eye | broke mid-trace |
| 3 | **24k** | **120 mm/min** | **0.08 mm** | **scratch touch-off** | **finished the PCB** |

Both PCB breaks happened mid-trace, not on a plunge. That points to side load: too deep in the fiberglass, with a by-eye zero probably running deeper still in places. In my first post I said to err deep, because a flat end mill's groove doesn't get wider with depth. That's true for the groove, but a 0.4 mm end mill can't take the extra load. Shallow and accurately zeroed is what works.

1 oz copper is about 0.035 mm thick, so 0.08 mm still leaves about 0.045 mm of margin for the blank not being perfectly flat. I wouldn't go below 0.06 mm.

**Scratch touch-off:** spindle on, jog down 0.01 mm at a time over scrap copper until the bit just leaves a bright mark, and set Z0 there. My electrical XYZ probe still won't trigger on a fresh 0.4 mm end mill. Oddly, it did trigger on one that had already cut for a while, as if the tip coating had worn through. The 1.0 mm drill and the 1/8 in end mill have plenty of depth margin, so the probe is fine for those.

The full table is on the [settings page](/settings/#cnc-router-pcb-isolation-milling).

## Checking the stack before any solder

Before soldering anything, I checked the PCB against the acrylic layers in the DXF files. I mapped the PCB coordinates onto the panel through the four standoff holes:

- **All 36 LED lead pairs** sit centered on the base's LED holes, within 0.01 mm. The standoff holes match exactly, and the diffuser's LED pockets are within 0.09 mm.
- **Every LED faces the same way:** anode (long lead) toward the panel's straight top edge. So there's no polarity map to follow.
- **Eight LEDs are tight:** their rims sit 0.5 mm from a switch body (four around the HARS/SAS toggle, two on each pot). The next closest is almost 3 mm. Those eight are the test LEDs for the dry fit.

<figure>
  <img src="/img/aux-lts-pcb-over-base.png" alt="Back view of the black base with the PCB outline overlaid: standoffs, cutouts, header, 36 LEDs with anode direction ticks and the 8 tight-clearance test LEDs circled" loading="lazy" />
  <figcaption>Back view, as it sits on the bench: black base, PCB outline in copper, cutouts dashed. Yellow ticks show which way each LED's anode lead goes (all up). Orange rings are the 8 LEDs with 0.5 mm to a switch body.</figcaption>
</figure>

### The screw-length trap

The plan was M3×12 pan-heads through the face into 6 mm female-female standoffs, and M3×6 holding the PCB on the back. Do the arithmetic:

- The three acrylic layers are 9.0 mm, so the M3×12 leaves **3.0 mm** of thread in the standoff.
- The PCB is 1.6 mm, so the M3×6 leaves **4.4 mm**.
- That's **7.4 mm of thread in a 6 mm standoff.** The screws meet in the middle and one of the two parts never clamps.

The fix is **M3×4 on the PCB** (2.4 mm in, leaving a 0.6 mm gap) or M3×5 with a 1 mm washer.

<figure>
  <img src="/img/aux-lts-stack-section.png" alt="Cross-section of the AUX LTS stack to scale: standoff with the screw overlap highlighted, an LED seated in the base, a toggle with its nut in the middle layer, and the header in the 6 mm gap" loading="lazy" />
  <figcaption>The stack, depth to scale. The red band is where the M3×12 and M3×6 overlap. The header's plastic strip and short pins leave 0.5 mm to the base.</figcaption>
</figure>

## Getting 36 LEDs in without a nightmare

The LEDs press-fit into 3.0 mm holes in the black base, rim on the back. That rim is what sets their height: the tip always ends up 1.0 mm into its pocket in the diffuser layer, wherever the PCB sits. So the base is the height jig. The question is how to get the PCB onto 36 of them.

What I didn't want to do:

- **Press all 36 into the base, then lower the PCB over them.** That's 72 leads into 1.0 mm holes at once, and the ones in the middle are unreachable.
- **Solder them to the PCB first at a measured height.** Every joint would have to be exact, and then 36 tips would have to press into the base together.

What works: **load the PCB first.** Each LED goes into the PCB from the front, one at a time, with the long lead in the ringed LED+ hole, hanging about 8 mm. I bend the leads out slightly on the copper side, just enough to hold it. Then the PCB comes down onto the base:

- A domed 3 mm tip finds a 3 mm hole much more easily than a thin lead finds a 1 mm one.
- As the PCB settles onto the standoffs, the slightly bent leads drag each rim onto the base, and the leads slide up through the PCB.
- Any LED that didn't seat has its leads standing taller than the rest. Pliers at the copper push it home.

I tried it with three LEDs in the press-fit holes as cut, and it worked.

<figure>
  <img src="/img/aux-lts-led-seating.png" alt="Seating fixture: middle layer face up on foam, base face down on it, LEDs hanging from the PCB on bent leads and seated in the base; one unseated LED's leads stand taller" loading="lazy" />
  <figcaption>The fixture: the diffuser layer on a foam pad, pocket side up, so every LED tip has somewhere to go, and the base face-down on top of it. The highlighted LED hasn't seated, and its leads give it away.</figcaption>
</figure>

If the press fit ever fights back, the fallback is opening the base holes to a slip fit with a 1/8 in (3.175 mm) drill in a pin vise.

## Next

1. Bare-PCB meter checks. LED+ to ground must read open.
2. Dry fit with the 8 test LEDs.
3. Solder the ground links and the header.
4. Load and solder all 36 LEDs, then the switch wiring.

First light after that.
