library AStructCoreStringFormat requires ALibraryCoreStringMisc, optional ALibraryCoreDebugMisc

	/**
	* Use this struct to format strings which should be internationalized.
	* @code
	local string message = Format(tr("It's %1% o'clock and you're going to die in %2% hours. Maybe you should ask %3% why you're still alive.")).t(GetTimeOfDay()).i(3).s("Peter").result()
	* @endcode
	* Instead of AFormat.create you can use the global function Format.
	* Instead of AFormat.result you can use the global function String.
	* This struct has been inspired by Boost C++ Libraries projects's format module (@link http://www.boost.org/doc/libs/1_43_0/libs/format/index.html).
	* Unfortunately vJass does not allow operator and method overloading.
	* @see String, Format, tr, sc, IntegerArg, RealArg, RealArgW, StringArg, HandleArg
	* @author Tamino Dauth
	*/
	struct AFormat
		private integer m_position
		private string m_text

		//! runtextmacro A_STRUCT_DEBUG("\"AFormat\"")

		/**
		* Ordered arguments formatting is implemented by storing the current format position.
		* @return Returns the current formatting position/argument.
		*/
		public method position takes nothing returns integer
			return this.m_position
		endmethod

		/**
		* @return Returns the current formatted text/string.
		*/
		public method text takes nothing returns string
			return this.m_text
		endmethod

		//! textmacro AFormatMethod takes NAME, TYPE, TYPECHARS, CONVERSION, PARAMETERS
			public method $NAME$ takes $TYPE$ value $PARAMETERS$ returns thistype
				local string positionString = "%" + I2S(this.m_position + 1) + "%"
				local boolean isFirst = true
				local integer index
				loop
					set index = FindString(this.m_text, positionString)
					if (index == -1) then
						// replace only one %typechar value
						if (isFirst) then
							set index = FindString(this.m_text, "%$TYPECHARS$")
							if (index != -1) then
								set this.m_text = SubString(this.m_text, 0, index) + $CONVERSION$ + SubString(this.m_text, index + StringLength("%$TYPECHARS$"), StringLength(this.m_text))
								set this.m_position = this.m_position + 1 // increase because of exiting loop
							else
								debug call this.print("Format error in string \"" + this.m_text + "\" at position " + I2S(this.m_position) + ".")
								return this // do not increase position?
							endif
						endif
						exitwhen (true)
					else
						set this.m_text = SubString(this.m_text, 0, index) + $CONVERSION$ + SubString(this.m_text, index + StringLength(positionString), StringLength(this.m_text))
						set isFirst = false
					endif

				endloop
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
		//! runtextmacro AFormatMethod("e", "integer", "e", "GetExternalString.evaluate(value)", "")
		//! runtextmacro AFormatMethod("externalString", "integer", "e", "GetExternalString.evaluate(value)", "")
		/// Use seconds as parameter!
		//! runtextmacro AFormatMethod("t", "integer", "t", "GetTimeString.evaluate(value)", "")
		/// Use seconds as parameter!
		//! runtextmacro AFormatMethod("time", "integer", "t", "GetTimeString.evaluate(value)", "")

		/**
		* @return Returns the formatted string result and destroys the instance.
		* @note Don't use the destroyed instance afterwards!
		* @see String
		*/
		public method result takes nothing returns string
			local string result = this.m_text
			call this.destroy()
			return result
		endmethod

		/**
		* Creates a new instance based on text @param text.
		* @see Format
		*/
		public static method create takes string text returns AFormat
			local thistype this = thistype.allocate()
			set this.m_position = 0
			set this.m_text = text
			return this
		endmethod
	endstruct

	/**
	* Global function and alternative to AFormat.result.
	* @return Returns the formatted result and destroys the formatting instance.
	* @see AFormat.result
	*/
	function String takes AFormat format returns string
		return format.result()
	endfunction

	/**
	* Global function and alternative to AFormat.create.
	* @return Returns a newly created formatting instance.
	* @see AFormat.create
	*/
	function Format takes string text returns AFormat
		return AFormat.create(text)
	endfunction

endlibrary