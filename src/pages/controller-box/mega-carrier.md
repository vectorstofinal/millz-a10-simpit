---
layout: ../../layouts/Page.astro
title: Mega carrier PCB
eyebrow: Controller box reference
lede: The PCB that sits on top of each Arduino Mega in a console box. Four panel sockets, a backlight dimmer per socket, a protected 12 V input and the dim bus. Pin maps, jumpers, limits, first power-up checks and the design files.
description: Mega 4-socket panel carrier PCB for the Millz A-10C sim cockpit. Socket-to-Mega pin map, dimmer and power design, dim bus pinout, jumpers, limits, bench checklist, parts list and KiCad and gerber files.
---

<figure>
  <img src="/img/mega-carrier-placement.png" alt="Parts placement of the Mega carrier PCB: four 2x13 sockets across the top half, gate drivers and FETs between them, two RJ45 jacks at lower left, fuses, capacitors and the 12 V connector at lower right" width="1600" height="1573" />
</figure>

<div class="note">Last updated 2026-11-25. <strong>The first version 1.2 carrier is built and works on the bench:</strong> power checks passed, it dims the AUX LTS backlight from the Mega, and it runs that panel in DCS. Tested so far: the 12 V input, all four dimmer outputs, and every signal line on socket S1. Not yet tested: the signal lines of S2 to S4, more than one panel at a time, the dim bus between two carriers, and the scope test in the checklist. Results are <a href="#results-on-the-first-carrier">below</a>; build history is in the <a href="#build-log">posts at the bottom</a>.</div>

## At a glance

| Item | Detail |
|---|---|
| What it is | An oversized shield for an Arduino Mega 2560. The Mega plugs in underneath |
| Size | 100 × 100 mm, 2 layers, 1.6 mm, 1 oz copper |
| Panel sockets | Four 2×13 box headers (S1 to S4), all wired the same. [Pinout standard](/posts/panel-connector-standard/) |
| Inputs per socket | 10 digital, 4 analog, 1 more digital-only. 60 in all, each through 1 kΩ |
| Backlight | One high-side dimmer per socket, 12 V, plan on 1 A per socket |
| Dimming signal | Comes in on the dim bus (RJ45). One carrier in the pit is the master and makes it |
| Power in | 12 V on a 2-pin pluggable terminal block. The Mega itself runs from USB |
| Made by | JLCPCB, assembled: 98 surface-mount parts and 5 connectors. I add the Mega pins and the RJ45 jacks |
| Design tools | KiCad 7, built by script, autorouted with Freerouting. There is no schematic |

## What's where

<figure>
  <img src="/img/mega-carrier-zones.png" alt="Top view of the Mega carrier PCB with eight numbered zones: 12 V entry, socket fuses, dimmers, panel sockets, signal protection, Mega headers, dim bus and a row of small parts" loading="lazy" width="1600" height="1823" />
  <figcaption>The PCB in eight zones. <a href="/posts/mega-carrier-tour/">A tour of the carrier PCB</a> explains what each one does.</figcaption>
</figure>

Finding your way on the bare PCB, sockets at the top:

- **Every square pad in the Mega header rows is a ground pin.**
- **Power header:** the leftmost group of eight in the bottom row. Left to right: not connected, IOREF, RESET, 3.3 V, 5 V, ground, ground, VIN. The carrier only uses the last four.
- **Double row:** the pair at the top is 5 V, the pair at the bottom is ground. D22 is the left column and D23 the right.
- **Sockets:** pin 1 is at the lower left, by the white triangle. Odd pins run along the bottom row, nearest the key notch, and even pins along the top row.
- **J8:** the top pad, the square one, is DIM1.

## Socket to Mega pin map

| Socket | D1–D10 (ribbon 11–20) | A1–A4 (ribbon 22–25) | A5 (ribbon 26), digital only |
|---|---|---|---|
| S1 | D22–D31 | A0–A3 | D14 |
| S2 | D32–D41 | A4–A7 | D15 |
| S3 | D42–D51 | A8–A11 | D16 |
| S4 | D2–D11 | A12–A15 | D17 |

On every socket:

| Ribbon pin | Goes to |
|---|---|
| 1, 2 | LED+, the dimmed 12 V from that socket's dimmer |
| 3, 4, 5, 6, 8, 21 | Ground |
| 7 | +5 V from the Mega, through a 100 mA resettable fuse |
| 9, 10 | RS-485 A and B, reserved. They go to header J9 and nowhere else |
| 11–20 | D1–D10 |
| 22–25 | A1–A4, each with 100 nF to ground on the Mega side of its 1 kΩ |
| 26 | A5, digital only |

Mega pins left free: D0, D1, D12 (the master's PWM output), D13, D18–D21, D52, D53. D11 is a socket input, so nothing may drive Timer1's other output.

## 12 V input

In order, from the connector J5 to the 12 V rail:

1. **J5**, a 5.08 mm right-angle pluggable terminal block header. Upper pin +12 V, lower pin ground, marked on the silkscreen. The plug fits one way.
2. **F5**, a 5 A resettable fuse (24 V part).
3. **Q5**, a P-channel FET as reverse-polarity protection, with a 15 V Zener across its gate.
4. **D5**, an SMBJ13A surge clamp, and two 220 µF capacitors.

From the rail, each socket has its own 1.5 A resettable fuse (F1 to F4).

Swapped 12 V leads are still dangerous if USB is plugged in, because the supply and the PC share a ground. That is why the connector is keyed, and why each carrier's 12 V lead gets an inline 7.5 A blade fuse. Wire each plug once, check it with a meter, and never swap leads at a live carrier.

## Dimmer, one per socket

- **Switch:** AO4407A P-channel FET. Source to the fused 12 V, drain to the socket's LED+.
- **Gate drive:** TC4426 dual inverting gate driver, one chip per pair of sockets. 100 Ω in the gate, 100 kΩ from gate to source.
- **At each FET:** 100 nF on the fused 12 V and a Schottky diode from ground to LED+.
- **Input protection:** the dim signal reaches the driver through 2.2 kΩ and a small FET that only conducts while 12 V is present. With 12 V off, a live dim line is disconnected from the unpowered driver. A diode makes that switch follow the 12 V rail down without delay.
- **Polarity:** dim line high means LEDs on. With the dim line unplugged, the LEDs are off.

The TC4426 has to be the inverting one. The non-inverting TC4427 fits the same pads, and it would turn the dimming upside down and leave the LEDs on whenever the dim line is unplugged.

## Dim bus

Two RJ45 jacks (J6, J7) wired in parallel, so carriers daisy-chain with ordinary straight-through patch cables.

| RJ45 pin | Signal |
|---|---|
| 1 | DIM1, the CONSOLE lighting group |
| 2 | Ground |
| 3, 4, 5, 7 | DIM2 to DIM5. Passed jack to jack and to the J8 tap pads only |
| 6, 8 | Not connected |

**This is not Ethernet.** Never plug a dim-bus cable into a network switch, and never into anything with PoE. The jacks are labeled, and the design is not meant to survive it.

## Jumpers and headers

| Mark | As delivered | What it does |
|---|---|---|
| JP1–JP4 | Bridged | Tie sockets S1–S4 to DIM1. Cut one and wire that socket to a J8 pad to put it in another lighting group |
| JP5 "MASTER" | Open | Bridge on one carrier only. Connects Mega D12 through 220 Ω to DIM1, making that Mega the dim master |
| JP6 "VIN=12V" | Open | Feeds the Mega's VIN from the 12 V rail through a diode and two small fuses, for running with USB unpowered. Leave open otherwise |
| J8 | Pads | DIM1 to DIM5 taps |
| J9 | Pads | RS-485 A and B, reserved |

Five M3 mounting holes.

## Limits

- **Outputs are weak.** Because of the 1 kΩ in every line, a socket pin used as an output can push only about 3 mA. A panel with indicator lamps needs a transistor per lamp, or that socket's 1 kΩ arrays swapped for 100 Ω ones.
- **LED current.** Each socket's fuse holds 1.5 A at room temperature. Plan on 1 A continuous per socket, which is about 80 LEDs of the kind in my AUX LTS panel.
- **Whole carrier.** The input fuse holds 5 A at room temperature. Plan on 4 A in open air and less in a closed, warm box.
- **+5 V per socket:** 100 mA. Enough for pots and small logic. Not for servos or LED strip. The fuse on this pin has already held a dead short for about half an hour with no damage.
- **A reversed ribbon is not survivable.** It puts 12 V on signal pins. Keyed sockets and keyed plugs are the only protection.
- **Fused 12 V traces** are 0.8 mm wide and up to 116 mm long: fine at 1 A, warm at a sustained 1.5 A.
- **Autorouted** and electrically checked, not hand-tidied.

## What I solder myself

- **Mega header pins**, on the bottom, only on carriers that get a Mega: five 1×8 strips, one 1×10, and two 1×18 side by side. 86 joints. Plug the strips into the Mega, set the carrier on top, and solder from the carrier's top side. The Mega is the jig.
- **Two RJ45 jacks**, Amphenol 54602-908LF. Test-fit one before soldering: two pegs and eight pins should seat without force. Mine fit. They are only needed once two carriers are linked.
- **The 12 V lead** for each carrier: a 2-pin 5.08 mm pluggable screw plug, with an inline blade fuse holder and a 7.5 A fuse.
- **JP5**, a solder blob, on the one carrier that is the dim master.

## First-article checklist

Before a Mega or a panel goes near the first carrier:

0. Wired 12 V plug inserted, nothing powered, no USB. Meter from the plug's "+" screw to the pad of F5 nearest the PCB edge, and from its "−" screw to pin 3 of a socket. Mark the plug.
1. No Mega fitted. 12 V from a current-limited supply. The rail should be present at F1 to F4 with nothing getting warm.
2. Continuity on each socket: pin 1 to the FET's drain, pin 7 to +5 V, pins 3–6, 8 and 21 to ground.
3. 12 V off, 5 V on the dim line: the gate driver's inputs should read 0 V.
4. 5 V held on the dim line, a 1 A load on a socket, then pull the 12 V plug. On a scope, the driver's input must stay below its supply pin all the way down.
5. Fit the Mega and plug the AUX LTS panel into S1. Every switch reads and both pots sweep the full range. Then load all four sockets and watch for false switch readings and jitter on the pots.
6. Scope LED+ at minimum and mid brightness: clean edges, nothing below ground, LEDs fully off at zero.

I'll also repeat the two measurements from the [breadboard dimming test](/posts/led-dimming-bench-test/): supply current at a fixed duty across PWM frequencies, and the pot-noise test.

### Results on the first carrier

Run on 2026-11-25. The story is in [the bench post](/posts/mega-carrier-first-power/).

| Step | Result |
|---|---|
| 0, plug continuity | Pass |
| 1, 12 V with no Mega | Pass |
| 2, socket continuity | Grounds pass on all four sockets |
| 3, 12 V off with 5 V on the dim line | Pass: driver inputs read 0 V |
| Added: 12 V on with 5 V on the dim line | Pass: LED+ of all four sockets goes to 12 V |
| Added: AUX LTS panel on S1 as the load | 36 LEDs lit, 0.435 A at 12 V |
| 4, scope test with the 12 V pulled | Not done yet |
| 5, Mega fitted, AUX LTS on S1 | Pass through a row-swap adapter: every switch line and both pots read on the designed pins, and the panel works in DCS |
| 5, four sockets loaded | Not done yet |
| 6, scope on LED+ | Not done yet. Dimming from the Mega works by eye |

The adapter in step 5 is for that panel, not the carrier. The AUX LTS header turned out to be a mirror image of the standard; see its [panel page](/panels/aux-lts/).

With no Mega fitted, the dim line can be driven from a bench supply at J8's top pad. The supply's negative has to be tied to the carrier's ground.

## Test sketches

- [Carrier_socket_scan.ino](/files/controller-box/Carrier_socket_scan.ino): prints which Mega pin moved when you work a control, with the socket and ribbon pin the design assigns to it. Run this first on any new carrier, socket or panel.
- [LED_dim_play.ino](/files/controller-box/LED_dim_play.ino): backlight dimming from the Serial Monitor, on D12. Needs JP5 bridged.
- [AUX_LTS_dcsbios.ino](/files/controller-box/AUX_LTS_dcsbios.ino): the AUX LTS panel on socket S1, for DCS.

## Firmware notes for the dim master

- Timer1 in mode 14 with TOP = 799 gives 20 kHz and 800 brightness steps on D12.
- Set D12 low before starting the timer, and force it low at zero brightness.
- Leave Timer1's other output (D11) disconnected, and run nothing else on Timer1.

## Parts

| Part | Where | LCSC |
|---|---|---|
| AO4407A P-FET, SOIC-8 (Alpha & Omega) | Q1–Q5 | C16072 |
| TC4426 dual inverting gate driver, SOIC-8 | U1, U2 | C27201 |
| 2N7002, SOT-23 | Q11–Q14 | C8545 |
| 2×13 box header, no ejectors (DC3-2.54-26PAS) | J1–J4 | C5156668 |
| Pluggable header, 2-pin 5.08 mm right-angle (WJ2EDGRC-5.08-02P) | J5 | C3697 |
| 4 × 1 kΩ resistor array, 0603×4 | RN1–RN16 | C20197 |
| Resettable fuse 1.5 A, 24 V, 1812 | F1–F4 | C70123 |
| Resettable fuse 5 A, 24 V, 2920 (2920L500/24SLDR) | F5 | C1512058 |
| Resettable fuse 0.1 A, 60 V, 1206 | F6, F7, F11–F14 | C20981 |
| B5819W Schottky, SOD-123 | D1–D4, D6, D8 | C8598 |
| SMBJ13A surge clamp, SMB | D5 | C353394 |
| 15 V Zener, SOD-123 | D7 | C2104 |
| 220 µF 25 V electrolytic, 6.3 × 7.7 mm | C17, C18 | C1881633 |
| 100 nF 0805, 10 µF 1206, and 0805 resistors | the rest | see the parts list file |
| RJ45 jack, Amphenol 54602-908LF | J6, J7 | hand-fitted, not in the parts list |

Three parts not to substitute: the TC4426 (inverting only), the AO4407A (genuine, for its ±25 V gate rating), and F5 (must be the 24 V part).

Do not use LCSC C9040 for the sockets. It is the same header with ejector latches, about 50 mm long, and it collides with its neighbors.

## Files

All original work, CC0. The first carrier built from these files works on the bench. What has and hasn't been tested is in the results above.

- [Gerbers and drill files (zip)](/files/mega-carrier/gerbers_mega_carrier_v1.2.zip)
- [Parts list for assembly (csv)](/files/mega-carrier/BOM_mega_carrier_v1.2.csv)
- [Placement file, top side (csv)](/files/mega-carrier/CPL_mega_carrier_v1.2.csv), with the rotations already corrected for JLCPCB
- [KiCad PCB](/files/mega-carrier/mega_carrier_v1_2.kicad_pcb) and [project file](/files/mega-carrier/mega_carrier_v1_2.kicad_pro) (KiCad 7)
- [Placement polarity check (png)](/img/mega-carrier-polarity-check.png): where pin 1, each cathode and each "+" must be in the fab's preview

<figure>
  <img src="/img/mega-carrier-copper.png" alt="Copper layers of the Mega carrier PCB: top layer in red, bottom layer in blue" loading="lazy" width="1600" height="1573" />
  <figcaption>Both copper layers. Red is the top, blue is the bottom.</figcaption>
</figure>

## Build log

- [A tour of the carrier PCB](/posts/mega-carrier-tour/): what every part is doing
- [The carrier PCBs work, and the panel that was a mirror image](/posts/mega-carrier-first-power/): first power, first panel, first DCS run
- [A carrier PCB for the Mega](/posts/mega-carrier-pcb/): the design, the reviews, the order
- [Backlight dimming on a breadboard](/posts/led-dimming-bench-test/): the dimming idea tested with an interim driver
- [The panel connector standard](/posts/panel-connector-standard/): the pinout this PCB implements
