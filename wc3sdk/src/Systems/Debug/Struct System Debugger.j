library AStructSystemsDebugSystemDebugger requires ALibraryCoreDebugMisc, ASystemsCharacter, ASystemsGui, ASystemsWorld

	/// @todo Should be a method of @struct ASystemDebugger, vJass bug.
	private function onHitActionShowInfo takes AWidget usedWidget returns nothing
		call usedWidget.getGui().showInfoDialog("Advanced Script Library")
	endfunction

	/// @todo Should be a method of @struct ASystemDebugger, vJass bug.
	private function onHitActionShowGuiSystemMainWindow takes AWidget usedWidget returns nothing
		call usedWidget.getGui().showMainWindowByIndex(0)
	endfunction

	/// @todo Should be a method of @struct ASystemDebugger, vJass bug.
	private function onHitActionShowCharacterSystemMainWindow takes AWidget usedWidget returns nothing
		call usedWidget.getGui().showMainWindowByIndex(1)
	endfunction

	/// @todo Should be a method of @struct ASystemDebugger, vJass bug.
	private function onHitActionShowWorldSystemMainWindow takes AWidget usedWidget returns nothing
		call usedWidget.getGui().showMainWindowByIndex(2)
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
		//start members
		private player user
		private trigger menuTrigger
		//members
		private AMainWindow mainWindow
		private AText headLineText
		private AButton guiSystemsButton
		private AButton characterSystemsButton
		private AButton worldSystemsButton

		private AMainWindow guiSystemWindow
		private AText guiSystemHeadLineText
		
		private AMainWindow characterSystemWindow
		private AText characterSystemHeadLineText
		private AButton initCharacterSystemButton
		private AButton createCharactersButton
		private ACheckBox removeUnitOnDestructionCheckBox
		private ACheckBox destroyOnPlayerLeavesCheckBox
		private ACheckBox destroyOnDeathCheckBox
		private ACheckBox useViewSystemCheckBox
		private ACheckBox useFocusSystemCheckBox
		private ACheckBox useMovementSystemCheckBox
		private ACheckBox useFightSystemCheckBox
		private ACheckBox useRevivalSystemCheckBox
		private ACheckBox useInventorySystemCheckBox
		
		private AMainWindow worldSystemWindow
		private AText worldSystemHeadLineText
		
		private method show takes nothing returns nothing
			if (this.mainWindow.isShown()) then
				call this.mainWindow.hide()
			else
				call this.mainWindow.show()
			endif
		endmethod

		private method createGuiSystemWindow takes nothing returns nothing
			set this.guiSystemWindow = AMainWindow.create(AGui.getPlayerGui(this.user), 0.0, 0.0, 1000.0, 1000.0, true, 0)
		
			set this.guiSystemHeadLineText = AText.create(this.mainWindow, 400.0, 200.0, 0.0, 0.0, 0, onTrackActionShowTooltip)
			call this.guiSystemHeadLineText.setTextAndSize("GUI-System", 15.0)
			call this.guiSystemHeadLineText.setTooltip("Die ASL bietet ein GUI-System, welches zum Erstellen grafischer Oberflächen verwendet werden kann.")
			call this.guiSystemHeadLineText.setTooltipSize(12.0)

			//show all kinds of widgets
		endmethod
		
		private method createCharacterSystemWindow takes nothing returns nothing
			set this.characterSystemWindow = AMainWindow.create(AGui.getPlayerGui(this.user), 0.0, 0.0, 1000.0, 1000.0, true, 0)

			set this.initCharacterSystemButton = AButton.create(this.characterSystemWindow, 0.0, 0.0, 80.0, 80.0, onHitActionInitCharacterSystem, onTrackActionShowTooltip)
			call this.initCharacterSystemButton.setTooltip("Initialisiert das Charaktersystem. Diese Aktion kann nur einmal im Spiel durchgeführt werden.")
			call this.initCharacterSystemButton.setTooltipSize(12.0)

			//private AButton createCharactersButton
			//private ACheckBox removeUnitOnDestructionCheckBox
			//private ACheckBox destroyOnPlayerLeavesCheckBox
			//private ACheckBox destroyOnDeathCheckBox
			//private ACheckBox useViewSystemCheckBox
			//private ACheckBox useFocusSystemCheckBox
			//private ACheckBox useMovementSystemCheckBox
			//private ACheckBox useFightSystemCheckBox
			//private ACheckBox useRevivalSystemCheckBox
			//private ACheckBox useInventorySystemCheckBox
		endmethod

		private method createWorldSystemWindow takes nothing returns nothing
		endmethod
	
		private method createGui takes nothing returns nothing
			set this.mainWindow = AMainWindow.create(AGui.getPlayerGui(this.user), 0.0, 0.0, 1000.0, 1000.0, true, KEY_ESCAPE)
		
			set this.headLineText = AText.create(this.mainWindow, 400.0, 200.0, 0.0, 0.0, onHitActionShowInfo, onTrackActionShowTooltip)
			call this.headLineText.setTextAndSize("Advanced Script Library", 15.0)
			call this.headLineText.setTooltip("Advanced Script Library.")
			call this.headLineText.setTooltipSize(12.0)
			
			set this.guiSystemsButton = AButton.create(this.mainWindow, 50.0, 50.0, 20.0, 20.0, onHitActionShowGuiSystemMainWindow, onTrackActionShowTooltip)
			call this.guiSystemsButton.setShortcut('g')
			call this.guiSystemsButton.setTooltip("Drücken Sie hier, um das GUI-System der ASL zu testen")
			call this.guiSystemsButton.setTooltipSize(12.0)
			
			set this.characterSystemsButton = AButton.create(this.mainWindow, 50.0, 50.0, 20.0, 20.0, onHitActionShowCharacterSystemMainWindow, onTrackActionShowTooltip)
			call this.characterSystemsButton.setShortcut('c')
			call this.characterSystemsButton.setTooltip("Drücken Sie hier, um das Charaktersystem der ASL zu testen.")
			call this.characterSystemsButton.setTooltipSize(12.0)
			
			set this.worldSystemsButton = AButton.create(this.mainWindow, 100.0, 50.0, 20.0, 20.0, onHitActionShowWorldSystemMainWindow, onTrackActionShowTooltip)
			call this.worldSystemsButton.setShortcut('w')
			call this.worldSystemsButton.setTooltip("Drücken Sie hier, um das Weltsystem der ASL zu testen.")
			call this.worldSystemsButton.setTooltipSize(12.0)

			call this.createGuiSystemWindow()
			call this.createCharacterSystemWindow()
			call this.createWorldSystemWindow()
		endmethod
		
		private static method triggerActionShow takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ASystemDebugger systemDebugger = AGetDebugHashTable().getHandleInteger(triggeringTrigger, "this")
			call systemDebugger.show()
			set triggeringTrigger = null
		endmethod
		
		private method createMenuTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.menuTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterKeyEventForPlayer(this.user, this.menuTrigger, ASystemDebugger.menuKey, true) //ALibraryInterfaceMisc
			set triggerAction = TriggerAddAction(this.menuTrigger, function ASystemDebugger.triggerActionShow)
			call AGetDebugHashTable().storeHandleInteger(this.menuTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		/// @param user The debugging player.
		public static method create takes player user returns ASystemDebugger
			local ASystemDebugger this = ASystemDebugger.allocate()
			//start members
			set this.user = user
			
			call this.createGui()
			call this.createMenuTrigger()
			return this
		endmethod
		
		private method destroyMenuTrigger takes nothing returns nothing
			call AGetDebugHashTable().destroyTrigger(this.menuTrigger)
			set this.menuTrigger = null
		endmethod
		
		public method onDestroy takes nothing returns nothing
		
			call this.destroyMenuTrigger()
		endmethod
		
		/// @param menuKey The key which should be pressed by the player for showing the debugger's GUI.
		public static method init takes integer menuKey returns nothing
			//static start members
			set ASystemDebugger.menuKey = menuKey
		endmethod
	endstruct

endlibrary