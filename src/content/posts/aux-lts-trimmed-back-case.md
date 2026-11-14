---
title: "AUX LTS: corners trimmed, and a back case"
date: 2026-11-14
summary: "The AUX LTS PCB turned out too wide to sit on the console deck's rails, so I notched its four corners to make room for wooden ears under the panel fasteners. The pushbuttons are wired, and the printed back case fits."
tags: [panels, pcb, 3d-printing, console]
hero: /img/aux-lts-trimmed-back.jpg
heroAlt: "Back of the AUX LTS panel: copper PCB with all four corners notched, showing the black base and its fastener holes; pots, toggles and pushbuttons wired"
files: [back-case-stl, back-case-scad, back-case-outline, trim-template]
---

<div class="bottom-line">The AUX LTS PCB is 143 mm wide on a 146 mm panel. That leaves no room for the console deck to hold the panel along its long edges. The fix: wooden "ears" in the deck reach in under the four corner fasteners, and the PCB gets a 12.5 mm notch at each corner to clear them. The panel drops straight in. The PCB is trimmed, the pushbuttons are wired, and the printed back case fits.</div>

Last time the panel was [wired, tested and stacked](/posts/aux-lts-wired-and-stacked/). Then I tried to figure out how it actually mounts in the console, and found the problem.

## Why the PCB needed trimming

The console tops are 11.9 mm birch deck plates with openings in them. Panels lie on top and screw down through their corner fasteners. My PCB hangs 6 mm below the panel's base layer, which puts it right inside the deck's thickness. So the deck opening has to clear the PCB. But a 143 mm PCB under a 146 mm panel leaves only 1.5 mm per side, with nothing left for a rail.

The answer is ears: the opening is the PCB outline plus 2 mm, and four tabs of wood reach in under the corner fasteners, each with a threaded insert. To make room, each PCB corner gets cut back 12.5 mm from the side edge, from the corner to 13 mm past its fastener. On this PCB that's 25 mm long at the pot end and 35 mm at the header end, so the deck ears are about 13.5 mm wide.

<figure>
  <img src="/img/aux-lts-trimmed-back.jpg" alt="Back of the AUX LTS panel: copper PCB with all four corners notched, showing the black base and its fastener holes; pots, toggles and pushbuttons wired; a steel rule across the middle" loading="lazy" width="1600" height="1200" />
  <figcaption>Corners trimmed. The black base shows through at each notch, fastener hole clear. The header is on the right.</figcaption>
</figure>

Only ground copper got cut. I checked the corner areas against the milling file: no traces, pads, LEDs or standoffs within 1 mm of the lines. A 1:1 template is in the files below.

Doing this after the pots and toggles were wired made it awkward. **New rule for the next PCBs:** keep the outline as small and inboard as possible, reaching an edge only where an LED needs it. Then the deck opening can follow the PCB with plenty of room, and the ears become one continuous ledge. The CNC cuts that outline, not me with a hand tool afterwards.

## Pushbuttons wired

The two MS25089 pushbuttons are in and wired, so every hook-up wire on this panel is done.

## The back case

<stl-viewer src="/files/aux-lts-back-case/aux-lts-back-case-v2.stl" label="AUX LTS back case v2" color="#c8ccc8"></stl-viewer>

The PCB is the back of the panel, so I wanted a cover over it. The case:

- **follows the trimmed PCB outline**, inset 0.5 mm, so it clears the deck ears too.
- **is 28 mm deep inside**, enough to clear the toggles and pots.
- **screws to the four standoffs.** An M3×8 replaces each M3×4 PCB screw. The screw goes down a 6.5 mm bore in each column and clamps through a 4 mm base, leaving 0.6 mm between its tip and the M3×12 face screw coming in from the other end of the standoff.
- **has an opening for the header and ribbon**, two banks of vents, and a 40 mm recess for a QR tag that links to this [panel's reference page](/panels/aux-lts/).

It printed floor-down without supports, and it fits.

The OpenSCAD file is generic. For the next panel I only change the block at the top: the outline DXF and layer, standoff positions, header position, depth and where the QR tag goes.

## Next

1. Cut the new deck plate for this part of the console. It holds AUX LTS and the emergency brake panel just ahead of it. Between them the two panels fill the plate almost exactly, so I'm measuring my console before I cut.
2. Plug into the bench rig and fly it against DCS.
