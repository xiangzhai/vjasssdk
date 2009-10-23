Advanced Script Library
Anleitung 1: Der Charakter
Die ASL enthält ein sogenanntes Charaktersystem. Dabei handelt es sich um eine Sammlung von kleineren Systemen, welche dem Anwender eine einfachere Erstellung eines Rollenspiels ermöglichen. Das System eignet sich nicht ausschließlich für Rollenspielcharakter, ist aber hauptsächlich dafür ausgelegt. In der folgenden Anleitung werden Sie sich mit jenem System auseinandersetzen, um es so effizient für Ihr eigenes Projekt nutzen zu können.

Die Charakter-Struktur
Die Basis des gesamten Systems bildet die Charakterstruktur ACharacter, deren Instanzen für jeweils einen Spielercharakter stehen. Die Struktur ist so ausgelegt, dass jeder menschliche Spieler genau einen Charakter besitzen kann.
Da das System mehrspielerfähig ist, existieren mehrere statische Methoden, die die Verwaltung von mehreren Charakteren gleichzeitig stark vereinfachen. So kann man zum Beispiel die Attribute aller Charaktere mit einer einzigen Methode erhöhen oder verringern, oder prüfen, ob sich alle Charaktere in einem Gebiet befinden.
Damit komme ich auch schon zur nächsten Eigenschaft der Struktur. Jede ACharacter-Instanz besitzt eine zugehörige Einheit. Jene Einheit repräsentiert den Charakter im Spielgeschehen und wird bei sämtlichen einheitenbezogenen Methoden verwendet.
Bevor Sie das System allerdings benutzen können, müssen Sie es wie die meisten anderen ASL-Systeme erst einmal initialisieren:
//Referenz: ACharacter.init(boolean removeUnitOnDestruction, boolean destroyOnPlayerLeaves, boolean destroyOnDeath, boolean useViewSystem, boolean useFocusSystem, boolean useMovementSystem, boolean useFightSystem, boolean useRevivalSystem, boolean useInventorySystem)
call ACharacter.init(true, true, true, false, false, false, false, false, false)
Der erste Parameter gibt an, ob die Einheit bei der Zerstörung der Instanz ebenfalls entfernt werden soll. Da man diese in der Regel nur für den Charakter benötigt, setze ich den Wert auf wahr. Der zweite Parameter gibt an, ob die Charakterinstanz zerstört werden soll, sobald der besitzende Spieler das Spiel verlässt.
Der dritte Parameter gibt an, ob die Charakterinstanz zerstört werden soll, wenn die Einheit des Charakters stirbt. Da ich in diesem Beispiel zunächst kein Wiederbelebungssystem verwende, setze ich diesen Wert ebenfalls auf wahr.
Die restlichen Parameter geben an, welche Untersysteme verwendet werden sollen. Einige davon besitzen wiederum eigene Initialisierungsfunktionen, welche bei ihrer Verwendung ebenfalls aufgerufen werden sollten.

 
