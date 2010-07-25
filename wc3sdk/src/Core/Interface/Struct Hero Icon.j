library AStructCoreInterfaceHeroIcon requires ALibraryCoreDebugMisc, ALibraryCoreGeneralUnit, ALibraryCoreInterfaceCamera, AStructCoreEnvironmentUnitCopy, AStructCoreGeneralHashTable

	struct AHeroIcon
		// static construction members
		private static real m_refreshTime
		// construction members
		private unit m_realHero
		private unit m_hero
		// members
		private AUnitCopy m_unitCopy
		private trigger m_selectionTrigger
		private trigger m_orderTrigger

		//! runtextmacro A_STRUCT_DEBUG("\"AHeroIcon\"")

		public method player takes nothing returns player
			return GetOwningPlayer(this.m_unitCopy.unit())
		endmethod

		public method realHero takes nothing returns unit
			return this.m_unitCopy.realUnit()
		endmethod

		public method hero takes nothing returns unit
			return this.m_unitCopy.unit()
		endmethod

		public method isEnabled takes nothing returns boolean
			return not IsUnitHidden(this.hero())
		endmethod

		public method enable takes nothing returns nothing
			call ShowUnit(this.hero(), true)
			call this.m_unitCopy.enable()
			call EnableTrigger(this.m_selectionTrigger)
			call EnableTrigger(this.m_orderTrigger)
		endmethod

		public method disable takes nothing returns nothing
			call ShowUnit(this.hero(), false)
			call this.m_unitCopy.disable()
			call DisableTrigger(this.m_selectionTrigger)
			call DisableTrigger(this.m_orderTrigger)
		endmethod

		public stub method onSelect takes nothing returns nothing
			call SmartCameraPanWithZForPlayer(this.player(), GetUnitX(this.hero()), GetUnitY(this.hero()), 0.0, 0.0)
			call SelectUnitForPlayerSingle(this.hero(), this.player())
		endmethod

		public stub method onOrder takes nothing returns nothing
			call IssueImmediateOrder(GetTriggerUnit(), "stop")
			call IssueTargetOrderById(GetTriggerUnit(), GetIssuedOrderId(), this.hero())
		endmethod

		private static method triggerConditionSelection takes nothing returns boolean
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			return GetTriggerPlayer() == this.player()
		endmethod

		private static method triggerActionSelection takes nothing returns nothing
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			call this.onSelect.evaluate()
		endmethod

		private static method triggerConditionOrder takes nothing returns boolean
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			return GetOrderTarget() == this.hero()
		endmethod

		private static method triggerActionOrder takes nothing returns nothing
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			call this.onOrder.evaluate()
		endmethod

		public static method create takes unit hero, player iconOwner, real x, real y, real facing returns thistype
			local thistype this = thistype.allocate()
static if (DEBUG_MODE) then
			debug call this.print("Hero \"" + GetUnitName(hero) + "\" is not a hero.")
endif
			// members
			set this.m_unitCopy = AUnitCopy.create(hero, thistype.m_refreshTime, x, y, facing)
			call this.m_unitCopy.setCopyVisibility(false)
			call this.m_unitCopy.setCopyPause(false)
			call this.m_unitCopy.setCopyVulnerbility(false)
			call this.m_unitCopy.setCopyDeath(false)
			//call ShowUnit(this.hero(), false) // starting enabled, has to be visible!
			call SetUnitOwner(this.hero(), iconOwner, false)
			call SetUnitInvulnerable(this.hero(), true)
			call SetUnitPathing(this.hero(), false)
			call this.m_unitCopy.start()

			set this.m_selectionTrigger = CreateTrigger()
			call TriggerRegisterUnitEvent(this.m_selectionTrigger, this.hero(), EVENT_UNIT_SELECTED)
			call TriggerAddCondition(this.m_selectionTrigger, Condition(function thistype.triggerConditionSelection))
			call TriggerAddAction(this.m_selectionTrigger, function thistype.triggerActionSelection)
			call AHashTable.global().setHandleInteger(this.m_selectionTrigger, "this", this)
			set this.m_orderTrigger = CreateTrigger()
			call TriggerRegisterAnyUnitEventBJ(this.m_orderTrigger, EVENT_PLAYER_UNIT_ISSUED_TARGET_ORDER)
			call TriggerAddCondition(this.m_orderTrigger, Condition(function thistype.triggerConditionOrder))
			call TriggerAddAction(this.m_orderTrigger, function thistype.triggerActionOrder)
			call AHashTable.global().setHandleInteger(this.m_orderTrigger, "this", this)

			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			// members
			call this.m_unitCopy.destroy()
			call AHashTable.global().destroyTrigger(this.m_selectionTrigger)
			set this.m_selectionTrigger = null
			call AHashTable.global().destroyTrigger(this.m_orderTrigger)
			set this.m_orderTrigger = null
		endmethod

		public static method init takes real refreshTime returns nothing
			// static construction members
			set thistype.m_refreshTime = refreshTime
		endmethod
	endstruct

endlibrary