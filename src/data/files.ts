// Every downloadable file on the site. Posts reference these by id in their `files:` frontmatter.
// Rule: only original work, or derivatives of sources released for reuse (The Warthog Project).
// Never add anything from the Vr-pits (FLIM) plan set or anything derived from it.

export type Origin = 'original' | 'twp-derivative';

export interface SiteFile {
  id: string;
  path: string;        // under /public
  name: string;
  kind: string;        // file type label
  group: string;
  desc: string;
  origin: Origin;
  view3d?: boolean;    // STL with an inline viewer
}

export const groups = [
  'AUX LTS panel (acrylic)',
  'AUX LTS board (PCB)',
  'Laser test files',
  'Printed parts',
  'Controller box',
];

export const files: SiteFile[] = [
  // AUX LTS panel
  { id: 'aux-top-dxf', path: '/files/aux-lts-panel/aux-lts-1-top.dxf', name: 'AUX LTS 1 TOP.dxf', kind: 'DXF (mm)', group: 'AUX LTS panel (acrylic)', origin: 'twp-derivative',
    desc: 'Top layer. Layers CUT, ENGRAVE (legends, arrows, ticks, HARS box) and KNOB_REF_DO_NOT_CUT. Centered on 0,0.' },
  { id: 'aux-mid-dxf', path: '/files/aux-lts-panel/aux-lts-2-middle.dxf', name: 'AUX LTS 2 MIDDLE.dxf', kind: 'DXF (mm)', group: 'AUX LTS panel (acrylic)', origin: 'twp-derivative',
    desc: 'Diffuser layer with 36 x 3.4 mm back-face LED pockets and the two captive hex pockets for the MS25089 nuts. Mirror it and cut back face up.' },
  { id: 'aux-base-dxf', path: '/files/aux-lts-panel/aux-lts-3-base.dxf', name: 'AUX LTS 3 BASE.dxf', kind: 'DXF (mm)', group: 'AUX LTS panel (acrylic)', origin: 'twp-derivative',
    desc: 'Black base: 36 x 3.0 mm LED press-fit holes, 1.8 x 2.6 mm pot anti-rotation slots, switch holes.' },
  { id: 'aux-all-dxf', path: '/files/aux-lts-panel/aux-lts-all-layers-edited.dxf', name: 'AUX LTS - all layers (edited).dxf', kind: 'DXF (mm)', group: 'AUX LTS panel (acrylic)', origin: 'twp-derivative',
    desc: 'All three sheets in one file with every edit from the build post (not just the MS25089 holes).' },
  { id: 'aux-top-lbrn', path: '/files/aux-lts-panel/aux-lts-1-top-white-matte-painted.lbrn2', name: 'AUX LTS 1 TOP (White Matte, painted).lbrn2', kind: 'LightBurn', group: 'AUX LTS panel (acrylic)', origin: 'twp-derivative',
    desc: 'Production job for the painted top. C01 legends Fill 1500 mm/s 52% 0.05 mm bi-dir; C00 cut 70% @ 30 mm/s. Painted face up, not mirrored.' },
  { id: 'aux-mid-lbrn', path: '/files/aux-lts-panel/aux-lts-2-middle-mirrored.lbrn2', name: 'AUX LTS 2 MIDDLE (mirrored).lbrn2', kind: 'LightBurn', group: 'AUX LTS panel (acrylic)', origin: 'twp-derivative',
    desc: 'Middle layer job I actually ran. C01 pockets Fill 200 mm/s 70% x2; C00 cut 70% @ 30 mm/s.' },
  { id: 'aux-base-lbrn', path: '/files/aux-lts-panel/aux-lts-3-base-black.lbrn2', name: 'AUX LTS 3 BASE (black).lbrn2', kind: 'LightBurn', group: 'AUX LTS panel (acrylic)', origin: 'twp-derivative',
    desc: 'Base job: LED holes first, then holes, slots and outline. Note: this one cuts Y-mirrored (see the laser post). It still fits flipped over.' },

  // PCB
  { id: 'pcb-vcarve', path: '/files/aux-lts-pcb/aux-lts-pcb-v2-vcarve-job-250x130.dxf', name: 'AUX LTS PCB v2 - VCarve job (250x130 blank).dxf', kind: 'DXF (mm)', group: 'AUX LTS board (PCB)', origin: 'original',
    desc: 'Import this one. Board layers 1-5 plus a test coupon (C1-C5), four Z-check rings and a blank reference rectangle. Copper side up.' },
  { id: 'pcb-cnc', path: '/files/aux-lts-pcb/aux-lts-pcb-v2-cnc-back-view.dxf', name: 'AUX LTS PCB v2 - CNC (back view).dxf', kind: 'DXF (mm)', group: 'AUX LTS board (PCB)', origin: 'original',
    desc: 'Board only, as milled (mirrored). Layers: 1 isolate 0.4 mm, 2 drill 1.0, 3 drill 3.2, 4 cutouts, 5 outline.' },
  { id: 'shroud-stl', path: '/files/aux-lts-pcb/idc-2x13-keyed-shroud.stl', name: 'IDC 2x13 keyed shroud.stl', kind: 'STL', group: 'AUX LTS board (PCB)', origin: 'original', view3d: true,
    desc: 'Printed shroud glued over the extra-long header pins on the copper side. 41 x 11.8 x 9 mm, cavity 38.4 x 9.2.' },

  // Laser tests
  { id: 'fit-strip', path: '/files/laser-tests/aux-lts-fit-test-strip.dxf', name: 'AUX LTS fit test strip.dxf', kind: 'DXF (mm)', group: 'Laser test files', origin: 'original',
    desc: 'Cut this first. LED, pot, toggle, MS25089 and M3 holes at a few sizes so you can check fits before cutting a panel.' },
  { id: 'paint-lettering', path: '/files/laser-tests/paint-lettering-test-3x5.lbrn2', name: 'Paint lettering test (LAMP TEST 3x5).lbrn2', kind: 'LightBurn', group: 'Laser test files', origin: 'original',
    desc: '15 copies of "LAMP TEST" in MS33558 (outlines), 10-30% x 300-600 mm/s. Engrave-through-paint ladder.' },
  { id: 'paint-banding', path: '/files/laser-tests/paint-banding-test-5x2.lbrn2', name: 'Paint banding test (LAMP 5x2).lbrn2', kind: 'LightBurn', group: 'Laser test files', origin: 'original',
    desc: 'Line interval vs scan direction at 600 mm/s and ~0.40 J/mm². Finds whether backlit banding is line pitch or scan offset.' },
  { id: 'paint-speed', path: '/files/laser-tests/paint-speed-test-5x2.lbrn2', name: 'Paint speed test (LAMP 5x2).lbrn2', kind: 'LightBurn', group: 'Laser test files', origin: 'original',
    desc: '600-1500 mm/s at two energy densities, 0.05 mm bi-dir. Where the 1500 mm/s / 52% production setting came from.' },

  // Printed parts
  { id: 'knob-stl', path: '/files/dimmer-knob/dimmer-knob-26mm.stl', name: 'Dimmer knob 26mm.stl', kind: 'STL', group: 'Printed parts', origin: 'original', view3d: true,
    desc: 'v6 dimmer knob for 6 mm knurled split-shaft B10K pots. Print skirt-down in PETG, no supports.' },
  { id: 'knob-scad', path: '/files/dimmer-knob/dimmer_knob.scad', name: 'dimmer_knob.scad', kind: 'OpenSCAD', group: 'Printed parts', origin: 'original',
    desc: 'Parametric source. Bore, key rib, pointer angle, grip height, knurl count are all variables at the top.' },

  // Controller box
  { id: 'bench-ino', path: '/files/controller-box/AUX_LTS_bench.ino', name: 'AUX_LTS_bench.ino', kind: 'Arduino', group: 'Controller box', origin: 'original',
    desc: 'DCS-BIOS sketch for a Mega 2560: AUX LTS on one universal 2x13 socket, plus a gamma-mapped high-side backlight dimmer.' },
  { id: 'bench-wiring', path: '/img/aux-lts-bench-rig-wiring.png', name: 'AUX LTS bench rig - wiring.png', kind: 'PNG', group: 'Controller box', origin: 'original',
    desc: 'Wiring for the bench rig: socket to Mega pins, TIP42C interim dimmer, ATX breakout, star ground.' },
];

export const byId = (id: string) => files.find((f) => f.id === id);

export const originLabel: Record<Origin, string> = {
  'original': 'Original · CC0',
  'twp-derivative': 'Based on The Warthog Project',
};
