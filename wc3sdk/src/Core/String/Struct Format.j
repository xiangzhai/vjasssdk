library AStructCoreStringFormat requires ALibraryCoreStringMisc, optional ALibraryCoreDebugMisc

	/**
	* Use this struct to format strings which should be internationalized.
	* @code
	local string message = Format(tr("It's %1% o'clock and you're going to die in %2% hours. Maybe you should ask %3% why you're still alive.")).t(GetTimeOfDay()).i(3).s("Peter").result()
	* @endcode
	* Instead of AFormat.result you can use the global String function.
	* This struct has been inspired by Boost C++ Libraries projects's format module (@link http://www.boost.org/doc/libs/1_43_0/libs/format/index.html).
	* Unfortunately vJass does not allow operator and method overloading.
	* @see String, Format, tr, sc, IntegerArg, RealArg, RealArgW, StringArg, HandleArg
	* @author Tamino Dauth
	*/
	struct AFormat
		private integer m_position
		private string m_text

		//! runtextmacro A_STRUCT_DEBUG("\"AFormat\"")

		public method position takes nothing returns integer
			return this.m_position
		endmethod

		public method text takes nothing returns string
			return this.m_text
		endmethod

		//! textmacro AFormatMethod takes NAME, TYPE, TYPECHARS, CONVERSION, PARAMETERS
			public method $NAME$ takes $TYPE$ value $PARAMETERS$ returns thistype
				local string positionString = "%" + I2S(this.m_position + 1) + "%"
				local integer index = FindString(this.m_text, positionString)
				if (index == -1) then
					set index = FindString(this.m_text, "%$TYPECHARS$")
					if (index != -1) then
						set this.m_text = SubString(this.m_text, 0, index) + $CONVERSION$ + SubString(this.m_text, index + StringLength("%$TYPECHARS$"), StringLength(this.m_text))
					else
						debug call this.print("Format error in string \"" + this.m_text + "\" at position " + I2S(this.m_position) + ".")
					endif
				else
					set this.m_text = SubString(this.m_text, 0, index) + $CONVERSION$ + SubString(this.m_text, index + StringLength(positionString), StringLength(this.m_text))
				endif
				set this.m_position = this.m_position + 1
				return this
			endmethod
		//! endtextmacro

		//! runtextmacro AFormatMethod("i", "integer", "i", "I2S(value)", "")
		//! runtextmacro AFormatMethod("integer", "integer", "i", "I2S(value)", "")
		//! runtextmacro AFormatMethod("r", "real", "r", "R2S(value)", "")
		//! runtextmacro AFormatMethod("real", "real", "r", "R2S(value)", "")
		//! runtextmacro AFormatMethod("rw", "real", "r", "R2SW(value, width, precision)", ", integer width, integer precision")
		//! runtextmacro AFormatMethod("realWidth", "real", "r", "R2SW(value, width, precision)", ", integer width, integer precision")
		//! runtextmacro AFormatMethod("s", "string", "s", "value", "")
		//! runtextmacro AFormatMethod("string", "string", "s", "value", "")
		//! runtextmacro AFormatMethod("h", "handle", "h", "I2S(GetHandleId(value))", "")
		//! runtextmacro AFormatMethod("handle", "handle", "h", "I2S(GetHandleId(value))", "")
		//! runtextmacro AFormatMethod("u", "unit", "u", "GetUnitName(value)", "")
		//! runtextmacro AFormatMethod("unit", "unit", "u", "GetUnitName(value)", "")
		//! runtextmacro AFormatMethod("it", "item", "it", "GetItemName(value)", "")
		//! runtextmacro AFormatMethod("item", "item", "it", "GetItemName(value)", "")
		//! runtextmacro AFormatMethod("d", "destructable", "d", "GetDestructableName(value)", "")
		//! runtextmacro AFormatMethod("destructable", "destructable", "d", "GetDestructableName(value)", "")
		//! runtextmacro AFormatMethod("p", "player", "p", "GetPlayerName(value)", "")
		//! runtextmacro AFormatMethod("player", "player", "p", "GetPlayerName(value)", "")
		//! runtextmacro AFormatMethod("he", "unit", "he", "GetHeroProperName(value)", "")
		//! runtextmacro AFormatMethod("hero", "unit", "he", "GetHeroProperName(value)", "")
		//! runtextmacro AFormatMethod("o", "integer", "o", "GetObjectName(value)", "")
		//! runtextmacro AFormatMethod("object", "integer", "o", "GetObjectName(value)", "")
		//! runtextmacro AFormatMethod("l", "string", "l", "GetLocalizedString(value)", "")
		//! runtextmacro AFormatMethod("localizedString", "string", "l", "GetLocalizedString(value)", "")
		//! runtextmacro AFormatMethod("k", "string", "k", "I2S(GetLocalizedHotkey(value))", "")
		//! runtextmacro AFormatMethod("localizedHotkey", "string", "k", "I2S(GetLocalizedHotkey(value))", "")
		//! runtextmacro AFormatMethod("e", "integer", "e", "GetExternalString(value)", "")
		//! runtextmacro AFormatMethod("externalString", "integer", "e", "GetExternalString(value)", "")
		/// Use seconds as parameter!
		//! runtextmacro AFormatMethod("t", "integer", "t", "GetTimeString(value)", "")
		/// Use seconds as parameter!
		//! runtextmacro AFormatMethod("time", "integer", "t", "GetTimeString(value)", "")

		public method result takes nothing returns string
			local string result = this.m_text
			call this.destroy()
			return result
		endmethod

		public static method create takes string text returns AFormat
			local thistype this = thistype.allocate()
			set this.m_position = 0
			set this.m_text = text
			return this
		endmethod
	endstruct

	function String takes AFormat format returns string
		local string result = format.text()
		call format.destroy()
		return result
	endfunction

	function Format takes string text returns AFormat
		return AFormat.create(text)
	endfunction

endlibrary