library AStructSystemsGuiMainWindow requires optional ALibraryCoreDebugMisc, AStructCoreGeneralHashTable, AStructCoreGeneralVector, ALibraryCoreInterfaceCamera, ALibraryCoreInterfaceCinematic, ALibraryCoreInterfaceTextTag, ALibraryCoreInterfaceMisc, ALibraryCoreEnvironmentSound, ALibraryCoreMathsRect, AStructSystemsGuiWidget

	/// @todo Should be a static function interface of @struct AMainWindow, vJass bug.
	function interface AMainWindowOnShowCondition takes AMainWindow mainWindow returns boolean

	/// @todo Should be a static function interface of @struct AMainWindow, vJass bug.
	function interface AMainWindowOnShowAction takes AMainWindow mainWindow returns nothing

	/// @todo Should be a static function interface of @struct AMainWindow, vJass bug.
	function interface AMainWindowOnHideCondition takes AMainWindow mainWindow returns boolean

	/// @todo Should be a static function interface of @struct AMainWindow, vJass bug.
	function interface AMainWindowOnHideAction takes AMainWindow mainWindow returns nothing

	struct AMainWindow
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
		private integer m_index
		private boolean m_isShown
		private AIntegerVector m_widgets
		private texttag m_tooltip
		private trigger m_shortcutTrigger
		private rect m_fogModifierRect
		private fogmodifier m_visibilityModifier
		private fogmodifier m_blackMaskModifier

		//! runtextmacro optional A_STRUCT_DEBUG("\"AMainWindow\"")

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

		public method index takes nothing returns integer
			return this.m_index
		endmethod

		public method isShown takes nothing returns boolean
			return this.m_isShown
		endmethod

		//convenience methods

		public method user takes nothing returns player
			return this.m_gui.user()
		endmethod

		//methods

		public method getX takes real x returns real
			return (this.m_x + x)
		endmethod

		public method getY takes real y returns real
			return (this.m_y - y)
		endmethod

		public method enableShortcut takes nothing returns nothing
			debug if (this.m_shortcut == -1) then
				debug call this.print("Main window does not use a shortcut.")
				debug return
			debug endif
			call EnableTrigger(this.m_shortcutTrigger)
		endmethod

		public method disableShortcut takes nothing returns nothing
			debug if (this.m_shortcut == -1) then
				debug call this.print("Main window does not use a shortcut.")
				debug return
			debug endif
			call DisableTrigger(this.m_shortcutTrigger)
		endmethod

		public method showTooltip takes AWidget usedWidget returns nothing
			if (this.m_tooltip == null) then
				set this.m_tooltip = CreateTextTag()
				call SetTextTagVisibility(this.m_tooltip, false)
			endif
			call SetTextTagTextBJ(this.m_tooltip, usedWidget.tooltip(), usedWidget.tooltipSize())
			call SetTextTagPos(this.m_tooltip, this.getX(this.m_tooltipX), this.getY(this.m_tooltipY), 0.0)
			call ShowTextTagForPlayer(this.user(), this.m_tooltip, true)
			if (thistype.tooltipSoundPath != null) then
				call PlaySoundFileForPlayer(this.user(), thistype.tooltipSoundPath)
			endif
		endmethod

		public method hideTooltip takes nothing returns nothing
			if (this.m_tooltip != null) then
				call ShowTextTagForPlayer(this.user(), this.m_tooltip, false)
			endif
		endmethod

		public method show takes nothing returns nothing
			local real x
			local real y
			local integer i
			if (this.m_onShowCondition != 0 and not this.m_onShowCondition.evaluate(this)) then
				return
			endif
			set x = this.m_x + (this.m_sizeX / 2.0)
			set y = this.m_y - (this.m_sizeY / 2.0)
			call this.m_gui.savePlayerData()
			call FogModifierStop(this.m_blackMaskModifier)
			call FogModifierStart(this.m_visibilityModifier)
			call ClearScreenMessagesForPlayer(this.user())
			call CameraSetupApplyForPlayer(false, thistype.cameraSetup, this.user(), 0.0)
			call PanCameraToTimedForPlayer(this.user(), x, y, 0.0)
			call SetCameraBoundsToPointForPlayer(this.user(), x, y) /// @todo DEBUG
			//widgets
			set i = 0
			loop
				exitwhen (i == this.m_widgets.size())
				call AWidget(this.m_widgets[i]).show()
				set i = i + 1
			endloop

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
			local integer i
			if (this.m_onHideCondition != 0 and not this.m_onHideCondition.evaluate(this)) then
				return
			endif

			call ResetCameraBoundsToMapRectForPlayer(this.user())
			call ResetToGameCameraForPlayer(this.user(), 0.0)
			call FogModifierStop(this.m_visibilityModifier)
			call FogModifierStart(this.m_blackMaskModifier)
			call this.m_gui.loadPlayerData() /// @todo DESYNC
			call this.hideTooltip()
			//widgets
			set i = 0
			loop
				exitwhen (i == this.m_widgets.size())
				call AWidget(this.m_widgets[i]).hide()
				set i = i + 1
			endloop

			if (this.m_useShortcuts) then
				call this.m_gui.disableShortcuts()
			endif
			set this.m_isShown = false
			call this.m_gui.resetShownMainWindow()

			if (this.m_onHideAction != 0) then
				call this.m_onHideAction.execute(this)
			endif
		endmethod

		/// Friend relationship to @struct AWidget, do not use.
		public method dockWidget takes AWidget usedWidget returns integer
			call this.m_widgets.pushBack(usedWidget)
			return this.m_widgets.backIndex()
		endmethod

		/// Friend relationship to @struct AWidget, do not use.
		public method undockWidgetByIndex takes integer index returns nothing
			call this.m_widgets.erase(index)
		endmethod

		private static method triggerActionPressShortcut takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
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
			set this.m_shortcutTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterKeyEventForPlayer(this.user(), this.m_shortcutTrigger, this.m_shortcut, true)
			set triggerAction = TriggerAddAction(this.m_shortcutTrigger, function thistype.triggerActionPressShortcut)
			call AHashTable.global().setHandleInteger(this.m_shortcutTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		/// @param shortcut If this value is -1 main window won't have any shortcut.
		public static method create takes AGui gui, real x, real y, real sizeX, real sizeY, boolean useShortcuts, integer shortcut returns thistype
			local thistype this = thistype.allocate()
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
			set this.m_index = gui.dockMainWindow(this)
			set this.m_widgets = AIntegerVector.create()
			set this.m_isShown = false
			set this.m_fogModifierRect = RectFromPointSize(this.m_x + this.m_sizeX / 2.0, this.m_y - this.m_sizeY / 2.0, this.m_sizeX, this.m_sizeY)
			set this.m_visibilityModifier = CreateFogModifierRect(this.user(), FOG_OF_WAR_VISIBLE, this.m_fogModifierRect, true, false)
			set this.m_blackMaskModifier = CreateFogModifierRect(this.user(), FOG_OF_WAR_MASKED, this.m_fogModifierRect, true, false)
			call FogModifierStart(this.m_blackMaskModifier)

			if (shortcut != -1) then
				debug if (not KeyIsValid(shortcut)) then
					debug call this.print("Shortcut has no valid key value.")
				debug endif
				call this.createShortcutTrigger()
			endif
			return this
		endmethod

		public static method createByRect takes AGui gui, rect usedRect, boolean useShortcuts, integer shortcut returns AMainWindow
			return thistype.create(gui, GetRectMinX(usedRect), GetRectMaxY(usedRect), GetRectWidthBJ(usedRect), GetRectHeightBJ(usedRect), useShortcuts, shortcut)
		endmethod

		public method onDestroy takes nothing returns nothing
			call this.m_gui.undockMainWindowByIndex(this.m_index)
			//members
			if (this.m_tooltip != null) then
				call DestroyTextTag(this.m_tooltip)
				set this.m_tooltip = null
			endif
			call RemoveRect(this.m_fogModifierRect)
			set this.m_fogModifierRect = null
			call DestroyFogModifier(this.m_visibilityModifier)
			set this.m_visibilityModifier = null
			call DestroyFogModifier(this.m_blackMaskModifier)
			set this.m_blackMaskModifier = null

			if (this.m_shortcut != -1) then
				call AHashTable.global().destroyTrigger(this.m_shortcutTrigger)
				set this.m_shortcutTrigger = null
			endif

			loop
				exitwhen (this.m_widgets.empty())
				call AWidget(this.m_widgets.back()).destroy()
				/// @todo don't pop back, is in destructor, check for errors
			endloop
			call this.m_widgets.destroy()
		endmethod

		/// @param cameraSetup The camera setup which is used as the players view on the main window.
		/// @param tooltipSoundPath The path of the sound which is played when player drags the cursor over the related object. If this value is null there won't be played any sound.
		public static method init takes camerasetup cameraSetup, string tooltipSoundPath returns nothing
			set thistype.cameraSetup = cameraSetup
			set thistype.tooltipSoundPath = tooltipSoundPath

			if (tooltipSoundPath != null) then
				call PreloadSoundFile(tooltipSoundPath) //ALibraryEnvironmentSound
			endif
		endmethod
	endstruct

endlibrary
