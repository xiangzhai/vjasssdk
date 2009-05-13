/// Do not use this library, it is unfinished!
library AStructSystemsCharacterView requires AStructSystemsCharacterCharacterHashTable, AStructSystemsCharacterAbstractCharacterSystem

	struct AView extends AAbstractCharacterSystem
		//static start members
		private static camerasetup cameraSetup
		private static real viewRefreshRate
		//members
		private trigger viewTrigger

		//methods

		public method enable takes nothing returns nothing
			call EnableTrigger(this.viewTrigger)
			call SetCameraTargetControllerNoZForPlayer(this.getUser(), this.getUnit(), 0.0, 0.0, false)
		endmethod

		public method disable takes nothing returns nothing
			call DisableTrigger(this.viewTrigger)
			call ResetToGameCameraForPlayer(this.getUser(), 0.0)
		endmethod

		private method refreshView takes nothing returns nothing
			local real z = GetTerrainZ(GetUnitX(this.getUnit()), GetUnitY(this.getUnit()))
			call CameraSetupApplyForPlayer(true, AView.cameraSetup, this.getUser(), 0.0)
			call SetCameraFieldForPlayer(this.getUser(), CAMERA_FIELD_ROTATION, GetUnitFacing(this.getUnit()), 0.0)
			call SetCameraFieldForPlayer(this.getUser(), CAMERA_FIELD_ZOFFSET, 128.0 + z + GetUnitFlyHeight(this.getUnit()), 0.0) //Eigentlich perfekter Z-Wert
        	endmethod

		private static method triggerActionRefreshView takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AView this = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "this")
			call this.refreshView()
			set triggeringTrigger = null
		endmethod

		private method createViewTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.viewTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterTimerEvent(this.viewTrigger, this.viewRefreshRate, true)
			set triggerAction = TriggerAddAction(this.viewTrigger, function AView.triggerActionRefreshView)
			call AGetCharacterHashTable().storeHandleInteger(this.viewTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		public static method create takes ACharacter character returns AView
			local AView this = AView.allocate(character)

			call this.createViewTrigger()
			return this
		endmethod

		private method destroyViewTrigger takes nothing returns nothing
			call AGetCharacterHashTable().destroyTrigger(this.viewTrigger)
			set this.viewTrigger = null
		endmethod

		public method onDestroy takes nothing returns nothing

			call this.destroyViewTrigger()
		endmethod

		public static method init takes camerasetup cameraSetup, real viewRefreshRate returns nothing
			//static start members
			set AView.cameraSetup = cameraSetup
			set AView.viewRefreshRate = viewRefreshRate
		endmethod
	endstruct

endlibrary