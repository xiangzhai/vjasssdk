Gespräche
Um ein Rollenspiel lebendiger zu gestalten, setzen Entwickler oft auf sogenannte Dialoge. Ein sehr gutes
Beispiel dafür ist die Gothic-Reihe, in welcher man als Spieler stets einen gewissen Grad an Freiheit
genießt, wählen zu können, was man zu anderen Nichtspielercharakteren sagt. Natürlich sind wir damit noch
Lichtjahre von einer künstlichen Intelligenz entfernt, aber es gibt den computergesteurten Charakteren einen
ganz eigenen Charakter und eine eigene Geschichte.
Diese Möglichkeit wollte ich ebenfalls in Die Macht des Feuers und so implementierte ich in der ASL eine Art
Gesprächssystem, welches sich sehr an dem von Gothic 2 orientiert.
Insgesamt braucht man lediglich zwei oder besser gesagt sogar nur eine Struktur zu kennen, um es benutzen zu
können. ATalk und AInfo bilden zusammen das gesamte System und interagieren natürlich auch mit der Struktur
ACharacter, da sie selbst ebenfalls Bestandteil des Charaktersystems sind.
Zusätzlich existiert inzwischen auch noch AInfoLog, welches eine Aufzeichnungsmöglichkeit für die geführten
Gespräche anbietet. So kann man mit Hilfe dieser Struktur grafische Oberflächen wie in Gothic 3 erstellen,
in welchen sich der Spieler nochmal die Gespräche durchlesen kann.
ATalk stellt ein Gespräch mit genau einer Einheit bzw. einem Nichtspielercharakter dar. Ich habe mich bis
jetzt darauf beschränkt, dass sich immer nur ein Charakter mit einem Nichtspielercharakter unterhalten
kann. Natürlich wäre es auch möglich das System so zu erweitern, sodass er mit mehreren Charakteren
gleichzeitig sprechen kann, allerdings finde ich, dass dies während des Spiels ein wenig unrealistisch wirkt,
zumal das System mit Einheitenausrichtungen und -animationen arbeitet.
Andererseits muss ich mir auch eingestehen, dass es etwas nervig seien könnte, wenn die Anzahl der Spieler
eher groß und die Anzahl der Nichtspielercharaktere eher klein gehalten werden würde.
Wie dem auch sei, beginnen wir einfach direkt mit der Initialisierung der beiden Strukturen:

library MyTalkLibrary requires Asl

	struct TalkPeter
		private static method onInit takes nothing returns nothing
			call ATalk.init("smart", 300.0, "Abilities\\Spells\\Other\\TalkToMe\\TalkToMe.mdl", tr("Ziel unterhält sich bereits."), tr("Ende"), tr("Zurück"))
			call AInfo.init(gg_cam_talk, KEY_ESCAPE, 1.0, "speech", "listen")
		endmethod
	endstruct

endlibrary

Das erste Argument des Aufrufs von ATalk.init legt den Befehl fest, welcher mit einem Nichtspielercharakter
als Ziel ausgeführt werden muss, damit das Gespräch beginnt. In diesem Fall handelt es sich um einen
einfachen Rechtsklick, mit welchem man den Charakter auf den Nichtspielercharakter zu schickt. Das zweite
Argument gibt die Mindestreichweite für den festgelegten Befehl an, damit der Charakter sich nicht mit dem
Ziel unterhält, wenn er an einem ganzen anderen Ort steht. Das dritte Argument legt den Spezialeffekt fest,
welcher auf Nichtspielercharakteren mit einem Gespräch erzeugt werden soll. Ist dieser Wert null, so wird
kein Effekt verwendet.
Das vierte Argument legt die Fehlermeldung fest, welche erscheint, wenn der Nichtspielercharakter bereits in
ein Gespräch verwickelt ist und die letzten beiden Argumente legen die Texte für die beiden immer wieder
verwendeten Buttons "Ende" und "Zurück" fest.
Das erste Argument des Aufrufs von AInfo.init legt das verwendete Kameraobjekt für Gespräche fest. Das
zweite Argument legt die Überspringtaste für Texte fest. Ist dieser Wert -1, so ist es den Spielern nicht
möglich, Texte zu überspringen. Das dritte Argument legt den Zeitintervall in Sekunden fest, in welchem ein
mögliches Überspringen durch den Spieler überprüft werden soll. Das bedeutet, dass der erschiene Text und
der abgespielte Ton nicht unbedingt unmittelbar nach der Betätigung der Escape-Taste ausgeblendet werden.
Die beiden letzten Argumente legen die Animationen für die Einheiten fest, die während eines Gesprächs
abgespielt werden sollen.
Leider gibt es kaum Modelle, die solche Animationen besitzen. Normalerweise besitzt nur das Portrait-Modell
einer Einheit eine Sprechanimation. Sind diese beiden Werte null, so werden keine Animationen abgespielt.

Nun möchte ich aber noch darauf eingehen, was denn nun eigentlich AInfo macht. AInfo stellt praktisch eine
Information dar. Man kann sie sich auch als einfachen Button vorstellen, der eine Aktion auslöst.
Allerdings bietet sie noch weitere Möglichkeiten. So kann eine Information zum Beispiel dauerhaft sein, was
bedeutet, dass sie immer wieder zur Auswahl steht (insofern die Bedingung true liefert). Dazu aber später
mehr. Schreiben wir zunächst einmal noch etwas mehr Code, in welchem wir ein kleines Beispielgespräch mit
dem Nichtspielercharakter Peter definieren. Dazu verwende ich mal wieder meine geliebte Vererbung, auch wenn
man es diesmal tatsächlich mit statischen Elementen umsetzen könnte, da ein Gespräch normalerweise pro Karte
einzigartig sein sollte:

library MyTalkLibrary requires Asl

	struct TalkPeter extends ATalk

		private method startPageAction takes nothing returns nothing
			call this.showInfo(0)
			call this.showInfo(1)
			call this.show()
		endmethod

		private static method infoAction0 takes AInfo info returns nothing
			call speech(info, false, "Hallo.", null)
			call speech(info, true, "Hallo. Wer bist du?", null)
			call speech(info, false, "Ich bin der singende, tanzende Abschaum der Welt.", null)
			call info.talk().showStartPage()
		endmethod

		public static method create takes nothing returns thistype
			local thistype this = thistype.create(gg_unit_peter, thistype.startPageAction)
			call this.addInfo(true, false, 0, thistype.infoAction0, "Hallo.")
			call this.addExitButton()
			return this
		endmethod

		private static method onInit takes nothing returns nothing
			call ATalk.init("smart", 300.0, "Abilities\\Spells\\Other\\TalkToMe\\TalkToMe.mdl", "Ziel unterhält sich bereits.", "Ende", "Zurück")
			call AInfo.init(gg_cam_talk, KEY_ESCAPE, 1.0, "speech", "listen")
		endmethod
	endstruct

endlibrary

Wie man sieht, erzeugen wir zunächst im Konstruktor unsere Instanz eines von ATalk geerbten Objekts. Dabei
werden im Konstruktor die verwendete Einheit und die verwende Startseitenfunktion aufgerufen. Die
Startseitenfunktion wird immer dann aufgerufen, wenn ein Charakter ein Gespräch mit dem
Nichtspielercharakter beginnt und erhält das Gespräch als Parameter.
Normalerweise ruft man in ihr mittels showInfo und dem zugehörigen Index alle AInfo-Instanzen des Gesprächs
auf, welche als Button eingezeigt werden sollen, natürlich nur wenn ihre Bedingung 0 ist oder true liefert.
Außerdem zeigen wir nach diesen Aufrufen noch den gesamten Gesprächs-Warcraft-Dialog mittels der Methode
show an.
Als nächstes Fügen wir ein AInfo-Objekt hinzu. Dazu bietet ATalk eine Art Kopie des Konstruktors eines
AInfo-Objektes an, welches das erzeugte AInfo automatisch dem Gespräch zuordnet und dessen internen Index
zurück gibt.
Das erste Argument gibt an, dass die Information dauerhaft angezeigt werden soll, also nicht verschwindet
nachdem der Spieler sie einmal ausgeführt hat. Der zweite Parameter gibt an, dass sie nicht "wichtig" ist.
Das bedeutet, dass sie nicht beim Öffnen des Dialogs automatisch ausgeführt, ohne dass der Spieler
irgendeinen Button drücken muss. Als nächstes folgt das Argument für die Bedingung, welches bei uns 0 ist,
da wir keine benötigen, dann das Argument für die Aktion.
Sowohl die Bedingungs- als auch die Aktions-Funktion erhalten die AInfo-Instanz als Parameter, über welche
man natürlich auch direkt auf die ATalk-Instanz zugreifen kann. An unserer Beispielfunktion infoAction0 kann
man gut erkennen, wie die meisten AInfo-Funktionen aussehen, wenn es sich um ein einfaches Gespräch handelt.
Mittels der globalen Funktio speech (Wäre es eine statische Methode, so würde die TriggerSleepAction in
unserer Funktion ignoriert werden, da sie vermutlich in einem neuen Thread gestartet werden würde.
Normalerweise geschieht das nur, wenn man eine Methode noch vor ihrer Definition aufruft, was eigentlich
nicht der Fall sein sollte. Trotzdem hat es bei mir in der Praxis nicht geklappt. Ich habe schon Vexorian
davon berichtet, mal schauen was passieren wird) kann man einen Text, alternativ auch mit Ton, à la Gothic
anzeigen lassen. Das erste Argument muss die Info-Instanz seien (diese würde bei einer Methode wegfallen),
das zweite Argument legt fest, ob der Text vom Nichtspielercharakter zum Spielercharakter gesagt wird oder
eben andersherum. Das dritte Argument legt den angezeigten Untertitel fest und das vierte Argument das
abgespielte Tonobjekt. Ist das letzte Argument null, so wird normalerweise 5 Sekunden gewartet, ansonsten
eben die Länge des Tonobjekts. Erinnern wir uns hierbei kurz an unserer Initialisierung der Struktur AInfo.
Dabei haben eine Übersprungstaste und eine Übersprungsüberprüfungsrate festgelegt. Drückt der Spieler also
bei einem dieser speech-Aufrufe die Taste Escape, so sollte diese spätestens nach ca. einer Sekunde
übersprungen werden. Zu guter Letzt rufen wir dann noch die Methode showStartPage der ATalk-Instanz auf, mit
welcher der Dialog zunächst geleert und danach die startPageAction-Funktion aufgerufen wird.
Da das gesamte System eigentlich besonders kompliziert sein sollte, werde ich auf eine große Menge an
weiteren Code-Beispielen verzichten. Ein weiteres sollte eigentlich reichen:

library MyTalkLibrary requires Asl

	struct TalkPeter extends ATalk

		private method startPageAction takes nothing returns nothing
			call this.showInfo(0)
			call this.showInfo(1)
			call this.showInfo(2)
			call this.show()
		endmethod

		private static method infoCondition0 takes AInfo info returns boolean
			local unit characterUnit = info.talk().character().unit()
			local boolean result = GetUnitTypeId(characterUnit) == 'Hpal'
			set characterUnit = null
			return result
		endmethod

		private static method infoAction0 takes AInfo info returns nothing
			call speech(info, true, "Du verdammter Paladin, verpiss dich bloß!", null)
			call info.talk.disable()
		endmethod

		private static method infoCondition1 takes AInfo info returns boolean
			local unit characterUnit = info.talk().character().unit()
			local boolean result = GetUnitTypeId(characterUnit) != 'Hpal'
			set characterUnit = null
			return result
		endmethod

		private static method infoAction1 takes AInfo info returns nothing
			call speech(info, false, "Hallo.", null)
			call speech(info, true, "Hallo. Wer bist du?", null)
			call speech(info, false, "Ich bin der singende, tanzende Abschaum der Welt.", null)
			call info.talk().showInfo(3)
			call info.talk().showInfo(4)
			call info.talk().showInfo(5)
			call info.talk().show()
		endmethod

		private static method infoAction0_0 takes AInfo info returns nothing
			call speech(info, false, "Und, wie läuft's so?", null)
			call speech(info, true, "Nerv nich!", null)
			call info.talk().showStartPage()
		endmethod

		private static method infoAction0_1 takes AInfo info returns nothing
			call speech(info, false, "Nerv nich!", null)
			call speech(info, true, "Und, wie läuft's so?", null)
			call info.talk().showStartPage()
		endmethod

		public static method create takes nothing returns thistype
			local thistype this = thistype.create(gg_unit_peter, startPageAction)
			call this.addInfo(false, true, infoCondition0, infoAction0, null) //0
			call this.addInfo(true, false, infoCondition1, infoAction1, "Hallo.") //1
			call this.addExitButton() //2

			call this.addInfo(true, false, 0, infoAction0_0, "Und, wie läuft's so?") //3
			call this.addInfo(true, false, 0, infoAction0_1, "Nerv nich!") //4
			call this.addBackToStartPageButton() //5
			return this
		endmethod

		private static method onInit takes nothing returns nothing
			call ATalk.init("smart", 300.0, "Abilities\\Spells\\Other\\TalkToMe\\TalkToMe.mdl", "Ziel unterhält sich bereits.", "Ende", "Zurück")
			call AInfo.init(gg_cam_talk, KEY_ESCAPE, 1.0, "speech", "listen")
		endmethod
	endstruct

endlibrary

Bei größeren Gesprächen kann es schon einmal zu Verwirrung bezüglich der Indizes kommen, allerdings ist dies
die schnellste Methode auf AInfo-Instanzen zuzugreifen. Natürlich kann man die Infos auch mit ihrem
Konstruktor erzeugen und in Elemente der Struktur speichern, jedoch würde das kaum einen Unterschied machen.
Zur Hilfe habe ich hinter jeden addInfo-Aufruf den dazugehörigen Index geschrieben.
Die Methode addBackToStartPageButton fügt einen Zurück-Button hinzu, welcher immer die Funktion
startPageAction aufruft.
Die Instanzen mit den Indizes 3 und 4 werden nicht auf der Startseite, sondern erst nachdem man "Hallo."
gesagt hat angezeigt.
Die "wichtige" Information erscheint, wenn der Charakter ein Paladin ist. Daraufhin liefert auch die
Bedingung von "Hallo." immer false, wodurch ein Paladin sich einmal eine Beleidigung anhören muss und sich
dann nicht mehr mit dem Nichtspielercharakter unterhalten kann.