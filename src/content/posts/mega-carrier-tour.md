---
title: "A tour of the carrier PCB: what every part is doing"
date: 2026-11-25
summary: "The Mega carrier PCB has 103 parts on it, and I couldn't have told you what half of them were for when I ordered it. Now that the first one works, here is the PCB zone by zone: what each group of parts is, why it's there, and what would go wrong without it."
tags: [controller-box, pcb, electronics, explainer]
---

<div class="bottom-line">The <a href="/controller-box/mega-carrier/">Mega carrier PCB</a> does three jobs: it gets 12 V to each panel's backlight safely, it dims that backlight, and it carries each panel's switches and knobs to the Arduino Mega underneath. Almost every part on it belongs to one of those three. The rest are fuses and guards for the days when something is plugged in wrong. This post walks the PCB in eight zones. The first carrier is built and working; that story is in <a href="/posts/mega-carrier-first-power/">the bench post</a>.</div>

I ordered this PCB after three rounds of review, and I trusted the reviews more than my own understanding. That isn't a comfortable way to own a design. So this is me explaining it back, one zone at a time. If I can't say what a part is for, it shouldn't be on there.

<figure>
  <img src="/img/mega-carrier-zones.png" alt="Top view of the Mega carrier PCB with eight numbered, colored zones: 12 V entry at lower right, socket fuses above it, two dimmer strips between the four panel sockets at the top, signal protection resistors around the middle, the Mega header rows, the dim bus jacks at lower left, and a row of small parts across the middle" loading="lazy" width="1600" height="1823" />
  <figcaption>The carrier from above, version 1.2. The numbers match the sections below.</figcaption>
</figure>

Two paths run through all of it. Power comes in at the lower right and ends at a panel's LEDs. The dimming signal comes in from the Mega or from another carrier and ends at the switch that turns those LEDs on and off.

<figure>
  <img src="/img/mega-carrier-power-dim-path.png" alt="Block diagram. Power path: 12 V plug, main fuse, reverse guard, surge clamp, then the shared 12 V rail to a socket fuse, a power switch and the panel LEDs. Dimming path: dim signal, input guard, gate driver, which controls the power switch" loading="lazy" width="1600" height="900" />
  <figcaption>The two paths. The socket fuse, power switch, input guard and gate driver exist once per socket.</figcaption>
</figure>

## 1. The 12 V entry

Everything the backlights use comes in here, up to about 4 A for a full carrier.

- **The plug (J5).** A two-pin screw plug that only fits one way. I wire each plug once, check it with a meter, and after that it can't go in backward.
- **The main fuse (F5).** A resettable fuse. It isn't a wire that burns through. It heats up when too much current flows, and its resistance climbs until almost nothing passes. Take the power away, let it cool, and it works again. This one carries 5 A all day at room temperature.
- **The reverse guard (Q5, with D7 and R64).** A transistor used as a one-way valve. With 12 V connected the right way round it switches itself on and is nearly a plain wire. Connected backward it stays off. A diode would do the same job but would waste half a volt and get hot at 4 A. D7 and R64 keep the transistor's control pin inside its safe range.
- **The surge clamp (D5).** It does nothing at 12 V. Above about 15 V it starts to conduct, so a spike from a long supply lead or a hot-plugged connector gets flattened before it reaches the chips.
- **Two big capacitors (C17, C18).** A local reservoir. The backlights are switched on and off thousands of times a second, and each switch-on wants current right now. The capacitors supply it from an inch away, so it doesn't have to be dragged down the supply lead.

**Without it:** one swapped wire or one shorted panel and the damage is whatever the power supply can deliver, which for an ATX supply is a lot.

One hole the reverse guard can't close: if the 12 V leads are swapped while USB is plugged in, current finds a path through the PC's ground. That is why the plug is keyed and why every carrier's 12 V lead gets its own inline fuse.

## 2. A fuse for each socket

Four more resettable fuses (F1 to F4), one per panel socket, each good for 1.5 A.

**Without them:** a short in one panel or its ribbon would trip the main fuse and take all four backlights down with it. With them, only the faulty panel goes dark, and you can tell which one it is.

## 3. The dimmers

There is one dimmer per socket, built in two strips between the upper and lower sockets. This is the busiest part of the PCB and the part I understood least.

**How it dims.** The LEDs are never run at half voltage. They are switched fully on and fully off 20,000 times a second, and the brightness is the fraction of the time they're on. That is fast enough that there's no flicker, and high enough that nothing whines.

**The power switch (Q1 to Q4).** One transistor per socket, sitting in the 12 V lead to that panel's LEDs. Switching the positive lead matters: the panel's ground stays a true ground, and the switches and knobs on that panel share it.

**The gate driver (U1, U2).** The power switch's control pin behaves like a small capacitor. To switch quickly, that capacitor has to be filled and emptied quickly, and it has to swing all the way to 12 V. A Mega pin can do neither. The driver chip can. Each chip has two channels, so two chips cover four sockets. It is the inverting kind on purpose: signal high means LEDs on, and an unplugged signal means LEDs off.

**The input guard (a 2.2 kΩ resistor and Q11 to Q14).** This one came out of the design reviews. Picture the PC on and the 12 V supply off. The Mega is still sending a dimming signal, into a driver chip that has no power. Pushing a chip's input above its own supply can damage it. So a small transistor sits in each signal line and only conducts while 12 V is present. The parts that control it are in zone 8.

**The small parts around each switch.** A resistor that holds the power switch off when the driver is unpowered. A diode from ground to the LED line (D1 to D4): when the switch opens, the ribbon's wiring tries to keep the current flowing for an instant and would pull the LED line below ground, and the diode gives that current somewhere to go. Capacitors next to each chip, for the same reservoir reason as the big ones.

**Without it:** the backlights would be on or off, with nothing in between.

## 4. The four panel sockets

Four identical 26-pin keyed sockets, S1 to S4. Any panel fits any socket. Each one carries:

| Ribbon pins | What |
|---|---|
| 1, 2 | The dimmed 12 V for that panel's LEDs |
| 3, 4, 5, 6, 8, 21 | Ground |
| 7 | 5 V, for the panel's knobs |
| 11 to 20 | Ten switch inputs |
| 22 to 25 | Four knob inputs |
| 26 | One more switch input |
| 9, 10 | Reserved |

Six ground pins looks like a waste. It isn't. The LED current comes home on the same grounds the knobs measure against, and on the [breadboard](/posts/led-dimming-bench-test/) one ground wire threw the knob readings off by 3 to 6%. Five wires cut that sixfold.

**Without the standard:** every panel would need its own wiring plan, and no panel could be moved or swapped.

## 5. Signal protection

All 60 switch and knob lines pass through a 1 kΩ resistor on their way to the Mega. They come four to a package, which is the sixteen small parts marked RN.

- **The resistors.** If something wrong ever reaches a Mega pin, the resistor limits what can flow. With 5 V across 1 kΩ, that's 5 milliamps, which hurts nothing. Static from a ribbon, a miswired panel, a pin set as an output by mistake: all survivable.
- **The capacitors (C1 to C16).** One on each knob input, on the Mega's side of the resistor. Together they form a filter that smooths out the backlight's switching noise before the Mega measures the knob.

**The cost:** a socket pin used as an output can only push about 3 milliamps. A panel with indicator lamps will need a transistor per lamp.

**Without it:** one mistake at a socket could kill a Mega pin, and the knobs would jitter whenever the backlight was dimmed.

## 6. The Mega headers

The carrier is a shield, just a big one. The Arduino Mega plugs in underneath, on 86 pins that I solder myself. The PCB is offset so the Mega's USB and power jacks stick out past the left edge.

Two things I wish I'd known before picking up a meter: every square pad in these rows is a ground pin, and not every carrier needs a Mega. A carrier with no Mega is still a four-socket dimmer.

## 7. The dim bus

Two RJ45 jacks, wired in parallel. They have nothing to do with networking. They carry the dimming signal from one carrier to the next on ordinary patch cables, so one lighting knob can dim every panel in the cockpit.

- **MASTER jumper (JP5).** Bridged on exactly one carrier. It connects that carrier's Mega to the bus, and that Mega makes the signal. Every other carrier just listens.
- **Tap pads (J8).** The bus has room for five lighting groups. The pads are where a socket can be moved to a different one.

**Without it:** every console box would need its own lighting knob, or a wire run from each box to the knob.

The jacks are labeled "NOT ETHERNET, NO PoE". Plugging one into a network switch is one of the things this PCB is not built to survive.

## 8. The row of small parts

The strip across the middle, plus one jumper at the bottom, is where the leftovers live.

- **JP1 to JP4.** Each ties one socket to lighting group 1. Cut one to move that socket to another group.
- **The input guard's bias (R61 to R63, C27, D8).** These tell the input guards in zone 3 whether 12 V is present. The diode makes them let go the instant the 12 V drops, not a fiftieth of a second later. That delay was a fault in version 1.1.
- **5 V fuses (F11 to F14).** A small resettable fuse on each socket's 5 V pin, 100 mA. This one earned its keep on day one, when a panel shorted that pin to ground for about half an hour and nothing was harmed.
- **Driver supply resistors (R15, R25).** They isolate the driver chips' supply from the noisy 12 V rail.
- **JP6, D6, F6, F7.** An option to run the Mega from the 12 V supply when no USB power is present. Left open.

## What it isn't built to survive

Three things, all handled by keying and labels, not by parts:

- A ribbon plugged in backward. That puts 12 V on signal pins. The sockets are keyed.
- Ethernet or PoE on the dim bus.
- Swapped 12 V leads with USB connected.

## Where to go from here

- The pin maps, limits, parts list and files are on the [reference page](/controller-box/mega-carrier/).
- How the first one came up on the bench, and the panel that turned out to be a mirror image, is in [the bench post](/posts/mega-carrier-first-power/).
- How it got designed and reviewed is in [the first carrier post](/posts/mega-carrier-pcb/).
