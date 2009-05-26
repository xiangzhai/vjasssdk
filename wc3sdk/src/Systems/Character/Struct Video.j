library AStructSystemsCharacterVideo requires ALibraryCoreDebugMisc, ALibraryCoreGeneralPlayer, ALibraryCoreStringConversion, ALibraryCoreInterfaceMisc, AStructSystemsCharacterCharacter

	/// @todo Should be a part of @struct AVideo, vJass bug.
	function interface AVideoInitAction takes AVideo video returns nothing

	/// @todo Should be a part of @struct AVideo, vJass bug.
	function interface AVideoPlayAction takes AVideo video returns nothing
	
	/// @todo Should be a part of @struct AVideo, vJass bug.
	function interface AVideoStopAction takes AVideo video returns nothing

	struct AVideo
		//static start members
		private static integer divident
		private static real filterDuration
		private static real waitInterval
		private static string textPlayerSkips
		private static string textSkip
		//static members
		private static real waitTime
		private static AVideo runningVideo
		private static boolean skipped
		private static integer skippingPlayers
		private static trigger skipTrigger
		//start members
		private AVideoInitAction initAction
		private AVideoPlayAction playAction
		private AVideoStopAction stopAction

		//! runtextmacro A_STRUCT_DEBUG("\"AVideo\"")

		//methods
		
		public method play takes nothing returns nothing
			local force playersAll
			debug if (AVideo.runningVideo != 0) then
				debug call this.print("Another Video is already running.")
			debug endif

			call CinematicFadeBJ(bj_CINEFADETYPE_FADEOUT, AVideo.waitTime, "ReplaceableTextures\\CameraMasks\\Black_mask.blp", 100.00, 100.00, 100.00, 0.0)
			call TriggerSleepAction(AVideo.waitTime)
			call ClearSelection()
			call ACharacter.setAllMovable(false)
			set playersAll = GetPlayersAll()
			call CinematicModeExBJ(true, playersAll, 0.0)
			set playersAll = null
			set AVideo.runningVideo = this
			if (this.initAction != 0) then
				call this.initAction.execute(this)
			endif
			debug call Print("Before video")

			call CinematicFadeBJ(bj_CINEFADETYPE_FADEIN, AVideo.waitTime, "ReplaceableTextures\\CameraMasks\\Black_mask.blp", 100.00, 100.00, 100.00, 0.0)
			call TriggerSleepAction(AVideo.waitTime)

			call EnableTrigger(AVideo.skipTrigger)
			call EnableUserControl(true) //otherwise we could not catch the press event (just the escape key)
			debug if (this.playAction == 0) then
				debug call this.print("Play action is 0.")
				debug return
			debug endif
			call this.playAction.execute(this)
		endmethod

		/// You have to call this method at the end of your video action.
		/// Since there is an execution of the action, TriggerSleepAction functions will be ignored, so this method could not be called by the play method.
		public method stop takes nothing returns nothing
			local force playersAll
			call DisableTrigger(AVideo.skipTrigger) 
			call EnableUserControl(false)
			call CinematicFadeBJ(bj_CINEFADETYPE_FADEOUT, AVideo.waitTime, "ReplaceableTextures\\CameraMasks\\Black_mask.blp", 100.00, 100.00, 100.00, 0.0)
			call TriggerSleepAction(AVideo.waitTime)
			set playersAll = GetPlayersAll()
			call CinematicModeExBJ(false, playersAll, 0.0)
			call EnableUserControl(false)
			set playersAll = null
			call ResetToGameCamera(0.0)
			if (this.stopAction != 0) then
				call this.stopAction.execute(this)
			endif
			call CinematicFadeBJ(bj_CINEFADETYPE_FADEIN, AVideo.waitTime, "ReplaceableTextures\\CameraMasks\\Black_mask.blp", 100.00, 100.00, 100.00, 0.0)
			call TriggerSleepAction(AVideo.waitTime)
			call EnableUserControl(true)

			call ACharacter.setAllMovable(true)
			set AVideo.runningVideo = 0
			set AVideo.skipped = false
			set AVideo.skippingPlayers = 0
			//No camera pan! Call it manually, please.
		endmethod
		
		/// Waits in video.
		/// @return Returns true if video was skipped
		public method wait takes real seconds returns boolean
			debug call this.print("Wait in video " + R2S(seconds) + ".")
			loop
				call TriggerSleepAction(AVideo.waitInterval)
				set seconds = seconds - AVideo.waitInterval
				debug call this.print("Remaining seconds: " + R2S(seconds) + ".")
				if (AVideo.skipped) then
					debug call this.print("Skip in wait method.")
					call this.stop()
					return true
				elseif (seconds <= 0) then
					debug call this.print("Was not skipped.")
					return false
				endif
			endloop
			return false
		endmethod

		public static method create takes AVideoInitAction initAction, AVideoPlayAction playAction, AVideoStopAction stopAction returns AVideo
			local AVideo this = AVideo.allocate()
			//start members
			set this.initAction = initAction
			set this.playAction = playAction
			set this.stopAction = stopAction

			return this
		endmethod
		
		private static method triggerConditionSkip takes nothing returns boolean
			debug if (AVideo.runningVideo == 0) then
				debug call AVideo.staticPrint("Running video is 0.")
			debug else
				debug call AVideo.staticPrint("Running video is not 0.")
			debug endif
			return AVideo.runningVideo != 0
		endmethod

		private static method triggerActionSkip takes nothing returns nothing
			local integer i
			local player triggerPlayer = GetTriggerPlayer()
			local player user
			local integer skipablePlayers = 0
			set AVideo.skippingPlayers = AVideo.skippingPlayers + 1
			call ACharacter.displayMessageToAll(ACharacter.messageTypeInfo, StringArg(AVideo.textPlayerSkips, GetPlayerName(triggerPlayer))) //AClassCharacterCharacter, ALibraryStringConversion
			set triggerPlayer = null

			//Jedes Mal neu berechnen, da Spieler as Spiel auch verlassen haben könnten.
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
					set user = Player(i)
					if (IsPlayerPlayingUser(user)) then //ALibraryGeneralPlayer
						set skipablePlayers = skipablePlayers + 1
					endif
					set user = null
				set i = i + 1
			endloop

			if (AVideo.skippingPlayers >= skipablePlayers / 2) then
				set AVideo.skipped = true
				call DisableTrigger(AVideo.skipTrigger) //do not allow skipping at twice!
				call ACharacter.displayMessageToAll(ACharacter.messageTypeInfo, AVideo.textSkip)
			endif
		endmethod

		private static method createSkipTrigger takes nothing returns nothing
			local integer i
			local player user
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set AVideo.skipTrigger = CreateTrigger()
			call DisableTrigger(AVideo.skipTrigger) //will be enabled by first running video
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then
					debug call AVideo.staticPrint("Create skip trigger for player " + I2S(i) + ".")
					set triggerEvent = TriggerRegisterKeyEventForPlayer(user, AVideo.skipTrigger, KEY_RIGHT, true) //ALibraryInterfaceMisc, important: If it is the escape key it is the same key as in the character selection.
					set triggerEvent = null
				endif
				set user = null
				set i = i + 1
			endloop
			set conditionFunction = Condition(function AVideo.triggerConditionSkip)
			set triggerCondition = TriggerAddCondition(AVideo.skipTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(AVideo.skipTrigger, function AVideo.triggerActionSkip)
			set triggerEvent = null
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		/// @param divident This value represents the divident which is used for comparing the number of skipping players with the number of requested skipping players for skipping the video.
		public static method init takes integer divident, real filterDuration, real waitInterval, string textPlayerSkips, string textSkip returns nothing
			//static start members
			set AVideo.divident = divident
			set AVideo.filterDuration = filterDuration
			set AVideo.waitInterval = waitInterval
			set AVideo.textPlayerSkips = textPlayerSkips
			set AVideo.textSkip = textSkip
			//static members
			set AVideo.waitTime = filterDuration / 2
			set AVideo.runningVideo = 0
			set AVideo.skipped = false
			set AVideo.skippingPlayers = 0

			call AVideo.createSkipTrigger()
		endmethod
		
		private static method destroySkipTrigger takes nothing returns nothing
			call AGetCharacterHashTable().destroyTrigger(AVideo.skipTrigger)
			set AVideo.skipTrigger = null
		endmethod
		
		public static method cleanUp takes nothing returns nothing
		
			call AVideo.destroySkipTrigger()
		endmethod
		
		//static members

		public static method wasSkipped takes nothing returns boolean
			debug if (AVideo.skipped) then
				debug call Print("Skipped is true.")
			debug endif
			return AVideo.skipped
		endmethod
		
		//static methods
		
		/// Waits until no video is running.
		/// @param interval Check interval.
		public static method waitForVideo takes real interval returns nothing
			loop
				exitwhen (AVideo.runningVideo == 0)
				call TriggerSleepAction(interval)
			endloop
		endmethod
	 endstruct

endlibrary