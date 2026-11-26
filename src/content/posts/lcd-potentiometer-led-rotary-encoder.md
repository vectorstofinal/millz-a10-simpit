---
title: "LCD, Potentiometer, LED, Rotary Encoder all working with DCS-BIOS"
date: 2024-01-16T17:23:00-05:00
summary: "What I learned getting a pot, a switch, an LED, a rotary encoder and an LCD working with DCS-BIOS, and the sketch."
tags: [electronics, dcs-bios, arduino]
hero: /img/vtf-lcd-breadboard.jpg
---

<div class="note">Originally posted on my old blog, <a href="https://vectorstofinal.blogspot.com/2024/01/lcd-potentiometer-led-rotary-encoder.html">Vectors To Final</a>, on January 16, 2024.</div>

Learned a lot today trying to get a pot going:

- DO NOT have `Serial.begin()` in the hardware code as that seems to disrupt the DCS BIOS comms
- DO NOT have a `delay` in the loop func - DCS BIOS can't do its thing
- When you run multiple-com-ports.cmd watch for errors - if you have the IDE connected to the device the script will fail to connect since it is tied up - it will errors with a message about the device (but the com number will be 1 lowered that what you put in the script itself, must be zero-indexed)
- Wire switches from GROUND to a digital pin, not from +5v
- YOU CAN stop the bios cmd, deploy new code via IDE, and then restart BIOS while the game is running, so that is helpful
- The BORT tool is called ...setup.exe - that is the binary itself, not a setup program
- BORT does not appear to update all its values shown on-demand, seems to not be 2-way comms with BIOS - though SOME vals do work 2-way
- The rotary encoder I was using would occasionally skip input or put out a double input.  It is just a lower quality unit.

<figure>
  <img src="/img/vtf-lcd-breadboard.jpg" alt="Close-up of a small OLED display module showing a pixel silhouette of an A-10" loading="lazy" width="1269" height="1128" />
  <figcaption>Smaller OLED, not the text-based one used in the code</figcaption>
</figure>

```cpp
//=================================================
/*

  The circuit:
 * LCD RS pin to digital pin 7
 * LCD Enable pin to digital pin 8
 * LCD D4 pin to digital pin 9
 * LCD D5 pin to digital pin 10
 * LCD D6 pin to digital pin 11
 * LCD D7 pin to digital pin 12
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 OTHER STUFF TOO

*/


#define DCSBIOS_DEFAULT_SERIAL

#include "DcsBios.h"
#include <LiquidCrystal.h>

#define POT_PIN A0
#define MASTER_LED 6

DcsBios::Potentiometer uhfVol("UHF_VOL", POT_PIN);
DcsBios::Switch2Pos ahcpTgp("AHCP_TGP", 5);
DcsBios::Switch2Pos lampTestBtn("LAMP_TEST_BTN", 4); // rote enc switch
DcsBios::LED masterCaution(0x1012, 0x0800, MASTER_LED);
DcsBios::RotaryEncoderT<POLL_EVERY_TIME, 2> arc21010mhzSel("ARC210_10MHZ_SEL", "DEC", "INC", 2, 3); // setting steps to 2 (in brackets) instead of default 4 -
          //get proper encoders to avoid this and other skip issues

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

//-------

void onArc210ComsecModeChange(char* newValue) {
  lcd.setCursor(0,0);
  lcd.print("                "); // 16 blanks to clear row
  lcd.setCursor(0,0);
  lcd.print(newValue);
}

DcsBios::StringBuffer<11> arc210ComsecModeBuffer(0x12ee, onArc210ComsecModeChange);

//------

void onArc210FrequencyChange(char* newValue) {
  lcd.setCursor(0,1); // col, row - zero index
  lcd.print("                "); // 16 blanks to clear row
  lcd.setCursor(0,1); // col, row - zero index
  lcd.print(newValue);
}

DcsBios::StringBuffer<7> arc210FrequencyBuffer(0x1382, onArc210FrequencyChange);

//------

void setup() {
  DcsBios::setup();

    lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Hello, World!");
}

void loop() {
  DcsBios::loop();
}
```
