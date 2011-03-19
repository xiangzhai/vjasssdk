library AStructSystemsCharacterVideo requires optional ALibraryCoreDebugMisc, AStructCoreGeneralVector, ALibraryCoreGeneralPlayer, ALibraryCoreGeneralUnit, ALibraryCoreInterfaceCinematic, AStructCoreInterfacePlayerSelection, ALibraryCoreInterfaceMisc, ALibraryCoreStringConversion, AStructSystemsCharacterCharacter, AStructSystemsCharacterTalk

	private interface AActorInterface
		public method restore takes nothing returns nothing
		public method restoreOnActorsLocation takes nothing returns nothing
		public method actor takes nothing returns unit
	endinterface

	private struct AActorData extends AActorInterface
		// construction members
		private unit m_unit
		// members
		private unit m_actor

		// construction members

		public method unit takes nothing returns unit
			return this.m_unit
		endmethod

		// members

		public method actor takes nothing returns unit
			return this.m_actor
		endmethod

		// methods

		public method restore takes nothing returns nothing
			call RemoveUnit(this.m_actor)
			set this.m_actor = null
			call ShowUnit(this.m_unit, true)
		endmethod

		public method restoreOnActorsLocation takes nothing returns nothing
			call BJDebugMsg("restore on actors location")
			call SetUnitX(this.m_unit, GetUnitX(this.m_actor))
			call SetUnitY(this.m_unit, GetUnitY(this.m_actor))
			call SetUnitFacing(this.m_unit, GetUnitFacing(this.m_actor))
			call this.restore()
		endmethod

		public method refresh takes nothing returns nothing
			if (this.m_actor != null) then
				call RemoveUnit(this.m_actor)
				set this.m_actor = null
			endif
			set this.m_actor = CopyUnit(this.unit(), GetUnitX(this.unit()), GetUnitY(this.unit()), GetUnitFacing(this.unit()), bj_UNIT_STATE_METHOD_MAXIMUM)
			//call SetUnitOwner(this.m_actor, newOwner, false)
			call ShowUnit(this.m_actor, true)
			call PauseUnit(this.m_actor, false)
			call SelectUnit(this.m_actor, false)
			call SetUnitInvulnerable(this.m_actor, true)
			call IssueImmediateOrder(this.m_actor, "stop") // cancel orders.
		endmethod

		public static method create takes unit oldUnit returns thistype
			local thistype this = thistype.allocate()
			//local player newOwner = Player(PLAYER_NEUTRAL_PASSIVE) // set passive owner so unit won't attack or be attacked
			// construction members
			call ShowUnit(oldUnit, false)
			set this.m_unit = oldUnit
			// members
			call this.refresh()
			//set newOwner = null
			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			// construction members
			set this.m_unit = null
			// members
			if (this.m_actor != null) then
				call RemoveUnit(this.m_actor)
				set this.m_actor = null
			endif
		endmethod
	endstruct
	
	private struct AUnitTypeActorData extends AActorInterface
		// construction members
		private player m_owner
		private integer m_unitTypeId
		private real m_x
		private real m_y
		private real m_face
		// members
		private unit m_actor
		
		// construction members
		
		public method owner takes nothing returns player
			return this.m_owner
		endmethod
		
		public method unitTypeId takes nothing returns integer
			return this.m_unitTypeId
		endmethod
		
		public method x takes nothing returns real
			return this.m_x
		endmethod
		
		public method y takes nothing returns real
			return this.m_y
		endmethod
		
		public method face takes nothing returns real
			return this.m_face
		endmethod

		// members

		public method actor takes nothing returns unit
			return this.m_actor
		endmethod

		// methods

		public method restore takes nothing returns nothing
			call RemoveUnit(this.m_actor)
			set this.m_actor = null
		endmethod
		
		public method restoreOnActorsLocation takes nothing returns nothing
			call this.restore()
		endmethod

		public method refresh takes nothing returns nothing
			if (this.m_actor != null) then
				call RemoveUnit(this.actor())
				set this.m_actor = null
			endif
			set this.m_actor = CreateUnit(this.owner(), this.unitTypeId(), this.x(), this.y(), this.face())
			call SetUnitInvulnerable(this.m_actor, true)
		endmethod

		public static method create takes player owner, integer unitTypeId, real x, real y, real face returns thistype
			local thistype this = thistype.allocate()
			// construction members
			set this.m_owner = owner
			set this.m_unitTypeId = unitTypeId
			set this.m_x = x
			set this.m_y = y
			set this.m_face = face
			// members
			call this.refresh()
			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			// construction members
			set this.m_owner = null
			// members
			if (this.m_actor != null) then
				call RemoveUnit(this.m_actor)
				set this.m_actor = null
			endif
		endmethod
	endstruct

	private struct AVideoPlayerData
		private player m_player
		private APlayerSelection m_selection
		private boolean m_hadDialog

		public method store takes nothing returns nothing
			if (this.m_selection != 0) then
				call this.m_selection.destroy()
			endif
			set this.m_selection = APlayerSelection.create(this.m_player)
			call this.m_selection.store()
			if (AGui.playerGui(this.m_player).dialog().isDisplayed()) then
				set this.m_hadDialog = true
				call AGui.playerGui(this.m_player).dialog().hide()
			else
				set this.m_hadDialog = false
			endif
		endmethod

		public method restore takes nothing returns nothing
			if (this.m_selection != 0) then
				call this.m_selection.restore()
			endif
			if (this.m_hadDialog) then
				call AGui.playerGui(this.m_player).dialog().show()
			endif
		endmethod

		public static method create takes player whichPlayer returns thistype
			local thistype this = thistype.allocate()
			set this.m_player = whichPlayer
			set this.m_selection = 0
			set this.m_hadDialog = false

			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			if (this.m_selection != 0) then
				call this.m_selection.destroy()
			endif
		endmethod
	endstruct

	/**
	* Stores all necessary character data which has to be restored after video.
	*/
	private struct AVideoCharacterData
		// construction members
		private ACharacter m_character
		// members
		private boolean m_isMovable

		public method store takes nothing returns nothing
			set this.m_isMovable = this.m_character.isMovable()
		endmethod

		public method restore takes nothing returns nothing
			if (this.m_character.isMovable() != this.m_isMovable) then
				call this.m_character.setMovable(this.m_isMovable)
			endif
		endmethod

		public static method create takes ACharacter character returns thistype
			local thistype this = thistype.allocate()
			set this.m_character = character

			return this
		endmethod
	endstruct

	/// @todo Should be a part of @struct AVideo, vJass bug.
	function interface AVideoAction takes AVideo video returns nothing

	/**
	* Provides access to a global video. Global means that the video is played/shown for all character owners.
	* The ASL character system doesn't support local videos which means videos for each single character owner.
	* Videos can have initialization, play and stop actions which has to be defined as function interface functions.
	* User can easily save and restore actors data by using methods @method saveActor and @method restoreActor.
	* Additionally there is a method called actor which gives user access to an almost exact copy of the "first character".
	* The first character is always the character of first player in list which still is online. List is starting with player 1 (id 0).
	* Since you don't use character units (beside the copied one) they will be hidden in video initialization.
	* Besides all units will be paused so you have to unpause a unit if you want to give orders (like move) to it.
	* Videos can be skipped by pressing a user-defined key. If at least half of players want to skip a video (have pressed that key) it will be skipped.
	* @see wait, waitForVideo, waitForCondition, AVideoAction, AVideoCondition
	*/
	struct AVideo
		// static construction members
		private static integer m_divident
		private static real m_filterDuration
		private static string m_textPlayerSkips
		private static string m_textSkip
		// static members
		private static real m_waitTime
		private static thistype m_runningVideo /// Do not access.
		private static sound m_playedSound
		private static boolean m_skipped
		private static integer m_skippingPlayers
		private static trigger m_skipTrigger
		private static AActorData m_actor //copy of first character
		private static AVideoPlayerData array m_playerData[12] /// @todo bj_MAX_PLAYERS
		private static AVideoCharacterData array m_playerCharacterData[12] /// @todo bj_MAX_PLAYERS
		private static AIntegerVector m_actorData
		private static real m_timeOfDay
		// dynamic members
		private AVideoAction m_initAction
		private AVideoAction m_playAction
		private AVideoAction m_stopAction
		private AVideoAction m_skipAction

		//! runtextmacro optional A_STRUCT_DEBUG("\"AVideo\"")

		// dynamic members

		public method setInitAction takes AVideoAction initAction returns nothing
			set this.m_initAction = initAction
		endmethod

		public method initAction takes nothing returns AVideoAction
			return this.m_initAction
		endmethod

		public method setPlayAction takes AVideoAction playAction returns nothing
			set this.m_playAction = playAction
		endmethod

		public method playAction takes nothing returns AVideoAction
			return this.m_playAction
		endmethod

		public method setStopAction takes AVideoAction stopAction returns nothing
			set this.m_stopAction = stopAction
		endmethod

		public method stopAction takes nothing returns AVideoAction
			return this.m_stopAction
		endmethod

		public method setSkipAction takes AVideoAction skipAction returns nothing
			set this.m_skipAction = skipAction
		endmethod

		public method skipAction takes nothing returns AVideoAction
			return this.m_skipAction
		endmethod

		// methods

		private static method savePlayerData takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				if (IsPlayerPlayingUser(Player(i))) then
					if (thistype.m_playerData[i] == 0) then
						set thistype.m_playerData[i] = AVideoPlayerData.create(Player(i))
					endif
					call thistype.m_playerData[i].store()
				endif
				if (ACharacter.playerCharacter(Player(i)) != 0) then
					if (thistype.m_playerCharacterData[i] == 0) then
						set thistype.m_playerCharacterData[i] = AVideoCharacterData.create(ACharacter.playerCharacter(Player(i)))
					endif
					call thistype.m_playerCharacterData[i].store()
				endif
				set i = i + 1
			endloop
		endmethod

		private static method restorePlayerData takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				if (thistype.m_playerData[i] != 0) then
					call thistype.m_playerData[i].restore()
				endif
				if (thistype.m_playerCharacterData[i] != 0) then
					if (ACharacter.playerCharacter(Player(i)) != 0) then
						call thistype.m_playerCharacterData[i].restore()
					endif
				endif
				set i = i + 1
			endloop
		endmethod

		public stub method onInitAction takes nothing returns nothing
			if (this.m_initAction != 0) then
				call this.m_initAction.evaluate(this) // evaluate since it is called between fading
			endif
		endmethod

		public stub method onPlayAction takes nothing returns nothing
			if (this.m_playAction != 0) then
				call this.m_playAction.execute(this) // execute since we need to be able to use TriggerSleepAction calls (stop has to be called in this function)
			endif
		endmethod

		public stub method onStopAction takes nothing returns nothing
			if (this.m_stopAction != 0) then
				call this.m_stopAction.evaluate(this) // evaluate since it is called between fading
			endif
		endmethod

		public stub method onSkipAction takes nothing returns nothing
			if (this.m_skipAction != 0) then
				call this.m_skipAction.evaluate(this) // evaluate since it is called before stop action
			endif
		endmethod

		/**
		* In addition to the usual game properties which are stored and restored by function CinematicModeExBJ the following things will be stored by this method and restored by AVideo.stop:
		* <ul>
		* <li>if any dialog was shown to a player</li>
		* <li>player selection</li>
		* <li>character movability</li>
		* <li>time of day</li>
		* </ul>
		* @see CinematicModeExBJ
		*/
		public method play takes nothing returns nothing
			local force playersAll
			debug if (thistype.m_runningVideo != 0) then
				debug call this.print("Another Video is already being run.")
				debug return
			debug endif
			set thistype.m_playedSound = null
			call CinematicFadeBJ(bj_CINEFADETYPE_FADEOUT, thistype.m_waitTime, "ReplaceableTextures\\CameraMasks\\Black_mask.blp", 100.00, 100.00, 100.00, 0.0)
			call TriggerSleepAction(thistype.m_waitTime)
			/// @todo disable experience gain of all characters?
			call thistype.savePlayerData()
			set thistype.m_timeOfDay = GetTimeOfDay()
			if (ATalk.initialized() and ATalk.disableEffectsInCinematicMode()) then
				call ATalk.hideAllEffects()
			endif
			call ClearSelection()
			call ACharacter.setAllMovable(false)
			call ACharacter.showAll(false)
			debug call Print("Before pausing all units.")
			call PauseAllUnits(true)
			debug call Print("After pausing all units.")
			if (thistype.m_actor != 0) then
				call thistype.m_actor.refresh()
			endif
			call SetCameraBoundsToRect(bj_mapInitialPlayableArea) // for all players
			set playersAll = GetPlayersAll()
			call CinematicModeBJ(true, playersAll) // Never use with value 0.0, unit portraits won't work anymore -> m_waitTime should be bigger than or equal to bj_CINEMODE_INTERFACEFADE
			//call CinematicModeExBJ(true, playersAll, 0.0)
			set playersAll = null
			set thistype.m_runningVideo = this
			call this.onInitAction.evaluate()
			call CinematicFadeBJ(bj_CINEFADETYPE_FADEIN, thistype.m_waitTime, "ReplaceableTextures\\CameraMasks\\Black_mask.blp", 100.00, 100.00, 100.00, 0.0)
			call TriggerSleepAction(thistype.m_waitTime)
			call EnableTrigger(thistype.m_skipTrigger)
			//call EnableUserControl(true) //otherwise we could not catch the press event (just the escape key)
			call this.onPlayAction.execute() // execute since we need to be able to use TriggerSleepAction calls (stop method has to be called in this method)
		endmethod

		/// You have to call this method at the end of your video action.
		/// Since there is an execution of the action, TriggerSleepAction functions will be ignored, so this method could not be called by the play method.
		public method stop takes nothing returns nothing
			local force playersAll
			debug if (thistype.m_runningVideo != this) then
				debug call this.print("Video is not being run.")
				debug return
			debug endif
			call DisableTrigger(thistype.m_skipTrigger)
			call CinematicFadeBJ(bj_CINEFADETYPE_FADEOUT, thistype.m_waitTime, "ReplaceableTextures\\CameraMasks\\Black_mask.blp", 100.00, 100.00, 100.00, 0.0)
			call TriggerSleepAction(thistype.m_waitTime)
			set playersAll = GetPlayersAll()
			call CinematicModeBJ(false, playersAll) // Never use with value 0.0, unit portraits won't work anymore -> m_waitTime should be bigger than or equal to bj_CINEMODE_INTERFACEFADE
			//call CinematicModeExBJ(true, playersAll, 0.0)
			set playersAll = null
			if (ATalk.initialized() and ATalk.disableEffectsInCinematicMode()) then
				call ATalk.showAllEffects()
			endif
			call ResetToGameCamera(0.0)
			if (thistype.m_actor != 0) then
				call thistype.m_actor.restore()
				call thistype.m_actor.destroy()
				set thistype.m_actor = 0
			endif
			call ACharacter.showAll(true)
			debug call Print("Before pausing all units.")
			call PauseAllUnits(false)
			debug call Print("After pausing all units.")
			call this.onStopAction.evaluate()
			call CinematicFadeBJ(bj_CINEFADETYPE_FADEIN, thistype.m_waitTime, "ReplaceableTextures\\CameraMasks\\Black_mask.blp", 100.00, 100.00, 100.00, 0.0)
			call SetTimeOfDay(thistype.m_timeOfDay)
			call TriggerSleepAction(thistype.m_waitTime)
			call thistype.restorePlayerData()
			set thistype.m_runningVideo = 0
			set thistype.m_skipped = false
			set thistype.m_skippingPlayers = 0
			//No camera pan! Call it manually, please.
		endmethod

		public method skip takes nothing returns nothing
			debug if (thistype.m_runningVideo != this) then
				debug call this.print("Video is not being run.")
				debug return
			debug endif
			if (thistype.m_playedSound != null) then
				call StopSound(thistype.m_playedSound, false, false)
				set thistype.m_playedSound = null
			endif
			call EndCinematicScene()
			set thistype.m_skipped = true
			call DisableTrigger(thistype.m_skipTrigger) // do not allow skipping at twice!
			call ACharacter.displayMessageToAll(ACharacter.messageTypeInfo, thistype.m_textSkip)
			call this.onSkipAction.evaluate()
			call this.stop.execute() // execute, since it uses TriggerSleepAction calls
		endmethod

		/**
		* Usually there must be at least playing players / divident of playing players who want to skip the video so that it will be skipped.
		* You can overwrite this method in your custom derived structure to avoid this default behaviour.
		* This method is called every time a player skips the video.
		*/
		public stub method onSkip takes integer skipablePlayers returns boolean
			if (thistype.m_skippingPlayers >= skipablePlayers / thistype.m_divident) then
				call this.skip()
				return true
			endif
			return false
		endmethod

		public static method create takes nothing returns thistype
			local thistype this = thistype.allocate()
			// dynamic members
			set this.m_initAction = 0
			set this.m_playAction = 0
			set this.m_stopAction = 0
			set this.m_skipAction = 0

			return this
		endmethod

		public static method playerSkips takes player whichPlayer returns boolean
			local integer i
			local player user
			local integer skipablePlayers = 0

			if (thistype.m_runningVideo == 0 or thistype.m_skipped) then
				return false
			endif

			set thistype.m_skippingPlayers = thistype.m_skippingPlayers + 1
			call ACharacter.displayMessageToAll(ACharacter.messageTypeInfo, StringArg(thistype.m_textPlayerSkips, GetPlayerName(whichPlayer)))

			// recalculate every time since players could have left the game by now
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
					set user = Player(i)
					if (IsPlayerPlayingUser(user)) then
						set skipablePlayers = skipablePlayers + 1
					endif
					set user = null
				set i = i + 1
			endloop

			return thistype.m_runningVideo.onSkip.evaluate(skipablePlayers)
		endmethod

		public static method transmissionFromUnitType takes integer unitType, player owner, string name, string text, sound playedSound returns nothing
			set thistype.m_playedSound = playedSound
			call TransmissionFromUnitType(unitType, owner, name, text, playedSound)
		endmethod

		public static method transmissionFromUnit takes unit whichUnit, string text, sound playedSound returns nothing
			set thistype.m_playedSound = playedSound
			call TransmissionFromUnit(whichUnit, text, playedSound)
		endmethod

		private static method triggerConditionSkip takes nothing returns boolean
			return thistype.m_runningVideo != 0
		endmethod

		private static method triggerActionSkip takes nothing returns nothing
			call thistype.playerSkips(GetTriggerPlayer())
		endmethod

		private static method createSkipTrigger takes nothing returns nothing
			local integer i
			local player user
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set thistype.m_skipTrigger = CreateTrigger()
			call DisableTrigger(thistype.m_skipTrigger) //will be enabled by first running video
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then
					set triggerEvent = TriggerRegisterKeyEventForPlayer(user, thistype.m_skipTrigger, AKeyEscape, true) //important: If it is the escape key it is the same key as in the character selection.
					set triggerEvent = null
				endif
				set user = null
				set i = i + 1
			endloop
			set conditionFunction = Condition(function thistype.triggerConditionSkip)
			set triggerCondition = TriggerAddCondition(thistype.m_skipTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(thistype.m_skipTrigger, function thistype.triggerActionSkip)
			set triggerEvent = null
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		/// @param divident This value represents the divident which is used for comparing the number of skipping players with the number of requested skipping players for skipping the video.
		public static method init takes integer divident, real filterDuration, string textPlayerSkips, string textSkip returns nothing
			local integer i
			// static construction members
			set thistype.m_divident = divident
			set thistype.m_filterDuration = filterDuration
			set thistype.m_textPlayerSkips = textPlayerSkips
			set thistype.m_textSkip = textSkip
			// static members
			set thistype.m_waitTime = filterDuration / 2
			debug if (thistype.m_waitTime < bj_CINEMODE_INTERFACEFADE) then
				debug call thistype.staticPrint("Wait time should be equal to or bigger than bj_CINEMODE_INTERFACEFADE (" + R2S(bj_CINEMODE_INTERFACEFADE) + " but it has value " + R2S(thistype.m_waitTime) + ".")
			debug endif
			set thistype.m_playedSound = null
			set thistype.m_runningVideo = 0
			set thistype.m_skipped = false
			set thistype.m_skippingPlayers = 0
			set thistype.m_actor = 0
			set thistype.m_actorData = AIntegerVector.create()
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set thistype.m_playerData[i] = 0
				set thistype.m_playerCharacterData[i] = 0
				set i = i + 1
			endloop
			set thistype.m_timeOfDay = 0.0

			call thistype.createSkipTrigger()
		endmethod

		private static method destroySkipTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(thistype.m_skipTrigger)
			set thistype.m_skipTrigger = null
		endmethod

		public static method cleanUp takes nothing returns nothing
			local integer i
			// static members
			loop
				exitwhen (thistype.m_actorData.empty())
				call AActorData(thistype.m_actorData.back()).destroy()
				call thistype.m_actorData.popBack()
			endloop
			call thistype.m_actorData.destroy()

			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				if (thistype.m_playerData[i] != 0) then
					call thistype.m_playerData[i].destroy()
				endif
				if (thistype.m_playerCharacterData[i] != 0) then
					call thistype.m_playerCharacterData[i].destroy()
				endif
				set i = i + 1
			endloop

			call thistype.destroySkipTrigger()
		endmethod

		// static members

		public static method runningVideo takes nothing returns thistype
			return thistype.m_runningVideo
		endmethod

		public static method skipped takes nothing returns boolean
			return thistype.m_skipped
		endmethod

		// static methods

		public static method isRunning takes nothing returns boolean
			return thistype.m_runningVideo != 0
		endmethod

		public static method actor takes nothing returns unit
			debug if (thistype.m_runningVideo == 0) then
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
		
		public static method createUnitActor takes player owner, integer unitTypeId, real x, real y, real face returns integer
			local AUnitTypeActorData data = AUnitTypeActorData.create(owner, unitTypeId, x, y, face)
			call thistype.m_actorData.pushBack(data)
			return thistype.m_actorData.backIndex()
		endmethod
		
		public static method createUnitActorAtLocation takes player owner, integer unitTypeId, location whichLocation, real face returns integer
			return thistype.createUnitActor(owner, unitTypeId, GetLocationX(whichLocation), GetLocationY(whichLocation), face)
		endmethod
		
		public static method createUnitActorAtRect takes player owner, integer unitTypeId, rect whichRect, real face returns integer
			return thistype.createUnitActor(owner, unitTypeId, GetRectCenterX(whichRect), GetRectCenterY(whichRect), face)
		endmethod

		public static method unitActor takes integer index returns unit
			return AActorData(thistype.m_actorData[index]).actor()
		endmethod

		public static method restoreUnitActor takes integer index returns nothing
			call AActorData(thistype.m_actorData[index]).restore()
			call AActorData(thistype.m_actorData[index]).destroy()
			call thistype.m_actorData.erase(index)
		endmethod

		public static method restoreUnitActorOnActorLocation takes integer index returns nothing
			call AActorData(thistype.m_actorData[index]).restoreOnActorsLocation()
			call AActorData(thistype.m_actorData[index]).destroy()
			call thistype.m_actorData.erase(index)
		endmethod

		public static method restoreUnitActors takes nothing returns nothing
			loop
				exitwhen (thistype.m_actorData.empty())
				call thistype.restoreUnitActor(thistype.m_actorData.backIndex())
			endloop
		endmethod

		public static method restoreUnitActorsOnActorsLocations takes nothing returns nothing
			loop
				exitwhen (thistype.m_actorData.empty())
				call thistype.restoreUnitActorOnActorLocation(thistype.m_actorData.backIndex())
			endloop
		endmethod

		public static method setActorsMoveSpeed takes real moveSpeed returns nothing
			local integer i = 0
			call SetUnitMoveSpeed(thistype.actor(), moveSpeed)
			loop
				exitwhen (i == thistype.m_actorData.size())
				call SetUnitMoveSpeed(AActorData(thistype.m_actorData[i]).actor(), moveSpeed)
				set i = i + 1
			endloop
		endmethod
	 endstruct

	/**
	* Waits (synchronized) until no video is running anymore.
	* @param interval Check interval.
	*/
	function waitForVideo takes real interval returns nothing
		loop
			exitwhen (AVideo.runningVideo() == 0)
			//call TriggerSleepAction(interval)
			call PolledWait(interval) // synchron waiting, important for multiplayer games
		endloop
	endfunction
	
	private function WaitCondition takes nothing returns boolean
		return AVideo.skipped()
	endfunction

	/**
	* Waits \p seconds game-time seconds. Cancels if video is skipped during this time.
	* Note that this function is like \ref PolledWait since it has to be synchronos.
	* \return Returns true if video was skipped during the wait phase. Otherwise it returns false (if wait time has expired normally).
	* \see PolledWait
	*/
	function wait takes real seconds returns boolean
		return WaitCheckingCondition(seconds, function WaitCondition, 0)
	endfunction

	/**
	* Condition function interface for video conditions which can be checked during wait phase.
	* \sa waitForCondition
	*/
	function interface AVideoCondition takes AVideo video returns boolean

	/**
	* Advanced conditional video wait function.
	* Checks every \p interval seconds for condition \p condition. If video is being skipped during this time it returns true.
	* Otherwise it returns false when condition is true.
	* \param interval Interval in seconds in which the condition will be checked.
	* \param condition Condition which will be checked. Use \ref AVideoCondition to create and pass a correct function.
	* \return Returns true if the video had been skipped before condition became true. Otherwise it returns false when condition becomes true.
	* \sa AVideoCondition
	*/
	function waitForCondition takes real interval, AVideoCondition condition returns boolean
		loop
			exitwhen (condition.evaluate(AVideo.runningVideo()))
			if (WaitCondition()) then
				return true
			endif
			call TriggerSleepAction(interval)
		endloop
		return false
	endfunction
endlibrary