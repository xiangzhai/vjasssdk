library AStructSystemsWorldLayer requires AModuleCoreGeneralSystemStruct, AStructCoreGeneralList, ALibraryCoreDebugMisc, ALibraryCoreMathsHandle, ALibraryCoreMathsUnit

	private struct ARegionData
		private region m_region
		private real m_z

		public method region takes nothing returns region
			return this.m_region
		endmethod

		public method z takes nothing returns real
			return this.m_z
		endmethod

		public static method create takes region whichRegion, real z returns thistype
			local thistype this = thistype.allocate()
			set this.m_region = whichRegion
			set this.m_z = z

			return this
		endmethod
	endstruct

	private struct AUnitData
		private unit m_unit
		private real m_flyHeight

		public method unit takes nothing returns unit
			return this.m_unit
		endmethod

		public method flyHeight takes nothing returns real
			return this.m_flyHeight
		endmethod

		public method restore takes nothing returns nothing
			call SetUnitFlyHeight(this.m_unit, this.m_flyHeight, 0.0)
		endmethod

		public static method create takes unit whichUnit returns thistype
			local thistype this = thistype.allocate()
			set this.m_unit = whichUnit
			set this.m_flyHeight = GetUnitFlyHeight(this.m_unit)

			return this
		endmethod
	endstruct

	/// @todo Should be contained by @struct ALayer, vJass bug.
	function interface ALayerOnEnterFunction takes unit whichUnit returns nothing

	/// @todo Should be contained by @struct ALayer, vJass bug.
	function interface ALayerOnLeaveFunction takes unit whichUnit returns nothing

	/**
	* Layers can be used to create multiple movable rects for units on the same position of map using
	* the z-axis.
	* As you can't do this by using the terrain editor or the Warcraft engine itself this structure
	* uses the native function SetUnitZ and turns off units pathing (by using
	* native function SetUnitPathing) to be usable on water and other blocking regions.
	* Each layer consists of several regions.
	* Each region has its own specific meaning and all regions should be placed in specific order
	* that the layer can be used correctly.
	* This image should describe what each region means to the layer:
	* @image TODO
	* As you can see entry regions have to be in another layer region since you need a z value which will be adjusted on the entering unit (when it enters the region, see note below).
	* Therefore, when a unit enters the layer (entry region) the system will search for the current region with a specified fly height and assign it to the entering unit.
	* Exit regions have to be at the end of a layer region since otherwise the unit will be reset into the region if it does not enter an exit region.
	* As already mentioned, when a unit leaves a layer region it will be reset if it did not enter a new one or an exit region!
	* @note Added regions and units won't be removed from game by the layer at any time! You'll have to take care yourself if you want to prevent memory leaks.
	* @note Region z values will be adjusted when the unit enters a region and are set relatively to the units current z value. Remember that the unit z values won't be refreshed automatically in one single region. Hence, we recommend to place one region for each different z value.
	*/
	struct ALayer
		private ALayerOnEnterFunction m_onEnterFunction
		private ALayerOnLeaveFunction m_onLeaveFunction
		private ARegionList m_entryRegions
		private ARegionList m_exitRegions
		private AIntegerList m_regions // Holds structs of ARegionData.
		private AIntegerList m_units /// Holds structs of AUnitData.
		private trigger m_enterTrigger
		private trigger m_leaveTrigger
		private trigger m_regionTrigger
		private trigger m_boundsTrigger

		implement ASystemStruct

		//! runtextmacro A_STRUCT_DEBUG("\"ALayer\"")

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

		public method addEntryRegion takes region whichRegion returns nothing
			call this.m_entryRegions.pushBack(whichRegion)
			call TriggerRegisterEnterRegion(this.m_enterTrigger, whichRegion, null)
		endmethod

		public method removeEntryRegion takes region whichRegion returns nothing
			call this.m_entryRegions.remove(whichRegion)
		endmethod

		public method addExitRegion takes region whichRegion returns nothing
			call this.m_exitRegions.pushBack(whichRegion)
			call TriggerRegisterEnterRegion(this.m_leaveTrigger, whichRegion, null)
		endmethod

		public method removeExitRegion takes region whichRegion returns nothing
			call this.m_exitRegions.remove(whichRegion)
			/// @todo Remove event
		endmethod

		public method addRegion takes region whichRegion, real z returns nothing
			call this.m_regions.pushBack(ARegionData.create(whichRegion, z))
			call TriggerRegisterEnterRegion(this.m_regionTrigger, whichRegion, null) // for height refresh!
			call TriggerRegisterLeaveRegion(this.m_boundsTrigger, whichRegion, null)
		endmethod

		private method regionDataByRegion takes region whichRegion returns ARegionData
			local AIntegerListIterator iterator = this.m_regions.begin()
			local ARegionData result = 0
			loop
				exitwhen (not iterator.isValid())
				if (ARegionData(iterator.data()).region() == whichRegion) then
					set result = ARegionData(iterator.data())
					exitwhen (true)
				endif
				call iterator.next()
			endloop
			call iterator.destroy()
			return result
		endmethod

		public method removeRegion takes region whichRegion returns nothing
			local ARegionData data = this.regionDataByRegion(whichRegion)
static if (DEBUG_MODE) then
			if (data == 0) then
				call this.print(Format(tr("Region %1% is not contained by layer.")).h(whichRegion).result())
				return
			endif
endif
			call this.m_regions.remove(data)
			call data.destroy()
			/// @todo Remove events
		endmethod

		public stub method onUnitEnters takes unit whichUnit returns nothing
			if (this.m_onEnterFunction != 0) then
				call this.m_onEnterFunction.execute(whichUnit)
			endif
		endmethod

		public stub method onUnitLeaves takes unit whichUnit returns nothing
			if (this.m_onLeaveFunction != 0) then
				call this.m_onLeaveFunction.execute(whichUnit)
			endif
		endmethod

		private method unitRegionData takes unit whichUnit returns ARegionData
			local AIntegerListIterator iterator = this.m_regions.begin()
			local ARegionData result = 0
			loop
				exitwhen (not iterator.isValid())
				if (IsUnitInRegion(ARegionData(iterator.data()).region(), whichUnit)) then
					set result = ARegionData(iterator.data())
					exitwhen (true)
				endif
				call iterator.next()
			endloop
			call iterator.destroy()
			return result
		endmethod

		/**
		* Unit has to be refreshed if it already belongs to layer and enters another layer region or when it enters an entry region.
		*/
		private method refreshUnit takes unit whichUnit, ARegionData data returns nothing
			call SetUnitZ(whichUnit, data.z())
		endmethod

		public method containsUnit takes unit whichUnit returns boolean
			local AIntegerListIterator iterator = this.m_units.begin()
			local boolean result = false
			loop
				exitwhen (not iterator.isValid())
				if (AUnitData(iterator.data()).unit() == whichUnit) then
					set result = true
					exitwhen (true)
				endif
				call iterator.next()
			endloop
			call iterator.destroy()
			return result
		endmethod

		/**
		* @return Returns true if unit could properly newly enter the layer. Otherwise it returns false.
		*/
		public method unitEnters takes unit whichUnit returns boolean
			local ARegionData data = this.unitRegionData(whichUnit)
			if (data == 0) then
				debug call this.print(Format("Unit %1% is not on region of layer.").u(whichUnit).result())
				return false
			endif
			if (this.containsUnit(whichUnit)) then
				debug call this.print(Format("Unit %1% does alread belong to layer.").u(whichUnit).result())
				return false
			endif
			call this.m_units.pushBack(AUnitData.create(whichUnit))
			call MakeUnitFlyable(whichUnit)
			call SetUnitPathing(whichUnit, false)
			call this.refreshUnit(whichUnit, data)
			debug call this.print("Unit " + GetUnitName(whichUnit) + " enters.")
			call this.onUnitEnters(whichUnit)
			return true
		endmethod

		private method unitUnitData takes unit whichUnit returns AUnitData
			local AIntegerListIterator iterator = this.m_units.begin()
			local AUnitData result = 0
			loop
				exitwhen (not iterator.isValid())
				if (AUnitData(iterator.data()).unit() == whichUnit) then
					set result = AUnitData(iterator.data())
					exitwhen (true)
				endif
				call iterator.next()
			endloop
			call iterator.destroy()
			return result
		endmethod

		private method unitLeavesByData takes AUnitData data returns nothing
			local unit whichUnit = data.unit()
			call this.m_units.remove(data)
			call data.restore()
			call data.destroy()
			call SetUnitPathing(whichUnit, true)
			debug call this.print("Unit " + GetUnitName(whichUnit) + " leaves.")

			call this.onUnitLeaves(whichUnit)
			set whichUnit = null
		endmethod

		public method unitLeaves takes unit whichUnit returns boolean
			local AUnitData data = this.unitUnitData(whichUnit)
			if (data == 0) then
				debug call this.print(Format("Unit %1% does not belong to layer.").u(whichUnit).result())
				return false
			endif
			call this.unitLeavesByData(data)
			return true
		endmethod

		public method unitsLeaveAll takes nothing returns nothing
			local AIntegerListIterator iterator = this.m_units.begin()
			loop
				exitwhen (not iterator.isValid())
				call this.unitLeavesByData(AUnitData(iterator.data()))
				call iterator.next()
			endloop
			call iterator.destroy()
		endmethod

		public method enable takes nothing returns nothing
			call EnableTrigger(this.m_enterTrigger)
			call EnableTrigger(this.m_leaveTrigger)
			call EnableTrigger(this.m_boundsTrigger)
		endmethod

		/**
		* Does not change already added/contained units!
		*/
		public method disable takes nothing returns nothing
			call DisableTrigger(this.m_enterTrigger)
			call DisableTrigger(this.m_leaveTrigger)
			call DisableTrigger(this.m_boundsTrigger)
		endmethod

		public method isEnabled takes nothing returns boolean
			return IsTriggerEnabled(this.m_boundsTrigger)
		endmethod

		private method regionContainsUnit takes unit whichUnit returns boolean
			local AIntegerListIterator iterator = this.m_regions.begin()
			local boolean result = false
			loop
				exitwhen (not iterator.isValid())
				if (IsUnitInRegion(ARegionData(iterator.data()).region(), whichUnit)) then
					set result = true
					exitwhen (true)
				endif
				call iterator.next()
			endloop
			call iterator.destroy()
			return result
		endmethod

		private static method triggerConditionEnter takes nothing returns boolean
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			debug call this.print("Run condition with trigger unit " + GetUnitName(GetTriggerUnit()))
			return not this.containsUnit(GetTriggerUnit())
		endmethod

		private static method triggerActionEnter takes nothing returns nothing
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			call this.unitEnters(GetTriggerUnit())
		endmethod

		private static method triggerConditionLeave takes nothing returns boolean
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")

			return this.containsUnit(GetTriggerUnit())
		endmethod

		private static method triggerActionLeave takes nothing returns nothing
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			call this.unitLeaves(GetTriggerUnit())
		endmethod

		private static method triggerConditionRegion takes nothing returns boolean
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")

			return this.containsUnit(GetTriggerUnit())
		endmethod

		private static method triggerActionRegion takes nothing returns nothing
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			local ARegionData data = this.regionDataByRegion(GetTriggeringRegion())
			call this.refreshUnit(GetTriggerUnit(), data)
		endmethod

		private method isUnitInExitRegion takes unit whichUnit returns boolean
			local ARegionData regionData = this.unitRegionData(whichUnit)
static if (DEBUG_MODE) then
			if (regionData == 0) then
				call this.print(Format("Unit %1% is not contained by layer.").u(whichUnit).result())
				return false
			endif
endif
			return this.m_exitRegions.contains(regionData.region())
		endmethod

		private static method triggerConditionBounds takes nothing returns boolean
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")

			return this.containsUnit(GetTriggerUnit()) and not this.isUnitInExitRegion(GetTriggerUnit()) and not this.regionContainsUnit(GetTriggerUnit())
		endmethod

		/// @todo Pause Unit/Move unit back correctly.
		private static method triggerActionBounds takes nothing returns nothing
			local thistype this = AHashTable.global().handleInteger(GetTriggeringTrigger(), "this")
			call SetUnitX(GetTriggerUnit(), GetUnitPolarProjectionX(GetTriggerUnit(), GetUnitFacing(GetTriggerUnit()) - 180.0, 100.0))
			call SetUnitY(GetTriggerUnit(), GetUnitPolarProjectionY(GetTriggerUnit(), GetUnitFacing(GetTriggerUnit()) - 180.0, 100.0))
			debug call this.print("Reset unit " + GetUnitName(GetTriggerUnit()) + " into bounds.")
		endmethod

		public static method create takes nothing returns thistype
			local thistype this = thistype.allocate()
			set this.m_onEnterFunction = 0
			set this.m_onLeaveFunction = 0
			set this.m_entryRegions = ARegionList.create()
			set this.m_exitRegions = ARegionList.create()
			set this.m_regions = AIntegerList.create()
			set this.m_units = AIntegerList.create()

			set this.m_enterTrigger = CreateTrigger()
			call TriggerAddCondition(this.m_enterTrigger, Condition(function thistype.triggerConditionEnter))
			call TriggerAddAction(this.m_enterTrigger, function thistype.triggerActionEnter)
			call AHashTable.global().setHandleInteger(this.m_enterTrigger, "this", this)

			set this.m_leaveTrigger = CreateTrigger()
			call TriggerAddCondition(this.m_leaveTrigger, Condition(function thistype.triggerConditionLeave))
			call TriggerAddAction(this.m_leaveTrigger, function thistype.triggerActionLeave)
			call AHashTable.global().setHandleInteger(this.m_leaveTrigger, "this", this)

			set this.m_regionTrigger = CreateTrigger()
			call TriggerAddCondition(this.m_regionTrigger, Condition(function thistype.triggerConditionRegion))
			call TriggerAddAction(this.m_regionTrigger, function thistype.triggerActionRegion)
			call AHashTable.global().setHandleInteger(this.m_regionTrigger, "this", this)

			set this.m_boundsTrigger = CreateTrigger()
			call TriggerAddCondition(this.m_boundsTrigger, Condition(function thistype.triggerConditionBounds))
			call TriggerAddAction(this.m_boundsTrigger, function thistype.triggerActionBounds)
			call AHashTable.global().setHandleInteger(this.m_boundsTrigger, "this", this)

			return this
		endmethod

		private method destroyRegions takes nothing returns nothing
			local AIntegerListIterator iterator = this.m_regions.begin()
			loop
				exitwhen (not iterator.isValid())
				call ARegionData(iterator.data()).destroy()
				call iterator.next()
			endloop
			call iterator.destroy()
			call this.m_regions.destroy()
		endmethod

		private method destroyUnits takes nothing returns nothing
			local AIntegerListIterator iterator = this.m_units.begin()
			loop
				exitwhen (not iterator.isValid())
				call AUnitData(iterator.data()).destroy()
				call iterator.next()
			endloop
			call iterator.destroy()
			call this.m_units.destroy()
		endmethod

		public method onDestroy takes nothing returns nothing
			call this.unitsLeaveAll()
			call this.m_entryRegions.destroy()
			call this.m_exitRegions.destroy()
			call this.destroyRegions()
			call this.destroyUnits()
			call AHashTable.global().destroyTrigger(this.m_enterTrigger)
			set this.m_enterTrigger = null
			call AHashTable.global().destroyTrigger(this.m_leaveTrigger)
			set this.m_enterTrigger = null
			call AHashTable.global().destroyTrigger(this.m_regionTrigger)
			set this.m_regionTrigger = null
			call AHashTable.global().destroyTrigger(this.m_boundsTrigger)
			set this.m_enterTrigger = null
		endmethod
	endstruct

endlibrary