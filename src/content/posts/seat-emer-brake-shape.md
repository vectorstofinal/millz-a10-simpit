---
title: "Seat adjust and emergency brake: working out the shape"
date: 2026-11-26
summary: "The seat switch box and the emergency brake handle are odd shapes that no panel file covers. I worked the shape out from reference photos and the cockpit in the sim. There's now a 3D model and a fit mockup ready to print."
tags: [panels, console, 3d-printing]
hero: /img/seat-emer-brake-3d.png
---

<div class="bottom-line">The forward corner of the left console holds the seat height switch on an angled box and the pull-out emergency brake handle. No panel file covers it, so I worked the shape out from references: photos in books and online, and the cockpit in the sim. The result is a <a href="/3d/seat-emer-brake/">3D model</a>, a <a href="/panels/seat-emer-brake/">page for the panel</a>, and a fit mockup ready to print. Nothing is built yet.</div>

<figure>
  <a href="/3d/seat-emer-brake/"><img src="/img/seat-emer-brake-3d.png" alt="3D model of the seat adjust and emergency brake corner: a dark box with an angled face carrying the SEAT UP / DOWN / HOLD toggle, and a striped EMER BRAKE handle rising from a small block beside it" loading="lazy" width="1206" height="646" /></a>
  <figcaption>Where it ended up. <a href="/3d/seat-emer-brake/">Open the model</a> to turn it around.</figcaption>
</figure>

## An odd-shaped corner

Most panels are flat, and [The Warthog Project](https://thewarthogproject.com) has files for them. This corner isn't flat. The seat switch sits on a slanted face of a box, the box is turned toward the pilot, and the brake handle comes out of a little block at an angle.

## Working from references

There's no drawing of this corner that I know of, so the shape had to come from looking. I used photos in books and online, and the cockpit in the sim, both in the game itself and in its model viewer.

The hard part is angles. Any single picture is a perspective view from wherever the camera happened to be, so an angle read off one picture is wrong by some amount you can't know. One face I first took for a 15° turn is really 45°. It took comparing a lot of views, from different directions, before the angles stopped moving.

## What came out of it

- **The box is a plain rectangular block.** It only looks skewed. In the real airplane this strip of the console is tilted about 15° nose-up and about 13° toward the pilot, and the box stands square to the bulkhead ahead of it, not to the deck it sits on.
- **The seat switch face is one corner of that block cut off at 45°.** It stands 63° up from the deck.
- **The brake handle's grip is 59° from its foot.** I had guessed 60 by eye, which was a nice surprise.
- **The brake shaft points 25° down into the deck.** That one is a problem when the deck is a plate you don't want a hole in.
- **The handle pulls out about 32 mm.**

Sizes are the weak part. None of the references gives a known dimension, so every length is a ratio scaled to a brake grip I assumed to be 78 mm long. Good enough to start with, and the mockup will correct it.

## Where I changed it

I'm building this on a flat plate, with everything square to that plate, so some of the jet's geometry doesn't carry over.

- **Shallower brake shaft.** At the jet's 25° the shaft would go through the plate. I laid it down to 8°, where it stays above the plate for its whole travel.
- **Upright handle.** The jet's leans 14° outboard relative to the plate, because it's square to the box and not to the deck. I made mine stand square to the plate.
- **My own back and outboard end.** Neither shows in any reference I have, so I shaped them to suit the plate.

<figure>
  <img src="/img/seat-emer-brake-3d-side-pulled.png" alt="Side view of the model with the brake handle pulled out: a round shaft spans the gap between the handle's foot and the guide block, and the handle hangs past the aft edge of the plate" loading="lazy" width="1000" height="410" />
  <figcaption>Handle at full pull, seen from the outboard side. The foot ends up well past the edge of the plate, over the next panel.</figcaption>
</figure>

## The catch: the next panel is in the way

The handle's foot hangs out over the [AUX LTS panel](/panels/aux-lts/) behind it, and further when pulled. I checked the model against that panel's controls. The dimmer knobs and pushbuttons are clear. The NVIS toggle is not: at full pull the handle touches its lever, by about 2 mm.

That's a model talking, with a toggle lever height I guessed. So before moving anything I'm going to find out for real.

## Next: a mockup on the real console

The mockup is three prints:

- **The base.** Plate, box and guide block in one piece. It screws to the same inserts the real panel will use, in the opening the [printed deck plate](/posts/printed-deck-plate/) left for it.
- **The handle**, on a flat bar that slides in the guide block.
- **A clip** that holds the handle at full pull, the worst case for clearance.

If the handle hits the toggle, it moves outboard or up. If the box is in the way of anything, I'll see that too. The mechanism, the switch for the brake and the backlight all wait until the shape is right.

Everything known about this panel is on [its own page](/panels/seat-emer-brake/), which will fill in as it gets built.
