library AStructSystemsCharacterCharacter requires ALibraryCoreDebugMisc, ALibraryCoreGeneralPlayer, ALibraryCoreInterfaceCinematicFilter, ALibraryCoreInterfaceCamera, AStructSystemsCharacterCharacterHashTable, AStructSystemsCharacterAbstractCharacterSystem

	/// This struct represents a single RPG character. Each player can own exactly one character.
	/// You can configure the character systems and enable or disable the several character system modules.
	/// Each module makes the system requiring more memory, so clearly think of which system modules are required and which aren't.
	struct ACharacter
		//static constant members
		public static constant integer messageTypeInfo = 0
		public static constant integer messageTypeError = 1
		private static constant integer maxSpells = 100
		//static start members
		private static boolean removeUnitOnDestruction
		private static boolean destroyOnPlayerLeaves
		private static boolean destroyOnDeath
		private static boolean useViewSystem
		private static boolean useFocusSystem
		private static boolean useMovementSystem
		private static boolean useFightSystem
		private static boolean useRevivalSystem
		private static boolean useInventorySystem
		//static members
		private static ACharacter array playerCharacter//[bj_MAX_PLAYERS] vjass bug
		//dynamic members
		private boolean movable
		private AClass class //No system
		private ATalk talk //No system
		private AShrine shrine //No system
		//start members
		private player user
		private unit usedUnit
		//members
		private trigger leaveTrigger
		private trigger deathTrigger
		//insert the character systems here
		private AView view
		private AFocus focus
		private AMovement movement
		private AFight fight
		private ARevival revival
		private AInventory inventory
		private integer m_spellCount
		private ASpell array spells[ACharacter.maxSpells]
		
		//! runtextmacro A_STRUCT_DEBUG("\"ACharacter\"")

		//dynamic members

		/// If the character is set unmovable he will be stopped immediatly and can not move until he will be set movable.
		/// This method is used for dialogs or cinematic sequences. It also disables all character systems.
		public method setMovable takes boolean movable returns nothing
			//Don't make him movable if he is already!
			//If he talks with another NPC he is already unmovable.
			debug if (this.movable == movable) then
				debug call Print("Character " + I2S(this) + ": setMovable() error.")
				debug return
			debug endif
			set this.movable = movable
			call IssueImmediateOrder(this.usedUnit, "stop") //new, required?!
			call PauseUnit(this.usedUnit, not movable)
			call SetUnitInvulnerable(this.usedUnit, not movable) 
			if (movable) then
				call this.enableSystems()
			else
				call this.disableSystems()
			endif
		endmethod

		public method getMovable takes nothing returns boolean
			return this.movable
		endmethod

		/// Friend relation to AClassSelection, don't use.
		public method setClass takes AClass class returns nothing
			set this.class = class
		endmethod

		public method getClass takes nothing returns AClass
			return this.class
		endmethod

		/// Friend relation to ATalk, don't use.
		public method setTalk takes ATalk talk returns nothing
			set this.talk = talk
		endmethod

		/// Friend relation to ATalk, don't use.
		public method getTalk takes nothing returns ATalk
			return this.talk
		endmethod

		/// Friend relation to AShrine, don't use.
		public method setShrine takes AShrine shrine returns nothing
			set this.shrine = shrine
		endmethod

		/// Friend relation to AShrine, don't use.
		public method getShrine takes nothing returns AShrine
			return this.shrine
		endmethod

		//start members

		public method getUser takes nothing returns player
			return this.user
		endmethod

		public method getUsedUnit takes nothing returns unit
			return this.usedUnit
		endmethod

		//members

		/// Friend relation to AShrine.
		/// Use it to setup the time.
		public method getRevival takes nothing returns ARevival
			return this.revival
		endmethod
		
		public method getInventory takes nothing returns AInventory
			return this.inventory
		endmethod
		
		public method spellCount takes nothing returns integer
			return this.m_spellCount
		endmethod
		
		//convenience methods

		public method getName takes nothing returns string
			return GetPlayerName(this.user)
		endmethod

		/// Displays a message to the owner of the character.
		/// @todo Change this method.
		/// @param messageType The message type.
		/// @param message The message text.
		public method displayMessage takes integer messageType, string message returns nothing
			call DisplayTimedTextToPlayer(this.user, 0.0, 0.0, 6.0, message)
		endmethod

		/// Displays a message to the owners of all other characters.
		public method displayMessageToAllOthers takes integer messageType, string message returns nothing
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (user != this.user) then
					call ACharacter.getPlayerCharacter(user).displayMessage(messageType, message)
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod

		public method addLevels takes integer levels, boolean showEffect returns nothing
			call SetHeroLevel(this.usedUnit, GetHeroLevel(this.usedUnit) + levels, showEffect)
		endmethod

		public method addSkillPoints takes integer skillPoints returns boolean
			return UnitModifySkillPoints(this.usedUnit, skillPoints)
		endmethod

		public method addExperience takes integer experience, boolean showEffect returns nothing
			call AddHeroXP(this.usedUnit, experience, showEffect)
		endmethod

		public method addStrength takes integer strength returns nothing
			call SetHeroStr(this.usedUnit, GetHeroStr(this.usedUnit, false) + strength, true)
		endmethod

		public method addAgility takes integer agility returns nothing
			call SetHeroAgi(this.usedUnit, GetHeroAgi(this.usedUnit, false) + agility, true)
		endmethod

		public method addIntelligence takes integer intelligence returns nothing
			call SetHeroInt(this.usedUnit, GetHeroInt(this.usedUnit, false) + intelligence, true)
		endmethod

		public method addGold takes integer gold returns nothing
			call SetPlayerState(this.user, PLAYER_STATE_RESOURCE_GOLD, GetPlayerState(this.user, PLAYER_STATE_RESOURCE_GOLD) + gold)
		endmethod

		public method addLumber takes integer lumber returns nothing
			call SetPlayerState(this.user, PLAYER_STATE_RESOURCE_LUMBER, GetPlayerState(this.user, PLAYER_STATE_RESOURCE_LUMBER) + lumber)
		endmethod
		
		public method showCinematicFilter takes real duration, blendmode blendMode, string texture, real red0, real green0, real blue0, real transparency0, real red1, real green1, real blue1, real transparency1 returns nothing
			call ShowGenericCinematicFilterForPlayer(this.user, duration, blendMode, texture, red0, green0, blue0, transparency0, red1, green1, blue1, transparency1)
		endmethod

		public method showBlackScreenCinematicFilter takes real duration returns nothing
			call ShowBlackScreenCinematicFilterForPlayer(this.user, duration)
		endmethod
		
		public method setX takes real x returns nothing
			call SetUnitX(this.usedUnit, x)
		endmethod
		
		public method setY takes real y returns nothing
			call SetUnitY(this.usedUnit, y)
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
			call SetUnitFacing(this.usedUnit, facing)
		endmethod
		
		public method show takes boolean show, boolean invulnerable, boolean pause returns nothing
			call ShowUnit(this.usedUnit, show)
			call SetUnitInvulnerable(this.usedUnit, invulnerable)
			call PauseUnit(this.usedUnit, pause)
		endmethod

		public method select takes boolean single returns nothing
			if (single) then
				call ClearSelectionForPlayer(this.user)
			endif
			call SelectUnitAddForPlayer(this.usedUnit, this.user)
		endmethod
		
		public method panCamera takes nothing returns nothing
			call PanCameraToForPlayer(this.user, GetUnitX(this.usedUnit), GetUnitY(this.usedUnit))
		endmethod
		
		public method setCameraBoundsToRect takes rect usedRect returns nothing
			call SetCameraBoundsToRectForPlayerBJ(this.user, usedRect)
		endmethod
		
		public method resetCameraBoundsToMapRect takes nothing returns nothing
			call ResetCameraBoundsToMapRectForPlayer(this.user)
		endmethod

		public method userId takes nothing returns integer
			return GetPlayerId(this.user)
		endmethod
		
		//methods
		
		/// Refresh events (without death events, there won't be any revival).
		/// Only use this method when replacing the character's unit for some time.
		public method replaceUnit takes unit newUnit returns nothing
			set this.usedUnit = newUnit
		endmethod
		
		/// Friend relation to ASpell, don't use.
		public method addSpell takes ASpell spell returns nothing
			debug if (this.m_spellCount == ACharacter.maxSpells) then
				debug call this.print("Spell maximum has been reached. Tried to add spell " + I2S(spell) + ".")
				debug return
			debug endif
			set this.spells[this.m_spellCount] = spell
			set this.m_spellCount = this.m_spellCount + 1
		endmethod
		
		public method spell takes integer index returns ASpell
			debug if (index > this.m_spellCount or index < 0) then
				debug call this.print("Wrong spell index: " + I2S(index) + ".")
				debug return 0
			debug endif
			return this.spells[index]
		endmethod
		
		public method spellByAbilityId takes integer abilityId returns ASpell
			local integer i = 0
			loop
				exitwhen (i == this.m_spellCount)
				if (this.spells[i].ability() == abilityId) then
					return this.spells[i]
				endif
				set i = i + 1
			endloop
			return 0
		endmethod

		private method enableSystems takes nothing returns nothing
			if (ACharacter.useViewSystem) then
				call this.view.enable()
			endif
			if (ACharacter.useFocusSystem) then
				call this.focus.enable()
			endif
			if (ACharacter.useMovementSystem) then
				call this.movement.enable()
			endif
			if (ACharacter.useFightSystem) then
				call this.fight.enable()
			endif
			if (ACharacter.useRevivalSystem) then
				call this.revival.enable()
			endif
			if (ACharacter.useInventorySystem) then
				call this.inventory.enable()
			endif
		endmethod

		private method disableSystems takes nothing returns nothing
			if (ACharacter.useViewSystem) then
				call this.view.disable()
			endif
			if (ACharacter.useFocusSystem) then
				call this.focus.disable()
			endif
			if (ACharacter.useMovementSystem) then
				call this.movement.disable()
			endif
			if (ACharacter.useFightSystem) then
				call this.fight.disable()
			endif
			if (ACharacter.useRevivalSystem) then
				call this.revival.disable()
			endif
			if (ACharacter.useInventorySystem) then
				call this.inventory.disable()
			endif
		endmethod

		private static method triggerActionDestroyCharacter takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ACharacter character = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "character")
			call ACharacter.destroy(character)
			set triggeringTrigger = null
		endmethod

		private static method triggerActionSetUnmovable takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ACharacter character = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "character")
			//call character.setMovable(false) /// TEST!!!
			set triggeringTrigger = null
		endmethod

		private method createLeaveTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.leaveTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterPlayerEvent(this.leaveTrigger, this.user, EVENT_PLAYER_LEAVE)
			if (ACharacter.destroyOnPlayerLeaves) then
				set triggerAction = TriggerAddAction(this.leaveTrigger, function ACharacter.triggerActionDestroyCharacter)
			else
				set triggerAction = TriggerAddAction(this.leaveTrigger, function ACharacter.triggerActionSetUnmovable)
			endif
			call AGetCharacterHashTable().storeHandleInteger(this.leaveTrigger, "character", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		/// DON'T MAKE HIM UNMOVABLE, disables all systems!
		private method createDeathTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			if (ACharacter.destroyOnDeath) then
				set this.deathTrigger = CreateTrigger()
				set triggerEvent = TriggerRegisterUnitEvent(this.deathTrigger, this.usedUnit, EVENT_UNIT_DEATH)
				set triggerAction = TriggerAddAction(this.deathTrigger, function ACharacter.triggerActionDestroyCharacter)
				call AGetCharacterHashTable().storeHandleInteger(this.deathTrigger, "character", this) //AClassCharacterCharacterHashTable
				set triggerEvent = null
				set triggerAction = null
			endif
		endmethod

		private method createSystems takes nothing returns nothing
			if (ACharacter.useViewSystem) then
				set this.view = AView.create(this)
			endif
			if (ACharacter.useFocusSystem) then
				set this.focus = AFocus.create(this)
			endif
			if (ACharacter.useMovementSystem) then
				set this.movement = AMovement.create(this)
			endif
			if (ACharacter.useFightSystem) then
				set this.fight = AFight.create(this)
			endif
			if (ACharacter.useRevivalSystem) then
				set this.revival = ARevival.create(this)
			endif
			if (ACharacter.useInventorySystem) then
				set this.inventory = AInventory.create(this)
			endif
		endmethod

		/// @todo Should be private, vJass bug
		//Private, every player only can have one character
		private static method create takes player user, unit usedUnit returns ACharacter
			local ACharacter this = ACharacter.allocate()
			debug call this.print("Create character")
			//start members
			set this.user = user
			set this.usedUnit = usedUnit
			//dynamic members
			set this.movable = true
			//members
			set this.m_spellCount = 0

			call this.createLeaveTrigger()
			call this.createDeathTrigger()
			call this.createSystems()
			return this
		endmethod

		private method removeUsedUnit takes nothing returns nothing
			if (ACharacter.removeUnitOnDestruction) then
				call RemoveUnit(this.usedUnit)
			endif
			set this.usedUnit = null
		endmethod

		private method destroyLeaveTrigger takes nothing returns nothing
			call AGetCharacterHashTable().destroyTrigger(this.leaveTrigger)
			set this.leaveTrigger = null
		endmethod

		private method destroyDeathTrigger takes nothing returns nothing
			if (ACharacter.destroyOnDeath) then
				call AGetCharacterHashTable().destroyTrigger(this.deathTrigger)
				set this.deathTrigger = null
			endif
		endmethod
		
		private method destroySpells takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.m_spellCount)
				call this.spells[i].destroy()
				set i = i + 1
			endloop
		endmethod

		private method destroySystems takes nothing returns nothing
			//ifs are important
			if (ACharacter.useViewSystem) then
				call this.view.destroy()
			endif
			if (ACharacter.useFocusSystem) then
				call this.focus.destroy()
			endif
			if (ACharacter.useMovementSystem) then
				call this.movement.destroy()
			endif
			if (ACharacter.useFightSystem) then
				call this.fight.destroy()
			endif
			if (ACharacter.useRevivalSystem) then
				call this.revival.destroy()
			endif
			if (ACharacter.useInventorySystem) then
				call this.inventory.destroy()
			endif
		endmethod

		//Automatic desturction when player leaves
		private method onDestroy takes nothing returns nothing
			//start members
			set this.user = null

			call this.removeUsedUnit()
			call this.destroyLeaveTrigger()
			call this.destroyDeathTrigger()
			call this.destroySpells()
			call this.destroySystems()
		endmethod

		/// Call this method before you use this class!
		/// @param destroyOnPlayerLeaves If this value is false the character will be set unmovable when his owner leaves otherwise he will be destroyed.
		/// @param destroyOnDeath If this value is false the character will be set unmovable when he dies otherwise he will be destroyed.
		/// @param useViewSystem Shows if the view system is used.
		/// @param useFocusSystem Shows if the focus system is used.
		/// @param useMovementSystem Shows if the movement system is used.
		/// @param useFightSystem Shows if the fight system is used.
		/// @param useRevivalSystem Shows if the revival system is used.
		/// @param useInventorySystem Shows if the inventory system is used.
		public static method init takes  boolean removeUnitOnDestruction, boolean destroyOnPlayerLeaves, boolean destroyOnDeath, boolean useViewSystem, boolean useFocusSystem, boolean useMovementSystem, boolean useFightSystem, boolean useRevivalSystem, boolean useInventorySystem returns nothing
			//static start members
			set ACharacter.removeUnitOnDestruction = removeUnitOnDestruction
			set ACharacter.destroyOnPlayerLeaves = destroyOnPlayerLeaves
			set ACharacter.destroyOnDeath = destroyOnDeath
			set ACharacter.useViewSystem = useViewSystem
			set ACharacter.useFocusSystem = useFocusSystem
			set ACharacter.useMovementSystem = useMovementSystem
			set ACharacter.useFightSystem = useFightSystem
			set ACharacter.useRevivalSystem = useRevivalSystem
			set ACharacter.useInventorySystem = useInventorySystem
			
			debug if (ACharacter.destroyOnDeath and ACharacter.useRevivalSystem) then
				debug call ACharacter.staticPrint("You're using destroy on death and use revival system options at the same time.")
			debug endif
		endmethod

		/// Each player can own exactly one character.
		/// Use this method to set the characters unit of a player.
		/// @param user The owner of the character.
		/// @param usedUnit The unit of the character.
		/// @return Character id.
		public static method setPlayerCharacter takes player user, unit usedUnit returns ACharacter
			local ACharacter character = ACharacter.create(user, usedUnit)
			set ACharacter.playerCharacter[GetPlayerId(user)] = character
			return character
		endmethod

		/// @param user The owner of the character.
		/// @return The character instance.
		public static method getPlayerCharacter takes player user returns ACharacter
			return ACharacter.playerCharacter[GetPlayerId(user)]
		endmethod

		/// If character will be destroyed automaticly you don't have to call this method.
		/// @param user The owner of the character.
		public static method destroyPlayerCharacter takes player user returns nothing
			call ACharacter.destroy(ACharacter.playerCharacter[GetPlayerId(user)])
		endmethod
		
		//static convenience methods
		
		/// @todo You could also check it by only comparing with the units owner character unit.
		public static method getCharacterByUnit takes unit usedUnit returns ACharacter
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (ACharacter.getPlayerCharacter(user) != 0 and ACharacter.getPlayerCharacter(user).getUsedUnit() == usedUnit) then
					set user = null
					return ACharacter.getPlayerCharacter(user)
				endif
				set user = null
				set i = i + 1
			endloop
			return 0
		endmethod

		public static method displayMessageByUser takes player user, integer messageType, string message returns nothing
			call ACharacter.getPlayerCharacter(user).displayMessage(messageType, message)
		endmethod

		public static method setAllMovable takes boolean movable returns nothing
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then
					call ACharacter.getPlayerCharacter(user).setMovable(movable)
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
				if (IsPlayerPlayingUser(user)) then
					call ACharacter.getPlayerCharacter(user).displayMessage(messageType, message)
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod

		public static method addLevelsToAll takes integer levels, boolean showEffect returns nothing
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then
					call ACharacter.getPlayerCharacter(user).addLevels(levels, showEffect)
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
				if (IsPlayerPlayingUser(user)) then
					call ACharacter.getPlayerCharacter(user).addSkillPoints(skillPoints)
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
				if (IsPlayerPlayingUser(user)) then
					call ACharacter.getPlayerCharacter(user).addExperience(experience, showEffect)
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
				if (IsPlayerPlayingUser(user)) then
					call ACharacter.getPlayerCharacter(user).addStrength(strength)
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
				if (IsPlayerPlayingUser(user)) then
					call ACharacter.getPlayerCharacter(user).addAgility(agility)
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
				if (IsPlayerPlayingUser(user)) then
					call ACharacter.getPlayerCharacter(user).addIntelligence(intelligence)
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
				if (IsPlayerPlayingUser(user)) then
					call ACharacter.getPlayerCharacter(user).addGold(gold)
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
				if (IsPlayerPlayingUser(user)) then
					call ACharacter.getPlayerCharacter(user).addLumber(lumber)
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
				if (IsPlayerPlayingUser(user)) then
					if ((ACharacter.getPlayerCharacter(user).getUsedUnit() == null) or (not RectContainsUnit(usedRect, ACharacter.getPlayerCharacter(user).getUsedUnit()))) then //RectContainsUnit shouldn't be that slow
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
				if (IsPlayerPlayingUser(user)) then
					call ACharacter.getPlayerCharacter(user).panCamera()
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod
	endstruct

endlibrary
