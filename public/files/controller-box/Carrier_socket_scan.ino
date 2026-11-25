/*
  A-10C simpit - Mega carrier socket scanner (no DCS-BIOS, no libraries)

  SKETCH VERSION: 2   (2026-11-25)
  Version rule: this number goes up by one every time this sketch is changed. Never reuse a number.
  History:
    v1  2026-11-25  First version. Watches every Mega pin that goes to a carrier socket and prints
                    which pin changed whenever a panel control is operated. Written to find out
                    which Mega pin each ribbon pin really lands on, after AUX_LTS_dcsbios v4 saw
                    no switch changes on the carrier.
    v2  2026-11-25  Unconnected analog inputs no longer print. They used to drift and show up as
                    changes (A2, A4...). Each analog input is now parked with its pull-up on, which
                    holds an unconnected one steady near 1023, and is only reported once something
                    pulls it away from there (a knob). The start-up list shows only the analog
                    inputs with something connected. Send r in the Serial Monitor to re-check
                    after plugging or unplugging a panel.

  Use   : upload, open the Serial Monitor at 115200 baud, then operate ONE control at a time.
          Turn the DCS-BIOS Bridge connection off first (it holds the COM port).
  Prints: for a switch or button, the Mega pin that went LOW (closed to ground) or HIGH (open).
          for a knob, the analog pin and its reading (0..1023) each time it moves 16 counts.
          After each pin comes where the carrier v1.2 DESIGN says that pin goes (socket, ribbon
          pin). If the control you touched is on a different ribbon pin, the design map is wrong.
  NOTE  : uploading this replaces whatever sketch is on the Mega. The backlight stays off (D12 low).
          A knob that is parked at its very top end looks unconnected until it is first moved.

  Pins watched: D2-D11, D14-D17, D22-D53 with the internal pull-up; A0-A15 as analog.
  Not touched : D0/D1 (USB serial), D13 (on-board LED), D18-D21 (free pins).
*/

#define SKETCH_VERSION 2

const uint8_t PIN_DIM = 12;              // dim line (through JP5): held low so the LEDs stay off
const unsigned long DEBOUNCE_MS = 15;
const unsigned long ANALOG_POLL_MS = 50;
const unsigned long VERIFY_MS = 500;     // one connected analog input is re-checked this often
const int ANALOG_STEP = 16;              // print a knob when it has moved this many counts
const int PARKED_MIN = 950;              // pull-up on: an unconnected input reads above this
const int WAKE_BELOW = 900;              // pull-up on: below this, something is driving the input

uint8_t stableState[54];
uint8_t candState[54];
unsigned long candSince[54];
int analogShown[16];
bool analogActive[16];                   // true = something drives it: pull-up off, changes printed

bool watched(uint8_t p) {
  return (p >= 2 && p <= 11) || (p >= 14 && p <= 17) || (p >= 22 && p <= 53);
}

// Where the carrier v1.2 design says a digital pin goes.
void printDigitalDesign(uint8_t p) {
  uint8_t socket = 0, ribbon = 0;
  if (p >= 22 && p <= 31)      { socket = 1; ribbon = 11 + (p - 22); }
  else if (p >= 32 && p <= 41) { socket = 2; ribbon = 11 + (p - 32); }
  else if (p >= 42 && p <= 51) { socket = 3; ribbon = 11 + (p - 42); }
  else if (p >= 2 && p <= 11)  { socket = 4; ribbon = 11 + (p - 2); }
  else if (p >= 14 && p <= 17) { socket = p - 13; ribbon = 26; }
  if (socket == 0) { Serial.print(F("   (design: not on a socket)")); return; }
  Serial.print(F("   (design: socket S"));  Serial.print(socket);
  Serial.print(F(", ribbon pin "));         Serial.print(ribbon);
  Serial.print(F(")"));
}

void printAnalogDesign(uint8_t a) {
  Serial.print(F("   (design: socket S"));  Serial.print(a / 4 + 1);
  Serial.print(F(", ribbon pin "));         Serial.print(22 + a % 4);
  Serial.print(F(")"));
}

int readAnalogPin(uint8_t a) {
  analogRead(A0 + a);                    // first read after a channel change is discarded
  return analogRead(A0 + a);
}

void printAnalogLine(uint8_t a, int v) {
  Serial.print(F("A"));  Serial.print(a);  Serial.print(F(" = "));  Serial.print(v);
  printAnalogDesign(a);
}

// Park every analog input with its pull-up on, then release the ones that something is driving.
void checkAnalogInputs() {
  for (uint8_t a = 0; a < 16; a++) { pinMode(A0 + a, INPUT_PULLUP);  analogActive[a] = false; }
  delay(30);                             // an unconnected input charges up to about 1023
  for (uint8_t a = 0; a < 16; a++) {
    if (readAnalogPin(a) < PARKED_MIN) { pinMode(A0 + a, INPUT);  analogActive[a] = true; }
  }
  delay(10);
  Serial.println(F("Analog inputs with something connected (0..1023):"));
  bool any = false;
  for (uint8_t a = 0; a < 16; a++) {
    if (!analogActive[a]) continue;
    any = true;
    analogShown[a] = readAnalogPin(a);
    Serial.print(F("  "));  printAnalogLine(a, analogShown[a]);  Serial.println();
  }
  if (!any) Serial.println(F("  none"));
}

// Re-check one connected analog input: if the pull-up can drag it to the top, nothing drives it.
void verifyOneAnalog() {
  static uint8_t next = 0;
  for (uint8_t i = 0; i < 16; i++) {
    uint8_t a = next;
    next = (next + 1) % 16;
    if (!analogActive[a]) continue;
    if (readAnalogPin(a) >= 850) return;         // too near the top to tell; leave it alone
    pinMode(A0 + a, INPUT_PULLUP);
    delay(20);
    int up = readAnalogPin(a);
    if (up >= 980) {
      analogActive[a] = false;                   // stays parked with the pull-up on
      Serial.print(F("A"));  Serial.print(a);  Serial.println(F(" is no longer connected"));
    } else {
      pinMode(A0 + a, INPUT);
      delay(4);
    }
    return;
  }
}

void setup() {
  pinMode(PIN_DIM, OUTPUT);
  digitalWrite(PIN_DIM, LOW);

  Serial.begin(115200);
  Serial.println();
  Serial.print(F("Carrier socket scanner, sketch version "));
  Serial.println(SKETCH_VERSION);

  for (uint8_t p = 0; p < 54; p++) {
    if (!watched(p)) continue;
    pinMode(p, INPUT_PULLUP);
  }
  delay(50);                             // let the pull-ups settle through the 1 k resistors

  Serial.println(F("Digital pins reading LOW (closed to ground) right now:"));
  bool any = false;
  for (uint8_t p = 0; p < 54; p++) {
    if (!watched(p)) continue;
    stableState[p] = candState[p] = (uint8_t)digitalRead(p);
    candSince[p] = 0;
    if (stableState[p] == LOW) {
      any = true;
      Serial.print(F("  D"));  Serial.print(p);  printDigitalDesign(p);  Serial.println();
    }
  }
  if (!any) Serial.println(F("  none"));

  checkAnalogInputs();
  Serial.println(F("Now operate one control at a time. Send r to re-check the analog inputs."));
}

void loop() {
  unsigned long now = millis();

  for (uint8_t p = 0; p < 54; p++) {
    if (!watched(p)) continue;
    uint8_t v = (uint8_t)digitalRead(p);
    if (v == stableState[p]) { candState[p] = v; continue; }
    if (v != candState[p]) { candState[p] = v; candSince[p] = now; continue; }
    if (now - candSince[p] < DEBOUNCE_MS) continue;
    stableState[p] = v;
    Serial.print(F("D"));  Serial.print(p);
    Serial.print(v == LOW ? F(" LOW ") : F(" HIGH"));
    printDigitalDesign(p);
    Serial.println();
  }

  static unsigned long lastAnalogMs = 0;
  if (now - lastAnalogMs >= ANALOG_POLL_MS) {
    lastAnalogMs = now;
    for (uint8_t a = 0; a < 16; a++) {
      int v = readAnalogPin(a);
      if (analogActive[a]) {
        if (abs(v - analogShown[a]) < ANALOG_STEP) continue;
        analogShown[a] = v;
        printAnalogLine(a, v);
        Serial.println();
      } else if (v < WAKE_BELOW) {               // something started driving a parked input
        pinMode(A0 + a, INPUT);
        analogActive[a] = true;
        delay(5);
        analogShown[a] = readAnalogPin(a);
        printAnalogLine(a, analogShown[a]);
        Serial.println(F("   now connected"));
      }
    }
  }

  static unsigned long lastVerifyMs = 0;
  if (now - lastVerifyMs >= VERIFY_MS) {
    lastVerifyMs = now;
    verifyOneAnalog();
  }

  while (Serial.available() > 0) {
    char c = (char)Serial.read();
    if (c == 'r' || c == 'R') checkAnalogInputs();
  }
}
