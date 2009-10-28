library AStructSystemsCharacterInfo requires optional ALibraryCoreDebugMisc, ALibraryCoreEnvironmentSound, ALibraryCoreGeneralPlayer, AStructCoreInterfaceThirdPersonCamera, ALibraryCoreInterfaceCinematic, ALibraryCoreInterfaceMisc, ALibraryCoreMathsUnit, AStructSystemsCharacterVideo


	/**
	* Methods are often called in their own threads -> TriggerSleepAction problem.
	*/
	function speech takes AInfo info, boolean toCharacter, string text, sound usedSound returns nothing
		local real duration
		local player user = info.talk().character().user()
		local unit speaker
		local unit listener
		local player speakerOwner
		call waitForVideo(1.0) // do not show any speeches during video
		if (toCharacter) then
			set speaker = info.talk().unit()
			set listener = info.talk().character().unit()
			set speakerOwner = GetOwningPlayer(info.talk().unit())
		else
			set speaker =  info.talk().character().unit()
			set listener = info.talk().unit()
			set speakerOwner = info.talk().character().user()
		endif
		if (usedSound != null) then
			set duration = GetSoundDurationBJ(usedSound)
		else
			set duration = bj_NOTHING_SOUND_DURATION
		endif
		if (AInfo.speechAnimation != null) then
			call SetUnitAnimation(speaker, AInfo.speechAnimation)
		endif
		if (AInfo.listenAnimation != null) then
			call SetUnitAnimation(listener, AInfo.listenAnimation)
		endif
		if (usedSound != null) then
			call PlaySoundForPlayer(user, usedSound)
		endif
		/*
		call CameraSetupApplyForPlayer(false, AInfo.cameraSetup, user, 0.0)
		call SetCameraFieldForPlayer(user, CAMERA_FIELD_ROTATION, GetUnitFacing(speaker) - 180.0, 0.0)
		call SetCameraFieldForPlayer(user, CAMERA_FIELD_ZOFFSET, GetUnitZ(speaker) + 128.0, 0.0)
		call SetCameraTargetControllerNoZForPlayer(user, speaker, 0.0, 0.0, false)
		*/
		call AThirdPersonCamera.playerThirdPersonCamera(user).enable(listener, 0.0)
		call SetCinematicSceneForPlayer(user, GetUnitTypeId(speaker), speakerOwner, GetUnitName(speaker), text, duration, duration)
		if (info.talk().character().talkLog() != 0) then
			call info.talk().character().talkLog().addMessage(info.talk(), text) //log message
		endif
		call waitForVideo(1.0) // do not show any speeches during video
		if (AInfo.skipKey == -1) then
			call TriggerSleepAction(duration)
		else
			set AInfo.playerHasSkipped[GetPlayerId(user)] = false
			loop
				exitwhen (duration <= 0.0)
				if (AInfo.playerHasSkipped[GetPlayerId(user)]) then
					//if (AInfo.skipKey != KEY_ESCAPE) then
						//call ClearScreenMessagesForPlayer(user) /// @todo Does not do anything.
					//endif
					set AInfo.playerHasSkipped[GetPlayerId(user)] = false
					return
				endif
				call TriggerSleepAction(AInfo.skipCheckRate)
				set duration = duration - AInfo.skipCheckRate
			endloop
		endif
		call StopSound(usedSound, true, false) //stop sound since speech could have been skipped by player
		call ResetUnitAnimation(speaker)
		call ResetUnitAnimation(listener)
		call AThirdPersonCamera.playerThirdPersonCamera(user).disable()
		set user = null
		set speaker = null
		set listener = null
		set speakerOwner = null
	endfunction

	/// @todo Shoud be a static member of @struct AInfo, vJass bug.
	function interface AInfoCondition takes AInfo info returns boolean

	/// @todo Shoud be a static member of @struct AInfo, vJass bug.
	function interface AInfoAction takes AInfo info returns nothing

	/**
	* Members of talks are called informations or infos. An info is a single object which informs
	* the user about something like new quests or important things which happened.
	* Infos can have their own condition, so player do not always get access to them.
	* This allows the creator to write nice multiple choice dialogs for different classes or players.
	* Additionally you're able to make infos permanent, so player can always use or get them if the condition is true.
	* Since there probably will be some longer speeches of an information they're skipable by pressing a specific key.
	* This must be explicit enabled when calling the struct initializer (@method AInfo.init).
	*/
	struct AInfo
		//static start members
		public static camerasetup cameraSetup /// Do not use.
		public static integer skipKey /// Do not use.
		public static real skipCheckRate /// Do not use.
		public static string speechAnimation /// Do not use.
		public static string listenAnimation /// Do not use.
		//static members
		private static trigger skipTrigger
		public static boolean array playerHasSkipped[12] //bj_MAX_PLAYERS /// Do not use.
		//start members
		private ATalk m_talk
		private boolean m_permanent
		private boolean m_important
		private AInfoCondition m_condition
		private AInfoAction m_action
		private string m_description
		//members
		private ADialogButton m_dialogButton
		private integer m_talkIndex
		private boolean array m_hasBeenShownToCharacter[12] //bj_MAX_PLAYERS

		//! runtextmacro optional A_STRUCT_DEBUG("\"AInfo\"")

		//start members

		public method talk takes nothing returns ATalk
			return this.m_talk
		endmethod

		public method permanent takes nothing returns boolean
			return this.m_permanent
		endmethod

		public method important takes nothing returns boolean
			return this.m_important
		endmethod

		public method condition takes nothing returns AInfoCondition
			return this.m_condition
		endmethod

		public method action takes nothing returns AInfoAction
			return this.m_action
		endmethod

		public method description takes nothing returns string
			return this.m_description
		endmethod
		
		//members
		
		public method isShown takes nothing returns boolean
			return this.m_dialogButton != 0
		endmethod
		
		public method dialogButtonIndex takes nothing returns integer
			return this.m_dialogButton.index()
		endmethod
		
		public method hasBeenShownToCharacter takes integer playerId returns boolean
			return this.m_hasBeenShownToCharacter[playerId]
		endmethod

		//methods

		private static method dialogButtonActionRunInfo takes ADialogButton dialogButton returns nothing
			local ATalk talk = ACharacter.playerCharacter(dialogButton.dialog().player()).talk()
			local thistype info = talk.getInfoByDialogButtonIndex(dialogButton.index())
			call talk.clear()
			call info.run()
		endmethod

		public method show takes nothing returns boolean
			local unit self = this.m_talk.character().unit()
			local player user = this.m_talk.character().user()
			local integer shortcut
			local boolean result = false
			if (this.m_permanent or not this.m_hasBeenShownToCharacter[GetPlayerId(user)]) then
				if (this.m_important) then
					if (this.m_condition == 0 or this.m_condition.evaluate(this)) then
						set result = true
						call this.run()
					endif
				else
					if (this.m_condition == 0 or this.m_condition.evaluate(this)) then
						set result = true
						set shortcut = AGui.playerGui(user).dialog().dialogButtons()
						set this.m_dialogButton = AGui.playerGui(user).dialog().addDialogButton("[" + I2S(shortcut) + "] " + this.m_description, AShortcut0 + shortcut, thistype.dialogButtonActionRunInfo)
					endif
				endif
			endif
			set self = null
			set user = null
			return result
		endmethod
		
		public method hide takes nothing returns nothing
			set this.m_dialogButton = 0
		endmethod

		public method run takes nothing returns nothing
			local unit self = this.m_talk.character().unit()
			local player user = GetOwningPlayer(self)
			set this.m_hasBeenShownToCharacter[GetPlayerId(user)] = true
			call this.m_action.execute(this)
			set self = null
			set user = null
		endmethod

		public static method create takes ATalk talk, boolean permanent, boolean important, AInfoCondition condition, AInfoAction action, string description returns AInfo
			local AInfo this = AInfo.allocate()
			//start members
			set this.m_talk = talk
			set this.m_permanent = permanent
			set this.m_important = important
			set this.m_condition = condition
			set this.m_action = action
			set this.m_description = description
			//members
			set this.m_dialogButton = 0
			set this.m_talkIndex = talk.addInfoInstance(this)
			return this
		endmethod
		
		public method onDestroy takes nothing returns nothing
			call this.m_talk.removeInfoInstanceByIndex(this.m_talkIndex)
		endmethod

		private static method triggerConditionSkip takes nothing returns boolean
			local player triggerPlayer = GetTriggerPlayer()
			local boolean result = ACharacter.playerCharacter(triggerPlayer).talk() != 0
			debug if (result) then
				debug call AInfo.staticPrint("Result of info skip is true.")
			debug endif
			set triggerPlayer = null
			return result
		endmethod

		private static method triggerActionSkip takes nothing returns nothing
			local player triggerPlayer = GetTriggerPlayer()
			//call ClearScreenMessagesForPlayer(triggerPlayer) //does not do anything
			set AInfo.playerHasSkipped[GetPlayerId(triggerPlayer)] = true
			/// @todo Stop sound immediatly
			set triggerPlayer = null
		endmethod

		private static method createSkipTrigger takes nothing returns nothing
			local integer i
			local player user
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set AInfo.skipTrigger = CreateTrigger()
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then
					set triggerEvent = TriggerRegisterKeyEventForPlayer(user, AInfo.skipTrigger, AInfo.skipKey, true)
					set triggerEvent = null
				endif
				set user = null
				set i = i + 1
			endloop
			set conditionFunction = Condition(function AInfo.triggerConditionSkip)
			set triggerCondition = TriggerAddCondition(AInfo.skipTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(AInfo.skipTrigger, function AInfo.triggerActionSkip)
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		public static method init takes camerasetup cameraSetup, integer skipKey, real skipCheckRate, string speechAnimation, string listenAnimation returns nothing
			//static start members
			set AInfo.cameraSetup = cameraSetup
			set AInfo.skipKey = skipKey
			set AInfo.skipCheckRate = skipCheckRate
			if (skipKey != -1) then
				call AInfo.createSkipTrigger()
			endif
			set AInfo.speechAnimation = speechAnimation
			set AInfo.listenAnimation = listenAnimation
		endmethod

		public static method cleanUp takes nothing returns nothing
			call DestroyTrigger(AInfo.skipTrigger)
			set AInfo.skipTrigger = null
		endmethod
	endstruct

endlibrary