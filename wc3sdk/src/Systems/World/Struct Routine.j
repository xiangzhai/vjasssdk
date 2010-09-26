library AStructSystemsWorldRoutine requires optional ALibraryCoreDebugMisc, AStructCoreGeneralHashTable, ALibraryCoreMathsUnit

	/*
	/// Isn't a method since it uses @function TriggerSleepAction.
	function moveNpcTo takes ARoutine routine, real x, real y returns boolean
		local unit npc = routine.unit()
		loop
			if (not routine.isInTime()) then
				set npc = null
				return false
			elseif (routine.isAbleToMove()) then
				call IssuePointOrder(npc, "move", x, y)
				exitwhen (true)
			endif
			call TriggerSleepAction(1.0)
		endloop
		set npc = null
		return routine.isInTime()
	endfunction

	/// Isn't a method since it uses @function TriggerSleepAction.
	function moveNpcToRectCenter takes ARoutine routine, rect targetRect returns boolean
		return moveNpcTo(routine, GetRectCenterX(targetRect), GetRectCenterY(targetRect))
	endfunction

	/// Isn't a method since it uses @function TriggerSleepAction.
	function waitForNewNpcRect takes ARoutine routine, rect newRect returns boolean
		local unit npc = routine.unit()
		loop
			exitwhen (RectContainsUnit(newRect, npc) or not routine.isInTime())
			call TriggerSleepAction(1.0)
		endloop
		set npc = null
		return routine.isInTime()
	endfunction

	/// Isn't a method since it uses @function TriggerSleepAction.
	function moveNpcToRectCenterAndWait takes ARoutine routine, rect targetRect returns boolean
		if (not moveNpcToRectCenter(routine, targetRect)) then
			return false
		endif
		if (not waitForNewNpcRect(routine, targetRect)) then
			return false
		endif
		return routine.isInTime()
	endfunction
	*/

	struct ARoutineData
		// construction members
		private ARoutineUnitData m_routineUnitData
		private real m_startTimeOfDay
		private real m_endTimeOfDay
		private rect m_targetRect
		// members
		private boolean m_isEnabled
		private trigger m_startTrigger
		private trigger m_endTrigger
		private trigger m_targetTrigger
		private region m_targetRegion

		//! runtextmacro optional A_STRUCT_DEBUG("\"ARoutineData\"")

		public method routineUnitData takes nothing returns ARoutineUnitData
			return this.m_routineUnitData
		endmethod

		public method startTimeOfDay takes nothing returns real
			return this.m_startTimeOfDay
		endmethod

		public method endTimeOfDay takes nothing returns real
			return this.m_endTimeOfDay
		endmethod

		public method targetRect takes nothing returns rect
			return this.m_targetRect
		endmethod

		// members

		public method isEnabled takes nothing returns boolean
			return this.m_isEnabled
		endmethod

		// methods

		public method enable takes nothing returns nothing
			set this.m_isEnabled = true
			call EnableTrigger(this.m_startTrigger)
			if (not IsUnitPaused(this.m_routineUnitData.unit.evaluate())) then
				call EnableTrigger(this.m_endTrigger)
				if (this.m_targetTrigger != null) then
					call EnableTrigger(this.m_targetTrigger)
				endif
			endif
		endmethod

		public method disable takes nothing returns nothing
			set this.m_isEnabled = false
			call DisableTrigger(this.m_startTrigger)
			if (not IsUnitPaused(this.m_routineUnitData.unit.evaluate())) then
				call DisableTrigger(this.m_endTrigger)
				if (this.m_targetTrigger != null) then
					call DisableTrigger(this.m_targetTrigger)
				endif
			endif
		endmethod

		public method setEnabled takes boolean enabled returns nothing
			if (enabled) then
				call this.enable()
			else
				call this.disable()
			endif
		endmethod

		public method isInTime takes nothing returns boolean
			if (this.m_endTimeOfDay < this.m_startTimeOfDay) then // next day
				return GetFloatGameState(GAME_STATE_TIME_OF_DAY) >= this.m_startTimeOfDay or GetFloatGameState(GAME_STATE_TIME_OF_DAY) <= this.m_startTimeOfDay
			endif
			return GetFloatGameState(GAME_STATE_TIME_OF_DAY) >= this.m_startTimeOfDay and GetFloatGameState(GAME_STATE_TIME_OF_DAY) <= this.m_endTimeOfDay
		endmethod

		private static method triggerConditionTarget takes nothing returns boolean
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			local unit triggerUnit = GetTriggerUnit()
			local boolean result = triggerUnit == this.m_routineUnitData.unit.evaluate()
			set triggeringTrigger = null
			set triggerUnit = null
			return result
		endmethod

		private static method triggerActionTarget takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			call this.destroyTargetTrigger.evaluate() // destroys this trigger
			if (this.m_routineUnitData.routine.evaluate().targetAction() != 0) then
				call this.m_routineUnitData.routine.evaluate().targetAction().execute(this)
			endif
			set triggeringTrigger = null
		endmethod

		private method createTargetTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_targetRegion = CreateRegion()
			call RegionAddRect(this.m_targetRegion, this.m_targetRect)
			set this.m_targetTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterEnterRegion(this.m_targetTrigger, this.m_targetRegion, null)
			call TriggerAddCondition(this.m_targetTrigger, Condition(function thistype.triggerConditionTarget))
			set triggerAction = TriggerAddAction(this.m_targetTrigger, function thistype.triggerActionTarget)
			call AHashTable.global().setHandleInteger(this.m_targetTrigger, "this", this)
			call IssueRectOrder(this.m_routineUnitData.unit(), "move", this.m_targetRect)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		private static method triggerActionStart takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			debug call this.print("Starting for unit " + GetUnitName(this.m_routineUnitData.unit.evaluate()))
			if (not IsUnitPaused(this.m_routineUnitData.unit.evaluate())) then
				if (thistype.unitHasNextRoutineData.evaluate(this.m_routineUnitData.unit.evaluate())) then
					call thistype.clearNextRoutineDataOfUnit.evaluate(this.m_routineUnitData.unit.evaluate())
				endif
				call thistype.setCurrentRoutineDataForUnit.evaluate(this.m_routineUnitData.unit.evaluate(), this)
				if (this.m_routineUnitData.routine.evaluate().startAction() != 0) then
					call this.m_routineUnitData.routine.evaluate().startAction().execute(this)
				endif
				if (this.m_routineUnitData.routine.evaluate().hasTarget()) then
					if (RectContainsUnit(this.m_targetRect, this.m_routineUnitData.unit.evaluate())) then
						if (this.m_routineUnitData.routine.evaluate().targetAction() != 0) then
							call this.m_routineUnitData.routine.evaluate().targetAction().execute(this)
						endif
					else
						call this.createTargetTrigger()
					endif
				endif
			else
				call thistype.setNextRoutineDataForUnit.evaluate(this.m_routineUnitData.unit.evaluate(), this)
			endif
			set triggeringTrigger = null
		endmethod

		private method createStartTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_startTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterGameStateEvent(this.m_startTrigger, GAME_STATE_TIME_OF_DAY, EQUAL, this.m_startTimeOfDay)
			set triggerAction = TriggerAddAction(this.m_startTrigger, function thistype.triggerActionStart)
			call AHashTable.global().setHandleInteger(this.m_startTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		private static method triggerActionEnd takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			call thistype.clearCurrentRoutineDataOfUnit.evaluate(this.m_routineUnitData.unit.evaluate())
			if (this.m_routineUnitData.routine.evaluate().hasTarget() and this.m_targetTrigger != null) then
				call this.destroyTargetTrigger.evaluate()
			endif
			if (this.m_routineUnitData.routine.evaluate().endAction() != 0) then
				call this.m_routineUnitData.routine.evaluate().endAction().execute(this)
			endif
			set triggeringTrigger = null
		endmethod

		private method createEndTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_endTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterGameStateEvent(this.m_endTrigger, GAME_STATE_TIME_OF_DAY, EQUAL, this.m_endTimeOfDay)
			set triggerAction = TriggerAddAction(this.m_endTrigger, function thistype.triggerActionEnd)
			call AHashTable.global().setHandleInteger(this.m_endTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		public static method create takes ARoutineUnitData routineUnitData, real startTimeOfDay, real endTimeOfDay, rect targetRect returns thistype
			local thistype this = thistype.allocate()
			// construction members
			set this.m_routineUnitData = routineUnitData
			set this.m_startTimeOfDay = startTimeOfDay
			set this.m_endTimeOfDay = endTimeOfDay
			set this.m_targetRect = targetRect
			// members
			set this.m_isEnabled = true
			set this.m_targetRegion = null

			call this.createStartTrigger()
			call this.createEndTrigger()

			return this
		endmethod

		private method destroyTargetTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_targetTrigger)
			set this.m_targetTrigger = null
			call RemoveRegion(this.m_targetRegion)
			set this.m_targetRegion = null
		endmethod

		public method onDestroy takes nothing returns nothing
			// construction members
			set this.m_targetRect = null
			// members
			call AHashTable.global().destroyTrigger(this.m_startTrigger)
			set this.m_startTrigger = null
			call AHashTable.global().destroyTrigger(this.m_endTrigger)
			set this.m_endTrigger = null
			if (this.m_targetTrigger != null) then
				call this.destroyTargetTrigger()
			endif
		endmethod

		public static method currentRoutineDataOfUnit takes unit whichUnit returns thistype
			return AHashTable.global().handleInteger(whichUnit, "ARoutineData:currentRoutineData")
		endmethod

		public static method unitHasCurrentRoutineData takes unit whichUnit returns boolean
			return AHashTable.global().hasHandleInteger(whichUnit, "ARoutineData:currentRoutineData")
		endmethod

		public static method nextRoutineDataOfUnit takes unit whichUnit returns thistype
			return AHashTable.global().handleInteger(whichUnit, "ARoutineData:nextRoutineData")
		endmethod

		public static method unitHasNextRoutineData takes unit whichUnit returns boolean
			return AHashTable.global().hasHandleInteger(whichUnit, "ARoutineData:nextRoutineData")
		endmethod

		public static method enableCurrentRoutineDataOfUnit takes unit whichUnit returns boolean
			local boolean result = thistype.unitHasCurrentRoutineData(whichUnit)
			if (result) then
				call thistype.currentRoutineDataOfUnit(whichUnit).enable()
			endif
			return result
		endmethod

		public static method disableCurrentRoutineDataOfUnit takes unit whichUnit returns boolean
			local boolean result = thistype.unitHasCurrentRoutineData(whichUnit)
			if (result) then
				call thistype.currentRoutineDataOfUnit(whichUnit).disable()
			endif
			return result
		endmethod

		public static method hookPauseUnit takes unit whichUnit, boolean flag returns nothing
			local thistype this

			if (not thistype.unitHasCurrentRoutineData(whichUnit)) then
				return
			endif

			set this = thistype.currentRoutineDataOfUnit(whichUnit)

			debug if (this == 0) then
				debug call thistype.staticPrint("Current routine unit data of unit " + GetUnitName(whichUnit) + " is 0.")
				debug return
			debug endif

			if (not this.m_isEnabled) then
				return
			endif

			// pause
			if (flag) then
				call DisableTrigger(this.m_endTrigger)
				if (this.m_targetTrigger != null) then
					call DisableTrigger(this.m_targetTrigger)
				endif
			// unpause
			else
				if (this.isInTime()) then
					if (this.m_routineUnitData.routine.evaluate().hasTarget()) then
						if (this.m_targetTrigger != null) then
							call EnableTrigger(this.m_endTrigger)
							if (this.m_targetTrigger != null) then
								call EnableTrigger(this.m_targetTrigger)
							endif
							call IssueRectOrder(this.m_routineUnitData.unit.evaluate(), "move", this.m_targetRect)
						elseif (this.m_routineUnitData.routine.evaluate().isLoop() and this.m_routineUnitData.routine.evaluate().targetAction() != 0) then
							call this.m_routineUnitData.routine.evaluate().targetAction().execute(this)
						endif
					endif
				// not in time
				else
					if (not thistype.unitHasNextRoutineData(this.m_routineUnitData.unit.evaluate())) then
						return
					endif

					set this = thistype.nextRoutineDataOfUnit(whichUnit)
					call thistype.clearNextRoutineDataOfUnit.evaluate(whichUnit)
					call thistype.setCurrentRoutineDataForUnit.evaluate(whichUnit, this)
					if (this.m_routineUnitData.routine.evaluate().startAction() != 0) then
						call this.m_routineUnitData.routine.evaluate().startAction().execute(this)
					endif
				endif
			endif
		endmethod

		public static method hookRemoveUnit takes unit whichUnit returns nothing
			call ARoutine.removeUnitFromAll.evaluate(whichUnit)
			if (thistype.unitHasCurrentRoutineData(whichUnit)) then
				call thistype.clearCurrentRoutineDataOfUnit.evaluate(whichUnit)
			endif
			if (thistype.unitHasNextRoutineData(whichUnit)) then
				call thistype.clearNextRoutineDataOfUnit.evaluate(whichUnit)
			endif
		endmethod

		private static method setCurrentRoutineDataForUnit takes unit whichUnit, thistype routineData returns nothing
			call AHashTable.global().setHandleInteger(whichUnit, "ARoutineData:currentRoutineData", routineData)
		endmethod

		private static method clearCurrentRoutineDataOfUnit takes unit whichUnit returns nothing
			call AHashTable.global().removeHandleInteger(whichUnit, "ARoutineData:currentRoutineData")
		endmethod

		private static method setNextRoutineDataForUnit takes unit whichUnit, thistype routineData returns nothing
			//don't check if there's already a value, just overwrite!
			call AHashTable.global().setHandleInteger(whichUnit, "ARoutineData:nextRoutineData", routineData)
		endmethod

		private static method clearNextRoutineDataOfUnit takes unit whichUnit returns nothing
			call AHashTable.global().removeHandleInteger(whichUnit, "ARoutineData:nextRoutineData")
		endmethod
	endstruct

	hook PauseUnit ARoutineData.hookPauseUnit
	hook RemoveUnit ARoutineData.hookRemoveUnit

	/**
	* Call this function in loop routine actions of type @functioninterface ARoutineAction to
	* continue action, otherwise it will be stopped!
	*/
	function AContinueRoutineLoop takes ARoutineData routineData, ARoutineAction routineAction returns nothing
		if (not IsUnitPaused(routineData.routineUnitData().unit.evaluate()) and routineData.isEnabled()) then
			call routineAction.execute(routineData)
		endif
		//otherwise cancel, routine loop action will be called automatically again when unit is unpaused and still in routine time
	endfunction

	struct ARoutineUnitData
		private ARoutine m_routine
		private unit m_unit
		private AIntegerList m_routineData

		public method routine takes nothing returns ARoutine
			return this.m_routine
		endmethod

		public method unit takes nothing returns unit
			return this.m_unit
		endmethod

		public method addTime takes real startTimeOfDay, real endTimeOfDay, rect targetRect returns ARoutineData
			local ARoutineData data = ARoutineData.create(this, startTimeOfDay, endTimeOfDay, targetRect)
			call this.m_routineData.pushBack(data)
			return data
		endmethod

		public method setTimeEnabled takes real startTimeOfDay, real endTimeOfDay, boolean enabled returns ARoutineData
			local AIntegerListIterator iterator = this.m_routineData.begin()
			local ARoutineData result = 0
			loop
				exitwhen (not iterator.isValid())
				if (ARoutineData(iterator.data()).startTimeOfDay() == startTimeOfDay and ARoutineData(iterator.data()).endTimeOfDay() == endTimeOfDay) then
					call ARoutineData(iterator.data()).setEnabled(enabled)
					set result = iterator.data()
					exitwhen (true)
				endif
				call iterator.next()
			endloop
			call iterator.destroy()
			return result
		endmethod

		/**
		* Enables or disables all unit routines instances.
		*/
		public method setEnabled takes boolean enabled returns nothing
			local AIntegerListIterator iterator = this.m_routineData.begin()
			loop
				exitwhen (not iterator.isValid())
				call ARoutineData(iterator.data()).setEnabled(enabled)
				call iterator.next()
			endloop
			call iterator.destroy()
		endmethod

		public method enable takes nothing returns nothing
			call this.setEnabled(true)
		endmethod

		public method disable takes nothing returns nothing
			call this.setEnabled(false)
		endmethod

		public static method create takes ARoutine routine, unit whichUnit returns thistype
			local thistype this = thistype.allocate()
			set this.m_routine = routine
			set this.m_unit = whichUnit
			set this.m_routineData = AIntegerList.create()
			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			set this.m_unit = null
			loop
				exitwhen (this.m_routineData.empty())
				call ARoutineData(this.m_routineData.back()).destroy()
				call this.m_routineData.popBack()
			endloop
			call this.m_routineData.destroy()
		endmethod
	endstruct

	/// @todo Should be a part of @struct ARoutine, vJass bug.
	function interface ARoutineAction takes ARoutineData routineData returns nothing

	/**
	* Provides NPC routine handling like in the game Gothic or Gothic II.
	* You are able to assign day times and routine actions using the function interface @functioninterface ARouteAction.
	* Additionally you can make periodic routines by setting a 'run rate'.
	* If the assigned unit is paused, routine won't be runned until unit gets unpaused.
	*/
	struct ARoutine
		// static members
		private static AIntegerVector m_routines
		// construction members
		private boolean m_hasTarget
		private boolean m_isLoop
		private ARoutineAction m_startAction
		private ARoutineAction m_endAction
		private ARoutineAction m_targetAction
		// members
		private AIntegerVector m_unitData
		private integer m_index

		//! runtextmacro A_STRUCT_DEBUG("\"ARoutine\"")

		// construction members

		public method hasTarget takes nothing returns boolean
			return this.m_hasTarget
		endmethod

		public method isLoop takes nothing returns boolean
			return this.m_isLoop
		endmethod

		public method startAction takes nothing returns ARoutineAction
			return this.m_startAction
		endmethod

		public method endAction takes nothing returns ARoutineAction
			return this.m_startAction
		endmethod

		public method targetAction takes nothing returns ARoutineAction
			return this.m_targetAction
		endmethod

		// methods

		public method unitIndex takes unit whichUnit returns integer
			local integer i = 0
			loop
				exitwhen (i == this.m_unitData.size())
				if (ARoutineUnitData(this.m_unitData[i]).unit() == whichUnit) then
					return i
				endif
				set i = i + 1
			endloop
			return -1
		endmethod

		public method unitRoutineUnitData takes unit whichUnit returns ARoutineUnitData
			local integer index = this.unitIndex(whichUnit)
			if (index == -1) then
				return 0
			endif
			return ARoutineUnitData(this.m_unitData[index])
		endmethod

		public method hasUnit takes unit whichUnit returns boolean
			return this.unitIndex(whichUnit) != -1
		endmethod
		/*
		public method isEnabledForUnit takes unit whichUnit returns boolean
			local integer i = 0
			debug if (not this.hasUnit(whichUnit)) then
				debug call this.printMethodError("isEnabledForUnit", "Unit " + GetUnitName(whichUnit) + " does not use this routine.")
				debug return false
			debug endif
			loop
				exitwhen (i == this.m_unitData.size())
				if (ARoutineUnitData(this.m_unitData[i]).unit() == whichUnit) then
					return ARoutineUnitData(this.m_unitData[i]).isEnabled()
				endif
				set i = i + 1
			endloop
			return false
		endmethod

		public method isInTimeForUnit takes unit whichUnit returns boolean
			local integer i = 0
			debug if (not this.hasUnit(whichUnit)) then
				debug call this.printMethodError("isInTimeForUnit", "Unit " + GetUnitName(whichUnit) + " does not use this routine.")
				debug return false
			debug endif
			loop
				exitwhen (i == this.m_unitData.size())
				if (ARoutineUnitData(this.m_unitData[i]).unit() == whichUnit) then
					return ARoutineUnitData(this.m_unitData[i]).isInTime()
				endif
				set i = i + 1
			endloop
			return false
		endmethod
		*/

		public method addUnit takes unit whichUnit returns integer
			if (this.hasUnit(whichUnit)) then
				return -1
			endif
			call this.m_unitData.pushBack(ARoutineUnitData.create(this, whichUnit))
			return this.m_unitData.backIndex()
		endmethod

		/**
		* @return Returns index of routine unit data. If unit has not been added yet, method returns -1.
		*/
		public method addUnitTime takes unit whichUnit, real startTimeOfDay, real endTimeOfDay, rect targetRect returns integer
			local integer index = this.unitIndex(whichUnit)
			if (index == -1) then
				set index = this.addUnit(whichUnit)
			endif
			call ARoutineUnitData(this.m_unitData[index]).addTime(startTimeOfDay, endTimeOfDay, targetRect)
			return index
		endmethod

		public method setEnabledForUnitByIndex takes integer index, boolean enabled returns nothing
			call ARoutineUnitData(this.m_unitData[index]).setEnabled(enabled)
		endmethod

		public method setEnabledForUnit takes unit whichUnit, boolean enabled returns integer
			local integer i = 0
			loop
				exitwhen (i == this.m_unitData.size())
				if (ARoutineUnitData(this.m_unitData[i]).unit() == whichUnit) then
					call this.setEnabledForUnitByIndex(i, enabled)
					return i
				endif
				set i = i + 1
			endloop
			return -1
		endmethod

		public method enableForUnitByIndex takes integer index returns nothing
			call this.setEnabledForUnitByIndex(index, true)
		endmethod

		public method enableForUnit takes unit whichUnit returns integer
			return this.setEnabledForUnit(whichUnit, true)
		endmethod

		public method disableForUnitByIndex takes integer index returns nothing
			call this.setEnabledForUnitByIndex(index, false)
		endmethod

		public method disableForUnit takes unit whichUnit returns integer
			return this.setEnabledForUnit(whichUnit, false)
		endmethod

		public method setTimeEnabledForUnitByIndex takes integer index, real startTimeOfDay, real endTimeOfDay, boolean enabled returns nothing
			call ARoutineUnitData(this.m_unitData[index]).setTimeEnabled(startTimeOfDay, endTimeOfDay, enabled)
		endmethod

		public method setTimeEnabledForUnit takes unit whichUnit, real startTimeOfDay, real endTimeOfDay, boolean enabled returns integer
			local integer i = 0
			loop
				exitwhen (i == this.m_unitData.size())
				if (ARoutineUnitData(this.m_unitData[i]).unit() == whichUnit) then
					call this.setTimeEnabledForUnitByIndex(i, startTimeOfDay, endTimeOfDay, enabled)
					return i
				endif
				set i = i + 1
			endloop
			return -1
		endmethod

		public method enableTimeForUnitByIndex takes integer index, real startTimeOfDay, real endTimeOfDay returns nothing
			call this.setTimeEnabledForUnitByIndex(index, startTimeOfDay, endTimeOfDay, true)
		endmethod

		public method enableTimeForUnit takes unit whichUnit, real startTimeOfDay, real endTimeOfDay returns integer
			return this.setTimeEnabledForUnit(whichUnit, startTimeOfDay, endTimeOfDay, true)
		endmethod

		public method disableTimeForUnitByIndex takes integer index, real startTimeOfDay, real endTimeOfDay returns nothing
			call this.setTimeEnabledForUnitByIndex(index, startTimeOfDay, endTimeOfDay, false)
		endmethod

		public method disableTimeForUnit takes unit whichUnit, real startTimeOfDay, real endTimeOfDay returns integer
			return this.setTimeEnabledForUnit(whichUnit, startTimeOfDay, endTimeOfDay, false)
		endmethod

		public method removeUnitByIndex takes integer index returns nothing
			call ARoutineUnitData(this.m_unitData[index]).destroy()
			call this.m_unitData.erase(index)
		endmethod

		public method removeUnit takes unit whichUnit returns integer
			local integer i = 0
			loop
				exitwhen (i == this.m_unitData.size())
				if (ARoutineUnitData(this.m_unitData[i]).unit() == whichUnit) then
					call this.removeUnitByIndex(i)
					return i
				endif
				set i = i + 1
			endloop
			return -1
		endmethod

		public static method create takes boolean hasTarget, boolean isLoop, ARoutineAction startAction, ARoutineAction endAction, ARoutineAction targetAction returns thistype
			local thistype this = thistype.allocate()
			// construction members
			set this.m_hasTarget = hasTarget
			set this.m_isLoop = isLoop
			set this.m_startAction = startAction
			set this.m_endAction = endAction
			set this.m_targetAction = targetAction
			// members
			set this.m_unitData = AIntegerVector.create()
			call thistype.m_routines.pushBack(this)
			set this.m_index = thistype.m_routines.backIndex()

			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			// members
			loop
				exitwhen (this.m_unitData.empty())
				call this.removeUnitByIndex(this.m_unitData.backIndex())
			endloop
			call this.m_unitData.destroy()
			call thistype.m_routines.erase(this.m_index)
		endmethod

		public static method init takes nothing returns nothing
			// static members
			set thistype.m_routines = AIntegerVector.create()
		endmethod

		public static method enableForUnitInAll takes unit whichUnit returns nothing
			local integer i = 0
			loop
				exitwhen (i == thistype.m_routines.size())
				call thistype(thistype.m_routines[i]).enableForUnit(whichUnit)
				set i = i + 1
			endloop
		endmethod

		public static method disableForUnitInAll takes unit whichUnit returns nothing
			local integer i = 0
			loop
				exitwhen (i == thistype.m_routines.size())
				call thistype(thistype.m_routines[i]).disableForUnit(whichUnit)
				set i = i + 1
			endloop
		endmethod

		public static method removeUnitFromAll takes unit whichUnit returns nothing
			local integer i = 0
			loop
				exitwhen (i == thistype.m_routines.size())
				call thistype(thistype.m_routines[i]).removeUnit(whichUnit)
				set i = i + 1
			endloop
		endmethod
	endstruct

endlibrary