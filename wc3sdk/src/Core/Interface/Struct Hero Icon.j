library AStructCoreInterfaceHeroIcon requires ALibraryCoreDebugMisc, ALibraryCoreGeneralUnit, ALibraryCoreInterfaceCamera, AStructCoreEnvironmentUnitCopy, AStructCoreGeneralHashTable

	/**
	* This struct provides the feature to add custom hero icons.
	* Customly added hero icons can be used to refer to heroes of allied players if you have no
	* shared advanced control.
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
		// construction members
		private unit m_unit
		private player m_playerCopy
		private real m_x
		private real m_y
		private real m_facing
		// members
		private integer m_selectionCounter
		private unit m_copiedUnit
		private AUnitCopy m_unitCopy
		private trigger m_selectionTrigger
		private trigger m_orderTrigger
		private trigger m_allianceTrigger

		//! runtextmacro A_STRUCT_DEBUG("\"AHeroIcon\"")

		// dynamic members

		/**
		* @param recognizeAllianceChanges This value is initially true if icon owner is not the same as unit owner. If this value is true hero icon will be disabled automatically when alliance is changed to shared advaned control (ALLIANCE_SHARED_ADVANCED_CONTROL) and unit is a hero that there aren't two hero icons.
		*/
		public method setRecognizeAllianceChanges takes boolean recognizeAllianceChanges returns nothing
			set this.m_recognizeAllianceChanges = recognizeAllianceChanges
		endmethod

		public method recognizeAllianceChanges takes nothing returns boolean
			return this.m_recognizeAllianceChanges
		endmethod

		// construction members

		public method unit takes nothing returns unit
			return this.m_unit
		endmethod

		public method playerCopy takes nothing returns player
			return this.m_playerCopy
		endmethod

		public method x takes nothing returns real
			return this.m_x
		endmethod

		public method y takes nothing returns real
			return this.m_y
		endmethod

		public method facing takes nothing returns real
			return this.m_facing
		endmethod

		// members

		public method unitCopy takes nothing returns unit
			return this.m_copiedUnit
		endmethod

		public method player takes nothing returns player
			return GetOwningPlayer(this.unit())
		endmethod

		public method isEnabled takes nothing returns boolean
			return this.m_unitCopy != 0
		endmethod

		private method createUnitCopy takes nothing returns nothing
			set this.m_unitCopy = AUnitCopy.create(this.unit(), thistype.m_refreshTime, this.x(), this.y(), this.facing())
			set this.m_copiedUnit = this.m_unitCopy.unit()
			call this.m_unitCopy.setCopyVisibility(false)
			call this.m_unitCopy.setCopyPause(false)
			call this.m_unitCopy.setCopyVulnerbility(false)
			call this.m_unitCopy.setCopyDeath(false)
			//call ShowUnit(this.unit(), false) // starting enabled, has to be visible!
			call SetUnitOwner(this.unitCopy(), this.playerCopy(), false)
			//call SetUnitInvulnerable(this.unitCopy(), true) // has to be attackable!
			call SetUnitPathing(this.unitCopy(), false)
			call this.m_unitCopy.start()
		endmethod

		public method enable takes nothing returns nothing
			if (this.isEnabled()) then
				return
			endif
			call this.createUnitCopy() // we need to create since hero icon is not hidden when unit is
			call EnableTrigger(this.m_selectionTrigger)
			call EnableTrigger(this.m_orderTrigger)
		endmethod

		public method disable takes nothing returns nothing
			if (not this.isEnabled()) then
				return
			endif
			//call this.m_unitCopy.disable()
			//call ShowUnit(this.unitCopy(), false)
			call this.m_unitCopy.destroy() // we need to destroy since hero icon is not hidden when unit is
			set this.m_unitCopy = 0
			set this.m_copiedUnit = null
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
			call SelectUnitForPlayerSingle(this.unit(), this.playerCopy())
		endmethod

		public stub method onDoubleSelect takes nothing returns nothing
			call SmartCameraPanWithZForPlayer(this.playerCopy(), GetUnitX(this.unit()), GetUnitY(this.unit()), 0.0, 0.0)
		endmethod

		public stub method onOrder takes nothing returns nothing
			call IssueImmediateOrder(GetTriggerUnit(), "stop")
			call IssueTargetOrderById(GetTriggerUnit(), GetIssuedOrderId(), this.unit())
		endmethod

		private static method triggerConditionSelection takes nothing returns boolean
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			if (GetTriggerUnit() == this.unitCopy()) then
				return GetTriggerPlayer() == this.playerCopy()
			endif
			if (GetTriggerUnit() != this.unit()) then // unit is always selected automatically!
				set this.m_selectionCounter = 0 // selecting other unit, no double click!
			endif
			return false
		endmethod

		private static method triggerActionSelection takes nothing returns nothing
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			set this.m_selectionCounter = this.m_selectionCounter + 1
			call this.onSelect.evaluate()
			if (this.m_selectionCounter == 2) then
				set this.m_selectionCounter = 0
				call this.onDoubleSelect.evaluate()
			endif
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
			call this.setEnabled(not GetPlayerAlliance(this.player(), this.playerCopy(), ALLIANCE_SHARED_ADVANCED_CONTROL))
			debug call this.print("Changing alliance!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
		endmethod

		public static method create takes unit whichUnit, player iconOwner, real x, real y, real facing returns thistype
			local thistype this = thistype.allocate()
			// dynamic members
			set this.m_recognizeAllianceChanges = not (GetOwningPlayer(whichUnit) == iconOwner)

			// construction members
			set this.m_unit = whichUnit
			set this.m_playerCopy = iconOwner
			set this.m_x = x
			set this.m_y = y
			set this.m_facing = facing
			// members
			set this.m_selectionCounter = 0
			call this.createUnitCopy()

			set this.m_selectionTrigger = CreateTrigger()
			call TriggerRegisterAnyUnitEventBJ(this.m_selectionTrigger, EVENT_PLAYER_UNIT_SELECTED)
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

			if (this.m_recognizeAllianceChanges) then
				debug call this.print("Recognizing alliance changes!")
				call this.setEnabled(not GetPlayerAlliance(this.player(), this.playerCopy(), ALLIANCE_SHARED_ADVANCED_CONTROL))
				debug if (GetPlayerAlliance(this.player(), this.playerCopy(), ALLIANCE_SHARED_ADVANCED_CONTROL)) then
					debug call this.print("Has shared advanced control")
				debug else
					debug call this.print("Has not!")
				debug endif
			endif

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

		/**
		* @param refreshTime Hero icons do use a gobal refresh time.
		*/
		public static method init takes real refreshTime returns nothing
			// static construction members
			set thistype.m_refreshTime = refreshTime
		endmethod
	endstruct

endlibrary