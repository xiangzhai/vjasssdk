library ALibraryCoreStringConversion requires ALibraryCoreStringMisc

	/// Converts an ASCII char to an integer.
	/// ASCII is the American Keyboard Standard.
	/// Conversion of a char to ASCII works with the integer type: 'a' = 97.
	/// @author Peppar
	/// @source http://www.wc3jass.com/
	/// @todo Syntax error?!
	function AsciiToChar takes integer i returns string
		if (i == 0) then
        		return null
		elseif ((i >= 8) and (i <= 10)) then
			return SubString("\b\t\n", i - 8, i - 7)
		elseif ((i >= 12) and (i <= 13)) then
			return SubString("\f\r", i - 12, i - 11)
		elseif ((i >= 32) and (i <= 127)) then
			return SubString(" !\"#$%%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~", i - 32, i - 31) //syntax error?
		endif
		return ""
	endfunction

	/// Converts a specific number of seconds to a string with minutes.
	/// Example: 120 seconds - 02:00.
	function GetTimeString takes integer seconds returns string
		local integer i
		local integer endMinutes = 0
		local integer endSeconds = 0
		local string timeString
		if (seconds >= 60) then
			set i = 1
			loop
				exitwhen (seconds < (i * 60))
				set endMinutes = endMinutes + 1
				set i = i + 1
			endloop
		endif
		set endSeconds = seconds - endMinutes * 60
		if (endMinutes >= 10) then
			set timeString = I2S(endMinutes)
		else
			set timeString = "0" + I2S(endMinutes)
		endif
		set timeString = timeString + ":"
		if (endSeconds >= 10) then
			set timeString = timeString + I2S(endSeconds)
		else
			set timeString = timeString + "0" + I2S(endSeconds)
		endif
		return timeString
	endfunction

	/// Converts string @param colorString to a player color.
	function StringToPlayerColor takes string colorString returns playercolor
		if (colorString == "ff0000") then
			return PLAYER_COLOR_RED
		elseif (colorString == "0000ff") then
			return PLAYER_COLOR_BLUE
		elseif (colorString == "") then //Fehlt noch
			return PLAYER_COLOR_CYAN
		elseif (colorString == "800080") then
			return PLAYER_COLOR_PURPLE
		elseif (colorString == "ffff00") then
			return PLAYER_COLOR_YELLOW
		elseif (colorString == "ff8000") then
			return PLAYER_COLOR_ORANGE
		elseif (colorString == "00ff00") then
			return PLAYER_COLOR_GREEN
		elseif (colorString == "ff80C0") then
			return PLAYER_COLOR_PINK
		elseif (colorString == "C0C0C0") then
			return PLAYER_COLOR_LIGHT_GRAY
		elseif (colorString == "0080ff") then
			return PLAYER_COLOR_LIGHT_BLUE
		elseif (colorString == "") then //Fehlt noch
			return PLAYER_COLOR_AQUA
		elseif (colorString == "804000") then
			return PLAYER_COLOR_BROWN
		endif
		return null
	endfunction

	/// Converts player color @param playerColor to a string.
	function PlayerColorToString takes playercolor playerColor returns string
		if (playerColor == PLAYER_COLOR_RED) then
			return "ff0000"
		elseif (playerColor == PLAYER_COLOR_BLUE) then
			return "0000ff"
		elseif (playerColor == PLAYER_COLOR_CYAN) then /// @todo missing
		elseif (playerColor == PLAYER_COLOR_PURPLE) then
			return "800080"
		elseif (playerColor == PLAYER_COLOR_YELLOW) then
			return "ffff00"
		elseif (playerColor == PLAYER_COLOR_ORANGE) then
			return "ff8000"
		elseif (playerColor == PLAYER_COLOR_GREEN) then
			return "00ff00"
		elseif (playerColor == PLAYER_COLOR_PINK) then
			return "ff80c0"
		elseif (playerColor == PLAYER_COLOR_LIGHT_GRAY) then
			return "c0c0c0"
		elseif (playerColor == PLAYER_COLOR_LIGHT_BLUE) then
			return "0080ff"
		elseif (playerColor == PLAYER_COLOR_AQUA) then /// @todo missing
		elseif (playerColor == PLAYER_COLOR_BROWN) then
			return "804000"
		endif
		return "ffffff"
	endfunction

	/// I changed the function and called my own string functions.
	/// @author Peppar
	/// @source http://www.wc3jass.com/
	function HighlightShortcut takes string usedString, integer shortcut, string colour returns string
		local string result
		local string newShortcut = AsciiToChar(shortcut)
		local integer shortcutPosition = FindString(usedString, newShortcut)
		if (shortcutPosition != -1) then
			if (colour == null) then
				set colour = "ffffcc00"
			endif
			set result = InsertString(usedString, shortcutPosition + 1, "|r")
			set result = InsertString(usedString, shortcutPosition, ("|c" + colour))
			return result
		endif
		return usedString
	endfunction

	//DOES WORK YEAAHHHH, DO NOT CHANGE
	//Hinter jedem Argument muss ein Leerzeichen sein.
	//Auch hinter dem letzten.
	//Diese Funktion dient der Mehrsprachenfhigkeit, ist aber nicht ganz so umfangreich wie in C (printf).
	//Hier gibt es keine Elipse und keine Typenerkennung, dafr ist die Funktion relativ schnell.
	//call Format("Herzlich Willkommen lieber %. Es ist %:% Uhr", "Barade " + I2S(10) + " " + I2S(30) + " ")
	//call DisplayTimedTextToPlayer(GetTriggerPlayer(), 0.0, 0.0, 100.0, Format("Herzlich Willkommen lieber %. Es ist %:% Uhr", "Barade " + I2S(10) + " " + I2S(30) + " "))
	//function Format takes string usedString, string arguments returns string
		//local integer charPosition
		//local integer argumentEnd
		//local string argument
		//local string result = usedString
		//loop
			//set charPosition = FindString(result, "%")
			//exitwhen (charPosition == -1)
			//set argumentEnd = FindString(arguments, " ")
			//set argument = SubString(arguments, 0, argumentEnd)
			//set arguments = SubString(arguments, (argumentEnd + 1), StringLength(arguments)) //Der Parameter muss gekrzt werden.
			//set result = RemoveSubString(result, charPosition, 1)
			//set result = InsertString(result, charPosition, argument)
			//set usedString = SubString(usedString, charPosition + 1, StringLength(usedString)) //Der benutzte String muss gekrzt werden
		//endloop
		//return result
	//endfunction

	//This function uses a string from the wts string file of the map.
	//Use it if you want translate your map in a language and you need arguments.
	//function FormatLocalized takes string usedString, string arguments returns string
		//return GetLocalizedString(Format(usedString, arguments))
	//endfunction

	/// New argument function.
	/// Is much faster because you don't have to filter all the arguments and don't have to convert the types by yourself.
	/// Usage example: StringArg(StringArg(IntegerArg(Text("You're %i years old and you're called %s. Besides you're %s."), 0), "Peter"), "gay")
	//! textmacro AStringArgumentMacro takes TYPE, TYPENAME, TYPECHAR, CONVERSIONFUNCTION
		function $TYPENAME$Arg takes string usedString, $TYPE$ value returns string
			local string result = usedString
			local integer charPosition = FindString(result, "%$TYPECHAR$")
			if (charPosition != -1) then
				set result = RemoveSubString(result, charPosition, 2)
				set result = InsertString(result, charPosition, $CONVERSIONFUNCTION$(value))
			endif
			return result
		endfunction
	//! endtextmacro

	//! runtextmacro AStringArgumentMacro("integer", "Integer", "i", "I2S")
	//! runtextmacro AStringArgumentMacro("real", "Real", "r", "R2S")
	//! runtextmacro AStringArgumentMacro("string", "String", "s", "")

	/// @author Extrarius
	/// @source http://www.wc3jass.com/
	function GetExternalString takes integer index returns string
		if (index < 0) then
			return ""
		elseif (index < 10) then
			return GetLocalizedString("TRIGSTR_00" + I2S(index))
		elseif (index < 100) then
			return GetLocalizedString("TRIGSTR_0" + I2S(index))
		else
			return GetLocalizedString("TRIGSTR_" + I2S(index))
		endif
	endfunction

	/// It is possible to search in the string files of the mpq directory "ui/FrameDef".
	/// There are some general string files.
	/// tr means translation.
	/// native GetLocalizedString takes string source returns string
	function tr takes string source returns string
		return GetLocalizedString(source)
	endfunction

	/// native GetLocalizedHotkey takes string source returns integer
	/// sc for shortcut.
	function sc takes string source returns integer
		return GetLocalizedHotkey(source)
	endfunction

	/// Not checked yet.
	/// @todo Bugged!
	function InsertLineBreaks takes string usedString, integer maxLineLength returns string
		local integer i
		local string result
		if (StringLength(usedString) <= maxLineLength) then
			return usedString
		endif
		set i = maxLineLength
		loop
			exitwhen (i >= StringLength(usedString))
				set result = result + SubString(usedString, i, maxLineLength) + "|n"
			set i = i + maxLineLength
		endloop
		
		set i = i - maxLineLength
		if (i > StringLength(usedString)) then
			set result = result + SubString(usedString, i, StringLength(usedString))
		endif
		
		return result
	endfunction

endlibrary