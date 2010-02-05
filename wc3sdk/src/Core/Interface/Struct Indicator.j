library AStructCoreInterfaceIndicator requires optional ALibraryCoreDebugMisc, AStructCoreGeneralHashTable

	/// @todo Some dynamic members could not be dynamic!
	/// @todo Implement as abstract struct and add child structs with various object types.
	struct AIndicator
		//static constant members
		public static constant integer objectTypeImage = 0
		public static constant integer objectTypeTextTag = 1
		public static constant integer objectTypeUnit = 2
		//dynamic members
		private handle m_object
		private integer m_objectType
		private real m_rate //test if there are errors because it is dynamic
		private real m_time //here it's the same
		private real m_red //we are using BJ functions so we're calculating with per cents
		private real m_green
		private real m_blue
		private real m_transparency
		//members
		private trigger m_refreshTrigger //= null
		private real m_elapsedTime //= 0.00 //Vergangene Zeit
		private boolean m_firstTime //= true

		//! runtextmacro optional A_STRUCT_DEBUG("\"AIndicator\"")

		//dynamic member methods

		public method setObject takes handle object returns nothing
			set this.m_object = object
		endmethod

		public method object takes nothing returns handle
			return this.m_object
		endmethod

		public method setObjectType takes integer objectType returns nothing
			set this.m_objectType = objectType
		endmethod

		public method objectType takes nothing returns integer
			return this.m_objectType
		endmethod

		public method setRate takes real rate returns nothing
			set this.m_rate = rate
		endmethod

		public method rate takes nothing returns real
			return this.m_rate
		endmethod

		public method setTime takes real time returns nothing
			set this.m_time = time
		endmethod

		public method time takes nothing returns real
			return this.m_time
		endmethod

		public method setRed takes real red returns nothing
			set this.m_red = red
		endmethod

		public method red takes nothing returns real
			return this.m_red
		endmethod

		public method setGreen takes real green returns nothing
			set this.m_green = green
		endmethod

		public method green takes nothing returns real
			return this.m_green
		endmethod

		public method setBlue takes real blue returns nothing
			set this.m_blue = blue
		endmethod

		public method blue takes nothing returns real
			return this.m_blue
		endmethod

		public method setTransparency takes real transparency returns nothing
			set this.m_transparency = transparency
		endmethod

		public method transparency takes nothing returns real
			return this.m_transparency
		endmethod

		//public methods

		/// Flash restarts when enabling the indicator.
		public method enable takes boolean enable returns nothing
			set this.m_elapsedTime = 0.0
			call EnableTrigger(this.m_refreshTrigger)
		endmethod

		public method disable takes nothing returns nothing
			call DisableTrigger(this.m_refreshTrigger)
			call this.reset.evaluate()
		endmethod

		//private methods

		private method indicateImage takes nothing returns nothing
			//call SetImageColorBJ(H2Image(this.m_object), this.m_red, this.m_green, this.m_blue, this.m_transparency)
			call TriggerSleepAction(this.m_rate / 2.0)
			//call SetImageColorBJ(H2Image(this.m_object), 100.0, 100.0, 100.0, 0.0)
		endmethod

		private method indicateTextTag takes nothing returns nothing
			//call SetTextTagColorBJ(H2TextTag(this.m_object), this.m_red, this.m_green, this.m_blue, this.m_transparency)
			call TriggerSleepAction(this.m_rate / 2.00)
			//call SetTextTagColorBJ(H2TextTag(this.m_object), 100.0, 100.0, 100.0, 0.0)
		endmethod

		private method indicateUnit takes nothing returns nothing
			//call SetUnitVertexColorBJ(H2Unit(this.m_object), this.m_red, this.m_green, this.m_blue, this.m_transparency)
			call TriggerSleepAction(this.m_rate / 2.00)
			//call SetUnitVertexColorBJ(H2Unit(this.m_object), 100.0, 100.0, 100.0, 0.0)
		endmethod

		private method indicateObject takes nothing returns nothing
			if (this.m_objectType == AIndicator.objectTypeImage) then
				call this.indicateImage()
			elseif (this.m_objectType == AIndicator.objectTypeTextTag) then
				call this.indicateTextTag()
			elseif (this.m_objectType == AIndicator.objectTypeUnit) then
				call this.indicateUnit()
			debug else
				debug call this.print("Invalid object type " + I2S(this.m_objectType) + ".")
			endif
		endmethod

		private method inrcreaseElapsedTime takes nothing returns nothing
			set this.m_elapsedTime = this.m_elapsedTime + this.m_rate
			if (this.m_elapsedTime >= this.m_time) then
				call this.enable(false)
			endif
		endmethod

		private static method triggerActionIndicateObject takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AIndicator this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			if (this.m_object != null) then
				call this.indicateObject()
				call this.inrcreaseElapsedTime()
			else
				call this.enable(false)
			endif
			set triggeringTrigger = null
		endmethod

		private method reset takes nothing returns nothing
			if (this.m_objectType == AIndicator.objectTypeImage) then
				//call SetImageColorBJ(this.m_object, 100.0, 100.0, 100.0, 0.0)
			elseif (this.m_objectType == AIndicator.objectTypeTextTag) then
				//call SetTextTagColorBJ(this.m_object, 100.0, 100.0, 100.0, 0.0)
			elseif (this.m_objectType == AIndicator.objectTypeUnit) then
				//call SetUnitVertexColorBJ(this.m_object, 100.0, 100.0, 100.0, 0.0)
			endif
		endmethod

		private method createRefreshTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_refreshTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterTimerEvent(this.m_refreshTrigger, this.m_rate, true)
			set triggerAction = TriggerAddAction(this.m_refreshTrigger, function AIndicator.triggerActionIndicateObject)
			call AHashTable.global().setHandleInteger(this.m_refreshTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		public static method create takes handle object, integer objectType, real rate, real time, real red, real green, real blue, real transparency returns AIndicator
			local AIndicator this = AIndicator.allocate()
			//dynamic members
			set this.m_object = object
			set this.m_objectType = objectType
			set this.m_rate = rate
			set this.m_time = time
			set this.m_red = red
			set this.m_green = green
			set this.m_blue = blue
			set this.m_transparency = transparency
			//members
			set this.m_elapsedTime = 0.00
			set this.m_firstTime = true

			call this.createRefreshTrigger()
			return this
		endmethod

		private method destroyRefreshTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_refreshTrigger)
			set this.m_refreshTrigger = null
		endmethod

		public method onDestroy takes nothing returns nothing
			//dynamic members
			set this.m_object = null

			call this.destroyRefreshTrigger()
		endmethod
	endstruct

endlibrary