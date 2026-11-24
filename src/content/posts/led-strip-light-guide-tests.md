---
title: "A COB strip in a slot: testing a new backlight"
date: 2026-11-23
summary: "The 36 discrete LEDs in the AUX LTS panel leave hotspots I couldn't fully fix. So I tested a 2.7 mm COB strip standing on edge in a laser-cut slot, under five different light-guide pieces. Clear acrylic with a white-then-black painted back won, and that's the plan for the next panel."
tags: [backlighting, acrylic, laser, testing]
hero: /img/led-strip-light-pieces.jpg
heroAlt: "Five test pieces lit green by an LED strip in a slot, each under the same engraved test top: clear bare, clear with a white and black back, milky bare, milky with a white and black back, and milky with a chrome and black back"
files: [strip-coupon-1, strip-coupon-2, strip-coupon-3]
---

<div class="bottom-line">A 2.7 mm wide 12 V COB strip fits on edge in a 1.85 mm laser-cut slot through 3 mm acrylic, and it bends around a 2.5 mm radius with either face out. It draws 11 mA per 25 mm segment. Of five light-guide pieces, clear acrylic with the back painted white and then black lit the legends most evenly. Its only fault was light pouring out of the bare edges, and painting the edges stopped that. Milky acrylic looked the same no matter what I put on its back. The next panel gets the strip. The AUX LTS panel stays as built.</div>

The [AUX LTS panel](/panels/aux-lts/) has 36 discrete 3 mm LEDs behind it, and you can see every one of them as a hotspot. I tried the cheap fixes. White copy paper as a reflector and diffuser helped the most, and scuffing the LED tips helped some. Together they were certainly better, but the hotspots were still there. With LEDs at that spacing under a 3 mm stack, I don't think there's a way to get it fully even.

The idea I wanted to try came from The Viper Project's F-16 panel video: a thin COB LED strip standing on edge in a channel cut through the light-guide layer, snaking past every legend. One continuous line of light instead of 36 points.

## The strip

I ordered three widths to compare. The 2.7 mm one showed up first: green, 12 V, 480 LEDs per meter, sold as a 5 m reel.

| | Measured |
|---|---|
| Width | 2.7 mm |
| Thickness | 1.8 mm, squeezes to about 1.6 |
| Cut interval | 25 mm |
| Current, 3 segments (75 mm) | 0.033 A at 12 V |
| Per segment | 11 mA |
| Per meter | 0.44 A (5.3 W) |
| Whole 5 m reel, fed from one end | 1.1 A |

That last line is worth a second look. The whole reel draws 1.1 A, which works out to half the per-meter current of a short piece. That's voltage drop along 5 m of thin copper. It won't matter for a panel that uses half a meter, but don't size anything from the whole-reel number.

For comparison, the AUX LTS panel's 36 LEDs draw 0.432 A. A panel with 0.6 m of strip would draw about 0.26 A.

## Coupon 1: what slot, and how tight a bend

The first coupon was eight straight slots from 1.0 to 2.4 mm wide, and five U-shaped slots with centerline radii from 10 mm down to 2.5 mm.

- **Slot width:** 1.8 mm as drawn held the strip but was snug. I'm using 1.85 mm.
- **Bend:** the tightest U, 2.5 mm radius, was fine with the lit face on the outside or the inside. That means hairpins on 5 mm centers work.
- **Depth:** my 3 mm sheet measures 2.97 mm, and the 2.7 mm strip sits just under flush. A 3 mm wide strip would have no room at all.

## Coupon 3: five light pieces

Then the real question: what should the layer around the strip be made of? I cut five identical pieces, each with one straight 1.85 mm slot down the middle, open at one end for the wires.

<figure>
  <img src="/img/led-strip-coupon-3.png" alt="Drawing of the light test piece: a 100 by 60 mm rectangle with four corner holes and one 1.85 mm wide slot along the centerline, open to the right edge" loading="lazy" width="878" height="575" />
  <figcaption>The light piece. 100 x 60 mm, one slot, 86 mm long. All three coupon files are in the downloads below.</figcaption>
</figure>

The five:

1. Clear, bare
2. Clear, back painted white and then black
3. Milky (2447), bare
4. Milky, back painted white and then black
5. Milky, back painted chrome and then black

The backs were painted before cutting, so no paint ended up on the slot walls where the light has to get in. The white is the working coat. It scatters light back up toward the legends. The black over it just makes it opaque.

One paint note: black spray went on fine over the white. Over the chrome it did poorly. If you want a black backing on chrome, use tape.

On top of each piece went the same test top: White Matte acrylic painted black, with rows of "LAMP" engraved through the paint every 4 mm out to 24 mm either side of the slot. That shows how fast the light falls off with distance from the strip.

<figure>
  <img src="/img/led-strip-light-pieces.jpg" alt="Five test pieces lit green by an LED strip in a slot, each under the same engraved test top: clear bare, clear with a white and black back, milky bare, milky with a white and black back, and milky with a chrome and black back" loading="lazy" width="1616" height="1176" />
  <figcaption>Same 75 mm of strip moved from piece to piece, same test top, dark room, phone exposure locked. The rows nearest the slot are blown out in every shot, so don't read peak brightness from these.</figcaption>
</figure>

What I saw:

- **Clear, bare:** only the rows right at the slot light up. Almost everything else leaves through the edges. This was the control and it did what a control should.
- **Clear with the white and black back:** a narrow bright core over the slot, and then the outer rows stay lit at a lower but pretty even level all the way out. This was my favorite, except for the light bleeding out of the bare edges.
- **All three milky pieces:** these looked alike. A wider bright band around the slot, then a steady fade to dim at the far rows. Bare, white or chrome on the back made no difference I could see.

That fits how the two materials work. Milky acrylic scatters light everywhere, so the light leaves close to the strip and never gets far enough for the back to matter. Clear acrylic carries light a long way and only lets it out where something scatters it, and the white paint on the back does that across the whole piece.

Every piece shows a brighter line directly over the slot. In the photos it looks harsh. In person it isn't bad. I also tried a sheet of copy paper between the light piece and the top, and it wasn't much help.

## Painting the edges

The answer to the bleed was the obvious one. I painted the edges of the clear white-backed piece, and that stopped it. That's the combination I'm going with: clear, white and then black on the back, and the same on the edges.

One leak is left. If the top layer doesn't perfectly cover the light guide, light still gets out toward the pilot around the top's edge. On a real panel I'll put the whole stack together, light it up, and hand paint wherever it shows.

## The process for a real panel

1. Peel the masking off one face of a clear sheet. Leave the other face masked.
2. Spray the bare face white, then black. Let it cure.
3. Laser-cut the whole layer, painted face down: outline, holes and the strip slot.
4. Tape over the slot, and over any holes where I want the glow. I like the glow around the toggle switches.
5. Spray the edges white, then black.
6. Peel the front masking and the tape last.

The front masking stays on the whole time. It protects the face the light comes out of, from the laser smoke and from the edge spray.

The coupon file is set to 30 mm/s at 70%, my 3 mm cut setting from the [settings page](/settings/).

## What I haven't done yet

- No real panel has been built this way. This is all coupons.
- I haven't measured the falloff, only looked at it. My photos clipped the brightest rows.
- Spray solvents can craze laser-cut acrylic edges. My edge-painted coupon needs a look after the paint has had a day or more.
- The 3 mm and 2 mm wide strips are untested.

The AUX LTS panel stays the way it is, discrete LEDs and all. The next panel gets a slot.
