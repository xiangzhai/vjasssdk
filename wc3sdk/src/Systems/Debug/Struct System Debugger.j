library AStructSystemsDebugSystemDebugger requires ALibraryCoreDebugMisc, AStructCoreGeneralHashTable, ALibraryCoreInterfaceMisc, ASystemsCharacter, ASystemsGui, ASystemsWorld

	/// @todo Should be a method of @struct ASystemDebugger, vJass bug.
	private function onHitActionShowInfo takes AWidget usedWidget returns nothing
		call usedWidget.gui().showInfoDialog("Advanced Script Library")
	endfunction

	/// @todo Should be a method of @struct ASystemDebugger, vJass bug.
	private function onHitActionShowGuiSystemMainWindow takes AWidget usedWidget returns nothing
		call usedWidget.gui().showMainWindowByIndex(0)
	endfunction

	/// @todo Should be a method of @struct ASystemDebugger, vJass bug.
	private function onHitActionShowCharacterSystemMainWindow takes AWidget usedWidget returns nothing
		call usedWidget.gui().showMainWindowByIndex(1)
	endfunction

	/// @todo Should be a method of @struct ASystemDebugger, vJass bug.
	private function onHitActionShowWorldSystemMainWindow takes AWidget usedWidget returns nothing
		call usedWidget.gui().showMainWindowByIndex(2)
	endfunction
	
	/// @todo Should be a method of @struct ASystemDebugger, vJass bug.
	private function onHitActionInitCharacterSystem takes AWidget usedWidget returns nothing
	endfunction

	/// @todo Finish code.
	/// The ASystemDebugger class provides a GUI for testing all systems of the ASL system module.
	/// You can create characters, NPCs, quests etc. easily by using buttons.
	/// Usually this reduces debugging time of the programer.
	struct ASystemDebugger
		//static start members
		private static integer menuKey
		//static members
		private static thistype array m_playerSystemDebugger[12] /// @todo bj_MAX_PLAYERS
		//start members
		private player m_user
		//members
		private trigger m_menuTrigger
		private AMainWindow m_mainWindow
		private AText m_headLineText
		private AButton m_guiSystemsButton
		private AButton m_characterSystemsButton
		private AButton m_worldSystemsButton

		private AMainWindow m_guiSystemWindow
		private AText m_guiSystemHeadLineText
		
		private AMainWindow m_characterSystemWindow
		private AText m_characterSystemHeadLineText
		private AButton m_createCharacterButton
		private AButton m_removeCharacterButton
		
		private AMainWindow worldSystemWindow
		private AText worldSystemHeadLineText
		
		private method showMenu takes nothing returns nothing
			call AGui.playerGui(this.m_user).dialog().clear()
			call AGui.playerGui(this.m_user).dialog().setMessage(tr("System-Debugger"))
			call AGui.playerGui(this.m_user).dialog().addDialogButton(tr("GUI-System"), 0, 0)
			call AGui.playerGui(this.m_user).dialog().addDialogButton(tr("Charaktersystem"), 0, 0)
			call AGui.playerGui(this.m_user).dialog().addDialogButton(tr("Weltsystem"), 0, 0)
			call AGui.playerGui(this.m_user).dialog().show()
		endmethod

		private method createGuiSystemWindow takes nothing returns nothing
			set this.m_guiSystemWindow = AMainWindow.create(AGui.playerGui(this.m_user), 0.0, 0.0, 1000.0, 1000.0, true, 0)
		
			set this.m_guiSystemHeadLineText = AText.create(this.m_mainWindow, 400.0, 200.0, 0.0, 0.0, 0, onTrackActionShowTooltip)
			call this.m_guiSystemHeadLineText.setTextAndSize("GUI-System", 15.0)
			call this.m_guiSystemHeadLineText.setTooltip("Die ASL bietet ein GUI-System, welches zum Erstellen grafischer Oberflächen verwendet werden kann.")
			call this.m_guiSystemHeadLineText.setTooltipSize(12.0)

			//show all kinds of widgets
		endmethod
		
		private method createCharacterSystemWindow takes nothing returns nothing
			set this.m_characterSystemWindow = AMainWindow.create(AGui.playerGui(this.m_user), 0.0, 0.0, 1000.0, 1000.0, true, 0)
		endmethod

		private method createWorldSystemWindow takes nothing returns nothing
		endmethod
	
		private method createGui takes nothing returns nothing
			set this.m_mainWindow = AMainWindow.create(AGui.playerGui(this.m_user), 0.0, 0.0, 1000.0, 1000.0, true, -1)
		
			set this.m_headLineText = AText.create(this.m_mainWindow, 400.0, 200.0, 0.0, 0.0, onHitActionShowInfo, onTrackActionShowTooltip)
			call this.m_headLineText.setTextAndSize("Advanced Script Library", 15.0)
			call this.m_headLineText.setTooltip("Advanced Script Library.")
			call this.m_headLineText.setTooltipSize(12.0)
			
			set this.m_guiSystemsButton = AButton.create(this.m_mainWindow, 50.0, 50.0, 20.0, 20.0, onHitActionShowGuiSystemMainWindow, onTrackActionShowTooltip)
			call this.m_guiSystemsButton.setShortcut('g')
			call this.m_guiSystemsButton.setTooltip("Drücken Sie hier, um das GUI-System der ASL zu testen")
			call this.m_guiSystemsButton.setTooltipSize(12.0)
			
			set this.m_characterSystemsButton = AButton.create(this.m_mainWindow, 50.0, 50.0, 20.0, 20.0, onHitActionShowCharacterSystemMainWindow, onTrackActionShowTooltip)
			call this.m_characterSystemsButton.setShortcut('c')
			call this.m_characterSystemsButton.setTooltip("Drücken Sie hier, um das Charaktersystem der ASL zu testen.")
			call this.m_characterSystemsButton.setTooltipSize(12.0)
			
			set this.m_worldSystemsButton = AButton.create(this.m_mainWindow, 100.0, 50.0, 20.0, 20.0, onHitActionShowWorldSystemMainWindow, onTrackActionShowTooltip)
			call this.m_worldSystemsButton.setShortcut('w')
			call this.m_worldSystemsButton.setTooltip("Drücken Sie hier, um das Weltsystem der ASL zu testen.")
			call this.m_worldSystemsButton.setTooltipSize(12.0)

			call this.createGuiSystemWindow()
			call this.createCharacterSystemWindow()
			call this.createWorldSystemWindow()
		endmethod
		
		private static method triggerActionShow takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype systemDebugger = AHashTable.global().handleInteger(triggeringTrigger, "this")
			call systemDebugger.showMenu()
			set triggeringTrigger = null
		endmethod
		
		private method createMenuTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_menuTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterKeyEventForPlayer(this.m_user, this.m_menuTrigger, thistype.menuKey, true)
			set triggerAction = TriggerAddAction(this.m_menuTrigger, function thistype.triggerActionShow)
			call AHashTable.global().setHandleInteger(this.m_menuTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		/// @param user The debugging player.
		public static method create takes player user returns thistype
			local thistype this = thistype.allocate()
			//start members
			set this.m_user = user
			
			call this.createMenuTrigger()
			call this.createGui()
			return this
		endmethod
		
		private method destroyMenuTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_menuTrigger)
			set this.m_menuTrigger = null
		endmethod
		
		public method onDestroy takes nothing returns nothing
		
			call this.destroyMenuTrigger()
			//main window is destroyed when gui is destroyed
		endmethod
		
		/// @param menuKey The key which should be pressed by the player for showing the debugger's GUI.
		public static method init takes integer menuKey returns nothing
			local integer i
			local player user
			//static start members
			set thistype.menuKey = menuKey
			//static members
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then
					set thistype.m_playerSystemDebugger[i] = 0
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod
		
		public static method playerSystemDebugger takes integer playerId returns thistype
			local player user
			if (thistype.m_playerSystemDebugger[playerId] == 0) then
				set user = Player(playerId)
				set thistype.m_playerSystemDebugger[playerId] = thistype.create(user)
				set user = null
			endif
			return thistype.m_playerSystemDebugger[playerId]
		endmethod
	endstruct

endlibrary