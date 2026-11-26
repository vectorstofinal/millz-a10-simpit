---
title: "Servo responding the fuel gauge changes in DCS"
date: 2024-01-25T17:35:00-05:00
summary: "One SG90 servo, powered off the Arduino, following the fuel quantity in DCS through DCS-BIOS."
tags: [electronics, dcs-bios, arduino]
---

<div class="note">Originally posted on my old blog, <a href="https://vectorstofinal.blogspot.com/2024/01/servo-responding-fuel-gauge-changes-in.html">Vectors To Final</a>, on January 25, 2024.</div>

Got a servo SG90 working, just powered off arduino since only running one - just needs one digital pin.  Seems to do just fine!

```cpp
//============

#include <Servo.h>
#define DCSBIOS_DEFAULT_SERIAL

#include "DcsBios.h"

DcsBios::ServoOutput fuelQtyL(0x10ca, 8, 544, 2400);

void setup() {
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}
```
