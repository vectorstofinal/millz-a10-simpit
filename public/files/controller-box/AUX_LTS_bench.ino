/*
  A-10C simpit — bench rig: AUX LTS panel on one universal 2x13 socket (left-console box prototype)
  Board : Arduino Mega 2560 (Elegoo, USB) + Mega protoshield
  Lib   : DCS-BIOS Arduino library (DCS-Skunkworks), v0.3.x
          (uses Potentiometer reverse/min/max and Dimmer(map function), both present in current master)

  Universal socket -> Mega mapping (same for every socket on this Mega):
    socket D1..D10 (ribbon pins 11..20)  -> Mega D22..D31  (INPUT_PULLUP, switch to GND)
    socket A1..A5  (ribbon pins 22..26)  -> Mega A0..A4
    socket +5V     (ribbon pin 7)        -> Mega 5V   (pot reference, ratiometric)
    socket GND     (3,4,5,6,8,21)        -> Mega GND  = ATX COM (star point)
    socket LED+    (1,2)                 <- TIP42C collector (high-side dimmer, PWM from Mega D9)

  AUX LTS board v2 pin use (see the AUX LTS board post):
    D2 FIRE DETECT TEST  D3 SIGNAL LT TEST  D5 NVIS OFF  D7 NVIS TOP  D9 HARS/SAS OVERRIDE
    A2 WPN STA pot (ends swapped on the board -> reversed here)   A4 REFUEL STATUS pot
*/

#define DCSBIOS_DEFAULT_SERIAL
#include "DcsBios.h"

// ---- socket map (socket channel -> Mega pin) ----
#define S_D(n)  (21 + (n))      // D1 -> 22 ... D10 -> 31
#define S_A1 A0
#define S_A2 A1
#define S_A3 A2
#define S_A4 A3
#define S_A5 A4
#define LED_PWM_PIN 9           // Timer2 (OC2B); reprogrammed to ~3.9 kHz below

// ---- pushbuttons (closed = "1") ----
DcsBios::Switch2Pos fireDetectTest("ALCP_FDBA_TEST", S_D(2));
DcsBios::Switch2Pos lampTest      ("LAMP_TEST_BTN",  S_D(3));

// ---- NVIS LTS 3-position: pinA low -> "0", pinB low -> "2", neither -> "1" (ALL) ----
// If DCS shows TOP and OFF swapped, exchange the two pins.
DcsBios::Switch3Pos nvisLts("ALCP_NVIS_LTS", S_D(7) /*TOP*/, S_D(5) /*OFF*/);

// ---- HARS/SAS OVERRIDE / NORM: closed sends "1". If DCS shows it inverted, set reverse = true ----
DcsBios::Switch2Pos harsSas("ALCP_HARSSAS", S_D(9), false);

// ---- pots (10-bit ADC -> 0..65535). Calibrate DIM/BRT endpoints with input_min / input_max ----
// Readings outside [min,max] are clamped, so the pot's extra travel past the panel's marks does nothing.
DcsBios::Potentiometer refuelStatusLts("ALCP_RSIL",   S_A4, false, 0, 1023);
DcsBios::Potentiometer wpnStaLts      ("ALCP_WPNSTA", S_A2, true,  0, 1023);   // reversed: ends swapped on the board

// ---- console backlight: DCS console-light brightness -> high-side PWM ----
// INT_CONSOLE_L_BRIGHT (Light System Control Panel), address 0x1370 = A_10C_INT_CONSOLE_L_BRIGHT_A
// Gamma ~2.2 plus a small floor so the lowest non-zero setting is still visibly lit.
const uint8_t MIN_DUTY = 6;
unsigned int consoleGamma(unsigned int v) {
  if (v == 0) return 0;
  float x = v / 65535.0f;
  float y = pow(x, 2.2f);
  return MIN_DUTY + (unsigned int)(y * (255 - MIN_DUTY) + 0.5f);
}
DcsBios::Dimmer consoleBacklight(0x1370, LED_PWM_PIN, consoleGamma);

void setup() {
  // Timer2 phase-correct PWM, prescaler 8 -> 16 MHz / 8 / 510 = ~3.9 kHz on D9/D10.
  // (Default is ~490 Hz.) Right for the TIP42C interim dimmer; the P-FET box will go to ~20-30 kHz.
  TCCR2B = (TCCR2B & 0b11111000) | 0b010;
  pinMode(LED_PWM_PIN, OUTPUT);
  analogWrite(LED_PWM_PIN, 0);     // LEDs off until DCS sends a brightness
  DcsBios::setup();
}

void loop() {
  DcsBios::loop();
}

/*
  Endpoint calibration (optional, after the panel is built):
    1. Temporarily print analogRead(S_A4) / analogRead(S_A2) over a test sketch (not while DCS-BIOS owns Serial).
    2. Note the raw value with the knob pointer exactly on DIM/OFF and on BRT.
    3. Put those as input_min / input_max above (for the reversed pot, still min < max in raw counts).
*/
