"foreach"-Schleifen sind Schleifen, die auf Behältertypen angewandt werden können. Sie ermöglichen
das Durchlaufen eines gesamten Behälters ohne manuelles Überprüfen der Position.
In der ASL sind "foreach"-Schleifen mit Hilfe von Textmakros verwendbar.
Diese Textmakros können jedoch nur auf Behältertypen angewandt werden, welche eine Iteratorenstruktur
verwenden. Momentan sind das die Behälterstrukturen, die man mittels der Textmakros A_LIST und A_MAP erzeugen kann.
Verwendet man eine "foreach"-Schleife mit einer bestimmten Behältervariable, so muss die globale Variable "aIterator" für einen Iteratorenzugriff verwendet werden.
Dies ist nötig, da das Makro ansonsten eine eigene lokale Variable anlegen müsste, womit es nur am
Anfang der Funktion verwendbar wäre.
Da eine globale Iteratoren-Variable verwendet wird, empfiehlt es sich auf keinen Fall, TriggerSleepAction-Aufrufe in "foreach"-Blöcken zu verwenden, da die globale Variable in dieser Zeit
von einer anderen Funktion verändert werden könnte.
Eine alternative zum Iteratorzugriff, stellt der Zugriff auf eine eigene Variable dar, welche der "foreach"-Anweisung übergeben wird.
Diese enthält bei jedem Durchlauf die Entsprechenden Daten des globalen Iterators.
Auch Rückwärtsdurchläufe sind möglich.

function TestFunction takes nothing returns nothing
	local AIntegerList list = AIntegerList.createWithSize(10, 0)
	local integer value
	//! runtextmacro A_FOREACH("list")
		debug call Print("Data: " + I2S(AIntegerListIterator(aIterator.data()))) // alternativer Iteratorzugriff
	//! runtextmacro A_FOREACH_END()
	//! runtextmacro A_FOREACH_2("value", "list", "AIntegerListIterator")
		debug call Print("Data: " + I2S(value))
	//! runtextmacro A_FOREACH_END()
	//! runtextmacro A_REVERSE_FOREACH_2("value", "list", "AIntegerListIterator")
		debug call Print("Data: " + I2S(value))
	//! runtextmacro A_REVERSE_FOREACH_END()
endfunction