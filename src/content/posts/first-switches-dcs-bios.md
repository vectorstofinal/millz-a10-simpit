---
title: "Where it started: first switches on DCS-BIOS"
date: 2024-01-16
summary: "Two weeks of breadboard work in January 2024: DCS-BIOS talking to an Arduino, a pot, a rotary encoder and an LCD. Plus the gotchas that cost me an evening each."
tags: [electronics, dcs-bios, arduino]
---

<div class="bottom-line">DCS-BIOS works on the first try once you know the rules. Don't call <code>Serial.begin()</code>, don't put a <code>delay()</code> in <code>loop()</code>, and wire switches to ground, not +5 V.</div>

This is the oldest entry in the log. Back in January 2024 I had a Mega on the bench and wanted to see a physical switch move something in the A-10C. I followed The Warthog Project's DCS-BIOS video and had a switch working the first night.

## Setup that worked

- **DCS-BIOS and the Arduino library from the DCS-Skunkworks org on GitHub.** Not the other fork, and I didn't use the FlightPanels tooling.
- **COM ports.** Find the Arduino's port in Device Manager under *Ports*. Edit `multiple-com-ports.cmd` in the DCS-BIOS `Programs` folder with your port numbers and run it. The first run unzips socat; run it again to confirm it connects. The console window isn't chatty, but it works.
- **Bort** (also from DCS-Skunkworks) is the control reference browser. Grab the latest exe from the releases; the file called `...setup.exe` *is* the program, not an installer. Point it at the JSON folder inside your DCS-BIOS install.
- **Windowed DCS.** Set DCS to a resolution below your monitor's and you get a window you can keep next to Bort.

## Gotchas

- **No `Serial.begin()` in your sketch.** DCS-BIOS owns the serial port. Opening it yourself breaks the comms.
- **No `delay()` in `loop()`.** DCS-BIOS has to run every pass.
- **Switches go from GROUND to the pin**, using the internal pull-up. Not from +5 V.
- **The COM script fails if the Arduino IDE has the port open.** The error shows a port number one lower than the one you typed (it's zero-indexed), which is confusing the first time.
- **You can hot-swap code.** Stop the DCS-BIOS script, upload from the IDE, restart the script. DCS can keep running the whole time.
- **Bort doesn't refresh every value both ways.** Some do, some don't. Don't use it to judge whether your output code works.
- **Cheap encoders skip.** I had to set the ARC-210 frequency encoder to 2 steps per detent instead of the default 4. Better encoders are on the list.

## The test sketch

One pot (UHF volume), one toggle (AHCP TGP), a pushbutton (lamp test), the MASTER CAUTION LED, an encoder on the ARC-210 10 MHz selector, and a 16x2 LCD showing the ARC-210 COMSEC mode and frequency strings. The pattern for string outputs:

```cpp
#define DCSBIOS_DEFAULT_SERIAL
#include "DcsBios.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

DcsBios::Potentiometer uhfVol("UHF_VOL", A0);
DcsBios::Switch2Pos ahcpTgp("AHCP_TGP", 5);
DcsBios::LED masterCaution(0x1012, 0x0800, 6);
DcsBios::RotaryEncoderT<POLL_EVERY_TIME, 2> arc210_10mhz(
    "ARC210_10MHZ_SEL", "DEC", "INC", 2, 3);   // 2 steps/detent for cheap encoders

void onArc210FrequencyChange(char* v) {
  lcd.setCursor(0, 1);
  lcd.print("                ");   // clear the row
  lcd.setCursor(0, 1);
  lcd.print(v);
}
DcsBios::StringBuffer<7> arc210Freq(0x1382, onArc210FrequencyChange);

void setup() { DcsBios::setup(); lcd.begin(16, 2); }
void loop()  { DcsBios::loop(); }
```

## The plan back then

I planned to use joystick-style HID for the "dumb" switches so they'd work in other aircraft too, and DCS-BIOS for anything two-way (lights, displays). That changed: the whole pit now runs on DCS-BIOS, because it keeps the switches and lights in sync with the jet. The [connector standard post](/posts/panel-connector-standard/) has the current architecture.

Later that month I printed The Warthog Project's standby compass and started on backlighting. One printing tip from that: organic/tree supports are great, but orient the part so the top of each tree has a *small* contact patch. If the tree flares out where it meets the part, you'll never break it loose. Flexing the supports with pliers works well.
