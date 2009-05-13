library AStructSystemsGuiMainWindow requires ALibraryCoreDebugMisc, AStructCoreGeneralContainer, AStructCoreInterfaceInterfaceHashTable, ALibraryCoreInterfaceCamera, ALibraryCoreInterfaceCinematic, ALibraryCoreInterfaceTextTag, ALibraryCoreInterfaceMisc, ALibraryCoreEnvironmentSound, AStructSystemsGuiWidget

	/// @todo Should be a static function interface of @struct AMainWindow, vJass bug.
	function interface AMainWindowOnShowCondition takes AMainWindow mainWindow returns boolean
	
	/// @todo Should be a static function interface of @struct AMainWindow, vJass bug.
	function interface AMainWindowOnShowAction takes AMainWindow mainWindow returns nothing
	
	/// @todo Should be a static function interface of @struct AMainWindow, vJass bug.
	function interface AMainWindowOnHideCondition takes AMainWindow mainWindow returns boolean

	/// @todo Should be a static function interface of @struct AMainWindow, vJass bug.
	function interface AMainWindowOnHideAction takes AMainWindow mainWindow returns nothing
	
	/// @todo Use @member AMainWindow.maxWidgets instead of 100.
	//! runtextmacro A_CONTAINER("AWidget", "Widget", "100")

	/// @todo Should be a static method of @struct AMainWindow, vJass bug.
	private function unaryFunctionShowWidget takes AWidget element returns nothing
		call element.show()
	endfunction

	/// @todo Should be a static method of @struct AMainWindow, vJass bug.
	private function unaryFunctionHideWidget takes AWidget element returns nothing
		call element.hide()
	endfunction

	/// @todo Should be a static method of @struct AMainWindow, vJass bug.
	private function unaryFunctionDestroyWidget takes AWidget element returns nothing
		//saver
		if (element != 0) then
			call element.destroy()
		endif
	endfunction

	struct AMainWindow
		//static constant members
		private static constant integer maxWidgets = 100 //public, see above (text macro)
		//static start members
		private static camerasetup cameraSetup
		private static string tooltipSoundPath
		//dynamic members
		private AMainWindowOnShowCondition m_onShowCondition
		private AMainWindowOnShowAction m_onShowAction
		private AMainWindowOnHideCondition m_onHideCondition
		private AMainWindowOnHideAction m_onHideAction
		private real m_tooltipX
		private real m_tooltipY
		//start members
		private AGui m_gui
		private real m_x
		private real m_y
		private real m_sizeX
		private real m_sizeY
		private boolean m_useShortcuts
		private integer m_shortcut
		//members
		private integer m_guiIndex
		private boolean m_isShown
		private AWidgetContainer container
		private texttag tooltip
		private trigger shortcutTrigger

		//! runtextmacro A_STRUCT_DEBUG("\"AMainWindow\"")
		
		//dynamic members
		
		/// The @param onShowCondition will be checked before the main window should be displayed.
		/// If it returns false the main window won't be displayed.
		public method setOnShowCondition takes AMainWindowOnShowAction onShowCondition returns nothing
			set this.m_onShowCondition = onShowCondition
		endmethod
		
		public method onShowCondition takes nothing returns AMainWindowOnShowCondition
			return this.m_onShowCondition
		endmethod
		
		public method setOnShowAction takes AMainWindowOnShowAction onShowAction returns nothing
			set this.m_onShowAction = onShowAction
		endmethod
		
		public method onShowAction takes nothing returns AMainWindowOnShowAction
			return this.m_onShowAction
		endmethod
		
		public method setOnHideCondition takes AMainWindowOnHideCondition onHideCondition returns nothing
			set this.m_onHideCondition = onHideCondition
		endmethod
		
		public method onHideCondition takes nothing returns AMainWindowOnHideCondition
			return this.m_onHideCondition
		endmethod
		
		public method setOnHideAction takes AMainWindowOnHideAction onHideAction returns nothing
			set this.m_onHideAction = onHideAction
		endmethod
		
		public method onHideAction takes nothing returns AMainWindowOnHideAction
			return this.m_onHideAction
		endmethod
		
		public method setTooltipX takes real tooltipX returns nothing
			set this.m_tooltipX = tooltipX
		endmethod
		
		public method tooltipX takes nothing returns real
			return this.m_tooltipX
		endmethod
		
		public method setTooltipY takes real tooltipY returns nothing
			set this.m_tooltipY = tooltipY
		endmethod
		
		public method tooltipY takes nothing returns real
			return this.m_tooltipY
		endmethod

		//start members

		public method gui takes nothing returns AGui
			return this.m_gui
		endmethod
		
		public method x takes nothing returns real
			return this.m_x
		endmethod
		
		public method y takes nothing returns real
			return this.m_y
		endmethod

		public method sizeX takes nothing returns real
			return this.m_sizeX
		endmethod

		public method sizeY takes nothing returns real
			return this.m_sizeY
		endmethod

		//members
		
		public method guiIndex takes nothing returns integer
			return this.m_guiIndex
		endmethod

		public method isShown takes nothing returns boolean
			return this.m_isShown
		endmethod
		
		//convenience methods
		
		public method user takes nothing returns player
			return this.m_gui.getUser()
		endmethod

		//methods
		
		public method getX takes real x returns real
			return (this.m_x + x)
		endmethod

		public method getY takes real y returns real
			return (this.m_y + y)
		endmethod
		
		public method enableShortcut takes nothing returns nothing
			debug if (this.m_shortcut == -1) then
				debug call this.print("Main window does not use a shortcut.")
			debug endif
			call EnableTrigger(this.shortcutTrigger)
		endmethod
		
		public method disableShortcut takes nothing returns nothing
			debug if (this.m_shortcut == -1) then
				debug call this.print("Main window does not use a shortcut.")
			debug endif
			call DisableTrigger(this.shortcutTrigger)
		endmethod

		public method showTooltip takes AWidget usedWidget returns nothing
			if (this.tooltip == null) then
				set this.tooltip = CreateTextTag()
			endif
			call SetTextTagTextBJ(this.tooltip, usedWidget.getTooltip(), usedWidget.getTooltipSize())
			call SetTextTagPos(this.tooltip, this.getX(this.m_tooltipX), this.getY(this.m_tooltipY), 0.0)
			call ShowTextTagForPlayer(this.user(), this.tooltip, true)
			if (AMainWindow.tooltipSoundPath != null) then
				call PlaySoundPathForPlayer(this.user(), AMainWindow.tooltipSoundPath)
			endif
		endmethod

		public method hideTooltip takes nothing returns nothing
			call ShowTextTagForPlayer(this.user(), this.tooltip, false)
		endmethod

		public method dockWidget takes AWidget usedWidget returns integer
			local integer i = 0
			loop
				exitwhen (i == AMainWindow.maxWidgets)
				if (this.container[i] == 0) then
					set this.container[i] = usedWidget
					return i
				endif
				set i = i + 1
			endloop
			debug call this.print("Could not dock widget. Maximum reached.")
			return -1
		endmethod

		public method undockWidget takes AWidget usedWidget returns nothing
			local integer i = 0
			loop
				exitwhen (i == AMainWindow.maxWidgets)
				if (this.container[i] == usedWidget) then
					set this.container[i] = 0
					return
				endif
				set i = i + 1
			endloop
			debug call this.print("Could not undock widget. Widget was not found.")
		endmethod

		public method show takes nothing returns nothing
			local real x
			local real y
			if (this.m_onShowCondition != 0 and not this.m_onShowCondition.evaluate(this)) then
				debug call this.print("Condition is false.")
				return
			endif
			set x = this.m_x + (this.m_sizeX / 2.0)
			set y = this.m_y + (this.m_sizeY / 2.0)
			call this.m_gui.savePlayerData()
			call ClearScreenMessagesForPlayer(this.user()) //ALibraryInterfaceCinematic
			call PanCameraToForPlayer(this.user(), x, y)
			call SetCameraBoundsToPointForPlayer(this.user(), x, y) //ALibraryInterfaceCamera
			call CameraSetupApplyForPlayer(true, AMainWindow.cameraSetup, this.user(), 0.0)
			//widgets
			call this.container.forEach(0, AMainWindow.maxWidgets, AWidgetContainerUnaryFunction.unaryFunctionShowWidget)

			if (this.m_useShortcuts) then
				call this.m_gui.enableShortcuts()
			endif
			set this.m_isShown = true
			call this.m_gui.hideShownMainWindowAndSetNew(this)
			if (this.m_onShowAction != 0) then
				call this.m_onShowAction.execute(this)
			endif
		endmethod

		public method hide takes nothing returns nothing
			if (this.m_onHideCondition != 0 and not this.m_onHideCondition.evaluate(this)) then
				debug call this.print("Condition is false.")
				return
			endif
			
			call ResetCameraBoundsToMapRectForPlayer(this.user())
			call ResetToGameCameraForPlayer(this.user(), 0.0)
			call this.m_gui.loadPlayerData()
			call this.hideTooltip()
			//widgets
			call this.container.forEach(0, AMainWindow.maxWidgets, AWidgetContainerUnaryFunction.unaryFunctionHideWidget)

			if (this.m_useShortcuts) then
				call this.m_gui.disableShortcuts()
			endif
			set this.m_isShown = false
			
			if (this.m_onHideAction != 0) then
				call this.m_onHideAction.execute(this)
			endif
		endmethod
		
		private static method triggerActionPressShortcut takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AMainWindow this = AGetInterfaceHashTable().getHandleInteger(triggeringTrigger, "this")
			debug call this.print("Press shortcut")
			if (not this.m_isShown) then
				call this.show()
			else
				call this.hide()
			endif
			set triggeringTrigger = null
		endmethod
		
		private method createShortcutTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.shortcutTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterKeyEventForPlayer(this.user(), this.shortcutTrigger, this.m_shortcut, true)
			set triggerAction = TriggerAddAction(this.shortcutTrigger, function AMainWindow.triggerActionPressShortcut)
			call AGetInterfaceHashTable().storeHandleInteger(this.shortcutTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		/// @param shortcut If this value is -1 main window won't have any shortcut.
		public static method create takes AGui gui, real x, real y, real sizeX, real sizeY, boolean useShortcuts, integer shortcut returns AMainWindow
			local AMainWindow this = AMainWindow.allocate()
			//dynamic members
			set this.m_onShowCondition  = 0
			set this.m_onShowAction = 0
			set this.m_onHideCondition = 0
			set this.m_onHideAction = 0
			set this.m_tooltipX = 0.0
			set this.m_tooltipY = 0.0
			//start members
			set this.m_gui = gui
			set this.m_x = x //insert a debug if the coordinates are out of map range
			set this.m_y = y
			set this.m_sizeX = sizeX
			set this.m_sizeY = sizeY
			set this.m_useShortcuts = useShortcuts
			set this.m_shortcut = shortcut
			//members
			set this.container = AWidgetContainer.create()
			set this.m_isShown = false

			if (gui != 0) then
				set this.m_guiIndex = gui.dockMainWindow(this)
			else
				set this.m_guiIndex = -1
			endif
			
			if (shortcut != -1) then
				debug if (not KeyIsValid(shortcut)) then
					debug call this.print("Shortcut has no valid key value.")
				debug endif
				call this.createShortcutTrigger()
			endif
			return this
		endmethod

		private method destroyDockedWidgets takes nothing returns nothing
			call this.container.forEach(0, AMainWindow.maxWidgets, AWidgetContainerUnaryFunction.unaryFunctionDestroyWidget)
		endmethod

		public method onDestroy takes nothing returns nothing
			//members
			call DestroyTextTag(this.tooltip)
			set this.tooltip = null
			if (this.m_shortcut != -1) then
				call AGetInterfaceHashTable().destroyTrigger(this.shortcutTrigger) 
				set this.shortcutTrigger = null
			endif

			call this.destroyDockedWidgets()
			call AWidgetContainer.destroy(this.container)

			if (this.m_gui != 0) then
				call this.m_gui.undockMainWindow(this)
			endif
		endmethod

		/// @param cameraSetup The camera setup which is used as the players view on the main window.
		/// @param tooltipSoundPath The path of the sound which is played when player drags the cursor over the related object. If this value is null there won't be played any sound.
		public static method init takes camerasetup cameraSetup, string tooltipSoundPath returns nothing
			set AMainWindow.cameraSetup = cameraSetup
			set AMainWindow.tooltipSoundPath = tooltipSoundPath

			if (tooltipSoundPath != null) then
				call PreloadSoundPath(tooltipSoundPath) //ALibraryEnvironmentSound
			endif
		endmethod
	endstruct

endlibrary
