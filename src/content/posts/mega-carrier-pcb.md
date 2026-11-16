---
title: "A carrier PCB for the Mega: four panel sockets, dimmers and a dim bus"
date: 2026-11-15
summary: "Instead of hand wiring every console box, each Arduino Mega gets a 100 mm carrier PCB with four panel sockets, a backlight dimmer per socket and a protected 12 V input. Version 1.2 is ordered. This is the design, what three rounds of review caught, and what the fab's placement preview caught that no review could."
tags: [controller-box, pcb, electronics, kicad]
files: [mega-carrier-gerbers, mega-carrier-bom, mega-carrier-cpl, mega-carrier-kicad-pcb, mega-carrier-kicad-pro, mega-carrier-polarity-check]
---

<div class="bottom-line">Each Arduino Mega in a console box now gets a carrier PCB: 100 x 100 mm, four 2x13 panel sockets, a high-side backlight dimmer and fuse per socket, a protected 12 V input, and two RJ45 jacks for the dimming signal. I ordered five assembled copies of version 1.2 from JLCPCB. Nothing has been built or powered yet, so treat the files as untested. The reference page is <a href="/controller-box/mega-carrier/">here</a>.</div>

The [connector standard](/posts/panel-connector-standard/) says every panel plugs into its console's box with one 26-way ribbon. The left console alone has about 13 sockets' worth of panels. Each socket is 15 signal lines to a Mega, a dimmer and a fuse on LED+, six grounds and a +5 V feed. Hand wiring that once on a proto shield is a project. Doing it 13 times is a reason to make a PCB.

## What's on it

<figure>
  <img src="/img/mega-carrier-placement.png" alt="Parts placement of the Mega carrier PCB: four 2x13 sockets across the top half, gate drivers and FETs between them, two RJ45 jacks at lower left, fuses, capacitors and the 12 V connector at lower right" loading="lazy" width="1600" height="1573" />
  <figcaption>Version 1.2, top side. The Mega plugs in underneath, on pins I solder myself.</figcaption>
</figure>

- **Four identical sockets.** Each gets 10 digital inputs, 4 analog inputs and one more digital-only pin. Any panel fits any socket.
- **1 k in series with all 60 inputs**, and a 100 nF capacitor on each of the 16 analog pins.
- **One dimmer per socket.** A P-channel FET switches the 12 V to that panel's LEDs, driven by a real gate driver so it can run at 20 kHz. Each socket has its own 1.5 A resettable fuse.
- **A dim bus on RJ45.** One Mega reads the lighting knobs and makes the PWM. Ordinary patch cables carry that signal to every carrier. The jacks are labeled "NOT ETHERNET, NO PoE", and I mean it.
- **A protected 12 V input**: 5 A resettable fuse, reverse-polarity FET, surge clamp, bulk capacitors, on a plug that only fits one way.

There is no schematic. The PCB is built by a Python script in KiCad, autorouted with Freerouting, and then checked by more scripts: design rules, the netlist, the power pours and the ground return.

## Three rounds of review

I had the design reviewed three times before paying for it. The logic and pinouts were right from the start. The faults were all at the edges, in what happens when one supply is on and another is off.

- **Version 1.0:** with USB on and 12 V off, the dimming signal could drive a gate driver's input above its own supply. The fix was a small pass switch on each input that opens when 12 V is gone.
- **Version 1.1:** my fix had its own fault. I had put capacitors on the pass switch's bias, and they kept it closed for about 20 ms after the 12 V collapsed. One diode fixed that. The same review found the ground under the power section was an island hanging on three small vias, and that swapped 12 V leads with USB plugged in would get around the reverse-polarity protection through the grounds. That last one is why the 12 V connector is now a plug that fits one way.
- **Version 1.2:** "ready to order", with checks to make at the fab's placement preview before paying.

The lesson I'm keeping: a fix can add a fault. Every change gets the same checks as the original.

## What the placement preview caught

No reviewer could have caught the next two, because they aren't in the netlist.

<figure>
  <img src="/img/mega-carrier-polarity-check.png" alt="Carrier PCB outline with red marks at pin 1 of every chip and socket, the cathode of every diode and the positive side of the capacitors and the 12 V connector" loading="lazy" width="1600" height="1499" />
  <figcaption>The cheat sheet for checking the placement preview: where pin 1, each cathode and each "+" has to be.</figcaption>
</figure>

**The wrong socket.** The part number I'd carried since the first draft turned out to be the version with ejector latches, about 50 mm long. In JLCPCB's placement preview the sockets were sitting on top of their neighbors. The listing calls it a "horn buckle" header, which had passed for a plain one. The right part is the box header with no ejectors.

**Rotations.** JLCPCB's part models don't all sit the same way as KiCad's. On the first upload every 8-pin chip was a quarter turn off, and at low zoom it looked fine. Zoom in. These are the corrections that worked for this PCB:

| Footprint | Add to KiCad's rotation |
|---|---|
| SOIC-8 | −90° |
| SOT-23 | +180° |
| 4 x 0603 resistor array | +90° |
| 2x13 box header | −90° |
| 0805, 1206, 1812, 2920, SOD-123, SMB, electrolytic, 12 V header | 0 |

Two more things from the order form. Choosing OSP as the surface finish quietly grays out the cheaper Economic assembly service; HASL brings it back. And a comma inside a parts-list comment shifts every column after it.

## What I still have to solder

JLCPCB places 98 surface-mount parts and the five connectors. I do the Mega's header pins on the bottom (86 joints, with the Mega itself as the jig) and the two RJ45 jacks, which were out of stock.

## What isn't proven

All of it. This PCB has never been built. The day after ordering I [tested the dimming idea on a breadboard](/posts/led-dimming-bench-test/), and that went well, but it was a different driver circuit and proves nothing about this layout. The first carrier gets a bench checklist before a Mega or a panel goes near it. That list is on the [reference page](/controller-box/mega-carrier/).

One limit I already know about: because of the 1 k resistors, a socket pin used as an output can only push about 3 mA. Panels with indicator lamps will need a transistor per lamp, or different resistor arrays on that socket.

## Next

1. When the PCBs arrive: test-fit a 12 V plug and an RJ45 jack before soldering anything.
2. Work through the first-article tests on a current-limited supply.
3. Then the AUX LTS panel moves from jumper wires to socket 1.
