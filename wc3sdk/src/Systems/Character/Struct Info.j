library AStructSystemsCharacterInfo requires optional ALibraryCoreDebugMisc, ALibraryCoreEnvironmentSound, ALibraryCoreGeneralPlayer, AStructCoreInterfaceThirdPersonCamera, ALibraryCoreInterfaceCinematic, ALibraryCoreInterfaceMisc, ALibraryCoreMathsUnit, AStructSystemsCharacterCharacter, AStructSystemsCharacterTalk, AStructSystemsCharacterVideo

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
		// static construction members
		public static boolean m_thirdPersonCamera /// Do not use.
		public static integer m_skipKey /// Do not use.
		public static real m_skipCheckRate /// Do not use.
		public static string m_speechAnimation /// Do not use.
		public static string m_listenAnimation /// Do not use.
		// static members
		private static trigger m_skipTrigger
		public static boolean array m_playerHasSkipped[12] /// @todo bj_MAX_PLAYERS Do not use.
		// construction members
		private ATalk m_talk
		private boolean m_permanent
		private boolean m_important
		private AInfoCondition m_condition
		private AInfoAction m_action
		private string m_description
		// members
		private ADialogButton m_dialogButton
		private integer m_talkIndex
		private boolean array m_hasBeenShownToCharacter[12] /// @todo bj_MAX_PLAYERS

		//! runtextmacro optional A_STRUCT_DEBUG("\"AInfo\"")

		// construction members

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

		// members

		public method isShown takes nothing returns boolean
			return this.m_dialogButton != 0
		endmethod

		public method dialogButtonIndex takes nothing returns integer
			return this.m_dialogButton.index()
		endmethod

		public method hasBeenShownToCharacter takes integer playerId returns boolean
			return this.m_hasBeenShownToCharacter[playerId]
		endmethod

		// methods

		public method run takes nothing returns nothing
			local unit self = this.m_talk.character().unit()
			local player user = GetOwningPlayer(self)
			set this.m_hasBeenShownToCharacter[GetPlayerId(user)] = true
			call this.m_action.execute(this)
			set self = null
			set user = null
		endmethod

		private static method dialogButtonActionRunInfo takes ADialogButton dialogButton returns nothing
			local ATalk talk = ACharacter.playerCharacter(dialogButton.dialog().player()).talk()
			local thistype info = talk.getInfoByDialogButtonIndex(dialogButton.index())
			call talk.clear()
			call info.run()
		endmethod

		public method show takes nothing returns boolean
			local unit self = this.m_talk.character().unit()
			local player user = this.m_talk.character().player()
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
						set this.m_dialogButton = AGui.playerGui(user).dialog().addDialogButtonIndex(this.m_description, thistype.dialogButtonActionRunInfo)
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

		public static method create takes ATalk talk, boolean permanent, boolean important, AInfoCondition condition, AInfoAction action, string description returns thistype
			local thistype this = thistype.allocate()
			// construction members
			set this.m_talk = talk
			set this.m_permanent = permanent
			set this.m_important = important
			set this.m_condition = condition
			set this.m_action = action
			set this.m_description = description
			// members
			set this.m_dialogButton = 0
			set this.m_talkIndex = talk.addInfoInstance(this)
			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			call this.m_talk.removeInfoInstanceByIndex(this.m_talkIndex)
		endmethod

		private static method triggerConditionSkip takes nothing returns boolean
			local player triggerPlayer = GetTriggerPlayer()
			local boolean result = ACharacter.playerCharacter(triggerPlayer).talk() != 0 and not thistype.m_playerHasSkipped[GetPlayerId(triggerPlayer)]
			set triggerPlayer = null
			return result
		endmethod

		// skipping actions are handled in speech function itself
		private static method triggerActionSkip takes nothing returns nothing
			local player triggerPlayer = GetTriggerPlayer()
			set thistype.m_playerHasSkipped[GetPlayerId(triggerPlayer)] = true
			set triggerPlayer = null
		endmethod

		private static method createSkipTrigger takes nothing returns nothing
			local integer i
			local player user
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set thistype.m_skipTrigger = CreateTrigger()
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then
					set triggerEvent = TriggerRegisterKeyEventForPlayer(user, thistype.m_skipTrigger, thistype.m_skipKey, true)
					set triggerEvent = null
				endif
				set user = null
				set i = i + 1
			endloop
			set conditionFunction = Condition(function thistype.triggerConditionSkip)
			set triggerCondition = TriggerAddCondition(thistype.m_skipTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(thistype.m_skipTrigger, function thistype.triggerActionSkip)
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		public static method init takes boolean thirdPersonCamera, integer skipKey, real skipCheckRate, string speechAnimation, string listenAnimation returns nothing
			// static construction members
			set thistype.m_thirdPersonCamera = thirdPersonCamera
			set thistype.m_skipKey = skipKey
			set thistype.m_skipCheckRate = skipCheckRate
			if (skipKey != -1) then
				call thistype.createSkipTrigger()
			endif
			set thistype.m_speechAnimation = speechAnimation
			set thistype.m_listenAnimation = listenAnimation
		endmethod

		public static method cleanUp takes nothing returns nothing
			call DestroyTrigger(thistype.m_skipTrigger)
			set thistype.m_skipTrigger = null
		endmethod
	endstruct

	/**
	* Methods are often called in their own threads by using .execute automatically -> TriggerSleepAction problem.
	*/
	function speech takes AInfo info, boolean toCharacter, string text, sound usedSound returns nothing
		local real duration
		local player user = info.talk().character().player()
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
			set speakerOwner = info.talk().character().player()
		endif
		if (usedSound != null) then
			set duration = GetSoundDurationBJ(usedSound)
		else
			set duration = bj_NOTHING_SOUND_DURATION
		endif
		if (AInfo.m_speechAnimation != null) then
			call SetUnitAnimation(speaker, AInfo.m_speechAnimation)
		endif
		if (AInfo.m_listenAnimation != null) then
			call SetUnitAnimation(listener, AInfo.m_listenAnimation)
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
		if (AInfo.m_thirdPersonCamera) then
			call AThirdPersonCamera.playerThirdPersonCamera(user).resetCamAoa()
			call AThirdPersonCamera.playerThirdPersonCamera(user).resetCamRot()
			call AThirdPersonCamera.playerThirdPersonCamera(user).enable(listener, 0.0)
		endif
		call SetCinematicSceneForPlayer(user, GetUnitTypeId(speaker), speakerOwner, GetUnitName(speaker), text, duration, duration)
		if (info.talk().character().talkLog() != 0) then
			call info.talk().character().talkLog().addSpeech(info, toCharacter, text, usedSound)
		endif
		if (AInfo.m_skipKey == -1) then
			call TriggerSleepAction(duration)
		else
			set AInfo.m_playerHasSkipped[GetPlayerId(user)] = false
			loop
				exitwhen (duration <= 0.0)
				if (AInfo.m_playerHasSkipped[GetPlayerId(user)]) then
					//if (AInfo.skipKey != KEY_ESCAPE) then
						//call ClearScreenMessagesForPlayer(user) /// @todo Does not do anything.
					//endif
					set AInfo.m_playerHasSkipped[GetPlayerId(user)] = false
					call StopSound(usedSound, false, false) // stop sound since speech could have been skipped by player
					call EndCinematicSceneForPlayer(user)
					exitwhen (true)
				endif
				call TriggerSleepAction(AInfo.m_skipCheckRate)
				set duration = duration - AInfo.m_skipCheckRate
			endloop
		endif
		call waitForVideo(1.0) // do not show any speeches during video
		call ResetUnitAnimation(speaker)
		call ResetUnitAnimation(listener)
		if (AInfo.m_thirdPersonCamera) then
			call AThirdPersonCamera.playerThirdPersonCamera(user).disable()
		endif
		set user = null
		set speaker = null
		set listener = null
		set speakerOwner = null
	endfunction

	function speech2 takes AInfo info, boolean toCharacter, string text, string soundFilePath returns nothing
		local sound whichSound = CreateSound(soundFilePath, false, false, true, 12700, 12700, "")
		if (toCharacter) then
			call SetSoundPosition(whichSound, GetUnitX(info.talk().unit()), GetUnitY(info.talk().unit()), GetUnitZ(info.talk().unit()))
		else
			call SetSoundPosition(whichSound, GetUnitX(info.talk().character().unit()), GetUnitY(info.talk().character().unit()), GetUnitZ(info.talk().character().unit()))
		endif
		call speech(info, toCharacter, text, whichSound)
		call KillSoundWhenDone(whichSound)
		set whichSound = null
	endfunction

endlibrary