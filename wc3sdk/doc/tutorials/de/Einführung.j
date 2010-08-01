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
	constant boolean A_DEBUG_NATIVES = false
	constant boolean A_NET = false
	constant boolean A_FPS_MOVEMENT = false
	constant real A_MAX_COLLISION_SIZE = 300.0
	constant integer A_MAX_COLLISION_ITERATIONS = 10
endglobals

//! import "Import Asl.j"

Dieser Code sollte am besten an den Anfang des eigenen Kartenskripts geschrieben werden, wenn man die ASL verwenden möchte.
Zunächst einmal werden die von der ASL verwendeten Optionen definiert. A_RTC legt fest, ob native Funktionen des RtC-Projekts verwendet werden
sollen. Dabei sollte man sicher gehen, ob man diese auch wirklich verwenden kann, da sie beim Wert true in das Kartenskript importiert und von der
ASL verwendet werden. A_JAPI legt das Gleiche, nur eben für das JAPI-Projekt fest. Enthält A_SYSTEMS den Wert true, so können auch die Untersysteme
der ASL verwendet werden, was allerdings auch eine Menge weiteren Codes bedeutet. A_OBJECTS gibt an, ob man Funktionen bezüglich der vom Projekt
wc3sdk, welches praktisch das Überprojekt der ASL ist, mitgelieferten MPQ-Archive bzw. Dateien verwenden möchte. Momentan sind das ausschließlich
Preload-Funktionen für die in den MPQ-Archiven enthaltenen Dateien.
A_DEBUG_HANDLES kann nur den Wert true enthalten, falls auch die vJass-Konstante DEBUG_MODE diesen enthält. Hat die Option den Wert true, so versucht die Struktur
ABenchmark sämtliche Handle-Erzeugungen und -Löschungen mitzuloggen. A_DEBUG_NATIVES bewirkt, dass die ASL versucht, mögliche Fehler in nativen Jass-Funktionen zu ermitteln.
A_NET kann nur true sein, falls A_RTC dies auch ist, da für die Netzwerkbibliotheken die nativen Funktionen des Projekts RtC verwendet werden.
Momentan existiert nur die unfertige Struktur AHost, welche später einmal Teil eines Netzwerksystems für mehr als 12 Spieler sein soll.
A_FPS_MOVEMENT gibt an, ob die Struktur AMovement des Charaktersystems das FPS-System des Projekts RtC verwenden soll und kann deshalb ebenfalls nur den Wert true enthalten, falls A_RTC dies auch tut.
Mit der Import-Anweisung wird die Hauptdatei der ASL in die Karte importiert.
Alle weiteren benötigten ASL-Dateien werden von ihr ausgehend in einer Art Baumstruktur automatisch importiert.
A_MAX_COLLISION_SIZE und A_MAX_COLLISION_ITERATIONS werden von Einheitenkollisionsgrößenermittlungsfunktionen verwendet und geben die maximale Kollisionsgröße einer
Einheit in der Karte und die Anzahl der Durchläufe der Auswertungsfunktionen an.
Eine Erhöhung der Durchläufe liefert ein genaueres Ergebnis, verlangsamt jedoch gleichzeitig die Funktionen.

Die Konstante DEBUG_MODE wird von vJass automatisch im Debug-Modus auf true gesetzt, weshalb man hierbei nichts angeben muss. Speichert man also
die Karte im Debug-Modus, so werden automatisch alle Debug-Dateien importiert. Anderenfalls nicht. Man sollte als nie das debug-Schlüsselwort
vor Anweisungen bezüglich der Debug-Funktionen und -Strukturen der ASL vergessen, da dies ansonsten im Normalmodus zu erheblich vielen Syntaxfehlern
führen kann.

3. Der Kern
Die ASL ist in ihren Kern und ihre Systeme aufgeteilt. Der Kern beinhaltet vor allem allgemeine Datentypen und Funktionen für die Warcraft-3-Standard-Datentypen.
Viele der enthaltenen Funktionen wurden nicht speziell von ASL-Entwicklern geschrieben, sondern  nahezu eins zu eins kopiert, da die ASL erst entstand als
es bereits viele Jass-Funktionen gab. Die Namen der Autoren sind in der Regel im Funktionskommentar aufgelistet. Die Datentypen, sprich Strukturen und Schnittstellen, stellen eine häufig in Projekten benötigte und verwendete Funktionalität zur Verfügung.
So existiert zum Beispiel die Struktur ADamageRecorder, die den erlittenen Schaden einer Einheit aufzeichnen.
Des Weiteren enthält das Kern-Modul "General" einige Behälterstrukturen, welche in Form eines Textmacros
mit dem entsprechenden Datentyp erzeugt werden können. Einige dieser Behälterstrukturen bieten Funktionen für sogenannte Iteratoren an, welche aus der Programmiersprache C++
abgeschaut wurden und einen Verweis auf enthaltene Elemente erlauben. Diese Iteratoren ermöglich ebenfalls die Verwendung eines "foreach"-Textmakros, das einfache "foreach"-Schleifen,
wie man sie aus Sprachen wie PHP oder Java kennt, nachbildet.
Für Testzwecke existiert das Modul "Debug", welches neben unzähligen Ausgabefunktionen Strukturen wie ABenchmark enthält, mittels welcher Zeitmessungen durchgeführt und deren Resultat ausgegeben werden können.
Momentan ist das Modul "Ai" leider noch leer, da die ASL keine Unterstützung in der KI-Programmierung anbietet. Der Autor empfiehlt daher, auf Projekte wie "AMAI" zurückzugreifen, wenn der Leser etwas
in diese Richtung entwickeln möchte.
Die ASL verwendet außer den übernommenen Grundfunktionen jedoch kaum gängige Systeme, wie sie z. B. auf wc3c.net zu finden sind.
Dies hat oftmals den Grund, dass die Systeme auf wc3c.net oder anderen Seiten nicht gut oder passend genug für die Verwendung in der ASL entworfen wurden.
Oftmals bietet die Entwicklung eigener Systeme im Nachhinein wesentlich mehr Möglichkeiten, komplexere Aufgaben zu bewältigen.

4. Die Systeme

5. Die Objekte (das wc3sdk-Projekt)
Die ASL ist Teil des wc3sdk-Projekts, welches nicht nur deren Code zur Verfügung stellt, sondern nebenbei auch noch eine ganze Reihe von Modellen und Texturen, die
für die Modifikation von Warcraft 3 The Frozen Throne geeignet sind.
Die meisten Modelle und Texturen stammen von Warcraft-Modifikationseiten des Internets und wurden bevor sie zu einem der Projektarchive hinzugefügt wurden, zumindest vorher getestet.
Da das wc3sdk-Projekt einst hauptsächlich für die sich in der Entwicklung befindlichen Modifikation "Die Macht des Feuers" gedacht war, sind viele Texturen und Modelle noch deutsch benannt.
Dies wird sich mit der Zeit jedoch ändern, da das wc3sdk-Projekt international zugänglich gemacht werden soll.
Auch befinden sich einige Dateien wie z. B. die Texturen von einigen Umgebungsmodellen noch nicht im korrekten Verzeichnis. Die Verzeichnisstruktur wurde erst während der Entwicklung entworfen
und da die Verschiebung einer Textur, eine notwendige Änderung des Pfades in der zugehörigen Modelldatei mit sich zieht, wird dieser aufwändige Prozess ebenfalls erst mit der Zeit durchgeführt
werden.
Kreaturen und Gegenstände wurden, wenn auch inzwischen auf Englisch, hauptsächlich nach ihrer Bedeutung in der Welt von Warcraft bzw. in "Die Macht des Feuers" benannt.
Natürlich können die Objekte im Spiel dennoch anders benannt werden.
Es ist geplant, SLK-Dateien für die verschiedenen Dateien zu schreiben, welche einen einfacheren Zugriff
vom Editor aus darauf erlauben.
