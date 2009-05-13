library AStructSystemsCharacterRevival requires ALibraryCoreDebugMisc, ALibraryCoreInterfaceMisc, AStructSystemsCharacterCharacterHashTable, AStructSystemsCharacterAbstractCharacterSystem

	struct ARevival extends AAbstractCharacterSystem
		//static start members
		private static boolean showDialog
		//dynamic members
		private real time
		private real x
		private real y
		private real facing
		//members
		private trigger revivalTrigger
		private timer usedTimer = null
		private timerdialog timerDialog = null

		//! runtextmacro A_STRUCT_DEBUG("\"ARevival\"")

		//dynamic members

		public method setTime takes real time returns nothing
			set this.time = time
		endmethod

		public method getTime takes nothing returns real
			return this.time
		endmethod

		public method setX takes real x returns nothing
			set this.x = x
		endmethod

		public method getX takes nothing returns real
			return this.x
		endmethod

		public method setY takes real y returns nothing
			set this.y = y
		endmethod

		public method getY takes nothing returns real
			return this.y
		endmethod

		public method setFacing takes real facing returns nothing
			set this.facing = facing
		endmethod

		public method getFacing takes nothing returns real
			return this.facing
		endmethod

		//methods

		public method enable takes nothing returns nothing
			call EnableTrigger(this.revivalTrigger)
			if (TimerGetRemaining(this.usedTimer) > 0.0) then
				call PauseTimerBJ(false, this.usedTimer)
				if (ARevival.showDialog) then //Der bersicht halber nicht in die Funktion bergeben
					call TimerDialogDisplay(this.timerDialog, true)
				endif
			endif
		endmethod

		public method disable takes nothing returns nothing
			call DisableTrigger(this.revivalTrigger)
			if (TimerGetRemaining(this.usedTimer) > 0.0) then
				call PauseTimerBJ(true, this.usedTimer)
				if (ARevival.showDialog) then
					call TimerDialogDisplay(this.timerDialog, false)
				endif
			endif
		endmethod
		
		private method revive takes nothing returns nothing
			call ReviveHero(this.getUnit(), this.x, this.y, true)
			call SetUnitFacing(this.getUnit(), this.facing)
		endmethod

		private static method timerFunctionRevival takes nothing returns nothing
			local timer expiredTimer = GetExpiredTimer()
			local ARevival revival = AGetCharacterHashTable().getHandleInteger(expiredTimer, "revival") //AClassCharacterCharacterHashTable
			debug call revival.print("Finished.")
			call revival.revive()
			call revival.end()
			//select unit
			call revival.getCharacter().setMovable(true)
			set expiredTimer = null
		endmethod

		private method start takes nothing returns nothing
			debug if (this.usedTimer == null) then
				debug call this.print("TIMER IS NULLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL")
			debug endif
			call TimerStart(this.usedTimer, this.time, false, function ARevival.timerFunctionRevival) //function ARevival.timerFunctionRevival //this.Time //Verursacht Bug
			if (ARevival.showDialog) then
				debug call this.print("Display, start with time " + R2S(this.time) + ".")
				debug if (this.timerDialog == null) then
					debug call this.print("Is null.")
				debug endif
				call TimerDialogDisplay(this.timerDialog, true)
			endif
		endmethod

		private method end takes nothing returns nothing
			call PauseTimerBJ(true, this.usedTimer) //Zur Sicherheit auch stoppen
			if (ARevival.showDialog) then
				call TimerDialogDisplay(this.timerDialog, false)
			endif
		endmethod

		private method createTimer takes nothing returns nothing
			set this.usedTimer = CreateTimer()
			call AGetCharacterHashTable().storeHandleInteger(this.usedTimer, "revival", this) //AClassCharacterCharacterHashTable
			if (ARevival.showDialog) then
				set this.timerDialog = CreateTimerDialog(this.usedTimer)
				call TimerDialogSetTitle(this.timerDialog, GetModifiedPlayerName(this.getUser())) //ALibraryInterfaceMisc
				//call TimerDialogDisplay(this.timerDialog, false) //test
			endif
		endmethod

		private static method triggerActionRevival takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local ARevival revival = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "revival") //AClassCharacterCharacterHashTable
			debug call revival.print("Character " + I2S(revival.getCharacter()) + " died and will be revived.")
			if (revival.time > 0.0) then
				debug call revival.print("Start revival")
				call revival.start()
			else
				call revival.revive()
			endif
			//set unmovable
			set triggeringTrigger = null
		endmethod

		private method createRevivalTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.revivalTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterDeathEvent(this.revivalTrigger, this.getUnit())
			set triggerAction = TriggerAddAction(this.revivalTrigger, function ARevival.triggerActionRevival)
			call AGetCharacterHashTable().storeHandleInteger(this.revivalTrigger, "revival", this) //AClassCharacterCharacterHashTable
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
			call AGetCharacterHashTable().destroyTimer(this.usedTimer) //AClassCharacterCharacterHashTable
			set this.usedTimer = null
			if (ARevival.showDialog) then
				call DestroyTimerDialog(this.timerDialog)
				set this.timerDialog = null
			endif
		endmethod

		private method destroyRevivalTrigger takes nothing returns nothing
			call AGetCharacterHashTable().destroyTrigger(this.revivalTrigger) //AClassCharacterCharacterHashTable
			set this.revivalTrigger = null
		endmethod

		public method onDestroy takes nothing returns nothing

			call this.destroyTimer()
			call this.destroyRevivalTrigger()
		endmethod

		public static method init takes boolean showDialog returns nothing
			//static start members
			set ARevival.showDialog = showDialog
		endmethod
	endstruct

endlibrary