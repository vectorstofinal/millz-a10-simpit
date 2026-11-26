---
title: "First time trying DCS-BIOS to talk to DCS sim"
date: 2024-01-01T17:10:00-05:00
summary: "DCS-BIOS controlling switches through an Arduino on the first try, plus notes on the COM port script and Bort."
tags: [electronics, dcs-bios, arduino]
---

<div class="note">Originally posted on my old blog, <a href="https://vectorstofinal.blogspot.com/2024/01/first-time-trying-dcs-bios-to-talk-to.html">Vectors To Final</a>, on January 1, 2024.</div>

Used the warthog proj video to learn how to use DCS-BIOS to control the switches.  Worked on the first try!

Found the Arduino com port in Device Manager under "Ports..."

Updated the "C:\Users\vecto\Saved Games\dcs-bios-master\Programs" file multiple-com-ports (to update the com ports for my Arduinos) and then let it run - the first time it unzipped socat for me, and then I re-ran the cmd to make sure it worked.

The cmd window did not have clear messages but it was working

Used [Bort](https://github.com/DCS-Skunkworks/Bort) as well, from the same org in github (not the other fork)

Don't download the code, find the latest exe in there to grab

It needs to know where the json dir is within DCSBios - I had that in Saved Games so pointed it there

<https://www.youtube.com/watch?v=LsAnP6H9BMU>

used the dcs-bios and arduino lib from that same shunkworks org - did not use FlightPanels thing

Based on warthog's notes, I plan to use joystick style coding for all dumb switches so they can work in other apps and aircraft.  Will use bios for two way comms, like lights, and more complicated stuff - even though I like coding for it better than the joystick style

I found that if I set my resolution in DCS lower than the monitor I could get a "windowed" version that was easier to use alongside Bort's window.
