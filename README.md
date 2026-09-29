# Rotaryknobs – Drehregler-Nachrüstung für das alte Thrusmtaster F1 Wheel

Mein altes Thrustmaster F1 Wheel hatte keine Drehregler, deswegen habe ich vier Drehregler
nachgerüstet und mit einem Arduino ausgelesen. Je nach Spiel meldet sich der Arduino
entweder als Tastatur oder als Joystick mit eigenen Achsen. Beide Varianten sind in
diesem Projekt enthalten.



## Die zwei Varianten



Manche Spiele erwarten für Einstellungen wie Traktionskontrolle oder Bremsbalance
Tastendrücke, andere (z. B. viele F1-Spiele) erwarten eine eigene Achse, die sich
stufenlos bewegt. Deshalb gibt es zwei Sketche für dieselbe Hardware:

|Datei|Meldet sich als|Wann sinnvoll|
|-|-|-|
|Rotaryknobs.ino|Tastatur|Spiele/Menüs, die per Tastendruck gesteuert werden (z. B. Pfeiltasten, +/-)|
|RotaryknobsF1.ino|Joystick (4 Achsen)|Spiele, die eigene Achsen für Regler erwarten (z. B. F1-Spiele)|

Nur eine der beiden Dateien gleichzeitig auf den Arduino hochladen, sonst gibt es
einen Konflikt zwischen den beiden USB-Gerätetypen.

### Variante 1: `Rotaryknobs.ino` (Tastatur)

Jede Raststufe eines Drehgebers löst einen kurzen Tastendruck aus, je nach
Drehrichtung eine von zwei Tasten.

|Drehgeber|Pins|Rechts drehen|Links drehen|Beispiel im Spiel|
|-|-|-|-|-|
|1|2 / 3|Pfeil hoch|Pfeil runter|Menü hoch/runter|
|2|4 / 5|Pfeil rechts|Pfeil links|Menü rechts/links|
|3|6 / 7|=|-|Wert erhöhen/senken|
|4|8 / 9|e|q|TC +/-|

Bibliothek zusätzlich zu Encoder: **Keyboard** (in der Arduino IDE enthalten).

### Variante 2: RotaryknobsF1.ino (Joystick-Achsen)

Jeder Drehgeber wird stufenlos auf eine Joystick-Achse abgebildet (Wertebereich
0–1023). Das Spiel liest die Achse genauso ein wie bei einem Lenkrad oder Pedal.

|Drehgeber|Pins|Achse|
|-|-|-|
|1|2 / 3|X|
|2|4 / 5|Y|
|3|6 / 7|Z|
|4|8 / 9|Rx|

Bibliothek zusätzlich zu Encoder: **Joystick** von Matthew Heironimus
("Arduino Joystick Library", über den Bibliotheksverwalter installierbar).

Der Drehbereich pro Regler (Anschlag zu Anschlag) ist im Code auf ±50 Raststufen
eingestellt (`DETENT\\\_RANGE`). Je nach eingebautem Drehgeber muss dieser Wert
angepasst werden, sonst wird der volle Achsbereich nicht ausgenutzt.

DETENT\_RANGE nach dem ersten Test an den echten Drehbereich anpassen -->

## Hardware (für beide Varianten gleich)

* Arduino mit nativem USB (Pro Micro, https://www.amazon.de/dp/B0D69JLJ97?ref\_=ppx\_hzsearch\_conn\_dt\_b\_fed\_asin\_title\_1)
* 4 Drehgeber (RUNCCI-YUN, EC11 360 Grad Drehgeber, 5 Pin, https://www.amazon.de/dp/B09SG3HF9N?ref\_=ppx\_hzod\_image\_dt\_b\_fed\_asin\_title\_0\_0)
* Verkabelung: je Drehgeber zwei Signalleitungen an die Pins aus der Tabelle, gemeinsame Masse (GND).
Interne Pull-up-Widerstände werden von der Encoder-Bibliothek aktiviert, externe sind nicht nötig.

## Benutzung

1. Gewünschten Sketch in der Arduino IDE öffnen (Tastatur- oder Joystick-Variante).
2. Benötigte Bibliotheken über den Bibliotheksverwalter installieren (Encoder, plus Keyboard oder Joystick).
3. Board und Port wählen, hochladen.
4. Arduino per USB an den PC anschließen. Windows/das Spiel erkennt ihn automatisch als Tastatur bzw. Joystick.
5. Bei der Joystick-Variante im Spiel die Achsen den gewünschten Funktionen zuweisen, bei der Tastatur-Variante die Tasten.

## So funktioniert es

* Die Encoder-Bibliothek zählt die Impulse jedes Drehgebers mit.
* Ein mechanischer Drehgeber liefert etwa 4 Zählschritte pro Raststufe, deshalb wird die Zählung durch 4 geteilt.
* **Tastatur-Variante:** Ändert sich die Raststufe, wird je nach Drehrichtung eine von zwei Tasten kurz gedrückt. Wird schnell gedreht, werden alle übersprungenen Raststufen nacheinander gesendet, es geht nichts verloren.
* **Joystick-Variante:** Die aktuelle Raststufe wird mit `map()` direkt auf einen Achswert zwischen 0 und 1023 umgerechnet und an den PC gemeldet, sobald sie sich ändert.

## Ideen für später

* Tastendruck bzw. Achsen-Update ohne `delay()` umsetzen, damit nichts verpasst werden kann
* Eine Taste je Drehgeber (Druckfunktion) ergänzen
* Beide Varianten über einen Schalter am Lenkrad umschaltbar machen, ohne neu flashen zu müssen

## Autor

Stefan Sezemsky

