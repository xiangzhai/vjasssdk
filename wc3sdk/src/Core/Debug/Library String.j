/// Test of all core string functions.
/// This is the last test I made with the core.
/// @todo String functions weren't checked at all.
/// @todo Test is not completed yet.
library ALibraryCoreDebugString requires ALibraryCoreDebugMisc, ACoreString

	private function GetStringConversionDebug takes nothing returns nothing
		local string PrintString = ""
		//function GetTimeString takes integer Seconds returns string - Works
		set PrintString = ("GetTimeString - " + GetTimeString(120))
		debug call Print(PrintString)
		//function StringToPlayerColour takes string String returns playercolor - Works
		if (StringToPlayerColor("ff0000") == PLAYER_COLOR_RED) then
			set PrintString = ("StringToPlayerColor - Works.")
		else
			set PrintString = ("StringToPlayerColor - Doesn't work.")
		endif
		debug call Print(PrintString)
		//function PlayerColourToString takes playercolor PlayerColour returns string - Works
		set PrintString = ("PlayerColorToString - " + PlayerColorToString(PLAYER_COLOR_BLUE) + "Blue text")
		debug call Print(PrintString)
	endfunction

	private function GetStringPoolFunctionsDebug takes nothing returns nothing
		local string PrintString = ""
		//function GetRandomChar takes string CharPool returns string - Works
		set PrintString = GetRandomChar("abc", 100.00) //if the chance is 100.00 it will be always upper case
		debug call Print(PrintString)
		//function GetRandomAlphabeticChar takes real RequestedChance returns string - Works
		set PrintString = GetRandomAlphabeticalChar(100.00) //if the chance is 100.00 it will be always upper case
		debug call Print(PrintString)
		//function GetRandomNumeralChar takes nothing returns string - Works
		set PrintString = GetRandomNumeralChar()
		debug call Print(PrintString)
		//function GetRandomPeculiarChar takes nothing returns string - Works
		set PrintString = GetRandomSpecialChar()
		debug call Print(PrintString)
		//function GetRandomString takes integer Length, real CaseSensitivityChance, boolean IncludingNumbers, boolean IncludingPeculiars returns string - Check it
		set PrintString = GetRandomString(23, 100.00, true, true, true) //if the chance is 100.00 it will be always upper case
		debug call Print(PrintString)
		//function IsStringFromStringPool takes string String, string StringPool returns boolean - Works
		if (not IsStringFromStringPool("Hallo", "1111")) then
			set PrintString = "IsStringFromStringPool - Works"
		else
			set PrintString = "IsStringFromStringPool - Doesn't work."
		endif
		debug call Print(PrintString)
		//function IsStringAlphabetic takes string String returns boolean
		if (IsStringAlphabetical("hallo")) then
			set PrintString = "IsStringAlphabetical - Works"
		else
			set PrintString = "IsStringAlphabetical - Doesn't work."
		endif
		debug call Print(PrintString)
		//function IsStringNumeral takes string String returns boolean
		if (IsStringNumeral("1243")) then
			set PrintString = "IsStringNumeral - Works"
		else
			set PrintString = "IsStringNumeral - Doesn't work."
		endif
		debug call Print(PrintString)
		//function IsStringPeculiar takes string String returns boolean
		if (IsStringSpecialChar("!()")) then
			set PrintString = "IsStringSpecialCharacter - Works"
		else
			set PrintString = "IsStringSpecialCharacter - Doesn't work."
		endif
		debug call Print(PrintString)
		//function IsStringInteger takes string String returns boolean
		if (IsStringInteger("-345")) then
			set PrintString = "IsStringInteger - Works"
		else
			set PrintString = "IsStringInteger - Doesn't work."
		endif
		debug call Print(PrintString)
	
		//- 0% binär
		//- 0 oktal
		//- 0x hexadezimal
	
		//function IsStringBinary takes string String returns boolean
		if (IsStringBinary("0%11011")) then
			set PrintString = "IsStringBinary - Works"
		else
			set PrintString = "IsStringBinary - Doesn't work."
		endif
		debug call Print(PrintString)
		//function IsStringOctal takes string String returns boolean
		if (IsStringOctal("0243")) then
			set PrintString = "IsStringOctal - Works"
		else
			set PrintString = "IsStringOctal - Doesn't work."
		endif
		debug call Print(PrintString)
		//function IsStringHexadecimal takes string String returns boolean
		if (IsStringHexadecimal("0xFF29A")) then
			set PrintString = "IsStringHexadecimal - Works"
		else
			set PrintString = "IsStringHexadecimal - Doesn't work."
		endif
		debug call Print(PrintString)
	endfunction

	function GetStringMiscFunctionsDebug takes nothing returns nothing
	endfunction

	function GetStringDebug takes nothing returns nothing
		call GetStringConversionDebug()
		call GetStringPoolFunctionsDebug()
		call GetStringMiscFunctionsDebug()
	endfunction

endlibrary