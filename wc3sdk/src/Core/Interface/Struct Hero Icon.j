library AStructCoreInterfaceHeroIcon requires ALibraryCoreDebugMisc, ALibraryCoreGeneralUnit, ALibraryCoreInterfaceCamera, AStructCoreEnvironmentUnitCopy, AStructCoreGeneralHashTable

	/**
	* This struct provides the feature to add custom hero icons.
	* Customly added hero icons can be used to refer to heroes of allied players if you have no
	* shared control.
	* There will be usual hero icons in the left of screen and all unit orders which have the icons
	* as targets will be forwarded to the actual hero.
	* For providing the hero icon internally an invulnerable, paused unit is created on map which is
	a nearly exact copy of the actual hero.
	*/
	struct AHeroIcon
		// static construction members
		private static real m_refreshTime
		// dynamic members
		private boolean m_recognizeAllianceChanges
		// members
		private AUnitCopy m_unitCopy
		private trigger m_selectionTrigger
		private trigger m_orderTrigger
		private trigger m_allianceTrigger

		//! runtextmacro A_STRUCT_DEBUG("\"AHeroIcon\"")

		// dynamic members

		/**
		* @param recognizeAllianceChanges This value is initially true if icon owner is not the same as unit owner. If this value is true hero icon will be disabled automatically when alliance is changed to shared control and unit is a hero that there aren't two hero icons.
		*/
		public method setRecognizeAllianceChanges takes boolean recognizeAllianceChanges returns nothing
			set this.m_recognizeAllianceChanges = recognizeAllianceChanges
		endmethod

		public method recognizeAllianceChanges takes nothing returns boolean
			return this.m_recognizeAllianceChanges
		endmethod

		// members

		public method unit takes nothing returns unit
			return this.m_unitCopy.realUnit()
		endmethod

		public method unitCopy takes nothing returns unit
			return this.m_unitCopy.unit()
		endmethod

		public method player takes nothing returns player
			return GetOwningPlayer(this.unit())
		endmethod

		public method playerCopy takes nothing returns player
			return GetOwningPlayer(this.unitCopy())
		endmethod

		public method isEnabled takes nothing returns boolean
			return not IsUnitHidden(this.unitCopy())
		endmethod

		public method enable takes nothing returns nothing
			call ShowUnit(this.unitCopy(), true)
			call this.m_unitCopy.enable()
			call EnableTrigger(this.m_selectionTrigger)
			call EnableTrigger(this.m_orderTrigger)
		endmethod

		public method disable takes nothing returns nothing
			call ShowUnit(this.unitCopy(), false)
			call this.m_unitCopy.disable()
			call DisableTrigger(this.m_selectionTrigger)
			call DisableTrigger(this.m_orderTrigger)
			// do never disable alliance trigger
		endmethod

		public method setEnabled takes boolean enabled returns nothing
			if (enabled) then
				call this.enable()
			else
				call this.disable()
			endif
		endmethod

		public stub method onSelect takes nothing returns nothing
			call SmartCameraPanWithZForPlayer(this.playerCopy(), GetUnitX(this.unit()), GetUnitY(this.unit()), 0.0, 0.0)
			call SelectUnitForPlayerSingle(this.unit(), this.playerCopy())
		endmethod

		public stub method onOrder takes nothing returns nothing
			call IssueImmediateOrder(GetTriggerUnit(), "stop")
			call IssueTargetOrderById(GetTriggerUnit(), GetIssuedOrderId(), this.unit())
		endmethod

		private static method triggerConditionSelection takes nothing returns boolean
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			return GetTriggerPlayer() == this.playerCopy()
		endmethod

		private static method triggerActionSelection takes nothing returns nothing
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			call this.onSelect.evaluate()
		endmethod

		private static method triggerConditionOrder takes nothing returns boolean
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			return GetOrderTarget() == this.unitCopy()
		endmethod

		private static method triggerActionOrder takes nothing returns nothing
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			call this.onOrder.evaluate()
		endmethod

		private static method triggerConditionAlliance takes nothing returns boolean
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			return IsUnitType(this.unitCopy(), UNIT_TYPE_HERO) and this.recognizeAllianceChanges()
		endmethod

		private static method triggerActionAlliance takes nothing returns nothing
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			call this.setEnabled(not GetPlayerAlliance(this.playerCopy(), this.player(), ALLIANCE_SHARED_CONTROL))
			debug call this.print("Changing alliance")
		endmethod

		public static method create takes unit whichUnit, player iconOwner, real x, real y, real facing returns thistype
			local thistype this = thistype.allocate()
			// dynamic members
			set this.m_recognizeAllianceChanges = not (GetOwningPlayer(whichUnit) == iconOwner)
			// members
			set this.m_unitCopy = AUnitCopy.create(whichUnit, thistype.m_refreshTime, x, y, facing)
			call this.m_unitCopy.setCopyVisibility(false)
			call this.m_unitCopy.setCopyPause(false)
			call this.m_unitCopy.setCopyVulnerbility(false)
			call this.m_unitCopy.setCopyDeath(false)
			//call ShowUnit(this.unit(), false) // starting enabled, has to be visible!
			call SetUnitOwner(this.unitCopy(), iconOwner, false)
			call SetUnitInvulnerable(this.unitCopy(), true)
			call SetUnitPathing(this.unitCopy(), false)
			call this.m_unitCopy.start()

			set this.m_selectionTrigger = CreateTrigger()
			call TriggerRegisterUnitEvent(this.m_selectionTrigger, this.unitCopy(), EVENT_UNIT_SELECTED)
			call TriggerAddCondition(this.m_selectionTrigger, Condition(function thistype.triggerConditionSelection))
			call TriggerAddAction(this.m_selectionTrigger, function thistype.triggerActionSelection)
			call AHashTable.global().setHandleInteger(this.m_selectionTrigger, "this", this)

			set this.m_orderTrigger = CreateTrigger()
			call TriggerRegisterAnyUnitEventBJ(this.m_orderTrigger, EVENT_PLAYER_UNIT_ISSUED_TARGET_ORDER)
			call TriggerAddCondition(this.m_orderTrigger, Condition(function thistype.triggerConditionOrder))
			call TriggerAddAction(this.m_orderTrigger, function thistype.triggerActionOrder)
			call AHashTable.global().setHandleInteger(this.m_orderTrigger, "this", this)

			set this.m_allianceTrigger = CreateTrigger()
			call TriggerRegisterPlayerAllianceChange(this.m_allianceTrigger, this.playerCopy(), ALLIANCE_SHARED_CONTROL)
			call TriggerAddCondition(this.m_allianceTrigger, Condition(function thistype.triggerConditionAlliance))
			call TriggerAddAction(this.m_allianceTrigger, function thistype.triggerActionAlliance)
			call AHashTable.global().setHandleInteger(this.m_allianceTrigger, "this", this)

			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			// members
			call this.m_unitCopy.destroy()
			call AHashTable.global().destroyTrigger(this.m_selectionTrigger)
			set this.m_selectionTrigger = null
			call AHashTable.global().destroyTrigger(this.m_orderTrigger)
			set this.m_orderTrigger = null
			call AHashTable.global().destroyTrigger(this.m_allianceTrigger)
			set this.m_allianceTrigger = null
		endmethod

		public static method init takes real refreshTime returns nothing
			// static construction members
			set thistype.m_refreshTime = refreshTime
		endmethod
	endstruct

endlibrary