Quests
In vielen Rollenspielen erhält der Spieler diverse Aufgaben für deren Erfüllung meist Belohnungen ausstehen. Diese Aufgaben werden auch Quests genannt. Sie sorgen dafür, dass man vom einfachen Gegnertöten weg und hin zu einem mit Handlung vollgepackten Spiel kommt. Natürlich kann ein Quest auch sehr einfach aufgebaut sein, dennoch bietet es meist eine willkommene Abwechslung zum „ziellosen“ Töten.
Für die Umsetzung solcher Quests bietet die  ASL in ihrem Charaktersystem die Strukturen AAbstractQuest, AQuest und AQuestItem an.
Normalerweise werden davon nur die beiden Strukturen AQuest und AQuestItem benötigt. AabstractQuest ist, wie der Name schon vermuten lässt, eine abstrakte Struktur (in vJass gibt es keine wirklich abstrakten Strukturen, lediglich Schnittstellen (interfaces)) und somit auch die Elternstruktur der beiden anderen.
Quests sind in der ASL folgendermaßen aufgebaut. Ein Quest hat einen Namen, einen zugehörigen Charakter, welcher das Quest erledigen soll, ein oder mehrere Ziele (hier kommt AquestItem ins Spiel) und alternativ auch eine Beschreibung und einen Symbolpfad für das Warcraft-3-Quest-Log. Selbstverständlich ist es auch möglich Quests für alle Charaktere und nicht nur für einen, also sozusagen Haupt-Quests, zu erzeugen. Dabei wird als zugehöriger Charakter einfach der Wert 0 eingetragen.
Doch kommen wir nun zunächst zur Initialisierung der benötigten Strukturen. Wie alle Strukturen der ASL, die explizit vom Benutzer initialisiert werden müssen, so besitzen auch die Strukturen AAbstractQuest und AQuest eigene Initialisierungsmethoden. Diese sollten auf jeden Fall vor der Benutzung jeglicher anderer Methoden (das gilt natürlich auch für die Struktur AQuestItem) aufgerufen werden:

library MyQuestLibrary requires Asl

	struct QuestHelpPeter
		private static method onInit takes nothing returns nothing
			call AAbstractQuest.init(30.0, "Sound\\Interface\\QuestNew.wav", "Sound\\Interface\\QuestCompleted.wav", "Sound\\Interface\\QuestFailed.wav", "%s (|c00ffffffNeu|r)", "%s (|cffc3dbffAbgeschlossen|r)", "%s (|c00ff0000Fehlgeschlagen|r)", "+%i Stufe(n)", "+%i Fähigkeitenpunkt(e)", "+%i Erfahrung", "+%i Stärke", "+%i Geschick", "+%i Wissen", "+%i Goldmünze(n)", "+%i Holz")
			call AQuest.init0(true, "Sound\\Interface\\QuestLog.wav")
		endmethod
	endmethod
	
endlibrary

Wie man sieht nimmt vor allem die Initialisierungsmethode der Struktur AabstractQuest eine Menge Argumente entgegen. Das erste Argument gibt die Ping-Rate aller aktiven Quests an. Jedes Quest und auch jedes Quest-Ziel kann einen Ping auf der Minikarte anzeigen lassen solange es aktiv ist. Beträgt der Wert dieser Rate 0, so wird die Pingfunktion für alle Quests und Quest-Ziele deaktivert. Die drei darauffolgenden Argumente geben die Dateipfade der Tondateien an, welche bei den drei möglichen Statusänderungen abgespielt werden. Sowohl ein Quest als auch ein Quest-Ziel kann entweder den Status „Neu“, „Abgeschlossen“ oder „Fehlgeschlagen“ haben. Alle weiteren Argumente dienen dem Quest-Belohnungssystem bzw. den möglichen Belohnungstypen dessen.
Die Initialisierungsmethode der Struktur Aquest - init0 – empfängt dagegen weit weniger Argumente. Das erste Argument gibt an, ob das warcraftinterne Quest-Log (welches normalerweise über F9 aufrufbar ist) verwendet werden soll. Ist dieses Argument wahr, so werden alle Änderungen am Quest auch automatisch im Quest-Log durchgeführt. Das zweite Argument gibt den Dateipfad der Tondatei an, welche bei Aktualisierungsnachrichten abgespielt werden soll.
Nachdem wir nun also die beiden Systeme initialisiert haben, können wir uns ans Werk machen, ein einfaches Quest zu basteln.
Am Anfang aller Dinge kommt natürlich erst einmal die Überlegung, worum es in unserem Quest überhaupt gehen soll. Ein einfaches Beispiel wäre das Unterstützen eines Verbündeten Kriegers, welcher von bösen Kreaturen angegriffen wird.
Wie das Quest letztendlich implementiert wird ist natürlich die Sache eines jeden selbst. Ich bevorzuge dabei die Methode der Vererbung:

library MyQuestLibrary initializer initFunction requires Asl

	struct QuestHelpPeter extends AQuest

		public static method create takes ACharacter character returns thistype
			local thistype this = thistype.allocate(character, "Hilf Peter!")
			return this
		endmethod
		
		private static method initFunction takes nothing returns nothing
			call AAbstractQuest.init(30.0, "Sound\\Interface\\QuestNew.wav", "Sound\\Interface\\QuestCompleted.wav", "Sound\\Interface\\QuestFailed.wav", "%s (|c00ffffffNeu|r)", "%s (|cffc3dbffAbgeschlossen|r)", "%s (|c00ff0000Fehlgeschlagen|r)", "+%i Stufe(n)", "+%i Fähigkeitenpunkt(e)", "+%i Erfahrung", "+%i Stärke", "+%i Geschick", "+%i Wissen", "+%i Goldmünze(n)", "+%i Holz")
			call AQuest.init0(true, „Sound\\Interface\\QuestLog.wav“)
		endmethod
	endstruct
	
endlibrary

Damit habe ich nun nichts anderes getan als eine neue Struktur zu erschaffen, welche von AQuest erbt und deren Konstruktor überlädt.
Allerdings fehlen uns jetzt noch die Quest-Ziele und irgendein Auslöser, der das Quest aktiviert und sich um Dinge wie den Tod Peters kümmern.
Da bei den meisten Quests ein solches Schema angewandt wird, kann man mittels der Methoden setStateEvent, setStateCondition und setStateAction für jedes Quest und auch jedes Quest-Ziel ein Ereignis, eine Bedingung und eine Aktion pro Status festlegen.
Nehmen wir also einmal an, dass Peter auf einem Hügel steht und um ihn herum wütende Orks stehen, die jeden Moment losschlagen könnten. Betritt der Charakter nun ein bestimmtes Gebiet, in welchem sich Peter aufhält, so soll das Quest aktiviert werden. Außerdem wollen wir noch ein Quest-Ziel namens „Hilf Peter die Orks zu vertreiben.“ hinzufügen:

library MyQuestLibrary initializer initFunction requires Asl

	struct QuestHelpPeter extends AQuest
		private AQuestItem m_questItem0
		
		private method stateEventNew takes trigger usedTrigger returns nothing
			local event triggerEvent = TriggerRegisterEnterRectSimple(usedTrigger, gg_rct_peters_zone)
			set triggerEvent = null
		endmethod

		private method stateConditionNew takes nothing returns boolean
			local unit triggerUnit = GetTriggerUnit()
			local ACharacter character = ACharacter.getCharacterByUnit(triggerUnit)
			set triggerUnit = null
			return character == this.character()
		endmethod

		private method stateActionNew takes nothing returns nothing
			local player user = this.character().user()
			call this.questItem(0).setState(AAbstractQuest.stateNew)
			call TransmissionFromUnitForPlayer(user, gg_unit_peter, "Hilf mir, die verdammten Orks wollen mir an den Kragen!", null)
			set user = null
		endmethod

		public static method create takes ACharacter character returns thistype
			local thistype this = thistype.allocate(character, "Hilf Peter!")
			call this.setStateEvent(AAbstractQuest.stateNew, stateEventNew)
			call this.setStateCondtiion(AAbstractQuest.stateNew, stateConditionNew)
			call this.setStateAction(AAbstractQuest.stateNew, stateActionNew)
			set this.m_questItem0 = AquestItem.create(this, "Hilf Peter die Orks zu vertreiben.")
			return this
		endmethod
		
		private static method onInit takes nothing returns nothing
			call AAbstractQuest.init(30.0, "Sound\\Interface\\QuestNew.wav“, „Sound\\Interface\\QuestCompleted.wav", "Sound\\Interface\\QuestFailed.wav", "%s (|c00ffffffNeu|r)", "%s (|cffc3dbffAbgeschlossen|r)", "%s (|c00ff0000Fehlgeschlagen|r)", "+%i Stufe(n)", "+%i Fähigkeitenpunkt(e)", "+%i Erfahrung", "+%i Stärke", "+%i Geschick", "+%i Wissen", "+%i Goldmünze(n)", "+%i Holz")
			call AQuest.init0(true, "Sound\\Interface\\QuestLog.wav")
		endmethod
	endstruct
	
endlibrary

So, da hat sich jetzt auf den ersten Blick eine Menge geändert. Gehen wir die Änderungen einmal Schritt für Schritt durch. Im Konstruktor werden dem Quest wie geplant ein Ereignis, eine Bedingung und eine Aktion für den Status „Neu“ zugewiesen. Dafür wird von der vJass-Implementation von Funktionsschnittstellen (function interfaces) Gebrauch gemacht. Die zugehörigen Funktionen sind oberhalb der Struktur definiert.
Zunächst einmal ist dort die Funktion stateEventNew. Sie hat die Aufgabe, wie auch alle anderen Ereignisfunktionen von Quests, dem internen Statusauslöser der Quest-Instanz ein Ereignis zuzuweisen. Dazu wird die Funktion  TriggerRegisterEnterRectSimple aufgerufen, da das Quest ja aktiviert werden soll, sobald der Charakter das entsprechende Gebiet betritt.
Natürlich müssen wir jetzt auch noch prüfen, ob die Einheit, welche das Gebiet betritt ein Charakter ist. Bei anderen Ereignissen hätten wir die Charaktereinheit schon bei der Registrierung direkt als Parameter übergeben können, in diesem Fall geht das leider nicht. Also prüft die Funktion stateConditionNew das. Sie hat keinen Auslöser als Parameter, da sie intern von einer standardmäßigen Bedingung mittels .evaluate aufgerufen wird. Es gilt hier noch zu beachten, dass sie wie eine normale Bedingung immer einen boolean-Wert zurückgeben muss.
Und zu guter Letzt wird noch die Aktionsfunktion namens stateActionNew definiert. Darin wird zunächst einmal das erste Quest-Ziel ebenfalls auf den Status „Neu“ gesetzt, was nicht automatisch passiert. Das liegt daran, dass man auch Quest-Ziele definieren können muss, die erst nachträglich erscheinen. Würde das Quest aber intern all seine Ziele auf den Status „Neu“ setzen, so wäre dies unmöglich.
Außerdem wird noch eine kleine Nachricht von Peter an den Besitzer des Charakters gesendet. Dazu wird eine ebenfalls aus der ASL stammende Funktion verwendet.
Nun ist also das Quest aktiv, doch was kommt als nächstes? Was passiert zum Beispiel, wenn Peter stirbt oder wann ist Peter gerettet?
Implementieren wir also als nächstes die Möglichkeit eines Fehlschlags, falls Peter stirbt. Es gilt hierbei zu beachten, dass dieses Quest nicht unbedingt mehrspielertauglich ist, außer man nimmt es in Kauf, dass es nur solange abgeschlossen werden kann, solange es bei keinem Spieler fehlschlägt und die eher merkwürdige Tatsache, dass Peter die ganze Zeit von Orks bedroht wird.

///// ACHTUNG ACHTUNG, ab hier Code umschreiben und an neue JassHelper-Version anpassen
library MyQuestLibrary initializer initFunction requires Asl

	private function initFunction takes nothing returns nothing
		call AAbstractQuest.init(30.0, "Sound\\Interface\\QuestNew.wav", "Sound\\Interface\\QuestCompleted.wav", "Sound\\Interface\\QuestFailed.wav", "%s (|c00ffffffNeu|r)", "%s (|cffc3dbffAbgeschlossen|r)", "%s (|c00ff0000Fehlgeschlagen|r)", "+%i Stufe(n)", "+%i Fähigkeitenpunkt(e)", "+%i Erfahrung", "+%i Stärke", "+%i Geschick", "+%i Wissen", "+%i Goldmünze(n)", "+%i Holz")
		call AQuest.init0(true, "Sound\\Interface\\QuestLog.wav")
	endfunction

	private function stateEventNew takes AQuest usedQuest, trigger usedTrigger returns nothing
		local event triggerEvent = TriggerRegisterEnterRectSimple(usedTrigger, gg_rct_peters_zone)
		set triggerEvent = null
	endfunction

	private function stateConditionNew takes AQuest usedQuest returns boolean
		local unit triggerUnit = GetTriggerUnit()
		local ACharacter character = ACharacter.getCharacterByUnit(triggerUnit)
		set triggerUnit = null
		return character == usedQuest.character()
	endfunction

	private function stateActionNew takes AQuest  usedQuest returns nothing
		local player user = usedQuest.character().user()
		call  usedQuest.questItem(0).setState(AAbstractQuest.stateNew)
		call TransmissionFromUnitForPlayer(user, gg_unit_peter, "Hilf mir, die verdammten Orks wollen mir an den Kragen!", null)
		set user = null
	endfunction

	private function stateEventFailed takes AQuest usedQuest, trigger usedTrigger returns nothing
		local event triggerEvent = TriggerRegisterUnitEvent(usedTrigger, gg_unit_peter, EVENT_UNIT_DEATH)
		set triggerEvent = null
	endfunction

	private function stateActionFailed takes AQuest  usedQuest returns nothing
		local player user = usedQuest.character().user()
		if (usedQuest.questItem(0).state() == AabstractQuest.stateNew) then
			call  usedQuest.questItem(0).setState(AAbstractQuest.stateFailed)
		endif
		call TransmissionFromUnitForPlayer(user, gg_unit_peter, "Ahhh, ich sterbe, wie furchtbar!!!!einseinself", null)
		set user = null
	endfunction

	struct QuestHelpPeter extends AQuest
		private AQuestItem m_questItem0

		public static method create takes ACharacter character returns thistype
			local thistype this = thistype.allocate(character, "Hilf Peter!")
			call this.setStateEvent(AAbstractQuest.stateNew, stateEventNew)
			call this.setStateCondtiion(AAbstractQuest.stateNew, stateConditionNew)
			call this.setStateAction(AAbstractQuest.stateNew, stateActionNew)
			call this.setStateEvent(AAbstractQuest.stateFailed, stateEventFailed)
			call this.setStateAction(AAbstractQuest.stateFailed, stateEventAction)
			set this.m_questItem0 = AQuestItem.create(this, "Hilf Peter die Orks zu vertreiben.")
			return this
		endmethod			
	endstruct
	
endlibrary

Dazu gibt es im Grunde genommen nicht viel zu schreiben, da es genau das gleiche Schema, nur dieses Mal ohne Bedingung, wie beim Status „Neu“ ist. Falls man es tatsächlich mehrspielerfähig machen wollte, so müsste man jetzt noch die Quest-Instanzen der anderen Charaktere zerstören, worauf ich aber verzichten werde.
Nun sollte man das Quest vielleicht auch noch abschließen können und dafür eine nette Belohnung erhalten.
Dafür nehme ich das einfache Beispiel zur Hand, dass man eine bestimmte Anzahl von Orks töten muss, bis Peter „geholfen“ ist. Das erfordert ein zusätzliches Zähler-Element, welches die Anzahl der getöteten Orks speichert. Wir nehmen einfach einmal, dass immer neue Orks erzeugt werden, die Peter auf seinem kleinen Hügel attackieren.
Nachdem der Charakter 10 von ihnen getötet hat, freut sich Peter über die Hilfe und gibt einem ein wenig Gold und natürlich erhält man auch noch etwas Erfahrung. Danach kann man Peter abkratzen lassen.

library MyQuestLibrary initializer initFunction requires Asl

	private function initFunction takes nothing returns nothing
		call AAbstractQuest.init(30.0, "Sound\\Interface\\QuestNew.wav", "Sound\\Interface\\QuestCompleted.wav", "Sound\\Interface\\QuestFailed.wav", "%s (|c00ffffffNeu|r)", "%s (|cffc3dbffAbgeschlossen|r)", "%s (|c00ff0000Fehlgeschlagen|r)", "+%i Stufe(n)", "+%i Fähigkeitenpunkt(e)", "+%i Erfahrung", "+%i Stärke", "+%i Geschick", "+%i Wissen", "+%i Goldmünze(n)", "+%i Holz")
		call AQuest.init0(true, "Sound\\Interface\\QuestLog.wav")
	endfunction

	private function stateEventNew takes AQuest usedQuest, trigger usedTrigger returns nothing
		local event triggerEvent = TriggerRegisterEnterRectSimple(usedTrigger, gg_rct_peters_zone)
		set triggerEvent = null
	endfunction

	private function stateConditionNew takes AQuest usedQuest returns boolean
		local unit triggerUnit = GetTriggerUnit()
		local ACharacter character = ACharacter.getCharacterByUnit(triggerUnit)
		set triggerUnit = null
		return character == usedQuest.character()
	endfunction

	private function stateActionNew takes AQuest  usedQuest returns nothing
		local player user = usedQuest.character().user()
		call  usedQuest.questItem(0).setState(AAbstractQuest.stateNew)
		call TransmissionFromUnitForPlayer(user, gg_unit_peter, "Hilf mir, die verdammten Orks wollen mir an den Kragen!", null)
		set user = null
	endfunction

	private function stateEventFailed takes AQuest usedQuest, trigger usedTrigger returns nothing
		local event triggerEvent = TriggerRegisterUnitEvent(usedTrigger, gg_unit_peter, EVENT_UNIT_DEATH)
		set triggerEvent = null
	endfunction

	private function stateActionFailed takes AQuest  usedQuest returns nothing
		local player user = usedQuest.character().user()
		if (usedQuest.questItem(0).state() == AabstractQuest.stateNew) then
			call  usedQuest.questItem(0).setState(AAbstractQuest.stateFailed)
		endif
		call TransmissionFromUnitForPlayer(user, gg_unit_peter, "Ahhh, ich sterbe, wie furchtbar!!!!einseinself", null)
		set user = null
	endfunction

	private function stateEventCompleted0 takes AQuestItem questItem, trigger usedTrigger returns nothing
		local player orcOwner = Player(10)
		local event triggerEvent = TriggerRegisterUnitEventSimple(usedTrigger, orcOwner, EVENT_PLAYER_UNIT_DEATH)
		set orcOwner = null
		set triggerEvent = null
	endfunction

	private function stateConditionCompleted0 takes AQuestItem questItem returns boolean
		local unit triggerUnit = GetTriggerUnit()
		local unit killingUnit = GetKillingUnit()
		local boolean result = false
		if (Acharacter.getCharacterByUnit(killingUnit) == questItem.character()) then
			if (GetUnitTypeId(triggerUnit) == 'ogru') then
				call QuestHelpPeter(questItem.quest()).addKilledOrcs(1)
				set result = QuestHelpPeter(questItem.quest()).killedOrcs() == 10
				if (not result) then
					call questItem.quest().displayUpdateMessage("Noch zu tötende Orks: " + I2S(10 – QuestHelpPeter(questItem.quest()).killedOrcs()) + ".")
				endif
			endif
		endif
		set triggerUnit = null
		set killingUnit = null
		return result
	endfunction

	private function stateActionCompleted0 takes AQuestItem questItem returns nothing
		local player user = questItem.character().user()
		call TransmissionFromUnitForPlayer(user, gg_unit_peter, "Danke, du hast mir wirklich sehr geholfen! Hier hast du etwas Gold.", null)
		set user = null
	endfunction

	struct QuestHelpPeter extends AQuest
		private AQuestItem m_questItem0
		private integer m_killedOrcs

		public method addKilledOrcs takes integer number returns nothing
			set this.m_killedOrcs = this.m_killedOrcs + number
		endmethod

		public method killedOrcs takes nothing returns integer
			return this.m_killedOrcs
		endmethod
		
		public static method create takes ACharacter character returns thistype
			local thistype this = thistype.allocate(character, "Hilf Peter!")
			call this.setStateEvent(AAbstractQuest.stateNew, stateEventNew)
			call this.setStateCondtiion(AAbstractQuest.stateNew, stateConditionNew)
			call this.setStateAction(AAbstractQuest.stateNew, stateActionNew)
			call this.setStateEvent(AAbstractQuest.stateFailed, stateEventFailed)
			call this.setStateAction(AAbstractQuest.stateFailed, stateEventAction)
			set this.m_questItem0 = AQuestItem.create(this, "Hilf Peter die Orks zu vertreiben.")
			call this.m_questItem0.setStateEvent(AAbstractQuest.stateCompleted, stateEventCompleted0)
			call this.m_questItem0.setStateCondition(AAbstractQuest.stateCompleted, stateConditionCompleted0)
			call this.m_questItem0.setStateAction(AAbstractQuest.stateCompleted, stateActionCompleted0)
			call this m_questItem0.setReward(AAbstractQuest.rewardGold, 300)
			call this m_questItem0.setReward(AAbstractQuest.rewardExperience, 1000)
			set this.m_killedOrcs = 0
			return this
		endmethod			
	endstruct
	
endlibrary

Bei diesem Beispiel brauchen wir die Aktion nur noch aufgrund der abgesendeten Nachricht von Peter. Der Zähler wird bereits in der Bedingung bearbeitet. Dazu muss der Rückgabewert der Methode quest der Struktur AQuestItem zum Typ QuestHelpPeter konvertiert werden.
Dieses Mal müssen wir den Status des Quest-Ziels nicht manuell setzen. Wird einem Quest der Status „Abgeschlossen“ oder „Fehlgeschlagen“ zugewiesen, so werden alle Quest-Ziele mit dem Status „Neu“ auf denselben Status gesetzt.
Die Quest-Belohnung hätte man in diesem Fall genauso gut der Quest- und nicht der Quest-Ziel-Instanz zuordnen können.
Belohnungen werden automatisch an den oder die Charaktere verteilt, sobald der entsprechende Status gesetzt wird.
Möchte man das Quest nun tatsächlich in seiner Karte benutzen, so muss man es nur noch mittels QuestHelpPeter.create(character) für den entsprechenden Charakter erzeugen. Vorher sollte man eventuell noch die globalen Variablen für den Peter und das Gebiet austauschen.
Es gilt noch anzumerken, dass man den Aufruf der Methode setEvent auch weglassen kann. In einem solchen Fall wird dann kein interner Auslöser erzeugt und die Bedingung wird überprüft sobald man setState aufruft (insofern man eine festgelegt hat), daraufhin wird dann auch die festgelegte Aktion aufgerufen.
So kann man das ganze auch mittels setState aus einem schon vorhandenen Auslöser steuern. Will man das Quest jetzt als Haupt-Quest, sprich für alle Spielercharakter, implementieren, so muss man den Parameter character entfernen und stattdessen 0 übergeben. Außerdem muss man die Bedingungen und Aktionen an alle Spieler anpassen:

library MyQuestLibrary initializer initFunction requires Asl

	private function initFunction takes nothing returns nothing
		call AAbstractQuest.init(30.0, "Sound\\Interface\\QuestNew.wav", "Sound\\Interface\\QuestCompleted.wav", "Sound\\Interface\\QuestFailed.wav", "%s (|c00ffffffNeu|r)", "%s (|cffc3dbffAbgeschlossen|r)", "%s (|c00ff0000Fehlgeschlagen|r)", "+%i Stufe(n)", "+%i Fähigkeitenpunkt(e)", "+%i Erfahrung", "+%i Stärke", "+%i Geschick", "+%i Wissen", "+%i Goldmünze(n)", "+%i Holz")
		call AQuest.init0(true, "Sound\\Interface\\QuestLog.wav")
	endfunction

	private function stateEventNew takes AQuest usedQuest, trigger usedTrigger returns nothing
		local event triggerEvent = TriggerRegisterEnterRectSimple(usedTrigger, gg_rct_peters_zone)
		set triggerEvent = null
	endfunction

	private function stateConditionNew takes AQuest usedQuest returns boolean
		local unit triggerUnit = GetTriggerUnit()
		local boolean result  = ACharacter.isUnitCharacter(triggerUnit)
		set triggerUnit = null
		return result
	endfunction

	private function stateActionNew takes AQuest  usedQuest returns nothing
		call  usedQuest.questItem(0).setState(AAbstractQuest.stateNew)
		call TransmissionFromUnit(user, gg_unit_peter, "Helft mir, die verdammten Orks wollen mir an den Kragen!", null)
	endfunction

	private function stateEventFailed takes AQuest usedQuest, trigger usedTrigger returns nothing
		local event triggerEvent = TriggerRegisterUnitEvent(usedTrigger, gg_unit_peter, EVENT_UNIT_DEATH)
		set triggerEvent = null
	endfunction

	private function stateActionFailed takes AQuest  usedQuest returns nothing
		if (usedQuest.questItem(0).state() == AabstractQuest.stateNew) then
			call  usedQuest.questItem(0).setState(AAbstractQuest.stateFailed)
		endif
		call TransmissionFromUnit(null, gg_unit_peter, "Ahhh, ich sterbe, wie furchtbar!!!!einseinself", null)
	endfunction

	private function stateEventCompleted0 takes AQuestItem questItem, trigger usedTrigger returns nothing
		local player orcOwner = Player(10)
		local event triggerEvent = TriggerRegisterUnitEventSimple(usedTrigger, orcOwner, EVENT_PLAYER_UNIT_DEATH)
		set orcOwner = null
		set triggerEvent = null
	endfunction

	private function stateConditionCompleted0 takes AQuestItem questItem returns boolean
		local unit triggerUnit = GetTriggerUnit()
		local unit killingUnit = GetKillingUnit()
		local boolean result = false
		if (ACharacter.isUnitCharacter(killingUnit)) then
			if (GetUnitTypeId(triggerUnit) == 'ogru') then
				call QuestHelpPeter(questItem.quest()).addKilledOrcs(1)
				set result = QuestHelpPeter(questItem.quest()).killedOrcs() == 10
				if (not result) then
					call questItem.quest().displayUpdateMessage("Noch zu tötende Orks: " + I2S(10 – QuestHelpPeter(questItem.quest()).killedOrcs()) + ".")
				endif
			endif
		endif
		set triggerUnit = null
		set killingUnit = null
		return result
	endfunction

	private function stateActionCompleted0 takes AQuestItem questItem returns nothing
		call TransmissionFromUnit(gg_unit_peter, "Danke, ihr habt mir wirklich sehr geholfen! Hier habt ihr etwas Gold.", null)
	endfunction

	struct QuestHelpPeter extends AQuest
		private AQuestItem m_questItem0
		private integer m_killedOrcs

		public method addKilledOrcs takes integer number returns nothing
			set this.m_killedOrcs = this.m_killedOrcs + number
		endmethod

		public method killedOrcs takes nothing returns integer
			return this.m_killedOrcs
		endmethod
		
		public static method create takes nothing returns thistype
			local thistype this = thistype.allocate(0, "Hilf Peter!")
			call this.setStateEvent(AAbstractQuest.stateNew, stateEventNew)
			call this.setStateCondtiion(AAbstractQuest.stateNew, stateConditionNew)
			call this.setStateAction(AAbstractQuest.stateNew, stateActionNew)
			call this.setStateEvent(AAbstractQuest.stateFailed, stateEventFailed)
			call this.setStateAction(AAbstractQuest.stateFailed, stateEventAction)
			set this.m_questItem0 = AQuestItem.create(this, "Hilf Peter die Orks zu vertreiben.")
			call this.m_questItem0.setStateEvent(AAbstractQuest.stateCompleted, stateEventCompleted0)
			call this.m_questItem0.setStateCondition(AAbstractQuest.stateCompleted, stateConditionCompleted0)
			call this.m_questItem0.setStateAction(AAbstractQuest.stateCompleted, stateActionCompleted0)
			call this m_questItem0.setReward(AAbstractQuest.rewardGold, 300)
			call this m_questItem0.setReward(AAbstractQuest.rewardExperience, 1000)
			set this.m_killedOrcs = 0
			return this
		endmethod		
	endstruct
	
endlibrary

Hierbei könnte man jetzt natürlich auch statische Elemente verwenden, da das Quest ja nur ein einziges Mal erzeugt wird.
Ein kleines Problem bereitet mir zudem noch das Warcraft-Quest-Log, da dort ja die Quests aller Spieler angezeigt werden und man somit auch die Quests der anderen sieht. Eine Möglichkeit bestünde darin, einfach den Namen des entsprechenden Spielers vor den Quest-Namen zu setzen, was aber nicht gerade schön wäre.
Anderenfalls kann man auch einfach sein eigenes Quest-Log implementieren.
Nun könnten wir dem Ganzen noch seinen letzten Schliff geben, indem wir dem Quest ein Symbol und eine Beschreibung zuweisen und einen regelmäßigen Ping-Effekt erzeugen lassen:

library MyQuestLibrary initializer initFunction requires Asl

	private function initFunction takes nothing returns nothing
		call AAbstractQuest.init(30.0, "Sound\\Interface\\QuestNew.wav", "Sound\\Interface\\QuestCompleted.wav", "Sound\\Interface\\QuestFailed.wav", "%s (|c00ffffffNeu|r)", "%s (|cffc3dbffAbgeschlossen|r)", "%s (|c00ff0000Fehlgeschlagen|r)", "+%i Stufe(n)", "+%i Fähigkeitenpunkt(e)", "+%i Erfahrung", "+%i Stärke", "+%i Geschick", "+%i Wissen", "+%i Goldmünze(n)", "+%i Holz")
		call AQuest.init0(true, "Sound\\Interface\\QuestLog.wav")
	endfunction

	private function stateEventNew takes AQuest usedQuest, trigger usedTrigger returns nothing
		local event triggerEvent = TriggerRegisterEnterRectSimple(usedTrigger, gg_rct_peters_zone)
		set triggerEvent = null
	endfunction

	private function stateConditionNew takes AQuest usedQuest returns boolean
		local unit triggerUnit = GetTriggerUnit()
		local boolean result  = ACharacter.isUnitCharacter(triggerUnit)
		set triggerUnit = null
		return result
	endfunction

	private function stateActionNew takes AQuest  usedQuest returns nothing
		call  usedQuest.questItem(0).setState(AAbstractQuest.stateNew)
		call TransmissionFromUnit(user, gg_unit_peter, "Helft mir, die verdammten Orks wollen mir an den Kragen!", null)
	endfunction

	private function stateEventFailed takes AQuest usedQuest, trigger usedTrigger returns nothing
		local event triggerEvent = TriggerRegisterUnitEvent(usedTrigger, gg_unit_peter, EVENT_UNIT_DEATH)
		set triggerEvent = null
	endfunction

	private function stateActionFailed takes AQuest  usedQuest returns nothing
		if (usedQuest.questItem(0).state() == AabstractQuest.stateNew) then
			call  usedQuest.questItem(0).setState(AAbstractQuest.stateFailed)
		endif
		call TransmissionFromUnit(null, gg_unit_peter, "Ahhh, ich sterbe, wie furchtbar!!!!einseinself", null)
	endfunction

	private function stateEventCompleted0 takes AQuestItem questItem, trigger usedTrigger returns nothing
		local player orcOwner = Player(10)
		local event triggerEvent = TriggerRegisterUnitEventSimple(usedTrigger, orcOwner, EVENT_PLAYER_UNIT_DEATH)
		set orcOwner = null
		set triggerEvent = null
	endfunction

	private function stateConditionCompleted0 takes AQuestItem questItem returns boolean
		local unit triggerUnit = GetTriggerUnit()
		local unit killingUnit = GetKillingUnit()
		local boolean result = false
		if (ACharacter.isUnitCharacter(killingUnit)) then
			if (GetUnitTypeId(triggerUnit) == 'ogru') then
				call QuestHelpPeter(questItem.quest()).addKilledOrcs(1)
				set result = QuestHelpPeter(questItem.quest()).killedOrcs() == 10
				if (not result) then
					call questItem.quest().displayUpdateMessage("Noch zu tötende Orks: " + I2S(10 – QuestHelpPeter(questItem.quest()).killedOrcs()) + ".")
				endif
			endif
		endif
		set triggerUnit = null
		set killingUnit = null
		return result
	endfunction

	private function stateActionCompleted0 takes AQuestItem questItem returns nothing
		call TransmissionFromUnit(gg_unit_peter, "Danke, ihr habt mir wirklich sehr geholfen! Hier habt ihr etwas Gold.", null)
	endfunction

	struct QuestHelpPeter extends AQuest
		private AQuestItem m_questItem0
		private integer m_killedOrcs

		public method addKilledOrcs takes integer number returns nothing
			set this.m_killedOrcs = this.m_killedOrcs + number
		endmethod

		public method killedOrcs takes nothing returns integer
			return this.m_killedOrcs
		endmethod
		
		public static method create takes nothing returns thistype
			local thistype this = thistype.allocate(0, "Hilf Peter!")
			call this.setIconPath("ReplaceableTextures\\CommandButtons\\BTNVillagerMan.blp")
			call this.setDescription("Peter wurde auf seinem kleinen Hügel von Orks eingekreist. Helft ihm wenigstens eine Weile zu überleben.")
			call this.setStateEvent(AAbstractQuest.stateNew, stateEventNew)
			call this.setStateCondtiion(AAbstractQuest.stateNew, stateConditionNew)
			call this.setStateAction(AAbstractQuest.stateNew, stateActionNew)
			call this.setStateEvent(AAbstractQuest.stateFailed, stateEventFailed)
			call this.setStateAction(AAbstractQuest.stateFailed, stateEventAction)
			set this.m_questItem0 = AQuestItem.create(this, "Hilf Peter die Orks zu vertreiben.")
			call this.m_questItem0.setStateEvent(AAbstractQuest.stateCompleted, stateEventCompleted0)
			call this.m_questItem0.setStateCondition(AAbstractQuest.stateCompleted, stateConditionCompleted0)
			call this.m_questItem0.setStateAction(AAbstractQuest.stateCompleted, stateActionCompleted0)
			call this m_questItem0.setReward(AAbstractQuest.rewardGold, 300)
			call this m_questItem0.setReward(AAbstractQuest.rewardExperience, 1000)
			call this.m_questItem0.setPingRect(gg_rct_peters_zone)
			set this.m_killedOrcs = 0
			return this
		endmethod			
	endstruct
	
endlibrary

Dynamische Ping-Koordinaten wie zum Beispiel die einer sich bewegenden Einheit, werden leider noch nicht vom System unterstützt.