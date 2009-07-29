library AStructSystemsCharacterRevival requires ALibraryCoreDebugMisc, AStructCoreGeneralHashTable, ALibraryCoreInterfaceMisc, AStructSystemsCharacterAbstractCharacterSystem

	struct ARevival extends AAbstractCharacterSystem
		//static start members
		private static boolean m_showDialog
		//dynamic members
		private real m_time
		private real m_x
		private real m_y
		private real m_facing
		//members
		private trigger m_revivalTrigger
		private timer m_timer
		private timerdialog m_timerDialog

		//! runtextmacro A_STRUCT_DEBUG("\"ARevival\"")

		//dynamic members

		public method setTime takes real time returns nothing
			set this.m_time = time
		endmethod

		public method time takes nothing returns real
			return this.m_time
		endmethod

		public method setX takes real x returns nothing
			set this.m_x = x
		endmethod

		public method x takes nothing returns real
			return this.m_x
		endmethod

		public method setY takes real y returns nothing
			set this.m_y = y
		endmethod

		public method y takes nothing returns real
			return this.m_y
		endmethod

		public method setFacing takes real facing returns nothing
			set this.m_facing = facing
		endmethod

		public method facing takes nothing returns real
			return this.m_facing
		endmethod

		//methods

		public method enable takes nothing returns nothing
			call EnableTrigger(this.m_revivalTrigger)
			if (TimerGetRemaining(this.m_timer) > 0.0) then
				call PauseTimerBJ(false, this.m_timer)
				if (ARevival.m_showDialog) then //Der bersicht halber nicht in die Funktion bergeben
					call TimerDialogDisplay(this.m_timerDialog, true)
				endif
			endif
		endmethod

		public method disable takes nothing returns nothing
			call DisableTrigger(this.m_revivalTrigger)
			if (TimerGetRemaining(this.m_timer) > 0.0) then
				call PauseTimerBJ(true, this.m_timer)
				if (ARevival.m_showDialog) then
					call TimerDialogDisplay(this.m_timerDialog, false)
				endif
			endif
		endmethod
		
		private method revive takes nothing returns nothing
			call ReviveHero(this.unit(), this.m_x, this.m_y, true)
			call SetUnitFacing(this.unit(), this.m_facing)
		endmethod

		private static method timerFunctionRevival takes nothing returns nothing
			local timer expiredTimer = GetExpiredTimer()
			local ARevival this = AHashTable.global().getHandleInteger(expiredTimer, "this")
			call this.revive()
			call this.end()
			set expiredTimer = null
		endmethod

		private method start takes nothing returns nothing
			call TimerStart(this.m_timer, this.m_time, false, function ARevival.timerFunctionRevival) //function ARevival.timerFunctionRevival //this.Time //Verursacht Bug
			if (ARevival.m_showDialog) then
				call TimerDialogDisplay(this.m_timerDialog, true)
			endif
		endmethod

		private method end takes nothing returns nothing
			call PauseTimer(this.m_timer) //Zur Sicherheit auch stoppen
			if (ARevival.m_showDialog) then
				call TimerDialogDisplay(this.m_timerDialog, false)
			endif
		endmethod

		private method createTimer takes nothing returns nothing
			set this.m_timer = CreateTimer()
			call AHashTable.global().storeHandleInteger(this.m_timer, "this", this)
			if (ARevival.m_showDialog) then
				set this.m_timerDialog = CreateTimerDialog(this.m_timer)
				call TimerDialogSetTitle(this.m_timerDialog, GetModifiedPlayerName(this.user()))
				//call TimerDialogDisplay(this.timerDialog, false) //test
			endif
		endmethod

		private static method triggerActionRevival takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ARevival this = AHashTable.global().getHandleInteger(triggeringTrigger, "this")
			if (this.m_time > 0.0) then
				call this.start()
			else
				call this.revive()
			endif
			//set unmovable
			set triggeringTrigger = null
		endmethod

		private method createRevivalTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_revivalTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterUnitEvent(this.m_revivalTrigger, this.unit(), EVENT_UNIT_DEATH) //TriggerRegisterDeathEvent(this.revivalTrigger, this.unit())
			set triggerAction = TriggerAddAction(this.m_revivalTrigger, function ARevival.triggerActionRevival)
			call AHashTable.global().storeHandleInteger(this.m_revivalTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		public static method create takes ACharacter character returns ARevival
			local ARevival this = ARevival.allocate(character)
			call this.createTimer()
			call this.createRevivalTrigger()
			return this
		endmethod

		private method destroyTimer takes nothing returns nothing
			call PauseTimer(this.m_timer)
			call AHashTable.global().destroyTimer(this.m_timer)
			set this.m_timer = null
			if (ARevival.m_showDialog) then
				call DestroyTimerDialog(this.m_timerDialog)
				set this.m_timerDialog = null
			endif
		endmethod

		private method destroyRevivalTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_revivalTrigger)
			set this.m_revivalTrigger = null
		endmethod

		public method onDestroy takes nothing returns nothing

			call this.destroyTimer()
			call this.destroyRevivalTrigger()
		endmethod

		public static method init takes boolean showDialog returns nothing
			//static start members
			set ARevival.m_showDialog = showDialog
		endmethod
	endstruct

endlibrary