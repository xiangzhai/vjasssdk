library AStructSystemsCharacterVideo requires ALibraryCoreDebugMisc, AStructCoreGeneralVector, ALibraryCoreGeneralPlayer, ALibraryCoreGeneralUnit, AStructCoreInterfacePlayerSelection, ALibraryCoreInterfaceMisc, ALibraryCoreStringConversion, AStructSystemsCharacterCharacter

	private struct AActorData
		//start members
		private unit m_unit
		//members
		private unit m_actor
		
		//start members
		
		public method unit takes nothing returns unit
			return this.m_unit
		endmethod
		
		//members
		
		public method actor takes nothing returns unit
			return this.m_actor
		endmethod
		
		//methods
		
		public method restore takes nothing returns nothing
			call RemoveUnit(this.m_actor)
			set this.m_actor = null
			call ShowUnit(this.m_unit, true)
		endmethod
		
		public static method create takes unit oldUnit returns thistype
			local thistype this = thistype.allocate()
			local player newOwner = Player(PLAYER_NEUTRAL_PASSIVE) // set passive owner so unit won't attack or be attacked
			//start members
			call ShowUnit(oldUnit, false)
			set this.m_unit = oldUnit
			//members
			set this.m_actor = CopyUnit(oldUnit, GetUnitX(oldUnit), GetUnitY(oldUnit), GetUnitFacing(oldUnit), bj_UNIT_STATE_METHOD_MAXIMUM)
			call SetUnitOwner(this.m_actor, newOwner, false)
			call ShowUnit(this.m_actor, true)
			if (IsUnitHidden(this.m_actor)) then
				call ShowUnit(this.m_actor, true)
			endif
			if (IsUnitPaused(this.m_actor)) then
				call PauseUnit(this.m_actor, false)
			endif
			call SetUnitInvulnerable(this.m_actor, true)
			call IssueImmediateOrder(this.m_actor, "stop") //cancel orders.
			set newOwner = null
			return this
		endmethod
		
		public method onDestroy takes nothing returns nothing
			//start members
			set this.m_unit = null
			//members
			if (this.m_actor != null) then
				call RemoveUnit(this.m_actor)
				set this.m_actor = null
			endif
		endmethod
	endstruct

	//! runtextmacro A_VECTOR("private", "AActorDataVector", "AActorData", "0", "100")
	
	/// @todo Should be a part of @struct AVideo, vJass bug.
	function interface AVideoInitAction takes AVideo video returns nothing

	/// @todo Should be a part of @struct AVideo, vJass bug.
	function interface AVideoPlayAction takes AVideo video returns nothing
	
	/// @todo Should be a part of @struct AVideo, vJass bug.
	function interface AVideoStopAction takes AVideo video returns nothing

	/// Waits until no video is running.
	/// @param interval Check interval.
	function waitForVideo takes real interval returns nothing
		loop
			exitwhen (AVideo.runningVideo == 0)
			call TriggerSleepAction(interval)
		endloop
	endfunction

	/// Provides access to a global video. Global means that the video is played/shown for all character owners.
	/// The ASL character system doesn't support local videos which means videos for each single character owner.
	/// Videos can have initialization, play and stop actions which has to be defined as function interface functions.
	/// User can easily save and restore actors data by using methods @method saveActor and @method restoreActor.
	/// Additionally there is a method called @method actor which gives user access to an almost exact copy of the "first character".
	/// The first character is always the character of first player in list which still is online. List is starting with player 1 (id 0).
	/// Since you don't use character units (beside the copied one) they will be hidden in video initialization.
	/// Besides all units will be paused so you have to unpause a unit if you want to give orders (like move) to it.
	/// Videos can be skipped by pressing a user-defined key. If at least half of players want to skip a video (have pressed that key) it will be skipped.
	struct AVideo
		//static start members
		private static integer divident
		private static real filterDuration
		private static real waitInterval
		private static string textPlayerSkips
		private static string textSkip
		//static members
		private static real waitTime
		public static thistype runningVideo /// Do not access.
		private static boolean skipped
		private static integer skippingPlayers
		private static trigger skipTrigger
		private static AActorData m_actor //copy of first character
		private static APlayerSelection array m_playerSelection[12] /// @todo bj_MAX_PLAYERS
		private static boolean array m_playerHadDialog[12] /// @todo bj_MAX_PLAYERS
		private static AActorDataVector m_actorData
		//start members
		private AVideoInitAction initAction
		private AVideoPlayAction playAction
		private AVideoStopAction stopAction

		//! runtextmacro A_STRUCT_DEBUG("\"AVideo\"")

		//methods
		
		public method play takes nothing returns nothing
			local force playersAll
			debug if (thistype.runningVideo != 0) then
				debug call this.print("Another Video is already running.")
			debug endif

			call CinematicFadeBJ(bj_CINEFADETYPE_FADEOUT, thistype.waitTime, "ReplaceableTextures\\CameraMasks\\Black_mask.blp", 100.00, 100.00, 100.00, 0.0)
			call TriggerSleepAction(thistype.waitTime)
			/// @todo disable experience gain of all characters?
			call thistype.savePlayerData()
			call ClearSelection()
			call ACharacter.setAllMovable(false)
			call ACharacter.showAll(false)
			call PauseAllUnitsBJ(true)
			set playersAll = GetPlayersAll()
			call CinematicModeExBJ(true, playersAll, 0.0)
			set playersAll = null
			set thistype.runningVideo = this
			if (this.initAction != 0) then
				call this.initAction.execute(this)
			endif
			//debug call Print("Before video")

			call CinematicFadeBJ(bj_CINEFADETYPE_FADEIN, thistype.waitTime, "ReplaceableTextures\\CameraMasks\\Black_mask.blp", 100.00, 100.00, 100.00, 0.0)
			call TriggerSleepAction(thistype.waitTime)

			call EnableTrigger(thistype.skipTrigger)
			//call EnableUserControl(true) //otherwise we could not catch the press event (just the escape key)
			//debug if (this.playAction == 0) then
				//debug call this.print("Play action is 0.")
				//debug return
			//debug endif
			call this.playAction.execute(this)
		endmethod

		/// You have to call this method at the end of your video action.
		/// Since there is an execution of the action, TriggerSleepAction functions will be ignored, so this method could not be called by the play method.
		public method stop takes nothing returns nothing
			local force playersAll
			debug call Print("Stop!")
			call DisableTrigger(thistype.skipTrigger) 
			call EnableUserControl(false)
			call CinematicFadeBJ(bj_CINEFADETYPE_FADEOUT, thistype.waitTime, "ReplaceableTextures\\CameraMasks\\Black_mask.blp", 100.00, 100.00, 100.00, 0.0)
			call TriggerSleepAction(thistype.waitTime)
			set playersAll = GetPlayersAll()
			call CinematicModeExBJ(false, playersAll, 0.0)
			call EnableUserControl(false)
			set playersAll = null
			call ResetToGameCamera(0.0)
			if (thistype.m_actor != 0) then
				call thistype.m_actor.restore()
				call thistype.m_actor.destroy()
				set thistype.m_actor = 0
			endif
			call ACharacter.showAll(true)
			call PauseAllUnitsBJ(false)
			if (this.stopAction != 0) then
				call this.stopAction.execute(this)
			endif
			call CinematicFadeBJ(bj_CINEFADETYPE_FADEIN, thistype.waitTime, "ReplaceableTextures\\CameraMasks\\Black_mask.blp", 100.00, 100.00, 100.00, 0.0)
			call TriggerSleepAction(thistype.waitTime)
			call thistype.restorePlayerData()
			call EnableUserControl(true)

			call ACharacter.setAllMovable(true)
			set thistype.runningVideo = 0
			set thistype.skipped = false
			set thistype.skippingPlayers = 0
			//No camera pan! Call it manually, please.
		endmethod
		
		/// Waits in video.
		/// @return Returns true if video was skipped
		public method wait takes real seconds returns boolean
			//debug call this.print("Wait in video " + R2S(seconds) + ".")
			loop
				call TriggerSleepAction(thistype.waitInterval)
				set seconds = seconds - thistype.waitInterval
				//debug call this.print("Remaining seconds: " + R2S(seconds) + ".")
				if (thistype.skipped) then
					return true
				elseif (seconds <= 0) then
					//debug call this.print("Was not skipped.")
					return false
				endif
			endloop
			return false
		endmethod

		public static method create takes AVideoInitAction initAction, AVideoPlayAction playAction, AVideoStopAction stopAction returns thistype
			local thistype this = thistype.allocate()
			//start members
			set this.initAction = initAction
			set this.playAction = playAction
			set this.stopAction = stopAction

			return this
		endmethod
		
		private static method triggerConditionSkip takes nothing returns boolean
			//debug if (thistype.runningVideo == 0) then
				//debug call thistype.staticPrint("Running video is 0.")
			//debug else
				//debug call thistype.staticPrint("Running video is not 0.")
			//debug endif
			return thistype.runningVideo != 0
		endmethod

		private static method triggerActionSkip takes nothing returns nothing
			local integer i
			local player triggerPlayer = GetTriggerPlayer()
			local player user
			local integer skipablePlayers = 0
			set thistype.skippingPlayers = thistype.skippingPlayers + 1
			call ACharacter.displayMessageToAll(ACharacter.messageTypeInfo, StringArg(thistype.textPlayerSkips, GetPlayerName(triggerPlayer))) //AClassCharacterCharacter, ALibraryStringConversion
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

			if (thistype.skippingPlayers >= skipablePlayers / 2) then
				set thistype.skipped = true
				call DisableTrigger(thistype.skipTrigger) //do not allow skipping at twice!
				call ACharacter.displayMessageToAll(ACharacter.messageTypeInfo, thistype.textSkip)
				call thistype.runningVideo.stop()
			endif
		endmethod

		private static method createSkipTrigger takes nothing returns nothing
			local integer i
			local player user
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set thistype.skipTrigger = CreateTrigger()
			call DisableTrigger(thistype.skipTrigger) //will be enabled by first running video
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then
					//debug call thistype.staticPrint("Create skip trigger for player " + I2S(i) + ".")
					set triggerEvent = TriggerRegisterKeyEventForPlayer(user, thistype.skipTrigger, KEY_ESCAPE, true) //ALibraryInterfaceMisc, important: If it is the escape key it is the same key as in the character selection.
					set triggerEvent = null
				endif
				set user = null
				set i = i + 1
			endloop
			set conditionFunction = Condition(function thistype.triggerConditionSkip)
			set triggerCondition = TriggerAddCondition(thistype.skipTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(thistype.skipTrigger, function thistype.triggerActionSkip)
			set triggerEvent = null
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		/// @param divident This value represents the divident which is used for comparing the number of skipping players with the number of requested skipping players for skipping the video.
		public static method init takes integer divident, real filterDuration, real waitInterval, string textPlayerSkips, string textSkip returns nothing
			local integer i
			local player user
			//static start members
			set thistype.divident = divident
			set thistype.filterDuration = filterDuration
			set thistype.waitInterval = waitInterval
			set thistype.textPlayerSkips = textPlayerSkips
			set thistype.textSkip = textSkip
			//static members
			set thistype.waitTime = filterDuration / 2
			set thistype.runningVideo = 0
			set thistype.skipped = false
			set thistype.skippingPlayers = 0
			set thistype.m_actor = 0
			set thistype.m_actorData = AActorDataVector.create()
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then
					set thistype.m_playerSelection[i] = 0
				endif
				set user = null
				set i = i + 1
			endloop

			call thistype.createSkipTrigger()
		endmethod
		
		private static method destroySkipTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(thistype.skipTrigger)
			set thistype.skipTrigger = null
		endmethod
		
		public static method cleanUp takes nothing returns nothing
			local integer i = thistype.m_actorData.backIndex()
			//static members
			loop
				exitwhen (i < 0)
				call thistype.m_actorData[i].destroy()
				set i = i - 1
			endloop
			call thistype.m_actorData.destroy()
		
			call thistype.destroySkipTrigger()
		endmethod
		
		//static members

		public static method wasSkipped takes nothing returns boolean
			//debug if (AVideo.skipped) then
				//debug call Print("Skipped is true.")
			//debug endif
			return thistype.skipped
		endmethod
		
		public static method actor takes nothing returns unit
			debug if (thistype.runningVideo == 0) then
				debug call thistype.staticPrint("Running video is 0.")
			debug endif
			if (thistype.m_actor == 0) then
				set thistype.m_actor = AActorData.create(ACharacter.getFirstCharacter().unit())
			endif
			return thistype.m_actor.actor()
		endmethod
		
		public static method saveUnitActor takes unit actor returns integer
			local AActorData data = AActorData.create(actor)
			call thistype.m_actorData.pushBack(data)
			return thistype.m_actorData.backIndex()
		endmethod
		
		public static method unitActor takes integer index returns unit
			return thistype.m_actorData[index].actor()
		endmethod
		
		public static method restoreUnitActor takes integer index returns boolean
			call thistype.m_actorData[index].restore()
			call thistype.m_actorData[index].destroy()
			call thistype.m_actorData.erase(index)
			return true
		endmethod
		
		public static method restoreUnitActors takes nothing returns nothing
			local integer i = thistype.m_actorData.backIndex()
			loop
				exitwhen (i < 0)
				call thistype.restoreUnitActor(i)
				set i = i - 1
			endloop
		endmethod
		
		private static method savePlayerData takes nothing returns nothing
			local player user
			local integer i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then
					if (thistype.m_playerSelection[i] == 0) then
						set thistype.m_playerSelection[i] = APlayerSelection.create(user)
					endif
					call thistype.m_playerSelection[i].save()
					if (AGui.playerGui(user).dialog().isDisplayed()) then
						set thistype.m_playerHadDialog[i] = true
						call AGui.playerGui(user).dialog().hide()
					else
						set thistype.m_playerHadDialog[i] = false
					endif
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod
		
		private static method restorePlayerData takes nothing returns nothing
			local player user
			local integer i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then
					if (thistype.m_playerHadDialog[i]) then
						call AGui.playerGui(user).dialog().show()
					endif
				endif
				if (thistype.m_playerSelection[i] != 0) then
					call thistype.m_playerSelection[i].restore()
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod
	 endstruct

endlibrary