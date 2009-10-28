/**
* Provides many functions for working with character pools and checking strings and characters for their content.
* @author Tamino Dauth
*/
library ALibraryCoreStringPool requires ALibraryCoreStringMisc

	globals
		constant string AAlphabeticalCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
		constant string ANumeralCharacters = "0123456789"
		constant string ASpecialCharacters = "!§$%&/()=?+-/*,.-;:_~#'|<>äöüß"
		constant string AWhiteSpaceCharacters = " \t"
		constant string ASignatureCharacters = "+-"
		constant string ABinaryCharacters = "01"
		constant string AOctalCharacters = "01234567"
		constant string AHexadecimalCharacters = "0123456789ABCDEF"
	endglobals

	/**
	* Generates a random character from the character pool @param charPool.
	* @param charPool Character pool which is used for generation of a random character.
	* @return Returns a random character from the character pool @param charPool.
	*/
	function GetRandomCharacter takes string charPool returns string
		local integer randomInteger = GetRandomInt(1, StringLength(charPool))
		return SubString(charPool, randomInteger - 1, randomInteger)
	endfunction
	
	/**
	* Generates a random alphabetical character.
	* @return Returns a random alphabetical character.
	*/
	function GetRandomAlphabeticalCharacter takes nothing returns string
		return GetRandomCharacter(AAlphabeticalCharacters)
	endfunction
	
	/**
	* Generates a random numeral character.
	* @return Returns a random numeral character.
	*/
	function GetRandomNumeralCharacter takes nothing returns string
		return GetRandomCharacter(ANumeralCharacters)
	endfunction
	
	/**
	* Generates a random special character.
	* @return Returns a random special character.
	*/
	function GetRandomSpecialCharacter takes nothing returns string
		return GetRandomCharacter(ASpecialCharacters)
	endfunction

	/**
	* Generates a random white-space character.
	* @return Returns a random white-space character.
	*/
	function GetRandomWhiteSpaceCharacter takes nothing returns string
		return GetRandomCharacter(AWhiteSpaceCharacters)
	endfunction

	/**
	* Generats a random string with the length @param length.
	* Is able to include various other characters.
	* @param length The length of the returned value.
	* @param includeNumberCharacters If this value is true number charactes will be added into the string pool.
	* @param includeSpecialCharacters If this value is true special characters will be added into the string pool.
	* @param includeWhiteSpaceCharacters If this value is true white-space characters will be added into the string pool.
	* @return Returns a random string generated from the selected string pool.
	*/
	function GetRandomString takes integer length, boolean includeNumberCharacters, boolean includeSpecialCharacters, boolean includeWhiteSpaceCharacters returns string
		local integer i
		local string characters = AAlphabeticalCharacters
		local string result = ""
		if (includeNumberCharacters) then
			set characters = characters + ANumeralCharacters
		endif
		if (includeSpecialCharacters) then
			set characters = characters + ASpecialCharacters
		endif
		if (includeWhiteSpaceCharacters) then
			set characters = characters + AWhiteSpaceCharacters
		endif
		set i = 0
		loop
			exitwhen (i == length)
			set result = result + GetRandomCharacter(characters)
			set i = i + 1
		endloop
		return result
	endfunction

	/**
	* Checks if string @param whichString contains only characters from character pool @param characterPool.
	* Checks each single character.
	* @param whichString String which should be checked.
	* @param characterPool Character pool which will be used for comparing all characters.
	* @return Returns if all characters from string @param whichString are contained by string pool @param characterPool.
	*/
	function IsStringFromCharacterPool takes string whichString, string characterPool returns boolean
		local integer i
		set i = 1
		loop
			exitwhen (i > StringLength(whichString))
			if (FindString(characterPool, SubString(whichString, i - 1, i)) == -1) then
				return false
			endif
			set i = i + 1
		endloop
		return true
	endfunction

	/**
	* Checks if string @param whichString is alphabetical.
	* @param whichString Checked string.
	* @return Returns true if string @param whichString contains only alphabetical characters.
	*/
	function IsStringAlphabetical takes string whichString returns boolean
		return IsStringFromCharacterPool(whichString, AAlphabeticalCharacters)
	endfunction

	/**
	* Checks if string @param whichString is numeral.
	* @param whichString Checked string.
	* @return Returns true if string @param whichString contains only numeral characters.
	*/
	function IsStringNumeral takes string whichString returns boolean
		return IsStringFromCharacterPool(whichString, ANumeralCharacters)
	endfunction

	/**
	* Checks if string @param whichString contains only special characters.
	* @param whichString Checked string.
	* @return Returns true if string @param whichString contains only special characters.
	*/
	function IsStringSpecial takes string whichString returns boolean
		return IsStringFromCharacterPool(whichString, ASpecialCharacters)
	endfunction

	/**
	* Checks if string @param whichString contains only white-space characters.
	* @param whichString Checked string.
	* @return Returns true if string @param whichString contains only white-space characters.
	*/
	function IsStringWhiteSpace takes string whichString returns boolean
		return IsStringFromCharacterPool(whichString, AWhiteSpaceCharacters)
	endfunction

	/**
	* Checks if string @param whichString is a signature.
	* @param whichString Checked string.
	* @return Returns true if string @param whichString is a signature.
	*/
	function IsStringSignature takes string whichString returns boolean
		return IsStringFromCharacterPool(whichString, ASignatureCharacters)
	endfunction

	/**
	* Checks if string @param which is an integer.
	* @param which Checked string.
	* @return Returns true if string @param which is an integer.
	*/
	function IsStringInteger takes string whichString returns boolean
		if (IsStringSignature(SubString(whichString, 0, 1))) then
			if (StringLength(whichString) > 1) then
				return IsStringNumeral(SubString(whichString, 1, StringLength(whichString)))
			endif
			return false
		endif
		return IsStringNumeral(SubString(whichString, 0, StringLength(whichString)))
	endfunction

	/**
	* Bei den folgenden Zahlentypen muss die Zahl immer mit einer bestimmten Ziffernfolge beginnen.
	* Ansonsten wird sie NICHT als eine solche Zahl erkannt.
	* Dies ist eine Anlehnung an C++ und wahrscheinlich auch an vielen andere Sprachen:
	* - 0% binär
	* - 0 oktal
	* - 0x hexadezimal
	*/

	/**
	* Checks if string @param whichString is binary.
	* @param whichString Checked string.
	* @return Returns true if string @param whichString is binary.
	*/
	function IsStringBinary takes string whichString returns boolean
		if (StringLength(whichString) >= 2 and SubString(whichString, 0, 2) == "0%") then
			if (StringLength(whichString) > 2) then
				return IsStringFromCharacterPool(SubString(whichString, 2, StringLength(whichString)), ABinaryCharacters)
			endif
			return false
		endif
		return false
	endfunction

	/**
	* Checks if string @param whichString is octal.
	* @param whichString Checked string.
	* @return Returns true if string @param whichString is octal.
	*/
	function IsStringOctal takes string whichString returns boolean
		if (StringLength(whichString) >= 1 and SubString(whichString, 0, 1) == "0") then
			if (StringLength(whichString) > 1) then
				return IsStringFromCharacterPool(SubString(whichString, 1, StringLength(whichString)), AOctalCharacters)
			endif
			return false
		endif
		return false
	endfunction

	/**
	* Checks if string @param whichString is hexadecimal.
	* @param whichString Checked string.
	* @return Returns true if string @param whichString is hexadecimal.
	*/
	function IsStringHexadecimal takes string whichString returns boolean
		if (StringLength(whichString) >= 2 and SubString(whichString, 0, 2) == "0x") then
			if (StringLength(whichString) > 2) then
				return IsStringFromCharacterPool(SubString(whichString, 2, StringLength(whichString)), AHexadecimalCharacters)
			endif
			return true
		endif
		return false
	endfunction

endlibrary