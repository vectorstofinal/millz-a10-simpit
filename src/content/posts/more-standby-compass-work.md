---
title: "More Standby Compass work"
date: 2024-01-26T17:37:00-05:00
summary: "Filled, sanded and painted the compass box, and tested the PSU breakout board with 50 green LEDs on 12 V."
tags: [standby-compass, electronics, backlighting]
hero: /img/vtf-compass-box.jpg
---

<div class="note">Originally posted on my old blog, <a href="https://vectorstofinal.blogspot.com/2024/01/more-standby-compass-work.html">Vectors To Final</a>, on January 26, 2024.</div>

Did the filling, sanding and painting of the box today.

<figure>
  <img src="/img/vtf-compass-box.jpg" alt="The painted standby compass box with its front bezel screwed on, before the compass parts go in" loading="lazy" width="1269" height="1472" />
</figure>

Got the computer power supply [breakout board](https://www.amazon.com/gp/product/B01NBU2C64/) working and tested it by powering [50 green LEDs](https://www.amazon.com/gp/product/B074SY1KP5/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&th=1) with 12v.  These LEDs have a resistor built-in so I ran them all in parallel.  They pulled about 13 mA at steady state, which is well under the typical 20 mA limit.  They were super bright.

Played with [an Arduino sim here](https://wokwi.com/projects/388099220969869313) - using wrong driver board - to try to think though the wiring.
