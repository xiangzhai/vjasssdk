library ALibraryCoreStringConversion requires ALibraryCoreStringMisc

	/**
	* Converts an ASCII char to an integer.
	* ASCII is the American Keyboard Standard.
	* Conversion of a char to ASCII works with the integer type: 'a' = 97.
	* @author Peppar
	* @link http://www.wc3jass.com/
	* @todo Syntax error?!
	*/
	function AsciiToChar takes integer i returns string
		if (i == 0) then
        		return null
		elseif ((i >= 8) and (i <= 10)) then
			return SubString("\b\t\n", i - 8, i - 7)
		elseif ((i >= 12) and (i <= 13)) then
			return SubString("\f\r", i - 12, i - 11)
		elseif ((i >= 32) and (i <= 127)) then
			return SubString(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~", i - 32, i - 31) // syntax error?
		endif
		return ""
	endfunction

	/// Converts string @param colorString to a player color.
	function StringToPlayerColor takes string colorString returns playercolor
		if (colorString == "ff0000") then
			return PLAYER_COLOR_RED
		elseif (colorString == "0000ff") then
			return PLAYER_COLOR_BLUE
		elseif (colorString == "1CB619") then
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
		elseif (colorString == "106246") then
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
		elseif (playerColor == PLAYER_COLOR_CYAN) then
			return "1CB619"
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
		elseif (playerColor == PLAYER_COLOR_AQUA) then
			return "106246"
		elseif (playerColor == PLAYER_COLOR_BROWN) then
			return "804000"
		endif
		return "ffffff"
	endfunction

	/**
	* I changed the function and called my own string functions.
	* @param colour If this value is null function will use "ffffcc00".
	* @author Peppar
	* @link http://www.wc3jass.com/
	*/
	function HighlightShortcut takes string whichString, integer shortcut, string colour returns string
		local string newShortcut = AsciiToChar(shortcut)
		local integer index = FindString(whichString, newShortcut)
		if (index != -1) then
			if (colour == null) then
				set colour = "ffffcc00"
			endif
			return SubString(whichString, 0, index) + "|c" + colour + newShortcut + "|r" + SubString(whichString, index + 1, StringLength(whichString))
		endif
		return whichString
	endfunction

	/**
	* @author Extrarius
	* @link http://www.wc3jass.com/
	*/
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

	/**
	* Same function as GetLocalizedString with shorted identifier.
	* @return Returns translated string from map string file ("war3map.wts") or game string files ("UI/FrameDef/.fdf").
	* @note If string won't be found it returns value of parameter source.
	* Usually World Editor's trigger editor replaces all function usages automatically by string source identifiers and adds strings to "war3map.wts" file.
	* @note Shortcuts and tooltips can be customized in file "CustomKeys.txt" in Warcraft 3 installation directory.
	* If you use source code you'll have to use tools like vjasstrans to generate that file.
	* @see GetLocalizedString, trp, sc, AFormat
	*/
	function tr takes string source returns string
		return GetLocalizedString(source)
	endfunction

	/**
	* Useful for expressions which have to be different when value unequal to one is used (plural).
	* @return Returns translated string from source singularSource if count is 1, otherwise it returns translated string from source pluralSource.
	* This is a very simple example of the function's usage with an English-language expression:
	* @code
	* local integer eggs = GetRandomInt(0, 100)
	* call Print(Format(trp("Find %1% egg", "Find %1% eggs", eggs)).i(eggs).result())
	* @endcode
	* @see GetLocalizedString, tr, sc, AFormat
	*/
	function trp takes string singularSource, string pluralSource, integer count returns string
		if (count == 1) then
			return tr(singularSource)
		endif
		return tr(pluralSource)
	endfunction

	/**
	* Same function as GetLocalizedHotkey with shorted identifier.
	* @return Returns localized shortcut.
	* @note Shortcuts and tooltips can be customized in file "CustomKeys.txt" in Warcraft 3 installation directory.
	* @see GetLocalizedString, trp, sc, AFormat
	*/
	function sc takes string source returns integer
		return GetLocalizedHotkey(source)
	endfunction

	/**
	* Inserts line break escape sequence characters into string whichString by separating it into sub strings of length maxLineLength.
	* Useful for displaying text paragraphs (such as dialog messages).
	* @return Returns whichString separated into sub strings of length maxLineLength ending with '|n' character.
	* @todo bugged?
	*/
	function InsertLineBreaks takes string whichString, integer maxLineLength returns string
		local integer i
		local string result = ""
		if (StringLength(whichString) <= maxLineLength) then
			return whichString
		endif
		set i = maxLineLength
		loop
			exitwhen (i > StringLength(whichString))
			set result = result + SubString(whichString, i - maxLineLength, i) + "|n"
			set i = i + maxLineLength
		endloop

		if (i < StringLength(whichString)) then
			set result = result + SubString(whichString, i - maxLineLength, StringLength(whichString))
		endif

		return result
	endfunction

	/**
	* Converts specific number of seconds into a string with minutes and hours.
	* Examples:
	* 120 seconds - 02:00
	* 7200 seconds - 02:00:00
	*/
	function GetTimeString takes integer seconds returns string
		local integer minutes = seconds / 60
		local integer hours = minutes / 60
		local string secondsString
		local string minutesString
		local string hoursString
		set seconds = ModuloInteger(seconds, 60)
		set minutes = ModuloInteger(minutes, 60)
		if (seconds >= 10) then
			set secondsString = I2S(seconds)
		else
			set secondsString = IntegerArg.evaluate(tr("0%i"), seconds)
		endif
		if (minutes >= 10) then
			set minutesString = I2S(minutes)
		else
			set minutesString = IntegerArg.evaluate(tr("0%i"), minutes)
		endif
		if (hours >= 10) then
			set hoursString = I2S(hours)
		else
			set hoursString = IntegerArg.evaluate(tr("0%i"), hours)
		endif
		return StringArg.evaluate(StringArg.evaluate(StringArg.evaluate(tr("%s:%s:%s"), hoursString), minutesString), secondsString)
	endfunction

	/**
	* Old argument function text macro for internationalisation of your code.
	* Usage of structure AFormat is highly recommended since it allows you to use argument numbers instead of types.
	* Allows you to create string-formatting function for any type.
	* Usage example:
	* @code
	* StringArg(StringArg(IntegerArg("You're %i years old and you're called %s. Besides you're %s.", 0), "Peter"), "gay")
	* @endcode
	* Compared to AFormat usage:
	* @code
	* Format("You're %1% years old and you're called %2%. Besides you're %3%.").i(0).s("Peter").("gay").result()
	* @endcode
	* @see AFormat, Format, String
	*/
	//! textmacro AStringArgumentMacro takes NAME, TYPE, TYPECHARS, CONVERSION, PARAMETERS
		function $NAME$ takes string whichString, $TYPE$ value $PARAMETERS$ returns string
			local integer index = FindString(whichString, "%$TYPECHARS$")
			if (index != -1) then
				set whichString = SubString(whichString, 0, index) + $CONVERSION$ + SubString(whichString, index + StringLength("%$TYPECHARS$"), StringLength(whichString))
			debug else
				debug call PrintFunctionError("$NAME$", "Missing member of type $TYPE$ in string \"" + whichString + "\".")
			endif
			return whichString
		endfunction
	//! endtextmacro

	//! runtextmacro AStringArgumentMacro("IArg", "integer", "i", "I2S(value)", "")
	//! runtextmacro AStringArgumentMacro("IntegerArg", "integer", "i", "I2S(value)", "")
	//! runtextmacro AStringArgumentMacro("RArg", "real", "r", "R2S(value)", "")
	//! runtextmacro AStringArgumentMacro("RealArg", "real", "r", "R2S(value)", "")
	/// @param width Width of argument string in characters (if it is too short there will be inserted space characters).
	//! runtextmacro AStringArgumentMacro("RWArg", "real", "r", "R2SW(value, width, precision)", ", integer width, integer precision")
	/// @param width Width of argument string in characters (if it is too short there will be inserted space characters).
	//! runtextmacro AStringArgumentMacro("RealWidthArg", "real", "r", "R2SW(value, width, precision)", ", integer width, integer precision")
	//! runtextmacro AStringArgumentMacro("SArg", "string", "s", "value", "")
	//! runtextmacro AStringArgumentMacro("StringArg", "string", "s", "value", "")
	//! runtextmacro AStringArgumentMacro("HArg", "handle", "h", "I2S(GetHandleId(value))", "")
	//! runtextmacro AStringArgumentMacro("HandleArg", "handle", "h", "I2S(GetHandleId(value))", "")
	//! runtextmacro AStringArgumentMacro("UArg", "unit", "u", "GetUnitName(value)", "")
	//! runtextmacro AStringArgumentMacro("UnitArg", "unit", "u", "GetUnitName(value)", "")
	//! runtextmacro AStringArgumentMacro("ItArg", "item", "it", "GetItemName(value)", "")
	//! runtextmacro AStringArgumentMacro("ItemArg", "item", "it", "GetItemName(value)", "")
	//! runtextmacro AStringArgumentMacro("DArg", "destructable", "d", "GetDestructableName(value)", "")
	//! runtextmacro AStringArgumentMacro("DestructableArg", "destructable", "d", "GetDestructableName(value)", "")
	//! runtextmacro AStringArgumentMacro("PArg", "player", "p", "GetPlayerName(value)", "")
	//! runtextmacro AStringArgumentMacro("PlayerArg", "player", "p", "GetPlayerName(value)", "")
	//! runtextmacro AStringArgumentMacro("HeArg", "unit", "he", "GetHeroProperName(value)", "")
	//! runtextmacro AStringArgumentMacro("HeroArg", "unit", "he", "GetHeroProperName(value)", "")
	//! runtextmacro AStringArgumentMacro("OArg", "integer", "o", "GetObjectName(value)", "")
	//! runtextmacro AStringArgumentMacro("ObjectArg", "integer", "o", "GetObjectName(value)", "")
	//! runtextmacro AStringArgumentMacro("LArg", "string", "l", "GetLocalizedString(value)", "")
	//! runtextmacro AStringArgumentMacro("LocalizedStringArg", "string", "l", "GetLocalizedString(value)", "")
	//! runtextmacro AStringArgumentMacro("KArg", "string", "k", "I2S(GetLocalizedHotkey(value))", "")
	//! runtextmacro AStringArgumentMacro("LocalizedHotkeyArg", "string", "k", "I2S(GetLocalizedHotkey(value))", "")
	//! runtextmacro AStringArgumentMacro("EArg", "integer", "e", "GetExternalString(value)", "")
	//! runtextmacro AStringArgumentMacro("ExternalStringArg", "integer", "e", "GetExternalString(value)", "")
	/// Use seconds as parameter!
	//! runtextmacro AStringArgumentMacro("TArg", "integer", "t", "GetTimeString(value)", "")
	/// Use seconds as parameter!
	//! runtextmacro AStringArgumentMacro("TimeArg", "integer", "t", "GetTimeString(value)", "")

endlibrary