---
title: "A10-C Standby Compass: Final Setup"
date: 2024-02-07T18:03:00-05:00
summary: "Everything soldered and squeezed into the case, an acrylic front glass, and the aerial refueling LEDs wired."
tags: [standby-compass, electronics, wiring, 3d-printing]
---

<div class="note">Originally posted on my old blog, <a href="https://vectorstofinal.blogspot.com/2024/02/a10-c-standby-compass-final-setup.html">Vectors To Final</a>, on February 7, 2024.</div>

<figure>
  <img src="/img/vtf-compass-final-1.jpg" alt="The finished standby compass in its case with the card lit green behind the front glass, and the LED box under it lit blue, amber and red" loading="lazy" width="1269" height="1599" />
</figure>

Got everything soldered together (after likely damaging one EasyDriver and moving to a backup) and squeezed it into the case.  At first I thought the compass wheel was much too high but then I realized the front bezel was installed upside down.

Used an Exacto knife to cut a piece of acrylic for the front glass and put a small vertical sticker on it to act as the reference line.  Not super pretty but works ok until I can cut with a laser instead (maybe an xTool).

Getting everything to fit was trickier than I expected, but it all worked out without too much rework required.  I used some 5.5mm x 2.1mm plugs for the 12v power plugs (tip/center was positive).

For the aerial refueling LEDs I used some more LEDs that have resistors built-in.  They are made for 12v so will likely bit a little dim since I am powering them off the digital out 5v pins from the Nano.  Don't have a way to make the lens for those yet so will wait to see if that is the case.  So far so good though!

Since I had about 8 wires routed out of the compass box, once it was all setup, I lost track of which wire was which.  Had to pull the guts out again and zip tie groups of wires before reinstalling things.  Haven't found a good way to label these tiny 22AWG wires yet.

Designed a [3D printed back cover](https://github.com/vectorstofinal/simpit/blob/main/Cover%20for%20aerial%20refuel%20LED%20box%20A-10%20v1.f3d) for the AR LEDs (in Fusion 360).

<figure>
  <img src="/img/vtf-compass-final-2.jpg" alt="The back of the compass case with LIGHT and MOTOR labels beside the two power plugs, and the USB cable coming out below" loading="lazy" width="1195" height="1600" />
</figure>
