library AStructCoreInterfaceMultiboardBar requires optional ALibraryCoreDebugMisc, ALibraryCoreMathsReal, AStructCoreGeneralHashTable

	/**
	* @todo vJass bug, should be a part of @struct AMultiboardBar.
	* This represents the function which controls both MultiboardBar values:
	* Value and maximum value.
	*/
	function interface AMultiboardBarValueFunction takes AMultiboardBar multiboardBar returns real

	/**
	* Multiboard bars can be used to show values in form of progress bars in the Warcraft 3 TFT multiboard.
	* They can either be horizontal or vertical and use custom icons.
	*/
	struct AMultiboardBar
		// static constant members
		private static constant integer maxLength = 20
		// construction members
		private multiboard m_multiboard
		private integer m_column
		private integer m_row
		private integer m_length
		private real m_refreshRate
		private boolean m_horizontal
		// dynamic members
		private real m_value
		private real m_maxValue
		private string array m_valueIcon[AMultiboardBar.maxLength]
		private string array m_emptyIcon[AMultiboardBar.maxLength]
		private AMultiboardBarValueFunction m_valueFunction
		private AMultiboardBarValueFunction m_maxValueFunction
		// members
		private integer m_colouredPart
		private trigger m_refreshTrigger

		//! runtextmacro optional A_STRUCT_DEBUG("\"AMultiboardBar\"")

		// dynamic member methods

		public method setValue takes real value returns nothing
			set this.m_value = value
		endmethod

		public method value takes nothing returns real
			return this.m_value
		endmethod

		public method setMaxValue takes real maxValue returns nothing
			set this.m_maxValue = maxValue
		endmethod

		public method maxValue takes nothing returns real
			return this.m_maxValue
		endmethod

		public method setValueIcon takes integer length, string valueIcon returns nothing
			set this.m_valueIcon[length] = valueIcon
		endmethod

		public method valueIcon takes integer length returns string
			return this.m_valueIcon[length]
		endmethod

		public method setEmptyIcon takes integer length, string emptyIcon returns nothing
			set this.m_emptyIcon[length] = emptyIcon
		endmethod

		public method emptyIcon takes integer length returns string
			return this.m_emptyIcon[length]
		endmethod

		/// Sets the function which should return the value of the multiboard bar when it refreshs.
		public method setValueFunction takes AMultiboardBarValueFunction valueFunction returns nothing
			set this.m_valueFunction = valueFunction
		endmethod

		public method valueFunction takes nothing returns AMultiboardBarValueFunction
			return this.m_valueFunction
		endmethod

		public method setMaxValueFunction takes AMultiboardBarValueFunction maxValueFunction returns nothing
			set this.m_maxValueFunction = maxValueFunction
		endmethod

		public method maxValueFunction takes nothing returns AMultiboardBarValueFunction
			return this.m_maxValueFunction
		endmethod

		// methods

		/// Refreshes multiboard bar.
		public method refresh takes nothing returns nothing
			local integer i
			local multiboarditem multiboardItem
			if (this.m_maxValue != 0) then
				set this.m_colouredPart = R2I(RoundTo(this.m_value * I2R(this.m_length) / this.m_maxValue, 1.0))
			else
				set this.m_colouredPart = 0
			endif
			set i = 0
			loop
				exitwhen (i == this.m_length)
				if (this.m_horizontal) then
					set multiboardItem = MultiboardGetItem(this.m_multiboard, this.m_row, this.m_column + i)
				else
					set multiboardItem = MultiboardGetItem(this.m_multiboard, this.m_row + i, this.m_column)
				endif
				// coloured part
				if (i < this.m_colouredPart) then
					call MultiboardSetItemIcon(multiboardItem, this.m_valueIcon[i])
				// plain Part
				else
					call MultiboardSetItemIcon(multiboardItem, this.m_emptyIcon[i])
				endif
				call MultiboardReleaseItem(multiboardItem) //TEST
				set multiboardItem = null
				set i = i + 1
			endloop
		endmethod

		/// First call after setting the length.
		public method setIcons takes integer start, integer end, string icon, boolean valueIcon returns nothing
			local integer i
			debug if ((start >= 0) and (start < this.m_length)) then
				debug if ((end > 0) and (end < this.m_length)) then
					set i = start
					loop
						exitwhen(i == end + 1)
						if (valueIcon) then
							set this.m_valueIcon[i] = icon
						else
							set this.m_emptyIcon[i] = icon
						endif
						set i = i + 1
					endloop
				debug else
					debug call this.print("The value 'end' has an invalid size: " + I2S(end) + ".")
				debug endif
			debug else
				debug call this.print("The value 'start' has an invalid size: " + I2S(start) + ".")
			debug endif
		endmethod

		// convenience methods

		/// Do only call after setting length!
		public method setAllIcons takes string icon, boolean valueIcon returns nothing
			call this.setIcons(0, this.m_length - 1, icon, valueIcon)
		endmethod

		/// @return The index of the first field (column or row) which is not used by the bar (alignment is left to right and up to bottom).
		public method firstFreeField takes nothing returns integer
			if (this.m_horizontal) then
				return this.m_column + this.m_length
			endif
			return this.m_row + this.m_length
		endmethod

		private method resizeMultiboard takes nothing returns nothing
			if (this.m_horizontal) then
				if (MultiboardGetColumnCount(this.m_multiboard) < (this.m_column + this.m_length)) then
					call MultiboardSetColumnCount(this.m_multiboard, this.m_column + this.m_length)
				endif
				if (MultiboardGetRowCount(this.m_multiboard) <= this.m_row) then
					call MultiboardSetRowCount(this.m_multiboard, this.m_row + 1)
				endif
			else
				if (MultiboardGetColumnCount(this.m_multiboard) <= this.m_column) then
					call MultiboardSetColumnCount(this.m_multiboard, this.m_column + 1)
				endif
				if (MultiboardGetRowCount(this.m_multiboard) < (this.m_row + this.m_length)) then
					call MultiboardSetRowCount(this.m_multiboard, (this.m_row + this.m_length))
				endif
			endif
		endmethod

		private method setupMultiboardItems takes nothing returns nothing
			local integer i
			local multiboarditem multiboardItem
			set i = 0
			loop
				exitwhen (i == this.m_length)
				if (this.m_horizontal) then
					set multiboardItem = MultiboardGetItem(this.m_multiboard, this.m_row, this.m_column + i)
				else
					set multiboardItem = MultiboardGetItem(this.m_multiboard, this.m_row + i, this.m_column)
				endif
				call MultiboardSetItemStyle(multiboardItem, false, true)
				call MultiboardSetItemWidth(multiboardItem, 0.01) // set each item to prevent changing the whole multiboard
				call MultiboardReleaseItem(multiboardItem)
				set multiboardItem = null
				set i = i + 1
			endloop
		endmethod

		private static method triggerActionRefresh takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AMultiboardBar this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			set this.m_value = this.m_valueFunction.evaluate(this)
			set this.m_maxValue = this.m_maxValueFunction.evaluate(this)
			call this.refresh()
			set triggeringTrigger = null
		endmethod

		private method createRefreshTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			if (this.m_refreshRate > 0.0) then
				set this.m_refreshTrigger = CreateTrigger()
				set triggerEvent = TriggerRegisterTimerEvent(this.m_refreshTrigger, this.m_refreshRate, true)
				set triggerAction = TriggerAddAction(this.m_refreshTrigger, function AMultiboardBar.triggerActionRefresh)
				call AHashTable.global().setHandleInteger(this.m_refreshTrigger, "this", this)
				set triggerEvent = null
				set triggerAction = null
			endif
		endmethod

		/**
		* If there aren't enough items in multiboard yet required onces will be added automatically.
		* @param refreshRate If this value is bigger than 0 multiboard bar will be refreshed.
		* @param horizontal This value is not dynamic.
		*/
		public static method create takes multiboard usedMultiboard, integer column, integer row, integer length, real refreshRate, boolean horizontal, real value, real maxValue, AMultiboardBarValueFunction valueFunction, AMultiboardBarValueFunction maxValueFunction returns thistype
			local thistype this = thistype.allocate()
			// construction members
			set this.m_multiboard = usedMultiboard
			set this.m_column = column
			set this.m_row = row
			set this.m_length = length
			set this.m_refreshRate = refreshRate
			set this.m_horizontal = horizontal
			// dynamic members
			set this.m_value = value
			set this.m_maxValue = maxValue
			set this.m_valueFunction = valueFunction
			set this.m_maxValueFunction = maxValueFunction
			// members
			set this.m_colouredPart = 0

			call this.resizeMultiboard()
			call this.setupMultiboardItems()
			call this.createRefreshTrigger()
			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			// construction members
			set this.m_multiboard = null
			// members
			if (this.m_refreshRate > 0.0) then
				call AHashTable.global().destroyTrigger(this.m_refreshTrigger)
				set this.m_refreshTrigger = null
			endif
		endmethod
	endstruct

endlibrary