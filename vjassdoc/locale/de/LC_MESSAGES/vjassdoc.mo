��    �        �         �
      �
  L   �
  T   ?  {   �  5     d   F  <   �  K   �  s   4  U   �  F   �  1   E  H   w  J   �  q     X   }  G   �          7     S     l     �     �     �     �     �     �     �     �  	     �        �  ,     T   8     �     �     �     �  
   �  C   �  6   &     ]  %   u     �  .   �     �  	   �     �  	        !  	   5     ?     Y     a     r     �     �     �  
   �     �  	   �     �     �     �     �  R   �  (   L     u     �     �     �     �      �     �  ,   �  
      3   +     _     g     n     �     �     �  #   �     �     �     �     �     �  .        ;     B     J     O  
   T     _     t     �     �     �     �     �     �     �     �          '     <  Y   B  8   �  -   �  )     )   -  E   W  E   �  K   �  #   /  O   S     �     �     �     �     �     �     �     �     �  4  �  '   .  Q   V  P   �  �   �  <   �  g   �  A   3  Q   u  �   �  h   S   P   �   0   !  K   >!  V   �!  �   �!  ]   l"  Q   �"     #     4#     P#     p#     �#     �#     �#     �#     �#  
   �#     �#     $  
   $  
  $     %%  /   9%  R   i%  !   �%     �%  	   �%     �%  
   &  H   &  6   `&      �&  .   �&     �&  *    '     +'     8'      D'     e'     q'  
   �'     �'     �'     �'     �'     �'     �'     �'     
(     (     +(  
   8(     C(     V(     _(     h(  &   �(     �(     �(     �(     �(     �(  "   )     $)  :   +)  	   f)  ?   p)     �)     �)     �)     �)     �)     *  '   *     D*     Q*     ^*     f*     r*  .   �*     �*  
   �*     �*     �*  	   �*     �*  
   �*     �*     �*      +  )   +  (   F+     o+     �+     �+     �+     �+     �+  Z   �+  E   H,  :   �,  2   �,  2   �,  P   /-  W   �-     �-  +   �-  i   .     �.     �.     �.     �.     �.     �.     �.     �.     �.         b       `       /   <   ?       o           9      &       >   D   B   ~   "       O   q   u   v                             6   ,   	   2   -      '       p   n   N   a       Q   V   
                 H   !       %           L   4       0       X   =   (   ;      _   m       e   )   [       R       1      }   ^                    8   U   d      3   s   W   C   r         k       g       h   f         A       j   :       S           i   ]                 $   y          .         t         w   T             \       5   c   Y   |             P   {   Z   I      G   �   F           7      E   K   +       #   @       M       *   x       z   l       J    
Report bugs to tamino@cdauth.de --dir <arg>          <arg> has to be replaced by the output directory path.
 --files <args>       <args> has to be replaced by the files which should be parsed.
 --importdirs <args>  <args> has to be replaced by one or more import directories (Used for the //! import macro in vJass).
 --private            Private objects will be parsed.
 --title <arg>        <arg> has to be replaced by the title which is used for the API documentation.
 --version            Shows the current version of vjassdoc.
 -a --alphabetical    All objects will be aranged in an alphabetical order.
 -b --database        Parsed objects will be saved in a SQLite3 database which could be read out by other programs.
 -d --debug           Lines starting with the vJass keyword 'debug' won't be ignored.
 -h --html            Program creates a simple HTML API documentation.
 -j --jass            vJass code will be ignored.
 -m --textmacros      Code between text macro statements will be parsed.
 -p --pages           Program creates an HTML file for each parsed object.
 -s --specialpages    Program creates additional HTML files containing more information about all parsed objects.
 -t --time            Detects the remaining time and shows it at the end of the process.
 -v --verbose         Program shows more information about the process.
 <strong>Author:</strong> <strong>Parameter:</strong> <strong>Return:</strong> <strong>Source:</strong> <strong>State:</strong> <strong>Todo:</strong> Canceled database creation. Child structs Code Comments Constant Constructor Container Copyright © 2008, 2009 Tamino Dauth
License GPLv2+: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law. Creating HTML file. Creating database.
Using SQLite version %s.
 Database "%s" does already exist. Do you want to replace it by the newer one? (y/n)
 Debug statement, ignoring line. Default return value Delegate Description Destructor Detected " character. Do not ignore // and /// expressions anymore. Detected " character. Ignoring // and /// expressions. Detected empty comment. Detected empty documentation comment. Documentation comments Duration:
%f seconds
CPU duration:
%f seconds
 Empty line. Extension Finished (%d lines, %d files).
 Full path Function interfaces Functions Generated by vjassdoc %s. Globals Import file %s.
 Inherited members Inherited methods Inherited type Initializer Interfaces Keywords Libraries Library Load database %s. Members Methods Missing arguments.
Use --help to get some information about all available options. Missing default return value expression. Missing directory argument. Missing file arguments. Missing title argument. Native No Not required space. Reason: %d.
 Object Objects of list %d already won't be parsed.
 Parameters Parsed file %s successfully (number %d, %d lines).
 Private Public Read line %d of file %s.
 Remove database %d. Return to start page Return type SQL statement of table %s is NULL.
 Scope Scopes Size Source file Source files Starting with not required space. Reason: %d.
 Static Structs Stub Text Text macro Text macro instances Text macros Type Types UNIX mode was detected. Unable to import file %s.
 Unable to open file %s.
 Undocumented objects Unknown answer. Unknown argument: %s.
 Unknown expression. Unknown option: %c.
 Value Warning: Although there won't be created any HTML files you have used the --pages option. Was unable to begin transaction.
State: %d.
Message: %s
 Was unable to commit.
State: %d.
Message: %s
 Was unable to create database.
State %d.
 Was unable to create database. State %d.
 Was unable to create table "%s" from list %d.
State: %d.
Message: %s
 Was unable to finalize prepared SQL statement of table %s.
State %d.
 Was unable to insert id of list %d into table "%s".
State: %d.
Message: %s
 Was unable to replace old database. Was unable to run execution of table "%s" from list %d.
State: %d.
Message: %s
 Win32 mode was detected. Yes en n no vJass API Documentation vjassdoc options:
 y yes Project-Id-Version: vjassdoc 0.2.3
Report-Msgid-Bugs-To: 
POT-Creation-Date: 2009-03-09 17:42+0100
PO-Revision-Date: 2009-03-09 17:54+0100
Last-Translator: Tamino Dauth <tamino.dauth@web.de>
Language-Team: Tamino Dauth <tamino@cdauth.de>
MIME-Version: 1.0
Content-Type: text/plain; charset=UTF-8
Content-Transfer-Encoding: 8bit
X-Poedit-Language: German
X-Poedit-SourceCharset: utf-8
X-Poedit-Basepath: /home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src
X-Poedit-SearchPath-0: /home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src
 
Melden Sie Fehler an tamino@cdauth.de. --dir <arg>          <arg> muss durch den Ausgabeverzeichnispfad ersetzt werden.
 --files <args>       <args> muss durch die zu parsenden Dateien ersetzt werden.
 --importdirs <args>  <args> muss durch eines oder mehrere Import-Verzeichnisse ersetzt werden (wird für das //!-Import-Makro aus vJass verwendet).
 --private            Private Objekte werden geparst werden.
 --title <arg>        <arg> muss durch den für die API-Dokumentation verwendeten Titel ersetzt werden.
 --version            Zeigt die laufende Version von vjassdoc an.
 -a --alphabetical    Alle Objekte werden in alphabetischer Reihenfolge sortiert.
 -b --database        Geparste Objekte werden in einer SQLite3-Datenbank gespeichert, welche von anderen Programmen ausgelesen werden kann.
 -d --debug           Zeilen, die mit dem vJass-Schlüsselwort "debug" beginnen, werden nicht ignoriert.
 -h --html            Das Programm erzeugt eine einfache HTML-API-Dokumentation.
 -j --jass            vJass-Code wird ignoriert.
 -m --textmacros      Der Code zwischen Textmakro-Anweisungen wird geparst.
 -p --pages           Das Programm erzeugt für jedes geparste Objekt eine HTML-Datei.
 -s --specialpages    Das Programm erzeugt zusätzliche HTML-Dateien, welche weitere Informationen über alle geparsten Objekte enthalten.
 -t --time            Ermittelt die vergangene Zeit und zeigt diese am Ende des Prozesses an.
 -v --verbose         Das Programm zeigt mehr Informationen über den Prozess an.
 <strong>Autor:</strong> <strong>Parameter:</strong> <strong>Rückgabewert:</strong> <strong>Quelle:</strong> <strong>Status:</strong> <strong>Zu tun:</strong> Datenbankerzeugung abgebrochen. Kindstrukturen Code Kommentare Konstant Konstruktor Behältnis Copyright © 2008, 2009 Tamino Dauth
Lizenz GPLv2+: GNU GPL Version 2 oder spätere <http://gnu.org/licenses/gpl.html>
Dies ist freie Software: Sie dürfen sie verändern und neu verteilen.
Es gibt vom Gesetz KEINERLEI GEWÄHRLEISTUNG für die zulässige Ausdehnung. Erzeuge HTML-Datei. Erzeuge Datenbank.
Verwende SQLite-Version %s.
 Datenbank "%s" existiert bereits. Soll sie durch die neuere ersetzt werden? (j/n)
 Debug-Anweisung, ignoriere Zeile. Standard-Rückgabewert Delegiert Beschreibung Destruktor "-Zeichen ermittelt. //- und ///-Ausdrücke werden nicht mehr ignoriert. "-Zeichen ermittelt. Ignoriere //- und ///-Ausdrücke. Leerer Kommentar wurde gefunden. Leerer Dokumentationskommentar wurde gefunden. Dokumentationskommentare Dauer:
%f Sekunden
CPU-Dauer:
%f Sekunden
 Leere Zeile. Erweiterung Fertig (%d Zeilen, %d Dateien).
 Ganzer Pfad Funktionsschnittstellen Funktionen Von vjassdoc %s erzeugt. Globale Importiere Datei %s.
 Geerbter Elemente Geerbte Methoden Geerbter Typ Initialisierer Schnittstellen Schlüsselwörter Bibliotheken Bibliothek Lade Datenbank %s. Elemente Methoden Win32-Modus wurde ermittelt. Standard-Rückgabewert-Ausdruck fehlt. Verzeichnis-Argument fehlt. Datei-Argumente fehlen. Titel-Argument fehlt. Nativ Nein Nicht benötigter Raum. Grund %d.
 Objekt Objekte der Liste %d werden bereits nicht geparst werden.
 Parameter Datei %s wurde erfolgreich durchlaufen (Nummer %d, %d Zeilen).
 Privat Öffentlich Lese Zeile %d von Datei %s.
 Entferne Datenbank %d. Zurück zur Startseite Rückgabe-Typ SQL-Anweisung der Tabelle %s ist NULL.
 Bezugsrahmen Bezugsrahmen Größe Quell-Datei Quell-Dateien Beginne mit nicht benötigtem Raum. Grund %d.
 Statisch Strukturen Stub Text Textmakro Textmakro-Instanzen Textmakros Typ Typen UNIX-Modus wurde ermittelt. Datei %s konnte nicht importiert werden.
 Datei %s konnte nicht geöffnet werden.
 Nicht dokumentierte Objekte Unbekannte Antwort. Unbekanntes Argument: %s.
 Unbekannter Ausdruck. Unbekannte Option: %c.
 Wert Warnung: Obwohl keine HTML-Dateien erzeugt werden, haben Sie die --pages-Option verwendet. Transaktion konnte nicht gestartet werden.
Status: %d.
Nachricht: %s
 Konnte nicht übergeben werden.
Status: %d.
Nachricht: %s
 Datenbank konnte nicht erzeugt werden.
Status %d.
 Datenbank konnte nicht erzeugt werden. Status %d.
 Tabelle "%s" der Liste %d konnte nicht angelegt werden.
Status %d.
Nachricht %s
 Vorbereitete SQL-Anweisung der Tabelle %s konnte nicht abgearbeitet werden.
Status %d.
 Status: %d.
Nachricht: %s
 Alte Datenbank konnte nicht ersetzt werden. Eine Ausführung der Tablle "%s" der Liste %d konnte nicht ausgeführt werden.
Status: %d.
Nachricht: %s
 Win32-Modus wurde ermittelt. Ja de n nein vJass-API-Dokumentation vjassdoc-Optionen:
 j ja 