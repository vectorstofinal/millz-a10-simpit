---
title: "Standby Compass: StepperTest"
date: 2024-01-31T12:18:00-05:00
summary: "The X27-168 stepper running from an EasyDriver on the breadboard, with the pin connections and things learned."
tags: [standby-compass, electronics, arduino, testing]
---

<div class="note">Originally posted on my old blog, <a href="https://vectorstofinal.blogspot.com/2024/01/standby-compass-servo-test.html">Vectors To Final</a>, on January 31, 2024.</div>

Yesterday I finally received the EasyDriver stepper driver and the X27-168 steppers(no more AliExpress orders if I can help it - took weeks to arrive).

I opened up the motor and removed the tiny plastic tab that was keeping it from spinning 360.  I soldered headers on one of the EasyDrivers for prototyping and soldered some solid core wire to the little leads on the motor.

After wiring it all up on the breadboard I used [this code](https://github.com/sparkfun/Easy_Driver/blob/master/Firmware/SparkFun_Easy_Driver_Basic_Demo/SparkFun_Easy_Driver_Basic_Demo.ino) to see it run.

For the connections: you need to put one coil's wire pair on the A pins for the EasyDriver and the other coil's on the B pins.  There is no polarity so the A coil wires can go on plus or minus.  I put the 12v power on the M+ and GND pin pair.  Then these as well:

```
// EasyDriver -> Nano
stp -> 2
dir -> 3
MS1 -> 4
MS2 -> 5
EN  -> 6
GND (by step pin) -> GND
```

**Do not connect/disconnect the motor when power is applied**. Power is constantly flowing to keep motor still and changing connection when powered will kill EasyDriver.

Things learned:

- Where I stored my pin vise (took a bit to find it) 😀😀
- Dupont connectors on breadboard wires DO NOT accept solder
- The [OmniFixo](https://omnifixo.com/) seems to be working well for me
- A sharp utility knife is a good way to split a row of header pins into smaller groups - just make sure to hold down each side when you cut else things go flying

<figure>
  <img src="/img/vtf-stepper-test.jpg" alt="An X27-168 stepper hanging by its wires above a breadboard holding an Arduino Nano and a red EasyDriver, with the ATX breakout at the right" loading="lazy" width="1269" height="952" />
</figure>
