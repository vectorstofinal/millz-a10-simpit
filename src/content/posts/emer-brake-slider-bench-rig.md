---
title: "Emergency brake handle: the mockup fits, and a slider that snaps with magnets"
date: 2026-11-27
summary: "The printed fit mockup went on the console and the shape is settled. Next was the part that moves: a slide bar held at each end by pairs of small magnets, with a microswitch at full pull. It works on a bench rig. One dead switch along the way."
tags: [panels, 3d-printing, testing, console]
files: [emer-brake-rig-base, emer-brake-rig-plate, emer-brake-rig-sled, emer-brake-rig-handle]
hero: /img/emer-brake-slider-mechanism.png
heroAlt: "Diagram of the emergency brake slider, pushed in and pulled out"
---

<div class="bottom-line">The fit mockup of the <a href="/panels/seat-emer-brake/">seat adjust and emergency brake panel</a> has been on the console and the shape is settled. The handle's slide now has a mechanism: pairs of 6 x 3 mm magnets hold it pushed in or pulled out, and a microswitch gets pressed at full pull. I built it as a small bench rig first and it works. The rig files are below. One microswitch did not survive.</div>

## The mockup did its job

The mockup is the whole corner as cheap prints: plate, box and guide block in one piece, plus the handle on a slide bar. It went on the console and showed what had to move. The list of changes is on the [panel page](/panels/seat-emer-brake/), and the [3D model](/3d/seat-emer-brake/) has them all. The last print of it fits.

## A flat bar where the jet has a round shaft

In the references the brake handle's shaft is round where it goes into the box. For the mockup I used a flat bar, 14.5 x 10 mm, only because it prints lying down. It turned out to be the better part: it can't rotate, so the handle stays upright with no extra parts, and by now I know what fit it needs. You only see the difference when the handle is pulled. I'm keeping it.

## Getting the slide fit right

The bar runs in a printed tunnel. Three tries:

| Tunnel (mm) | Result |
|---|---|
| 15.0 x 10.5 | Moves, but tight |
| 15.3 x 10.8 | Free, but too much slop side to side |
| 15.0 x 10.8 | Good |

So about 0.25 mm per side is right across the width, and the height wanted more than I gave it at first. The tunnel's roof is a printed bridge.

## How it holds and how it switches

<figure>
  <img src="/img/emer-brake-slider-mechanism.png" alt="Side view diagram of the slider in two positions. Pushed in, a plate on the end of the slide bar sits against magnets in a home wall. Pulled out 32 mm, the same plate sits against magnets in the end of the guide sleeve and presses the lever of a microswitch." loading="lazy" width="1360" height="900" />
  <figcaption>The slider from the side. The plate on the end of the bar is the only moving part besides the handle.</figcaption>
</figure>

The real handle has two positions, in and out, and stays where you put it. So there is no spring. Magnets hold it at both ends.

- **A stop plate on the end of the bar.** It screws to the bar's tip, inside the box, and it's what keeps the handle from pulling all the way out. Two magnets sit in holes that go right through it, so they work from both faces.
- **Two more magnets at each end of the travel.** One pair in the inner end of the guide sleeve, one pair in a wall at the far end. Pushed in, the plate snaps to the wall. Pulled out 32 mm, it snaps to the sleeve.
- **All six magnets have to face the same way.** The easy way to keep track is to stack them, mark the same face on each, and put every mark toward the same end.
- **The fixed magnets sit 0.2 mm below their surfaces**, so the plate lands on plastic and the magnets never quite touch. I wanted it to really grab, which is why there are two at each stop and not one.
- **The switch is wiped, not hit.** An ear on the plate slides along the microswitch's lever, starting from the hinge end, and pushes it down over the last part of the pull. The handle can't crush the switch by going too far, because it never pushes straight at it.

In DCS-BIOS the handle is `EMER_BRAKE`, a plain two-position switch, so one input is all it needs. That part isn't wired yet.

## The bench rig

Before building this into the real box I made it as a rig: a base with the guide sleeve, the far wall and a deck for the switch. It uses the mockup handle. The switch lies on a small sled with two pegs through its mounting holes, and the sled has slots so the switch can be slid toward the plate until it clicks just before full pull.

<stl-viewer src="/files/emer-brake-rig/emer-brake-rig-base-v2.stl" label="Bench rig base" color="#c8ccc8"></stl-viewer>

It works. The handle snaps in at one end and snaps out at the other.

Hardware: six 6 x 3 mm disc magnets, one M3 x 10 screw for the plate, two M3 x 8 screws with washers for the sled, and a subminiature lever microswitch (the common 20 x 10 x 6.4 mm size, mounting holes 9.5 mm apart). The screws cut their own threads in printed pilot holes.

## What went wrong

- **A rib in the wrong place.** The first sled had a fence along the back of the switch. That is exactly where the terminals come out. Gone in the next version.
- **Glue killed a switch.** The pegs had a little play in the switch's holes, so I glued the switch down. After that it still clicked, but the meter showed no connection on any pair of terminals. My guess is that glue, or its fumes, got inside. That switch is scrap.
- **The fix is pegs that fit.** They're now 2.35 mm for the 2.4 mm holes, a little wider at the root, so the switch presses on and stays without glue.

Two things I'd tell myself: no super glue anywhere near a small switch, and check a switch with the meter before mounting it, so you know it was good going in.

## Where it stands

- **Works on the bench:** slide fit, the snap at both ends, the stop.
- **Not done:** I haven't confirmed the replacement switch electrically on the rig yet.
- **Next:** build the same parts into the real box, which has the slide at 8° instead of level and has to stay open underneath so I can get at all this.
