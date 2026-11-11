---
title: "One cable per panel: my 26-pin standard and the controller box"
date: 2026-11-09
summary: "Every panel plugs into its console's controller box with one 26-way ribbon. Pinout, why the LED dimming is on the high side, power rules, and the DCS-BIOS bench rig that drives AUX LTS."
tags: [electronics, dcs-bios, arduino, wiring]
hero: /img/aux-lts-bench-rig-wiring.png
files: [bench-ino, bench-wiring]
---

<div class="bottom-line">Each panel has one 2x13 header and one ribbon to a controller box in its console. Simple panels are passive: up to 15 inputs on an Arduino Mega running DCS-BIOS. Dense panels (radios, intercom, IFF) get their own USB-C board on the console's powered hub. The box dims the backlights with high-side PWM, so every panel board can be one big ground pour.</div>

I wanted a controller box in each console, with every panel plugging into it on one simple cable. That needed a standard, settled before I milled the first board.

## The pinout

| Pin | Signal | Notes |
|---|---|---|
| 1–2 | LED+ 12 V, dimmed | Two pins, ~2 A per panel max. PWM-switched on the high side in the box |
| 3–4 | GND (LED return) | Same net as logic ground on the panel |
| 5–6 | GND | Switch commons |
| 7 | +5 V | Pot reference for passive panels. Left open on USB-powered panels |
| 8 | GND | |
| 9–10 | RS-485 A/B | Reserved |
| 11–20 | D1–D10 | Digital inputs, switched to ground, pull-ups in the box |
| 21 | GND | Separates the analog pins from the digital block |
| 22–26 | A1–A5 | Pots, or extra digital inputs |

Rules:

- **15 inputs per passive panel** (10 digital plus up to 5 analog). Use whichever pins route cleanly on the board.
- **3-position toggles use 2 pins** (one per outer position); the center is "neither".
- **Pots:** ends to GND and pin 7, wiper to an A pin. If you swap a pot's ends to make routing easier, invert it in firmware.
- **Out of pins?** Put momentary buttons or a hat on one analog pin through a resistor ladder, as long as they're never pressed together.

## Why high-side dimming

I started with low-side PWM (switching LED−). That forces LED− to be its own net, so on a single-sided home-milled board, ground has to be a routed trace tree. AUX LTS needed about ten jumpers that way.

Switching LED+ instead (a P-channel MOSFET per socket in the box) makes the whole board pour ground. Signals run surrounded by quiet copper, and AUX LTS routes with zero trace jumpers and two short ground links.

The cost: LED return current shares the ribbon ground with logic. At ~0.5 A over six ground conductors that's ~17 mV of ripple per meter at the PWM frequency. That's a few ADC counts, which a 100 nF cap and some averaging take care of. Fine for brightness knobs. Tie every ground pin together at one star point in the box.

## Power rules

- **12 V LED rail** from a spare ATX supply through a breakout board, with a polyfuse on each socket's LED+ (1.1 A standard, 0.75 A for small panels, 1.35 A for big ones).
- **Logic 5 V comes from USB** (the Megas sit on a powered hub), never from the ATX 5 V on the same net.
- **Never through the ribbon:** motors, servos, solenoids, magnetically-held switch coils, shakers, fans. Those panels get a separate locking 2-pin power connector, plus a flyback diode on every coil. Inductive spikes and ground bounce on 28 AWG ribbon make pots jitter and can reset an MCU.
- Small instrument steppers (X27/VID29, 20–30 mA per coil) can run from a panel MCU's USB 5 V if the total is modest.

## Dimming

- One IRF9540N P-FET per socket, source to +12 V, drain to the socket's LED+. A small N-FET or NPN drives the gate from the MCU, with a 10 k pull-up to +12 V so the LEDs stay off at boot.
- **Built-in-resistor LEDs dim evenly with PWM.** Dropping the voltage instead makes them dim unevenly and drop out one by one.
- **Brightness follows the jet:** DCS-BIOS exports the cockpit lighting knob levels. The box maps them to duty cycle with a ~2.2 gamma curve and a small floor.
- **20–30 kHz** in the real box: inaudible, and it avoids banding on cameras and VR passthrough. If the gate edges are slow at 20 kHz, 5–10 kHz is still flicker-free.

## The bench rig

Before building a box, I wired one universal socket on a Mega 2560 with a protoshield:

<figure>
  <img src="/img/aux-lts-bench-rig-wiring.png" alt="Bench rig wiring: 2x13 socket to Arduino Mega, TIP42C interim dimmer, ATX breakout and star ground" loading="lazy" />
</figure>

| Ribbon pin | Mega |
|---|---|
| 1–2 LED+ | Dimmer output |
| GND pins | GND + ATX COM (star) |
| 7 +5 V | Mega 5V |
| 11–20 D1–D10 | D22–D31, `INPUT_PULLUP` |
| 22–26 A1–A5 | A0–A4, 100 nF to ground each |
| (PWM) | D9, Timer2 set to ~3.9 kHz |

The interim dimmer uses parts from my assortment kit: a **TIP42C PNP** high-side switch driven by a **2N7000**, at ~3.9 kHz (slow enough for a BJT). The real box gets the IRF9540Ns at 20–30 kHz.

The sketch maps the AUX LTS controls to their DCS-BIOS identifiers. I checked them against the A-10C module:

```cpp
DcsBios::Switch2Pos fireDetectTest("ALCP_FDBA_TEST", S_D(2));
DcsBios::Switch2Pos lampTest      ("LAMP_TEST_BTN",  S_D(3));
DcsBios::Switch3Pos nvisLts       ("ALCP_NVIS_LTS",  S_D(7) /*TOP*/, S_D(5) /*OFF*/);
DcsBios::Switch2Pos harsSas       ("ALCP_HARSSAS",   S_D(9), false);
DcsBios::Potentiometer refuelStatusLts("ALCP_RSIL",   S_A4, false, 0, 1023);
DcsBios::Potentiometer wpnStaLts      ("ALCP_WPNSTA", S_A2, true,  0, 1023);  // ends swapped on the board

// Console backlight follows the CONSOLE knob on the Light System Control Panel
DcsBios::Dimmer consoleBacklight(0x1370, LED_PWM_PIN, consoleGamma);
```

Two things to check on your own build: the module doesn't document the NVIS 3-position order (swap the pins if TOP and OFF come out reversed), and set `reverse` on HARS/SAS if it reads inverted. The `Potentiometer` min/max arguments let you calibrate the knob's marked DIM and BRT positions to 0 and 100%, and clamp anything beyond them.

## Box architecture

- One **powered USB hub per console**, inside the console. A single USB-C panel-mount coupler on the console back is the only cable to the PC.
- **Passive panels:** universal 2x13 sockets on USB-C Mega 2560 clones, about four sockets per Mega.
- **Dense panels:** their own USB-C board (Pro Micro or Mega 2560 Pro mini) on the hub. Only LED+ and ground come over the ribbon, and pin 7 stays open so box 5 V and USB 5 V never meet.
- **Left console count:** about eight passive sockets (two Megas) plus five dense boards: intercom, IFF, ARC-210, UHF, VHF FM.
