library AStructCoreInterfaceIndicator requires ALibraryCoreGeneralConversion, AStructCoreInterfaceInterfaceHashTable

	struct AIndicator
		//static constant members
		public static constant integer objectTypeImage = 0
		public static constant integer objectTypeTextTag = 1
		public static constant integer objectTypeUnit = 2
		//dynamic members
		private handle object
		private integer objectType
		private real rate //test if there are errors because it is dynamic
		private real time //here it's the same
		private real red //we are using BJ functions so we're calculating with per cents
		private real green
		private real blue
		private real transparency
		//members
		private trigger refreshTrigger //= null
		private real elapsedTime //= 0.00 //Vergangene Zeit
		private boolean firstTime //= true

		//dynamic member methods

		public method setObject takes handle object returns nothing
			set this.object = object
		endmethod

		public method getObject takes nothing returns handle
			return this.object
		endmethod

		public method setObjectType takes integer objectType returns nothing
			set this.objectType = objectType
		endmethod

		public method getObjectType takes nothing returns integer
			return this.objectType
		endmethod

		public method setRate takes real rate returns nothing
			set this.rate = rate
		endmethod

		public method getRate takes nothing returns real
			return this.rate
		endmethod

		public method setTime takes real time returns nothing
			set this.time = time
		endmethod

		public method getTime takes nothing returns real
			return this.time
		endmethod

		public method setRed takes real red returns nothing
			set this.red = red
		endmethod

		public method getRed takes nothing returns real
			return this.red
		endmethod

		public method setGreen takes real green returns nothing
			set this.green = green
		endmethod

		public method getGreen takes nothing returns real
			return this.green
		endmethod

		public method setBlue takes real blue returns nothing
			set this.blue = blue
		endmethod

		public method getBlue takes nothing returns real
			return this.blue
		endmethod

		public method setTransparency takes real transparency returns nothing
			set this.transparency = transparency
		endmethod

		public method getTransparency takes nothing returns real
			return this.transparency
		endmethod

		//public methods

		/// Flash restarts when enabling the indicator.
		public method enable takes boolean enable returns nothing
			set this.elapsedTime = 0.0
			call EnableTrigger(this.refreshTrigger)
		endmethod

		public method disable takes nothing returns nothing
			call DisableTrigger(this.refreshTrigger)
			call this.reset()
		endmethod

		//private methods

		private method indicateImage takes nothing returns nothing
			call SetImageColorBJ(I2Image(H2I(this.object)), this.red, this.green, this.blue, this.transparency) //ALibraryGeneralConversion
			call TriggerSleepAction(this.rate / 2.0)
			call SetImageColorBJ(I2Image(H2I(this.object)), 100.0, 100.0, 100.0, 0.0) //ALibraryGeneralConversion
		endmethod

		private method indicateTextTag takes nothing returns nothing
			call SetTextTagColorBJ(I2TextTag(H2I(this.object)), this.red, this.green, this.blue, this.transparency) //ALibraryGeneralConversion
			call TriggerSleepAction(this.rate / 2.00)
			call SetTextTagColorBJ(I2TextTag(H2I(this.object)), 100.0, 100.0, 100.0, 0.0) //ALibraryGeneralConversion
		endmethod

		private method indicateUnit takes nothing returns nothing
			call SetUnitVertexColorBJ(I2Unit(H2I(this.object)), this.red, this.green, this.blue, this.transparency) //ALibraryGeneralConversion
			call TriggerSleepAction(this.rate / 2.00)
			call SetUnitVertexColorBJ(I2Unit(H2I(this.object)), 100.0, 100.0, 100.0, 0.0) //ALibraryGeneralConversion
		endmethod

		private method indicateObject takes nothing returns nothing
			if (this.objectType == AIndicator.objectTypeImage) then
				call this.indicateImage()
			elseif (this.objectType == AIndicator.objectTypeTextTag) then
				call this.indicateTextTag()
			else
				call this.indicateUnit()
			endif
		endmethod

		private method inrcreaseElapsedTime takes nothing returns nothing
			set this.elapsedTime = (this.elapsedTime + this.rate)
			if (this.elapsedTime >= this.time) then
				call this.enable(false)
			endif
		endmethod

		private static method triggerActionIndicateObject takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AIndicator indicator = AGetInterfaceHashTable().getHandleInteger(triggeringTrigger, "indicator") //AClassInterfaceInterfaceHashTable
			if (indicator.object != null) then
				call indicator.indicateObject()
				call indicator.inrcreaseElapsedTime()
			else
				call indicator.enable(false)
			endif
        		set triggeringTrigger = null
		endmethod

		private method reset takes nothing returns nothing
			if (this.objectType == AIndicator.objectTypeImage) then
				call SetImageColorBJ(I2Image(H2I(this.object)), 100.0, 100.0, 100.0, 0.0) //ALibraryGeneralConversion
			elseif (this.objectType == AIndicator.objectTypeTextTag) then
				call SetTextTagColorBJ(I2TextTag(H2I(this.object)), 100.0, 100.0, 100.0, 0.0) //ALibraryGeneralConversion
			elseif (this.objectType == AIndicator.objectTypeUnit) then
				call SetUnitVertexColorBJ(I2Unit(H2I(this.object)), 100.0, 100.0, 100.0, 0.0) //ALibraryGeneralConversion
			endif
		endmethod

		private method createRefreshTrigger takes nothing returns nothing
			local event triggerEvent
            		local triggeraction triggerAction
			set this.refreshTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterTimerEvent(this.refreshTrigger, this.rate, true)
			set triggerAction = TriggerAddAction(this.refreshTrigger, function AIndicator.triggerActionIndicateObject)
			call AGetInterfaceHashTable().storeHandleInteger(this.refreshTrigger, "indicator", this) //AClassInterfaceInterfaceHashTable
			set triggerEvent = null
			set triggerAction = null
		endmethod

		public static method create takes handle object, integer objectType, real rate, real time, real red, real green, real blue, real transparency returns AIndicator
			local AIndicator this = AIndicator.allocate()
			//dynamic members
			set this.object = object
			set this.objectType = objectType
			set this.rate = rate
			set this.time = time
			set this.red = red
			set this.green = green
			set this.blue = blue
			set this.transparency = transparency
			//members
			set this.elapsedTime = 0.00
			set this.firstTime = true

			call this.createRefreshTrigger()
			return this
		endmethod

		private method destroyRefreshTrigger takes nothing returns nothing
			call AGetInterfaceHashTable().destroyTrigger(this.refreshTrigger) //AClassInterfaceInterfaceHashTable
			set this.refreshTrigger = null
		endmethod

		public method onDestroy takes nothing returns nothing
			//dynamic members
			set this.object = null

			call this.destroyRefreshTrigger()
		endmethod
	endstruct

endlibrary