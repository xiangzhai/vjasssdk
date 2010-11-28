library AStructSystemsCharacterTalk requires optional AModuleCoreSystemStruct, ALibraryCoreDebugMisc, AStructCoreGeneralHashTable, AStructCoreGeneralList, AStructCoreGeneralVector, ALibraryCoreInterfaceMisc, ALibraryCoreMathsHandle, AStructSystemsCharacterCharacter

	/// @todo Should be a part of @struct ATalk, vJass bug.
	function interface ATalkStartAction takes ATalk talk returns nothing

	/**
	* Talks are a kind of dialogs with NPCs which are implemented by using the Warcraft 3 dialog natives.
	* This means that choices in the form of dialog buttons are shown to a character owner.
	* If the owner presses any button an user-defined function will be called where the user can define the whole talk.
	* One talk contains one or several infos (@struct AInfo). These infos contain the user-defined function and an user-defined condition.
	* Besides they can have various other properties.
	* Note that only one character owner can use one talk at the same time. There is no support for several character owners talking to the same NPC, yet.
	*/
	struct ATalk
		// static construction members
		private static string m_order
		private static real m_maxDistance
		private static string m_effectPath
		private static boolean m_hideUserInterface
		private static boolean m_disableEffectsInCinematicMode
		private static string m_textErrorMessage
		private static string m_textExit
		private static string m_textBack
		// static members
		private static AIntegerList m_talks
		// construction members
		private unit m_unit
		private ATalkStartAction m_startAction
		// members
		private AIntegerVector m_infos
		private ACharacter m_character
		private boolean m_isEnabled
		private trigger m_orderTrigger
		private effect m_effect

		implement ASystemStruct

		//! runtextmacro optional A_STRUCT_DEBUG("\"ATalk\"")

		// construction members

		/// @return Returns the NPC's unit.
		public method unit takes nothing returns unit
			return this.m_unit
		endmethod

		public method startAction takes nothing returns ATalkStartAction
			return this.m_startAction
		endmethod

		//members

		/// @return Returns the character which is talking currently to the NPC.
		public method character takes nothing returns ACharacter
			return this.m_character
		endmethod

		public method isEnabled takes nothing returns boolean
			return this.m_isEnabled
		endmethod

		//methods

		public method showStartPage takes nothing returns nothing
			call this.m_startAction.execute(this) //create buttons
		endmethod

		/**
		* Shows the talk's Warcraft 3 dialog.
		* If no info has been added start page is shown automatically.
		*/
		public method show takes nothing returns nothing
			if (AGui.playerGui(this.m_character.player()).dialog().dialogButtons() == 0) then
				call this.showStartPage()
			else
				call AGui.playerGui(this.m_character.player()).dialog().show()
			endif
		endmethod

		/**
		* Shows a range of infos and a Warcraft 3 dialog.
		*/
		public method showRange takes integer first, integer last returns nothing
			local integer i = first
static if (DEBUG_MODE) then
			if (first < 0 or first >= this.m_infos.size()) then
				call this.print("Wrong first value " + I2S(first) + ".")
			endif
			if (last < 0 or last >= this.m_infos.size()) then
				call this.print("Wrong last value " + I2S(last) + ".")
			endif
endif
			loop
				exitwhen (i > last)
				call AInfo(this.m_infos[i]).show.evaluate()
				set i = i + 1
			endloop
			call this.show()
		endmethod

		public method showUntil takes integer last returns nothing
			call this.showRange(0, last)
		endmethod

		public method showAll takes nothing returns nothing
			call this.showUntil(this.m_infos.backIndex())
		endmethod

		public method hide takes nothing returns nothing
			call AGui.playerGui(this.m_character.player()).dialog().hide()
		endmethod

		/**
		* You do not have to clear the dialog anywhere since @struct AInfo clears it whenever an info is runned!
		*/
		public method clear takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.m_infos.size())
				call AInfo(this.m_infos[i]).hide.evaluate()
				set i = i + 1
			endloop
			call AGui.playerGui(this.m_character.player()).dialog().clear()
		endmethod

		public method addInfo takes boolean permanent, boolean important, AInfoCondition condition, AInfoAction action, string description returns AInfo
			return AInfo.create.evaluate(this, permanent, important, condition, action, description)
		endmethod

		public method addBackButton takes AInfoAction action returns AInfo
			return AInfo.create.evaluate(this, true, false, 0, action, thistype.m_textBack)
		endmethod

		private static method infoActionBackToStartPage takes AInfo info returns nothing
			call info.talk.evaluate().showStartPage()
		endmethod

		public method addBackToStartPageButton takes nothing returns AInfo
			return AInfo.create.evaluate(this, true, false, 0, thistype.infoActionBackToStartPage, thistype.m_textBack)
		endmethod

		/// @returns Returns if info with index @param index has already been shown to the character which is talking currently to the NPC.
		public method infoHasBeenShown takes integer index returns boolean
			return AInfo(this.m_infos[index]).hasBeenShownToCharacter.evaluate(this.m_character.userId())
		endmethod

		public method infoHasBeenShownToCharacter takes integer index, ACharacter character returns boolean
			return AInfo(this.m_infos[index]).hasBeenShownToCharacter.evaluate(character.userId())
		endmethod

		public method isOpen takes nothing returns boolean
			return this.m_character != 0
		endmethod

		/// Usually you don't have to call this method since talks will be activated by a specific unit order.
		public method openForCharacter takes ACharacter character returns nothing
			debug if (this.m_character != 0) then
				debug call this.print("Character is not 0.")
				debug return
			debug endif
			set this.m_character = character
			if (thistype.m_hideUserInterface) then
				call SetUserInterfaceForPlayer(character.player(), false, true)
			endif
			call character.setTalk(this)
			call character.setMovable(false)
			call PauseUnit(this.m_unit, true) // disables routines or something else of NPC
			call SetUnitFacing(character.unit(), GetAngleBetweenUnits(character.unit(), this.m_unit))
			call SetUnitFacing(this.m_unit, GetAngleBetweenUnits(this.m_unit, character.unit()))
			call SetUnitLookAt(character.unit(), "bone_head", this.m_unit, 0.0, 0.0, GetUnitFlyHeight(this.m_unit) + 90.0)
			call SetUnitLookAt(this.m_unit, "bone_head", character.unit(), 0.0, 0.0, GetUnitFlyHeight(character.unit()) + 90.0)
			call AThirdPersonCamera.playerThirdPersonCamera(character.player()).resetCamAoa()
			call AThirdPersonCamera.playerThirdPersonCamera(character.player()).resetCamRot()
			call AThirdPersonCamera.playerThirdPersonCamera(character.player()).disable()
			call AThirdPersonCamera.playerThirdPersonCamera(character.player()).enable(character.unit(), 0.0)
			call AGui.playerGui(character.player()).dialog().clear()
			call AGui.playerGui(character.player()).dialog().setMessage(GetUnitName(this.m_unit))
			call this.m_startAction.execute(this) // create buttons
		endmethod

		public method isClosed takes nothing returns boolean
			return this.m_character == 0
		endmethod

		public method close takes nothing returns nothing
			local player characterUser = this.m_character.player()
			call AGui.playerGui(characterUser).dialog().clear()
			call ResetUnitLookAt(this.m_character.unit())
			call ResetUnitLookAt(this.m_unit)
			if (thistype.m_hideUserInterface) then
				call SetUserInterfaceForPlayer(characterUser, true, true)
			endif
			if (not ACharacter.useViewSystem() or not this.m_character.view().enableAgain()) then
				call AThirdPersonCamera.playerThirdPersonCamera(characterUser).pause()
				call ResetToGameCameraForPlayer(characterUser, 0.0)
			endif
			call this.m_character.setTalk(0)
			call this.m_character.setMovable(true)
			set this.m_character = 0
			call PauseUnit(this.m_unit, false) //Enables routines or something else
			set characterUser = null
		endmethod

		private static method infoActionExit takes AInfo info returns nothing
			call info.talk.evaluate().close()
		endmethod

		public method addExitButton takes  nothing returns AInfo
			return AInfo.create.evaluate(this, true, false, 0, thistype.infoActionExit, thistype.m_textExit)
		endmethod

		public method enable takes nothing returns nothing
			set this.m_isEnabled = true
			call EnableTrigger(this.m_orderTrigger)
			if (thistype.m_effectPath != null) then
				set this.m_effect = AddSpecialEffectTarget(thistype.m_effectPath, this.m_unit, "overhead")
			endif
		endmethod

		public method disable takes nothing returns nothing
			set this.m_isEnabled = false
			call DisableTrigger(this.m_orderTrigger)
			if (thistype.m_effectPath != null) then
				call DestroyEffect(this.m_effect)
				set this.m_effect = null
			endif
		endmethod

		public method isInfoShown takes integer index returns boolean
			return AInfo(this.m_infos[index]).isShown.evaluate()
		endmethod

		public method infos takes nothing returns integer
			return this.m_infos.size()
		endmethod

		public method shownInfos takes nothing returns integer
			local integer result = 0
			local integer i = 0
			loop
				exitwhen (i == this.m_infos.size())
				if (this.isInfoShown(i)) then
					set result = result + 1
				endif
				set i = i + 1
			endloop
			return result
		endmethod

		/// Used by @struct AInfo.
		public method showInfo takes integer index returns boolean
			return AInfo(this.m_infos[index]).show.evaluate()
		endmethod

		/// Used by @function ADialogButtonAction.
		public method runInfo takes integer index returns nothing
			call AInfo(this.m_infos[index]).run.evaluate()
			debug if (not this.isClosed() and this.shownInfos() == 0) then
				debug call this.print("No infos were shown anymore after last one although talk hasn't been closed yet.")
				debug call this.showStartPage()
			debug endif
		endmethod

		public method getInfoByDialogButtonIndex takes integer dialogButtonIndex returns AInfo
			local integer i = 0
			loop
				exitwhen (i == this.m_infos.size())
				if (AInfo(this.m_infos[i]).isShown.evaluate() and AInfo(this.m_infos[i]).dialogButtonIndex.evaluate() == dialogButtonIndex) then
					return AInfo(this.m_infos[i])
				endif
				set i = i + 1
			endloop
			return 0
		endmethod

		/// Friend relationship to @struct AInfo, do not use.
		public method addInfoInstance takes AInfo info returns integer
			call this.m_infos.pushBack(info)
			return this.m_infos.backIndex()
		endmethod

		/// Friend relationship to @struct AInfo, do not use.
		public method removeInfoInstanceByIndex takes integer index returns nothing
			call this.m_infos.erase(index)
		endmethod

		private static method triggerConditionOpen takes nothing returns boolean
			local trigger triggeringTrigger
			local unit triggerUnit
			local player owner
			local unit orderTargetUnit
			local thistype this
			local boolean result = false
			if (GetIssuedOrderId() == OrderId(thistype.m_order)) then // right click
				set triggerUnit = GetTriggerUnit()
				set owner = GetOwningPlayer(triggerUnit)
				// Is character, if there is shared control or controller is computer player talks can not be used.
				if (GetPlayerSlotState(owner) != PLAYER_SLOT_STATE_LEFT and GetPlayerController(owner) != MAP_CONTROL_COMPUTER and triggerUnit == ACharacter.playerCharacter(owner).unit()) then
					set triggeringTrigger = GetTriggeringTrigger()
					set this = AHashTable.global().handleInteger(triggeringTrigger, "this")
					set orderTargetUnit = GetOrderTargetUnit()
					if (orderTargetUnit == this.m_unit) then
						if (GetDistanceBetweenUnits(triggerUnit, orderTargetUnit, 0.0, 0.0) <= thistype.m_maxDistance) then //Z value is not checked
							set result = (this.m_character == 0)
							if (not result) then
								call ACharacter.playerCharacter(owner).displayMessage(ACharacter.messageTypeError, thistype.m_textErrorMessage)
							endif
						endif
					endif
					set triggeringTrigger = null
					set orderTargetUnit = null
				endif
				set triggerUnit = null
				set owner = null
			endif
			return result
		endmethod

		private static method triggerActionOpen takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local player triggerPlayer = GetTriggerPlayer()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			call IssueImmediateOrder(ACharacter.playerCharacter(triggerPlayer).unit(), "stop")
			call this.openForCharacter(ACharacter.playerCharacter(triggerPlayer))
			set triggeringTrigger = null
			set triggerPlayer = null
		endmethod

		private method createOrderTrigger takes nothing returns nothing
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.m_orderTrigger = CreateTrigger()
			call TriggerRegisterAnyUnitEventBJ(this.m_orderTrigger, EVENT_PLAYER_UNIT_ISSUED_UNIT_ORDER)
			set conditionFunction = Condition(function thistype.triggerConditionOpen)
			set triggerCondition = TriggerAddCondition(this.m_orderTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.m_orderTrigger, function thistype.triggerActionOpen)
			call AHashTable.global().setHandleInteger(this.m_orderTrigger, "this", this)
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		private method createEffect takes nothing returns nothing
			if (thistype.m_effectPath != null) then
				set this.m_effect = AddSpecialEffectTarget(thistype.m_effectPath, this.m_unit, "overhead")
			endif
		endmethod

		public static method create takes unit usedUnit, ATalkStartAction startAction returns thistype
			local thistype this = thistype.allocate()
			// construction members
			set this.m_unit = usedUnit
			set this.m_startAction = startAction
			// members
			set this.m_infos = AIntegerVector.create()
			set this.m_character = 0
			set this.m_isEnabled = true

			if (thistype.m_effectPath != null and thistype.m_disableEffectsInCinematicMode) then
				call thistype.m_talks.pushBack(this)
			endif

			call this.createOrderTrigger()
			call this.createEffect()
			return this
		endmethod

		private method destroyOrderTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_orderTrigger)
			set this.m_orderTrigger = null
		endmethod

		private method destroyEffect takes nothing returns nothing
			if (thistype.m_effectPath != null) then
				call DestroyEffect(this.m_effect)
				set this.m_effect = null
			endif
		endmethod

		private method destroyInfos takes nothing returns nothing
			loop
				exitwhen (this.m_infos.empty())
				call AInfo(this.m_infos.back()).destroy.evaluate()
			endloop
			call this.m_infos.destroy()
			if (thistype.m_effectPath != null and thistype.m_disableEffectsInCinematicMode) then
				call thistype.m_talks.remove(this)
			endif
		endmethod

		public method onDestroy takes nothing returns nothing
			// construction members
			set this.m_unit = null

			call this.destroyInfos()
			call this.destroyOrderTrigger()
			call this.destroyEffect()
		endmethod

		private static method onInit takes nothing returns nothing
			call thistype.setName("ATalk")
		endmethod

		public static method init takes string order, real maxDistance, string effectPath, boolean hideUserInterface, boolean disableEffectsInCinematicMode, string textErrorMessage, string textExit, string textBack returns nothing
			// static construction members
			set thistype.m_order = order
			set thistype.m_maxDistance = maxDistance
			set thistype.m_effectPath = effectPath
			set thistype.m_hideUserInterface = hideUserInterface
			set thistype.m_disableEffectsInCinematicMode = disableEffectsInCinematicMode
			set thistype.m_textErrorMessage = textErrorMessage
			set thistype.m_textExit = textExit
			set thistype.m_textBack = textBack

			if (thistype.m_effectPath != null and thistype.m_disableEffectsInCinematicMode) then
				set thistype.m_talks = AIntegerList.create()
			endif
			call thistype.initialize()
		endmethod

		public static method disableEffectsInCinematicMode takes nothing returns boolean
			return thistype.m_disableEffectsInCinematicMode
		endmethod

		public static method hideAllEffects takes nothing returns nothing
			local AIntegerListIterator iterator = thistype.m_talks.begin()
			loop
				exitwhen (not iterator.isValid())
				call thistype(iterator.data()).destroyEffect()
				call iterator.next()
			endloop
			call iterator.destroy()
		endmethod

		public static method showAllEffects takes nothing returns nothing
			local AIntegerListIterator iterator = thistype.m_talks.begin()
			loop
				exitwhen (not iterator.isValid())
				call thistype(iterator.data()).createEffect()
				call iterator.next()
			endloop
			call iterator.destroy()
		endmethod
	endstruct

endlibrary