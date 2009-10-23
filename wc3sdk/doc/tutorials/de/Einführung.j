1. Vorwort

Seit dem Erscheinen und der Weiterentwicklung von vJass hat sich so einiges in der Warcraft-3-Modding-Welt getan.
Auf Wc3C.net gibt es inzwischen eine Menge Systeme und Zauber, welche in vJass geschrieben sind. vJass setzt sich durch,
da man damit einfach wesentlich angenehmer Code schreiben kann. Vor allem bei größeren Projekten zeigt sich das, da man mit
reinem Jass oft nur ein unübersichtliches Gewurschtel zusammenbekommt und auch die nötigen Syntaxfreiheiten fehlen.
Mit der Advanced Script Library (kurz ASL) habe ich versucht einige gute Funktionen und Systeme zu einer Art großen Bibliothek
zusammenzufassen, deren Verwendung dem Benutzer eine Menge Arbeit abnimmt.
Sie bietet neben ihren nützlichen Kernfunktionen, -strukturen und -makros auch einige erweiterte Untersysteme für die Welt-
und Charaktergestaltung. Auch ein Bonus-Mod- und ein GUI-System sind mit dabei. Hierbei gilt es noch anzumerken, dass die ASL
unter anderem für mein Rollenspielprojekt "Die Macht des Feuers" entstanden ist, was den Schwerpunkt der RPG-Unterstützung (Charakter-
und Weltsysteme) erklärt.

Die Defizite liegen vor allem bei der Warcraft-KI-Unterstützung, da hier noch keinerlei Funktionen existieren. Auch für andere Kartentypen,
wie z. B. AOS- oder Heroline-Karten, gibt es keine direkte Unterstützung, wobei man sich bei jeglicher Art von Ein-Held-pro-Spieler-Karte,
das Charaktersystem zu Nutzen machen kann.

Sowohl die Warcraft-3-Modifikation RtC als auch JAPI werden unterstützt bzw. deren native Funktionen verwendet, falls vom Benutzer angegeben.
Außerdem existieren erste Funktionen für eine Netzwerkspielunterstützung mittels der vom Projekt RtC bereitgestellten Netzwerkfunktionen.

2. Einbindung

Lange Rede, kurzer Sinn, was also muss man tun, um die ASL verwenden zu können?
Zunächst einmal muss man ein paar globale Konstanten definieren. Seitdem vJass um statische If-Anweisungen erweitert wurde, habe ich mich
mit dem Nichtimportieren von nicht benötigtem Code wesentlich leichter getan:
globals
	constant boolean A_RTC = false
	constant boolean A_JAPI = false
	constant boolean A_SYSTEMS = true
	constant boolean A_OBJECTS = false
	constant boolean A_DEBUG_HANDLES = false
	constant boolean A_NET = false
	constant boolean A_FPS_MOVEMENT = false
endglobals

//! import "Import Asl.j"

Dieser Code sollte am besten an den Anfang des eigenen Kartenskripts geschrieben werden, wenn man die ASL verwenden möchte.
Zunächst einmal werden die von der ASL verwendeten Optionen definiert. A_RTC legt fest, ob native Funktionen des RtC-Projekts verwendet werden
sollen. Dabei sollte man sicher gehen, ob man diese auch wirklich verwenden kann, da sie beim Wert true in das Kartenskript importiert und von der
ASL verwendet werden. A_JAPI legt das Gleiche, nur eben für das JAPI-Projekt fest. Enthält A_SYSTEMS den Wert true, so können auch die Untersysteme
der ASL verwendet werden, was allerdings auch eine Menge weiteren Codes bedeutet. A_OBJECTS gibt an, ob man Funktionen bezüglich der vom Projekt
wc3sdk, welches praktisch das Überprojekt der ASL ist, mitgelieferten MPQ-Archive bzw. Dateien verwenden möchte. Momentan sind das ausschließlich
Preload-Funktionen für die in den MPQ-Archiven enthaltenen Dateien.
A_DEBUG_HANDLES kann nur den Wert true enthalten, falls auch A_DEBUG diesen enthält. Hat die Option den Wert true, so versucht die Struktur
ABenchmark sämtliche Handle-Erzeugungen und -Löschungen mitzuloggen.
A_NET kann nur true sein, falls A_RTC dies auch ist, da für die Netzwerkbibliotheken die nativen Funktionen des Projekts RtC verwendet werden.
Momentan existiert nur die unfertige Struktur AHost, welche später einmal Teil eines Netzwerksystems für mehr als 12 Spieler sein soll.
A_FPS_MOVEMENT gibt an, ob die Struktur AMovement des Charaktersystems das FPS-System des Projekts RtC verwenden soll und kann deshalb ebenfalls nur den Wert true enthalten, falls A_RTC dies auch tut.
Mit der Import-Anweisung wird die Hauptdatei der ASL in die Karte importiert.
Alle weiteren benötigten ASL-Dateien werden von ihr ausgehend in einer Art Baumstruktur automatisch importiert.

Die Konstante DEBUG_MODE wird von vJass automatisch im Debug-Modus auf true gesetzt, weshalb man hierbei nichts angeben muss. Speichert man also
die Karte im Debug-Modus, so werden automatisch alle Debug-Dateien importiert. Anderenfalls nicht. Man sollte als nie das debug-Schlüsselwort
vor Anweisungen bezüglich der Debug-Funktionen und -Strukturen der ASL vergessen, da dies ansonsten im Normalmodus zu erheblich vielen Syntaxfehlern
führen kann.

3. Der Kern
Die ASL ist in ihren Kern und ihre Systeme aufgeteilt. Der Kern beinhaltet vor allem allgemeine Datentypen und Funktionen für die Warcraft-3-Standard-
Datentypen.

4. Die Systeme

5. Die Objekte (das wc3sdk-Projekt)
