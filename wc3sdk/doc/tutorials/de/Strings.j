Der String (deutsch Zeichenkette) ist ein Datentyp, der es dem Entwickler ermöglicht, Texte bzw. mehrere Zeichen hintereinander in einer Variable zu hinterlegen.
Leider existieren nicht besonders viele native Funktionen zur String-Behandlung.
Aus diesem Grund enthält die ASL die Bibliothek ACoreString. Diese ist wiederum in mehrere Unterbibliotheken aufgeteilt, welche Funktionen und Strukturen zur erweiterten String-Behandlung enthalten.
Möchte man einen String in einem anderen suchen, so kann man dazu die Funktion FindString nutzen. ReplaceSubString ersetzt einen Teilstring eines anderen Strings, indem man die Position und den ersetzenden String angibt.
ReplaceString tut das Gleiche, allerdings wird der zu ersetzende String und nicht dessen Position angegeben.

