Möchte man gerne die Zeit stoppen, um zum Beispiel die Dauer von bestimmten Funktionsaufrufen zu ermitteln, so kann man dazu die Struktur ABenchmark verwenden:

function myTestFunction takes nothing returns nothing
	debug local ABenchmark benchmark = ABenchmark.create("Benchmark 0")
	debug call benchmark.start()
	// do something here ...
	debug call benchmark.stop()
	debug call benchmark.show()
	debug call benchmark.destroy()
endfunction

Zunächst erzeugen wir eine neue Instanz. Jede Benchmark erhält einen eigenen Namen, der lediglich der Anzeige bzw. Identifikation dienen soll.
Als Nächstes wird der benchmarkinterne Timer gestartet. Verwendet man die JAPI, so werden deren native Funktionen genutzt, welche wesentlich
genauer messen können sollten.
Danach kann der zu messende Code ausgeführt werden. Zu guter Letzt wird der Timer wieder gestoppt und die Dauer auf dem Bildschirm mittels der Methode show angezeigt.

Die statische Methode showBenchmarks zeigt alle vorhandenen Benchmarks an.

Seit der Einführung von Hooks in vJass, kann die ABenchmark-Struktur noch einiges mehr. So werden möglichst alle Handle-erzeugenden native Funktionen abgefangen und das erzeugte Handle in einer Liste gespeichert, falls die globale Konstante A_DEBUG_HANDLES den Wert true enthält. Sobald das Handle gelöscht wird, wird es wieder aus der Liste entfernt. So ist es dem Entwickler möglich, alle vorhandenen Handle-Objekte auf der Karte zu jedem beliebigen Zeitpunkt abzufragen.
Momentan existieren folgende statische Methoden: showUnits, showItems und showDestructables.
