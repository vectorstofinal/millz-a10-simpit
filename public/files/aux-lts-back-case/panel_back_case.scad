// Panel back case — Millz A-10 SimPit
// One script for every panel. Only the PANEL block changes.
//
// Coordinates: the panel's PCB CNC DXF ("back view", copper side up), in mm.
// z = 0 is the PCB copper surface; the case rises toward the back of the panel.
// The case screws onto the panel's 4 standoffs: M3 x 8 through each 4 mm boss,
// replacing the M3 x 4 PCB screws (2.4 mm of thread in the standoff, as before).
//
// Print floor-down (set print_orientation = true), PETG, no supports.

/* ===== PANEL: AUX LTS ===== */
outline_dxf   = "aux-lts-pcb-v2-trimmed-outline.dxf";   // corners trimmed for the deck ears (2026-11-14)
outline_layer = "5_OUTLINE_TRIMMED";
standoffs     = [[94.0, 116.98], [55.21, 116.98], [34.8, 19.6], [118.25, 21.39]];
header_center = [76.525, 15.963];   // 2x13 header (shroud 41 x 11.8)
header_open   = [50, 22];           // opening around the shroud + ribbon socket (x, y)
header_to_edge = -1;                // -1: notch runs to the low-y edge, +1: to the high-y edge
inner_depth   = 28;                 // clear height above the copper (J: 28 mm clears everything)
qr_pocket_center = [76.5, 70];      // recess for a QR tag on the outside of the floor
/* ========================== */

wall        = 2.0;    // side walls
floor_t     = 2.0;    // floor (back face)
edge_inset  = 0.5;    // case outline inset from the PCB outline
boss_d      = 10;     // screw column outer diameter
boss_bore   = 6.5;    // screwdriver + head clearance down the column
boss_base   = 4.0;    // solid plate under the screw head (sits on the PCB)
screw_hole  = 3.4;    // M3 clearance
vent_len    = 30;
vent_w      = 3;
qr_pocket   = [42, 42, 0.8];   // fits a 40 mm tag, 0.8 mm deep
print_orientation = true;

$fn = 48;
H = inner_depth + floor_t;

module outline2d(inset) { offset(delta = -inset) import(outline_dxf, layer = outline_layer); }

module shell() {
    difference() {
        linear_extrude(H) outline2d(edge_inset);
        translate([0, 0, -0.01]) linear_extrude(inner_depth + 0.01) outline2d(edge_inset + wall);
    }
}

module bosses() {
    for (p = standoffs) translate([p[0], p[1], 0]) cylinder(d = boss_d, h = H);
}

module boss_holes() {
    for (p = standoffs) translate([p[0], p[1], 0]) {
        translate([0, 0, -1]) cylinder(d = screw_hole, h = H + 2);       // screw
        translate([0, 0, boss_base]) cylinder(d = boss_bore, h = H);    // head + driver access
    }
}

module header_notch() {
    // Opening through floor and wall around the header, running out to the PCB edge.
    w = header_open[0]; d = header_open[1];
    reach = 200;
    translate([header_center[0] - w/2, 0, -1])
        if (header_to_edge < 0)
            translate([0, header_center[1] + d/2 - reach, 0]) cube([w, reach, H + 2]);
        else
            translate([0, header_center[1] - d/2, 0]) cube([w, reach, H + 2]);
}

module vents() {
    // Two groups of slots in the floor, either side of the QR pocket, clear of the bosses.
    for (side = [-1, 1]) for (i = [0 : 3]) {
        x = qr_pocket_center[0] + side * (qr_pocket[0] / 2 + 8 + i * 7);
        translate([x - vent_w/2, qr_pocket_center[1] - vent_len/2, H - floor_t - 1]) cube([vent_w, vent_len, floor_t + 2]);
    }
}

module qr_recess() {
    translate([qr_pocket_center[0] - qr_pocket[0]/2, qr_pocket_center[1] - qr_pocket[1]/2, H - qr_pocket[2]])
        cube([qr_pocket[0], qr_pocket[1], qr_pocket[2] + 1]);
}

module case_body() {
    difference() {
        intersection() {
            union() { shell(); bosses(); }
            linear_extrude(H) outline2d(edge_inset);   // keep bosses inside the outline
        }
        boss_holes();
        header_notch();
        vents();
        qr_recess();
    }
}

if (print_orientation) translate([0, 0, H]) rotate([180, 0, 0]) case_body();   // floor down (a rotation, not a mirror)
else case_body();
