library AStructSystemsCharacterTalk requires optional ALibraryCoreDebugMisc, AStructCoreGeneralHashTable, AStructCoreGeneralVector, ALibraryCoreInterfaceMisc, ALibraryCoreMathsHandle, AStructSystemsCharacterCharacter

	/// @todo Should be a part of @struct ATalk, vJass bug.
	function interface ATalkStartAction takes ATalk talk returns nothing
	
	/// @todo Should be a static method of @struct ATalk, vJass bug.
	private function AInfoActionBackToStartPage takes AInfo info returns nothing
		call info.talk().showStartPage()
	endfunction
	
	/// @todo Should be a static method of @struct ATalk, vJass bug.
	private function AInfoActionExit takes AInfo info returns nothing
		call info.talk().disable()
	endfunction

	/**
	* Talks are a kind of dialogs with NPCs which are implemented by using the Warcraft 3 dialog natives.
	* This means that choices in the form of dialog buttons are shown to a character owner.
	* If the owner presses any button an user-defined function will be called where the user can define the whole talk.
	* One talk contains one or several infos (@struct AInfo). These infos contain the user-defined function and an user-defined condition.
	* Besides they can have various other properties.
	* Note that only one character owner can use one talk at the same time. There is no support for several character owners talking to the same NPC, yet.
	*/
	struct ATalk
		public static constant integer maxInfos = 100
		//static start members
		private static string order
		private static real maxDistance
		private static string effectPath
		private static string textErrorMessage
		private static string textExit
		private static string textBack
		//start members
		private unit m_unit
		private ATalkStartAction m_startAction
		//members
		private AIntegerVector m_infos
		private ACharacter m_character
		private trigger m_orderTrigger
		private effect m_effect
		
		//! runtextmacro optional A_STRUCT_DEBUG("\"ATalk\"")
		
		//start members
		
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
		
		//convenience methods
		
		/**
		* Shows the talk's Warcraft 3 dialog.
		* If no info has been added start page is shown automatically.
		*/
		public method show takes nothing returns nothing
			if (AGui.playerGui(this.m_character.user()).dialog().dialogButtons() == 0) then
				call this.showStartPage()
			else
				call AGui.playerGui(this.m_character.user()).dialog().show()
			endif
		endmethod
		
		public method showAll takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.m_infos.size())
				call AInfo(this.m_infos[i]).show()
				set i = i + 1
			endloop
			call this.show()
		endmethod
		
		public method hide takes nothing returns nothing
			call AGui.playerGui(this.m_character.user()).dialog().hide()
		endmethod
		
		/// You do not have to clear the dialog in the start action!
		public method clear takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.m_infos.size())
				call AInfo(this.m_infos[i]).hide()
				set i = i + 1
			endloop
			call AGui.playerGui(this.m_character.user()).dialog().clear()
		endmethod
		
		public method addInfo takes boolean permanent, boolean important, AInfoCondition condition, AInfoAction action, string description returns AInfo
			return AInfo.create(this, permanent, important, condition, action, description)
		endmethod
		
		public method addBackButton takes AInfoAction action returns AInfo
			return AInfo.create(this, true, false, 0, action, ATalk.textBack)
		endmethod
		
		public method addBackToStartPageButton takes nothing returns AInfo
			return AInfo.create(this, true, false, 0, AInfoActionBackToStartPage, ATalk.textBack)
		endmethod
		
		public method addExitButton takes  nothing returns AInfo
			return AInfo.create(this, true, false, 0, AInfoActionExit, ATalk.textExit)
		endmethod
		
		public method showStartPage takes nothing returns nothing
			call this.m_startAction.execute(this) //create buttons
		endmethod
		
		/// @returns Returns if info with index @param index has already been shown to the character which is talking currently to the NPC.
		public method infoHasBeenShown takes integer index returns boolean
			return AInfo(this.m_infos[index]).hasBeenShownToCharacter(this.m_character.userId())
		endmethod
		
		//methods
		
		/// Usually you don't have to call this method since talks will be activated by a specific unit order.
		public method enableForCharacter takes ACharacter character returns nothing
			debug if (this.m_character != 0) then
				debug call this.print("Character is not 0.")
				debug return
			debug endif
			set this.m_character = character
			call SetUserInterfaceForPlayer(character.user(), false, true)
			call character.setTalk(this)
			call character.setMovable(false)
			call PauseUnit(this.m_unit, true) //Disables routines or something else
			call SetUnitFacing(character.unit(), GetAngleBetweenUnits(character.unit(), this.m_unit))
			call SetUnitFacing(this.m_unit, GetAngleBetweenUnits(this.m_unit, character.unit()))
			call SetUnitLookAt(character.unit(), "bone_head", this.m_unit, 0.0, 0.0, GetUnitFlyHeight(this.m_unit) + 90.0)
			call SetUnitLookAt(this.m_unit, "bone_head", character.unit(), 0.0, 0.0, GetUnitFlyHeight(character.unit()) + 90.0)
			call AThirdPersonCamera.playerThirdPersonCamera(character.user()).resetCamAoa()
			call AThirdPersonCamera.playerThirdPersonCamera(character.user()).resetCamRot()
			call AThirdPersonCamera.playerThirdPersonCamera(character.user()).disable()
			call AThirdPersonCamera.playerThirdPersonCamera(character.user()).enable(character.unit(), 0.0)
			call AGui.playerGui(character.user()).dialog().clear()
			call AGui.playerGui(character.user()).dialog().setMessage(GetUnitName(this.m_unit))
			//call this.clear()
			call this.m_startAction.execute(this) //create buttons
		endmethod
		
		public method disable takes nothing returns nothing
			local player characterUser = this.m_character.user()
			call AGui.playerGui(characterUser).dialog().clear()
			call ResetUnitLookAt(this.m_character.unit())
			call ResetUnitLookAt(this.m_unit)
			call SetUserInterfaceForPlayer(characterUser, true, true)
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
		
		/// Used by @struct AInfo.
		public method showInfo takes integer index returns boolean
			return AInfo(this.m_infos[index]).show()
		endmethod
		
		/// Used by @function ADialogButtonAction.
		public method runInfo takes integer index returns nothing
			call AInfo(this.m_infos[index]).run()
		endmethod
		
		public method getInfoByDialogButtonIndex takes integer dialogButtonIndex returns AInfo
			local integer i = 0
			loop
				exitwhen (i == this.m_infos.size())
				if (AInfo(this.m_infos[i]).isShown() and AInfo(this.m_infos[i]).dialogButtonIndex() == dialogButtonIndex) then
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
		
		private static method triggerConditionEnable takes nothing returns boolean
			local trigger triggeringTrigger
			local unit triggerUnit
			local player owner
			local unit orderTargetUnit
			local ATalk this
			local boolean result = false
			if (GetIssuedOrderId() == OrderId(ATalk.order)) then //Rechtsklick
				set triggerUnit = GetTriggerUnit()
				set owner = GetOwningPlayer(triggerUnit)
				// Is character, if there is shared control or controller is computer player talks can not be used.
				if (GetPlayerSlotState(owner) != PLAYER_SLOT_STATE_LEFT and GetPlayerController(owner) != MAP_CONTROL_COMPUTER and triggerUnit == ACharacter.playerCharacter(owner).unit()) then
					set triggeringTrigger = GetTriggeringTrigger()
					set this = AHashTable.global().handleInteger(triggeringTrigger, "this")
					set orderTargetUnit = GetOrderTargetUnit() 
					if (orderTargetUnit == this.m_unit) then
						if (GetDistanceBetweenUnits(triggerUnit, orderTargetUnit, 0.0, 0.0) <= ATalk.maxDistance) then //Z value is not checked
							set result = (this.m_character == 0)
							if (not result) then
								call ACharacter.playerCharacter(owner).displayMessage(ACharacter.messageTypeError, ATalk.textErrorMessage)
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

		private static method triggerActionEnable takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local player triggerPlayer = GetTriggerPlayer()
			local ATalk this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			call IssueImmediateOrder(ACharacter.playerCharacter(triggerPlayer).unit(), "stop")
			call this.enableForCharacter(ACharacter.playerCharacter(triggerPlayer))
			set triggeringTrigger = null
			set triggerPlayer = null
		endmethod

		private method createOrderTrigger takes nothing returns nothing
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.m_orderTrigger = CreateTrigger()
			call TriggerRegisterAnyUnitEventBJ(this.m_orderTrigger, EVENT_PLAYER_UNIT_ISSUED_UNIT_ORDER)
			set conditionFunction = Condition(function ATalk.triggerConditionEnable)
			set triggerCondition = TriggerAddCondition(this.m_orderTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.m_orderTrigger, function ATalk.triggerActionEnable)
			call AHashTable.global().setHandleInteger(this.m_orderTrigger, "this", this)
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod
		
		private method createEffect takes nothing returns nothing
			if (ATalk.effectPath != null) then
				set this.m_effect = AddSpecialEffectTarget(ATalk.effectPath, this.m_unit, "overhead")
			endif
		endmethod
		
		public static method create takes unit usedUnit, ATalkStartAction startAction returns ATalk
			local ATalk this = ATalk.allocate()
			//start members
			set this.m_unit = usedUnit
			set this.m_startAction = startAction
			//members
			set this.m_infos = AIntegerVector.create()
			set this.m_character = 0
		
			call this.createOrderTrigger()
			call this.createEffect()
			return this
		endmethod
		
		private method destroyOrderTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_orderTrigger)
			set this.m_orderTrigger = null
		endmethod

		private method destroyEffect takes nothing returns nothing
			if (ATalk.effectPath != null) then
				call DestroyEffect(this.m_effect)
				set this.m_effect = null
			endif
		endmethod

		private method destroyInfos takes nothing returns nothing
			loop
				exitwhen (this.m_infos.empty())
				call AInfo(this.m_infos.back()).destroy()
			endloop
			call this.m_infos.destroy()
		endmethod

		public method onDestroy takes nothing returns nothing
			//start members
			set this.m_unit = null
		
			call this.destroyInfos()
			call this.destroyOrderTrigger()
			call this.destroyEffect()
		endmethod

		public static method init takes string order, real maxDistance, string effectPath, string textErrorMessage, string textExit, string textBack returns nothing
			//static start members
			set ATalk.order = order
			set ATalk.maxDistance = maxDistance
			set ATalk.effectPath = effectPath
			set ATalk.textErrorMessage = textErrorMessage
			set ATalk.textExit = textExit
			set ATalk.textBack = textBack
		endmethod
	endstruct

endlibrary