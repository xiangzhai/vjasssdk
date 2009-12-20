library AStructSystemsWorldLayer requires AModuleCoreGeneralSystemStruct, ALibraryCoreMathsHandle, ALibraryCoreMathsUnit

	/// @todo Should be contained by @struct ALayer, vJass bug.
	function interface ALayerOnEnterFunction takes unit whichUnit returns nothing

	/// @todo Should be contained by @struct ALayer, vJass bug.
	function interface ALayerOnLeaveFunction takes unit whichUnit returns nothing

	/**
	* @todo Untested!
	*/
	struct ALayer
		private real m_height
		private ALayerOnEnterFunction m_onEnterFunction
		private ALayerOnLeaveFunction m_onLeaveFunction
		private region m_entry
		private region m_exit
		private region m_region
		private AUnitVector m_units
		private trigger m_enterTrigger
		private trigger m_leaveTrigger
		private trigger m_boundsTrigger

		implement ASystemStruct

		public method setHeight takes real height returns nothing
			local integer i = 0
			set this.m_height = height

			loop
				exitwhen (i == this.m_units.size())
				call SetUnitFlyHeight(this.m_units[i], this.m_height, 0.0)
				set i = i + 1
			endloop
		endmethod

		public method height takes nothing returns real
			return this.m_height
		endmethod

		public method setOnEnterFunction takes ALayerOnEnterFunction onEnterFunction returns nothing
			set this.m_onEnterFunction = onEnterFunction
		endmethod

		public method onEnterFunction takes nothing returns ALayerOnEnterFunction
			return this.m_onEnterFunction
		endmethod

		public method setOnLeaveFunction takes ALayerOnLeaveFunction onLeaveFunction returns nothing
			set this.m_onLeaveFunction = onLeaveFunction
		endmethod

		public method onLeaveFunction takes nothing returns ALayerOnLeaveFunction
			return this.m_onLeaveFunction
		endmethod

		public method addEntryRect takes rect whichRect returns nothing
			call RegionAddRect(this.m_entry, whichRect)
		endmethod

		public method removeEntryRect takes rect whichRect returns nothing
			call RegionClearRect(this.m_entry, whichRect)
		endmethod

		public method addExitRect takes rect whichRect returns nothing
			call RegionAddRect(this.m_exit, whichRect)
		endmethod

		public method removeExitRect takes rect whichRect returns nothing
			call RegionClearRect(this.m_exit, whichRect)
		endmethod

		public method addRect takes rect whichRect returns nothing
			call RegionAddRect(this.m_region, whichRect)
		endmethod

		public method removeRect takes rect whichRect returns nothing
			call RegionClearRect(this.m_region, whichRect)
		endmethod

		public method unitEnters takes unit whichUnit returns nothing
			call this.m_units.pushBack(whichUnit)
			call MakeUnitFlyable(whichUnit)
			call SetUnitFlyHeight(whichUnit, this.m_height, 0.0)

			if (this.m_onEnterFunction != 0) then
				call this.m_onEnterFunction.execute(whichUnit)
			endif
		endmethod

		public method unitLeaves takes unit whichUnit returns nothing
			call this.m_units.remove(whichUnit)
			call SetUnitFlyHeight(whichUnit, 0.0, 0.0)

			if (this.m_onLeaveFunction != 0) then
				call this.m_onLeaveFunction.execute(whichUnit)
			endif
		endmethod

		public method unitLeaveAll takes nothing returns nothing
			local integer i

			loop
				exitwhen (this.m_units.empty())
				call this.unitLeaves(this.m_units.back())
			endloop
		endmethod

		public method enable takes nothing returns nothing
			call EnableTrigger(this.m_enterTrigger)
			call EnableTrigger(this.m_leaveTrigger)
			call EnableTrigger(this.m_boundsTrigger)
		endmethod

		/**
		* Does not change already added units!
		*/
		public method disable takes nothing returns nothing
			call DisableTrigger(this.m_enterTrigger)
			call DisableTrigger(this.m_leaveTrigger)
			call DisableTrigger(this.m_boundsTrigger)
		endmethod

		public method isEnabled takes nothing returns boolean
			return IsTriggerEnabled(this.m_boundsTrigger)
		endmethod

		private static method eventFilterEnter takes nothing returns boolean
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")

			return not this.m_units.contains(GetTriggerUnit())
		endmethod

		private static method triggerActionEnter takes nothing returns nothing
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			call this.unitEnters(GetTriggerUnit())
		endmethod

		private static method eventFilterLeave takes nothing returns boolean
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")

			return this.m_units.contains(GetTriggerUnit())
		endmethod

		private static method triggerActionLeave takes nothing returns nothing
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			call this.unitLeaves(GetTriggerUnit())
		endmethod

		private static method eventFilterBounds takes nothing returns boolean
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")

			return this.m_units.contains(GetTriggerUnit()) and not IsUnitInRegion(this.m_exit, GetTriggerUnit())
		endmethod

		private static method triggerActionBounds takes nothing returns nothing
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			call SetUnitX(GetTriggerUnit(), GetUnitPolarProjectionX(GetTriggerUnit(), GetUnitFacing(GetTriggerUnit()) - 180.0, 100.0))
			call SetUnitY(GetTriggerUnit(), GetUnitPolarProjectionY(GetTriggerUnit(), GetUnitFacing(GetTriggerUnit()) - 180.0, 100.0))
		endmethod

		public static method create takes nothing returns thistype
			local thistype this = thistype.allocate()
			set this.m_height = 0.0
			set this.m_onEnterFunction = 0
			set this.m_onLeaveFunction = 0
			set this.m_entry = CreateRegion()
			set this.m_exit = CreateRegion()
			set this.m_region = CreateRegion()
			set this.m_units = AUnitVector.create()
			set this.m_enterTrigger = CreateTrigger()
			call TriggerRegisterEnterRegion(this.m_enterTrigger, this.m_entry, Filter(function thistype.eventFilterEnter))
			call TriggerAddAction(this.m_enterTrigger, function thistype.triggerActionEnter)
			call AHashTable.global().setHandleInteger(this.m_enterTrigger, "this", this)
			set this.m_leaveTrigger = CreateTrigger()
			call TriggerRegisterLeaveRegion(this.m_leaveTrigger, this.m_exit, Filter(function thistype.eventFilterLeave))
			call TriggerAddAction(this.m_leaveTrigger, function thistype.triggerActionLeave)
			call AHashTable.global().setHandleInteger(this.m_leaveTrigger, "this", this)
			set this.m_boundsTrigger = CreateTrigger()
			call TriggerRegisterLeaveRegion(this.m_boundsTrigger, this.m_region, Filter(function thistype.eventFilterBounds))
			call TriggerAddAction(this.m_boundsTrigger, function thistype.triggerActionBounds)

			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			call RemoveRegion(this.m_entry)
			set this.m_entry = null
			call RemoveRegion(this.m_exit)
			set this.m_exit = null
			call RemoveRegion(this.m_region)
			set this.m_region = null
			call this.m_units.destroy()
			call AHashTable.global().destroyTrigger(this.m_enterTrigger)
			set this.m_enterTrigger = null
			call AHashTable.global().destroyTrigger(this.m_leaveTrigger)
			set this.m_enterTrigger = null
			call AHashTable.global().destroyTrigger(this.m_boundsTrigger)
			set this.m_enterTrigger = null
		endmethod
	endstruct

endlibrary