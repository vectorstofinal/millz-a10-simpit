---
title: "AUX LTS: wired, tested and stacked"
date: 2026-11-13
summary: "The switches and pots are wired, every connection checks out from the far end of the ribbon, and the painted top layer is on. It finally looks like an A-10 panel. One toggle went in backwards, and fixing it took one word of firmware."
tags: [panels, electronics, wiring, testing]
hero: /img/aux-lts-lit-panel.jpg
heroAlt: "Assembled AUX LTS panel backlit green through the diffuser: engraved legends glowing, pushbuttons fitted, REFUEL pot with a printed knob"
---

<div class="bottom-line">The AUX LTS panel is wired, tested end to end through the ribbon, and stacked with the painted top layer. Eleven of the fifteen hook-up wires are in; the last four go on the pushbuttons, which mount from the front. One toggle was wired with its outer lug backwards. That's a one-word fix in the DCS-BIOS sketch, because the common lug was right.</div>

Since [first light](/posts/aux-lts-first-light/), the panel has gone from a lit PCB to something that looks like it belongs in the jet.

<figure>
  <img src="/img/aux-lts-stacked-front.jpg" alt="AUX LTS panel assembled: three acrylic layers, painted top with engraved legends, REFUEL pot knob, NVIS and HARS/SAS toggles, pushbutton holes still empty" loading="lazy" width="1600" height="1200" />
  <figcaption>All three layers on. REFUEL wears the <a href="/posts/printed-dimmer-knob/">printed dimmer knob</a>. The two pushbutton holes are still empty, and one of the MS25089s is sitting on the mat to the right.</figcaption>
</figure>

## Hook-up wires

<figure>
  <img src="/img/aux-lts-wired-back.jpg" alt="Copper side of the AUX LTS PCB with the toggles and pots wired to their pads in 22 AWG solid wire, and the ribbon cable plugged into the header" loading="lazy" width="1600" height="1200" />
  <figcaption>The back. Each lug has a short wire to the pad next to it, plus two long runs that cross the PCB to pads beside the header. The grey printed part at the bottom left is the header shroud. It goes on last.</figcaption>
</figure>

- **Wire:** 22 AWG solid core. Stranded is more flexible, but it's hard to get into small holes. Solid is fine for wiring that never moves, as long as there's a small bend near each pad so a bump can't lever the joint and lift a milled pad.
- **The two long wires first:** the left pot's +5 V and the right pot's wiper both land on pads right next to the header, so they went in before anything else crowded the area.
- **Leave the pushbutton holes clear.** The MS25089 bodies pass through the PCB, so no wire crosses those holes.

### Toggles: measure, don't assume

I checked each toggle's terminals with a meter before wiring. Many toggles close the terminal on the *opposite* side from the lever. NVIS came out right. HARS/SAS I wired backwards: pin 19 closes at NORM instead of OVERRIDE.

Because the middle (common) terminal went to the right pad, the switch still works, just inverted. The fix is the `reverse` flag on its `Switch2Pos` line in the bench sketch. The rule: **get the common right, and the outer terminals can be fixed in firmware.** Swap a common with an outer terminal and no firmware can save you.

### Pots: why the readings top out at 5 kΩ

Both 10 kΩ pots share the +5 V pin, so measuring from a wiper to ground also runs through the other pot. Each wiper sweeps about 0 to 5 kΩ, not 0 to 10 kΩ, and +5 V to ground reads about 5 kΩ whatever the knobs are doing.

| Pot | Clockwise | Counter-clockwise |
|---|---|---|
| REFUEL (pin 25) | ~5.2 kΩ | 15 Ω |
| WPN STA (pin 23) | 17 Ω | ~5 kΩ |

WPN STA runs backwards because its ends were swapped to make the PCB routing easier. The sketch reverses it in software. The bottom of the travel looked a bit jumpy on the meter's ohms range, which is mostly the meter auto-ranging. Measuring voltage, with 5 V on the pot, is the better test, and it's what the Arduino actually reads.

## Testing from the far end of the ribbon

I tested from the far end of the cable so the ribbon itself gets checked too. Don't push meter probes into the socket; fat tips spread the contacts. **Push a spare long pin header into the socket instead** and probe that.

To find pin 1 on a header with no key: pins 1 and 2 beep to each other (they're tied together on the PCB for LED+), and pins 3, 4, 5, 6, 8 and 21 are all ground. Then I worked through each switch position, checking every signal pin each time, not just the one I expected to change. A closed switch read about 1.4 Ω through the switch, the ribbon and my meter leads.

## The top layer

The painted White Matte top lasered cleanly with the settings from the [paint tests](/settings/): legends filled at 1500 mm/s and 52% power, then the cut at 70% and 30 mm/s. With the diffuser and top layer on, the stack is 9 mm thick. M3×12 screws through the face leave 3 mm of thread in each standoff, which only works with short screws on the PCB side ([the screw-length trap](/posts/aux-lts-pcb-milled/#the-screw-length-trap)).

Before the middle layer goes on, the toggle and pot nuts have to be tight, because you can't reach them afterwards. And the two pushbutton nuts drop into their hex pockets in the middle layer before the top goes on.

## Lit through the full stack

<figure>
  <img src="/img/aux-lts-lit-panel.jpg" alt="AUX LTS panel lit green through the diffuser and painted top: all legends glowing evenly, two black MS25089 pushbuttons fitted, REFUEL pot with a printed knob" loading="lazy" width="1600" height="1330" />
  <figcaption>12 V into ribbon pins 1 (LED+) and 3 (ground), all 36 LEDs through the diffuser and the painted White Matte top. The pushbuttons are in.</figcaption>
</figure>

The legends light evenly, and the unlit face is a proper dark matte. Three things to clean up:

- **Light leaking around the switches.** Green rings show around both toggles and the WPN STA shaft. That's light escaping from the diffuser layer's cut edges behind each hole. The fix is to blacken those edges with a paint pen, or put a black washer under each nut.
- **Glow at the panel edges** and corner notches, for the same reason: the diffuser's outer edge needs blacking out too.
- **A few hot spots** over individual LEDs. A phone at full brightness exaggerates them, so I'll judge them dimmed in a dark room before scuffing any LED tips.

## Next

1. Screw in the two MS25089 pushbuttons from the front and wire their four leads.
2. Glue the printed header shroud on. Without it, the ribbon socket could go on backwards and put 12 V where it doesn't belong.
3. Plug into the bench rig and fly it against DCS.
