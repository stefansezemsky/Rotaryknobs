/*
 * RotaryknobsF1 - vier Drehgeber als USB-Joystick-Achsen (Simracing-Lenkrad)
 *
 * Variante fuer Spiele, die eigene Achsen erwarten (z. B. F1-Spiele fuer
 * Bremsbalance oder Differenzial), statt einzelner Tastendruecke.
 * Jeder Drehgeber wird stufenlos auf eine Joystick-Achse gemappt.
 *
 * Board:        Arduino mit nativem USB (z. B. Leonardo, Micro, Pro Micro),
 *               nur diese Boards koennen sich als Joystick melden
 * Bibliotheken: Encoder (Paul Stoffregen)
 *               Joystick (Matthew Heironimus, "Arduino Joystick Library")
 *
 * Belegung:
 *   Drehgeber 1 (Pins 2/3)  ->  X-Achse
 *   Drehgeber 2 (Pins 4/5)  ->  Y-Achse
 *   Drehgeber 3 (Pins 6/7)  ->  Z-Achse
 *   Drehgeber 4 (Pins 8/9)  ->  Rx-Achse
 */

#include <Encoder.h>
#include <Joystick.h>

// Ein mechanischer Drehgeber liefert meist 4 Zaehlschritte pro Raststufe.
const int COUNTS_PER_DETENT = 4;

// Wieviele Raststufen der Drehgeber in eine Richtung hat, bevor der Anschlag
// erreicht ist. Legt zusammen mit ANALOG_MAX den Wertebereich der Achse fest.
// TODO: an den tatsaechlichen Drehbereich deiner Regler anpassen.
const int DETENT_RANGE = 50;   // Regler bewegt sich zwischen -50 und +50
const int ANALOG_MAX = 1023;   // ueblicher Wertebereich fuer Joystick-Achsen

// Wie oft die Drehgeber geprueft werden (ms). Kleinere Werte reagieren
// schneller, sind aber unnoetig, wenn ohnehin nur bei Aenderung gesendet wird.
const int POLL_INTERVAL_MS = 10;

// Drehgeber (Pin A, Pin B)
Encoder encoder1(2, 3);
Encoder encoder2(4, 5);
Encoder encoder3(6, 7);
Encoder encoder4(8, 9);

// Zuletzt gesendete Raststufe je Drehgeber
long pos1 = 0;
long pos2 = 0;
long pos3 = 0;
long pos4 = 0;

// Joystick mit 4 Achsen (X, Y, Z, Rx), ohne Buttons und ohne weitere Achsen.
Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_MULTI_AXIS,
  0, 0,                 // keine Buttons, kein Hat-Switch
  true, true, true,     // X, Y, Z aktiv
  true, false, false,   // Rx aktiv, Ry/Rz nicht
  false, false, false, false, false // Rudder, Throttle, Accelerator, Brake, Steering nicht
);

// Liest einen Drehgeber und meldet den Wert nur bei Aenderung an die Achse.
// setAxis ist z. B. Joystick.setXAxis.
void updateAxis(Encoder &enc, long &lastPos, void (Joystick_::*setAxis)(int32_t)) {
  long newPos = enc.read() / COUNTS_PER_DETENT;
  if (newPos != lastPos) {
    int value = map(newPos, -DETENT_RANGE, DETENT_RANGE, 0, ANALOG_MAX);
    value = constrain(value, 0, ANALOG_MAX);
    (Joystick.*setAxis)(value);
    lastPos = newPos;
  }
}

void setup() {
  Joystick.begin();
}

void loop() {
  updateAxis(encoder1, pos1, &Joystick_::setXAxis);
  updateAxis(encoder2, pos2, &Joystick_::setYAxis);
  updateAxis(encoder3, pos3, &Joystick_::setZAxis);
  updateAxis(encoder4, pos4, &Joystick_::setRxAxis);

  delay(POLL_INTERVAL_MS);
}
