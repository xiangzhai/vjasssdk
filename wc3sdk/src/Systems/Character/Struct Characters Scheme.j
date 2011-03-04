library AStructSystemsCharacterCharactersScheme requires AModuleCoreGeneralSystemStruct, optional ALibraryCoreDebugMisc, ALibraryCoreGeneralPlayer, AStructCoreInterfaceMultiboardBar, ALibraryCoreInterfaceMisc, ALibraryCoreInterfaceMultiboard, AStructSystemsCharacterCharacter

	/// @todo Should be contained by \ref ACharactersScheme, vJass bug.
	function interface ACharactersSchemeMaxExperience takes unit hero returns integer

	/**
	* Characters scheme uses one multiboard to display information about all playing characters.
	* These information can be:
	* - name
	* - class
	* - level
	* - experience until next level
	* - life
	* - mana
	*/
	struct ACharactersScheme
		// static start members
		private static real m_refreshRate
		private static boolean m_showPlayerName
		private static boolean m_showUnitName
		private static boolean m_showLevel
		private static integer m_experienceLength
		private static ACharactersSchemeMaxExperience m_experienceFormula
		private static integer m_hitPointsLength
		private static integer m_manaLength
		private static string m_textTitle
		private static string m_textLevel
		private static string m_textLeftGame
		// static members
		private static trigger m_refreshTrigger
		private static multiboard m_multiboard
		private static AMultiboardBar array m_experienceBar[12] //bj_MAX_PLAYERS /// @todo vJass bug
		private static AMultiboardBar array m_hitPointsBar[12] //bj_MAX_PLAYERS /// @todo vJass bug
		private static AMultiboardBar array m_manaBar[12] //bj_MAX_PLAYERS /// @todo vJass bug
		private static boolean array m_destroyed[12] //bj_MAX_PLAYERS /// @todo vJass bug
		private static integer m_maxPlayers

		implement ASystemStruct

		//! runtextmacro optional A_STRUCT_DEBUG("\"ACharactersScheme\"")

		private static method create takes nothing returns thistype
			return 0
		endmethod

		private method onDestroy takes nothing returns nothing
		endmethod

		private static method onInit takes nothing returns nothing
			call thistype.setName("ACharactersScheme")
		endmethod

		private static method firstColumnExists takes nothing returns boolean
			return thistype.m_showPlayerName or thistype.m_showUnitName or thistype.m_showLevel
		endmethod

		private static method firstColumnString takes ACharacter character returns string
			local string text = ""
			if (thistype.m_showPlayerName) then
				set text = GetModifiedPlayerName(character.player())
			endif
			if (thistype.m_showUnitName) then
				set text = text + " [" + GetUnitName(character.unit()) + "]"
			endif
			if (thistype.m_showLevel) then
				if (IsUnitType(character.unit(), UNIT_TYPE_HERO)) then
					set text = text + " - " + thistype.m_textLevel + " " + I2S(GetHeroLevel(character.unit()))
				// if hero has overtaken an enemy etc.
				else
					set text = text + " - " + thistype.m_textLevel + " " + I2S(GetUnitLevel(character.unit()))
				endif
			endif
			return text
		endmethod

		private static method triggerActionRefresh takes nothing returns nothing
			local integer i
			local multiboarditem multiboardItem
			local player user
			local string columnString
			set i = 0
			loop
				exitwhen (i == thistype.m_maxPlayers)
				set user = Player(i)
				if ((IsPlayerPlayingUser(user) and ACharacter.playerCharacter(user) != 0) or (ACharacter.shareOnPlayerLeaves() and ACharacter.playerCharacter(user) != 0)) then
					if (thistype.firstColumnExists()) then
						set multiboardItem = MultiboardGetItem(thistype.m_multiboard, i, 0)
						set columnString = thistype.firstColumnString(ACharacter.playerCharacter(user))
						call MultiboardSetItemValue(multiboardItem, columnString)
						call MultiboardSetItemWidth(multiboardItem, StringLength(columnString) * 0.003)
						call MultiboardReleaseItem(multiboardItem)
						set multiboardItem = null
					endif

					if (IsUnitAliveBJ(ACharacter.playerCharacter(user).unit())) then
						if (thistype.m_experienceLength > 0) then
							if (IsUnitType(ACharacter.playerCharacter(user).unit(), UNIT_TYPE_HERO)) then
								call thistype.m_experienceBar[i].setValue(GetHeroXP(ACharacter.playerCharacter(user).unit()))
								call thistype.m_experienceBar[i].setMaxValue(thistype.m_experienceFormula.evaluate(ACharacter.playerCharacter(user).unit()))
								call thistype.m_experienceBar[i].refresh()
							endif
						endif

						if (thistype.m_hitPointsLength > 0) then
							call thistype.m_hitPointsBar[i].setValue(GetUnitState(ACharacter.playerCharacter(user).unit(), UNIT_STATE_LIFE))
							call thistype.m_hitPointsBar[i].setMaxValue(GetUnitState(ACharacter.playerCharacter(user).unit(), UNIT_STATE_MAX_LIFE))
							call thistype.m_hitPointsBar[i].refresh()
						endif

						if (thistype.m_manaLength > 0) then
							call thistype.m_manaBar[i].setValue(GetUnitState(ACharacter.playerCharacter(user).unit(), UNIT_STATE_MANA))
							call thistype.m_manaBar[i].setMaxValue(GetUnitState(ACharacter.playerCharacter(user).unit(), UNIT_STATE_MAX_MANA))
							call thistype.m_manaBar[i].refresh()
						endif
					else
						if (thistype.m_hitPointsLength > 0) then
							call thistype.m_hitPointsBar[i].setValue(0)
							call thistype.m_hitPointsBar[i].setMaxValue(1)
							call thistype.m_hitPointsBar[i].refresh()
						endif

						if (thistype.m_manaLength > 0) then
							call thistype.m_manaBar[i].setValue(0)
							call thistype.m_manaBar[i].setMaxValue(1)
							call thistype.m_manaBar[i].refresh()
						endif
					endif
				elseif (not thistype.m_destroyed[i]) then
					if (thistype.firstColumnExists()) then
						set multiboardItem = MultiboardGetItem(thistype.m_multiboard, i, 0)
						call MultiboardSetItemValue(multiboardItem, thistype.m_textLeftGame)
						call MultiboardSetItemWidth(multiboardItem, StringLength(thistype.m_textLeftGame) * 0.003)
						call MultiboardReleaseItem(multiboardItem)
						set multiboardItem = null
					endif

					if (thistype.m_experienceLength > 0) then
						call thistype.m_experienceBar[i].destroy()
						set thistype.m_experienceBar[i] = 0
					endif

					if (thistype.m_hitPointsLength > 0) then
						call thistype.m_hitPointsBar[i].destroy()
						set thistype.m_hitPointsBar[i] = 0
					endif

					if (thistype.m_manaLength > 0) then
						call thistype.m_manaBar[i].destroy()
						set thistype.m_manaBar[i] = 0
					endif

					set thistype.m_destroyed[i] = true
				endif
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
			local integer column
			set thistype.m_multiboard = CreateMultiboard()
			call MultiboardDisplay(thistype.m_multiboard, false)
			call MultiboardSetTitleText(thistype.m_multiboard, thistype.m_textTitle)
			set column = thistype.m_experienceLength + thistype.m_hitPointsLength + thistype.m_manaLength
			if (thistype.firstColumnExists()) then
				set column = column + 1
			endif
			call MultiboardSetColumnCount(thistype.m_multiboard, column)
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (ACharacter.playerCharacter(user) != 0) then
					call MultiboardSetRowCount(thistype.m_multiboard, MultiboardGetRowCount(thistype.m_multiboard) + 1)

					if (thistype.firstColumnExists.evaluate()) then
						set multiboardItem = MultiboardGetItem(thistype.m_multiboard, i, 0)
						call MultiboardSetItemStyle(multiboardItem, true, false)
						call MultiboardReleaseItem(multiboardItem)
						set multiboardItem = null
					endif

					if (thistype.m_experienceLength > 0) then
						if (thistype.firstColumnExists()) then
							set column = 1
						else
							set column = 0
						endif
						set thistype.m_experienceBar[i] = AMultiboardBar.create(thistype.m_multiboard, column, i, thistype.m_experienceLength, 0.0, true)
					endif

					if (thistype.m_hitPointsLength > 0) then
						if (thistype.m_experienceLength > 0) then
							set column = thistype.m_experienceBar[i].firstFreeField()
						elseif (thistype.firstColumnExists()) then
							set column = 1
						else
							set column = 0
						endif

						set thistype.m_hitPointsBar[i] = AMultiboardBar.create(thistype.m_multiboard, column, i, thistype.m_hitPointsLength, 0.0, true)
					endif

					if (thistype.m_manaLength > 0) then
						if (thistype.m_hitPointsLength > 0) then
							set column = thistype.m_hitPointsBar[i].firstFreeField()
						elseif (thistype.m_experienceLength > 0) then
							set column = thistype.m_experienceBar[i].firstFreeField()
						elseif (thistype.firstColumnExists()) then
							set column = 1
						else
							set column = 0
						endif

						set thistype.m_manaBar[i] = AMultiboardBar.create(thistype.m_multiboard, column, i, 10, 0.0, true)
					endif

					set thistype.m_maxPlayers = i + 1
				else
					set thistype.m_destroyed[i] = true
				endif
				set i = i + 1
			endloop
		endmethod

		/**
		* Call this method before you use this struct!
		* Call this after a trigger sleep action, multiboard is created!
		* Call this AFTER character creation/character class selection
		* @param refreshRate Should be bigger than 0.0.
		* @param experienceFormula Function which returns the maximum experience of a hero.
		*/
		public static method init takes real refreshRate, boolean showPlayerName, boolean showUnitName, boolean showLevel, integer experienceLength, ACharactersSchemeMaxExperience experienceFormula, integer hitPointsLength, integer manaLength, string textTitle, string textLevel, string textLeftGame returns nothing
			//static start members
			set thistype.m_refreshRate = refreshRate
			debug if (refreshRate <= 0) then
				debug call thistype.staticPrint("Refresh rate is <= 0")
			debug endif
			set thistype.m_showPlayerName = showPlayerName
			set thistype.m_showUnitName = showUnitName
			set thistype.m_showLevel = showLevel
			set thistype.m_experienceLength = experienceLength
			set thistype.m_experienceFormula = experienceFormula
			set thistype.m_hitPointsLength = hitPointsLength
			set thistype.m_manaLength = manaLength
			set thistype.m_textTitle = textTitle
			set thistype.m_textLevel = textLevel
			set thistype.m_textLeftGame = textLeftGame
			//static members
			call thistype.createRefreshTrigger()
			call thistype.createMultiboard()
			call thistype.initialize()
		endmethod

		public static method setExperienceBarValueIconForCharacter takes ACharacter character, integer length, string valueIcon returns nothing
			debug if (GetPlayerId(character.player()) >= thistype.m_maxPlayers) then
				debug call thistype.staticPrint("Character " + I2S(character) + " is not contained.")
			debug endif
			call thistype.m_experienceBar[GetPlayerId(character.player())].setValueIcon(length, valueIcon)
		endmethod

		public static method setExperienceBarEmptyIconForCharacter takes ACharacter character, integer length, string emptyIcon returns nothing
			debug if (GetPlayerId(character.player()) >= thistype.m_maxPlayers) then
				debug call thistype.staticPrint("Character " + I2S(character) + " is not contained.")
			debug endif
			call thistype.m_experienceBar[GetPlayerId(character.player())].setEmptyIcon(length, emptyIcon)
		endmethod

		public static method setExperienceBarValueIcon takes integer length, string valueIcon returns nothing
			local integer i = 0
			loop
				exitwhen (i == thistype.m_maxPlayers)
				call thistype.m_experienceBar[i].setValueIcon(length, valueIcon)
				set i = i + 1
			endloop
		endmethod

		public static method setExperienceBarEmptyIcon takes integer length, string emptyIcon returns nothing
			local integer i = 0
			loop
				exitwhen (i == thistype.m_maxPlayers)
				call thistype.m_experienceBar[i].setEmptyIcon(length, emptyIcon)
				set i = i + 1
			endloop
		endmethod

		public static method setHitPointsBarValueIconForCharacter takes ACharacter character, integer length, string valueIcon returns nothing
			debug if (GetPlayerId(character.player()) >= thistype.m_maxPlayers) then
				debug call thistype.staticPrint("Character " + I2S(character) + " is not contained.")
			debug endif
			call thistype.m_hitPointsBar[GetPlayerId(character.player())].setValueIcon(length, valueIcon)
		endmethod

		public static method setHitPointsBarEmptyIconForCharacter takes ACharacter character, integer length, string emptyIcon returns nothing
			debug if (GetPlayerId(character.player()) >= thistype.m_maxPlayers) then
				debug call thistype.staticPrint("Character " + I2S(character) + " is not contained.")
			debug endif
			call thistype.m_hitPointsBar[GetPlayerId(character.player())].setEmptyIcon(length, emptyIcon)
		endmethod

		public static method setHitPointsBarValueIcon takes integer length, string valueIcon returns nothing
			local integer i = 0
			loop
				exitwhen (i == thistype.m_maxPlayers)
				call thistype.m_hitPointsBar[i].setValueIcon(length, valueIcon)
				set i = i + 1
			endloop
		endmethod

		public static method setHitPointsBarEmptyIcon takes integer length, string emptyIcon returns nothing
			local integer i = 0
			loop
				exitwhen (i == thistype.m_maxPlayers)
				call thistype.m_hitPointsBar[i].setEmptyIcon(length, emptyIcon)
				set i = i + 1
			endloop
		endmethod

		public static method setManaBarValueIconForCharacter takes ACharacter character, integer length, string valueIcon returns nothing
			debug if (GetPlayerId(character.player()) >= thistype.m_maxPlayers) then
				debug call thistype.staticPrint("Character " + I2S(character) + " is not contained.")
			debug endif
			call thistype.m_manaBar[GetPlayerId(character.player())].setValueIcon(length, valueIcon)
		endmethod

		public static method setManaBarEmptyIconForCharacter takes ACharacter character, integer length, string emptyIcon returns nothing
			debug if (GetPlayerId(character.player()) >= thistype.m_maxPlayers) then
				debug call thistype.staticPrint("Character " + I2S(character) + " is not contained.")
			debug endif
			call thistype.m_manaBar[GetPlayerId(character.player())].setEmptyIcon(length, emptyIcon)
		endmethod

		public static method setManaBarValueIcon takes integer length, string valueIcon returns nothing
			local integer i = 0
			loop
				exitwhen (i == thistype.m_maxPlayers)
				call thistype.m_manaBar[i].setValueIcon(length, valueIcon)
				set i = i + 1
			endloop
		endmethod

		public static method setManaBarEmptyIcon takes integer length, string emptyIcon returns nothing
			local integer i = 0
			loop
				exitwhen (i == thistype.m_maxPlayers)
				call thistype.m_manaBar[i].setEmptyIcon(length, emptyIcon)
				set i = i + 1
			endloop
		endmethod

		public static method show takes nothing returns nothing
			call thistype.triggerActionRefresh()
			call EnableTrigger(thistype.m_refreshTrigger)
			call MultiboardDisplay(thistype.m_multiboard, true)
		endmethod

		public static method showForPlayer takes player user returns nothing
			call thistype.triggerActionRefresh()
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