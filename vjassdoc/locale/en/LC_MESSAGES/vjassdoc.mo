��    �      $  �   ,      �
      �
  L     T   O  {   �  5      d   V  <   �  K   �  s   D  U   �  F     1   U  H   �  J   �  q     X   �  G   �     .     G     c     |     �     �     �     �     �     �     �       	     �          ,     T   H     �     �     �     �  
   �  C   �  6   6     m  %   �     �  .   �     �  	   �       	   '     1  	   E     O     i     q     �     �     �     �  
   �     �  	   �     �     �     �       R   	  (   \     �     �     �     �     �      �     �  ,     
   0  3   ;     o     w     ~     �     �     �  #   �     �     �     �            .        K     R     Z     _  
   d     o     �     �     �     �     �     �     �     �          #     7     L  Y   R  8   �  -   �  )     )   =  E   g  E   �  K   �  ;   ?  #   {  O   �     �                              ,     ?     A  5  E      {  L   �  T   �  {   >  5   �  d   �  <   U  K   �  s   �  U   R   F   �   1   �   H   !!  J   j!  q   �!  X   '"  G   �"     �"     �"     �"     #     /#     G#     ^#     z#     �#     �#     �#     �#  	   �#  �   �#     �$  ,   �$     �$     �$     %     .%     7%  
   C%  C   N%  6   �%     �%  %   �%     &  .   &     M&  	   Y&     c&  	   �&     �&  	   �&     �&     �&     �&     �&     �&     '     '  
   '     ('  	   1'     ;'     C'     U'     ]'  R   e'  (   �'     �'     �'     (     -(     4(      7(     X(  ,   _(  
   �(  3   �(     �(     �(     �(     �(     )     )  #   ))     M)     S)     Z)     _)     k)  .   x)     �)     �)     �)     �)  
   �)     �)     �)     �)     �)     �)     *     **     C*     X*     h*     *     �*     �*  Y   �*  8   +  -   A+  )   o+  )   �+  E   �+  E   	,  K   O,  ;   �,  #   �,  O   �,     K-     d-     h-     k-     m-     p-     �-     �-     �-         b       `       /   <   ?       o           9      &       >   D   B      "       O   q   u   w                             6   ,   	   2   -   �   '       p   n   N   a       Q   V   
                 H   !       %           L   4       0   {   X   =   (   ;      _   m       e   )   [       R       1      ~   ^                    8   U   d      3   s   W   C   r         k       g       h   f         A       j   :       S           i   ]                 $   z          .         t         v   T             \       5   c   Y   }             P   |   Z   I      G   �   F           7      E   K   +       #   @       M       *   y       x   l       J    
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
 Was unable to prepare SQL statement of table %s.
State %d.
 Was unable to replace old database. Was unable to run execution of table "%s" from list %d.
State: %d.
Message: %s
 Win32 mode was detected. Yes en n no vJass API Documentation vjassdoc options:
 y yes Project-Id-Version: vjassdoc 0.2.3
Report-Msgid-Bugs-To: 
POT-Creation-Date: 2009-03-09 17:42+0100
PO-Revision-Date: 2009-03-09 17:46+0100
Last-Translator: Tamino Dauth <tamino.dauth@web.de>
Language-Team: Tamino Dauth <tamino@cdauth.de>
MIME-Version: 1.0
Content-Type: text/plain; charset=utf-8
Content-Transfer-Encoding: 8bit
X-Poedit-Language: English
X-Poedit-SourceCharset: utf-8
X-Poedit-Basepath: /home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src
X-Poedit-SearchPath-0: /home/tamino/Dokumente/Projekte/Informatik/C++/vjasssdk/vjassdoc/src
 
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
License GPLv2: GNU GPL version 2 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law. Creating HTML file. Creating database.
Using SQLite version %s.
 UNIX mode was detected. Debug statement, ignoring line. Default return value Delegate Description Destructor Detected " character. Do not ignore // and /// expressions anymore. Detected " character. Ignoring // and /// expressions. Detected empty comment. Detected empty documentation comment. Documentation comments Duration:
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
 Was unable to prepare SQL statement of table %s.
State %d.
 Was unable to replace old database. Was unable to run execution of table "%s" from list %d.
State: %d.
Message: %s
 Win32 mode was detected. Yes en n no vJass API Documentation vjassdoc options:
 y yes 