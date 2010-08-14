/// The fps common.j file is required.
/// @todo Do not use this library, it is unfinished!
library AStructSystemsCharacterFight requires optional ALibraryCoreDebugMisc, AStructCoreGeneralHashTable, AStructSystemsCharacterAbstractCharacterSystem, AStructSystemsCharacterCharacter

	struct AFight extends AAbstractCharacterSystem
		// static construction members
		private static boolean m_useFps
		private static string m_order
		private static real m_time
		private static string m_hitAnimation
		private static real m_angle
		// members
		private trigger m_fightTrigger
		private timer m_timer
		private boolean m_hasOrdered
		private boolean m_canOrder

		//! runtextmacro optional A_STRUCT_DEBUG("\"AFight\"")

		public method enable takes nothing returns nothing
			call super.enable()
			call EnableTrigger(this.m_fightTrigger)
			if (TimerGetRemaining(this.m_timer) > 0.0) then
				call PauseTimerBJ(false, this.m_timer)
			endif
		endmethod

		public method disable takes nothing returns nothing
			call super.disable()
			call DisableTrigger(this.m_fightTrigger)
			if (TimerGetRemaining(this.m_timer) > 0.0) then
				call PauseTimerBJ(true, this.m_timer)
			endif
		endmethod

		private method hit takes nothing returns nothing
			set this.m_hasOrdered = false
			set this.m_canOrder = false

			//call UnitDamagePoint(this.unit(), 0.0, unitPolarProjectionX(this.unit(), (GetUnitFacing(this.unit()) + thistype.angle), AFocus.range)

			set this.m_canOrder = true
		endmethod

		private static method timerFunctionReset takes nothing returns nothing
			local timer expiredTimer = GetExpiredTimer()
			local thistype this = AHashTable.global().handleInteger(expiredTimer, "this")
			set this.m_hasOrdered = false
			set expiredTimer = null
		endmethod

		private method startTimer takes nothing returns nothing
			set this.m_hasOrdered = true
			call TimerStart(this.m_timer, thistype.m_time, false, function thistype.timerFunctionReset)
			call AHashTable.global().setHandleInteger(this.m_timer, "this", this)
		endmethod

		private static method triggerConditionOrder takes nothing returns boolean
			return GetIssuedOrderId() == OrderId(thistype.m_order)
		endmethod

		private static method triggerActionOrder takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			if (this.m_canOrder) then
				if (this.m_hasOrdered) then
					call this.hit()
				else
					call this.startTimer()
				endif
			endif
			set triggeringTrigger = null
		endmethod

		private method createFightTrigger takes nothing returns nothing
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			if (not thistype.m_useFps) then
				set triggerEvent = TriggerRegisterUnitEvent(this.m_fightTrigger, this.character().unit(), EVENT_UNIT_ISSUED_ORDER)
				set conditionFunction = Condition(function thistype.triggerConditionOrder)
				set triggerCondition = TriggerAddCondition(this.m_fightTrigger, conditionFunction)
				set triggerEvent = null
			else
				//call TriggerRegisterMouseEvent(this.m_fightTrigger, EVENT_LMOUSEDOWN)
			endif
			set triggerAction = TriggerAddAction(this.m_fightTrigger, function thistype.triggerActionOrder)
			call AHashTable.global().setHandleInteger(this.m_fightTrigger, "this", this)
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		public static method create takes ACharacter character returns thistype
			local thistype this = thistype.allocate(character)
			//members
			set this.m_timer = CreateTimer()

			call this.createFightTrigger()
			return this
		endmethod

		private method destroyUsedTimer takes nothing returns nothing
			call AHashTable.global().destroyTimer(this.m_timer)
			set this.m_timer = null
		endmethod

		private method destroyFightTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_fightTrigger)
			set this.m_fightTrigger = null
		endmethod

		public method onDestroy takes nothing returns nothing

			call this.destroyUsedTimer()
			call this.destroyFightTrigger()
		endmethod

		/**
		* @param useFps false, new implementation, uses the left mouse key
		* @param order "move"
		* @param time 0.5
		* @param hitAnimation "attack"
		* @param angle 20.0
		*/
		public static method init takes boolean useFps, string order, real time, string hitAnimation, real angle returns nothing
			debug if (useFps and not Asl.useRtc()) then
				debug call thistype.staticPrint("FPS is enabled but RtC isn't.")
			debug endif
			//static start members
			set thistype.m_useFps = useFps
			set thistype.m_order = order
			set thistype.m_time = time
			set thistype.m_hitAnimation = hitAnimation
			set thistype.m_angle = angle
		endmethod
	endstruct

endlibrary