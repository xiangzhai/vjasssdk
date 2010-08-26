Advanced Script Library
Anleitung 1: Der Charakter
Die ASL enthält ein sogenanntes Charaktersystem. Dabei handelt es sich um eine Sammlung von kleineren Systemen, welche dem Anwender eine einfachere Erstellung eines Rollenspiels ermöglichen. Das System eignet sich nicht ausschließlich für Rollenspielcharakter, ist aber hauptsächlich dafür ausgelegt. In der folgenden Anleitung werden Sie sich mit jenem System auseinandersetzen, um es so effizient für Ihr eigenes Projekt nutzen zu können.

Die Charakter-Struktur
Die Basis des gesamten Systems bildet die Charakterstruktur ACharacter, deren Instanzen für jeweils einen Spielercharakter stehen. Die Struktur ist so ausgelegt, dass jeder menschliche Spieler genau einen Charakter besitzen kann.
Da das System mehrspielerfähig ist, existieren mehrere statische Methoden, die die Verwaltung von mehreren Charakteren gleichzeitig stark vereinfachen. So kann man zum Beispiel die Attribute aller Charaktere mit einer einzigen Methode erhöhen oder verringern, oder prüfen, ob sich alle Charaktere in einem Gebiet befinden.
Damit komme ich auch schon zur nächsten Eigenschaft der Struktur. Jede ACharacter-Instanz besitzt eine zugehörige Einheit. Jene Einheit repräsentiert den Charakter im Spielgeschehen und wird bei sämtlichen einheitenbezogenen Methoden verwendet.
Bevor Sie das System allerdings benutzen können, müssen Sie es wie die meisten anderen ASL-Systeme erst einmal initialisieren. Rufen Sie dabei die statische Methode ACharacter.init auf.
Falls Sie die Charakterinstanzen nicht selbst erzeugen bzw. eine Klassenauswahl zur Verfügung stellen
wollen, können sie dabei auf die Struktur AClassSelection zurückgreifen.
Diese muss allerdings auch initialisiert werden, sowie sämtliche Charaktersysteme, die sie verwenden.



