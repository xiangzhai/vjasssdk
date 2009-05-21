library AStructSystemsCharacterClassSelection requires ALibraryCoreDebugMisc, ALibraryCoreGeneralPlayer, ALibraryCoreInterfaceCinematic, ALibraryCoreInterfaceMisc, ALibraryCoreInterfaceMultiboard, AStructSystemsCharacterCharacterHashTable, AStructSystemsCharacterCharacter, AStructSystemsCharacterClass

	/// @todo Should be a part of @struct AClassSelection, vJass bug.
	function interface AClassSelectionSelectClassAction takes ACharacter character, AClass class returns nothing

	/// @todo Should be a part of @struct AClassSelection, vJass bug.
	function interface AClassSelectionStartGameAction takes nothing returns nothing

	struct AClassSelection
		//static constant members
		private static constant integer maxInfoSheetItems = 4 + 12 + 1 + 8 //4 + AClass.maxAbilities + 1 + AClass.maxDescriptionLines
		private static constant integer maxInfoSheetIconItems = 4 + 12 //4 + AClass.maxAbilities
		//static start members
		private static camerasetup cameraSetup
		private static real x
		private static real y
		private static real facing
		private static real refreshRate
		private static real rotationAngle
		private static AClass firstClass
		private static AClass lastClass
		private static AClassSelectionStartGameAction startGameAction
		private static string strengthIconPath
		private static string agilityIconPath
		private static string intelligenceIconPath
		private static string textStrength
		private static string textAgility
		private static string textIntelligence
		private static string textAbilities
		private static string textDescription
		//static members
		private static integer stack //required for the start game action
		//start members
		private player user
		private real startX
		private real startY
		private real startFacing
		private AClassSelectionSelectClassAction selectClassAction
		//members
		private unit classUnit
		private trigger refreshTrigger
		private trigger changePreviousTrigger
		private trigger changeNextTrigger
		private trigger selectTrigger
		private multiboard infoSheet
		private AClass class
		
		//! runtextmacro A_STRUCT_DEBUG("\"AClassSelection\"")

		//methods
		
		public method show takes nothing returns nothing
			call ClearScreenMessagesForPlayer(this.user)
			call SetUserInterfaceForPlayer(this.user, false, true)
			call ShowMultiboardForPlayer(this.user, this.infoSheet, true)
			call MultiboardMinimize(this.infoSheet, false)
			call this.createUnit()
		endmethod

		//get new test
		private method refreshInfoSheet takes nothing returns nothing
			local integer i
			local multiboarditem multiboardItem
			call MultiboardSetTitleText(this.infoSheet, GetUnitName(this.classUnit))
			//strength
			set multiboardItem = MultiboardGetItem(this.infoSheet, 0, 0)
			call MultiboardSetItemValue(multiboardItem, AClassSelection.textStrength + ": " + I2S(GetHeroStr(this.classUnit, false)))
			call MultiboardReleaseItem(multiboardItem)
			set multiboardItem = null
			//agility
			set multiboardItem = MultiboardGetItem(this.infoSheet, 1, 0)
			call MultiboardSetItemValue(multiboardItem, AClassSelection.textAgility + ": " + I2S(GetHeroAgi(this.classUnit, false)))
			call MultiboardReleaseItem(multiboardItem)
			set multiboardItem = null
			//intelligence
			set multiboardItem = MultiboardGetItem(this.infoSheet, 2, 0)
			call MultiboardSetItemValue(multiboardItem, AClassSelection.textIntelligence + ": " + I2S(GetHeroInt(this.classUnit, false)))
			call MultiboardReleaseItem(multiboardItem)
			set multiboardItem = null
			//abilities
			set i = 4
			loop
				exitwhen(i == AClassSelection.maxInfoSheetIconItems)
				set multiboardItem = MultiboardGetItem(this.infoSheet, i, 0)
				if (this.class.getUsedAbility(i - 4) != 0) then
					call MultiboardSetItemIcon(multiboardItem, this.class.getAbilityIconPath(i - 4))
					call MultiboardSetItemValue(multiboardItem, GetObjectName(this.class.getUsedAbility(i - 4)))
					call MultiboardSetItemStyle(multiboardItem, true, true)
				else
					call MultiboardSetItemStyle(multiboardItem, false, false)
				endif
				call MultiboardReleaseItem(multiboardItem)
				set multiboardItem = null
				set i = i + 1
			endloop
			//description
			set i = AClassSelection.maxInfoSheetIconItems + 1
			loop
				exitwhen(i == AClassSelection.maxInfoSheetItems)
				set multiboardItem = MultiboardGetItem(this.infoSheet, i, 0)
				if (StringLength(this.class.getDescriptionLine(i - AClassSelection.maxInfoSheetIconItems + 1)) > 0) then
					call MultiboardSetItemValue(multiboardItem, this.class.getDescriptionLine(i - AClassSelection.maxInfoSheetIconItems + 1)) //AClassCharacterClass
					call MultiboardSetItemStyle(multiboardItem, true, false)
				else
					call MultiboardSetItemStyle(multiboardItem, false, false)
				endif
				call MultiboardReleaseItem(multiboardItem)
				set multiboardItem = null
				set i = i + 1
			endloop
		endmethod

		private method createUnit takes nothing returns nothing
			if (this.classUnit != null) then
				call RemoveUnit(this.classUnit)
				set this.classUnit = null
			endif
			set this.classUnit = CreateUnit(this.user, this.class.getUnitType(), AClassSelection.x, AClassSelection.y, AClassSelection.facing) //AClassCharacterClass
			call SetUnitInvulnerable(this.classUnit, true)
			call PauseUnit(this.classUnit, true)
			//Für andere Spieler unsichtbar machen

			call SetUnitAnimation(this.classUnit, this.class.getAnimation())
			call PlaySoundPathForPlayer(this.user, this.class.getSoundPath())
			//call SetCameraTargetControllerNoZForPlayer(this.user, this.classUnit, 0.0, 0.0, false)
		
			call this.refreshInfoSheet()
		endmethod

		private method selectClass takes nothing returns nothing
			call SetUserInterfaceForPlayer(this.user, false, false)
			call ResetToGameCameraForPlayer(this.user, 0.0)
			call PanCameraToForPlayer(this.user, this.startX, this.startY)
			call this.selectClassAction.execute(ACharacter.setPlayerCharacter(this.user, this.class.generateUnit(this.user, this.startX, this.startY, this.startFacing)), this.class)
			call this.destroy()
			if (AClassSelection.stack == 0) then
				call AClassSelection.startGameAction.execute()
			endif
		endmethod

		private static method triggerActionRefresh takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AClassSelection this = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "this") 
			debug if (AClassSelection.cameraSetup != null) then
				call CameraSetupApplyForPlayer(true, AClassSelection.cameraSetup, this.user, 0.0)
			debug else
				debug call this.print("No camera object.")
			debug endif
			call SetUnitFacingTimed(this.classUnit, GetUnitFacing(this.classUnit)  + AClassSelection.rotationAngle, AClassSelection.refreshRate)
			set triggeringTrigger = null
		endmethod

		private method createRefreshTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			if (AClassSelection.refreshRate > 0.0) then
				set this.refreshTrigger = CreateTrigger()
				set triggerEvent = TriggerRegisterTimerEvent(this.refreshTrigger, AClassSelection.refreshRate, true)
				set triggerAction = TriggerAddAction(this.refreshTrigger, function AClassSelection.triggerActionRefresh)
				call AGetCharacterHashTable().storeHandleInteger(this.refreshTrigger, "this", this)
				set triggerEvent = null
				set triggerAction = null
			endif
		endmethod

		private static method triggerActionChangeToPrevious takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AClassSelection this = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "this")
			if (this.class == AClassSelection.firstClass) then
				set this.class = AClassSelection.lastClass
			else
				set this.class = this.class - 1
			endif
			call this.createUnit()
			set triggeringTrigger = null
		endmethod

		private method createChangePreviousTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.changePreviousTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterKeyEventForPlayer(this.user, this.changePreviousTrigger, KEY_LEFT, true)
			set triggerAction = TriggerAddAction(this.changePreviousTrigger, function AClassSelection.triggerActionChangeToPrevious)
			call AGetCharacterHashTable().storeHandleInteger(this.changePreviousTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		private static method triggerActionChangeToNext takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AClassSelection this = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "this")
			if (this.class == AClassSelection.lastClass) then
				set this.class = AClassSelection.firstClass
			else
				set this.class = this.class + 1
			endif
			call this.createUnit()
			set triggeringTrigger = null
		endmethod

		private method createChangeNextTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.changeNextTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterKeyEventForPlayer(this.user, this.changeNextTrigger, KEY_RIGHT, true)
			set triggerAction = TriggerAddAction(this.changeNextTrigger, function AClassSelection.triggerActionChangeToNext)
			call AGetCharacterHashTable().storeHandleInteger(this.changeNextTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		private static method triggerActionSelectClass takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AClassSelection this = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "this")
			call this.selectClass()
			set triggeringTrigger = null
		endmethod

		private method createSelectTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.selectTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterKeyEventForPlayer(this.user, this.selectTrigger, KEY_ESCAPE, true)
			set triggerAction = TriggerAddAction(this.selectTrigger, function AClassSelection.triggerActionSelectClass)
			call AGetCharacterHashTable().storeHandleInteger(this.selectTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		//release test
		private method createInfoSheet takes nothing returns nothing
			local integer i
			local multiboarditem multiboardItem
			set this.infoSheet = CreateMultiboard()
			call MultiboardSetColumnCount(this.infoSheet, 1)
			call MultiboardSetRowCount(this.infoSheet, AClassSelection.maxInfoSheetItems)
			set i = 0
			loop
				exitwhen (i == AClassSelection.maxInfoSheetItems)
				set multiboardItem = MultiboardGetItem(this.infoSheet, i, 0)
				if (i < AClassSelection.maxInfoSheetIconItems) then
					call MultiboardSetItemStyle(multiboardItem, true, true)
				else
					call MultiboardSetItemStyle(multiboardItem, true, false)
				endif
				call MultiboardReleaseItem(multiboardItem)
				set multiboardItem = null
				set i = i + 1
			endloop
			call MultiboardSetItemsWidth(this.infoSheet, 0.20)

			//strength
			set multiboardItem = MultiboardGetItem(this.infoSheet, 0, 0)
			call MultiboardSetItemIcon(multiboardItem, AClassSelection.strengthIconPath)
			call MultiboardReleaseItem(multiboardItem)
			set multiboardItem = null
			//agility
			set multiboardItem = MultiboardGetItem(this.infoSheet, 1, 0)
			call MultiboardSetItemIcon(multiboardItem, AClassSelection.agilityIconPath)
			call MultiboardReleaseItem(multiboardItem)
			set multiboardItem = null
			//intelligence
			set multiboardItem = MultiboardGetItem(this.infoSheet, 2, 0)
			call MultiboardSetItemIcon(multiboardItem, AClassSelection.intelligenceIconPath)
			call MultiboardReleaseItem(multiboardItem)
			set multiboardItem = null
			//abilities
			set multiboardItem = MultiboardGetItem(this.infoSheet, 3, 0)
			call MultiboardSetItemValue(multiboardItem, AClassSelection.textAbilities + ": ")
			call MultiboardReleaseItem(multiboardItem)
			set multiboardItem = null
			//description
			set multiboardItem = MultiboardGetItem(this.infoSheet, AClassSelection.maxInfoSheetIconItems, 0)
			call MultiboardSetItemValue(multiboardItem, AClassSelection.textDescription + ": ")
			call MultiboardReleaseItem(multiboardItem)
			set multiboardItem = null
		endmethod

		public static method create takes player user, real startX, real startY, real startFacing, AClassSelectionSelectClassAction selectClassAction returns AClassSelection
			local AClassSelection this = AClassSelection.allocate()
			//start members
			set this.user = user
			set this.startFacing = startFacing
			set this.startX = startX
			set this.startY = startY
			set this.selectClassAction = selectClassAction
			//members
			set this.class = AClassSelection.firstClass
			//static members
			set AClassSelection.stack = AClassSelection.stack + 1

			call this.createRefreshTrigger()
			call this.createChangePreviousTrigger()
			call this.createChangeNextTrigger()
			call this.createSelectTrigger()
			call this.createInfoSheet()
			return this
		endmethod

		private method destroyRefreshTrigger takes nothing returns nothing
			call AGetCharacterHashTable().destroyTrigger(this.refreshTrigger)
			set this.refreshTrigger = null
		endmethod

		private method destroyChangePreviousTrigger takes nothing returns nothing
			call AGetCharacterHashTable().destroyTrigger(this.changePreviousTrigger)
			set this.changePreviousTrigger = null
		endmethod

		private method destroyChangeNextTrigger takes nothing returns nothing
			call AGetCharacterHashTable().destroyTrigger(this.changeNextTrigger)
			set this.changeNextTrigger = null
		endmethod

		private method destroySelectTrigger takes nothing returns nothing
			call AGetCharacterHashTable().destroyTrigger(this.selectTrigger)
			set this.selectTrigger = null
		endmethod

		private method destroyInfoSheet takes nothing returns nothing
			call DestroyMultiboard(this.infoSheet)
			set this.infoSheet = null
		endmethod
		
		private method removeClassUnit takes nothing returns nothing
			call RemoveUnit(this.classUnit)
			set this.classUnit = null
		endmethod

		private method onDestroy takes nothing returns nothing
			//start members
			set this.user = null
			//static members
			set AClassSelection.stack = AClassSelection.stack - 1

			call this.destroyRefreshTrigger()
			call this.destroyChangePreviousTrigger()
			call this.destroyChangeNextTrigger()
			call this.destroySelectTrigger()
			call this.destroyInfoSheet()
			call this.removeClassUnit()
		endmethod
		
		public static method init takes camerasetup cameraSetup, real x, real y, real facing, real refreshRate, real rotationAngle, AClass firstClass, AClass lastClass, AClassSelectionStartGameAction startGameAction, string strengthIconPath, string agilityIconPath, string intelligenceIconPath, string textStrength, string textAgility, string textIntelligence, string textAbilities, string textDescription returns nothing
			//static start members
			set AClassSelection.cameraSetup = cameraSetup
			set AClassSelection.x = x
			set AClassSelection.y = y
			set AClassSelection.facing = facing
			set AClassSelection.refreshRate = refreshRate
			set AClassSelection.rotationAngle = rotationAngle
			set AClassSelection.firstClass = firstClass
			set AClassSelection.lastClass = lastClass
			set AClassSelection.startGameAction = startGameAction
			set AClassSelection.strengthIconPath = strengthIconPath
			set AClassSelection.agilityIconPath = agilityIconPath
			set AClassSelection.intelligenceIconPath = intelligenceIconPath
			set AClassSelection.textStrength = textStrength
			set AClassSelection.textAgility = textAgility
			set AClassSelection.textIntelligence = textIntelligence
			set AClassSelection.textAbilities = textAbilities
			set AClassSelection.textDescription = textDescription
			//static members
			set AClassSelection.stack = 0
		endmethod
	endstruct

endlibrary