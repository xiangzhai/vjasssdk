library AStructSystemsWorldRoutine requires AStructCoreGeneralHashTable

	/// @todo Should be a part of @struct ARoutine, vJass bug.
	function interface ARoutineAction takes ARoutine routine returns nothing

	/// ARoute provides NPC routine handling like in the game Gothic or Gothic II.
	/// You are able to assign day times and routine actions using the function interface @functioninterface ARouteAction.
	/// Additionally you can make periodic routines by setting a 'run rate'.
	/// If the assigned unit is paused, routine won't be runned until unit gets unpaused.
	struct ARoutine
		//start members
		private unit m_unit
		private real m_startTimeOfDay
		private real m_endTimeOfDay
		private real m_runRate
		private ARoutineAction m_action
		//members
		private trigger m_runTrigger
		private trigger m_enableTrigger
		private trigger m_disableTrigger

		//start members

		//You will need this in the routine action
		public method unit takes nothing returns unit
			return this.m_unit
		endmethod
		
		public method startTimeOfDay takes nothing returns real
			return this.m_startTimeOfDay
		endmethod
		
		public method endTimeOfDay takes nothing returns real
			return this.m_endTimeOfDay
		endmethod
		
		public method runRate takes nothing returns real
			return this.m_runRate
		endmethod
		
		public method action takes nothing returns ARoutineAction
			return this.m_action
		endmethod

		//methods

		public method enable takes nothing returns nothing
			if (this.m_runRate > 0.0) then
				call EnableTrigger(this.m_runTrigger)
				call EnableTrigger(this.m_disableTrigger)
			endif
			call EnableTrigger(this.m_enableTrigger)
		endmethod

		public method disable takes nothing returns nothing
			if (this.m_runRate > 0.0) then
				call DisableTrigger(this.m_runTrigger)
				call DisableTrigger(this.m_disableTrigger)
			endif
			call DisableTrigger(this.m_enableTrigger)
		endmethod

		public method isInTime takes nothing returns boolean
			if (this.m_endTimeOfDay < this.m_startTimeOfDay) then //next day
				return GetFloatGameState(GAME_STATE_TIME_OF_DAY) >= this.m_startTimeOfDay or GetFloatGameState(GAME_STATE_TIME_OF_DAY) <= this.m_startTimeOfDay
			endif
			return GetFloatGameState(GAME_STATE_TIME_OF_DAY) >= this.m_startTimeOfDay and GetFloatGameState(GAME_STATE_TIME_OF_DAY) <= this.m_endTimeOfDay
		endmethod

		//Call PauseUnit if you want to stop the routine.
		public method isAbleToMove takes nothing returns boolean
			return not IsUnitPaused(this.m_unit)
		endmethod

		private method run takes nothing returns nothing
			call this.m_action.execute(this)
		endmethod
		
		private static method runCondition takes nothing returns boolean
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ARoutine this = AHashTable.global().getHandleInteger(triggeringTrigger, "this")
			local boolean result = this.isAbleToMove()
			set triggeringTrigger = null
			return result
		endmethod

		private static method runAction takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ARoutine this = AHashTable.global().getHandleInteger(triggeringTrigger, "this")
			call this.run()
			set triggeringTrigger = null
		endmethod

		private method createRunTrigger takes nothing returns nothing
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			if (this.m_runRate > 0.0) then
				set this.m_runTrigger = CreateTrigger()
				set triggerEvent = TriggerRegisterTimerEvent(this.m_runTrigger, this.m_runRate, true)
				set conditionFunction = Condition(function ARoutine.runCondition)
				set triggerCondition = TriggerAddCondition(this.m_runTrigger, conditionFunction)
				set triggerAction = TriggerAddAction(this.m_runTrigger, function ARoutine.runAction)
				call AHashTable.global().storeHandleInteger(this.m_runTrigger, "this", this)
				set triggerEvent = null
				set conditionFunction = null
				set triggerCondition = null
				set triggerAction = null
			endif
		endmethod

		private static method triggerActionEnable takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ARoutine this = AHashTable.global().getHandleInteger(triggeringTrigger, "this")
			if (this.m_runRate > 0.0) then
				call EnableTrigger(this.m_runTrigger)
			else
				call this.run()
			endif
			set triggeringTrigger = null
		endmethod

		private method createEnableTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_enableTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterGameStateEvent(this.m_enableTrigger, GAME_STATE_TIME_OF_DAY, EQUAL, this.m_startTimeOfDay)
			set triggerAction = TriggerAddAction(this.m_enableTrigger, function ARoutine.triggerActionEnable)
			call AHashTable.global().storeHandleInteger(this.m_enableTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		private static method triggerActionDisable takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ARoutine this = AHashTable.global().getHandleInteger(triggeringTrigger, "this")
			call DisableTrigger(this.m_runTrigger)
			set triggeringTrigger = null
		endmethod

		private method createDisableTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			if (this.m_runRate > 0.0) then
				set this.m_disableTrigger = CreateTrigger()
				set triggerEvent = TriggerRegisterGameStateEvent(this.m_disableTrigger, GAME_STATE_TIME_OF_DAY, EQUAL, this.m_endTimeOfDay)
				set triggerAction = TriggerAddAction(this.m_disableTrigger, function ARoutine.triggerActionDisable)
				call AHashTable.global().storeHandleInteger(this.m_disableTrigger, "this", this)
				set triggerEvent = null
				set triggerAction = null
			endif
		endmethod

		/// Usually it will be runned when the assigned time of day is reached.
		/// @param runRate If runRate is <= 0.0 action won't be runned at an interval.
		public static method create takes unit usedUnit, real startTimeOfDay, real endTimeOfDay, real runRate, ARoutineAction action returns ARoutine
			local ARoutine this = ARoutine.allocate()
			//start members
			set this.m_unit = usedUnit
			set this.m_startTimeOfDay = startTimeOfDay
			set this.m_endTimeOfDay = endTimeOfDay
			set this.m_runRate = runRate
			set this.m_action = action

			call this.createRunTrigger()
			call this.createEnableTrigger()
			call this.createDisableTrigger()
			return this
		endmethod

		private method destroyRunTrigger takes nothing returns nothing
			if (this.m_runRate > 0.0) then
				call AHashTable.global().destroyTrigger(this.m_runTrigger)
				set this.m_runTrigger = null
			endif
		endmethod

		private method destroyEnableTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_enableTrigger)
			set this.m_enableTrigger = null
		endmethod

		private method destroyDisableTrigger takes nothing returns nothing
			if (this.m_runRate > 0.0) then
				call AHashTable.global().destroyTrigger(this.m_disableTrigger)
				set this.m_disableTrigger = null
			endif
		endmethod

		public method onDestroy takes nothing returns nothing
			//start members
			set this.m_unit = null

			call this.destroyRunTrigger()
			call this.destroyEnableTrigger()
			call this.destroyDisableTrigger()
		endmethod
	endstruct

endlibrary