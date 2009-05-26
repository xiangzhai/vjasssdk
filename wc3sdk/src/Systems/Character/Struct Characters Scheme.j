library AStructSystemsCharacterCharactersScheme requires ALibraryCoreDebugMisc, ALibraryCoreGeneralPlayer, AStructCoreInterfaceMultiboardBar, ALibraryCoreInterfaceMisc, AStructSystemsCharacterCharacter

	struct ACharactersScheme
		//static start members
		private static real refreshRate
		private static integer barLength
		private static string textTitle
		private static string textLevel
		private static string textLeftGame
		//static members
		private static trigger refreshTrigger
		private static multiboard usedMultiboard
		private static AMultiboardBar array hitPointsBar[12] //bj_MAX_PLAYERS /// @todo vJass bug
		private static AMultiboardBar array manaBar[12] //bj_MAX_PLAYERS /// @todo vJass bug
		private static integer maxPlayers
		
		//! runtextmacro A_STRUCT_DEBUG("\"ACharactersScheme\"")

		private static method create takes nothing returns ACharactersScheme
			return 0
		endmethod

		private method onDestroy takes nothing returns nothing
		endmethod

		/// Call this method before you use this struct!
		/// Call this after a trigger sleep action, multiboard is created!
		/// @param refreshRate Should be bigger than 0.0.
		public static method init takes real refreshRate, integer barLength, string textTitle, string textLevel, string textLeftGame returns nothing
			//static start members
			set ACharactersScheme.refreshRate = refreshRate
			debug if (refreshRate <= 0) then
				debug call ACharactersScheme.staticPrint("Refresh rate is <= 0")
			debug endif
			set ACharactersScheme.barLength = barLength
			set ACharactersScheme.textTitle = textTitle
			set ACharactersScheme.textLevel = textLevel
			set ACharactersScheme.textLeftGame = textLeftGame
			//static members
			call ACharactersScheme.createRefreshTrigger()
			call ACharactersScheme.createMultiboard()
		endmethod

		public static method show takes nothing returns nothing
			call EnableTrigger(ACharactersScheme.refreshTrigger)
			call MultiboardDisplay(ACharactersScheme.usedMultiboard, true)
		endmethod

		public static method hide takes nothing returns nothing
			call MultiboardDisplay(ACharactersScheme.usedMultiboard, false)
			call DisableTrigger(ACharactersScheme.refreshTrigger)
		endmethod

		public static method maximize takes nothing returns nothing
			call EnableTrigger(ACharactersScheme.refreshTrigger)
			call MultiboardMinimize(ACharactersScheme.usedMultiboard, false)
		endmethod

		public static method minimize takes nothing returns nothing
			call MultiboardMinimize(ACharactersScheme.usedMultiboard, true)
			call DisableTrigger(ACharactersScheme.refreshTrigger)
		endmethod
		
		private static method triggerActionRefresh takes nothing returns nothing
			local integer i
			local multiboarditem multiboardItem
			local player user
			set i = 0
			loop
				exitwhen (i == ACharactersScheme.maxPlayers)
				set multiboardItem = MultiboardGetItem(ACharactersScheme.usedMultiboard, i, 0)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then //ALibraryGeneralPlayer
					//refresh name (class - unit name) - level
					call MultiboardSetItemValue(multiboardItem, GetModifiedPlayerName(user) + " [" + GetUnitName(ACharacter.getPlayerCharacter(user).getUsedUnit()) + "] - " + ACharactersScheme.textLevel + " " + I2S(GetHeroLevel(ACharacter.getPlayerCharacter(user).getUsedUnit()))) //ALibraryInterfaceMisc

					if (IsUnitAliveBJ(ACharacter.getPlayerCharacter(user).getUsedUnit())) then
						call ACharactersScheme.hitPointsBar[i].setValue(GetUnitState(ACharacter.getPlayerCharacter(user).getUsedUnit(), UNIT_STATE_LIFE))
						call ACharactersScheme.hitPointsBar[i].setMaxValue(GetUnitState(ACharacter.getPlayerCharacter(user).getUsedUnit(), UNIT_STATE_MAX_LIFE))
						call ACharactersScheme.hitPointsBar[i].refresh()

						call ACharactersScheme.manaBar[i].setValue(GetUnitState(ACharacter.getPlayerCharacter(user).getUsedUnit(), UNIT_STATE_MANA))
						call ACharactersScheme.manaBar[i].setMaxValue(GetUnitState(ACharacter.getPlayerCharacter(user).getUsedUnit(), UNIT_STATE_MAX_MANA))
						call ACharactersScheme.manaBar[i].refresh()
					else
						call ACharactersScheme.hitPointsBar[i].setValue(0)
						call ACharactersScheme.hitPointsBar[i].refresh()
						call ACharactersScheme.manaBar[i].setValue(0)
						call ACharactersScheme.manaBar[i].refresh()
						
					endif
				elseif (ACharactersScheme.hitPointsBar[i] != 0) then
					call MultiboardSetItemValue(multiboardItem, ACharactersScheme.textLeftGame)

					call AMultiboardBar.destroy(ACharactersScheme.hitPointsBar[i])
					set ACharactersScheme.hitPointsBar[i] = 0
					call AMultiboardBar.destroy(ACharactersScheme.manaBar[i])
					set ACharactersScheme.manaBar[i] = 0
				endif
				call MultiboardReleaseItem(multiboardItem)
				set multiboardItem = null
				set i = i + 1
			endloop
		endmethod

		private static method createRefreshTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			debug call ACharactersScheme.staticPrint("TRIGGER 1")
			set ACharactersScheme.refreshTrigger = CreateTrigger()
			debug call ACharactersScheme.staticPrint("TRIGGER 2")
			set triggerEvent = TriggerRegisterTimerEvent(ACharactersScheme.refreshTrigger, ACharactersScheme.refreshRate, true)
			debug call ACharactersScheme.staticPrint("TRIGGER 3")
			set triggerAction = TriggerAddAction(ACharactersScheme.refreshTrigger, function ACharactersScheme.triggerActionRefresh)
			debug call ACharactersScheme.staticPrint("TRIGGER 4")
			call DisableTrigger(ACharactersScheme.refreshTrigger)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		private static method createMultiboard takes nothing returns nothing
			local integer i
			local player user
			local multiboarditem multiboardItem

			set ACharactersScheme.usedMultiboard = CreateMultiboard()
			debug call ACharactersScheme.staticPrint("1")
			call MultiboardSetTitleText(ACharactersScheme.usedMultiboard, ACharactersScheme.textTitle)
			debug call ACharactersScheme.staticPrint("2")
			call MultiboardSetColumnCount(ACharactersScheme.usedMultiboard, 12)
			debug call ACharactersScheme.staticPrint("3")

			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then
					call MultiboardSetRowCount(ACharactersScheme.usedMultiboard, MultiboardGetRowCount(ACharactersScheme.usedMultiboard) + 1)
					set multiboardItem = MultiboardGetItem(ACharactersScheme.usedMultiboard, i, 0)
					call MultiboardSetItemWidth(multiboardItem, 0.35) /// @todo check it
					debug call ACharactersScheme.staticPrint("4")
					call MultiboardReleaseItem(multiboardItem) // added second release call
					call MultiboardSetItemStyle(multiboardItem, true, false)
					call MultiboardReleaseItem(multiboardItem)
					debug call ACharactersScheme.staticPrint("5")
					set multiboardItem = null

					set ACharactersScheme.hitPointsBar[i] = AMultiboardBar.create(ACharactersScheme.usedMultiboard, 1, i, 10, 0.0, true, 0.0, 0.0, 0, 0)
					call ACharactersScheme.hitPointsBar[i].setAllIcons("Icons\\Interface\\Bars\\White.blp", false) //empty icons
					call ACharactersScheme.hitPointsBar[i].setAllIcons("Icons\\Interface\\Bars\\Green.blp", true)
					debug call ACharactersScheme.staticPrint("6")
					
					set multiboardItem = MultiboardGetItem(ACharactersScheme.usedMultiboard, i, ACharactersScheme.hitPointsBar[i].getFirstFreeField())
					call MultiboardSetItemWidth(multiboardItem, 0.03) /// @todo check it
					call MultiboardReleaseItem(multiboardItem)
					call MultiboardSetItemStyle(multiboardItem, true, false)
					call MultiboardReleaseItem(multiboardItem)
					call MultiboardSetItemValue(multiboardItem, "|||")
					call MultiboardReleaseItem(multiboardItem)
					set multiboardItem = null

					set ACharactersScheme.manaBar[i] = AMultiboardBar.create(ACharactersScheme.usedMultiboard, ACharactersScheme.hitPointsBar[i].getFirstFreeField() + 1, i, 10, 0.0, true, 0.0, 0.0, 0, 0)
					call ACharactersScheme.manaBar[i].setAllIcons("Icons\\Interface\\Bars\\White.blp", false) //empty icons
					call ACharactersScheme.manaBar[i].setAllIcons("Icons\\Interface\\Bars\\Blue.blp", true)
					debug call ACharactersScheme.staticPrint("7")
					set ACharactersScheme.maxPlayers = i + 1
				endif
				set i = i + 1
			endloop
			debug call ACharactersScheme.staticPrint("8")
		endmethod
	endstruct

endlibrary