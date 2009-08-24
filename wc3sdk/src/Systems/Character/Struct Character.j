library AStructSystemsCharacterCharacter requires ALibraryCoreDebugMisc,AStructCoreGeneralHashTable, AStructCoreGeneralVector, ALibraryCoreGeneralPlayer, ALibraryCoreGeneralUnit, ALibraryCoreInterfaceCinematicFilter, ALibraryCoreInterfaceCamera, AStructSystemsCharacterAbstractCharacterSystem

	/**
	* This struct represents a single RPG character. Each player can own exactly one character.
	* You can configure the character systems and enable or disable the several character system modules.
	* Each module makes the system requiring more memory, so clearly think of which system modules are required and which aren't.
	*/
	struct ACharacter
		//static constant members
		public static constant integer messageTypeInfo = 0
		public static constant integer messageTypeError = 1
		public static constant integer maxSpells = 100
		//static start members
		private static boolean m_removeUnitOnDestruction
		private static boolean m_destroyOnPlayerLeaves
		private static boolean m_destroyOnDeath
		private static boolean m_useViewSystem
		private static boolean m_useFocusSystem
		private static boolean m_useMovementSystem
		private static boolean m_useFightSystem
		private static boolean m_useRevivalSystem
		private static boolean m_useInventorySystem
		private static boolean m_useTalkLogSystem
		//static members
		private static thistype array m_playerCharacter[12] //[bj_MAX_PLAYERS] vjass bug
		//dynamic members
		private boolean m_isMovable
		private AClass m_class //No system
		private ATalk m_talk //No system
		private AShrine m_shrine //No system
		//start members
		private player m_user
		private unit m_unit
		//members
		private trigger m_leaveTrigger
		private trigger m_deathTrigger
		//insert the character systems here
		private AView m_view
		private AFocus m_focus
		private AMovement m_movement
		private AFight m_fight
		private ARevival m_revival
		private AInventory m_inventory
		private ATalkLog m_talkLog
		private AIntegerVector m_spells
		
		//! runtextmacro A_STRUCT_DEBUG("\"ACharacter\"")

		//dynamic members

		/**
		* If the character is set unmovable he will be stopped immediatly and can not move until he will be set movable.
		* This method is used for dialogs or cinematic sequences.
		* Besides it disables all character systems!
		*/
		public method setMovable takes boolean movable returns nothing
			//Don't make him movable if he is already!
			//If he talks with another NPC he is already unmovable.
			debug if (this.m_isMovable == movable) then
				debug call this.print("setMovable() error.")
				debug return
			debug endif
			set this.m_isMovable = movable
			call IssueImmediateOrder(this.m_unit, "stop") //new, required?!
			call PauseUnit(this.m_unit, not movable)
			call SetUnitInvulnerable(this.m_unit, not movable)
			if (movable) then
				call this.enableMovableSystems()
			else
				call this.disableMovableSystems()
			endif
		endmethod

		/**
		* @return Returns if the character is movable. Unmovable characters are paused and invulnerable. Additionally all systems are disabled.
		* @see setMovable
		*/
		public method isMovable takes nothing returns boolean
			return this.m_isMovable
		endmethod

		/// Friend relation to AClassSelection, don't use.
		public method setClass takes AClass class returns nothing
			set this.m_class = class
		endmethod

		/**
		* @return Returns class of the character. Classes are used by class selection which allows user to choose between different classes/unit types which are during the game.
		* @see AClass
		* @see AClassSelection 
		*/
		public method class takes nothing returns AClass
			return this.m_class
		endmethod

		/// Friend relation to ATalk, don't use.
		public method setTalk takes ATalk talk returns nothing
			set this.m_talk = talk
		endmethod

		/// Friend relation to ATalk, don't use.
		public method talk takes nothing returns ATalk
			return this.m_talk
		endmethod

		/// Friend relation to AShrine, don't use.
		public method setShrine takes AShrine shrine returns nothing
			set this.m_shrine = shrine
		endmethod

		/// Friend relation to AShrine, don't use.
		public method shrine takes nothing returns AShrine
			return this.m_shrine
		endmethod

		//start members

		/**
		* Each character has its own owner. This must be a human playing player.
		* Each human playing player can own exact one character.
		* @return Returns character's owning player
		*/
		public method user takes nothing returns player
			return this.m_user
		endmethod

		/**
		* Characters are represented by units in game.
		* @return Returns character's unit.
		*/
		public method unit takes nothing returns unit
			return this.m_unit
		endmethod

		//members

		public method view takes nothing returns AView
			return this.m_view
		endmethod
		
		public method focus takes nothing returns AFocus
			return this.m_focus
		endmethod
		
		public method movement takes nothing returns AMovement
			return this.m_movement
		endmethod
		
		public method fight takes nothing returns AFight
			return this.m_fight
		endmethod

		/// Friend relation to AShrine.
		/// Use it to setup the time.
		public method revival takes nothing returns ARevival
			return this.m_revival
		endmethod
		
		public method inventory takes nothing returns AInventory
			return this.m_inventory
		endmethod
		
		public method talkLog takes nothing returns ATalkLog
			return this.m_talkLog
		endmethod

		public method spellCount takes nothing returns integer
			return this.m_spells.size()
		endmethod
		
		//convenience methods

		public method name takes nothing returns string
			return GetPlayerName(this.m_user)
		endmethod

		/// Displays a message to the owner of the character.
		/// @todo Change this method.
		/// @param messageType The message type.
		/// @param message The message text.
		public method displayMessage takes integer messageType, string message returns nothing
			call DisplayTimedTextToPlayer(this.m_user, 0.0, 0.0, 6.0, message)
		endmethod

		/// Displays a message to the owners of all other characters.
		public method displayMessageToAllOthers takes integer messageType, string message returns nothing
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (user != this.m_user) then
					call thistype.playerCharacter(user).displayMessage(messageType, message)
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod

		public method addLevels takes integer levels, boolean showEffect returns nothing
			call SetHeroLevel(this.m_unit, GetHeroLevel(this.m_unit) + levels, showEffect)
		endmethod

		public method addSkillPoints takes integer skillPoints returns boolean
			return UnitModifySkillPoints(this.m_unit, skillPoints)
		endmethod
		
		public method removeSkillPoints takes integer skillPoints returns boolean
			return UnitModifySkillPoints(this.m_unit, -skillPoints)
		endmethod

		public method addExperience takes integer experience, boolean showEffect returns nothing
			call AddHeroXP(this.m_unit, experience, showEffect)
		endmethod

		public method addStrength takes integer strength returns nothing
			call SetHeroStr(this.m_unit, GetHeroStr(this.m_unit, false) + strength, true)
		endmethod

		public method addAgility takes integer agility returns nothing
			call SetHeroAgi(this.m_unit, GetHeroAgi(this.m_unit, false) + agility, true)
		endmethod

		public method addIntelligence takes integer intelligence returns nothing
			call SetHeroInt(this.m_unit, GetHeroInt(this.m_unit, false) + intelligence, true)
		endmethod

		public method addGold takes integer gold returns nothing
			call SetPlayerState(this.m_user, PLAYER_STATE_RESOURCE_GOLD, GetPlayerState(this.m_user, PLAYER_STATE_RESOURCE_GOLD) + gold)
		endmethod
		
		public method removeGold takes integer gold returns nothing
			call SetPlayerState(this.m_user, PLAYER_STATE_RESOURCE_GOLD, GetPlayerState(this.m_user, PLAYER_STATE_RESOURCE_GOLD) - gold)
		endmethod
		
		public method gold takes nothing returns integer
			return GetPlayerState(this.m_user, PLAYER_STATE_RESOURCE_GOLD)
		endmethod

		public method addLumber takes integer lumber returns nothing
			call SetPlayerState(this.m_user, PLAYER_STATE_RESOURCE_LUMBER, GetPlayerState(this.m_user, PLAYER_STATE_RESOURCE_LUMBER) + lumber)
		endmethod
		
		public method removeLumber takes integer lumber returns nothing
			call SetPlayerState(this.m_user, PLAYER_STATE_RESOURCE_LUMBER, GetPlayerState(this.m_user, PLAYER_STATE_RESOURCE_LUMBER) - lumber)
		endmethod
		
		public method lumber takes nothing returns integer
			return GetPlayerState(this.m_user, PLAYER_STATE_RESOURCE_LUMBER)
		endmethod
		
		public method showCinematicFilter takes real duration, blendmode blendMode, string texture, real red0, real green0, real blue0, real transparency0, real red1, real green1, real blue1, real transparency1 returns nothing
			call ShowGenericCinematicFilterForPlayer(this.m_user, duration, blendMode, texture, red0, green0, blue0, transparency0, red1, green1, blue1, transparency1)
		endmethod

		public method showBlackScreenCinematicFilter takes real duration returns nothing
			call ShowBlackScreenCinematicFilterForPlayer(this.m_user, duration)
		endmethod
		
		public method setX takes real x returns nothing
			call SetUnitX(this.m_unit, x)
		endmethod
		
		public method setY takes real y returns nothing
			call SetUnitY(this.m_unit, y)
		endmethod
		
		public method setPosition takes real x, real y returns nothing
			call SetUnitX(this.m_unit, x)
			call SetUnitY(this.m_unit, y)
		endmethod
		
		public method setLocation takes location usedLocation returns nothing
			call this.setX(GetLocationX(usedLocation))
			call this.setY(GetLocationY(usedLocation))
		endmethod
		
		public method setRect takes rect usedRect returns nothing
			call this.setX(GetRectCenterX(usedRect))
			call this.setY(GetRectCenterY(usedRect))
		endmethod
		
		public method setFacing takes real facing returns nothing
			call SetUnitFacing(this.m_unit, facing)
		endmethod
		
		public method show takes boolean show returns nothing
			call ShowUnit(this.m_unit, show)
		endmethod

		public method select takes boolean single returns nothing
			if (single) then
				call ClearSelectionForPlayer(this.m_user)
			endif
			call SelectUnitAddForPlayer(this.m_unit, this.m_user)
		endmethod
		
		public method panCamera takes nothing returns nothing
			call PanCameraToForPlayer(this.m_user, GetUnitX(this.m_unit), GetUnitY(this.m_unit))
		endmethod
		
		public method panCameraSmart takes nothing returns nothing
			call SmartCameraPanWithZForPlayer(this.m_user, GetUnitX(this.m_unit), GetUnitY(this.m_unit), 0.0, 0.0)
		endmethod
		
		public method setCamera takes nothing returns nothing
			call SetCameraPositionForPlayer(this.m_user, GetUnitX(this.m_unit), GetUnitY(this.m_unit))
		endmethod
		
		public method setCameraBoundsToRect takes rect usedRect returns nothing
			call SetCameraBoundsToRectForPlayerBJ(this.m_user, usedRect)
		endmethod
		
		public method resetCameraBoundsToMapRect takes nothing returns nothing
			call ResetCameraBoundsToMapRectForPlayer(this.m_user)
		endmethod

		public method userId takes nothing returns integer
			return GetPlayerId(this.m_user)
		endmethod
		
		public method level takes nothing returns integer
			return GetHeroLevel(this.m_unit)
		endmethod
		
		public method skillPoints takes nothing returns integer
			return GetHeroSkillPoints(this.m_unit)
		endmethod
		
		public method copy takes integer stateMethod returns unit
			return CopyUnit(this.m_unit, GetUnitX(this.m_unit), GetUnitY(this.m_unit), GetUnitFacing(this.m_unit), stateMethod)
		endmethod
		
		//methods
		
		/// Refresh events (without death events, there won't be any revival).
		/// Only use this method when replacing the character's unit for some time.
		public method replaceUnit takes unit newUnit returns nothing
			set this.m_unit = newUnit
		endmethod
		
		/// Friend relation to ASpell, don't use.
		public method addSpell takes ASpell spell returns nothing
			call this.m_spells.pushBack(spell)
		endmethod
		
		public method spell takes integer index returns ASpell
			return this.m_spells[index]
		endmethod
		
		public method spellByAbilityId takes integer abilityId returns ASpell
			local integer i = 0
			loop
				exitwhen (i == this.m_spells.size())
				if (ASpell(this.m_spells[i]).ability() == abilityId) then
					return ASpell(this.m_spells[i])
				endif
				set i = i + 1
			endloop
			return 0
		endmethod

		private method enableMovableSystems takes nothing returns nothing
			if (thistype.m_useViewSystem and this.m_view.enableAgain()) then
				call this.m_view.enable()
			endif
			if (thistype.m_useFocusSystem and this.m_focus.enableAgain()) then
				call this.m_focus.enable()
			endif
			if (thistype.m_useMovementSystem and this.m_movement.enableAgain()) then
				call this.m_movement.enable()
			endif
			if (thistype.m_useFightSystem and this.m_fight.enableAgain()) then
				call this.m_fight.enable()
			endif
			if (thistype.m_useRevivalSystem and this.m_revival.enableAgain()) then
				call this.m_revival.enable()
			endif
			/*
			if (thistype.m_useInventorySystem and this.m_inventory.enableAgain()) then
				call this.m_inventory.enable()
			endif
			if (thistype.m_useTalkLogSystem and this.m_talkLog.enableAgain()) then
				call this.m_talkLog.enable()
			endif
			*/
		endmethod

		private method disableMovableSystems takes nothing returns nothing
			if (thistype.m_useViewSystem and this.m_view.isEnabled()) then
				call this.m_view.disable()
			endif
			if (thistype.m_useFocusSystem and this.m_focus.isEnabled()) then
				call this.m_focus.disable()
			endif
			if (thistype.m_useMovementSystem and this.m_movement.isEnabled()) then
				call this.m_movement.disable()
			endif
			if (thistype.m_useFightSystem and this.m_fight.isEnabled()) then
				call this.m_fight.disable()
			endif
			if (thistype.m_useRevivalSystem and this.m_revival.isEnabled()) then
				call this.m_revival.disable()
			endif
			/*
			if (thistype.m_useInventorySystem and this.m_inventory.isEnabled()) then
				call this.m_inventory.disable()
			endif
			if (thistype.m_useTalkLogSystem and this.m_talkLog.isEnabled()) then
				call this.m_talkLog.disable()
			endif
			*/
		endmethod

		private static method triggerActionDestroyCharacter takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			call thistype.destroy(this)
			set triggeringTrigger = null
		endmethod

		private static method triggerActionSetUnmovable takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			call this.setMovable(false)
			set triggeringTrigger = null
		endmethod

		private method createLeaveTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_leaveTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterPlayerEvent(this.m_leaveTrigger, this.m_user, EVENT_PLAYER_LEAVE)
			if (thistype.m_destroyOnPlayerLeaves) then
				set triggerAction = TriggerAddAction(this.m_leaveTrigger, function thistype.triggerActionDestroyCharacter)
			else
				set triggerAction = TriggerAddAction(this.m_leaveTrigger, function thistype.triggerActionSetUnmovable)
			endif
			call AHashTable.global().setHandleInteger(this.m_leaveTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		/// DON'T MAKE HIM UNMOVABLE, disables all systems!
		private method createDeathTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			if (thistype.m_destroyOnDeath) then
				set this.m_deathTrigger = CreateTrigger()
				set triggerEvent = TriggerRegisterUnitEvent(this.m_deathTrigger, this.m_unit, EVENT_UNIT_DEATH)
				set triggerAction = TriggerAddAction(this.m_deathTrigger, function thistype.triggerActionDestroyCharacter)
				call AHashTable.global().setHandleInteger(this.m_deathTrigger, "this", this)
				set triggerEvent = null
				set triggerAction = null
			endif
		endmethod

		private method createSystems takes nothing returns nothing
			if (thistype.m_useViewSystem) then
				set this.m_view = AView.create(this)
			endif
			if (thistype.m_useFocusSystem) then
				set this.m_focus = AFocus.create(this)
			endif
			if (thistype.m_useMovementSystem) then
				set this.m_movement = AMovement.create(this)
			endif
			if (thistype.m_useFightSystem) then
				set this.m_fight = AFight.create(this)
			endif
			if (thistype.m_useRevivalSystem) then
				set this.m_revival = ARevival.create(this)
			endif
			if (thistype.m_useInventorySystem) then
				set this.m_inventory = AInventory.create(this)
			endif
			if (thistype.m_useTalkLogSystem) then
				set this.m_talkLog = ATalkLog.create(this)
			endif
		endmethod

		/// @todo Should be private, vJass bug
		//Private, every player only can have one character
		private static method create takes player user, unit usedUnit returns thistype
			local thistype this = thistype.allocate()
			//start members
			set this.m_user = user
			set this.m_unit = usedUnit
			call AHashTable.global().setHandleInteger(usedUnit, "ACharacter", this)
			//dynamic members
			set this.m_isMovable = true
			//members
			set this.m_inventory = 0
			set this.m_talkLog = 0
			set this.m_spells = AIntegerVector.create()

			call this.createLeaveTrigger()
			call this.createDeathTrigger()
			call this.createSystems()
			return this
		endmethod

		private method removeUnit takes nothing returns nothing
			if (thistype.m_removeUnitOnDestruction) then
				call RemoveUnit(this.m_unit)
			endif
			call AHashTable.global().flushHandle(this.m_unit)
			set this.m_unit = null
		endmethod

		private method destroyLeaveTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_leaveTrigger)
			set this.m_leaveTrigger = null
		endmethod

		private method destroyDeathTrigger takes nothing returns nothing
			if (thistype.m_destroyOnDeath) then
				call AHashTable.global().destroyTrigger(this.m_deathTrigger)
				set this.m_deathTrigger = null
			endif
		endmethod

		private method destroySystems takes nothing returns nothing
			//ifs are important
			if (thistype.m_useViewSystem) then
				call this.m_view.destroy()
			endif
			if (thistype.m_useFocusSystem) then
				call this.m_focus.destroy()
			endif
			if (thistype.m_useMovementSystem) then
				call this.m_movement.destroy()
			endif
			if (thistype.m_useFightSystem) then
				call this.m_fight.destroy()
			endif
			if (thistype.m_useRevivalSystem) then
				call this.m_revival.destroy()
			endif
			if (thistype.m_useInventorySystem) then
				call this.m_inventory.destroy()
			endif
			if (thistype.m_useTalkLogSystem) then
				call this.m_talkLog.destroy()
			endif
		endmethod

		//Automatic destruction when player leaves
		private method onDestroy takes nothing returns nothing
			//start members
			set this.m_user = null
			//members
			loop
				exitwhen (this.m_spells.empty())
				call ASpell(this.m_spells.back()).destroy()
			endloop
			call this.m_spells.destroy()

			call this.removeUnit()
			call this.destroyLeaveTrigger()
			call this.destroyDeathTrigger()
			call this.destroySystems()
		endmethod

		/**
		* Call this method before you use this class!
		* @param destroyOnPlayerLeaves If this value is false the character will be set unmovable when his owner leaves otherwise he will be destroyed.
		* @param destroyOnDeath If this value is false the character will be set unmovable when he dies otherwise he will be destroyed.
		* @param useViewSystem Shows if the view system is used.
		* @param useFocusSystem Shows if the focus system is used.
		* @param useMovementSystem Shows if the movement system is used.
		* @param useFightSystem Shows if the fight system is used.
		* @param useRevivalSystem Shows if the revival system is used.
		* @param useInventorySystem Shows if the inventory system is used.
		* @param useTalkLogSystem Shows if the talk log system is used.
		* @see AView
		* @see AFocus
		* @see AMovement
		* @see AFight
		* @see ARevival
		* @see AInventory
		* @see ATalkLog
		*/
		public static method init takes  boolean removeUnitOnDestruction, boolean destroyOnPlayerLeaves, boolean destroyOnDeath, boolean useViewSystem, boolean useFocusSystem, boolean useMovementSystem, boolean useFightSystem, boolean useRevivalSystem, boolean useInventorySystem, boolean useTalkLogSystem returns nothing
			//static start members
			set thistype.m_removeUnitOnDestruction = removeUnitOnDestruction
			set thistype.m_destroyOnPlayerLeaves = destroyOnPlayerLeaves
			set thistype.m_destroyOnDeath = destroyOnDeath
			set thistype.m_useViewSystem = useViewSystem
			set thistype.m_useFocusSystem = useFocusSystem
			set thistype.m_useMovementSystem = useMovementSystem
			set thistype.m_useFightSystem = useFightSystem
			set thistype.m_useRevivalSystem = useRevivalSystem
			set thistype.m_useInventorySystem = useInventorySystem
			set thistype.m_useTalkLogSystem = useTalkLogSystem
			
			debug if (thistype.m_destroyOnDeath and thistype.m_useRevivalSystem) then
				debug call thistype.staticPrint("You're using destroy on death and use revival system options at the same time.")
			debug endif
		endmethod
		
		public static method removeUnitOnDestruction takes nothing returns boolean
			return thistype.m_removeUnitOnDestruction
		endmethod
		
		public static method destroyOnPlayerLeaves takes nothing returns boolean
			return thistype.m_destroyOnPlayerLeaves
		endmethod
		
		public static method destroyOnDeath takes nothing returns boolean
			return thistype.m_destroyOnDeath
		endmethod
		
		public static method useViewSystem takes nothing returns boolean
			return thistype.m_useViewSystem
		endmethod
		
		public static method useFocusSystem takes nothing returns boolean
			return thistype.m_useFocusSystem
		endmethod
		
		public static method useMovementSystem takes nothing returns boolean
			return thistype.m_useMovementSystem
		endmethod
		
		public static method useFightSystem takes nothing returns boolean
			return thistype.m_useFightSystem
		endmethod
		
		public static method useRevivalSystem takes nothing returns boolean
			return thistype.m_useRevivalSystem
		endmethod
		
		public static method useInventorySystem takes nothing returns boolean
			return thistype.m_useInventorySystem
		endmethod
		
		public static method useTalkLogSystem takes nothing returns boolean
			return thistype.m_useTalkLogSystem
		endmethod

		/**
		* Each human playing player can own exact one character.
		* Use this method to set the unit of a player character.
		* @param user The owner of the character.
		* @param usedUnit The unit of the character.
		* @return Returns player character.
		*/
		public static method setPlayerCharacter takes player user, unit usedUnit returns thistype
			local thistype character = thistype.create(user, usedUnit)
			set thistype.m_playerCharacter[GetPlayerId(user)] = character
			return character
		endmethod

		/// @param user The owner of the character.
		/// @return The character instance.
		public static method playerCharacter takes player user returns thistype
			return thistype.m_playerCharacter[GetPlayerId(user)]
		endmethod

		/// If character will be destroyed automaticly you don't have to call this method.
		/// @param user The owner of the character.
		public static method destroyPlayerCharacter takes player user returns nothing
			call thistype.destroy(thistype.m_playerCharacter[GetPlayerId(user)])
		endmethod
		
		//static convenience methods
		
		/// @todo You could also check it by only comparing with the units owner character unit.
		public static method getCharacterByUnit takes unit usedUnit returns thistype
			return AHashTable.global().handleInteger(usedUnit, "ACharacter")
			/*
			local integer i
			local player user
			debug if (usedUnit == null) then
				debug call thistype.staticPrint("Used unit is null.")
			debug endif
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (thistype.playerCharacter(user) != 0 and thistype.playerCharacter(user).unit() == usedUnit) then
					set user = null
					return thistype.playerCharacter(user)
				endif
				set user = null
				set i = i + 1
			endloop
			return 0
			*/
		endmethod
		
		public static method isUnitCharacter takes unit usedUnit returns boolean
			return thistype.getCharacterByUnit(usedUnit) != 0
		endmethod
		
		public static method getFirstCharacter takes nothing returns thistype
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (thistype.playerCharacter(user) != 0) then
					set user = null
					return thistype.playerCharacter(user)
				endif
				set user = null
				set i = i + 1
			endloop
			return 0
		endmethod

		public static method displayMessageByUser takes player user, integer messageType, string message returns nothing
			call thistype.playerCharacter(user).displayMessage(messageType, message)
		endmethod

		public static method setAllMovable takes boolean movable returns nothing
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (thistype.playerCharacter(user) != 0) then
					call thistype.playerCharacter(user).setMovable(movable)
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod

		/// Displays a message to each owner of every character.
		public static method displayMessageToAll takes integer messageType, string message returns nothing
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (thistype.playerCharacter(user) != 0) then
					call thistype.playerCharacter(user).displayMessage(messageType, message)
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod
		
		public static method countAllPlaying takes nothing returns integer
			local integer i
			local player user
			local integer result = 0
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user) and thistype.playerCharacter(user) != 0) then
					set result = result + 1
				endif
				set user = null
				set i = i + 1
			endloop
			return result
		endmethod

		public static method addLevelsToAll takes integer levels, boolean showEffect returns nothing
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (thistype.playerCharacter(user) != 0) then
					call thistype.playerCharacter(user).addLevels(levels, showEffect)
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod

		public static method addSkillPointsToAll takes integer skillPoints returns nothing
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (thistype.playerCharacter(user) != 0) then
					call thistype.playerCharacter(user).addSkillPoints(skillPoints)
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod

		public static method addExperienceToAll takes integer experience, boolean showEffect returns nothing
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (thistype.playerCharacter(user) != 0) then
					call thistype.playerCharacter(user).addExperience(experience, showEffect)
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod

		public static method addStrengthToAll takes integer strength returns nothing
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (thistype.playerCharacter(user) != 0) then
					call thistype.playerCharacter(user).addStrength(strength)
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod

		public static method addAgilityToAll takes integer agility returns nothing
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (thistype.playerCharacter(user) != 0) then
					call thistype.playerCharacter(user).addAgility(agility)
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod

		public static method addIntelligenceToAll takes integer intelligence returns nothing
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (thistype.playerCharacter(user) != 0) then
					call thistype.playerCharacter(user).addIntelligence(intelligence)
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod

		public static method addGoldToAll takes integer gold returns nothing
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (thistype.playerCharacter(user) != 0) then
					call thistype.playerCharacter(user).addGold(gold)
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod

		public static method addLumberToAll takes integer lumber returns nothing
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (thistype.playerCharacter(user) != 0) then
					call thistype.playerCharacter(user).addLumber(lumber)
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod
		
		public static method allUnitsAreInRect takes rect usedRect returns boolean
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (thistype.playerCharacter(user) != 0) then
					if ((thistype.playerCharacter(user).unit() == null) or (not RectContainsUnit(usedRect, thistype.playerCharacter(user).unit()))) then //RectContainsUnit shouldn't be that slow
						return false
					endif
				endif
				set user = null
				set i = i + 1
			endloop
			return true
		endmethod
		
		public static method panCameraToAll takes nothing returns nothing
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (thistype.playerCharacter(user) != 0) then
					call thistype.playerCharacter(user).panCamera()
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod
		
		public static method panCameraSmartToAll takes nothing returns nothing
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (thistype.playerCharacter(user) != 0) then
					call thistype.playerCharacter(user).panCameraSmart()
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod
		
		public static method showAll takes boolean show returns nothing
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (thistype.playerCharacter(user) != 0) then
					call thistype.playerCharacter(user).show(show)
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod
		
		public static method enableShrineForAll takes AShrine shrine, boolean showMessage returns nothing
			local integer i
			local player user
			debug if (not thistype.m_useRevivalSystem) then
				debug call thistype.staticPrint("Revival system is not enabled.")
				debug return
			debug endif
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (thistype.playerCharacter(user) != 0) then
					call shrine.enableForCharacter(thistype.playerCharacter(user), showMessage)
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod
	endstruct

endlibrary
