/*
  A-10C simpit - LED dimming bench play sketch (no DCS-BIOS)

  SKETCH VERSION: 1   (2026-11-15)
  Version rule: this number goes up by one every time this sketch is changed. Never reuse a number.
  History:
    v1  2026-11-15  First version. High-side backlight PWM on D12 (Timer1, 16-bit) with every
                    setting adjustable live from the Serial Monitor: PWM frequency, gamma, floor,
                    cap, knob end points, direct duty, raw pulse width, slow sweep, and a pot-noise
                    test that shows what the LED return current does to the panel's pot readings.

  Purpose: find the numbers for the dim-master firmware before the Mega carrier PCBs arrive.
  D12 is used (not D9 as in the older bench sketches) because D12 / Timer1 OC1B is the dim-master
  pin on the carrier, and D9 becomes a socket-4 input there.

  Board : Arduino Mega 2560. No libraries needed.
  PC    : Arduino Serial Monitor at 115200 baud, line ending "Newline".
          Turn the DCS-BIOS Bridge connection off first (it holds the COM port).
  NOTE  : uploading this replaces AUX_LTS_dcsbios on the Mega. Upload that sketch again afterwards.

  Wiring (interim high-side dimmer, kit parts):
    Mega D12 -> 100 ohm -> 2N7000 gate; 100 k from gate to GND; 2N7000 source to GND
    2N7000 drain -> 680 ohm -> TIP42C base; 10 k from base to emitter
    TIP42C emitter -> +12 V (bench supply, current limit about 0.6 A)
    TIP42C collector -> ribbon pins 1 and 2 (LED+)
    Bench supply negative -> Mega GND, at the same point as the ribbon GND jumpers
    Stand-in CONSOLE knob: B10K, ends to Mega 5V and GND, wiper to A8 (100 nF to GND if handy)
    Panel pots stay as wired for AUX_LTS_dcsbios: ribbon 23 -> A1 (WPN STA), ribbon 25 -> A2 (REFUEL)

  Commands (type in the Serial Monitor, then Enter):
    ?          help and current settings
    k          knob mode (default): the A8 knob sets brightness through the curve
    d <pct>    manual duty in percent, 0 to 100 (ignores knob and curve)   e.g.  d 0.5
    t <ticks>  manual on-time in timer ticks (finest step; for finding the low end)   e.g.  t 3
    s          slow sweep, off -> full -> off over 10 s, through the curve
    f <Hz>     PWM frequency, 30 to 40000   e.g.  f 20000
    g <gamma>  curve exponent, 0.5 to 4 (1 = linear)   e.g.  g 2.2
    lo <pct>   floor: duty at the lowest lit knob position   e.g.  lo 0.5
    hi <pct>   cap: duty at full knob   e.g.  hi 60
    ko <raw>   knob reading (0..1023) at or below which the LEDs are off
    kb <raw>   knob reading at or above which brightness is full
    n          pot-noise test (takes about 6 s; LEDs go off, full, then 50 %)
    q          status lines on / off
*/

#define SKETCH_VERSION 1

// ---- pins (Mega) ----
// LED PWM is D12 = PB6 = Timer1 OC1B. It is driven through the port and timer registers directly.
const uint8_t PIN_KNOB    = A8;   // stand-in CONSOLE knob
const uint8_t PIN_WPN_STA = A1;   // ribbon 23, panel pot (read only by the noise test)
const uint8_t PIN_REFUEL  = A2;   // ribbon 25, panel pot (read only by the noise test)

// Measured 2026-11-13: all 36 AUX LTS LEDs draw 0.432 A at 12 V. Used only for the estimate shown.
const float FULL_ON_AMPS = 0.432f;

// ---- settings (all adjustable from the Serial Monitor) ----
float pwmHz    = 1000.0f;   // start low: the interim TIP42C is slow. Try f 20000 to see why.
float gammaExp = 2.2f;
float floorPct = 0.5f;      // duty at the lowest lit knob position
float capPct   = 100.0f;    // duty at full knob
int   knobOff  = 10;
int   knobBrt  = 1013;

// ---- state ----
const uint8_t MODE_KNOB = 0, MODE_MANUAL = 1, MODE_SWEEP = 2;
uint8_t  mode = MODE_KNOB;
uint16_t pwmTop = 15999;        // timer counts 0..pwmTop
uint16_t pwmPrescale = 1;
uint32_t onTicks = 0;           // LED on-time per period, in timer ticks (0..pwmTop+1)
float    currentX = 0.0f;       // knob/sweep position 0..1 before the curve
float    knobFiltered = -1.0f;
int      knobApplied = 0;
bool     knobForce = true;       // re-apply the knob on the next reading
bool     statusOn = true;
bool     statusDirty = true;
unsigned long sweepStartMs = 0;

// ---------------------------------------------------------------- PWM (Timer1, mode 14) ----------

uint32_t pwmSteps() { return (uint32_t)pwmTop + 1; }

// Set the on-time in ticks. 0 = pin held low, pwmSteps() = pin held high (no PWM edges at all).
void pwmWriteTicks(uint32_t n) {
  uint32_t steps = pwmSteps();
  if (n > steps) n = steps;
  onTicks = n;
  if (n == 0) {
    TCCR1A &= ~_BV(COM1B1);       // disconnect the timer from the pin
    PORTB  &= ~_BV(PB6);          // D12 low: LEDs off
  } else if (n == steps) {
    TCCR1A &= ~_BV(COM1B1);
    PORTB  |= _BV(PB6);           // D12 high: LEDs full on
  } else {
    OCR1B = (uint16_t)(n - 1);    // high for n ticks of each period
    TCCR1A |= _BV(COM1B1);        // timer drives the pin
    PORTB  &= ~_BV(PB6);          // so a later disconnect starts from low
  }
  statusDirty = true;
}

uint32_t fractionToTicks(float frac) {
  if (frac <= 0.0f) return 0;
  uint32_t steps = pwmSteps();
  uint32_t n = (uint32_t)(frac * (float)steps + 0.5f);
  if (n < 1) n = 1;
  if (n > steps) n = steps;
  return n;
}

// Pick the smallest prescaler that fits the period in 16 bits (best resolution), keep the duty.
void pwmSetFrequency(float hz) {
  static const uint16_t presc[5] = {1, 8, 64, 256, 1024};
  if (hz < 30.0f) hz = 30.0f;
  if (hz > 40000.0f) hz = 40000.0f;

  float keepFraction = (float)onTicks / (float)pwmSteps();

  uint8_t i = 0;
  uint32_t ticks = 0;
  for (i = 0; i < 5; i++) {
    ticks = (uint32_t)((float)F_CPU / ((float)presc[i] * hz) + 0.5f);
    if (ticks <= 65536UL) break;
  }
  if (i > 4) i = 4;
  if (ticks > 65536UL) ticks = 65536UL;
  if (ticks < 2) ticks = 2;

  TCCR1B = 0;                         // stop the timer
  TCCR1A = _BV(WGM11);                // OC1A (D11), OC1B, OC1C (D13) all disconnected
  PORTB &= ~_BV(PB6);                 // D12 low while the timer is reconfigured
  pwmPrescale = presc[i];
  pwmTop = (uint16_t)(ticks - 1);
  ICR1 = pwmTop;
  TCNT1 = 0;
  TCCR1B = _BV(WGM13) | _BV(WGM12) | (uint8_t)(i + 1);   // fast PWM, TOP = ICR1, start
  pwmHz = (float)F_CPU / ((float)pwmPrescale * (float)ticks);

  pwmWriteTicks(fractionToTicks(keepFraction));
}

// ---------------------------------------------------------------- brightness curve ---------------

// x = knob or sweep position 0..1. Returns duty as a fraction 0..1. x = 0 is fully off.
float curve(float x) {
  if (x <= 0.0f) return 0.0f;
  if (x > 1.0f) x = 1.0f;
  float lo = floorPct / 100.0f;
  float hi = capPct / 100.0f;
  return lo + (hi - lo) * pow(x, gammaExp);
}

void applyX(float x) {
  currentX = x;
  pwmWriteTicks(fractionToTicks(curve(x)));
}

float knobToX(int raw) {
  if (raw <= knobOff) return 0.0f;
  if (raw >= knobBrt) return 1.0f;
  float x = (float)(raw - knobOff) / (float)(knobBrt - knobOff);
  return (x < 0.001f) ? 0.001f : x;
}

void updateKnob() {
  static unsigned long lastMs = 0;
  unsigned long now = millis();
  if (now - lastMs < 10) return;
  lastMs = now;

  int raw = analogRead(PIN_KNOB);
  if (knobFiltered < 0.0f) knobFiltered = raw;
  else knobFiltered += ((float)raw - knobFiltered) * 0.2f;   // low-pass, about 50 ms

  int value = (int)(knobFiltered + 0.5f);
  if (!knobForce && abs(value - knobApplied) < 2) return;    // deadband against ADC noise
  knobForce = false;
  knobApplied = value;
  if (mode == MODE_KNOB) applyX(knobToX(value));
}

void updateSweep() {
  static unsigned long lastMs = 0;
  unsigned long now = millis();
  if (mode != MODE_SWEEP || now - lastMs < 20) return;
  lastMs = now;
  unsigned long phase = (now - sweepStartMs) % 10000UL;
  float x = (phase < 5000UL) ? (float)phase / 5000.0f : (float)(10000UL - phase) / 5000.0f;
  applyX(x);
}

// ---------------------------------------------------------------- status and help ----------------

void printStatus() {
  uint32_t steps = pwmSteps();
  float duty = 100.0f * (float)onTicks / (float)steps;
  float usPerTick = (float)pwmPrescale * 1000000.0f / (float)F_CPU;

  if (mode == MODE_KNOB) Serial.print(F("KNOB   "));
  else if (mode == MODE_MANUAL) Serial.print(F("MANUAL "));
  else Serial.print(F("SWEEP  "));
  Serial.print(F("knob raw "));  Serial.print(knobApplied);
  if (mode != MODE_MANUAL) { Serial.print(F("  x "));  Serial.print(currentX * 100.0f, 1);  Serial.print(F("%")); }
  Serial.print(F("  duty "));    Serial.print(duty, 3);  Serial.print(F("%"));
  Serial.print(F("  on "));      Serial.print(onTicks);  Serial.print(F("/"));  Serial.print(steps);
  Serial.print(F(" ticks = "));  Serial.print((float)onTicks * usPerTick, 2);  Serial.print(F(" us"));
  Serial.print(F("  "));         Serial.print(pwmHz, 1);  Serial.print(F(" Hz"));
  Serial.print(F("  ~"));        Serial.print(FULL_ON_AMPS * 1000.0f * duty / 100.0f, 0);
  Serial.println(F(" mA avg (estimate, 36 LEDs)"));
}

void printSettings() {
  Serial.print(F("Settings: f "));  Serial.print(pwmHz, 1);
  Serial.print(F(" Hz ("));         Serial.print(pwmSteps());
  Serial.print(F(" steps, "));      Serial.print((float)pwmPrescale * 1000000.0f / (float)F_CPU, 4);
  Serial.print(F(" us each)  g "));  Serial.print(gammaExp, 2);
  Serial.print(F("  lo "));         Serial.print(floorPct, 3);
  Serial.print(F("%  hi "));        Serial.print(capPct, 1);
  Serial.print(F("%  ko "));        Serial.print(knobOff);
  Serial.print(F("  kb "));         Serial.println(knobBrt);
}

void printHelp() {
  Serial.println(F("Commands:  k = knob mode   d <pct> = manual duty   t <ticks> = manual on-time"));
  Serial.println(F("           s = slow sweep  f <Hz> = PWM frequency  g <gamma>  lo <pct>  hi <pct>"));
  Serial.println(F("           ko <raw> / kb <raw> = knob off / full points   n = pot-noise test"));
  Serial.println(F("           q = status lines on/off   ? = this help"));
  printSettings();
}

// ---------------------------------------------------------------- pot-noise test -----------------

// Reads the three analog pins as fast as possible for durationMs. Gives mean and peak-to-peak.
void sampleBlock(unsigned long durationMs, float *meanOut, int *ppOut) {
  const uint8_t pins[3] = {PIN_WPN_STA, PIN_REFUEL, PIN_KNOB};
  int lo[3] = {1023, 1023, 1023};
  int hi[3] = {0, 0, 0};
  unsigned long sum[3] = {0, 0, 0};
  unsigned long n = 0;
  unsigned long t0 = millis();
  while (millis() - t0 < durationMs) {
    for (uint8_t i = 0; i < 3; i++) {
      analogRead(pins[i]);                  // first read after a channel change is discarded
      int v = analogRead(pins[i]);
      if (v < lo[i]) lo[i] = v;
      if (v > hi[i]) hi[i] = v;
      sum[i] += (unsigned long)v;
    }
    n++;
  }
  for (uint8_t i = 0; i < 3; i++) {
    meanOut[i] = (n > 0) ? (float)sum[i] / (float)n : 0.0f;
    ppOut[i] = hi[i] - lo[i];
  }
}

void runNoiseTest() {
  float mOff[3], mOn[3], mPwm[3];
  int pOff[3], pOn[3], pPwm[3];
  uint32_t saved = onTicks;
  uint32_t steps = pwmSteps();

  Serial.println(F("Pot-noise test, about 6 s. Best with both panel pots and the A8 knob near mid-travel."));
  pwmWriteTicks(0);          delay(300);  sampleBlock(1500, mOff, pOff);
  pwmWriteTicks(steps);      delay(300);  sampleBlock(1500, mOn,  pOn);
  pwmWriteTicks(steps / 2);  delay(300);  sampleBlock(1500, mPwm, pPwm);
  pwmWriteTicks(saved);

  for (uint8_t i = 0; i < 3; i++) {
    if (i == 0) Serial.print(F("A1 WPN STA (ribbon): "));
    else if (i == 1) Serial.print(F("A2 REFUEL  (ribbon): "));
    else Serial.print(F("A8 knob    (local) : "));
    Serial.print(F("LEDs off mean "));   Serial.print(mOff[i], 1);
    Serial.print(F(" p-p "));            Serial.print(pOff[i]);
    Serial.print(F("  |  full on mean ")); Serial.print(mOn[i], 1);
    Serial.print(F(" p-p "));            Serial.print(pOn[i]);
    Serial.print(F(" shift "));          Serial.print(mOn[i] - mOff[i], 1);
    Serial.print(F("  |  50% PWM mean ")); Serial.print(mPwm[i], 1);
    Serial.print(F(" p-p "));            Serial.print(pPwm[i]);
    Serial.print(F(" shift "));          Serial.println(mPwm[i] - mOff[i], 1);
  }
  Serial.print(F("PWM was "));  Serial.print(pwmHz, 1);
  Serial.println(F(" Hz. 1 count = 4.9 mV. A8 does not go through the ribbon, so it is the reference."));
  Serial.println(F("A shift on A1/A2 but not A8 = LED return current lifting the panel ground."));
  knobForce = true;           // make knob mode re-apply on the next reading
}

// ---------------------------------------------------------------- serial commands ----------------

void handleLine(char *line) {
  char cmd[4] = {0, 0, 0, 0};
  char *p = line;
  while (*p == ' ') p++;
  uint8_t n = 0;
  while ((isalpha(*p) || *p == '?') && n < 3) cmd[n++] = (char)tolower(*p++);
  while (*p == ' ' || *p == '=') p++;
  bool hasArg = (*p != 0);
  float arg = hasArg ? atof(p) : 0.0f;

  if (!strcmp(cmd, "?") || !strcmp(cmd, "h")) {
    printHelp();
  } else if (!strcmp(cmd, "k")) {
    mode = MODE_KNOB;  knobForce = true;
  } else if (!strcmp(cmd, "s")) {
    mode = MODE_SWEEP;  sweepStartMs = millis();
  } else if (!strcmp(cmd, "d") && hasArg) {
    if (arg < 0.0f) arg = 0.0f;
    if (arg > 100.0f) arg = 100.0f;
    mode = MODE_MANUAL;  pwmWriteTicks(fractionToTicks(arg / 100.0f));
  } else if (!strcmp(cmd, "t") && hasArg) {
    if (arg < 0.0f) arg = 0.0f;
    mode = MODE_MANUAL;  pwmWriteTicks((uint32_t)(arg + 0.5f));
  } else if (!strcmp(cmd, "f") && hasArg) {
    pwmSetFrequency(arg);  knobForce = true;  printSettings();
  } else if (!strcmp(cmd, "g") && hasArg) {
    if (arg < 0.5f) arg = 0.5f;
    if (arg > 4.0f) arg = 4.0f;
    gammaExp = arg;  knobForce = true;  printSettings();
  } else if (!strcmp(cmd, "lo") && hasArg) {
    if (arg < 0.0f) arg = 0.0f;
    if (arg > capPct) arg = capPct;
    floorPct = arg;  knobForce = true;  printSettings();
  } else if (!strcmp(cmd, "hi") && hasArg) {
    if (arg > 100.0f) arg = 100.0f;
    if (arg < floorPct) arg = floorPct;
    capPct = arg;  knobForce = true;  printSettings();
  } else if (!strcmp(cmd, "ko") && hasArg) {
    knobOff = constrain((int)arg, 0, knobBrt - 1);  knobForce = true;  printSettings();
  } else if (!strcmp(cmd, "kb") && hasArg) {
    knobBrt = constrain((int)arg, knobOff + 1, 1023);  knobForce = true;  printSettings();
  } else if (!strcmp(cmd, "n")) {
    runNoiseTest();
  } else if (!strcmp(cmd, "q")) {
    statusOn = !statusOn;
    Serial.println(statusOn ? F("Status lines on") : F("Status lines off"));
  } else {
    Serial.println(F("Not understood. Type ? for help."));
  }
}

void updateSerial() {
  static char buf[32];
  static uint8_t len = 0;
  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == '\n' || c == '\r') {
      if (len > 0) { buf[len] = 0;  handleLine(buf);  len = 0; }
    } else if (len < sizeof(buf) - 1) {
      buf[len++] = c;
    }
  }
}

void updateStatus() {
  static unsigned long lastMs = 0;
  unsigned long now = millis();
  unsigned long interval = (mode == MODE_SWEEP) ? 500UL : 250UL;
  if (!statusOn || !statusDirty || now - lastMs < interval) return;
  lastMs = now;
  statusDirty = false;
  printStatus();
}

// ---------------------------------------------------------------- setup / loop -------------------

void setup() {
  PORTB &= ~_BV(PB6);           // D12 low before it becomes an output and before the timer starts
  DDRB  |= _BV(PB6);
  pwmSetFrequency(pwmHz);       // leaves the LEDs off (on-time 0)

  Serial.begin(115200);
  Serial.println();
  Serial.print(F("LED dim play, sketch version "));
  Serial.println(SKETCH_VERSION);
  printHelp();
}

void loop() {
  updateSerial();
  updateKnob();
  updateSweep();
  updateStatus();
}
