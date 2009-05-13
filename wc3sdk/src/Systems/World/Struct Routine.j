/// @todo Untestet!
library AStructSystemsWorldRoutine requires AStructSystemsWorldWorldHashTable

	/// @todo Should be a part of @struct ARoutine, vJass bug.
	function interface ARoutineAction takes ARoutine routine returns nothing

	/// ARoute provides NPC routine handling like in the game Gothic or Gothic II.
	/// You are able to assign day times and routine actions using the function interface @functioninterface ARouteAction.
	/// Additionally you can make periodic routines by setting a 'run rate'.
	/// If the assigned unit is paused, routine won't be runned until unit gets unpaused.
	struct ARoutine
		//start members
		private unit usedUnit
		private real startTimeOfDay
		private real endTimeOfDay
		private real runRate
		private ARoutineAction routineAction
		//members
		private trigger runTrigger
		private trigger enableTrigger
		private trigger disableTrigger

		//start members

		//You will need this in the routine action
		public method getUsedUnit takes nothing returns unit
			return this.usedUnit
		endmethod

		//methods

		public method enable takes nothing returns nothing
			if (this.runRate > 0.0) then
				call EnableTrigger(this.runTrigger)
				call EnableTrigger(this.disableTrigger)
			endif
			call EnableTrigger(this.enableTrigger)
		endmethod

		public method disable takes nothing returns nothing
			if (this.runRate > 0.0) then
				call DisableTrigger(this.runTrigger)
				call DisableTrigger(this.disableTrigger)
			endif
			call DisableTrigger(this.enableTrigger)
		endmethod

		public method isInTime takes nothing returns boolean
			return GetFloatGameState(GAME_STATE_TIME_OF_DAY) <= this.endTimeOfDay //n�chsten Tag eintragen, wenn Endzeit kleiner oder gleich der Startzeit ist
		endmethod

		//Call PauseUnit if you want to stop the routine.
		public method isAbleToMove takes nothing returns boolean
			return not IsUnitPaused(this.usedUnit)
		endmethod

		private method run takes nothing returns nothing
			call this.routineAction.execute(this)
		endmethod
		
		private static method runCondition takes nothing returns boolean
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ARoutine routine = AGetWorldHashTable().getHandleInteger(triggeringTrigger, "routine") //AStructWorldWorldHashTable
			local boolean result = routine.isAbleToMove()
			set triggeringTrigger = null
			return result
		endmethod

		private static method runAction takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ARoutine routine = AGetWorldHashTable().getHandleInteger(triggeringTrigger, "routine") //AStructWorldWorldHashTable
			call routine.run()
			set triggeringTrigger = null
		endmethod

		private method createRunTrigger takes nothing returns nothing
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			if (this.runRate > 0.0) then
				set this.runTrigger = CreateTrigger()
				set triggerEvent = TriggerRegisterTimerEvent(this.runTrigger, this.runRate, true)
				set conditionFunction = Condition(function ARoutine.runCondition)
				set triggerCondition = TriggerAddCondition(this.runTrigger, conditionFunction)
				set triggerAction = TriggerAddAction(this.runTrigger, function ARoutine.runAction)
				call AGetWorldHashTable().storeHandleInteger(this.runTrigger, "routine", this) //AStructWorldWorldHashTable
				set triggerEvent = null
				set conditionFunction = null
				set triggerCondition = null
				set triggerAction = null
			endif
		endmethod

		private static method triggerActionEnable takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ARoutine routine = AGetWorldHashTable().getHandleInteger(triggeringTrigger, "routine") //AStructWorldWorldHashTable
			if (routine.runRate > 0.0) then
				call EnableTrigger(routine.runTrigger)
			else
				call routine.run()
			endif
			set triggeringTrigger = null
		endmethod

		private method createEnableTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.enableTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterGameStateEvent(this.enableTrigger, GAME_STATE_TIME_OF_DAY, EQUAL, this.startTimeOfDay)
			set triggerAction = TriggerAddAction(this.enableTrigger, function ARoutine.triggerActionEnable)
			call AGetWorldHashTable().storeHandleInteger(this.enableTrigger, "routine", this) //AStructWorldWorldHashTable
			set triggerEvent = null
			set triggerAction = null
		endmethod

		private static method triggerActionDisable takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ARoutine routine = AGetWorldHashTable().getHandleInteger(triggeringTrigger, "routine") //AStructWorldWorldHashTable
			call DisableTrigger(routine.runTrigger)
			set triggeringTrigger = null
		endmethod

		private method createDisableTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			if (this.runRate > 0.0) then
				set this.disableTrigger = CreateTrigger()
				set triggerEvent = TriggerRegisterGameStateEvent(this.disableTrigger, GAME_STATE_TIME_OF_DAY, EQUAL, this.endTimeOfDay)
				set triggerAction = TriggerAddAction(this.disableTrigger, function ARoutine.triggerActionDisable)
				call AGetWorldHashTable().storeHandleInteger(this.disableTrigger, "routine", this) //AStructWorldWorldHashTable
				set triggerEvent = null
				set triggerAction = null
			endif
		endmethod

		/// Usually it will be runned when the assigned time of day is reached.
		/// @param runRate If runRate is <= 0.0 action won't be runned at an interval.
		public static method create takes unit usedUnit, real startTimeOfDay, real endTimeOfDay, real runRate, ARoutineAction routineAction returns ARoutine
			local ARoutine this = ARoutine.allocate()
			//start members
			set this.usedUnit = usedUnit
			set this.startTimeOfDay = startTimeOfDay
			set this.endTimeOfDay = endTimeOfDay
			set this.runRate = runRate
			set this.routineAction = routineAction

			call this.createRunTrigger()
			call this.createEnableTrigger()
			call this.createDisableTrigger()
			return this
		endmethod

		private method destroyRunTrigger takes nothing returns nothing
			if (this.runRate > 0.0) then
				call AGetWorldHashTable().destroyTrigger(this.runTrigger) //AStructWorldWorldHashTable
				set this.runTrigger = null
			endif
		endmethod

		private method destroyEnableTrigger takes nothing returns nothing
			call AGetWorldHashTable().destroyTrigger(this.enableTrigger) //AStructWorldWorldHashTable
			set this.enableTrigger = null
		endmethod

		private method destroyDisableTrigger takes nothing returns nothing
			if (this.runRate > 0.0) then
				call AGetWorldHashTable().destroyTrigger(this.disableTrigger) //AStructWorldWorldHashTable
				set this.disableTrigger = null
			endif
		endmethod

		public method onDestroy takes nothing returns nothing
			//start members
			set this.usedUnit = null

			call this.destroyRunTrigger()
			call this.destroyEnableTrigger()
			call this.destroyDisableTrigger()
		endmethod
	endstruct

endlibrary