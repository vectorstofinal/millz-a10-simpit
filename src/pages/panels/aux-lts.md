---
layout: ../../layouts/Page.astro
title: AUX LTS panel
eyebrow: Panel reference · Left console
lede: Everything about this panel in one place, including what's in it, how it's wired and how to take it apart. If you scanned the QR code on the back of the panel, this is the page you want.
description: AUX LTS panel for the Millz A-10C sim cockpit. Build details, parts list, pinout, DCS-BIOS mapping, and disassembly and reassembly steps.
console: Left console
status: Built, lit and working in DCS on the carrier PCB, through a row-swap adapter.
summary: Refuel and weapon-station light dimmers, NVIS and HARS/SAS toggles, and the lamp-test and fire-detect pushbuttons. Three acrylic layers, 36 green LEDs, home-milled PCB.
photo: /img/aux-lts-lit-panel.jpg
---

<figure>
  <img src="/img/aux-lts-lit-panel.jpg" alt="AUX LTS panel backlit green: REFUEL STATUS and WPN STA dimmers, NVIS and HARS/SAS toggles, SIGNAL LIGHTS LAMP TEST and FIRE DETECT pushbuttons" width="1600" height="1330" />
</figure>

<div class="note">Last updated 2026-11-25. The panel is built, lit, and works in DCS from socket S1 of the <a href="/controller-box/mega-carrier/">Mega carrier PCB</a>. <strong>Its ribbon header is a mirror image of the connector standard, so it needs a row-swap adapter in its cable.</strong> See <a href="#header-pinout-this-panel">the header pinout</a>. Build history is in the <a href="#build-log">posts at the bottom</a>.</div>

## At a glance

| Item | Detail |
|---|---|
| Location | Left console, plugs into the left-console controller box |
| Size | 143.5 × 120.4 mm face; stack 9.0 mm (three 3 mm acrylic layers) plus the PCB on 6 mm standoffs behind it |
| Design | [The Warthog Project](https://thewarthogproject.com) AUX LTS file, with [my edits](/posts/aux-lts-panel/) |
| Backlight | 36 × 3 mm green LEDs, built-in resistors, 12 V. **0.432 A total** (12.0 mA each) |
| Connector | 2×13 pin header (2.54 mm), 26-way ribbon to the console box. [Pinout standard](/posts/panel-connector-standard/) |
| Firmware | DCS-BIOS on the console box's Arduino Mega ([sketch, v4](/files/controller-box/AUX_LTS_dcsbios.ino)) |

## Controls

| Control | Part | Header pin | DCS-BIOS | Notes |
|---|---|---|---|---|
| REFUEL STATUS & INDEXER LIGHTS (DIM–BRT) | B10K pot, 16 mm | 25 (A4) | `ALCP_RSIL` | Standard wiring |
| WPN STA STATUS LTS (OFF–BRT) | B10K pot, 16 mm | 23 (A2) | `ALCP_WPNSTA` | Ends swapped on the PCB; **reversed in firmware** (`reverse = true`) |
| NVIS LTS (TOP / ALL / OFF) | 3-position toggle, ON-OFF-ON | 15, 17 | `ALCP_NVIS_LTS` | Three terminals, center = ground. ALL = neither closed. Pin 15 closes at **TOP** and pin 17 at **OFF**, the opposite of the PCB pad labels. Confirmed on the carrier. Handled by the pin order in firmware |
| HARS/SAS (OVERRIDE / NORM) | 2-position toggle | 19 | `ALCP_HARSSAS` | Two terminals only, no center one. As mounted, pin 19 closes at **NORM**. Fixed with `reverse = true` |
| SIGNAL LIGHTS LAMP TEST | MS25089-3G pushbutton | 13 (D3) | `LAMP_TEST_BTN` | Momentary. Wired to the normally-open pair |
| FIRE DETECT BLEED AIR LEAK TEST | MS25089-3G pushbutton | 12 (D2) | `ALCP_FDBA_TEST` | Momentary. Wired to the normally-open pair |

## Terminals and wire colors

<figure>
  <img src="/img/aux-lts-terminals-back.jpg" alt="Copper side of the AUX LTS PCB: the NVIS toggle with three screw terminals, the HARS/SAS toggle with two, both pots soldered, and the two pushbuttons with screws in their normally-open terminals" width="1200" height="1600" />
  <figcaption>Back of the panel, header at the bottom. The pushbuttons aren't wired yet; the screws mark the two terminals each one will use.</figcaption>
</figure>

"Upper" and "lower" mean toward the top and bottom edges of the panel.

| Control | Terminals | Wires as built |
|---|---|---|
| NVIS LTS toggle | Three screw terminals with solder lugs | Upper: green. Center: black, ground. Lower: blue |
| HARS/SAS toggle | Two screw terminals, one at each end. The center position has no terminal | Upper: yellow, to pin 19. Lower: black, ground |
| REFUEL STATUS pot | Three solder lugs | Black ground, blue wiper (short), red +5 V (long, to the pad by pin 7) |
| WPN STA pot | Three solder lugs | Black ground, blue wiper (long, to the pad by pin 23), red +5 V (short) |
| Pushbuttons (2) | Four screw terminals in an X | One normally-open terminal to the signal pad, the opposite one to ground |

**Pushbutton terminals.** The MS25089-3G is a two-circuit switch, not a plain normally-open button. Its four terminals are two separate pairs: one pair is closed until you press the button, and the other pair closes when you press it. On mine the normally-open pair is the two terminals directly across from each other. I put screws only in that pair, so there's no question later about which ones to wire. Leave the other pair empty. These are surplus parts, so check each one with a meter first.

**HARS/SAS toggle.** With only two terminals there is no "other lug" to move the wire to: the switch is simply closed in one lever position and open in the other. As I mounted it, closed is NORM, so the firmware reverses it.

## Header pinout (this panel)

<div class="note warn"><strong>This header is a mirror image of the standard.</strong> Its plastic strip is on the front of the PCB and the ribbon plugs onto the long pins at the back. I numbered the pins as seen from the front, so a standard ribbon socket pushed onto the back finds the two rows exchanged: pin 1 where pin 2 should be, 3 where 4 should be, and so on. The backlight still works, because pins 1 and 2 are both LED+ and pins 3 to 6 are all ground. The controls land one pin off, and pin 7 (+5 V) swaps with pin 8 (ground), which shorts the box's 5 V. The table below is the PCB's own numbering. The PCB files on this page have the same fault.</div>

**The adapter.** A long double-ended 2×13 pin header with a ribbon socket pushed onto each side swaps the rows back. This panel's cable is two ordinary ribbons joined that way. Put the red stripes at the same end; the two sockets' key bumps then sit on opposite faces. Details and a drawing are in [the bench post](/posts/mega-carrier-first-power/).

Copper side up, header toward you: pin 1 is the **left end of the row nearer the PCB center**. Pins 1 and 2 are joined by a short copper bar.

| Pin | Signal | Pin | Signal |
|---|---|---|---|
| 1 | LED+ 12 V (dimmed) | 2 | LED+ |
| 3 | GND | 4 | GND |
| 5 | GND | 6 | GND |
| 7 | +5 V (pot feed) | 8 | GND |
| 9–11 | — | 12 | D2 FIRE DETECT |
| 13 | D3 LAMP TEST | 14 | — |
| 15 | D5 NVIS OFF | 16 | — |
| 17 | D7 NVIS TOP | 18 | — |
| 19 | D9 HARS/SAS | 20 | — |
| 21 | GND | 22 | — |
| 23 | A2 WPN STA wiper | 24 | — |
| 25 | A4 REFUEL wiper | 26 | — |

**Bench light test:** 12 V into pin 1, ground on pin 3, current limit 0.6 A. Expect about 0.43 A.

## What's in it

**Layers, front to back**

1. **Top:** White Matte cast acrylic, 3 mm, matte face painted black, with the legends laser-engraved through the paint.
2. **Middle (diffuser):** Milky White 2447 cast acrylic, 3 mm. It has 36 LED pockets in its back face, two hex pockets that hold the pushbutton nuts, and clearance pockets for the toggle and pot nuts.
3. **Base:** black cast acrylic, 3 mm. It has 36 press-fit 3.0 mm LED holes, the switch holes and two pot anti-rotation slots.
4. **PCB:** single-sided FR-4, isolation-milled. Copper faces the back. See [milling the PCB](/posts/aux-lts-pcb-milled/). Its four corners are cut back 12.5 mm to clear the console deck ears ([why](/posts/aux-lts-trimmed-back-case/)); the [3D model](/3d/aux-lts/) shows the PCB as trimmed.

**Parts list**

| Qty | Part | Notes |
|---|---|---|
| 2 | B10K pot, 16 mm, M7 bushing, knurled split shaft | Anti-rotation tab sits in the base slot, lugs pointing down |
| 1 | Toggle, ON-OFF-ON, 16 × 30 mm case, three screw terminals | NVIS |
| 1 | Toggle, 2-position, same footprint, two screw terminals | HARS/SAS |
| 2 | MS25089-3G pushbutton | Two-circuit switch, four screw terminals. Mounts from the front; nut captive in the middle layer, no washer. Wires go on the normally-open pair |
| 36 | 3 mm green LED, built-in resistor, 12 V | Water-clear |
| 2 | [Printed dimmer knob](/posts/printed-dimmer-knob/) | PETG, 0.1 mm layers |
| 4 | M3 × 6 mm brass hex standoff, female-female | |
| 4 | M3 × 12 button-head screw | Face, through all three layers |
| 4 | M3 × 4 screw | PCB to standoffs. **Not M3×6**: that collides with the face screw inside the standoff |
| 1 | 2×13 straight pin header, long | Long pins trimmed to 8 mm above the copper ([gauge](/files/aux-lts-pcb/header-pin-trim-gauge-8mm.stl)) |
| 1 | Row-swap adapter: a second long 2×13 pin header and a second 26-way ribbon | In the cable, because this panel's header is mirrored. Slide the header's plastic strip to the middle of the pins |
| 1 | [Printed keyed shroud](/files/aux-lts-pcb/idc-2x13-keyed-shroud.stl) | Epoxied over the header, key notch toward the PCB center |
| 2 | Ground link "staples" | Bent LED lead offcuts on the front, 3.8 mm span |
| — | 22 AWG solid hook-up wire | 15 wires from the switch terminals to pads beside each control. Toggles and pushbuttons are on screw terminals, pots are soldered |

## Taking it apart

<div class="note warn">Unplug the ribbon first. Tools: a 2 mm hex key for the face screws, a wrench or nut driver for the switch nuts (toggle nuts about 14 mm across the flats, pot nuts about 9.5 mm), a small screwdriver for the toggle and pushbutton terminals, and a soldering iron for the pots.</div>

1. **Remove the panel from the console** (four corner fasteners) and unplug the ribbon. Pull the socket straight off; don't rock it.
2. **Pull off both knobs.** They're press-fit on the knurled shafts.
3. **Remove the four M3×12 face screws.** The standoffs stay on the PCB.
4. **Lift the top and middle layers off together.** The pushbuttons come with them, clamped between their flanges and the captive nuts in the middle layer, and slide out of the base and the PCB. Their wires stay attached, so lift gently. If a wire is too short, disconnect it at the pushbutton's screw terminal first.
5. **Separate top and middle (only if needed):** unscrew each pushbutton from the front by its collar. Disconnect its wires first, or they'll twist. Its nut drops out of the hex pocket.
6. **Free the switches from the base:** unscrew the toggle and pot nuts from the front of the base. To take a toggle out completely, loosen its terminal screws and lift the wires off.
7. **Pull the base straight off the PCB.** The 36 LEDs are a press fit in the base, so it takes a firm, even pull. The pots (soldered) stay wired to the PCB and drop out of their holes; so do the toggles, unless you've already freed them from their terminals.

To replace an LED, unsolder it from the copper side once the base is off.

## Putting it back together

1. **Base onto the PCB:** pot tabs into their slots, lugs down. Press the base evenly over all 36 LEDs until the rims seat.
2. **Toggle and pot nuts on the front of the base, torqued now.** You can't reach them once the middle layer is on.
3. **Pushbutton nuts** (no washers) into the two hex pockets of the middle layer.
4. **Middle layer** on with its pocketed face toward the base. It should sit flat with no rocking. If it rocks, an LED tip is missing its pocket.
5. **Top layer, then the four M3×12 screws:** snug, in a cross pattern. Overtightening cracks acrylic and marks the paint.
6. **Pushbuttons:** screw them in from the front by the collar until the flange sits flat. Snug only; use threadlocker if one works loose. Reconnect their wires if you removed them, after the button is screwed in.
7. **Knobs on,** pointers lined up with the DIM/OFF marks at the end of the travel.
8. **Check:** 12 V on pin 1, ground on pin 3. Expect about 0.43 A with all 36 LEDs lit.

## DCS-BIOS code

The panel has no microcontroller of its own; it runs on its console box's Arduino Mega. These are the lines that drive it, ready to drop into the box sketch. Only the pin macros change with the socket the panel plugs into. This matches version 4 of the sketch, tested in DCS on the carrier PCB on 2026-11-25.

```cpp
// AUX LTS panel (left console). DCS-BIOS Arduino library, A-10C module.
// Socket pin map: set for the box socket this panel plugs into.
// Shown for socket S1 of the carrier: ribbon pins 11-20 (D1-D10) -> Mega D22-D31, pins 22-25 (A1-A4) -> A0-A3.
#define AUX_D(n) (21 + (n))   // socket D1..D10 -> Mega pin
#define AUX_A2   A1           // ribbon pin 23
#define AUX_A4   A3           // ribbon pin 25

// Pushbuttons: closed = pressed. 10 ms debounce; the library's 50 ms default is noticeable.
DcsBios::Switch2Pos auxFireDetectTest("ALCP_FDBA_TEST", AUX_D(2), false, 10);   // pin 12
DcsBios::Switch2Pos auxLampTest      ("LAMP_TEST_BTN",  AUX_D(3), false, 10);   // pin 13

// NVIS LTS: first pin sends 0 (OFF in DCS), second sends 2 (TOP); neither closed = 1 (ALL).
// On this panel pin 17 closes at OFF and pin 15 closes at TOP.
DcsBios::Switch3Pos auxNvisLts("ALCP_NVIS_LTS", AUX_D(7), AUX_D(5));            // pins 17, 15

// HARS/SAS: DCS takes 1 as OVERRIDE. Pin 19 closes at NORM on this panel, so reverse = true.
DcsBios::Switch2Pos auxHarsSas("ALCP_HARSSAS", AUX_D(9), true);                 // pin 19

// Dimmer pots. Read every 20 ms, send on a change of 256 (of 65535), light smoothing.
// The library's default Potentiometer floods messages and the sim knobs lag behind your hand.
typedef DcsBios::PotentiometerEWMA<20, 256, 2> PanelPot;
PanelPot auxRefuelLts("ALCP_RSIL",   AUX_A4, false, 0, 1023);
PanelPot auxWpnStaLts("ALCP_WPNSTA", AUX_A2, true,  0, 1023);   // ends swapped on the PCB
```

- All six control names are checked against the DCS-BIOS A-10C module. In DCS itself, `ALCP_WPNSTA` is marked "No Function": the knob moves in the virtual cockpit but does nothing.
- **Switch directions come from testing, not documentation.** DCS-BIOS doesn't say which end of a switch is 0. For this panel: NVIS LTS 0 = OFF, 2 = TOP; HARS/SAS 1 = OVERRIDE.
- **Both pots sweep the full 0 to 1023**, so the min/max values stay at 0 and 1023.
- **Startup sync belongs to the box sketch, not the panel.** The library only sends a control when it changes, so the sim doesn't know where the switches are when a mission starts. The bench sketch watches the data coming from DCS and re-sends every control about 3 seconds after it starts. Copy that part (`StreamWatch` and `updateStartupSync`) from the full sketch.
- **The backlight isn't in this code.** It's dimmed locally by the console box from the lighting knob (high-side PWM on ribbon pins 1–2), with no DCS in the loop.
- Full sketch as tested on the carrier: [AUX_LTS_dcsbios.ino (v4)](/files/controller-box/AUX_LTS_dcsbios.ino). Its pin numbers are socket S1 (D23, D24, D26, D28, D30, A1, A3), the same as the macros above. Versions 1 to 3 followed my breadboard jumpers; that story is in [the first DCS test post](/posts/aux-lts-first-dcs-test/).

## Known quirks

- **Mirrored ribbon header.** See the [header pinout](#header-pinout-this-panel). It needs the row-swap adapter in its cable.
- **Light leaks** around the toggles, the WPN STA shaft and the panel edges, where light escapes from the diffuser's cut edges. Fix: blacken those edges with a paint pen.
- **A few hot spots** over single LEDs. Scuff the LED tips with fine sandpaper if they bother you at flying brightness.
- **Faint laser haze** around the switch holes on the painted face. Clean with mild soapy water, never alcohol or acetone, which can craze the cut edges.

## Files

- **Acrylic:** [top DXF](/files/aux-lts-panel/aux-lts-1-top.dxf) · [top LightBurn job](/files/aux-lts-panel/aux-lts-1-top-white-matte-painted.lbrn2) · [middle DXF](/files/aux-lts-panel/aux-lts-2-middle.dxf) · [middle LightBurn job](/files/aux-lts-panel/aux-lts-2-middle-mirrored.lbrn2) · [base DXF](/files/aux-lts-panel/aux-lts-3-base.dxf) · [base LightBurn job](/files/aux-lts-panel/aux-lts-3-base-black.lbrn2)
- **PCB:** [VCarve job DXF](/files/aux-lts-pcb/aux-lts-pcb-v2-vcarve-job-250x130.dxf) · [CNC DXF](/files/aux-lts-pcb/aux-lts-pcb-v2-cnc-back-view.dxf) · [trimmed outline DXF](/files/aux-lts-back-case/aux-lts-pcb-v2-trimmed-outline.dxf) · [corner trim template](/files/aux-lts-back-case/aux-lts-pcb-corner-trim-template.pdf)
- **Printed:** [header shroud](/files/aux-lts-pcb/idc-2x13-keyed-shroud.stl) · [pin trim gauge](/files/aux-lts-pcb/header-pin-trim-gauge-8mm.stl) · [dimmer knob](/files/dimmer-knob/dimmer-knob-26mm.stl)
- **Firmware:** [AUX_LTS_dcsbios.ino (v4)](/files/controller-box/AUX_LTS_dcsbios.ino) · [Carrier_socket_scan.ino](/files/controller-box/Carrier_socket_scan.ino) for checking which pin each control lands on · [AUX_LTS_bench.ino](/files/controller-box/AUX_LTS_bench.ino), the older bench sketch
- **3D:** [interactive model](/3d/aux-lts/)

## Build log

- [AUX LTS: three layers of acrylic and 36 green LEDs](/posts/aux-lts-panel/)
- [Milling the AUX LTS board on the CNC router](/posts/aux-lts-board/)
- [AUX LTS PCB: milled, and how 36 LEDs get in](/posts/aux-lts-pcb-milled/)
- [AUX LTS: first light](/posts/aux-lts-first-light/)
- [AUX LTS: wired, tested and stacked](/posts/aux-lts-wired-and-stacked/)
- [AUX LTS: corners trimmed, and a back case](/posts/aux-lts-trimmed-back-case/)
- [AUX LTS: first controller test, and it works in DCS](/posts/aux-lts-first-dcs-test/)
- [The carrier PCBs work, and the panel that was a mirror image](/posts/mega-carrier-first-power/)
