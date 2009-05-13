/// The fps common.j file is required.
/// Do not use this library, it is unfinished!
library AStructSystemsCharacterFight requires AStructSystemsCharacterCharacterHashTable, AStructSystemsCharacterAbstractCharacterSystem

	struct AFight extends AAbstractCharacterSystem
		//static start members
		private static boolean useFps
		private static string order
		private static real time
		private static string hitAnimation
		private static real angle
		//members
		private trigger fightTrigger
		private timer usedTimer
		private boolean hasOrdered
		private boolean canOrder

		public method enable takes nothing returns nothing
			call EnableTrigger(this.fightTrigger)
			if (TimerGetRemaining(this.usedTimer) > 0.0) then
				call PauseTimerBJ(false, this.usedTimer)
			endif
		endmethod

		public method disable takes nothing returns nothing
			call DisableTrigger(this.fightTrigger)
			if (TimerGetRemaining(this.usedTimer) > 0.0) then
				call PauseTimerBJ(true, this.usedTimer)
			endif
		endmethod

		private method hit takes nothing returns nothing
			set this.hasOrdered = false
			set this.canOrder = false

			//call UnitDamagePoint(this.getUnit(), 0.0, GetUnitPolarProjectionX(this.getUnit(), (GetUnitFacing(this.getUnit()) + AFight.angle), AFocus.range)

			set this.canOrder = true
		endmethod

		private static method timerFunctionReset takes nothing returns nothing
			local timer expiredTimer = GetExpiredTimer()
			local AFight fight = AGetCharacterHashTable().getHandleInteger(expiredTimer, "fight")
			set fight.hasOrdered = false
			set expiredTimer = null
		endmethod

		private method startTimer takes nothing returns nothing
			set this.hasOrdered = true
			call TimerStart(this.usedTimer, AFight.time, false, function AFight.timerFunctionReset)
			call AGetCharacterHashTable().storeHandleInteger(this.usedTimer, "fight", this)
		endmethod

		private static method triggerConditionOrder takes nothing returns boolean
			return GetIssuedOrderId() == OrderId(AFight.order)
		endmethod

		private static method triggerActionOrder takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AFight fight = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "fight")
			if (fight.canOrder) then
				if (fight.hasOrdered) then
					call fight.hit()
				else
					call fight.startTimer()
				endif
			endif
			set triggeringTrigger = null
		endmethod

		private method createFightTrigger takes nothing returns nothing
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			if (not AFight.useFps) then
				set triggerEvent = TriggerRegisterUnitEvent(this.fightTrigger, this.getUnit(), EVENT_UNIT_ISSUED_ORDER) //Return nothing
				set conditionFunction = Condition(function AFight.triggerConditionOrder)
				set triggerCondition = TriggerAddCondition(this.fightTrigger, conditionFunction)
				set triggerEvent = null
			else
				//call TriggerRegisterMouseEvent(this.fightTrigger, EVENT_LMOUSEDOWN) //fpscommon.j
			endif
			set triggerAction = TriggerAddAction(this.fightTrigger, function AFight.triggerActionOrder)
			call AGetCharacterHashTable().storeHandleInteger(this.fightTrigger, "fight", this) //AClassCharacterCharacterHashTable
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		public static method create takes ACharacter character returns AFight
			local AFight this = AFight.allocate(character)
			//members
			set this.usedTimer = CreateTimer()

			call this.createFightTrigger()
			return this
		endmethod

		private method destroyUsedTimer takes nothing returns nothing
			call AGetCharacterHashTable().destroyTimer(this.usedTimer)
			set this.usedTimer = null
		endmethod

		private method destroyFightTrigger takes nothing returns nothing
			call AGetCharacterHashTable().destroyTrigger(this.fightTrigger)
			set this.fightTrigger = null
		endmethod

		public method onDestroy takes nothing returns nothing

			call this.destroyUsedTimer()
			call this.destroyFightTrigger()
		endmethod

		/// @param useFps false, new implementation, uses the left mouse key
		/// @param order "move"
		/// @param time 0.5
		/// @param hitAnimation "attack"
		/// @param angle 20.0
		public static method init takes boolean useFps, string order, real time, string hitAnimation, real angle returns nothing
			//static start members
			set AFight.useFps = useFps
			set AFight.order = order
			set AFight.time = time
			set AFight.hitAnimation = hitAnimation
			set AFight.angle = angle
		endmethod
	endstruct

endlibrary