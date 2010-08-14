/// @todo Do not use this library, it is unfinished!
library AStructSystemsCharacterFocus requires optional ALibraryCoreDebugMisc, AStructCoreGeneralHashTable, ALibraryCoreMathsRect, ALibraryCoreMathsHandle, ALibraryCoreInterfaceMisc, ALibraryCoreInterfaceTextTag, ALibraryCoreEnvironmentUnit, AStructCoreStringFormat, AStructSystemsCharacterAbstractCharacterSystem

	/**
	* Should provide character focusing as in games like Gothic.
	* Text tags are used as focus titles.
	* @todo Untested!
	* @todo Remove worker trigger options and add hero icon of target -> AHeroIcon!
	*/
	struct AFocus extends AAbstractCharacterSystem
		// static construction membes
		private static real m_refreshRate
		private static integer m_workerUnitType
		// dynamic members
		private real m_range
		private real m_angle
		private real m_height
		private boolean m_showText
		private boolean m_indicateTarget
		private string m_textTargetName // string arguments: color, name, level
		// members
		private trigger m_focusTrigger
		private texttag m_textTag
		private trigger m_workerTrigger
		private unit m_worker
		private unit m_target
		private boolean m_fixedTarget

		//! runtextmacro optional A_STRUCT_DEBUG("\"AFocus\"")

		public method enable takes nothing returns nothing
			call super.enable()
			call EnableTrigger(this.m_focusTrigger)
			call ShowTextTagForPlayer(this.character().player(), this.m_textTag, this.m_target != null and this.m_showText)
			call EnableTrigger(this.m_workerTrigger)
		endmethod

		public method disable takes nothing returns nothing
			call super.disable()
			call DisableTrigger(this.m_focusTrigger)
			call ShowTextTagForPlayer(this.character().player(), this.m_textTag, false)
			call DisableTrigger(this.m_workerTrigger)
		endmethod

		private method isTargetInFocus takes nothing returns boolean
			if (GetDistanceBetweenUnits(this.character().unit(), this.m_target, 0.0, 0.0) > this.m_range) then
			// Optional kann man auch mit Z-Wert (Extrafunktion) überprfen lassen, würde aber mehr Speicher benötigen
				return false
			// Erst überprfen
			elseif (GetAngleBetweenUnits(this.character().unit(), this.m_target) > this.m_angle) then
				return false
			endif
			return true
		endmethod

		// Don't destroy the group!
		private method getNearestTarget takes group usedGroup returns unit
			local unit first = null
			local unit nearest = null
			loop
				set first = FirstOfGroup(usedGroup)
				exitwhen (first == null)
				if (this.character().unit() != first) then //Darf sich nicht selbst anvisieren
					if ((nearest == null) or (GetDistanceBetweenUnits(this.character().unit(), first, 0.0, 0.0) < GetDistanceBetweenUnits(this.character().unit(), nearest, 0.0, 0.0))) then //ALibraryMathsHandle
						set nearest = first
					endif
				endif
				call GroupRemoveUnit(usedGroup, first)
				set first = null
			endloop
			return nearest
		endmethod

		private method getTargetName takes nothing returns string
			local integer state = GetUnitAllianceStateToUnit(this.character().unit(), this.m_target)
			local string colour
			if (state == bj_ALLIANCE_UNALLIED) then
				set colour = "ff0000"
			elseif (state == bj_ALLIANCE_ALLIED) then
				set colour = "00ff00"
			else // neutral
				set colour = "ffcc00"
			endif
			return Format(this.m_textTargetName).s(colour).s(GetUnitName(this.m_target)).i(GetUnitLevel(this.m_target)).result()
		endmethod

		private method showTargetText takes nothing returns nothing
			if (this.m_showText) then
				call SetTextTagTextBJ(this.m_textTag, this.getTargetName(), 12.0)
				call SetTextTagPos(this.m_textTag, GetUnitX(this.m_target), GetUnitY(this.m_target), (GetUnitFlyHeight(this.m_target) + this.m_height))
				call ShowTextTagForPlayer(this.character().player(), this.m_textTag, true)
			endif
		endmethod

		private method indicateTheTarget takes real red, real green, real blue, real alpha returns nothing
			if (this.m_indicateTarget) then
				call SetUnitVertexColourForPlayer(this.character().player(), this.m_target, red, green, blue, alpha)
			endif
		endmethod

		private method getNewTarget takes nothing returns nothing
			local real x1 = GetUnitX(this.character().unit())
			local real y1 = GetUnitY(this.character().unit())
			local real x3 = GetUnitPolarProjectionX(this.character().unit(), (GetUnitFacing(this.character().unit()) + this.m_angle), this.m_range)
			local real y3 = GetUnitPolarProjectionY(this.character().unit(), (GetUnitFacing(this.character().unit()) + this.m_angle), this.m_range)
			local real x4 = GetUnitPolarProjectionX(this.character().unit(), (GetUnitFacing(this.character().unit()) - this.m_angle), this.m_range)
			local real y4 = GetUnitPolarProjectionY(this.character().unit(), (GetUnitFacing(this.character().unit()) - this.m_angle), this.m_range)
			local group targetGroup = GetGroupInRectByCoordinates(x1, y1, x1, y1, x3, y3, x4, y4)
			debug if (IsUnitGroupEmptyBJ(targetGroup)) then
				debug call Print("Group is empty")
			debug else
				debug call Print("Group is not empty")
			debug endif
			set this.m_target = this.getNearestTarget(targetGroup) // group will be changed
			call DestroyGroup(targetGroup)
			set targetGroup = null
			if (this.m_target == null) then
				call ShowTextTagForPlayer(this.character().player(), this.m_textTag, false)
				return
			endif
			call this.showTargetText()
			call this.indicateTheTarget(100.0, 50.0, 50.0, 0.0)
		endmethod

		private static method triggerActionFocus takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AFocus this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			// already has target
			if (this.m_target != null) then
				// old target is out of range -> need new one
				if (not this.isTargetInFocus()) then
					call this.indicateTheTarget(100.0, 100.0, 100.0, 0.0)
					call this.getNewTarget()
				endif
			// doesn't have target
			else
				call this.getNewTarget()
			endif
			set triggeringTrigger = null
		endmethod

		private method createFocusTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_focusTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterTimerEvent(this.m_focusTrigger, thistype.m_refreshRate, true)
			set triggerAction = TriggerAddAction(this.m_focusTrigger, function thistype.triggerActionFocus)
			call AHashTable.global().setHandleInteger(this.m_focusTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod

		private method createTextTag takes nothing returns nothing
			set this.m_textTag = CreateTextTag()
			call SetTextTagVisibility(this.m_textTag, false)
		endmethod

		private method createWorkerTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_worker = CreateUnit(this.character().player(), thistype.m_workerUnitType, 0.0, 0.0, 0.0)
			call SetUnitInvulnerable(this.m_worker, true)
			call PauseUnit(this.m_worker, true)

			set this.m_workerTrigger = CreateTrigger()
			/// @todo Add events and action
		endmethod

		public static method create takes ACharacter character returns thistype
			local thistype this = thistype.allocate(character)
			// dynamic members
			set this.m_range = 500.0
			set this.m_angle = 30.0
			set this.m_height = 70.0
			set this.m_showText = true
			set this.m_indicateTarget = true
			set this.m_textTargetName = tr("|cff%1%%2%|r (%2%)")
			// members
			set this.m_fixedTarget = false

			call this.createFocusTrigger()
			call this.createTextTag()
			call this.createWorkerTrigger()
			return this
		endmethod

		private method destroyFocusTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_focusTrigger)
			set this.m_focusTrigger = null
		endmethod

		private method destroyTextTag takes nothing returns nothing
			call DestroyTextTag(this.m_textTag)
			set this.m_textTag = null
		endmethod

		private method destroyWorkerTrigger takes nothing returns nothing
			call RemoveUnit(this.m_worker)
			set this.m_worker = null
			call AHashTable.global().destroyTrigger(this.m_workerTrigger)
			set this.m_workerTrigger = null
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
		*/
		public static method init takes real refreshRate, integer workerUnitType returns nothing
			// static construction members
			debug if (refreshRate <= 0.0) then
				debug call thistype.staticPrint("Refresh rate is less or equal to 0")
				debug set refreshRate = 1.0
			debug endif
			set thistype.m_refreshRate = refreshRate
			set thistype.m_workerUnitType = workerUnitType
		endmethod
	endstruct

endlibrary