---
title: "Backlight dimming on a breadboard: what the bench says"
date: 2026-11-16
summary: "While the controller-box PCBs are being made, I built the interim dimmer on a breadboard and ran the AUX LTS backlight through it. It dims cleanly at 20 kHz, one ground wire is nowhere near enough, and a phone camera sees no banding at any frequency."
tags: [controller-box, backlighting, electronics, arduino, testing]
hero: /img/led-dimming-bench.jpg
heroAlt: "AUX LTS panel lit green on the bench, connected by ribbon cable to an Arduino Mega and a breadboard with a TIP42C dimmer and a knob, with the bench supply showing 12.00 V and 0.424 A"
files: [led-dim-play-ino, led-dimmer-breadboard]
---

<div class="bottom-line">The backlight scheme from the <a href="/posts/panel-connector-standard/">connector standard</a> holds up on real hardware. A two-transistor dimmer on a breadboard takes the 36 AUX LTS LEDs from full (0.45 A at 12 V) down to barely lit, at 20 kHz, with no dropout. Sharing the ribbon ground with LED current works, but only with all the ground pins connected: one wire threw the pot readings off by 3 to 6%, five wires cut that sixfold. No whine at any frequency and no banding on a phone camera. The minimum brightness gets picked later, in the dark, in the pit.</div>

The carrier PCBs for the controller box are on order. Until they show up I wanted to play with LED power and dimming, because the connector standard makes two bets I had never tested: that the LEDs can be PWM-dimmed on the high side from the box, and that the LED current can come home on the same ribbon grounds the pots use.

<figure>
  <img src="/img/led-dimming-bench.jpg" alt="AUX LTS panel lit green on the bench, connected by ribbon cable to an Arduino Mega and a breadboard with a TIP42C dimmer and a knob, with the bench supply showing 12.00 V and 0.424 A" loading="lazy" width="1600" height="1200" />
  <figcaption>The whole test: panel, ribbon, Mega, and the dimmer and knob on the breadboard. The supply is showing 12 V and 0.42 A.</figcaption>
</figure>

## The circuit

This is the interim dimmer from my parts kit: a TIP42C switching the 12 V, with a 2N7000 pulling its base down. The Mega drives the 2N7000 from D12, because D12 is the pin the carrier uses for its dimming output.

<figure>
  <img src="/img/led-dimmer-breadboard.png" alt="Breadboard layout: TIP42C in row c at columns 12 to 14, 2N7000 in row h at columns 10 to 12, four resistors, top plus rail is 12 V and bottom minus rail is ground" loading="lazy" width="1360" height="1076" />
  <figcaption>The layout I worked from, seen from the row-j edge. Top + rail is 12 V, bottom − rail is ground. The knob went on the same breadboard later, off to one side.</figcaption>
</figure>

| Part | Holes |
|---|---|
| TIP42C, label toward you | c12 base, c13 collector, c14 emitter |
| 2N7000, flat toward you | h10 source, h11 gate, h12 drain |
| 10 k | top + rail to a12 |
| 680 Ω | d12 to g12, across the channel |
| 100 Ω | f7 to f11, then j7 to Mega D12 |
| 100 k | j11 to bottom − rail |
| Jumpers | a14 to top + rail, j10 to bottom − rail |

LED+ leaves from a13 and b13 to ribbon pins 1 and 2. The supply, the Mega's ground and the ribbon grounds all land on the bottom − rail.

## First power, no code

The 100 k holds the 2N7000 off, so with the Mega unplugged the LEDs have to stay dark. They did: 0 A. Then I touched the D12 wire to 5 V. All 36 lit, 0.45 A. That's the 0.432 A I measured at [first light](/posts/aux-lts-first-light/) plus about 17 mA of base drive through the 680 Ω.

## A sketch to play with

The test sketch has no DCS-BIOS in it. Everything is a command typed into the serial monitor: `d 25` for 25% duty, `f 20000` for 20 kHz, `t 1` for the shortest pulse the timer can make, `g` for the curve, `k` to hand control to a knob, and `n` for a pot-noise test. It's in the downloads below.

## Measuring switching delay without a scope

At 1 kHz the supply current tracked the duty: 0.13 A at 25%, 0.25 A at 50%, 0.35 A at 75%, 0.44 A at 100%. The middle readings ran a little high, and my first guess was a slow transistor.

There's a way to check that with just the supply's ammeter. Hold the duty at 25% and raise the frequency. A transistor that turns off late adds the same few microseconds to every pulse, so the faster the pulses come, the more extra current you see.

| PWM frequency | Supply current at 25% |
|---|---|
| 1 kHz | 0.13 A |
| 5 kHz | 0.125 A |
| 20 kHz | 0.145 A |

Nothing moved between 1 and 5 kHz, so the excess there was the meter averaging a pulsed load. From 5 to 20 kHz it rose 0.020 A, which works out to about 3 microseconds of late turn-off. At 20 kHz the shortest pulses pick up less than that: 1% duty drew 0.015 A and 5% drew 0.045 A.

I expected a plain bipolar transistor to give up above a few kHz. It didn't. It dims evenly at 20 kHz all the way down to the shortest pulse, where the LEDs are barely lit.

## How dim is dim enough

I can't answer that on a bright bench. At 1 kHz and 0.2% duty the LEDs are almost invisible in daylight, and the pit will be a much darker room. So the minimum brightness, the maximum and the curve all stay adjustable in the firmware, and I'll set them from the seat with the lights off.

A side idea from this: any knob in the cockpit that DCS doesn't actually use can be put to work for things like this. A spare knob for minimum backlight brightness, for example.

## One ground wire is not enough

This was the test I cared most about. The pots on the panel and the LED current share the ribbon's ground wires. If the LED current lifts the panel's ground, every pot reads high.

The sketch reads both panel pots with the LEDs off, full on, and at 50% PWM. Both knobs were parked at mid-travel (about 505 of 1023).

| Ribbon grounds connected | Shift at full brightness | Jitter under PWM | Panel ground lifted by |
|---|---|---|---|
| Pin 3 only | +31 counts | 36 counts | about 0.30 V |
| Pins 3, 4, 5, 6, 8 | +5 counts | 6 counts | about 0.05 V |

With one wire, a knob turned all the way down would read about 6% instead of zero, and under PWM the reading jumps around enough to make DCS-BIOS send updates nonstop. Five wires cut it sixfold. Most of what's left is jumper wire and breadboard contacts. The carrier ties all six grounds together on copper and has a filter capacitor on every analog input, so I expect it to do better, and I'll repeat this test on it to find out.

At 20 kHz the jitter with five grounds was 14 counts, more than at 1 kHz. That's with no filter capacitors at all, which is exactly what the carrier adds.

## Camera and noise

I pointed my iPhone at the panel in video mode at 200 Hz, 1 kHz, 5 kHz and 20 kHz. No bands at any of them, and nothing audible from the breadboard or the supply either. That clears the phone, not a VR headset's passthrough cameras, but since the plan is 20 kHz anyway it's one less thing to worry about.

## The knob

Last, a spare 10 k pot as a stand-in for the CONSOLE lighting knob. It reads 0 to 1022, the LEDs come on just above the bottom stop, and a slow automatic sweep looked smooth going up and down, with no visible steps. In daylight I couldn't tell much difference between a straight-line curve and a gamma curve. I leaned toward straight-line, but that's another one for the dark room.

## What this changes

Nothing in the design, which is the good news. The things I'm carrying forward:

- **Every ground pin gets used.** The panel PCB already ties them together. The box has to as well.
- **20 kHz stays.**
- **Minimum, maximum and curve are settings**, saved on the Mega and tuned in the pit.

## Next

1. When the carrier PCBs arrive, run the same delay and ground tests on the real driver.
2. Write the dimming firmware with those three settings adjustable.
3. Put the DCS-BIOS sketch back on the Mega so the panel is a panel again.
