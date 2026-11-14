---
title: "AUX LTS: first light"
date: 2026-11-13
summary: "All 36 LEDs on the AUX LTS PCB are soldered and lit: 0.432 A at 12 V, 12 mA each. Ground links made from LED lead offcuts, a printed gauge for trimming header pins, what made the soldering hard, and what I'm changing on the next PCB."
tags: [pcb, backlighting, electronics, soldering]
hero: /img/aux-lts-first-light.jpg
heroAlt: "AUX LTS black base layer with all 36 green LEDs lit, two toggles and two pots fitted"
files: [trim-gauge-stl, shroud-stl]
---

<div class="bottom-line">All 36 LEDs on the AUX LTS PCB light up and draw 0.432 A at 12 V, exactly 12.0 mA each. The two ground links are staples bent from LED lead offcuts. The header pins came out 12 mm long and I cut them to 8 mm with a printed gauge. Soldering a mask-free milled PCB with 0.4 mm gaps was the hard part. Minimal solder fixed most of the bridging, and the next PCB gets more room.</div>

This picks up from [AUX LTS PCB: milled](/posts/aux-lts-pcb-milled/). The PCB is milled, the LED loading method is proven, and now it gets parts.

## Ground links: two staples

The traces split the PCB's ground copper into three separate areas. Only the center one reaches the header's main ground pins (3–8). The outer area has 17 LEDs and no header ground pin at all, so without a link, those 17 stay dark.

<figure>
  <img src="/img/aux-lts-gnd-links.png" alt="Copper side of the AUX LTS PCB: outer, center and HARS/SAS ground areas in three colors, LED+ trace in yellow, two link positions circled" loading="lazy" />
  <figcaption>Copper side up. The three ground areas: outer (17 LEDs, 3 standoffs), center (13 LEDs, header ground pins 3–8) and HARS/SAS (6 LEDs, header pin 21). Link 2 crosses the LED+ trace; Link 1 crosses a switch signal trace.</figcaption>
</figure>

Each link is a U bent from a trimmed LED lead, with legs 3.8 mm apart. It goes in from the front and is soldered to the ground copper on each side. Bare wire is fine, because a single-sided PCB has no copper on the front. Fit the links before the PCB goes onto the base, since the front is covered once it's mounted.

<figure>
  <img src="/img/aux-lts-gnd-staple.png" alt="Side view of a ground link staple: lying on the bare front, legs through two holes, soldered to the ground pour either side of a trace" loading="lazy" />
</figure>

To check: header pin 3 should beep to the top standoff pads (through Link 2) and to pin 21 (through Link 1).

## The header: 12 mm of pin is too much

I planned on a header that leaves about 7.9 mm of pin above the copper. The one I used left **12 mm**. The printed shroud is 9 mm tall, and the ribbon socket wants the pins about 1 mm below its rim. At 12 mm, the socket hits the tips before it seats.

So I printed a gauge. It's a ring the same footprint as the shroud, but 8.0 mm tall (it printed at almost exactly 8.0). Slide it over the pins, rest the flat face of the flush cutters on top, cut, and file off the burrs. Hold a finger over each pin, because the offcuts fly.

<stl-viewer src="/files/aux-lts-pcb/header-pin-trim-gauge-8mm.stl" label="Header pin trim gauge, 8 mm"></stl-viewer>

I only soldered the 16 pins that carry a connection. The other 10 sit on tiny isolated copper rings that go nowhere. Leaving them bare costs nothing (16 joints hold the header fine) and avoids the tightest gap on the PCB, 0.475 mm next to unused pin 10. The ribbon cable checked out end to end from the far connector.

## Soldering without a solder mask

This pushed my skills. The milled gaps are 0.4–0.5 mm and there's no solder mask, so solder goes wherever it wants. What worked:

- **Less solder.** I used 0.5 mm wire and fed only 2–3 mm per joint. That fixed most of the bridging.
- **Backlight to find bridges.** Hold the PCB up to a bright light. FR-4 glows wherever the copper was milled away, so a bridge shows up as a dark blob across a bright line.
- **Fix bridges with flux and a clean tip** dragged through the gap. Use solder wick for the big ones. Run a hobby knife along the groove afterwards.
- **A hotter iron on the ground joints.** The cathodes sit right in the ground copper, which soaks up heat. A cool iron makes the solder ball up and sag across the gap.
- **Meter it the right way round.** Once LEDs are in, LED+ to ground conducts one way. Check with red on ground and black on LED+: that must read open. I checked every five or so LEDs, so a bridge never had many joints to hide behind.

## Loading 36 LEDs

Each LED went into the PCB first, one at a time, and the PCB carried them down into the base's 3.0 mm press-fit holes, as described in the [last post](/posts/aux-lts-pcb-milled/#getting-36-leds-in-without-a-nightmare).

At about 20 LEDs I took the PCB back off to see if the press fit was going to lock it on for good. It was a tiny bit snug pulling apart and went back together easily. The LEDs actually helped it line up. So the holes stay as cut.

## First light

<figure>
  <img src="/img/aux-lts-first-light.jpg" alt="AUX LTS black base layer on the bench with all 36 green LEDs lit through their holes. Two toggles and two pots are fitted; the two pushbutton holes are still empty." loading="lazy" width="1600" height="1237" />
  <figcaption>The lighting test: the black base with the PCB behind it, 12 V in, all 36 LEDs lit. The toggles and pots are fitted; the two pushbuttons, the diffuser and the painted top come next. Shot with the panel upside down.</figcaption>
</figure>

| LEDs soldered | Current at 12 V | Per LED |
|---|---|---|
| 20 | 240 mA | 12.0 mA |
| 36 | 432 mA | 12.0 mA |

My first reading on the full set was 0.333 A. That was the bench supply sitting in current limit from the half-way test. It dropped its voltage and dimmed every LED. With the limit raised to 0.6 A, the reading was 0.432 A, exactly 36 × 12.0 mA.

## What changes on the next PCB

- **Thermal reliefs** on every pad in the ground copper: a ring cut around the pad, joined back by four thin spokes. The pad heats like any other.
- **At least 0.8 mm of clearance around every pad**, with the copper cleared away from the header area completely.
- **Spread out the header pins.** Panels that don't need many connections won't use pins sitting next to each other. A checkerboard pattern on the 26-pin standard still gives 7 inputs.
- **A pin 1 marker**, milled into the copper and laser-marked on the front.
- **Every ground link at the same 3.8 mm span**, so one staple size fits all.

Next: wiring the switches and pots, gluing the shroud, the full acrylic stack, and the first test against DCS on the bench rig.
