/*
  A-10C simpit - AUX LTS panel, DCS-BIOS sketch (Mega carrier v1.2, panel on socket S1)

  SKETCH VERSION: 4   (2026-11-25)
  Version rule: this number goes up by one every time this sketch is changed. Never reuse a number.
  History:
    v1  2026-11-14  First DCS-BIOS version for the as-wired bench hookup. Based on
                    AUX_LTS_bench_local.ino (unversioned, 2026-11-12), with the pin map changed to
                    J's jumpers, NVIS pins exchanged per the serial test, and the backlight section
                    made optional (off) because no dimmer or CONSOLE pot is wired yet.
    v2  2026-11-14  After the first DCS test: NVIS pin order exchanged and HARS/SAS reversed (both
                    read backward in the sim); pushbutton debounce cut from 50 ms to 10 ms;
                    hardware state is re-sent to DCS about 3 s after the export stream starts.
    v3  2026-11-14  Pots: knobs moved in slow motion in the sim. Pot messages are now rate-limited
                    (poll every 20 ms instead of every loop pass), with lighter smoothing and a
                    coarser send step, so a turn sends at most about 50 messages per second per pot.
    v4  2026-11-25  Pin map changed from the bench jumpers to the Mega carrier v1.2 with the panel's
                    ribbon in socket S1. Two pins move: FIRE DETECT TEST D22 -> D23 and REFUEL
                    STATUS A2 -> A3 (the bench jumpers for those two were one pin off the
                    socket standard). Everything else is unchanged from v3. The optional
                    backlight section still uses the bench pins and now refuses to compile.

  Board : Arduino Mega 2560
  Lib   : DCS-BIOS Arduino library (DCS-Skunkworks, or the FP-Fork 0.3.9 in Dropbox)
  PC    : DCS-BIOS serial link at 250000 baud (close the Arduino Serial Monitor first)

  Socket S1 map (ribbon pin -> Mega pin). S1 carries ribbon 11-20 on D22-D31 and ribbon 22-25 on
  A0-A3, each through 1 k on the carrier. Switch behavior is as found on the bench, 2026-11-14:
    ribbon  3  GND                 -> GND (carrier)
    ribbon  7  +5 V pot reference  -> 5V through the carrier's 100 mA fuse
    ribbon 12  FIRE DETECT TEST    -> D23   closed while pressed
    ribbon 13  SIGNAL LIGHTS TEST  -> D24   closed while pressed
    ribbon 15  NVIS                -> D26   closed with the lever at TOP  (PCB pad is labeled "NVIS OFF")
    ribbon 17  NVIS                -> D28   closed with the lever at OFF  (PCB pad is labeled "NVIS TOP")
    ribbon 19  HARS/SAS            -> D30   closed with the lever at NORM
  DCS values found on 2026-11-14: ALCP_NVIS_LTS 0 = OFF, 2 = TOP; ALCP_HARSSAS 1 = OVERRIDE.
    ribbon 23  WPN STA pot wiper   -> A1    reads 1023 at full CCW (ends swapped on the PCB)
    ribbon 25  REFUEL STATUS wiper -> A3    reads 0 at full CCW
  Both pots sweep the full 0..1023, so no endpoint trimming.
*/

#define SKETCH_VERSION 4

#define DCSBIOS_DEFAULT_SERIAL
#include "DcsBios.h"

// Leave at 0. The backlight section below is the old bench version (PWM on D9, Timer2). On the
// carrier D9 is a socket-4 input and the dim pin is D12 (Timer1), so it has to be ported before it
// can be used. Until then, dim with LED_dim_play. With 0 the sketch only reads the panel's controls.
#define ENABLE_BACKLIGHT 0
#if ENABLE_BACKLIGHT
#error "Backlight section not ported to the carrier yet: it drives D9, which is a socket-4 input there."
#endif

// ---- pins (Mega) ----
const uint8_t PIN_FDBA_TEST = 23;   // ribbon 12
const uint8_t PIN_LAMP_TEST = 24;   // ribbon 13
const uint8_t PIN_NVIS_TOP  = 26;   // ribbon 15: closed at TOP
const uint8_t PIN_NVIS_OFF  = 28;   // ribbon 17: closed at OFF
const uint8_t PIN_HARS_SAS  = 30;   // ribbon 19: closed at NORM
const uint8_t PIN_WPN_STA   = A1;   // ribbon 23
const uint8_t PIN_REFUEL    = A3;   // ribbon 25

// ---- pushbuttons: closed sends "1". Switch2Pos(msg, pin, reverse, debounce_ms) ----
// Debounce is 10 ms (library default 50) to cut the press-to-sim delay. Raise it if a single
// press ever registers twice.
const unsigned long BUTTON_DEBOUNCE_MS = 10;
DcsBios::Switch2Pos fireDetectTest("ALCP_FDBA_TEST", PIN_FDBA_TEST, false, BUTTON_DEBOUNCE_MS);
DcsBios::Switch2Pos lampTest      ("LAMP_TEST_BTN",  PIN_LAMP_TEST, false, BUTTON_DEBOUNCE_MS);

// ---- NVIS LTS 3-position: pinA closed sends "0", pinB closed sends "2", neither sends "1" ----
// Found in DCS on 2026-11-14: "0" is OFF and "2" is TOP, so the OFF pin goes first.
DcsBios::Switch3Pos nvisLts("ALCP_NVIS_LTS", PIN_NVIS_OFF, PIN_NVIS_TOP);

// ---- HARS/SAS OVERRIDE / NORM ----
// Found in DCS on 2026-11-14: "1" is OVERRIDE. The switch is closed at NORM, so reverse = true
// (closed sends "0", open sends "1").
DcsBios::Switch2Pos harsSas("ALCP_HARSSAS", PIN_HARS_SAS, true);

// ---- pots: PanelPot(msg, pin, reverse, input_min, input_max) ----
// The library default (DcsBios::Potentiometer) reads the pot on every loop pass and sends whenever
// the value moves by 128 of 65535, which can be hundreds of messages per second while turning.
// These three numbers tune that. All values are on the 0..65535 scale DCS-BIOS uses.
//   POT_POLL_MS    how often each pot is read. 20 ms = at most 50 messages per second per pot.
//   POT_HYSTERESIS minimum change before a new value is sent. 256 = 256 steps over full travel.
//   POT_EWMA_DIV   smoothing. Each read moves 1/N of the way to the new value. 2 = light (settles
//                  in about 60 ms at 20 ms polling). Higher is smoother but slower.
// If the sim knob still trails the real one: lower POT_POLL_MS to 10. If it jitters at rest: raise
// POT_HYSTERESIS to 384 or POT_EWMA_DIV to 3.
#define POT_POLL_MS    20
#define POT_HYSTERESIS 256
#define POT_EWMA_DIV   2
typedef DcsBios::PotentiometerEWMA<POT_POLL_MS, POT_HYSTERESIS, POT_EWMA_DIV> PanelPot;

PanelPot refuelStatusLts("ALCP_RSIL",   PIN_REFUEL,  false, 0, 1023);
PanelPot wpnStaLts      ("ALCP_WPNSTA", PIN_WPN_STA, true,  0, 1023);   // reversed

#if ENABLE_BACKLIGHT
// ---- console backlight: stand-in CONSOLE knob -> high-side PWM, local only (no DCS in the loop) ----
const uint8_t LED_PWM_PIN      = 9;     // Timer2, set to ~3.9 kHz in setup()
const uint8_t CONSOLE_KNOB_PIN = A8;    // spare B10K: ends to 5V and GND, wiper here, 100 nF to GND
const int     KNOB_OFF = 10;            // raw counts at or below this: LEDs off
const int     KNOB_BRT = 1013;          // raw counts at or above this: full brightness
const uint8_t MIN_DUTY = 6;             // floor so the lowest lit setting is visible

// The same knob reported to DCS so the virtual cockpit follows. It does not affect the LEDs.
PanelPot consoleKnob("LCP_CONSOLE", CONSOLE_KNOB_PIN, false, KNOB_OFF, KNOB_BRT);

uint8_t knobToDuty(int raw) {
  if (raw <= KNOB_OFF) return 0;
  if (raw >= KNOB_BRT) return 255;
  float x = (float)(raw - KNOB_OFF) / (float)(KNOB_BRT - KNOB_OFF);
  return MIN_DUTY + (uint8_t)(pow(x, 2.2f) * (255 - MIN_DUTY) + 0.5f);   // gamma 2.2
}

void updateBacklight() {
  static unsigned long lastMs = 0;
  static float filtered = -1.0f;
  static int applied = -1000;

  unsigned long now = millis();
  if (now - lastMs < 10) return;
  lastMs = now;

  int raw = analogRead(CONSOLE_KNOB_PIN);
  if (filtered < 0) filtered = raw;
  else filtered += (raw - filtered) * 0.2f;       // low-pass, ~50 ms

  int value = (int)(filtered + 0.5f);
  if (abs(value - applied) < 2) return;           // deadband against ADC noise
  applied = value;
  analogWrite(LED_PWM_PIN, knobToDuty(value));
}
#endif

// ---- startup sync: push the hardware state to DCS when the export stream starts ----
// The library only sends a control when it changes, so DCS never learns where the switches and
// pots already are. This watches the export stream (any data word counts) and, about 3 s after it
// starts, makes every control send its current state once. It runs again after any gap in the
// stream longer than 3 s (new mission, restart, bridge reconnected).
const unsigned long SYNC_DELAY_MS  = 3000;   // wait after the stream starts, so the cockpit is ready
const unsigned long STREAM_GAP_MS  = 3000;   // silence longer than this = stream stopped

class StreamWatch : public DcsBios::ExportStreamListener {
  public:
    volatile unsigned long lastDataMs;
    volatile bool gotData;
    StreamWatch() : DcsBios::ExportStreamListener(0x0000, 0xFFFE), lastDataMs(0), gotData(false) {}
    void onDcsBiosWrite(unsigned int address, unsigned int value) {
      (void)address; (void)value;
      lastDataMs = millis();
      gotData = true;
    }
};
StreamWatch streamWatch;

void updateStartupSync() {
  static bool streamActive = false;
  static bool syncPending  = false;
  static unsigned long streamStartMs = 0;

  unsigned long now = millis();
  if (streamWatch.gotData) {
    streamWatch.gotData = false;
    if (!streamActive) {
      streamActive  = true;
      syncPending   = true;
      streamStartMs = now;
    }
  } else if (streamActive && (now - streamWatch.lastDataMs > STREAM_GAP_MS)) {
    streamActive = false;
    syncPending  = false;
  }

  if (syncPending && (now - streamStartMs >= SYNC_DELAY_MS)) {
    syncPending = false;
    DcsBios::resetAllStates();    // every control re-sends its current state on the next poll
  }
}

void setup() {
#if ENABLE_BACKLIGHT
  TCCR2B = (TCCR2B & 0b11111000) | 0b010;         // Timer2 prescaler 8 -> ~3.9 kHz on D9
  pinMode(LED_PWM_PIN, OUTPUT);
  analogWrite(LED_PWM_PIN, 0);
#endif
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
  updateStartupSync();
#if ENABLE_BACKLIGHT
  updateBacklight();
#endif
}
