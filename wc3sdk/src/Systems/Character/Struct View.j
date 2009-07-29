/// Do not use this library, it is unfinished!
library AStructSystemsCharacterView requires AStructCoreGeneralHashTable, AStructSystemsCharacterAbstractCharacterSystem

	struct AView extends AAbstractCharacterSystem
		//static start members
		private static camerasetup cameraSetup
		private static real viewRefreshRate
		//members
		private trigger m_viewTrigger

		//methods

		public method enable takes nothing returns nothing
			call EnableTrigger(this.m_viewTrigger)
			call SetCameraTargetControllerNoZForPlayer(this.user(), this.unit(), 0.0, 0.0, false) /// @todo is not called?!
		endmethod

		public method disable takes nothing returns nothing
			call DisableTrigger(this.m_viewTrigger)
			call ResetToGameCameraForPlayer(this.user(), 0.0)
		endmethod

		private method refreshView takes nothing returns nothing
			local real z = GetTerrainZ(GetUnitX(this.unit()), GetUnitY(this.unit()))
			call CameraSetupApplyForPlayer(false, thistype.cameraSetup, this.user(), 0.0)
			call SetCameraFieldForPlayer(this.user(), CAMERA_FIELD_ROTATION, GetUnitFacing(this.unit()), 0.0)
			call SetCameraFieldForPlayer(this.user(), CAMERA_FIELD_ZOFFSET, z + GetUnitFlyHeight(this.unit()) + 128.0, 0.0) //Eigentlich perfekter Z-Wert
		endmethod

		private static method triggerActionRefreshView takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().getHandleInteger(triggeringTrigger, "this")
			call this.refreshView()
			set triggeringTrigger = null
		endmethod

		private method createViewTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_viewTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterTimerEvent(this.m_viewTrigger, thistype.viewRefreshRate, true)
			set triggerAction = TriggerAddAction(this.m_viewTrigger, function thistype.triggerActionRefreshView)
			call AHashTable.global().storeHandleInteger(this.m_viewTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		public static method create takes ACharacter character returns AView
			local AView this = AView.allocate(character)
			//call SetCameraTargetControllerNoZForPlayer(this.user(), this.unit(), 0.0, 0.0, false)

			call this.createViewTrigger()
			return this
		endmethod

		private method destroyViewTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_viewTrigger)
			set this.m_viewTrigger = null
		endmethod

		public method onDestroy takes nothing returns nothing

			call this.destroyViewTrigger()
		endmethod

		public static method init takes camerasetup cameraSetup, real viewRefreshRate returns nothing
			//static start members
			set thistype.cameraSetup = cameraSetup
			set thistype.viewRefreshRate = viewRefreshRate
		endmethod
	endstruct

endlibrary