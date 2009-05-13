library AStructSystemsGuiGui requires ALibraryCoreDebugMisc, AStructCoreGeneralContainer, ALibraryCoreGeneralPlayer, ALibraryCoreStringConversion, ALibraryCoreInterfaceCamera, ALibraryCoreInterfaceMisc, AStructCoreInterfaceInterfaceHashTable

	/// @todo AGui.maxMainWindows should be the max size value
	//! runtextmacro A_CONTAINER("AMainWindow", "MainWindow", "4")

	/// @todo Should be a static member of @struct AGui, vJass bug.
	/// This is the generic shortcut function interface.
	/// @param id Id of the object which belongs to the function call.
	function interface AGuiOnPressShortcutAction takes integer id returns nothing

	/// @todo Should be a static member of @struct AGui, vJass bug.
	/// If you want to use an explicit gui action use this.
	function interface AGuiOnPressGuiShortcutAction takes AGui gui returns nothing

	/// @todo Should be a static member of @struct AGui, vJass bug.
	/// Represents the generic function interface for dialog button actions.
	function interface AGuiDialogButtonAction takes AGui gui, integer index returns nothing

	/// @todo Should be a static method of @struct AGui, vJass bug.
	private function AGuiDialogButtonActionNextPage takes AGui gui, integer index returns nothing
		call gui.changeToNextPage(index)
	endfunction

	/// @todo Should be a static method of @struct AGui, vJass bug.
	private function AGuiDialogButtonActionPreviousPage takes AGui gui, integer index returns nothing
		call gui.changeToPreviousPage(index)
	endfunction

	/// @todo Should be a static method of @struct AGui, vJass bug.
	private function unaryFunctionDestroyMainWindow takes AMainWindow element returns nothing
		//saver
		if (element != 0) then
			call element.destroy()
		endif
	endfunction

	/// AGui represents the graphical user interface which can be used by every player.
	/// Each player can have exactly one GUI.
	/// A GUI contains main windows which are user-defined areas on the map. That's necessary because
	/// trackables, textes and images are map-placed objects.
	/// When you destroy an instance of AGui all docked main windows will be destroy automatically.
	struct AGui
		//static constant members
		public static constant integer shortcutArrowUpDown = 0 //these are the special shortcuts
		public static constant integer shortcutArrowDownDown = 1
		public static constant integer shortcutArrowLeftDown = 2
		public static constant integer shortcutArrowRightDown = 3
		public static constant integer shortcutArrowUpUp = 4
		public static constant integer shortcutArrowDownUp = 5
		public static constant integer shortcutArrowLeftUp = 6
		public static constant integer shortcutArrowRightUp = 7
		public static constant integer shortcutEscape = 8 //last value has to be lesser than 'a'
		private static constant integer maxSpecialShortcuts = 9
		private static constant integer maxMainWindows = 4 //public see above in the text macro
		private static constant integer maxShortcuts = 91 //'Z' + 1
		private static constant integer maxDialogButtons = 100
		//static start members
		private static integer shortcutHandlerUnitType
		private static real shortcutHandlerX
		private static real shortcutHandlerY
		private static string textNextPage
		private static integer shortcutNextPage
		private static string textPreviousPage
		private static integer shortcutPreviousPage
		private static string textOk
		private static integer shortcutOk
		//static dynamic members
		private static integer array shortcutAbility[AGui.maxShortcuts]
		//static members
		private static AGui array playerGui //[bj_MAX_PLAYERS] /// @todo vJass bug
		//dynamic members
		private AGuiOnPressShortcutAction array onPressShortcutAction[AGui.maxShortcuts]
		private integer array onPressShortcutActionId[AGui.maxShortcuts]
		//start members
		private player user
		//members
		private AMainWindowContainer container
		private AMainWindow shownMainWindow
		private trigger leaveTrigger
		private unit shortcutHandler
		private trigger shortcutHandleTrigger
		private trigger array specialShortcutHandleTrigger[AGui.maxSpecialShortcuts]
		private real playerDataCameraX
		private real playerDataCameraY
		private group playerDataSelection
		private dialog usedDialog
		private integer dialogButtons
		private string array dialogButtonText[AGui.maxDialogButtons]
		private integer array dialogButtonShortcut[AGui.maxDialogButtons]
		private boolean array dialogButtonIsQuitButton[AGui.maxDialogButtons]
		private boolean array dialogButtonDoScoreScreen[AGui.maxDialogButtons]
		private AGuiDialogButtonAction array dialogButtonAction[AGui.maxDialogButtons]
		private button array dialogButton[AGui.maxDialogButtons]
		private trigger array dialogButtonTrigger[AGui.maxDialogButtons]

		//! runtextmacro A_STRUCT_DEBUG("\"AGui\"")

		//dynamic members

		/// Each shortcut (including special shortcuts) can have its own action.
		/// Additionally you can add shortcuts to widgets that they'll get the widgets action automatically.
		public method setOnPressShortcutAction takes integer shortcut, AGuiOnPressShortcutAction onPressShortcutAction, integer id returns nothing
			if ((shortcut >= 0) and (shortcut < AGui.maxSpecialShortcuts)) then
				call this.createSpecialShortcutTrigger(shortcut)
			endif
			set this.onPressShortcutAction[shortcut] = onPressShortcutAction
			set this.onPressShortcutActionId[shortcut] = id
		endmethod

		/// Convenience method.
		/// Assigns a function to a shortcut. The function gets the AGui instance as argument.
		public method setOnPressGuiShortcutAction takes integer shortcut, AGuiOnPressGuiShortcutAction onPressGuiShortcutAction returns nothing
			call this.setOnPressShortcutAction(shortcut, onPressGuiShortcutAction, this)
		endmethod

		//start members

		public method getUser takes nothing returns player
			return this.user
		endmethod

		//methods

		/// @todo desynchronisation?
		/// Saves GUI users camera data and selection in game.
		public method savePlayerData takes nothing returns nothing
			set this.playerDataCameraX = GetCameraTargetPositionXOfPlayer(this.user) //ALibraryInterfaceCamera
			set this.playerDataCameraY = GetCameraTargetPositionYOfPlayer(this.user) //ALibraryInterfaceCamera
			set this.playerDataSelection = GetUnitsSelectedAll(this.user)
		endmethod

		/// Loads GUI users camera data and selection from time before he has enabled the GUI.
		public method loadPlayerData takes nothing returns nothing
			call PanCameraToForPlayer(this.user, this.playerDataCameraX, this.playerDataCameraY)
			call SelectGroupForPlayerBJ(this.playerDataSelection, this.user)
			call DestroyGroup(this.playerDataSelection)
			set this.playerDataSelection = null
		endmethod

		/// If you dock a main window it will be destroyed when the GUI will be destroyed.
		/// Friend relation to AMainWindow. In general you do not need to use this method.
		/// @return Container index.
		public method dockMainWindow takes AMainWindow mainWindow returns integer
			local integer i = 0
			loop
				exitwhen (i == AGui.maxMainWindows)
				if (this.container[i] == 0) then
					set this.container[i] = mainWindow
					return i
				endif
				set i = i + 1
			endloop
			debug call this.print("Could not dock main window " + I2S(mainWindow) + ". Maximum reached.")
			return -1
		endmethod

		/// Undocks a main window from GUI. If a main window is undocked there won't be any relationships between it and the GUI anymore.
		public method undockMainWindow takes AMainWindow mainWindow returns nothing
			local integer i = 0
			loop
				exitwhen (i == AGui.maxMainWindows)
				if (this.container[i] == mainWindow) then
					set this.container[i] = 0
					return
				endif
				set i = i + 1
			endloop
			debug call this.print("Could not undock main window " + I2S(mainWindow) + ". Main window was not found.")
		endmethod

		public method showMainWindowByIndex takes integer index returns nothing
			call this.container[index].show()
		endmethod

		/// @todo Friend relation to @struct AMainWindow, do not use!
		public method hideShownMainWindowAndSetNew takes AMainWindow mainWindow returns nothing
			if (this.shownMainWindow != 0) then
				call this.shownMainWindow.hide()
			endif
			set this.shownMainWindow = mainWindow
		endmethod

		public method enableShortcuts takes nothing returns nothing
			local integer i
			call PauseUnit(this.shortcutHandler, false)
			call SelectUnitForPlayerSingle(this.shortcutHandler, this.user)
			call EnableTrigger(this.shortcutHandleTrigger)
			call this.enableSpecialShortcutTriggers()
			set i = 0
			loop
				exitwhen (i == AGui.maxShortcuts)
				if (this.onPressShortcutActionId[i] != 0) then
					call UnitAddAbility(this.shortcutHandler, AGui.shortcutAbility[i])
				endif
				set i = i + 1
			endloop
		endmethod

		/// Resets all shortcut actions!
		public method disableShortcuts takes nothing returns nothing
			local integer i
			call PauseUnit(this.shortcutHandler, true)
			call SelectUnitRemoveForPlayer(this.shortcutHandler, this.user)
			call DisableTrigger(this.shortcutHandleTrigger)
			call this.disableSpecialShortcutTriggers()
			
			set i = 0
			loop
				exitwhen (i == AGui.maxShortcuts)
				if (this.onPressShortcutActionId[i] != 0) then
					call UnitRemoveAbility(this.shortcutHandler, AGui.shortcutAbility[i])
				endif
				set this.onPressShortcutActionId[i] = 0
				set i = i + 1
			endloop
		endmethod

		public method clearDialog takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.dialogButtons)
				set this.dialogButton[i] = null
				set i = i + 1
			endloop
			if (this.dialogButtons > 0) then
				call DialogClear(this.usedDialog)
				set this.dialogButtons = 0
			endif
		endmethod

		/// @todo Check the maxLineLength argument. InsertLineBreaks is bugged.
		public method setDialogTitle takes string title returns nothing
			call DialogSetMessage(this.usedDialog, title) //InsertLineBreaks(title, 10) //ALibraryStringConversion
		endmethod

		public method showDialog takes nothing returns nothing
			call DialogDisplay(this.user, this.usedDialog, true)
		endmethod

		public method hideDialog takes nothing returns nothing
			call DialogDisplay(this.user, this.usedDialog, false)
		endmethod

		public method addExtendedDialogButton takes string text, integer shortcut, boolean isQuitButton, boolean doScoreScreen, AGuiDialogButtonAction dialogButtonAction returns integer
			local integer id = this.dialogButtons
			local boolean nextPage = false
			local integer i = AMaxDialogButtons - 1
			debug if (id >= AGui.maxDialogButtons) then
				debug call this.print("Dialog button maximum has been reached.")
				debug return -1
			debug endif
			loop
				exitwhen (i >= AGui.maxDialogButtons)
				if (id == i) then
					if (id + 1 < AGui.maxDialogButtons and id + 2 < AGui.maxDialogButtons) then
						set nextPage = true
						set this.dialogButtonText[id] = AGui.textNextPage
						set this.dialogButtonShortcut[id] = AGui.shortcutNextPage
						set this.dialogButtonIsQuitButton[id] = false
						set this.dialogButtonDoScoreScreen[id] = false
						set this.dialogButtonAction[id] = AGuiDialogButtonActionNextPage
						call this.addDialogButtonById(id)
						set this.dialogButtons = id + 1
						set id = this.dialogButtons
						set this.dialogButtonText[id] = AGui.textPreviousPage
						set this.dialogButtonShortcut[id] = AGui.shortcutPreviousPage
						set this.dialogButtonIsQuitButton[id] = false
						set this.dialogButtonDoScoreScreen[id] = false
						set this.dialogButtonAction[id] = AGuiDialogButtonActionPreviousPage
						call this.addDialogButtonById(id)
						set this.dialogButtons = id + 1
						set id = this.dialogButtons
						exitwhen (true)
					else
						return -1
					endif
				endif
				set i = i + AMaxDialogButtons
			endloop
			
			set this.dialogButtonText[id] = text
			set this.dialogButtonShortcut[id] = shortcut
			set this.dialogButtonIsQuitButton[id] = isQuitButton
			set this.dialogButtonDoScoreScreen[id] = doScoreScreen
			set this.dialogButtonAction[id] = dialogButtonAction
			
			if (not nextPage) then
				call this.addDialogButtonById(id)
			endif
			
			set this.dialogButtons = id + 1
			
			return id
		endmethod
		
		public method addDialogButton takes string text, integer shortcut,  AGuiDialogButtonAction dialogButtonAction returns integer
			return this.addExtendedDialogButton(text, shortcut, false, false, dialogButtonAction)
		endmethod
		
		/// Is just public since static methods can not be used as function interface functions.
		/// Usually you don't need this method.
		public method changeToNextPage takes integer fromIndex returns nothing
			local integer exitValue = fromIndex + AMaxDialogButtons
			local integer i = fromIndex
			call this.clearDialog() //hide?
			loop
				exitwhen (i == this.dialogButtons or i == exitValue)
				call this.addDialogButtonById(i)
				set i = i + 1
			endloop
		endmethod

		/// Is just public since static methods can not be used as function interface functions.
		/// Usually you don't need this method.
		public method changeToPreviousPage takes integer fromIndex returns nothing
			local integer exitValue = fromIndex - 1
			local integer i = fromIndex - AMaxDialogButtons
			call this.clearDialog() //hide?
			loop
				exitwhen (i == exitValue)
				call this.addDialogButtonById(i)
				set i = i + 1
			endloop
		endmethod

		/// Convenience method.
		/// Shows a simple dialog with an OK button.
		/// @param text Displayed text.
		public method showInfoDialog takes string text returns nothing
			call this.clearDialog()
			call this.setDialogTitle(text)
			call this.addDialogButton(AGui.textOk, AGui.shortcutOk, 0)
			call this.showDialog()
		endmethod
		
		private method addDialogButtonById takes integer id returns nothing
			if (not this.dialogButtonIsQuitButton[id]) then
				set this.dialogButton[id] = DialogAddButton(this.usedDialog, HighlightShortcut(this.dialogButtonText[id], this.dialogButtonShortcut[id], "ffffcc00"), this.dialogButtonShortcut[id])
			else
				 set this.dialogButton[id] = DialogAddQuitButton(this.usedDialog, this.dialogButtonDoScoreScreen[id], HighlightShortcut(this.dialogButtonText[id], this.dialogButtonShortcut[id], "ffffcc00"), this.dialogButtonShortcut[id])
			endif
			call this.createDialogButtonTrigger(id)
		endmethod

		private method enableSpecialShortcutTriggers takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == AGui.maxSpecialShortcuts)
					if (this.specialShortcutHandleTrigger[i] != null) then
						call EnableTrigger(this.specialShortcutHandleTrigger[i])
					endif
				set i = i + 1
			endloop
		endmethod

		private method disableSpecialShortcutTriggers takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == AGui.maxSpecialShortcuts)
					if (this.specialShortcutHandleTrigger[i] != null) then
						call DisableTrigger(this.specialShortcutHandleTrigger[i])
					endif
				set i = i + 1
			endloop
		endmethod

		private static method triggerActionPlayerLeaves takes nothing returns nothing
			local player triggerPlayer = GetTriggerPlayer()
			call AGui.playerGui[GetPlayerId(triggerPlayer)].destroy()
			set triggerPlayer = null
		endmethod

		private method createLeaveTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.leaveTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterPlayerEvent(this.leaveTrigger, this.user, EVENT_PLAYER_LEAVE)
			set triggerAction = TriggerAddAction(this.leaveTrigger, function AGui.triggerActionPlayerLeaves)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		private method createShortcutHandler takes nothing returns nothing
			set this.shortcutHandler = CreateUnit(this.user, AGui.shortcutHandlerUnitType, AGui.shortcutHandlerX, AGui.shortcutHandlerY, 0.0)
			call SetUnitInvulnerable(this.shortcutHandler, true)
		endmethod

		private static method triggerActionOnPressShortcut takes nothing returns nothing
			local integer i
			local integer abilityId  = GetSpellAbilityId()
			local trigger triggeringTrigger
			local AGui this
			debug call Print("SHORTCUT TRIGGER with Ability " + I2S(abilityId))
			set i = 'a'
			loop
				exitwhen (i == AGui.maxShortcuts)
				if (abilityId == AGui.shortcutAbility[i]) then
					debug call Print("EXIT!!!! " + I2S(i))
					set triggeringTrigger = GetTriggeringTrigger()
					set this = AGetInterfaceHashTable().getHandleInteger(triggeringTrigger, "this")
					debug if (this.onPressShortcutAction[i] != 0) then
						debug call Print("Action exists")
					debug endif
					call this.onPressShortcutAction[i].execute(this.onPressShortcutActionId[i])
					set triggeringTrigger = null
					exitwhen (true)
				endif
				set i  = i + 1
			endloop
		endmethod

		private method createShortcutHandleTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.shortcutHandleTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterUnitEvent(this.shortcutHandleTrigger, this.shortcutHandler, EVENT_UNIT_SPELL_CAST)
			set triggerAction = TriggerAddAction(this.shortcutHandleTrigger, function AGui.triggerActionOnPressShortcut)
			call AGetInterfaceHashTable().storeHandleInteger(this.shortcutHandleTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		private static method triggerActionOnPressSpecialShortcut takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AGui this = AGetInterfaceHashTable().getHandleInteger(triggeringTrigger, "this")
			local integer shortcut = AGetInterfaceHashTable().getHandleInteger(triggeringTrigger, "shortcut")
			debug call Print("SHORTCUT " + I2S(shortcut))
			call this.onPressShortcutAction[shortcut].execute(this)
			set triggeringTrigger = null
		endmethod

		/// This method won't be called in the constructor.
		/// It is a dynamic creation.
		private method createSpecialShortcutTrigger takes integer shortcut returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			//don't check if the action is 0, it can be changed dynamicly!
			//the trigger can't be changed dynamicly
			if (this.specialShortcutHandleTrigger[shortcut] == null) then
				set this.specialShortcutHandleTrigger[shortcut] = CreateTrigger()
				if (shortcut == AGui.shortcutArrowUpDown) then
					set triggerEvent = TriggerRegisterPlayerEvent(this.specialShortcutHandleTrigger[shortcut], this.user, EVENT_PLAYER_ARROW_UP_DOWN)
				elseif (shortcut == AGui.shortcutArrowDownDown) then
					set triggerEvent = TriggerRegisterPlayerEvent(this.specialShortcutHandleTrigger[shortcut], this.user, EVENT_PLAYER_ARROW_DOWN_DOWN)
				elseif (shortcut == AGui.shortcutArrowLeftDown) then
					set triggerEvent = TriggerRegisterPlayerEvent(this.specialShortcutHandleTrigger[shortcut], this.user, EVENT_PLAYER_ARROW_LEFT_DOWN)
				elseif (shortcut == AGui.shortcutArrowRightDown) then
					set triggerEvent = TriggerRegisterPlayerEvent(this.specialShortcutHandleTrigger[shortcut], this.user, EVENT_PLAYER_ARROW_RIGHT_DOWN)
				elseif (shortcut == AGui.shortcutArrowUpUp) then
					set triggerEvent = TriggerRegisterPlayerEvent(this.specialShortcutHandleTrigger[shortcut], this.user, EVENT_PLAYER_ARROW_UP_UP)
				elseif (shortcut == AGui.shortcutArrowDownUp) then
					set triggerEvent = TriggerRegisterPlayerEvent(this.specialShortcutHandleTrigger[shortcut], this.user, EVENT_PLAYER_ARROW_DOWN_UP)
				elseif (shortcut == AGui.shortcutArrowLeftUp) then
					set triggerEvent = TriggerRegisterPlayerEvent(this.specialShortcutHandleTrigger[shortcut], this.user, EVENT_PLAYER_ARROW_LEFT_UP)
				elseif (shortcut == AGui.shortcutArrowRightUp) then
					set triggerEvent = TriggerRegisterPlayerEvent(this.specialShortcutHandleTrigger[shortcut], this.user, EVENT_PLAYER_ARROW_RIGHT_UP)
				elseif (shortcut == AGui.shortcutEscape) then
					set triggerEvent = TriggerRegisterPlayerEvent(this.specialShortcutHandleTrigger[shortcut], this.user, EVENT_PLAYER_END_CINEMATIC)
				endif
				set triggerAction = TriggerAddAction(this.specialShortcutHandleTrigger[shortcut], function AGui.triggerActionOnPressSpecialShortcut)
				call AGetInterfaceHashTable().storeHandleInteger(this.specialShortcutHandleTrigger[shortcut], "this", this)
				call AGetInterfaceHashTable().storeHandleInteger(this.specialShortcutHandleTrigger[shortcut], "shortcut", shortcut)
				set triggerEvent = null
				set triggerAction = null
			endif
		endmethod

		private static method triggerActionRunDialogButtonAction takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AGui this = AGetInterfaceHashTable().getHandleInteger(triggeringTrigger, "this")
			local integer index = AGetInterfaceHashTable().getHandleInteger(triggeringTrigger, "index")
			call this.dialogButtonAction[index].execute(this, index)
			set triggeringTrigger = null
		endmethod

		private method createDialogButtonTrigger takes integer index returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			if ((this.dialogButtonTrigger == null) and (this.dialogButtonAction[index] != 0)) then
				set this.dialogButtonTrigger[index] = CreateTrigger()
				set triggerEvent = TriggerRegisterDialogButtonEvent(this.dialogButtonTrigger[index], this.dialogButton[index])
				set triggerAction = TriggerAddAction(this.dialogButtonTrigger[index], function AGui.triggerActionRunDialogButtonAction)
				call AGetInterfaceHashTable().storeHandleInteger(this.dialogButtonTrigger[index], "this", this) //AClassInterfaceInterfaceHashTable
				call AGetInterfaceHashTable().storeHandleInteger(this.dialogButtonTrigger[index], "index", index) //AClassInterfaceInterfaceHashTable
				set triggerEvent = null
				set triggerAction = null
			endif
		endmethod

		/// Don't use the constructor.
		/// Use @method getPlayerGui.
		/// Shortcuts will be disabled first.
		private static method create takes player user returns AGui
			local AGui this = AGui.allocate()
			//start members
			set this.user = user
			//members
			set this.container = AMainWindowContainer.create()
			set this.shownMainWindow = 0
			set this.usedDialog = DialogCreate()
			set this.dialogButtons = 0

			call this.createLeaveTrigger()
			call this.createShortcutHandler()
			call this.createShortcutHandleTrigger()
			call this.disableShortcuts()
			return this
		endmethod

		private method destroyDockedMainWindows takes nothing returns nothing
			call this.container.forEach(0, AGui.maxMainWindows, AMainWindowContainerUnaryFunction.unaryFunctionDestroyMainWindow)
		endmethod

		private method destroyDialogButtons takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == AGui.maxDialogButtons)
				set this.dialogButton[i] = null
				set i = i + 1
			endloop
		endmethod

		private method destroyLeaveTrigger takes nothing returns nothing
			call DestroyTrigger(this.leaveTrigger)
			set this.leaveTrigger = null
		endmethod

		private method removeShortcutHandler takes nothing returns nothing
			call RemoveUnit(this.shortcutHandler)
			set this.shortcutHandler = null
		endmethod

		private method destroyShortcutHandleTrigger takes nothing returns nothing
			call AGetInterfaceHashTable().destroyTrigger(this.shortcutHandleTrigger)
			set this.shortcutHandleTrigger = null
		endmethod

		private method destroySpecialShortcutTriggers takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == AGui.maxSpecialShortcuts)
					if (this.specialShortcutHandleTrigger[i] != null) then
						call AGetInterfaceHashTable().destroyTrigger(this.specialShortcutHandleTrigger[i])
						set this.specialShortcutHandleTrigger[i] = null
					endif
				set i = i + 1
			endloop
		endmethod

		private method destroyDialogButtonTriggers takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == AGui.maxDialogButtons)
				if (this.dialogButtonTrigger[i] != null) then
					call AGetInterfaceHashTable().destroyTrigger(this.dialogButtonTrigger[i])
					set this.dialogButtonTrigger[i] = null
				endif
				set i = i + 1
			endloop
		endmethod

		/// Will be destroyed when player leaves the game.
		private method onDestroy takes nothing returns nothing
			//elements
			call this.destroyDockedMainWindows()
			call AMainWindowContainer.destroy(this.container)
			call this.clearDialog()
			call DialogDestroy(this.usedDialog)
			set this.usedDialog = null

			call this.destroyDialogButtons()
			call this.destroyLeaveTrigger()
			call this.removeShortcutHandler()
			call this.destroyShortcutHandleTrigger()
			call this.destroySpecialShortcutTriggers()
			call this.destroyDialogButtonTriggers()
		endmethod

		/// @param shortcutHandlerUnitType The unit type of the unit which is selected during the display time of the GUI. It should have all shortcut abilities.
		/// @param shortcutHandlerX The x coordinate of the shortcut handlers position.
		/// @param shortcutHandlerY The y coordinate of the shortcut handlers position.
		/// @param textOk The text which is displayed as Ok text.
		/// @param shortcutOk The shortcut which is used for the Ok text.
		public static method init takes integer shortcutHandlerUnitType, real shortcutHandlerX, real shortcutHandlerY, string textNextPage, integer shortcutNextPage, string textPreviousPage, integer shortcutPreviousPage, string textOk, integer shortcutOk returns nothing
			local integer i
			//static start members
			set AGui.shortcutHandlerUnitType = shortcutHandlerUnitType
			set AGui.shortcutHandlerX = shortcutHandlerX
			set AGui.shortcutHandlerY = shortcutHandlerY
			set AGui.textNextPage = textNextPage
			set AGui.shortcutNextPage = shortcutNextPage
			set AGui.textPreviousPage = textPreviousPage
			set AGui.shortcutPreviousPage = shortcutPreviousPage
			set AGui.textOk = textOk
			set AGui.shortcutOk = shortcutOk
		endmethod

		//static dynamic members

		public static method setShortcutAbility takes integer shortcutAbility, integer abilityId returns nothing
			set AGui.shortcutAbility[shortcutAbility] = abilityId
		endmethod

		public static method getShortcutAbility takes integer shortcutAbility returns integer
			return AGui.shortcutAbility[shortcutAbility]
		endmethod

		//static methods

		public static method getPlayerGui takes player user returns AGui
			if (AGui.playerGui[GetPlayerId(user)] == 0) then
				debug call Print("Create gui for player " + GetPlayerName(user))
				set AGui.playerGui[GetPlayerId(user)] = AGui.create(user) //If you forget [GetPlayerId(user)] there is a jasshelper error
			endif
			return AGui.playerGui[GetPlayerId(user)]
		endmethod

		//static convenience methods
		
		public static method setOnPressGuiShortcutActionForAll takes integer shortcut, AGuiOnPressGuiShortcutAction onPressGuiShortcutAction returns nothing
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then
					call AGui.getPlayerGui(user).setOnPressGuiShortcutAction(shortcut, onPressGuiShortcutAction)
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod
		
		public static method showInfoDialogToAll takes string text returns nothing
			local integer i
			local player user
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then
					call AGui.getPlayerGui(user).showInfoDialog(text)
				endif
				set user = null
				set i = i + 1
			endloop
		endmethod
	endstruct

endlibrary