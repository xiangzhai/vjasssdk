/**
* Provides many functions for working with string pools and checking strings and characters for their content.
* @author Tamino Dauth
* @todo Not tested yet!
*/
library ALibraryCoreStringPool requires ALibraryCoreStringMisc

	globals
		private constant string ALPHABETICAL_CHARS = "abcdefghijklmnopqrstuvwxyz"
		private constant string NUMERAL_CHARS = "0123456789"
		private constant string SPECIAL_CHARS = "!§$%&/()=?+-/*,.-;:_~#'|<>"
		private constant string LANGUAGE_SPECIAL_CHARS = "äöüß" /// @todo Use localized string function.
		private constant string SIGNATURE_CHARS = "+-" //Not a random pool. Is used for checking integers.
		private constant string BINARY_CHARS = "01" //Here it's the same.
		private constant string OCTAL_CHARS = "01234567"
		private constant string HEXADECIMAL_CHARS = "0123456789ABCDEF"
	endglobals

	/** Generates a random character from the character pool @param charPool.
	* @param charPool Character pool which is used for generation of the random character.
	* @param capitalisationChance The percental chance for returning a capitalised character.
	* @return A random character from the character pool @param charPool.
	*/
	function GetRandomChar takes string charPool, real capitalisationChance returns string
		local integer randomInteger = GetRandomInt(1, StringLength(charPool))
		local string result = SubString(charPool, (randomInteger - 1), randomInteger)
		local real chance
		if (capitalisationChance > 0.0) then
			set chance = GetRandomReal(1.0, 100.0)
			if (chance <= capitalisationChance) then
				return StringCase(result, true)
			endif
		endif
		return StringCase(result, false)
	endfunction
	
	/**
	* Generates a random alphabetical character.
	* @param capitalisationChance The percental chance for returning a capitalised character.
	* @return A random alphabetical character.
	*/
	function GetRandomAlphabeticalChar takes real capitalisationChance returns string
		return GetRandomChar(ALPHABETICAL_CHARS, capitalisationChance)
	endfunction
	
	/**
	* Generates a random numeral character.
	* @return A random numeral character.
	*/
	function GetRandomNumeralChar takes nothing returns string
		return I2S(GetRandomInt(0, 9))
		//return GetRandomChar(NUMERAL_CHARS, 0.0) //Theoretisch auch durch I2S(GetRandomInt(0, 9)) lösbar. Brauche den StringPool aber vielleicht noch für IsStringNumeral
	endfunction
	
	/**
	* Generates a random special character.
	* @return A random special character.
	*/
	function GetRandomSpecialChar takes nothing returns string
		return GetRandomChar(SPECIAL_CHARS, 0.0)
	endfunction
	
	/**
	* Generates a random special character of the selected map or player language.
	* @param capitalisationChance The percental chance for returning a capitalised character.
	* @return A random special character of the selected map or player language.
	*/
	function GetRandomLanguageSpecialChar takes real capitalisationChance returns string
		return GetRandomChar(LANGUAGE_SPECIAL_CHARS, capitalisationChance)
	endfunction

	/**
	* Generats a random string with the length @param length.
	* Is able to include various other characters.
	* @param length The length of the returned value.
	* @param capitalisationChance The percental chance for returnsing a capitalised character.
	* @param includeNumbers If this value is true number charactes will be added into the string pool.
	* @param includeSpecialChars If this value is true special characters will be added into the string pool.
	* @param includeLanguageSpecialChars If this value is true special characts of the selected map or player language will be added into the string pool.
	* @return A random string generated from the selected string pool.
	*/
	function GetRandomString takes integer length, real capitalisationChance, boolean includeNumbers, boolean includeSpecialChars, boolean includeLanguageSpecialChars returns string
		local integer i
		local string chars = ALPHABETICAL_CHARS
		local string result = ""
		//case sensitivity shouldn't do anything here
		if (includeNumbers) then
			set chars = chars + NUMERAL_CHARS
		endif
		//case sensitivity shouldn't do anything here
		if (includeSpecialChars) then
			set chars = chars + SPECIAL_CHARS
		endif
		if (includeLanguageSpecialChars) then
			set chars = chars + LANGUAGE_SPECIAL_CHARS
		endif
		set i = 0
		loop
			exitwhen (i == length)
			set result = result + GetRandomChar(chars, capitalisationChance)
			set i = i + 1
		endloop
		return result
	endfunction

	/**
	* Checks if string @param usedString contains only characters from string pool @param stringPool.
	* Checks each single character.
	* @param usedString String which should be checked.
	* @param stringPool String pool which will be used for comparing all characters.
	* @return Returns if all characters from string @param usedString are contained by string pool @param stringPool.
	*/
	function IsStringFromStringPool takes string usedString, string stringPool returns boolean
		local integer i
		set i = 1
		loop
			exitwhen (i > StringLength(usedString))
			if (FindString(stringPool, StringCase(usedString, false)) == -1) then //ALibraryStringMisc
				return false
			endif
			set i = (i + 1)
		endloop
		return true
	endfunction

	/**
	* Checks if string @param usedString is alphabetical.
	* @param usedString Checked string.
	* @return Returns true if string @param usedString contains only alphabetical characters.
	*/
	function IsStringAlphabetical takes string usedString returns boolean
		return IsStringFromStringPool(usedString, ALPHABETICAL_CHARS)
	endfunction

	/**
	* Checks if string @param usedString is numeral.
	* @param usedString Checked string.
	* @return Returns true if string @param usedString contains only numeral characters.
	*/
	function IsStringNumeral takes string usedString returns boolean
		return IsStringFromStringPool(usedString, NUMERAL_CHARS)
	endfunction

	/**
	* Checks if string @param usedString contains only special characters.
	* @param usedString Checked string.
	* @return Returns true if string @param usedString contains only special characters.
	*/
	function IsStringSpecialChar takes string usedString returns boolean
		return IsStringFromStringPool(usedString, SPECIAL_CHARS)
	endfunction
	
	/**
	* Checks if string @param usedString contains only special characters from the selected map or player language.
	* @param usedString Checked string.
	* @return Returns true if string @param usedString contains only special characters from the selected map or player language.
	*/
	function IsStringLanguageSpecialChar takes string usedString returns boolean
		return IsStringFromStringPool(usedString, LANGUAGE_SPECIAL_CHARS)
	endfunction

	/**
	* Checks if string @param usedString is a signature.
	* @param usedString Checked string.
	* @return Returns true if string @param usedString is a signature.
	*/
	function IsStringSignature takes string usedString returns boolean
		return IsStringFromStringPool(usedString, SIGNATURE_CHARS)
	endfunction

	/**
	* Checks if string @param usedString is an integer.
	* @param usedString Checked string.
	* @return Returns true if string @param usedString is an integer.
	*/
	function IsStringInteger takes string usedString returns boolean
		if (IsStringSignature(SubString(usedString, 0, 1))) then
			return IsStringNumeral(SubString(usedString, 1, StringLength(usedString)))
		endif
		return IsStringNumeral(SubString(usedString, 0, StringLength(usedString)))
	endfunction

	/**
	* Bei den folgenden Zahlentypen muss die Zahl immer mit einer bestimmten Ziffernfolge beginnen.
	* Ansonsten wird sie NICHT als eine solche Zahl erkannt.
	* Dies ist eine Anlehnung an C++ und wahrscheinlich auch an vielen andere Sprachen:
	* - 0% binär
	* - 0 oktal
	* - 0x hexadezimal
	* Checks if string @param usedString is binary.
	* @param usedString Checked string.
	* @return Returns true if string @param usedString is binary.
	*/
	function IsStringBinary takes string usedString returns boolean
		if (SubString(usedString, 0, 2) == "0%") then
			return IsStringFromStringPool(SubString(usedString, 2, StringLength(usedString)), BINARY_CHARS)
		endif
		return false
	endfunction

	/**
	* Bei den folgenden Zahlentypen muss die Zahl immer mit einer bestimmten Ziffernfolge beginnen.
	* Ansonsten wird sie NICHT als eine solche Zahl erkannt.
	* Dies ist eine Anlehnung an C++ und wahrscheinlich auch an vielen andere Sprachen:
	* - 0% binär
	* - 0 oktal
	* - 0x hexadezimal
	* Checks if string @param usedString is octal.
	* @param usedString Checked string.
	* @return Returns true if string @param usedString is octal.
	*/
	function IsStringOctal takes string usedString returns boolean
		if (SubString(usedString, 0, 1) == "0") then
			return IsStringFromStringPool(SubString(usedString, 1, StringLength(usedString)), OCTAL_CHARS)
		endif
		return false
	endfunction

	/**
	* Bei den folgenden Zahlentypen muss die Zahl immer mit einer bestimmten Ziffernfolge beginnen.
	* Ansonsten wird sie NICHT als eine solche Zahl erkannt.
	* Dies ist eine Anlehnung an C++ und wahrscheinlich auch an vielen andere Sprachen:
	* - 0% binär
	* - 0 oktal
	* - 0x hexadezimal
	* Checks if string @param usedString is hexadecimal.
	* @param usedString Checked string.
	* @return Returns true if string @param usedString is hexadecimal.
	*/
	function IsStringHexadecimal takes string usedString returns boolean
		if (SubString(usedString, 0, 2) == "0x") then
			return IsStringFromStringPool(SubString(usedString, 2, StringLength(usedString)), HEXADECIMAL_CHARS)
		endif
		return false
	endfunction

endlibrary