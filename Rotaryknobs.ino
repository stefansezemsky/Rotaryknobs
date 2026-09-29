/*
 * Rotaryknobs - vier Drehgeber als USB-Tastatur (Simracing-Lenkrad)
 *
 * Jeder Drehgeber sendet je Raststufe einen Tastendruck, je nach Drehrichtung
 * eine von zwei Tasten. Der Arduino meldet sich am PC als normale Tastatur.
 *
 * Board:        Arduino mit nativem USB (z. B. Leonardo, Micro, Pro Micro),
 *               nur diese Boards koennen sich als Tastatur melden
 * Bibliotheken: Encoder (Paul Stoffregen), Keyboard (in der Arduino IDE enthalten)
 *
 * Belegung:
 *   Drehgeber 1 (Pins 2/3)  ->  Pfeil hoch / Pfeil runter
 *   Drehgeber 2 (Pins 4/5)  ->  Pfeil rechts / Pfeil links
 *   Drehgeber 3 (Pins 6/7)  ->  '=' / '-'   (z. B. +/- im Spiel)
 *   Drehgeber 4 (Pins 8/9)  ->  'e' / 'q'   (z. B. TC +/-)
 */

#include <Encoder.h>
#include <Keyboard.h>

// Ein mechanischer Drehgeber liefert meist 4 Zaehlschritte pro Raststufe.
const int COUNTS_PER_DETENT = 4;

// Wie lange die Taste gedrueckt bleibt (ms). Spiele erkennen sehr kurze
// Tastendruecke manchmal nicht.
const int KEY_HOLD_MS = 50;

// Drehgeber (Pin A, Pin B)
Encoder enc1(2, 3);
Encoder enc2(4, 5);
Encoder enc3(6, 7);
Encoder enc4(8, 9);

// Zuletzt verarbeitete Raststufe je Drehgeber
long pos1 = 0;
long pos2 = 0;
long pos3 = 0;
long pos4 = 0;

// Sendet einen kurzen Tastendruck an den PC.
void sendKey(uint8_t key) {
  Keyboard.press(key);
  delay(KEY_HOLD_MS);
  Keyboard.release(key);
}

// Liest einen Drehgeber und sendet fuer jede neue Raststufe eine Taste:
// keyPlus bei Drehung in Zaehlrichtung +, keyMinus bei Gegenrichtung.
void handleEncoder(Encoder &enc, long &lastPos, uint8_t keyPlus, uint8_t keyMinus) {
  long newPos = enc.read() / COUNTS_PER_DETENT;

  // Schleifen statt if: wird schnell gedreht, gehen keine Raststufen verloren.
  while (lastPos < newPos) {
    sendKey(keyPlus);
    lastPos++;
  }
  while (lastPos > newPos) {
    sendKey(keyMinus);
    lastPos--;
  }
}

void setup() {
  Keyboard.begin();
}

void loop() {
  handleEncoder(enc1, pos1, KEY_UP_ARROW, KEY_DOWN_ARROW);
  handleEncoder(enc2, pos2, KEY_RIGHT_ARROW, KEY_LEFT_ARROW);
  handleEncoder(enc3, pos3, '=', '-');
  handleEncoder(enc4, pos4, 'e', 'q');   // Kleinbuchstaben: kein Shift wird mitgesendet
}
