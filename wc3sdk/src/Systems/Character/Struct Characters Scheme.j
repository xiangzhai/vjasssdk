library AStructSystemsCharacterCharactersScheme requires ALibraryCoreDebugMisc, ALibraryCoreGeneralPlayer, AStructCoreInterfaceMultiboardBar, ALibraryCoreInterfaceMisc, ALibraryCoreInterfaceMultiboard, AStructSystemsCharacterCharacter

	struct ACharactersScheme
		//static start members
		private static real m_refreshRate
		private static integer m_barLength
		private static string m_textTitle
		private static string m_textLevel
		private static string m_textLeftGame
		//static members
		private static trigger m_refreshTrigger
		private static multiboard m_multiboard
		private static AMultiboardBar array m_hitPointsBar[12] //bj_MAX_PLAYERS /// @todo vJass bug
		private static AMultiboardBar array m_manaBar[12] //bj_MAX_PLAYERS /// @todo vJass bug
		private static integer m_maxPlayers
		
		//! runtextmacro optional A_STRUCT_DEBUG("\"ACharactersScheme\"")

		private static method create takes nothing returns thistype
			return 0
		endmethod

		private method onDestroy takes nothing returns nothing
		endmethod

		private static method triggerActionRefresh takes nothing returns nothing
			local integer i
			local multiboarditem multiboardItem
			local player user
			set i = 0
			loop
				exitwhen (i == thistype.m_maxPlayers)
				set multiboardItem = MultiboardGetItem(thistype.m_multiboard, i, 0)
				set user = Player(i)
				if ((IsPlayerPlayingUser(user) and ACharacter.playerCharacter(user) != 0) or (ACharacter.shareOnPlayerLeaves() and ACharacter.playerCharacter(user) != 0)) then
					//refresh name (class - unit name) - level
					call MultiboardSetItemValue(multiboardItem, GetModifiedPlayerName(user) + " [" + GetUnitName(ACharacter.playerCharacter(user).unit()) + "] - " + thistype.m_textLevel + " " + I2S(GetHeroLevel(ACharacter.playerCharacter(user).unit())))

					if (IsUnitAliveBJ(ACharacter.playerCharacter(user).unit())) then
						call thistype.m_hitPointsBar[i].setValue(GetUnitState(ACharacter.playerCharacter(user).unit(), UNIT_STATE_LIFE))
						call thistype.m_hitPointsBar[i].setMaxValue(GetUnitState(ACharacter.playerCharacter(user).unit(), UNIT_STATE_MAX_LIFE))
						call thistype.m_hitPointsBar[i].refresh()

						call thistype.m_manaBar[i].setValue(GetUnitState(ACharacter.playerCharacter(user).unit(), UNIT_STATE_MANA))
						call thistype.m_manaBar[i].setMaxValue(GetUnitState(ACharacter.playerCharacter(user).unit(), UNIT_STATE_MAX_MANA))
						call thistype.m_manaBar[i].refresh()
					else
						call thistype.m_hitPointsBar[i].setValue(0)
						call thistype.m_hitPointsBar[i].setMaxValue(1)
						call thistype.m_hitPointsBar[i].refresh()
						call thistype.m_manaBar[i].setValue(0)
						call thistype.m_manaBar[i].setMaxValue(1)
						call thistype.m_manaBar[i].refresh()
						
					endif
				elseif (thistype.m_hitPointsBar[i] != 0) then
					call MultiboardSetItemValue(multiboardItem, thistype.m_textLeftGame)

					call AMultiboardBar.destroy(thistype.m_hitPointsBar[i])
					set thistype.m_hitPointsBar[i] = 0
					call AMultiboardBar.destroy(thistype.m_manaBar[i])
					set thistype.m_manaBar[i] = 0
				endif
				call MultiboardReleaseItem(multiboardItem)
				set multiboardItem = null
				set i = i + 1
			endloop
		endmethod

		private static method createRefreshTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set thistype.m_refreshTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterTimerEvent(thistype.m_refreshTrigger, thistype.m_refreshRate, true)
			set triggerAction = TriggerAddAction(thistype.m_refreshTrigger, function thistype.triggerActionRefresh)
			call DisableTrigger(thistype.m_refreshTrigger)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		/// Call this AFTER character creation/character class selection
		private static method createMultiboard takes nothing returns nothing
			local integer i
			local player user
			local multiboarditem multiboardItem
			set thistype.m_multiboard = CreateMultiboard()
			call MultiboardSetTitleText(thistype.m_multiboard, thistype.m_textTitle)
			call MultiboardSetColumnCount(thistype.m_multiboard, 12)
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				//if (IsPlayerPlayingUser(user)) then // computer players are allowed now
				if (ACharacter.playerCharacter(user) != 0) then
					call MultiboardSetRowCount(thistype.m_multiboard, MultiboardGetRowCount(thistype.m_multiboard) + 1)
					set multiboardItem = MultiboardGetItem(thistype.m_multiboard, i, 0)
					call MultiboardSetItemWidth(multiboardItem, 0.35) /// @todo check it
					call MultiboardSetItemStyle(multiboardItem, true, false)
					call MultiboardReleaseItem(multiboardItem)
					set multiboardItem = null

					set thistype.m_hitPointsBar[i] = AMultiboardBar.create(thistype.m_multiboard, 1, i, 10, 0.0, true, 0.0, 0.0, 0, 0)
					call thistype.m_hitPointsBar[i].setAllIcons("Icons\\Interface\\Bars\\White.blp", false) //empty icons
					call thistype.m_hitPointsBar[i].setAllIcons("Icons\\Interface\\Bars\\Green.blp", true)
					
					set multiboardItem = MultiboardGetItem(thistype.m_multiboard, i, thistype.m_hitPointsBar[i].firstFreeField())
					//call MultiboardSetItemWidth(multiboardItem, 0.08) /// @todo check it
					call MultiboardSetItemStyle(multiboardItem, false, true)
					call MultiboardReleaseItem(multiboardItem)
					//set separator icon
					set multiboardItem = null

					set thistype.m_manaBar[i] = AMultiboardBar.create(thistype.m_multiboard, thistype.m_hitPointsBar[i].firstFreeField() + 1, i, 10, 0.0, true, 0.0, 0.0, 0, 0)
					call thistype.m_manaBar[i].setAllIcons("Icons\\Interface\\Bars\\White.blp", false) //empty icons
					call thistype.m_manaBar[i].setAllIcons("Icons\\Interface\\Bars\\Blue.blp", true)
					set thistype.m_maxPlayers = i + 1
				endif
				//endif
				set i = i + 1
			endloop
		endmethod

		/// Call this method before you use this struct!
		/// Call this after a trigger sleep action, multiboard is created!
		/// Call this AFTER character creation/character class selection
		/// @param refreshRate Should be bigger than 0.0.
		public static method init takes real refreshRate, integer barLength, string textTitle, string textLevel, string textLeftGame returns nothing
			//static start members
			set thistype.m_refreshRate = refreshRate
			debug if (refreshRate <= 0) then
				debug call thistype.staticPrint("Refresh rate is <= 0")
			debug endif
			set thistype.m_barLength = barLength
			set thistype.m_textTitle = textTitle
			set thistype.m_textLevel = textLevel
			set thistype.m_textLeftGame = textLeftGame
			//static members
			call thistype.createRefreshTrigger()
			call thistype.createMultiboard()
		endmethod

		public static method show takes nothing returns nothing
			call EnableTrigger(thistype.m_refreshTrigger)
			call MultiboardDisplay(thistype.m_multiboard, true)
		endmethod
		
		public static method showForPlayer takes player user returns nothing
			call ShowMultiboardForPlayer(user, thistype.m_multiboard, true)
		endmethod

		public static method hide takes nothing returns nothing
			call MultiboardDisplay(thistype.m_multiboard, false)
			call DisableTrigger(thistype.m_refreshTrigger)
		endmethod
		
		public static method hideForPlayer takes player user returns nothing
			call ShowMultiboardForPlayer(user, thistype.m_multiboard, false)
		endmethod

		public static method maximize takes nothing returns nothing
			call EnableTrigger(thistype.m_refreshTrigger)
			call MultiboardMinimize(thistype.m_multiboard, false)
		endmethod

		public static method minimize takes nothing returns nothing
			call MultiboardMinimize(thistype.m_multiboard, true)
			call DisableTrigger(thistype.m_refreshTrigger)
		endmethod
	endstruct

endlibrary