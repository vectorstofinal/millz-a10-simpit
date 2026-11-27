---
layout: ../../layouts/Page.astro
title: Seat adjust and emergency brake panel
eyebrow: Panel reference · Left console
lede: The seat height switch and the emergency brake handle, in the forward corner of the left console. This one is still a design. A printed fit mockup has been on the console and the shape has been revised from it; the real panel is not built yet.
description: Seat adjust and emergency brake panel for the Millz A-10C sim cockpit. Shape worked out from reference photos and the sim, 3D model, planned controls and DCS-BIOS mapping.
console: Left console
status: Concept. Fit mockup test-fitted, shape revised.
summary: Seat height toggle on an angled box and the pull-out EMER BRAKE handle beside it. Shape worked out from reference photos and the sim, then revised from a printed fit mockup. Not built yet.
photo: /img/seat-emer-brake-3d-v20.png
---

<figure>
  <a href="/3d/seat-emer-brake/"><img src="/img/seat-emer-brake-3d-v20.png" alt="3D model of the seat adjust and emergency brake corner: a dark box with an angled face carrying the SEAT UP / DOWN / HOLD toggle, and a striped EMER BRAKE handle rising from a small block beside it" width="1206" height="758" /></a>
  <figcaption>The concept model, as revised after the fit mockup. <a href="/3d/seat-emer-brake/">Open it in 3D</a> to turn it around and pull the brake handle.</figcaption>
</figure>

<div class="note">Last updated 2026-11-26. <strong>The real panel is not built yet.</strong> A printed fit mockup has been on the console, and the model shown here includes the changes that came out of it. The mechanism, the wiring and the backlight are not designed.</div>

## At a glance

| Item | Detail |
|---|---|
| Location | Left console, forward corner, directly ahead of the [AUX LTS panel](/panels/aux-lts/) |
| What it is | A flat plate with a box standing on it. The box has one angled face that carries the seat switch. The emergency brake handle slides out of a small block at the foot of the box |
| Starting point | [The Warthog Project](https://thewarthogproject.com) Seat Adjust panel and lower plate, for the plate outline and the switch panel layout |
| Shape | Worked out from reference photos and the cockpit in the sim. [Build post](/posts/seat-emer-brake-shape/) |
| 3D model | [Interactive model](/3d/seat-emer-brake/) |
| Status | Concept. Fit mockup test-fitted, shape revised |

## Controls (planned)

| Control | Part | DCS-BIOS | Notes |
|---|---|---|---|
| SEAT (UP / HOLD / DOWN) | 3-position toggle, ON-OFF-ON, spring return to center from both sides | `SEAT_ADJUST` | Full-size toggle with a printed paddle cap. Not wired yet |
| EMER BRAKE | Pull-out handle | `EMER_BRAKE` | Slides straight out and back, about 32 mm. It will close a switch at one end of its travel. The mechanism is not designed yet |

Control names are from the DCS-BIOS A-10C module. No header pins are assigned yet, so there is no code section on this page.

## The shape

No panel file covers this corner, so the shape comes from references: photos in books and online, and the cockpit in the sim, both in the game and in its model viewer. The [build post](/posts/seat-emer-brake-shape/) has the story.

| Angle | In the jet | In my model |
|---|---|---|
| Seat switch face, from the deck | 63° | 63° |
| Seat switch face, turned toward the pilot | 45° to the aft wall | 45° |
| Aft wall of the box, from the deck | 65° | 65° |
| Top of the box, to the deck | 28° | 28° |
| Brake grip, from its foot | 59° | 65° |
| Brake shaft, to the deck | 25° | 8° |
| Brake handle, side to side | Leans 14° outboard | Upright |
| Brake travel | About 32 mm | 32 mm |

The angles are the part I trust. Sizes are not: none of the references gives a dimension to scale from, so every length is a ratio scaled to a brake grip I assumed to be 78 mm long.

## Where I left the jet behind

- **The brake shaft is shallower.** In the jet it points 25° down into the deck. Here that would put the shaft through the plate. At 8° it stays above it.
- **The handle is upright.** The jet's leans outboard relative to the plate, because it is square to the box and not to the deck. Mine stands square to the plate.
- **The back and the outboard end of the box are my own.** They don't show in any reference I have.
- **The seat panel has an even margin all round.** That means its bottom edge is slanted, not square like the Warthog Project panel it started from.
- **Plain rounded corners on top of the handle.** The references show small chamfers.
- **The handle is taller and more upright.** 12 mm taller, with the grip at 65° to its foot instead of 59°, and a shorter foot. Changed after trying the mockup on the console.
- **Each side of the handle is one flat surface.** The taper runs from the foot's underside to the top, so the handle prints lying on its side.

## What the fit mockup changed

Before designing any mechanism I wanted the shape on the real console. The mockup is three prints: the plate with the box and guide block as one piece, the handle on a slide bar, and a clip that holds the handle at full pull. It screws to the console with M4 screws into heat-set inserts.

Changes made after trying it in place:

- **Plate 9 mm thick instead of 3**, so its top is level with the face of the AUX LTS panel behind it.
- **Mounting screws in pockets**, 12 mm across and 6 mm deep, so the heads sit below the surface.
- **Everything on the plate moved 5 mm forward**, and the plate's aft edge trimmed 4 mm, for room against the AUX LTS panel.
- **Handle and its guide block moved 10 mm outboard.**
- **Handle foot 15 mm shorter**, so the handle sits closer to the box when pushed in.
- **Handle 12 mm taller and more upright.**
- **Slide tunnel a little looser.** The first one moved but was tight.

The first version of the model had the handle touching the AUX LTS panel's NVIS toggle at full pull. With these changes the model shows about 8 mm between them. The nearest control is now that panel's left dimmer knob, about 3 mm from the handle at full pull.

No files here yet. They'll go up once the shape has settled.

## Files

None yet.

## Build log

- [Seat adjust and emergency brake: working out the shape](/posts/seat-emer-brake-shape/) (2026-11-26)
- [A printed deck plate, and the AUX LTS panel in its console](/posts/printed-deck-plate/) (2026-11-25): where the opening for this panel comes from
