/// ALibraryCoreDebugMisc is required because we have some checkings about the length.
library AStructCoreInterfaceMultiboardBar requires ALibraryCoreDebugMisc, AStructCoreInterfaceInterfaceHashTable

	/// @todo vJass bug, should be a part of @struct AMultiboardBar.
	/// This represents the function which controls both MultiboardBar values:
	/// Value and maximum value.
	function interface AMultiboardBarValueFunction takes AMultiboardBar multiboardBar returns real

	struct AMultiboardBar
		//static constant members
		private static constant integer maxLength = 20
		//start members
		private multiboard usedMultiboard
		private integer column
		private integer row
		private integer length
		private real refreshRate
		private boolean horizontal
		//dynamic members
		private real value
		private real maxValue
		private string array valueIcon[AMultiboardBar.maxLength]
		private string array emptyIcon[AMultiboardBar.maxLength]
		private AMultiboardBarValueFunction valueFunction
		private AMultiboardBarValueFunction maxValueFunction
		//members
		private integer colouredPart
		private trigger refreshTrigger

		//! runtextmacro A_STRUCT_DEBUG("\"AMultiboardBar\"")

		//dynamic member methods

		public method setValue takes real value returns nothing
			set this.value = value
		endmethod

		public method getValue takes nothing returns real
			return this.value
		endmethod

		public method setMaxValue takes real maxValue returns nothing
			set this.maxValue = maxValue
		endmethod

		public method getMaxValue takes nothing returns real
			return this.maxValue
		endmethod

		public method setValueIcon takes integer length, string valueIcon returns nothing
			set this.valueIcon[length] = valueIcon
		endmethod

		public method getValueIcon takes integer length returns string
			return this.valueIcon[length]
		endmethod

		public method setEmptyIcon takes integer length, string emptyIcon returns nothing
			set this.emptyIcon[length] = emptyIcon
		endmethod

		public method getEmptyIcon takes integer length returns string
			return this.emptyIcon[length]
		endmethod

		/// Sets the function which should return the value of the multiboard bar when it refreshs.
		public method setValueFunction takes AMultiboardBarValueFunction valueFunction returns nothing
			set this.valueFunction = valueFunction
		endmethod

		public method getValueFunction takes nothing returns AMultiboardBarValueFunction
			return this.valueFunction
		endmethod

		public method setMaxValueFunction takes AMultiboardBarValueFunction maxValueFunction returns nothing
			set this.maxValueFunction = maxValueFunction
		endmethod

		public method getMaxValueFunction takes nothing returns AMultiboardBarValueFunction
			return this.maxValueFunction
		endmethod

		//methods
		
		/// Die Farbe des Feldes mit Wert wird je nach Anteil des Wertes vom Maximalwert gesetzt.
		public method refresh takes nothing returns nothing
			local integer i
			local multiboarditem multiboardItem
			set this.colouredPart = R2I((this.value / this.maxValue) * I2R(this.length))
			set i = 0
			loop
                		exitwhen (i == this.length)
				if (this.horizontal) then
					set multiboardItem = MultiboardGetItem(this.usedMultiboard, this.row, this.column + i)
				else
					set multiboardItem = MultiboardGetItem(this.usedMultiboard, this.row + i, this.column)
				endif
				//coloured part
				if (i < this.colouredPart) then
					call MultiboardSetItemIcon(multiboardItem, this.valueIcon[i])
				//plain Part
				else
					call MultiboardSetItemIcon(multiboardItem, this.emptyIcon[i])
				endif
				call MultiboardReleaseItem(multiboardItem) //TEST
				set multiboardItem = null
				set i = i + 1
			endloop
		endmethod

		/// First call after setting the length.
		public method setIcons takes integer start, integer end, string icon, boolean valueIcon returns nothing
			local integer i
			debug if ((start >= 0) and (start < this.length)) then
				debug if ((end > 0) and (end < this.length)) then
					set i = start
					loop
						exitwhen(i == (end + 1))
						if (valueIcon) then
							set this.valueIcon[i] = icon	
						else
							set this.emptyIcon[i] = icon
						endif
						set i = (i + 1)
					endloop
				debug else
					debug call this.print("The value 'End' has an invalid size.")
				debug endif
			debug else
				debug call this.print("The value 'Start' has an invalid size.")
			debug endif
		endmethod
		
		//comfort methods

		/// Erst aufrufen, nachdem man die Länge gesetzt hat.
		public method setAllIcons takes string icon, boolean valueIcon returns nothing
			call this.setIcons(0, (this.length - 1), icon, valueIcon)
		endmethod
		
		/// @return The index of the first field (column or row) which is not used by the bar (alignment is left to right and up to bottom).
		public method getFirstFreeField takes nothing returns integer
			if (this.horizontal) then
				return this.column + this.length
			endif
			return this.row + this.length
		endmethod

		private method resizeMultiboard takes nothing returns nothing
			if (this.horizontal) then
				if (MultiboardGetColumnCount(this.usedMultiboard) < (this.column + this.length)) then
					call MultiboardSetColumnCount(this.usedMultiboard, (this.column + this.length))
				endif
				if (MultiboardGetRowCount(this.usedMultiboard) <= this.row) then
					call MultiboardSetRowCount(this.usedMultiboard, (this.row + 1))
				endif
			else
				if (MultiboardGetColumnCount(this.usedMultiboard) <= this.column) then
					call MultiboardSetColumnCount(this.usedMultiboard, (this.column + 1))
				endif
				if (MultiboardGetRowCount(this.usedMultiboard) < (this.row + this.length)) then
					call MultiboardSetRowCount(this.usedMultiboard, (this.row + this.length))
				endif
			endif
		endmethod

		private method setupMultiboardItems takes nothing returns nothing
			local integer i
			local multiboarditem multiboardItem
			set i = 0
			loop
				exitwhen (i == this.length)
				if (this.horizontal) then
					set multiboardItem = MultiboardGetItem(this.usedMultiboard, this.row, this.column + i)
				else
					set multiboardItem = MultiboardGetItem(this.usedMultiboard, this.row + i, this.column)
				endif
				call MultiboardSetItemStyle(multiboardItem, false, true)
				call MultiboardSetItemWidth(multiboardItem, 0.01) //Einzeln einstellen, um nicht das ganze Multiboard zu verändern
				call MultiboardReleaseItem(multiboardItem)
				set multiboardItem = null
				set i = i + 1
			endloop
		endmethod

		private static method triggerActionRefresh takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AMultiboardBar multiboardBar = AGetInterfaceHashTable().getHandleInteger(triggeringTrigger, "multiboardBar") //AClassInterfaceInterfaceHashTable
			set multiboardBar.value = multiboardBar.valueFunction.evaluate(multiboardBar) //Man braucht evaluate, da die Funktion einen Wert zurckliefert.
			set multiboardBar.maxValue = multiboardBar.maxValueFunction.evaluate(multiboardBar)
			call multiboardBar.refresh() //Das false ist egal, da Horizontal nur beim ersten Erschaffen gesetzt wird
			set triggeringTrigger = null
		endmethod

		private method createRefreshTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			if (this.refreshRate > 0.0) then
				set this.refreshTrigger = CreateTrigger()
				set triggerEvent = TriggerRegisterTimerEvent(this.refreshTrigger, this.refreshRate, true)
				set triggerAction = TriggerAddAction(this.refreshTrigger, function AMultiboardBar.triggerActionRefresh)
				call AGetInterfaceHashTable().storeHandleInteger(this.refreshTrigger, "multiboardBar", this) //ALibraryInterfaceHashTable
				set triggerEvent = null
				set triggerAction = null
			endif
		endmethod
		
		/// If there aren't enough items in multiboard yet required onces will be added automatically.
		/// @param refreshRate If this value is bigger than 0 multiboard bar will be refreshed.
		/// @param horizontal This value is not dynamic.
		public static method create takes multiboard usedMultiboard, integer column, integer row, integer length, real refreshRate, boolean horizontal, real value, real maxValue, AMultiboardBarValueFunction valueFunction, AMultiboardBarValueFunction maxValueFunction returns AMultiboardBar
			local AMultiboardBar this = AMultiboardBar.allocate()
			//start members
			set this.usedMultiboard = usedMultiboard
			set this.column = column
			set this.row = row
			set this.length = length
			set this.refreshRate = refreshRate
			set this.horizontal = horizontal
			//dynamic members
			set this.value = value
			set this.maxValue = maxValue
			set this.valueFunction = valueFunction
			set this.maxValueFunction = maxValueFunction
			//members
			set this.colouredPart = 0

			call this.resizeMultiboard()
			call this.setupMultiboardItems()
			call this.createRefreshTrigger()
			return this
		endmethod

		private method destroyRefreshTrigger takes nothing returns nothing
			if (this.refreshRate > 0.0) then
				call AGetInterfaceHashTable().destroyTrigger(this.refreshTrigger) //AClassInterfaceInterfaceHashTable
				set this.refreshTrigger = null
			endif
		endmethod

		public method onDestroy takes nothing returns nothing
			//start members
			set this.usedMultiboard = null

			call this.destroyRefreshTrigger()
		endmethod
	endstruct

endlibrary