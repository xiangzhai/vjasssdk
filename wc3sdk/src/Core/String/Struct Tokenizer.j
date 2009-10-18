library AStructCoreStringTokenizer requires ALibraryCoreDebugMisc

	/**
	* @link http://www.wc3c.net/showthread.php?t=106830
	* @author ToukoAozaki
	* @author Tamino Dauth
	*/
	struct ATokenizer
		//dynamic members
		private string m_separators
		private integer m_position
		//start members
		private string m_string

		//! runtextmacro optional A_STRUCT_DEBUG("\"ATokenizer\"")

		//dynamic members

		/**
		* @param separators Should be list of separating characters.
		*/
		public method setSeparators takes string separators returns nothing
			set this.m_separators = separators
		endmethod

		public method separators takes nothing returns string
			return this.m_separators
		endmethod

		public method setPosition takes integer position returns nothing
			debug if (position < 0) then
				debug set position = 0
				debug call this.print("Position is smaller than 0.")
			debug endif
			set this.m_position = position
		endmethod

		public method position takes nothing returns integer
			return this.m_position
		endmethod

		//start members

		public method string takes nothing returns string
			return this.m_string
		endmethod

		public static method create takes string whichString returns thistype
			local thistype this = thistype.allocate()
			//dynamic members
			set this.m_separators = " \t"
			set this.m_position = 0
			//start members
			set this.m_string = whichString
			return this
		endmethod

		public method hasMore takes nothing returns boolean
			return this.m_position < StringLength(this.m_string)
		endmethod

		public method next takes nothing returns string
			local string subString
			local integer i
			local integer j
			local boolean found
			local string result
			if (not this.hasMore()) then
				return null
			//invalid separator
			elseif (this.m_separators == null or this.m_separators == "") then
				set i = StringLength(this.m_string)
			else
				set found = false
				set i = this.m_position
				loop
					exitwhen (i >= StringLength(this.m_string))
					set subString = SubString(this.m_string, i, i + 1)
					set j = 0
					loop
						exitwhen (j >= StringLength(this.m_separators))
						if (subString ==  SubString(this.m_separators, j, j + 1)) then
							set found = true
							exitwhen (true)
						endif
						set j = j + 1
					endloop
					exitwhen (found)
					set i = i + 1
				endloop
			endif
			set result = SubString(this.m_string, this.m_position, i)
			set this.m_position = i + 1
			return result
		endmethod

		public method reset takes nothing returns nothing
			set this.m_position = 0
		endmethod
	endstruct

endlibrary