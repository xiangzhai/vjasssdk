library AStructSystemsGuiWidget requires ALibraryCoreInterfaceTrackable, ALibraryCoreEnvironmentSound, AStructSystemsGuiGui

	/// @todo Should be a static method of @struct AWidget, vJass bug.
	function interface AWidgetOnHitAction takes AWidget usedWidget returns nothing

	/// @todo Should be a static method of @struct AWidget, vJass bug.
	function interface AWidgetOnTrackAction takes AWidget usedWidget returns nothing
	
	/// @todo Should be a static method of @struct AWidget, vJass bug.
	/// Use this method as track action if you want to have the generic tooltip.
	/// You can also use another track action and call this method in your custom action.
	function onTrackActionShowTooltip takes AWidget usedWidget returns nothing
		call usedWidget.getMainWindow().showTooltip(usedWidget)
	endfunction

	struct AWidget
		//static start members
		private static string onHitSoundPath = null
		private static string onTrackSoundPath = null
		//dynamic members
		private boolean m_shown
		private integer shortcut //Wenn das Tastenkürzel gedrückt wird, wird auch die onHitFunction ausgeführt. Die Tastenkürzel werden über eine ausgewählte Einheit mit entsprechenden Fähigkeiten gesteuert.
		private string tooltip
		private real tooltipSize
		//start members
		private AMainWindow mainWindow
		private real x
		private real y
		private real sizeX
		private real sizeY
		private AWidgetOnHitAction onHitAction
		private AWidgetOnTrackAction onTrackAction
		//members
		private integer m_mainWindowIndex
		private trackable usedTrackable
		private trigger onHitTrigger
		private trigger onTrackTrigger

		//dynamic members

		public method setShown takes boolean shown returns nothing
			if (this.m_shown == shown) then
				return
			endif
			if (shown) then
				call this.show()
			else
				call this.hide()
			endif
		endmethod

		public method isShown takes nothing returns boolean
			return this.m_shown
		endmethod

		/// Important: Set all shortcuts when before showing the GUI.
		/// When hiding the GUI all shortcut actions will be reseted (because of the different main windows).
		public method setShortcut takes integer shortcut returns nothing
			//clear old action
			if (this.shortcut != 0) then
				call this.mainWindow.gui().setOnPressShortcutAction(shortcut, 0, 0)
			endif
			set this.shortcut = shortcut
			if (this.onHitAction != 0) then
				call this.mainWindow.gui().setOnPressShortcutAction(shortcut, this.onHitAction, this)
			endif
		endmethod

		public method getShortcut takes nothing returns integer
			return this.shortcut
		endmethod

		public method setTooltip takes string tooltip returns nothing
			set this.tooltip = tooltip
		endmethod

		public method getTooltip takes nothing returns string
			return this.tooltip
		endmethod

		public method setTooltipSize takes real tooltipSize returns nothing
			set this.tooltipSize = tooltipSize
		endmethod

		public method getTooltipSize takes nothing returns real
			return this.tooltipSize
		endmethod

		//start members

		public method getMainWindow takes nothing returns AMainWindow
			return this.mainWindow
		endmethod

		public method getX takes nothing returns real
			return this.x
		endmethod

		public method getY takes nothing returns real
			return this.y
		endmethod

		public method getSizeX takes nothing returns real
			return this.sizeX
		endmethod

		public method getSizeY takes nothing returns real
			return this.sizeY
		endmethod
		
		//members
		
		public method mainWindowIndex takes nothing returns integer
			return this.m_mainWindowIndex
		endmethod
		
		//comfort methods
		
		public method getGui takes nothing returns AGui
			return this.mainWindow.gui()
		endmethod

		public method getUser takes nothing returns player
			return this.mainWindow.user()
		endmethod

		//methods

		public stub method show takes nothing returns nothing
			call this.enableOnHitTrigger()
			call this.enableOnTrackTrigger()
			set this.m_shown = true
		endmethod

		public stub method hide takes nothing returns nothing
			call this.disableOnHitTrigger()
			call this.disableOnTrackTrigger()
			set this.m_shown = false
		endmethod

		private method enableOnHitTrigger takes nothing returns nothing
			if (this.onHitAction != 0) then
				call EnableTrigger(this.onHitTrigger)
			endif
		endmethod

		private method enableOnTrackTrigger takes nothing returns nothing
			if (this.onTrackAction != 0) then
				call EnableTrigger(this.onTrackTrigger)
			endif
		endmethod

		private method disableOnHitTrigger takes nothing returns nothing
			if (this.onHitAction != 0) then
				call DisableTrigger(this.onHitTrigger)
			endif
		endmethod

		private method disableOnTrackTrigger takes nothing returns nothing
			if (this.onTrackAction != 0) then
				call DisableTrigger(this.onTrackTrigger)
			endif
		endmethod

		private method createTrackable takes nothing returns nothing
			if ((this.onHitAction != 0) or (this.onTrackAction != 0)) then
				set this.usedTrackable = CreateTrackableForPlayer(this.getUser(), AWidget.getTrackablePathBySize(this.sizeX, this.sizeY), this.mainWindow.getX(this.x), this.mainWindow.getY(this.y), 0.0) //ALibraryInterfaceTrackable
			endif
		endmethod

		private static method triggerActionOnHit takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AWidget this = AGetInterfaceHashTable().getHandleInteger(triggeringTrigger, "this")
			call this.onHitAction.execute(this)
			if (AWidget.onHitSoundPath != null) then
				call PlaySoundPathForPlayer(this.getUser(), AWidget.onHitSoundPath)
			endif
			set triggeringTrigger = null
		endmethod

		private method createOnHitTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			if (this.onHitAction != 0) then
				set this.onHitTrigger = CreateTrigger()
				set triggerEvent = TriggerRegisterTrackableHitEvent(this.onHitTrigger, this.usedTrackable)
				set triggerAction = TriggerAddAction(this.onHitTrigger, function AWidget.triggerActionOnHit)
				call AGetInterfaceHashTable().storeHandleInteger(this.onHitTrigger, "this", this)
				set triggerEvent = null
				set triggerAction = null
			endif
		endmethod

		private static method triggerActionOnTrack takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AWidget this = AGetInterfaceHashTable().getHandleInteger(triggeringTrigger, "this")
			call this.onTrackAction.execute(this)
			if (AWidget.onTrackSoundPath != null) then
				call PlaySoundPathForPlayer(this.getUser(), AWidget.onTrackSoundPath)
			endif
			set triggeringTrigger = null
		endmethod

		private method createOnTrackTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			if (this.onTrackAction != 0) then
				set this.onTrackTrigger = CreateTrigger()
				set triggerEvent = TriggerRegisterTrackableTrackEvent(this.onTrackTrigger, this.usedTrackable)
				set triggerAction = TriggerAddAction(this.onTrackTrigger, function AWidget.triggerActionOnTrack)
				call AGetInterfaceHashTable().storeHandleInteger(this.onTrackTrigger, "this", this)
				set triggerEvent = null
				set triggerAction = null
			endif
		endmethod

		public static method create takes AMainWindow mainWindow, real x, real y, real sizeX, real sizeY, AWidgetOnHitAction onHitAction, AWidgetOnTrackAction onTrackAction returns AWidget
			local AWidget this = AWidget.allocate()
			//dynamic members
			set this.m_shown = false
			//start members
			set this.mainWindow = mainWindow
			set this.x = x
			set this.y = y
			set this.sizeX = sizeX
			set this.sizeY = sizeY
			set this.onHitAction = onHitAction
			set this.onTrackAction = onTrackAction

			call this.createTrackable()
			call this.createOnHitTrigger()
			call this.createOnTrackTrigger()

			if (mainWindow != 0) then
				set this.m_mainWindowIndex = mainWindow.dockWidget(this)
			else
				set this.m_mainWindowIndex = -1
			endif

			return this
		endmethod

		private method destroyTrackable takes nothing returns nothing
			if ((this.onHitAction != 0) or (this.onTrackAction != 0)) then
				//we can't destroy trackables :-[
				set this.usedTrackable = null
			endif
		endmethod

		private method destroyOnHitTrigger takes nothing returns nothing
			if (this.onHitAction != 0) then
				call AGetInterfaceHashTable().destroyTrigger(this.onHitTrigger) 
				set this.onHitTrigger = null
			endif
		endmethod

		private method destroyOnTrackTrigger takes nothing returns nothing
			if (this.onTrackAction != 0) then
				call AGetInterfaceHashTable().destroyTrigger(this.onTrackTrigger)
				set this.onTrackTrigger = null
			endif
		endmethod

		public method onDestroy takes nothing returns nothing

			call this.destroyOnHitTrigger()
			call this.destroyOnTrackTrigger()

			if (this.mainWindow != 0) then
				call this.mainWindow.undockWidget(this)
			endif
		endmethod

		public static method init takes string onHitSoundPath, string onTrackSoundPath returns nothing
			set AWidget.onHitSoundPath = onHitSoundPath
			set AWidget.onTrackSoundPath = onTrackSoundPath

			if (onHitSoundPath != null) then
				call PreloadSoundPath(onHitSoundPath)
			endif
			if (onTrackSoundPath != null) then
				call PreloadSoundPath(onTrackSoundPath)
			endif
		endmethod

		public static method getTrackablePathBySize takes real sizeX, real sizeY returns string
			//I need a list of models, which has the specific sizes of trackables
			return "units\\nightelf\\Wisp\\Wisp.mdx"
		endmethod
	endstruct

endlibrary