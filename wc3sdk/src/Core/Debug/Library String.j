/// Test of all core string functions.
library ALibraryCoreDebugString requires AStructCoreDebugBenchmark, ALibraryCoreDebugMisc, ACoreString

	private function GetStringConversionDebug takes nothing returns nothing
		debug call Print("GetTimeString - " + GetTimeString(120))
		if (StringToPlayerColor("ff0000") == PLAYER_COLOR_RED) then
			debug call Print("StringToPlayerColor - Works.")
		else
			debug call Print("StringToPlayerColor - Doesn't work.")
		endif
		debug call Print("PlayerColorToString - " + PlayerColorToString(PLAYER_COLOR_BLUE) + "Blue text")
	endfunction

	private function GetStringPoolFunctionsDebug takes string testString, string testParameter0 returns nothing
		debug call Print("GetRandomCharacter - Result is \"" + GetRandomCharacter(testString) + "\".")
		debug call Print("GetRandomAlphabeticalCharacter - Result is \"" + GetRandomAlphabeticalCharacter() + "\".")
		debug call Print("GetRandomNumeralCharacter - Result is \"" + GetRandomNumeralCharacter() + "\".")
		debug call Print("GetRandomSpecialCharacter - Result is \"" + GetRandomSpecialCharacter() + "\".")
		debug call Print("GetRandomString - Result is \"" + GetRandomString(23, true, true, true) + "\".")
		if (IsStringFromCharacterPool(testString, testParameter0)) then
			debug call Print("IsStringFromCharacterPool - Result is true.")
		else
			debug call Print("IsStringFromCharacterPool - Result is false.")
		endif
		if (IsStringAlphabetical(testString)) then
			debug call Print("IsStringAlphabetical - Result is true.")
		else
			debug call Print("IsStringAlphabetical - Result is false.")
		endif
		if (IsStringNumeral(testString)) then
			debug call Print("IsStringNumeral - Result is true.")
		else
			debug call Print("IsStringNumeral - Result is false.")
		endif
		if (IsStringSpecial(testString)) then
			debug call Print("IsStringSpecial - Result is true.")
		else
			debug call Print("IsStringSpecial - Result is false.")
		endif
		if (IsStringInteger(testString)) then
			debug call Print("IsStringInteger - Result is true.")
		else
			debug call Print("IsStringInteger - Result is false.")
		endif
		if (IsStringBinary(testString)) then
			debug call Print("IsStringBinary - Result is true.")
		else
			debug call Print("IsStringBinary - Result is false.")
		endif
		if (IsStringOctal(testString)) then
			debug call Print("IsStringOctal - Result is true.")
		else
			debug call Print("IsStringOctal - Result is false.")
		endif
		if (IsStringHexadecimal(testString)) then
			debug call Print("IsStringHexadecimal - Result is true.")
		else
			debug call Print("IsStringHexadecimal - Result is false.")
		endif
	endfunction

	function GetStringMiscFunctionsDebug takes string testString, string testParameter0, string testParameter1 returns nothing
		if (FindString(testString, testParameter0) != -1) then
			debug call Print("FindString - Found it.")
		else
			debug call Print("FindString - Did not find it.")
		endif
		debug call Print("ReplaceSubString - Result is \"" + ReplaceSubString(testString, 0, testParameter0) + "\"")
		debug call Print("ReplaceString - Result is \"" + ReplaceString(testString, testParameter0, testParameter1) + "\"")
		debug call Print("RemoveSubString - Result is \"" + RemoveSubString(testString, 0, StringLength(testParameter0)) + "\"")
		debug call Print("RemoveString - Result is \"" + RemoveString(testString, testParameter0) + "\"")
		debug call Print("InsertString - Result is \"" + InsertString(testString, 0, testParameter0) + "\"")
		debug call Print("MoveSubString - Result is \"" + MoveSubString(testString, 0, StringLength(testParameter0), 0) + "\"") /// @todo Move to old position to prevent crashes
		debug call Print("MoveString - Result is \"" + MoveString(testString, testParameter0, 0) + "\"") /// @todo Move to old position to prevent crashes
		debug call Print("ReverseString - Result is \"" + ReverseString(testString) + "\"")
		debug if (StringMatch(testString, testParameter0, true)) then
			debug call Print("StringMatch - Result is true.")
		debug else
			debug call Print("StringMatch - Result is false.")
		debug endif
	endfunction

	function GetStringFormatDebug takes nothing returns nothing
		debug call Print(Format("Hallo %1%, wie heißt du? Du bist %2%!!!!").s("Peter").s("ein Arschloch").result())
	endfunction

	function AStringDebug takes nothing returns nothing
		local ABenchmark benchmark = ABenchmark.create("String debug")
		local string testString = "Lieber Peter, gestern ging es mir gut."
		local string testParameter0 = "ging"
		local string testParameter1 = "auch"
		call benchmark.start()
		call GetStringConversionDebug()
		call GetStringPoolFunctionsDebug(testString, testParameter0)
		call GetStringMiscFunctionsDebug(testString, testParameter0, testParameter1)
		call GetStringFormatDebug()
		call benchmark.stop()
		call benchmark.show()
		call benchmark.destroy()
	endfunction

endlibrary