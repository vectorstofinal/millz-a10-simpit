// A-10C simpit — panel dimmer knob (AUX LTS, Lighting Control Panel, Fuel RCVR LT)
// For 6 mm knurled split-shaft B10K pots (18T, 1 mm slot ~6.9 deep, 8.5 mm shaft above bushing).
// Units: mm. Print skirt-down, PETG, 0.4 nozzle, no supports.
//
// Height logic: bushing tip sits at the back of the 3 mm top layer, so 8.5 - 3.0 = 5.5 mm of
// shaft stands above the panel face. The shaft tip seats on the bore ceiling (bore_depth),
// which leaves the skirt floating (5.5 - bore_depth) above the paint.
//
// Pointer logic (front view, clock angles measured clockwise from 12 o'clock):
//   - With the pot fully CCW (lugs at 6 o'clock), J measured the slot running 10–4 o'clock.
//   - The DIM/OFF tick on the AUX LTS face is at 7:30 (225°), BRT at 4:30 (135°).
//   - So the pointer (roof ridge + skirt triangle) sits at 225° while the rib sits at 300° / 120°.
//     Re-check on the fit ring; if it's off, change slot_clock only.

part        = "knob";   // "knob" | "rings" | "ring"
// ---- fit (tune from the fit-test rings) ----
bore_d      = 6.15;     // nominal bore; FDM PETG prints small holes ~0.1–0.2 under
rib_w       = 0.85;     // key rib that drops into the 1 mm shaft slot
bore_depth  = 5.0;      // shaft tip bottoms here -> 0.5 mm skirt gap
mouth_chamf = 0.5;      // lead-in at the bore mouth
// ---- pointer / slot alignment ----
slot_clock    = 300;    // slot direction at full CCW (10 o'clock end)
pointer_clock = 225;    // DIM/OFF tick (7:30)
// ---- body (matched to J's reference photos 2026-11-08) ----
// Two-tier MS-style knob: shallow conical skirt, fine-knurled cylindrical grip,
// low gable "roof" on top whose ridge lines up with the pointer, and a white
// triangle on the skirt slope, plus a smooth, slightly recessed band at the top edge. No groove on top.
skirt_d     = 26;
skirt_edge  = 0.8;      // vertical edge at the skirt rim
skirt_rise  = 2.0;      // cone rise from rim to grip (~24 deg)
grip_d      = 17;
grip_h      = 11.5;     // from the skirt cone top to the roof eaves (+2 mm on the knurl, J 2026-11-09)
roof_slope  = 8;        // degrees each side of the ridge
knurl_n     = 36;       // fine straight knurl
knurl_depth = 0.45;
knurl_z0    = 0.6;      // plain band above the skirt cone
// ---- top rim: smooth band, slightly recessed from the knurl crests (J's CONSOLE / AUX INST photos) ----
rim_h       = 1.5;      // band height below the eaves
rim_inset   = 0.8;      // radial step in from the knurl crests (0.35 below the knurl roots)
// ---- pointer triangle (recess on the skirt slope, fill with white paint) ----
tri_r0      = 9.3;      // base of the triangle (near the grip)
tri_r1      = 12.4;     // tip (near the skirt rim)
tri_w       = 2.6;
tri_depth   = 0.45;
$fn = 120;

cone_top  = skirt_edge + skirt_rise;
eave_z    = cone_top + grip_h;
rim_d     = grip_d - 2*rim_inset;
ridge_z   = eave_z + (rim_d/2) * tan(roof_slope);
total_h   = ridge_z;
function dir(clock) = [sin(clock), cos(clock)];

module skirt_solid(drop = 0) {
    translate([0, 0, -drop]) {
        cylinder(d = skirt_d, h = skirt_edge);
        translate([0, 0, skirt_edge]) cylinder(d1 = skirt_d, d2 = grip_d, h = skirt_rise);
    }
}

module grip_solid() {
    // cylinder up to the ridge, then cut the two roof planes
    rotate([0, 0, -pointer_clock])   // ridge runs along local +Y = pointer direction
    difference() {
        union() {
            cylinder(d = grip_d, h = eave_z - rim_h);   // knurled section ends in a flat step
            cylinder(d = rim_d, h = ridge_z);           // recessed smooth top band
        }
        for (s = [-1, 1])
            translate([0, 0, ridge_z]) rotate([0, s * roof_slope, 0])
                translate([s > 0 ? 0 : -rim_d, -rim_d, 0]) cube([rim_d, 2*rim_d, 10]);
    }
}

module knurl() {
    for (i = [0 : knurl_n - 1]) {
        a = i * 360 / knurl_n;
        rotate([0, 0, a]) translate([grip_d/2, 0, cone_top + knurl_z0])
            rotate([0, 0, 45]) translate([-knurl_depth/sqrt(2), -knurl_depth/sqrt(2), 0])
                cube([knurl_depth*sqrt(2), knurl_depth*sqrt(2), grip_h - knurl_z0 - rim_h]);
    }
}

module pointer_triangle() {
    intersection() {
        rotate([0, 0, -pointer_clock])
            linear_extrude(cone_top + 1)
                polygon([[-tri_w/2, tri_r0], [tri_w/2, tri_r0], [0, tri_r1]]);
        difference() { cylinder(d = skirt_d + 2, h = cone_top + 1); skirt_solid(tri_depth); }
    }
}

module bore() {
    difference() {
        union() {
            translate([0, 0, -1]) cylinder(d = bore_d, h = bore_depth + 1);
            translate([0, 0, -0.01]) cylinder(d1 = bore_d + 2*mouth_chamf, d2 = bore_d, h = mouth_chamf);
        }
        rotate([0, 0, -slot_clock])
            hull() {
                translate([-rib_w/2, -bore_d/2 - 1, 0.6]) cube([rib_w, bore_d + 2, bore_depth]);
                translate([-rib_w/4, -bore_d/2 - 1, 0.0]) cube([rib_w/2, bore_d + 2, 0.6]);
            }
    }
}

module knob() {
    difference() {
        union() { skirt_solid(); grip_solid(); }
        knurl();
        pointer_triangle();
        bore();
    }
}

// Fit-test ring: same bore, rib, depth and pointer as the knob, 12 mm OD, numbered on top.
module ring(bd, rw, label) {
    h = bore_depth + 1.2;
    difference() {
        cylinder(d = 12, h = h);
        translate([0, 0, 0]) {
            // reuse the bore with overridden fit values
            difference() {
                union() {
                    translate([0, 0, -1]) cylinder(d = bd, h = bore_depth + 1);
                    translate([0, 0, -0.01]) cylinder(d1 = bd + 2*mouth_chamf, d2 = bd, h = mouth_chamf);
                }
                rotate([0, 0, -slot_clock]) hull() {
                    translate([-rw/2, -bd/2 - 1, 0.6]) cube([rw, bd + 2, bore_depth]);
                    translate([-rw/4, -bd/2 - 1, 0.0]) cube([rw/2, bd + 2, 0.6]);
                }
            }
        }
        // pointer notch on the side, so alignment can be checked on a mounted pot
        rotate([0, 0, -pointer_clock]) translate([-0.6, 6 - 1.0, -1]) cube([1.2, 2, h + 2]);
        // number engraved on top
        translate([0, 0, h - 0.6]) linear_extrude(1)
            text(label, size = 4.2, halign = "center", valign = "center", font = "Liberation Sans:style=Bold");
    }
}

// 3 bores x 2 ribs. Labels: 1–3 = rib 0.75, 4–6 = rib 0.90; bores 6.00 / 6.15 / 6.30.
module rings() {
    bds = [6.00, 6.15, 6.30]; rws = [0.75, 0.90];
    for (j = [0:1]) for (i = [0:2])
        translate([i * 16, -j * 16, 0]) ring(bds[i], rws[j], str(j*3 + i + 1));
}

if (part == "knob")  knob();
if (part == "rings") rings();
if (part == "ring")  ring(bore_d, rib_w, "X");
