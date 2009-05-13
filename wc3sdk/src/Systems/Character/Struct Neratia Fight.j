library AStructSystemsCharacterNeratiaFight requires ALibraryCoreDebugMisc, ALibraryCoreGeneralPlayer, ALibraryCoreGeneralUnit, ALibraryCoreStringConversion, ALibraryCoreInterfaceMisc, AStructSystemsCharacterCharacter, AStructSystemsCharacterCharacterHashTable, AStructSystemsGuiGui

	struct ANeratiaFightArea
		//dynamic members
		private rect array m_startRect[3] /// @todo vJass bug ANeratiaFight.maxGroups
		private real array m_startFacing[3] /// @todo vJass bug ANeratiaFight.maxGroups
		private camerasetup array m_cameraSetup[3] /// @todo vJass bug ANeratiaFight.maxGroups
		private rect m_rect

		//! runtextmacro A_STRUCT_DEBUG("\"ANeratiaFightArea\"")

		public method setStartRect takes integer index, rect startRect returns nothing
			debug if ((index < 0) or (index >= ANeratiaFight.maxGroups)) then
				debug call this.print("Invalid group index: " + I2S(index) + ".")
				debug return
			debug endif
			set this.m_startRect[index] = startRect
		endmethod
		
		public method startRect takes integer index returns rect
			debug if ((index < 0) or (index >= ANeratiaFight.maxGroups)) then
				debug call this.print("Invalid group index: " + I2S(index) + ".")
				debug return null
			debug endif
			return this.m_startRect[index]
		endmethod

		public method setStartFacing takes integer index, real startFacing returns nothing
			debug if ((index < 0) or (index >= ANeratiaFight.maxGroups)) then
				debug call this.print("Invalid group index: " + I2S(index) + ".")
				debug return
			debug endif
			set this.m_startFacing[index] = startFacing
		endmethod

		public method startFacing takes integer index returns real
			debug if ((index < 0) or (index >= ANeratiaFight.maxGroups)) then
				debug call this.print("Invalid group index: " + I2S(index) + ".")
				debug return 0.0
			debug endif
			return this.m_startFacing[index]
		endmethod

		public method setCameraSetup takes integer index, camerasetup cameraSetup returns nothing
			debug if ((index < 0) or (index >= ANeratiaFight.maxGroups)) then
				debug call this.print("Invalid group index: " + I2S(index) + ".")
				debug return
			debug endif
			set this.m_cameraSetup[index] = cameraSetup
		endmethod
		
		public method cameraSetup takes integer index returns camerasetup
			debug if ((index < 0) or (index >= ANeratiaFight.maxGroups)) then
				debug call this.print("Invalid group index: " + I2S(index) + ".")
				debug return null
			debug endif
			return this.m_cameraSetup[index]
		endmethod

		public method setRect takes rect usedRect returns nothing
			debug if (usedRect == null) then
				debug call this.print("Invalid rect.")
				debug return
			debug endif
			set this.m_rect = usedRect
		endmethod

		public method rect takes nothing returns rect
			return this.m_rect
		endmethod
	endstruct

	/// @todo vJass bug, should be a part of @struct ANeratiaFight.
	function interface ANeratiaFightEventCondition takes ANeratiaFight neratiaFight returns boolean

	/// @todo vJass bug, should be a part of @struct ANeratiaFight.
	function interface ANeratiaFightEventAction takes ANeratiaFight neratiaFight returns nothing
	
	/// @todo vJass bug, should be a part of @struct ANeratiaFight.
	private function dialogButtonActionSlash takes AGui gui, integer index returns nothing
	endfunction
	
	/// @todo vJass bug, should be a part of @struct ANeratiaFight.
	private function dialogButtonActionPass takes AGui gui, integer index returns nothing
	endfunction
	
	/// @todo vJass bug, should be a part of @struct ANeratiaFight.
	private function dialogButtonActionStroke takes AGui gui, integer index returns nothing
	endfunction

	/// The character fight system especially for Corexx's project Neratia.
	struct ANeratiaFight
		public static constant integer maxGroups = 3
		private static constant integer maxEvents = 3
		private static constant integer maxUsers = bj_MAX_PLAYERS
		private ANeratiaFightEventCondition array m_eventCondition[ANeratiaFight.maxEvents]
		private ANeratiaFightEventAction array m_eventAction[ANeratiaFight.maxEvents]
		//members
		private ANeratiaFightArea m_area
		private group array m_unitGroup[ANeratiaFight.maxGroups]
		private integer m_userCount
		private player array m_users[12] /// @todo vJass bug, ANeratiaFight.maxUsers
		private integer array m_userGroups[12] /// @todo vJass bug, ANeratiaFight.maxUsers
		private integer m_userIndex
		private unit m_selectedUnit
		private unit m_selectedEnemy
		private integer m_selectedEnemyGroupIndex
		private boolean m_hasToSelectAnEnemy
		private trigger selectionTrigger

		//! runtextmacro A_STRUCT_DEBUG("\"ANeratiaFight\"")
		
		//dynamic members
		
		public method setEventCondition takes integer index, ANeratiaFightEventCondition eventCondition returns nothing
			debug if ((index < 0) or (index >= ANeratiaFight.maxEvents)) then
				debug call this.print("Invalid group index: " + I2S(index) + ".")
				debug return
			debug endif
			set this.m_eventCondition[index] = eventCondition
		endmethod
		
		public method eventCondition takes integer index returns ANeratiaFightEventCondition
			debug if ((index < 0) or (index >= ANeratiaFight.maxEvents)) then
				debug call this.print("Invalid group index: " + I2S(index) + ".")
				debug return 0
			debug endif
			return this.m_eventCondition[index]
		endmethod
		
		public method setEventAction takes integer index, ANeratiaFightEventAction eventAction returns nothing
			debug if ((index < 0) or (index >= ANeratiaFight.maxEvents)) then
				debug call this.print("Invalid group index: " + I2S(index) + ".")
				debug return
			debug endif
			set this.m_eventAction[index] = eventAction
		endmethod
		
		public method eventAction takes integer index returns ANeratiaFightEventAction
			debug if ((index < 0) or (index >= ANeratiaFight.maxEvents)) then
				debug call this.print("Invalid group index: " + I2S(index) + ".")
				debug return 0
			debug endif
			return this.m_eventAction[index]
		endmethod
		
		//members
		
		public method area takes nothing returns ANeratiaFightArea
			return this.m_area
		endmethod
		
		//private group array m_unitGroup[ANeratiaFight.maxGroups]
		//private integer m_userCount
		//private player array m_users[12] /// @todo vJass bug, ANeratiaFight.maxUsers
		//private integer array m_userGroups[12]
		
		////methods
		
		public method displayMessage takes boolean toSelectingUser, integer messageType, string text returns nothing
			local player user
			local integer i = 0
			loop
				exitwhen (i == ANeratiaFight.maxUsers)
				set user = Player(i)
				if (IsPlayerPlayingUser(user) and ((toSelectingUser and user != this.m_users[this.m_userIndex]) or not toSelectingUser)) then
					call ACharacter.getPlayerCharacter(user).displayMessage(messageType, text)
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod

		public method addUnit takes unit usedUnit, integer groupIndex returns nothing
			call GroupAddUnit(this.m_unitGroup[groupIndex], usedUnit)
		endmethod

		public method addUser takes player user, integer groupIndex returns nothing
			debug if (this.m_userCount == ANeratiaFight.maxUsers) then
				debug call this.print("Unable to add user " + GetPlayerName(user) + " to group " + I2S(groupIndex) + ". User maximum has been reached.")
				debug return
			debug endif
			set this.m_users[this.m_userCount] = user
			set this.m_userGroups[this.m_userCount] = groupIndex
			set this.m_userCount = this.m_userCount + 1
		endmethod

		public method start takes ANeratiaFightArea area returns nothing
			local integer i
			local player user
			set this.m_area = area
			//fadeout
			
			set i = 0
			loop
				exitwhen (i == this.m_userCount)
				if (IsPlayerPlayingUser(user)) then
					call SetUserInterfaceForPlayer(this.m_users[i], false, false)
					call CameraSetupApplyForPlayer(false, this.m_area.cameraSetup(this.m_userGroups[i]), this.m_users[i], 0.0)
					call ClearSelectionForPlayer(this.m_users[i])
				endif
				set i = i + 1
			endloop
			set i = 0
			loop
				exitwhen (i == ANeratiaFight.maxGroups)
				call this.initUnitsOfGroup(i)
				set i = i + 1
			endloop
		endmethod

		private method initUnitsOfGroup takes integer groupIndex returns nothing
			local group groupBackup = CreateGroup()
			local unit first
			loop
				exitwhen (IsUnitGroupEmptyBJ(this.m_unitGroup[groupIndex]))
				set first = FirstOfGroupSave(this.m_unitGroup[groupIndex])
				call PauseUnit(first, true)
				call SetUnitInvulnerable(first, true)
				call SetUnitX(first, 0.0) /// @todo Get x coordinate.
				call SetUnitY(first, 0.0) /// @todo Get y coordinate.
				call SetUnitFacing(first, this.m_area.startFacing(groupIndex))
				call GroupRemoveUnit(this.m_unitGroup[groupIndex], first)
				call GroupAddUnit(groupBackup, first)
				set first = null
			endloop
			call DestroyGroup(this.m_unitGroup[groupIndex])
			set this.m_unitGroup[groupIndex] = null
			set this.m_unitGroup[groupIndex] = groupBackup
			set groupBackup = null
		endmethod
		
		private method turn takes integer userIndex returns nothing
			set this.m_userIndex = userIndex
			if (IsPlayerPlayingUser(this.m_users[userIndex])) then
				call ACharacter.displayMessageByUser(this.m_users[userIndex], ACharacter.messageTypeInfo, tr("Wählen Sie eine Einheit mit Hilfe eines Doppelklicks aus."))
			else
				/// @todo Start AI.
			endif
			call this.displayMessage(false, ACharacter.messageTypeInfo, StringArg(tr("%s ist am Zug."), GetPlayerName(this.m_users[userIndex])))
		endmethod
		
		private method showMeeleAttackTypeDialog takes nothing returns nothing
			call AGui.getPlayerGui(this.m_users[this.m_userIndex]).clearDialog()
			call AGui.getPlayerGui(this.m_users[this.m_userIndex]).setDialogTitle(tr("Nahkampfattacke"))
			call AGui.getPlayerGui(this.m_users[this.m_userIndex]).addDialogButton(tr("Hieb"), 'H', dialogButtonActionSlash)
			call AGui.getPlayerGui(this.m_users[this.m_userIndex]).addDialogButton(tr("Stich"), 'S', dialogButtonActionPass)
			call AGui.getPlayerGui(this.m_users[this.m_userIndex]).addDialogButton(tr("Streich"), 'T', dialogButtonActionStroke)
			call AGui.getPlayerGui(this.m_users[this.m_userIndex]).showDialog()
		endmethod
		
		private method showRangeAttackTypeDialog takes nothing returns nothing
			//optional
		endmethod
		
		private method selectGroup takes integer groupIndex returns nothing
			if (this.m_hasToSelectAnEnemy) then
				set this.m_selectedEnemyGroupIndex = groupIndex
			endif
			call CameraSetupApplyForPlayer(false, this.m_area.cameraSetup(groupIndex), this.m_users[this.m_userIndex], 0.0)
		endmethod
		
		private method selectUnit takes unit usedUnit, boolean finally  returns nothing
			if (this.m_hasToSelectAnEnemy) then
				set this.m_selectedEnemy = usedUnit
				if (finally) then
					call this.showMeeleAttackTypeDialog()
				endif
			else
				set this.m_selectedUnit = usedUnit
				if (finally) then
					set this.m_hasToSelectAnEnemy = true
					call this.playerSelectsNextGroup()
				endif
			endif
			//Effect erzeugen/bewegen
			//call 
			if (finally) then
				call this.displayMessage(false, ACharacter.messageTypeInfo, StringArg(StringArg(tr("%s hat die Einheit %s ausgewählt."), GetPlayerName(this.m_users[this.m_userIndex])), GetUnitName(usedUnit)))
			endif
		endmethod
		
		private method getNewUnit takes integer groupIndex, unit previousSelectedUnit, boolean right returns nothing
			local group groupBackup = CreateGroup()
			local unit realFirst = FirstOfGroupSave(this.m_unitGroup[groupIndex])
			local unit previous
			local unit first
			local unit selectedUnit = null
			loop
				exitwhen (IsUnitGroupEmptyBJ(this.m_unitGroup[groupIndex]))
				set first = FirstOfGroupSave(this.m_unitGroup[groupIndex])
				if (first == previousSelectedUnit) then
					if (right) then
						call GroupRemoveUnit(this.m_unitGroup[groupIndex], first)
						call GroupAddUnit(groupBackup, first)
						set first = null
						set first = FirstOfGroupSave(this.m_unitGroup[groupIndex])
						if (first == null and realFirst != previousSelectedUnit) then
							set selectedUnit = realFirst
						else
							set selectedUnit = first
						endif
					else
						set selectedUnit = previous
					endif
				endif
				call GroupRemoveUnit(this.m_unitGroup[groupIndex], first)
				call GroupAddUnit(groupBackup, first)
				set previous = null
				set previous = first
				set first = null
				if (selectedUnit != null) then
					call this.selectUnit(selectedUnit, false)
					set selectedUnit = null
				endif
			endloop
			call DestroyGroup(this.m_unitGroup[groupIndex])
			set this.m_unitGroup[groupIndex] = null
			set this.m_unitGroup[groupIndex] = groupBackup
			set groupBackup = null
			set realFirst = null
			set previous = null
		endmethod
		
		private method playerSelectsNextUnit takes nothing returns nothing
			if (this.m_hasToSelectAnEnemy) then
				call this.getNewUnit(this.m_selectedEnemyGroupIndex, this.m_selectedEnemy, true)
			else
				call this.getNewUnit(this.m_userGroups[this.m_userIndex], this.m_selectedUnit, true)
			endif
		endmethod
		
		private method playerSelectsPreviousUnit takes nothing returns nothing
			if (this.m_hasToSelectAnEnemy) then
				call this.getNewUnit(this.m_selectedEnemyGroupIndex, this.m_selectedEnemy, false)
			else
				call this.getNewUnit(this.m_userGroups[this.m_userIndex], this.m_selectedUnit, false)
			endif
		endmethod
		
		private method playerSelectsNextGroup takes nothing returns nothing
			local integer i
			if (this.m_selectedEnemyGroupIndex < ANeratiaFight.maxGroups - 1) then
				set i = i + 1
			else
				set i = 0
			endif
			loop
				exitwhen (i == ANeratiaFight.maxGroups)
				if (not IsUnitGroupEmptyBJ(this.m_unitGroup[i])) then
					if (this.m_selectedEnemyGroupIndex != i) then
						call this.selectGroup(i)
						call this.getNewUnit(this.m_selectedEnemyGroupIndex, this.m_selectedEnemy, true)
					endif
					exitwhen (true)
				endif
				set i = i + 1
			endloop
		endmethod
		
		private method playerSelectsPreviousGroup takes nothing returns nothing
			local integer i
			if (this.m_selectedEnemyGroupIndex > 0) then
				set i = i - 1
			else
				set i = ANeratiaFight.maxGroups - 1
			endif
			loop
				exitwhen (i < 0)
				if (not IsUnitGroupEmptyBJ(this.m_unitGroup[i])) then
					if (this.m_selectedEnemyGroupIndex != i) then
						call this.selectGroup(i)
						call this.getNewUnit(this.m_selectedEnemyGroupIndex, this.m_selectedEnemy, true)
					endif
					exitwhen (true)
				endif
				set i = i + 1
			endloop
		endmethod
		
		private static method triggerConditionSelect takes nothing returns boolean
			local player user = GetTriggerPlayer()
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ANeratiaFight this = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "this")
			local boolean result = user == this.m_users[this.m_userIndex]
			set user = null
			set triggeringTrigger = null
			return result
		endmethod
		
		private static method triggerActionSelect takes nothing returns nothing
			local player user = GetTriggerPlayer()
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ANeratiaFight this = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "this")
			local eventid eventId = GetTriggerEventId()
			if (eventId == EVENT_PLAYER_ARROW_UP_DOWN) then
				call this.playerSelectsNextUnit()
			elseif (eventId == EVENT_PLAYER_ARROW_DOWN_DOWN) then
				call this.playerSelectsPreviousUnit()
			elseif (eventId == EVENT_PLAYER_ARROW_RIGHT_DOWN and this.m_hasToSelectAnEnemy) then
				call this.playerSelectsNextGroup()
			elseif (eventId == EVENT_PLAYER_ARROW_LEFT_DOWN and this.m_hasToSelectAnEnemy) then
				call this.playerSelectsPreviousGroup()
			endif
			set user = null
			set triggeringTrigger = null
			set eventId = null
		endmethod
		
		private method createSelectionTrigger takes nothing returns nothing
			local integer i
			local player user
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.selectionTrigger = CreateTrigger()
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then
					set triggerEvent = TriggerRegisterKeyEventForPlayer(user, this.selectionTrigger, KEY_UP, true)
					set triggerEvent = null
					set triggerEvent = TriggerRegisterKeyEventForPlayer(user, this.selectionTrigger, KEY_DOWN, true)
					set triggerEvent = null
					set triggerEvent = TriggerRegisterKeyEventForPlayer(user, this.selectionTrigger, KEY_RIGHT, true)
					set triggerEvent = null
					set triggerEvent = TriggerRegisterKeyEventForPlayer(user, this.selectionTrigger, KEY_LEFT, true)
					set triggerEvent = null
				endif
				set user = null
				set i = i + 1
			endloop
			set conditionFunction = Condition(function ANeratiaFight.triggerConditionSelect)
			set triggerCondition = TriggerAddCondition(this.selectionTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.selectionTrigger, function ANeratiaFight.triggerActionSelect)
			set triggerEvent = null
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		/// Was ACharacterSystem
		public static method create takes nothing returns ANeratiaFight
			local ANeratiaFight this = ANeratiaFight.allocate()
			local integer i = 0
			//members
			set this.m_area = 0
			loop
				exitwhen (i == ANeratiaFight.maxGroups)
				set this.m_unitGroup[i] = CreateGroup()
				set i = i + 1
			endloop
			set this.m_userCount = 0
			set this.m_userIndex = -1
			set this.m_hasToSelectAnEnemy = false

			call this.createSelectionTrigger()
			return this
		endmethod
		
		private method destroySelectionTrigger takes nothing returns nothing
			call AGetCharacterHashTable().destroyTrigger(this.selectionTrigger)
			set this.selectionTrigger = null
		endmethod

		public method onDestroy takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == ANeratiaFight.maxGroups)
				call DestroyGroup(this.m_unitGroup[i])
				set this.m_unitGroup[i] = null
				set i = i + 1
			endloop
			set i = 0
			loop
				exitwhen (i == this.m_userCount)
				set this.m_users[i] = null
				set i = i + 1
			endloop
			set this.m_selectedUnit = null
			set this.m_selectedEnemy = null
			
			call this.destroySelectionTrigger()
		endmethod
	endstruct

endlibrary