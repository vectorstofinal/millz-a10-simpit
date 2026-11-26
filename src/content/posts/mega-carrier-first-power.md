---
title: "The carrier PCBs work, and the panel that was a mirror image"
date: 2026-11-25
summary: "The five Mega carrier PCBs arrived. The first one passed its power checks, dims the AUX LTS backlight from the Mega, and now runs the panel in DCS, lighting included. In between, DCS saw nothing at all, and the fault turned out to be in the panel, not the carrier: its ribbon header is numbered as a mirror image. A second cable and a spare pin header fixed it."
tags: [controller-box, pcb, electronics, testing, dcs-bios, aux-lts]
files: [carrier-socket-scan-ino, aux-lts-dcsbios-ino, led-dim-play-ino]
hero: /img/mega-carrier-first-bench.jpg
---

<div class="bottom-line">The first <a href="/controller-box/mega-carrier/">Mega carrier PCB</a> works. It passed every power check, lit the AUX LTS panel at 0.435 A, dims it from the Mega, and runs both toggles, both pushbuttons and both knobs in DCS. Getting there took a detour. With the panel plugged straight in, DCS saw nothing. A small scanner sketch showed every control arriving one pin off, and the cause was the panel's own PCB: I had numbered its ribbon header from the wrong side, so it's a mirror image of the standard. Two stock ribbon cables with a long pin header between them swap the rows back. The carrier itself needed no changes.</div>

## What arrived, and what I soldered

Five assembled PCBs from JLCPCB, with all 98 surface-mount parts and the five connectors on them. The fab's final placement drawing matched the preview I'd checked before paying.

Two things were mine to add:

- **The Mega's header pins**, 86 of them. The trick is to push the pin strips into the Mega first and lower the carrier onto them, so the Mega holds everything square while you solder from the top. Tack one pin per strip, check it sits flat, then do the rest.
- **The MASTER jumper**, a solder blob across JP5, so this carrier's Mega can drive the dimmers.

The RJ45 jacks I'd ordered fit their footprint. A single carrier doesn't need them; they come into play when two carriers are linked.

## Power checks, before a Mega went near it

I ran these on the bare PCB with a current-limited bench supply. They're the first half of the checklist on the [reference page](/controller-box/mega-carrier/#first-article-checklist).

| Check | Result |
|---|---|
| Wired 12 V plug, nothing powered: "+" reaches the main fuse, "−" reaches a socket ground | Pass |
| 12 V on, current limit low: rail present at all four socket fuses, nothing warm, LED pins at 0 V | Pass |
| Ground pins on every socket | Pass |
| 12 V off, 5 V on the dim line: the gate drivers' inputs read 0 V | Pass |
| 12 V on, 5 V on the dim line: the LED pins of all four sockets go to 12 V | Pass |
| AUX LTS panel on socket 1, 5 V on the dim line | All 36 LEDs lit, **0.435 A** |

That fourth line is the fault the design reviews were about: a dimming signal arriving while the 12 V is off. The input guards did their job.

<figure>
  <img src="/img/mega-carrier-first-bench.jpg" alt="A green carrier PCB on a blue bench mat with no Arduino under it, joined by a gray ribbon cable to the AUX LTS panel, whose legends are lit green. Jumper wires run from a small breadboard to the carrier's 12 V plug and dim line. The bench supply behind shows 12.00 V and 0.452 A on channel 1 and 5.00 V on channel 3" loading="lazy" width="1600" height="1200" />
  <figcaption>A carrier with no Mega under it, lighting the AUX LTS panel from socket 1. The bench supply's third channel holds the dim line at 5 V through the breadboard. The display reads 12 V and 0.45 A.</figcaption>
</figure>

The expected current for this panel is 0.432 A, so the dimmer is passing everything the LEDs want.

Two bench notes for next time. My supply's channels are isolated from each other, so the 5 V channel needed its negative terminal jumpered to the 12 V channel's before any reading meant anything. And with no Mega fitted, the top pad of J8 is the dim line.

## Dimming from the Mega

With the pins on and JP5 bridged, I uploaded the dimming test sketch from the [breadboard post](/posts/led-dimming-bench-test/), updated for the carrier. The panel dims from the Serial Monitor, this time through the real driver and with no jumper wires.

## Then DCS saw nothing

The DCS-BIOS sketch uploaded, the Bridge connected, and not one switch or knob did anything.

The dimming test had just proven the Mega, the USB link and the upload path, so it had to be the pins. So I made the tool I should have had from the start: a sketch that watches every Mega pin that goes to a socket and prints which one moved.

```
D22 LOW    (design: socket S1, ribbon pin 11)
D22 HIGH   (design: socket S1, ribbon pin 11)
D25 LOW    (design: socket S1, ribbon pin 14)
D25 HIGH   (design: socket S1, ribbon pin 14)
D27 LOW    (design: socket S1, ribbon pin 16)
```

Every control was there, and every one was on the pin next to where it belonged.

| Control | Should move | Moved |
|---|---|---|
| FIRE DETECT TEST | D23 | D22 |
| SIGNAL LIGHTS TEST | D24 | D25 |
| NVIS at TOP | D26 | D27 |
| NVIS at OFF | D28 | D29 |
| HARS/SAS | D30 | D31 |

## The mirror

The carrier was right. The panel was wrong, and had been since I drew its PCB.

The AUX LTS header has its plastic strip on the front of the PCB, and the ribbon plugs onto the long pins sticking out the back. I numbered the pins as they look from the front. A cable coming at them from the back sees the two rows exchanged: pin 1 where pin 2 should be, 3 where 4 should be, all the way along.

It never showed on the breadboard, because every jumper there was placed by what the pin did. I found "the pin that goes low when I press FIRE DETECT" and wired that. The pin's number never came into it.

It also hid behind the backlight. Pins 1 and 2 are both LED power and pins 3 to 6 are all ground, so those swap onto each other and the panel lights up as if nothing were wrong.

The pair that matters is 7 and 8. Pin 7 is the 5 V feed for the knobs and pin 8 is a ground. Swapped, the knobs get no 5 V, and the carrier's 5 V lands on the panel's ground. That is a dead short, and it sat there for about half an hour while I chased the problem. Each socket's 5 V pin has its own small resettable fuse, and that fuse held the short the whole time. Nothing was damaged.

## The fix: two cables and a pin header

No firmware can fix a swapped power pin. But the same mistake, made a second time on purpose, cancels the first.

<figure>
  <img src="/img/ribbon-row-swap-join.png" alt="Two ribbon cable sockets facing each other with a double-ended pin header between them. The left socket has its key bump up and the right one has its key bump down. The upper pin joins hole 1 of the left socket to hole 2 of the right socket, and the lower pin joins hole 2 to hole 1" loading="lazy" width="1600" height="760" />
  <figcaption>Two sockets on one pin header, seen from the red-stripe edge. The pins run straight through, but the second socket is turned around to face the first, so its rows sit the other way up.</figcaption>
</figure>

A long pin header with a ribbon socket pushed onto each side swaps the rows, the same way the panel does. So the cable is now two ordinary 50 cm ribbons joined by a spare long header, with its plastic strip slid to the middle so both sides have enough pin.

Getting the join right is easy to check. Put the red stripes at the same end, and the key bumps on the two sockets end up on opposite faces. If both bumps face the same way, one cable is on backward.

Before any 12 V went back on, the scanner confirmed it on USB power alone: D23, D24, D26, D28, D30, and both knobs sweeping on A1 and A3. Exactly the design.

## And then it worked

The DCS-BIOS sketch, now version 4 with the carrier's pin numbers, went back on. Everything on the panel now follows in the cockpit.

That is the first panel running the way the whole cockpit is meant to: one ribbon into a socket, and the box's Mega talking to DCS.

**Later the same day:** the backlight and DCS now run from one sketch. A spare knob wired to socket 3 stands in for the console lighting knob. It dims the real LEDs directly, and the same reading goes to DCS, so the console-lights knob in the virtual cockpit turns with it. The first try had the sim knob jumping to the opposite end at each end of the travel. I had told DCS-BIOS the knob's range was a little narrower than it really is, and the library wraps readings that fall outside the range instead of stopping at the end. Giving it the full range fixed it.

## What I'm taking from it

- **Number a connector from the side the cable plugs into.** Every other panel PCB I've drawn gets checked for this before it's milled.
- **Jumper wires hide numbering mistakes.** A breadboard test proves the circuit. It doesn't prove the pinout.
- **Scan first.** A sketch that prints which pin moved would have found this in one minute. It's now the first thing any new panel or socket gets.
- **Small fuses on small rails.** The 100 mA fuse on each socket's 5 V pin came out of a design review. It paid for itself in the first hour.

## Not done yet

- Sockets 2, 3 and 4 with the scanner.
- The scope test for the moment the 12 V is pulled while the dim line is live.
- Pot noise with the backlight dimmed, and the driver's switching delay, to compare with the breadboard numbers.
- A second carrier with RJ45 jacks, to try the dim bus.
- A permanent answer for the AUX LTS header: keep the adapter, make a small row-swap PCB, or re-mill the panel's PCB.

What each part of the carrier is for is in [a tour of the PCB](/posts/mega-carrier-tour/).
