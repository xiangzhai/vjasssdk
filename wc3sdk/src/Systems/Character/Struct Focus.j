/// Do not use this library, it is unfinished!
library AStructSystemsCharacterFocus requires ALibraryCoreDebugMisc, AStructCoreGeneralHashTable, ALibraryCoreMathsRect, ALibraryCoreMathsHandle, ALibraryCoreInterfaceMisc, ALibraryCoreInterfaceTextTag, ALibraryCoreEnvironmentUnit, AStructSystemsCharacterAbstractCharacterSystem

	struct AFocus extends AAbstractCharacterSystem
		//static start members
		private static real refreshRate
		private static real range
		private static real angle
		private static integer workerUnitType
		private static boolean showText
		private static boolean indicateTarget
		private static string textLevel
		//members
		private trigger focusTrigger
		private texttag textTag
		private trigger workerTrigger
		private unit worker
		private unit target
		private boolean fixedTarget
		
		//! runtextmacro optional A_STRUCT_DEBUG("\"AFocus\"")

		public method enable takes nothing returns nothing
			call super.enable()
			call EnableTrigger(this.focusTrigger)
			call ShowTextTagForPlayer(this.user(), this.textTag, true)
			call EnableTrigger(this.workerTrigger)
		endmethod

		public method disable takes nothing returns nothing
			call super.disable()
			call DisableTrigger(this.focusTrigger)
			call ShowTextTagForPlayer(this.user(), this.textTag, false)
			call DisableTrigger(this.workerTrigger)
		endmethod

		private method isTargetInFocus takes nothing returns boolean
			if (GetDistanceBetweenUnits(this.unit(), this.target, 0.0, 0.0) > AFocus.range) then
			//Optional kann man auch mit Z-Wert (Extrafunktion) überprfen lassen, würde aber mehr Speicher ziehen
				return false
			//Erst überprfen
			elseif (GetAngleBetweenUnits(this.unit(), this.target) > AFocus.angle) then
				return false
			endif
			return true
		endmethod

		//Don't destroy the group!
		private method getNearestTarget takes group usedGroup returns unit
			local unit first = null
			local unit nearest = null
			loop
				set first = FirstOfGroup(usedGroup)
				exitwhen (first == null)
				if (this.unit() != first) then //Darf sich nicht selbst anvisieren
					if ((nearest == null) or (GetDistanceBetweenUnits(this.unit(), first, 0.0, 0.0) < GetDistanceBetweenUnits(this.unit(), nearest, 0.0, 0.0))) then //ALibraryMathsHandle
						set nearest = first
					endif
				endif
				call GroupRemoveUnit(usedGroup, first)
				set first = null
			endloop
			return nearest
		endmethod

		private method getTargetName takes nothing returns string
			local integer state = GetUnitAllianceStateToUnit(this.unit(), this.target)
			local string colour
			if (state == bj_ALLIANCE_UNALLIED) then
				set colour = "|c00ff0000"
			elseif (state == bj_ALLIANCE_ALLIED) then
				set colour = "|c0000ff00"
			else //Neutral
				set colour = "|c00ffcc00"
			endif
			return (colour + GetUnitName(this.target) + "|r " + AFocus.textLevel + ":" + I2S(GetUnitLevel(this.target)))
		endmethod

		private method showTargetText takes nothing returns nothing
			if (AFocus.showText) then
				call SetTextTagTextBJ(this.textTag, this.getTargetName(), 12.0)
				call SetTextTagPos(this.textTag, GetUnitX(this.target), GetUnitY(this.target), (GetUnitFlyHeight(this.target) + 70.0))
				call ShowTextTagForPlayer(this.user(), this.textTag, true)
			endif
		endmethod

		private method indicateTheTarget takes real red, real green, real blue, real alpha returns nothing
			if (AFocus.indicateTarget) then
				call SetUnitVertexColourForPlayer(this.user(), this.target, red, green, blue, alpha)
			endif
		endmethod

		private method getNewTarget takes nothing returns nothing
			local real x1 = GetUnitX(this.unit())
			local real y1 = GetUnitY(this.unit())
			local real x3 = GetUnitPolarProjectionX(this.unit(), (GetUnitFacing(this.unit()) + AFocus.angle), AFocus.range) //ALibraryMathsHandle
			local real y3 = GetUnitPolarProjectionY(this.unit(), (GetUnitFacing(this.unit()) + AFocus.angle), AFocus.range) //ALibraryMathsHandle
			local real x4 = GetUnitPolarProjectionX(this.unit(), (GetUnitFacing(this.unit()) - AFocus.angle), AFocus.range) //ALibraryMathsHandle
			local real y4 = GetUnitPolarProjectionY(this.unit(), (GetUnitFacing(this.unit()) - AFocus.angle), AFocus.range) //ALibraryMathsHandle
			local group targetGroup = GetGroupInRectByCoordinates(x1, y1, x1, y1, x3, y3, x4, y4)
			debug if (IsUnitGroupEmptyBJ(targetGroup)) then
				debug call Print("Group is empty")
			debug else
				debug call Print("Group is not empty")
			debug endif
			set this.target = this.getNearestTarget(targetGroup) //Gruppe wird verändert
			call DestroyGroup(targetGroup)
			set targetGroup = null
			if (this.target == null) then
				call ShowTextTagForPlayer(this.user(), this.textTag, false)
				return
			endif
			call this.showTargetText()
			call this.indicateTheTarget(100.0, 50.0, 50.0, 0.0)
		endmethod

		private static method triggerActionFocus takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AFocus this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			//Hat bereits ein Ziel
			if (this.target != null) then
				//Altes Objekt ist außer Reichweite - Bentige neues Ziel
				if (not this.isTargetInFocus()) then
					call this.indicateTheTarget(100.0, 100.0, 100.0, 0.0)
					call this.getNewTarget()
				endif
			//Hat kein Ziel
			else
				call this.getNewTarget()
			endif
			set triggeringTrigger = null
		endmethod

		private method createFocusTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.focusTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterTimerEvent(this.focusTrigger, AFocus.refreshRate, true)
			set triggerAction = TriggerAddAction(this.focusTrigger, function AFocus.triggerActionFocus)
			call AHashTable.global().setHandleInteger(this.focusTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		private method createTextTag takes nothing returns nothing
			set this.textTag = CreateTextTag()
			call SetTextTagVisibility(this.textTag, false)
		endmethod

		private method createWorkerTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			//set this.worker = CreateUnit(this.getCharacter().user(), AFocus.workerUnitType, 

			set this.workerTrigger = CreateTrigger()
			//Noch ausarbeiten
		endmethod

		public static method create takes ACharacter character returns AFocus
			local AFocus this = AFocus.allocate(character)
			//members
			set this.fixedTarget = false

			call this.createFocusTrigger()
			call this.createTextTag()
			call this.createWorkerTrigger()
			return this
		endmethod

		private method destroyFocusTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.focusTrigger)
			set this.focusTrigger = null
		endmethod

		private method destroyTextTag takes nothing returns nothing
			call DestroyTextTag(this.textTag)
			set this.textTag = null
		endmethod

		private method destroyWorkerTrigger takes nothing returns nothing
			call RemoveUnit(this.worker)
			set this.worker = null
			call AHashTable.global().destroyTrigger(this.workerTrigger)
			set this.workerTrigger = null
		endmethod

		public method onDestroy takes nothing returns nothing

			call this.destroyFocusTrigger()
			call this.destroyTextTag()
			call this.destroyWorkerTrigger()
		endmethod

		/**
		* @param refreshRate 1.0 //If this value is 0.0 there is no text
		* @param range 500.0
		* @param angle 30.0 //Grad 0-360
		* @param workerUnitType 'HPEA'
		* @param showText true
		* @param indicateTarget true
		* @param textLevel Stufe
		*/
		public static method init takes real refreshRate, real range, real angle, integer workerUnitType, boolean showText, boolean indicateTarget, string textLevel returns nothing
			//static start members
			set AFocus.refreshRate = refreshRate
			set AFocus.range = range
			set AFocus.angle = angle
			set AFocus.workerUnitType = workerUnitType
			set AFocus.showText = showText
			set AFocus.indicateTarget = indicateTarget
			set AFocus.textLevel = textLevel
		endmethod
	endstruct

endlibrary