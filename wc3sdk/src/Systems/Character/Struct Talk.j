library AStructSystemsCharacterTalk requires ALibraryCoreDebugMisc, ALibraryCoreGeneralPlayer, ALibraryCoreMathsHandle, ALibraryCoreInterfaceCinematic, ALibraryCoreInterfaceMisc, AStructSystemsGuiGui, AStructSystemsCharacterCharacterHashTable, AStructSystemsCharacterCharacter

	private struct AInfo
		//start members
		private ATalk talk
		private string text
		private sound usedSound
		private boolean toCharacter
		private boolean permanent
		private boolean important
		private ATalkButtonCondition condition
		private ATalkButtonAction buttonAction
		//members
		private boolean array wasPressed[12] //[bj_MAX_PLAYERS] /// @todo vJass bug

		//! runtextmacro A_STRUCT_DEBUG("\"AInfo\"")

		//members

		public method setWasPressed takes integer playerId, boolean wasPressed returns nothing
			set this.wasPressed[playerId] = wasPressed
		endmethod

		//methods

		public method show takes nothing returns nothing
			debug if (this.talk.getCharacter() == 0) then
				debug call this.print("Talk is not enabled.")
				debug return
			debug endif

			if (not this.permanent) then
				if (this.wasPressed[GetPlayerId(this.talk.getCharacter().getUser())]) then
					return
				endif
			endif

			if (this.condition != 0 and not this.condition.evaluate(this.talk)) then
				return
			endif

			if (this.important) then
				set this.wasPressed[GetPlayerId(this.talk.getCharacter().getUser())] = true
				call this.talk.hide()
				call this.talk.speech(this.text, this.usedSound, true) /// @todo trigger sleep action in method
				call this.buttonAction.execute(this.talk)
			else
				call this.talk.addButton(this.text, this.buttonAction, this)
			endif
		endmethod

		public method run takes nothing returns nothing
			set this.wasPressed[GetPlayerId(this.talk.getCharacter().getUser())] = true
			call this.talk.speech(this.text, this.usedSound, false) /// @todo Bug 2: Does not wait!
		endmethod

		public static method create takes ATalk talk, string text, sound usedSound, boolean permanent, boolean important, ATalkButtonCondition condition, ATalkButtonAction buttonAction returns AInfo
			local AInfo this = AInfo.allocate()
			//start members
			set this.talk = talk
			set this.text = text
			set this.usedSound = usedSound
			set this.permanent = permanent
			set this.important = important
			set this.condition = condition
			set this.buttonAction = buttonAction

			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			//start members
			set this.usedSound = null
		endmethod
	endstruct

	/// @todo Should be a part of @struct ATalk, vJass bug.
	function interface ATalkStartAction takes ATalk talk returns nothing

	/// @todo Should be a part of @struct ATalk, vJass bug.
	function interface ATalkButtonCondition takes ATalk talk returns boolean

	/// @todo Should be a part of @struct ATalk, vJass bug.
	function interface ATalkButtonAction takes ATalk talk returns nothing

	/// @todo Should be a part of @struct ATalk, vJass bug.
	private function GuiDialogButtonActionTalkAction takes AGui gui, integer index returns nothing
		local ATalk talk = ACharacter.getPlayerCharacter(gui.getUser()).getTalk()
		call talk.runButtonAction(index)
	endfunction

	/// @todo Should be a part of @struct ATalk, vJass bug.
	private function talkButtonActionExit takes ATalk talk returns nothing
		call talk.disable()
	endfunction

	/// @todo Should be a part of @struct ATalk, vJass bug.
	private function talkButtonActionBackToFirstPage takes ATalk talk returns nothing
		call talk.showFirstPage()
	endfunction
	
	struct ATalk
		//static constant members
		private static constant integer maxButtons = 100
		private static constant integer maxInfos = 50
		//static start members
		private static camerasetup cameraSetup
		private static integer skipKey
		private static real skipCheckRate
		private static string order
		private static real maxDistance
		private static string effectPath
		private static string speechAnimation
		private static string listenAnimation
		private static string textErrorMessage
		private static string textExit
		private static string textBack
		//static members
		private static boolean array playerHasSkipped[12] /// @todo bj_MAX_PLAYERS
		private static trigger skipTrigger
		//start members
		private unit usedUnit
		private ATalkStartAction startAction //Create all buttons in your start action
		//members
		private trigger orderTrigger
		private effect usedEffect
		private ATalkButtonAction array buttonAction[ATalk.maxButtons]
		private AInfo array buttonInfo[ATalk.maxButtons]
		private integer buttons
		private AInfo array info[ATalk.maxInfos]
		private integer infos
		private ACharacter character //Optional, um nicht über die Spielernummer zugreifen zu müssen

		//! runtextmacro A_STRUCT_DEBUG("\"ATalk\"")

		//members

		public method getCharacter takes nothing returns ACharacter
			return this.character
		endmethod
		
		//comfort methods
		
		public method showSimpleButton takes string text, ATalkButtonAction buttonAction returns nothing
			call this.addButton(text, buttonAction, 0)
		endmethod

		//Eine einfache Vorlage ohne spezielle Funktion
		public method showExitButton takes nothing returns nothing
			call this.addButton(ATalk.textExit, ATalkButtonAction.talkButtonActionExit, 0)
		endmethod

		public method showBackButton takes ATalkButtonAction buttonAction returns nothing
			call this.addButton(ATalk.textBack, buttonAction, 0)
		endmethod
		
		public method showBackToFirstPageButton takes nothing returns nothing
			call this.showBackButton(talkButtonActionBackToFirstPage)
		endmethod

		//methods

		/// @todo trigger sleep action in method
		public method speech takes string text, sound usedSound, boolean toCharacter returns nothing
			local real time = ATalk.getTime(usedSound)
			call SetUnitAnimation(this.character.getUsedUnit(), ATalk.speechAnimation) //Falls vorhanden
			call SetUnitAnimation(this.usedUnit, ATalk.listenAnimation) //Falls vorhanden
			if (usedSound != null) then
				call PlaySoundForPlayer(this.character.getUser(), usedSound) //ALibraryEnvironmentSound
			endif
			call this.refreshView(toCharacter)
			if (toCharacter) then
				call SetCinematicSceneForPlayer(this.character.getUser(), GetUnitTypeId(this.usedUnit), GetUnitName(this.usedUnit), text, time, time) //ALibraryInterfaceCinematic
			else
				call SetCinematicSceneForPlayer(this.character.getUser(), GetUnitTypeId(this.character.getUsedUnit()), GetUnitName(this.character.getUsedUnit()), text, time, time) //ALibraryInterfaceCinematic
			endif
			debug call this.print("Before wait.")
			//call StopSound(usedSound, true, false) //Eventuell den zweiten Parameter auf false setzen
			call this.wait(usedSound)
			debug call this.print("After wait.")
			call this.resetAnimations()
		endmethod

		/// @param important The NPC tells the info to the character when the character wants to start a talk with him.
		public method addInfo takes string text, sound usedSound, boolean permanent, boolean important, ATalkButtonCondition condition, ATalkButtonAction buttonAction returns nothing
			local integer id = this.infos
			debug if (id == ATalk.maxInfos) then
				debug call this.print("Info maximum reached.")
				debug return
			debug endif

			set this.infos = this.infos + 1
			set this.info[id] = AInfo.create(this, text, usedSound, permanent, important, condition, buttonAction)
		endmethod
		
		public method clear takes nothing returns nothing
			set this.buttons = 0
			call AGui.getPlayerGui(this.character.getUser()).clearDialog()//You do not have to clear the dialog in the start action!
		endmethod

		public method showFirstPage takes nothing returns nothing
			call this.clear()
			call this.startAction.execute(this) //create buttons
			//call this.show() //don't call this beacause the start action is often called by button actions
		endmethod

		public method showInfo takes integer index returns nothing
			debug if (this.info[index] == 0) then
				debug call this.print("Empty info.")
			debug endif
			call this.info[index].show()
		endmethod

		//call this method at the end of each dialog
		public method show takes nothing returns nothing
			call AGui.getPlayerGui(this.character.getUser()).setDialogTitle(GetUnitName(this.usedUnit))
			call AGui.getPlayerGui(this.character.getUser()).showDialog()
		endmethod

		//call this if you want to hide the dialog (not cancel)
		public method hide takes nothing returns nothing
			call AGui.getPlayerGui(this.character.getUser()).hideDialog()
		endmethod

		//Call this to disable the dialog during the talk.
		public method disable takes nothing returns nothing
			call AGui.getPlayerGui(this.character.getUser()).hideDialog()
			call this.resetFacings()
			call SetUserInterfaceForPlayer(this.character.getUser(), true, true)
			call ResetToGameCameraForPlayer(this.character.getUser(), 0.0)
			call this.character.setTalk(0)
			call this.character.setMovable(true)
			set this.character = 0

			call PauseUnit(this.usedUnit, false) //Disables routines or something else
		endmethod

		/// Friend relation to library (To AInfo). Don't use this method!
		public method addButton takes string text, ATalkButtonAction buttonAction, AInfo info returns nothing
			local integer id = this.buttons
			debug if (id == ATalk.maxButtons) then
				debug call this.print("Button maximum reached.")
				debug return
			debug endif

			if (AGui.getPlayerGui(this.character.getUser()).addDialogButton("[" + I2S(this.buttons + 1) + "] " + text,  id + SHORTCUT_1, GuiDialogButtonActionTalkAction) != - 1) then
				set this.buttons = this.buttons + 1
				set this.buttonAction[id] = buttonAction
				set this.buttonInfo[id] = info
			endif
		endmethod

		/// Friend relation to the function interface.
		/// Don't use this method!
		public method runButtonAction takes integer index returns nothing
			if (this.buttonInfo[index] != 0) then
				call this.hide()
				call this.buttonInfo[index].run()
			endif
			//No clear if no info
			if (this.buttonAction[index] != 0) then
				call this.buttonAction[index].execute(this)
			else
				call this.showFirstPage()
			endif
		endmethod
		
		private method wait takes sound usedSound returns nothing
			local real time = ATalk.getTime(usedSound)
			debug if (time < ATalk.skipCheckRate) then
				debug call this.print("Time is smaller than skip check rate.")
			debug endif
			debug call this.print("Wait " + R2S(time) + ".")
			if (ATalk.skipKey == -1) then
				call TriggerSleepAction(time)
			else
				loop
					exitwhen (time <= 0.0)
					debug call this.print("Before trigger sleep with time: " + R2S(time))
					call TriggerSleepAction(ATalk.skipCheckRate)
					set time = time - ATalk.skipCheckRate
					debug call this.print("Loop with remaining time: " + R2S(time))
					if (time <= 0.0) then
						return
					elseif (ATalk.playerHasSkipped[GetPlayerId(this.character.getUser())]) then
						debug call this.print("Talk " + I2S(this) + " was skipped.")
						if (ATalk.skipKey != KEY_ESCAPE) then //ALibraryInterfaceMisc
							call ClearScreenMessagesForPlayer(this.character.getUser()) //ALibraryInterfaceCinematic
						endif
						call StopSound(usedSound, true, false) //Eventuell den zweiten Parameter auf false setzen
						call this.resetAnimations()
						set ATalk.playerHasSkipped[GetPlayerId(this.character.getUser())] = false
						return
					endif
				endloop
			endif
		endmethod

		//Normally you do not need this method
		private method enable takes ACharacter character returns nothing
			debug if (this.character != 0) then
				debug call this.print("Character is not 0 in talk " + I2S(this) + ".")
			debug else
				debug call this.print("Character is 0. Important!")
			debug endif
			debug call this.print("Enable talk for character " + I2S(character) + ".")
			set ATalk.playerHasSkipped[GetPlayerId(character.getUser())] = false //saver!
			call PauseUnit(this.usedUnit, true) //Disables routines or something else
			set this.character = character
			call SetUserInterfaceForPlayer(character.getUser(), false, true)
			call character.setTalk(this)
			call character.setMovable(false)
			call this.refreshView(false)
			call this.refreshFacings()
			call this.clear()
			
			debug if (this.startAction == 0) then
				debug call this.print("Start action is 0")
			debug else
				debug call this.print("Start action is NOT 0")
			debug endif
			
			call this.startAction.execute(this) /// @todo execute? //create buttons
			debug call this.print("After start action")
			//call this.show() //don't call this beacause the start action is often called by button actions
		endmethod

		private method refreshView takes boolean toCharacter returns nothing
			call CameraSetupApplyForPlayer(true, ATalk.cameraSetup, this.character.getUser(), 0.0)
			if (toCharacter) then
				call SetCameraFieldForPlayer(this.character.getUser(), CAMERA_FIELD_ROTATION, GetUnitFacing(this.usedUnit) - 180.0, 0.0)
				call SetCameraFieldForPlayer(this.character.getUser(), CAMERA_FIELD_ZOFFSET, CameraSetupGetField(ATalk.cameraSetup, CAMERA_FIELD_ZOFFSET) + GetUnitFlyHeight(this.usedUnit), 0.0)
				call SetCameraTargetControllerNoZForPlayer(this.character.getUser(), this.usedUnit, 0.0, 0.0, false)
			else
				call SetCameraFieldForPlayer(this.character.getUser(), CAMERA_FIELD_ROTATION, GetUnitFacing(this.character.getUsedUnit()) - 180.0, 0.0)
				call SetCameraFieldForPlayer(this.character.getUser(), CAMERA_FIELD_ZOFFSET, CameraSetupGetField(ATalk.cameraSetup, CAMERA_FIELD_ZOFFSET) + GetUnitFlyHeight(this.character.getUsedUnit()), 0.0)
				call SetCameraTargetControllerNoZForPlayer(this.character.getUser(), this.character.getUsedUnit(), 0.0, 0.0, false)
			endif
		endmethod

		private method refreshFacings takes nothing returns nothing
			call SetUnitFacing(this.character.getUsedUnit(), GetAngleBetweenUnits(this.character.getUsedUnit(), this.usedUnit)) //ALibraryMathsHandle
			call SetUnitFacing(this.usedUnit, GetAngleBetweenUnits(this.character.getUsedUnit(), this.usedUnit) - 180.0) //ALibraryMathsHandle
			call SetUnitLookAt(this.character.getUsedUnit(), "bone_head", this.usedUnit, 0.0, 0.0, GetUnitFlyHeight(this.usedUnit) + 90.0)
			call SetUnitLookAt(this.usedUnit, "bone_head", this.character.getUsedUnit(), 0.0, 0.0, GetUnitFlyHeight(this.character.getUsedUnit()) + 90.0)
		endmethod

		private method resetFacings takes nothing returns nothing
			call ResetUnitLookAt(this.character.getUsedUnit())
			call ResetUnitLookAt(this.usedUnit)
		endmethod

		private method resetAnimations takes nothing returns nothing
			call ResetUnitAnimation(this.character.getUsedUnit())
			call ResetUnitAnimation(this.usedUnit)
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
				//Is character
				if (triggerUnit == ACharacter.getPlayerCharacter(owner).getUsedUnit()) then //AClassCharacterCharacter
					set triggeringTrigger = GetTriggeringTrigger()
					set this = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "this") //ACharacterClassClassHashTable
					set orderTargetUnit = GetOrderTargetUnit() 
					if (orderTargetUnit == this.usedUnit) then
						if (GetDistanceBetweenUnits(triggerUnit, orderTargetUnit, 0.0, 0.0) <= ATalk.maxDistance) then //ALibraryMathsHandle, Z-Wert wird nicht überprüft
							set result = (this.character == 0)
							if (not result) then
								call ACharacter.getPlayerCharacter(owner).displayMessage(ACharacter.messageTypeError, ATalk.textErrorMessage)
							endif
						endif
					endif
					set triggeringTrigger = null
					set orderTargetUnit = null
				endif
				set triggerUnit = null
				set owner = null
			endif
			debug if (result) then
				debug call Print("Talk dialog " + I2S(this) + " was enabled.")
			debug endif
			return result
		endmethod

		private static method triggerActionEnable takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local player triggerPlayer = GetTriggerPlayer()
			local ATalk this = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "this") //AClassCharacterCharacterHashTable
			call IssueImmediateOrder(ACharacter.getPlayerCharacter(triggerPlayer).getUsedUnit(), "stop") //AClassCharacterCharacter
			call this.enable(ACharacter.getPlayerCharacter(triggerPlayer)) //AClassCharacterCharacter
			debug call Print("Enable talk for player " + GetPlayerName(triggerPlayer))
			set triggeringTrigger = null
			set triggerPlayer = null
		endmethod

		private method createOrderTrigger takes nothing returns nothing
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.orderTrigger = CreateTrigger()
			call TriggerRegisterAnyUnitEventBJ(this.orderTrigger, EVENT_PLAYER_UNIT_ISSUED_UNIT_ORDER) //Returned nichts
			set conditionFunction = Condition(function ATalk.triggerConditionEnable)
			set triggerCondition = TriggerAddCondition(this.orderTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.orderTrigger, function ATalk.triggerActionEnable)
			call AGetCharacterHashTable().storeHandleInteger(this.orderTrigger, "this", this) //AClassCharacterCharacterHashTable
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		public static method create takes unit usedUnit, ATalkStartAction startAction returns ATalk
			local ATalk this = ATalk.allocate()
			//start members
			set this.usedUnit = usedUnit
			set this.startAction = startAction
			//members
			set this.buttons = 0
			set this.infos = 0
			set this.character = 0

			call this.createOrderTrigger()
			if (ATalk.effectPath != null) then
				set this.usedEffect = AddSpecialEffectTarget(ATalk.effectPath, this.usedUnit, "overhead")
			endif
			return this
		endmethod

		private method destroyOrderTrigger takes nothing returns nothing
			call AGetCharacterHashTable().destroyTrigger(this.orderTrigger) //AClassCharacterCharacterHashTable
			set this.orderTrigger = null
		endmethod

		private method destroyEffect takes nothing returns nothing
			if (ATalk.effectPath != null) then
				call DestroyEffect(this.usedEffect)
				set this.usedEffect = null
			endif
		endmethod

		private method destroyInfos takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.infos)
				call AInfo.destroy(this.info[i])
				set i = i + 1
			endloop
		endmethod

		public method onDestroy takes nothing returns nothing

			call this.destroyOrderTrigger()
			call this.destroyEffect()
			call this.destroyInfos()
		endmethod

		/// @param skipKey If this value is -1 players can not skip talks.
		/// @param effectPath If this value is null there is no effect
		public static method init takes camerasetup cameraSetup, integer skipKey, real skipCheckRate, string order, real maxDistance, string effectPath, string speechAnimation, string listenAnimation, string textErrorMessage, string textExit, string textBack returns nothing
			//static start members
			set ATalk.cameraSetup = cameraSetup
			set ATalk.skipKey = skipKey
			set ATalk.skipCheckRate = skipCheckRate
			set ATalk.order = order
			set ATalk.maxDistance = maxDistance
			set ATalk.effectPath = effectPath
			set ATalk.speechAnimation = speechAnimation
			set ATalk.listenAnimation = listenAnimation
			set ATalk.textErrorMessage = textErrorMessage
			set ATalk.textExit = textExit
			set ATalk.textBack = textBack
			if (skipKey != -1) then
				call ATalk.createSkipTrigger()
			endif
		endmethod
		
		private static method triggerActionSkip takes nothing returns nothing
			local player user = GetTriggerPlayer()
			if (ACharacter.getPlayerCharacter(user).getTalk() != 0) then
				set ATalk.playerHasSkipped[GetPlayerId(user)] = true
				debug call Print("Player " + GetPlayerName(user) + " skips talk.")
			endif
			set user = null
		endmethod
		
		private static method createSkipTrigger takes nothing returns nothing
			local integer i
			local player user
			local event triggerEvent
			local triggeraction triggerAction
			set ATalk.skipTrigger = CreateTrigger()
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then //ALibraryGeneralPlayer
					set triggerEvent = TriggerRegisterKeyEventForPlayer(user, ATalk.skipTrigger, ATalk.skipKey, true) //ALibraryInterfaceMisc
					set triggerEvent = null
				endif
				set user = null
				set i = i + 1
			endloop
			set triggerAction = TriggerAddAction(ATalk.skipTrigger, function ATalk.triggerActionSkip)
			set triggerAction = null
		endmethod
		
		private static method getTime takes sound usedSound returns real
			if (usedSound != null) then
				return GetSoundDurationBJ(usedSound)
			endif
			return bj_NOTHING_SOUND_DURATION
		endmethod
	endstruct

endlibrary
