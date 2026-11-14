---
title: "AUX LTS: first controller test, and it works in DCS"
date: 2026-11-14
summary: "The AUX LTS panel went onto an Arduino Mega through its ribbon cable. All seven signals read correctly, and after three sketch versions the real panel drives the one in the sim, including syncing its switch positions at startup."
tags: [panels, arduino, dcs-bios, controller-box]
hero: /img/aux-lts-dcs-first-test.jpg
heroAlt: "DCS A-10C left console, looking down at the AUX LTS panel with its HARS/SAS and NVIS LTS toggles, two dimmer knobs and two test buttons"
files: [aux-lts-dcsbios-ino]
---

<div class="bottom-line">The AUX LTS panel now moves its twin in DCS. Two pots, two pushbuttons and two toggles all read correctly on a Mega through the ribbon cable. Four things needed fixing in the sketch, none in the wiring: both toggles were backward, the buttons had a small delay, the sim didn't pick up my switch positions at startup, and the knobs moved in slow motion. Version 3 of the sketch fixes all four.</div>

Last time I [trimmed the PCB corners and printed a back case](/posts/aux-lts-trimmed-back-case/). The last item on that list was to plug the panel in and fly it against DCS. That happened today.

## Step 1: plain serial test, no sim

Before involving DCS I wanted to know the panel itself was right. So the first sketches just printed readings to the Arduino serial monitor.

I started with four jumpers from the far end of the ribbon to the Mega: ground, +5 V, and the two pot wipers. Both pots swept the full 0 to 1023. The WPN STA pot reads backward (high at full counter-clockwise), which I expected, because its ends are swapped on the PCB to make the routing easier. That gets flipped in the sketch.

Then five more jumpers for the switches:

| Ribbon pin | Control | Mega pin |
|---|---|---|
| 12 | FIRE DETECT test button | D22 |
| 13 | SIGNAL LIGHTS lamp test button | D24 |
| 15 | NVIS LTS toggle | D26 |
| 17 | NVIS LTS toggle | D28 |
| 19 | HARS/SAS toggle | D30 |
| 23 | WPN STA pot | A1 |
| 25 | REFUEL STATUS pot | A2 |

Both buttons read 1 only while pressed. The HARS/SAS toggle closes in the NORM position. The NVIS toggle worked but read backward against the pad labels on my PCB. No rewiring for any of it. Every one of these is a one-line fix in the sketch.

## Step 2: DCS-BIOS

DCS doesn't talk to an Arduino by itself. The chain is DCS, then the DCS-BIOS export script, then a small bridge program on the PC, then the COM port. I had forgotten the bridge step, and with it missing nothing happens at all. I'm using DCS-BIOS Bridge. Pick the COM port, switch it on, and the traffic counters start moving.

<figure>
  <img src="/img/dcs-bios-bridge-com3.png" alt="DCS-BIOS Bridge window showing COM3, an Arduino Mega 2560, with write and read counters and the last message ALCP_RSIL 20752" loading="lazy" width="877" height="510" />
  <figcaption>The bridge, connected to the Mega. The last message is the REFUEL STATUS pot reporting its position.</figcaption>
</figure>

Two things to remember. The Arduino serial monitor has to be closed, because it holds the COM port. And the bridge connection has to be off to upload a new sketch. Turning it back on afterward just worked, with DCS still running.

## What version 1 got wrong

<figure>
  <img src="/img/aux-lts-dcs-first-test.jpg" alt="DCS A-10C left console, looking down at the AUX LTS panel" loading="lazy" width="1600" height="900" />
  <figcaption>The panel in the sim during the test. This is the one my hardware is now driving.</figcaption>
</figure>

Everything moved in the sim on the first try, but:

- **Both toggles were backward.** DCS-BIOS doesn't document which end of a switch is 0. Now I know for this panel: on NVIS LTS, 0 is OFF and 2 is TOP. On HARS/SAS, 1 is OVERRIDE.
- **The lamp test button had a slight delay**, more than clicking the same button with the mouse. The DCS-BIOS library waits 50 ms for a switch to settle before it sends anything. I cut that to 10 ms for the two pushbuttons.
- **The sim didn't match my switches at startup.** The library only sends a control when it changes. So if a toggle is already in OVERRIDE when the mission starts, DCS never hears about it.

## Version 2

The fix for the startup problem: the sketch watches the data coming from DCS. About 3 seconds after that data starts, it makes every control send its current position once. It does the same again after any gap in the data longer than 3 seconds.

That worked better than I expected. The sim picked up my switch positions at mission start, and again when I went out to the game menu and came back.

## Version 3: knobs in slow motion

One more thing showed up after that. The two dimmer knobs moved in slow motion in the sim compared to my hand.

By default the DCS-BIOS library reads a pot on every pass through its loop and sends a message for every small change. Turning a knob can produce hundreds of messages a second, and the sim knob ends up playing back a backlog. Version 3 reads each pot every 20 ms, so a turn sends at most about 50 messages a second, with a slightly coarser step and lighter smoothing. The knobs are much, much better now.

Starting with this one, every sketch carries a version number in its header that goes up with each change. The download below is version 3.

## Next

1. Wire the backlight dimmer and a stand-in CONSOLE knob on the bench, and turn that part of the sketch on.
2. Cut the deck plate so the panel can go into the console.
