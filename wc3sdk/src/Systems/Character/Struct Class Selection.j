library AStructSystemsCharacterClassSelection requires optional ALibraryCoreDebugMisc, AStructCoreGeneralHashTable, ALibraryCoreGeneralPlayer, ALibraryCoreInterfaceCinematic, ALibraryCoreInterfaceMisc, ALibraryCoreInterfaceMultiboard, AStructSystemsCharacterCharacter, AStructSystemsCharacterClass

	/// @todo Should be part of @struct AClassSelection, vJass bug.
	function interface AClassSelectionSelectClassAction takes ACharacter character, AClass class, boolean last returns nothing

	/// @todo Should be part of @struct AClassSelection, vJass bug.
	function interface AClassSelectionCharacterCreationAction takes AClassSelection classSelection, unit whichUnit returns ACharacter

	/// @todo Should be part of @struct AClassSelection, vJass bug.
	function interface AClassSelectionStartGameAction takes nothing returns nothing

	struct AClassSelection
		// static construction members
		private static camerasetup m_cameraSetup
		private static boolean m_hideUserInterface
		private static real m_x
		private static real m_y
		private static real m_facing
		private static real m_refreshRate
		private static real m_rotationAngle
		private static AClass m_firstClass
		private static AClass m_lastClass
		private static AClassSelectionCharacterCreationAction m_characterCreationAction
		private static AClassSelectionStartGameAction m_startGameAction
		private static string m_strengthIconPath
		private static string m_agilityIconPath
		private static string m_intelligenceIconPath
		private static string m_textTitle
		private static string m_textStrength
		private static string m_textAgility
		private static string m_textIntelligence
		private static string m_textAbilities
		private static string m_textDescription
		// static members
		private static thistype array m_playerClassSelection[12] /// @todo bj_MAX_PLAYERS, JassHelper bug
		private static integer m_stack //required for the start game action
		// construction members
		private player m_user
		private real m_startX
		private real m_startY
		private real m_startFacing
		private AClassSelectionSelectClassAction m_selectClassAction
		// members
		private unit m_classUnit
		private trigger m_leaveTrigger
		private trigger m_refreshTrigger
		private trigger m_changePreviousTrigger
		private trigger m_changeNextTrigger
		private trigger m_selectTrigger
		private multiboard m_infoSheet
		private AClass m_class

		//! runtextmacro optional A_STRUCT_DEBUG("\"AClassSelection\"")

		// construction members

		public method player takes nothing returns player
			return this.m_user
		endmethod

		public method startX takes nothing returns real
			return this.m_startX
		endmethod

		public method startY takes nothing returns real
			return this.m_startY
		endmethod

		public method startFacing takes nothing returns real
			return this.m_startFacing
		endmethod

		// methods

		private method selectClass takes nothing returns nothing
			local integer i
			local ACharacter character = 0
			local unit whichUnit = this.m_class.generateUnit(this.m_user, this.m_startX, this.m_startY, this.m_startFacing)

			if (thistype.m_characterCreationAction != 0) then
				set character = thistype.m_characterCreationAction.evaluate(this, whichUnit)
				call ACharacter.setPlayerCharacterByCharacter(character)
			else
				set character = ACharacter.setPlayerCharacter(this.m_user, whichUnit)
			endif

			if (GetPlayerController(this.m_user) == MAP_CONTROL_COMPUTER or (GetPlayerSlotState(this.m_user) == PLAYER_SLOT_STATE_LEFT and ACharacter.shareOnPlayerLeaves())) then
				call character.shareControl(true)
				// show info sheet since multiboard with team resources is displayed!
				set i = 0
				loop
					exitwhen (i == bj_MAX_PLAYERS)
					if (IsPlayerPlayingUser(Player(i)) and ACharacter.playerCharacter(Player(i)) == 0) then
						call ShowMultiboardForPlayer(Player(i), thistype.playerClassSelection(Player(i)).m_infoSheet, true)
					endif
					set i = i + 1
				endloop
			endif
			if (thistype.m_hideUserInterface) then
				call SetUserInterfaceForPlayer(this.m_user, true, true)
			endif
			call ResetToGameCameraForPlayer(this.m_user, 0.0)
			call character.setClass(this.m_class)
			call this.m_selectClassAction.execute(character, this.m_class, thistype.m_stack == 1)
			call this.destroy()
		endmethod

		private method selectRandomClass takes nothing returns nothing
			set this.m_class = GetRandomInt(thistype.m_firstClass, thistype.m_lastClass)
			call this.selectClass()
		endmethod

		private method mostLineCharacters takes AStringVector initialVector returns integer
			local AStringVector vector = AStringVector.createByOther(initialVector)
			local integer result = 0
			local integer i
			call vector.pushBack(thistype.m_textAbilities)
			call vector.pushBack(thistype.m_textDescription)
			set i = 0
			loop
				exitwhen (i == vector.size())
				if (StringLength(vector[i]) > result) then
					set result = StringLength(vector[i])
				endif
				set i = i + 1
			endloop
			call vector.destroy()
			return result
		endmethod

		private method refreshInfoSheet takes nothing returns nothing
			local integer count = 0
			local integer i
			local multiboarditem multiboardItem
			local string strengthText
			local string agilityText
			local string intelligenceText
			local AStringVector strings = AStringVector.create()
			local integer index = 0

			if (thistype.m_hideUserInterface) then
				set count = 3
				set strengthText = IntegerArg(thistype.m_textStrength, GetHeroStr(this.m_classUnit, false))
				set agilityText = IntegerArg(thistype.m_textAgility, GetHeroAgi(this.m_classUnit, false))
				set intelligenceText = IntegerArg(thistype.m_textIntelligence, GetHeroInt(this.m_classUnit, false))
				call strings.pushBack(strengthText)
				call strings.pushBack(agilityText)
				call strings.pushBack(intelligenceText)
			endif

			set i = 0
			loop
				exitwhen(i == this.m_class.abilities())
				call strings.pushBack(GetObjectName(this.m_class.ability(i)))
				set i = i + 1
			endloop
			set i = 0
			loop
				exitwhen(i == this.m_class.descriptionLines())
				call strings.pushBack(this.m_class.descriptionLine(i))
				set i = i + 1
			endloop

			//call MultiboardClear(this.m_infoSheet) // clears not everything?!
			call DestroyMultiboard(this.m_infoSheet)
			set this.m_infoSheet = null
			set this.m_infoSheet = CreateMultiboard()
			call MultiboardSetItemsWidth(this.m_infoSheet, this.mostLineCharacters(strings) * 0.005)
			call MultiboardSetColumnCount(this.m_infoSheet, 1)
			call strings.destroy()

			if (this.m_class.abilities() > 0) then
				set count = count + 1 + this.m_class.abilities()
			endif

			if (this.m_class.descriptionLines() > 0) then
				set count = count + 1 + this.m_class.descriptionLines()
			endif

			call MultiboardSetRowCount(this.m_infoSheet, count)
			call MultiboardSetTitleText(this.m_infoSheet, IntegerArg(IntegerArg(StringArg(thistype.m_textTitle, GetUnitName(this.m_classUnit)), this.m_class), thistype.m_lastClass - thistype.m_firstClass + 1))
			if (thistype.m_hideUserInterface) then
				// strength
				set multiboardItem = MultiboardGetItem(this.m_infoSheet, 0, 0)
				call MultiboardSetItemStyle(multiboardItem, true, true)
				call MultiboardSetItemIcon(multiboardItem, thistype.m_strengthIconPath)
				call MultiboardSetItemValue(multiboardItem, strengthText)
				call MultiboardReleaseItem(multiboardItem)
				set multiboardItem = null
				// agility
				set multiboardItem = MultiboardGetItem(this.m_infoSheet, 1, 0)
				call MultiboardSetItemStyle(multiboardItem, true, true)
				call MultiboardSetItemIcon(multiboardItem, thistype.m_agilityIconPath)
				call MultiboardSetItemValue(multiboardItem, agilityText)
				call MultiboardReleaseItem(multiboardItem)
				set multiboardItem = null
				// intelligence
				set multiboardItem = MultiboardGetItem(this.m_infoSheet, 2, 0)
				call MultiboardSetItemStyle(multiboardItem, true, true)
				call MultiboardSetItemIcon(multiboardItem, thistype.m_intelligenceIconPath)
				call MultiboardSetItemValue(multiboardItem, intelligenceText)
				call MultiboardReleaseItem(multiboardItem)
				set multiboardItem = null
				set index = 3
			endif

			if (this.m_class.abilities() > 0) then
				set multiboardItem = MultiboardGetItem(this.m_infoSheet, index, 0)
				call MultiboardSetItemStyle(multiboardItem, true, false)
				call MultiboardSetItemValue(multiboardItem, thistype.m_textAbilities)
				call MultiboardReleaseItem(multiboardItem)
				set multiboardItem = null
				set index = index + 1

				set i = 0
				loop
					exitwhen(i == this.m_class.abilities())
					set multiboardItem = MultiboardGetItem(this.m_infoSheet, index, 0)
					call MultiboardSetItemStyle(multiboardItem, true, true)
					call MultiboardSetItemIcon(multiboardItem, this.m_class.abilityIconPath(i))
					call MultiboardSetItemValue(multiboardItem, GetObjectName(this.m_class.ability(i)))
					call MultiboardReleaseItem(multiboardItem)
					set multiboardItem = null
					set i = i + 1
					set index = index + 1
				endloop
			endif

			if (this.m_class.descriptionLines() > 0) then
				set multiboardItem = MultiboardGetItem(this.m_infoSheet, index, 0)
				call MultiboardSetItemStyle(multiboardItem, true, false)
				call MultiboardSetItemValue(multiboardItem, thistype.m_textDescription)
				call MultiboardReleaseItem(multiboardItem)
				set multiboardItem = null
				set index = index + 1

				set i = 0
				loop
					exitwhen(i == this.m_class.descriptionLines())
					set multiboardItem = MultiboardGetItem(this.m_infoSheet, index, 0)
					call MultiboardSetItemStyle(multiboardItem, true, false)
					call MultiboardSetItemValue(multiboardItem, this.m_class.descriptionLine(i))
					call MultiboardReleaseItem(multiboardItem)
					set multiboardItem = null
					set i = i + 1
					set index = index + 1
				endloop
			endif
			call ShowMultiboardForPlayer(this.m_user, this.m_infoSheet, true)
			call MultiboardMinimize(this.m_infoSheet, false)
		endmethod

		private method createUnit takes nothing returns nothing
			if (this.m_classUnit != null) then
				call RemoveUnit(this.m_classUnit)
				set this.m_classUnit = null
			endif
			set this.m_classUnit = CreateUnit(this.m_user, this.m_class.unitType(), thistype.m_x, thistype.m_y, thistype.m_facing)
			call SetUnitInvulnerable(this.m_classUnit, true)
			call PauseUnit(this.m_classUnit, true)
			call SetUnitPathing(this.m_classUnit, false)
			/// @todo Has to be set although unit is being paused?!
			if (IsUnitType(this.m_classUnit, UNIT_TYPE_HERO)) then
				call SuspendHeroXP(this.m_classUnit, true)
			endif
			// refresh position
			call SetUnitPosition(this.m_classUnit, thistype.m_x, thistype.m_y)
			//call ShowUnit(this.m_classUnit, false)
			/// @todo Desync
			//call ShowUnitForPlayer(this.m_user, this.m_classUnit, true)
			call SetUnitAnimation(this.m_classUnit, this.m_class.animation())
			call PlaySoundFileForPlayer(this.m_user, this.m_class.soundPath())
			//call SetCameraTargetControllerNoZForPlayer(this.user, this.classUnit, 0.0, 0.0, false)
			if (not thistype.m_hideUserInterface) then
				call SelectUnitForPlayerSingle(this.m_classUnit, this.m_user)
			endif

			call this.refreshInfoSheet()
		endmethod

		public method show takes nothing returns nothing
			if (GetPlayerController(this.m_user) == MAP_CONTROL_COMPUTER or GetPlayerSlotState(this.m_user) == PLAYER_SLOT_STATE_LEFT) then
				call this.selectRandomClass()
			else
				call ClearScreenMessagesForPlayer(this.m_user)
				if (thistype.m_hideUserInterface) then
					call SetUserInterfaceForPlayer(this.m_user, false, true)
				endif
				call this.createUnit()
			endif
		endmethod

		private static method triggerActionPlayerLeaves takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			local player whichPlayer
			local integer i
			if (ACharacter.shareOnPlayerLeaves()) then
				set i = 0
				loop
					exitwhen (i == bj_MAX_PLAYERS)
					if (i != GetPlayerId(this.m_user)) then
						set whichPlayer = Player(i)
						call SetPlayerAlliance(this.m_user, whichPlayer, ALLIANCE_SHARED_CONTROL, true)
						set whichPlayer = null
					endif
					set i = i + 1
				endloop
			endif
			if (ACharacter.destroyOnPlayerLeaves()) then
				call this.destroy()
			else
				call this.selectClass()
			endif
			set triggeringTrigger = null
		endmethod

		private method createLeaveTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_leaveTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterPlayerEvent(this.m_leaveTrigger, this.m_user, EVENT_PLAYER_LEAVE)
			set triggerAction = TriggerAddAction(this.m_leaveTrigger, function thistype.triggerActionPlayerLeaves)
			call AHashTable.global().setHandleInteger(this.m_leaveTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		private static method triggerActionRefresh takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			debug if (thistype.m_cameraSetup != null) then
				call CameraSetupApplyForPlayer(true, thistype.m_cameraSetup, this.m_user, 0.0)
			debug else
				debug call this.print("No camera object.")
			debug endif
			call SetUnitFacingTimed(this.m_classUnit, GetUnitFacing(this.m_classUnit) + thistype.m_rotationAngle, thistype.m_refreshRate)
			set triggeringTrigger = null
		endmethod

		private method createRefreshTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			if (thistype.m_refreshRate > 0.0) then
				set this.m_refreshTrigger = CreateTrigger()
				set triggerEvent = TriggerRegisterTimerEvent(this.m_refreshTrigger, thistype.m_refreshRate, true)
				set triggerAction = TriggerAddAction(this.m_refreshTrigger, function thistype.triggerActionRefresh)
				call AHashTable.global().setHandleInteger(this.m_refreshTrigger, "this", this)
				set triggerEvent = null
				set triggerAction = null
			endif
		endmethod

		private static method triggerActionChangeToPrevious takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			if (this.m_class == thistype.m_firstClass) then
				set this.m_class = thistype.m_lastClass
			else
				set this.m_class = this.m_class - 1
			endif
			call this.createUnit()
			set triggeringTrigger = null
		endmethod

		private method createChangePreviousTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_changePreviousTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterKeyEventForPlayer(this.m_user, this.m_changePreviousTrigger, AKeyLeft, true)
			set triggerAction = TriggerAddAction(this.m_changePreviousTrigger, function thistype.triggerActionChangeToPrevious)
			call AHashTable.global().setHandleInteger(this.m_changePreviousTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		private static method triggerActionChangeToNext takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			if (this.m_class == thistype.m_lastClass) then
				set this.m_class = thistype.m_firstClass
			else
				set this.m_class = this.m_class + 1
			endif
			call this.createUnit()
			set triggeringTrigger = null
		endmethod

		private method createChangeNextTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_changeNextTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterKeyEventForPlayer(this.m_user, this.m_changeNextTrigger, AKeyRight, true)
			set triggerAction = TriggerAddAction(this.m_changeNextTrigger, function thistype.triggerActionChangeToNext)
			call AHashTable.global().setHandleInteger(this.m_changeNextTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		private static method triggerActionSelectClass takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			call this.selectClass()
			set triggeringTrigger = null
		endmethod

		private method createSelectTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_selectTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterKeyEventForPlayer(this.m_user, this.m_selectTrigger, AKeyEscape, true)
			set triggerAction = TriggerAddAction(this.m_selectTrigger, function thistype.triggerActionSelectClass)
			call AHashTable.global().setHandleInteger(this.m_selectTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		private method createInfoSheet takes nothing returns nothing
			set this.m_infoSheet = CreateMultiboard()
		endmethod

		public static method create takes player user, real startX, real startY, real startFacing, AClassSelectionSelectClassAction selectClassAction returns thistype
			local thistype this = thistype.allocate()
			// construction members
			set this.m_user = user
			set this.m_startFacing = startFacing
			set this.m_startX = startX
			set this.m_startY = startY
			set this.m_selectClassAction = selectClassAction
			// members
			set this.m_class = thistype.m_firstClass
			// static members
			set thistype.m_playerClassSelection[GetPlayerId(user)] = this
			set thistype.m_stack = thistype.m_stack + 1

			call this.createLeaveTrigger()
			call this.createRefreshTrigger()
			call this.createChangePreviousTrigger()
			call this.createChangeNextTrigger()
			call this.createSelectTrigger()
			call this.createInfoSheet()
			return this
		endmethod

		private method destroyLeaveTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_leaveTrigger)
			set this.m_leaveTrigger = null
		endmethod

		private method destroyRefreshTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_refreshTrigger)
			set this.m_refreshTrigger = null
		endmethod

		private method destroyChangePreviousTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_changePreviousTrigger)
			set this.m_changePreviousTrigger = null
		endmethod

		private method destroyChangeNextTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_changeNextTrigger)
			set this.m_changeNextTrigger = null
		endmethod

		private method destroySelectTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_selectTrigger)
			set this.m_selectTrigger = null
		endmethod

		private method destroyInfoSheet takes nothing returns nothing
			call DestroyMultiboard(this.m_infoSheet)
			set this.m_infoSheet = null
		endmethod

		private method removeClassUnit takes nothing returns nothing
			call RemoveUnit(this.m_classUnit)
			set this.m_classUnit = null
		endmethod

		private method onDestroy takes nothing returns nothing
			// construction members
			set this.m_user = null
			// static members
			set thistype.m_stack = thistype.m_stack - 1

			call this.destroyLeaveTrigger()
			call this.destroyRefreshTrigger()
			call this.destroyChangePreviousTrigger()
			call this.destroyChangeNextTrigger()
			call this.destroySelectTrigger()
			call this.destroyInfoSheet()
			call this.removeClassUnit()

			if (thistype.m_stack == 0 and thistype.m_startGameAction != 0) then
				call thistype.m_startGameAction.execute()
			endif
		endmethod

		public static method init takes camerasetup cameraSetup, boolean hideUserInterface, real x, real y, real facing, real refreshRate, real rotationAngle, AClass firstClass, AClass lastClass, AClassSelectionCharacterCreationAction characterCreationAction, AClassSelectionStartGameAction startGameAction, string strengthIconPath, string agilityIconPath, string intelligenceIconPath, string textTitle, string textStrength, string textAgility, string textIntelligence, string textAbilities, string textDescription returns nothing
			local integer i
			// static construction members
			set thistype.m_cameraSetup = cameraSetup
			set thistype.m_hideUserInterface = hideUserInterface
			set thistype.m_x = x
			set thistype.m_y = y
			set thistype.m_facing = facing
			set thistype.m_refreshRate = refreshRate
			set thistype.m_rotationAngle = rotationAngle
			set thistype.m_firstClass = firstClass
			set thistype.m_lastClass = lastClass
			set thistype.m_characterCreationAction = characterCreationAction
			set thistype.m_startGameAction = startGameAction
			set thistype.m_strengthIconPath = strengthIconPath
			set thistype.m_agilityIconPath = agilityIconPath
			set thistype.m_intelligenceIconPath = intelligenceIconPath
			set thistype.m_textTitle = textTitle
			set thistype.m_textStrength = textStrength
			set thistype.m_textAgility = textAgility
			set thistype.m_textIntelligence = textIntelligence
			set thistype.m_textAbilities = textAbilities
			set thistype.m_textDescription = textDescription
			//static members
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set thistype.m_playerClassSelection[i] = 0
				set i = i + 1
			endloop
			set thistype.m_stack = 0
		endmethod

		public static method playerClassSelection takes player whichPlayer returns thistype
			return thistype.m_playerClassSelection[GetPlayerId(whichPlayer)]
		endmethod
	endstruct

endlibrary