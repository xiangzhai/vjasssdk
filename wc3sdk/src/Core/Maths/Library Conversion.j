library ALibraryCoreMathsConversion requires ALibraryCoreDebugMisc, ALibraryCoreStringMisc

	/// Converts an integer into any numeral system
	/// The A in I2A stands for alpha-numeric.
	function I2A takes integer value, string chars returns string
		local string alphanumeric = ""
		local integer index = 0
		debug if ((value >= 0) or (value <= 0x7FFFFFFF)) then
			loop
				set index = (ModuloInteger(value, StringLength(chars)) + 1)
				set value = (value / StringLength(chars))
				set alphanumeric = (SubStringBJ(chars, index, index) + alphanumeric)
				exitwhen (value == 0)
			endloop
		debug else
			debug call Print("Invalid integer.")
		debug endif
		return alphanumeric
	endfunction

	/// Converts a number from any numeral system into an integer.
	/// The A in A2I stands for alpha-numeric.
	function A2I takes string alphanumeric, string chars returns integer
		local integer value = 0
		local integer index = 1
		loop
			set value = value + ((FindString(chars, SubString(alphanumeric, (StringLength(alphanumeric) - 1), StringLength(alphanumeric)))) * index)
			exitwhen (StringLength(alphanumeric) == 1)
			set alphanumeric = SubString(alphanumeric, 0, (StringLength(alphanumeric) - 1))
			set index = index * StringLength(chars)
		endloop
		return value
	endfunction

	//! textmacro AMathsConversionMacro takes TYPENAME, CHARS
		/// @state untested
		function I2$TYPENAME$ takes integer Integer returns string
			return I2A(Integer, "$CHARS$")
		endfunction

		/// @state untested
		function $TYPENAME$2I takes string $TYPENAME$ returns integer
			return A2I($TYPENAME$, "$CHARS$")
		endfunction
	//! endtextmacro

	//! runtextmacro AMathsConversionMacro("Binary", "01")
	//! runtextmacro AMathsConversionMacro("Octal", "01234567")
	//! runtextmacro AMathsConversionMacro("Hexadecimal", "0123456789ABCDEF")

endlibrary