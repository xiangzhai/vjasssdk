library AStructCoreGeneralGroup requires AStructCoreGeneralVector, ALibraryCoreGeneralUnit

	struct AGroup
		//members
		private AUnitVector m_units

		//members

		public method units takes nothing returns AUnitVector
			return this.m_units
		endmethod

		// methods

		public method fillGroup takes group whichGroup returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.m_units.size())
				call GroupAddUnit(whichGroup, this.m_units[i])
				set i = i + 1
			endloop
		endmethod

		/**
		* Creates a new Warcraft-3-like group from the group.
		* @return Returns a newly created group.
		*/
		public method group takes nothing returns group
			local group whichGroup = CreateGroup()
			call this.fillGroup(whichGroup)
			return whichGroup
		endmethod

		/**
		* Adds all units of group @param whichGroup to the group.
		* @param destroy If this value is true group @param whichGroup will be destroyed after it has been added.
		* @param clear If this value is true group @param whichGroup will be cleared after it has been added. This value has no effect if destroy is already true. If both parameters are false group @param whichGroup won't change. Unfortunately the method has to re-add all units (limited Warcraft 3 natives).
		*/
		public method addGroup takes group whichGroup, boolean destroy, boolean clear returns nothing
			local unit firstOfGroup
			local integer i
			loop
				exitwhen (IsUnitGroupEmptyBJ(whichGroup))
				set firstOfGroup = FirstOfGroupSave(whichGroup)
				call this.m_units.pushBack(firstOfGroup)
				call GroupRemoveUnit(whichGroup, firstOfGroup)
				set firstOfGroup = null
			endloop
			call GroupClear(whichGroup)
			if (destroy) then
				call DestroyGroup(whichGroup)
				set whichGroup = null
			elseif (not clear) then
				set i = 0
				loop
					exitwhen (i == this.m_units.size())
					call GroupAddUnit(whichGroup, this.m_units[i])
					set i = i + 1
				endloop
			endif
		endmethod

		public method addUnitsOfType takes string unitTypeName, boolexpr filter returns nothing
			local group whichGroup = CreateGroup()
			call GroupEnumUnitsOfType(whichGroup, unitTypeName, filter)
			call this.addGroup(whichGroup, true, false)
			set whichGroup = null
		endmethod

		public method addUnitsOfPlayer takes player whichPlayer, boolexpr filter returns nothing
			local group whichGroup = CreateGroup()
			call GroupEnumUnitsOfPlayer(whichGroup, whichPlayer, filter)
			call this.addGroup(whichGroup, true, false)
			set whichGroup = null
		endmethod

		public method addUnitsOfTypeCounted takes string unitTypeName, boolexpr filter, integer countLimit returns nothing
			local group whichGroup = CreateGroup()
			call GroupEnumUnitsOfTypeCounted(whichGroup, unitTypeName, filter, countLimit)
			call this.addGroup(whichGroup, true, false)
			set whichGroup = null
		endmethod

		public method addUnitsInRect takes rect whichRect, boolexpr filter returns nothing
			local group whichGroup = CreateGroup()
			call GroupEnumUnitsInRect(whichGroup, whichRect, filter)
			call this.addGroup(whichGroup, true, false)
			set whichGroup = null
		endmethod

		public method addUnitsInRectCounted takes rect whichRect, boolexpr filter, integer countLimit returns nothing
			local group whichGroup = CreateGroup()
			call GroupEnumUnitsInRectCounted(whichGroup, whichRect, filter, countLimit)
			call this.addGroup(whichGroup, true, false)
			set whichGroup = null
		endmethod

		public method addUnitsInRange takes real x, real y, real radius, boolexpr filter returns nothing
			local group whichGroup = CreateGroup()
			call GroupEnumUnitsInRange(whichGroup, x, y, radius, filter)
			call this.addGroup(whichGroup, true, false)
			set whichGroup = null
		endmethod

		public method addUnitsInRangeOfLocation takes location whichLocation, real radius, boolexpr filter returns nothing
			local group whichGroup = CreateGroup()
			call GroupEnumUnitsInRangeOfLoc(whichGroup, whichLocation, radius, filter)
			call this.addGroup(whichGroup, true, false)
			set whichGroup = null
		endmethod

		public method addUnitsInRangeCounted takes real x, real y, real radius, boolexpr filter, integer countLimit returns nothing
			local group whichGroup = CreateGroup()
			call GroupEnumUnitsInRangeCounted(whichGroup, x, y, radius, filter, countLimit)
			call this.addGroup(whichGroup, true, false)
			set whichGroup = null
		endmethod

		public method addUnitsInRangeOfLocationCounted takes location whichLocation, real radius, boolexpr filter, integer countLimit returns nothing
			local group whichGroup = CreateGroup()
			call GroupEnumUnitsInRangeOfLocCounted(whichGroup, whichLocation, radius, filter, countLimit)
			call this.addGroup(whichGroup, true, false)
			set whichGroup = null
		endmethod

		public method addUnitsSelected takes player whichPlayer, boolexpr filter returns nothing
			local group whichGroup = CreateGroup()
			call GroupEnumUnitsSelected(whichGroup, whichPlayer, filter)
			call this.addGroup(whichGroup, true, false)
			set whichGroup = null
		endmethod

		public method immediateOrder takes string order returns boolean
			local group whichGroup = this.group()
			local boolean result = GroupImmediateOrder(whichGroup, order)
			call DestroyGroup(whichGroup)
			set whichGroup = null
			return result
		endmethod

		public method immediateOrderById takes integer order returns boolean
			local group whichGroup = this.group()
			local boolean result = GroupImmediateOrderById(whichGroup, order)
			call DestroyGroup(whichGroup)
			set whichGroup = null
			return result
		endmethod

		public method pointOrder takes string order, real x, real y returns boolean
			local group whichGroup = this.group()
			local boolean result = GroupPointOrder(whichGroup, order, x, y)
			call DestroyGroup(whichGroup)
			set whichGroup = null
			return result
		endmethod

		public method locationOrder takes string order, location whichLocation returns boolean
			local group whichGroup = CreateGroup()
			local boolean result = GroupPointOrderLoc(whichGroup, order, whichLocation)
			call DestroyGroup(whichGroup)
			set whichGroup = null
			return result
		endmethod

		public method pointOrderById takes integer order, real x, real y returns boolean
			local group whichGroup = CreateGroup()
			local boolean result = GroupPointOrderById(whichGroup, order, x, y)
			call DestroyGroup(whichGroup)
			set whichGroup = null
			return result
		endmethod

		public method locationOrderById takes integer order, location whichLocation returns boolean
			local group whichGroup = CreateGroup()
			local boolean result = GroupPointOrderByIdLoc(whichGroup, order, whichLocation)
			call DestroyGroup(whichGroup)
			set whichGroup = null
			return result
		endmethod

		public method targetOrder takes string order, widget targetWidget returns boolean
			local group whichGroup = CreateGroup()
			local boolean result = GroupTargetOrder(whichGroup, order, targetWidget)
			call DestroyGroup(whichGroup)
			set whichGroup = null
			return result
		endmethod

		public method targetOrderById takes integer order, widget targetWidget returns boolean
			local group whichGroup = CreateGroup()
			local boolean result = GroupTargetOrderById(whichGroup, order, targetWidget)
			call DestroyGroup(whichGroup)
			set whichGroup = null
			return result
		endmethod

		public method isDead takes nothing returns boolean
			local integer i = 0
			loop
				exitwhen (i == this.m_units.size())
				if (not IsUnitDeadBJ(this.m_units[i])) then
					return false
				endif
				set i = i + 1
			endloop
			return true
		endmethod

		public method hasUnitsOfPlayer takes player whichPlayer returns boolean
			local player owner
			local integer i = 0
			loop
				exitwhen (i == this.m_units.size())
				set owner = GetOwningPlayer(this.m_units[i])
				if (owner == whichPlayer) then
					set owner = null
					return true
				endif
				set i = i + 1
				set owner = null
			endloop
			return false
		endmethod

		public method removeUnitsOfPlayer takes player whichPlayer returns nothing
			local player owner
			local integer i = 0
			loop
				exitwhen (i == this.m_units.size())
				set owner = GetOwningPlayer(this.m_units[i])
				if (owner == whichPlayer) then
					call this.m_units.erase(i)
				else
					set i = i + 1
				endif
				set owner = null
			endloop
		endmethod

		public method hasAlliesOfPlayer takes player whichPlayer returns boolean
			local player owner
			local integer i = 0
			loop
				exitwhen (i == this.m_units.size())
				set owner = GetOwningPlayer(this.m_units[i])
				if (IsPlayerAlly(owner, whichPlayer)) then
					set owner = null
					return true
				endif
				set i = i + 1
				set owner = null
			endloop
			return false
		endmethod

		public method hasAlliesOfUnit takes unit whichUnit returns boolean
			local player owner = GetOwningPlayer(whichUnit)
			local boolean result = this.hasAlliesOfPlayer(owner)
			set owner = null
			return result
		endmethod

		public method removeAlliesOfPlayer takes player whichPlayer returns nothing
			local player owner
			local integer i = 0
			loop
				exitwhen (i == this.m_units.size())
				set owner = GetOwningPlayer(this.m_units[i])
				if (IsPlayerAlly(owner, whichPlayer)) then
					call this.m_units.erase(i)
				else
					set i = i + 1
				endif
				set owner = null
			endloop
		endmethod

		public method removeAlliesOfUnit takes unit whichUnit returns nothing
			local player owner = GetOwningPlayer(whichUnit)
			call this.removeAlliesOfPlayer(owner)
			set owner = null
		endmethod

		public method hasEnemiesOfPlayer takes player whichPlayer returns boolean
			local player owner
			local integer i = 0
			loop
				exitwhen (i == this.m_units.size())
				set owner = GetOwningPlayer(this.m_units[i])
				if (not IsPlayerAlly(owner, whichPlayer)) then
					set owner = null
					return true
				endif
				set i = i + 1
				set owner = null
			endloop
			return false
		endmethod

		public method hasEnemiesOfUnit takes unit whichUnit returns boolean
			local player owner = GetOwningPlayer(whichUnit)
			local boolean result = this.hasEnemiesOfPlayer(owner)
			set owner = null
			return result
		endmethod

		public method removeEnemiesOfPlayer takes player whichPlayer returns nothing
			local player owner
			local integer i = 0
			loop
				exitwhen (i == this.m_units.size())
				set owner = GetOwningPlayer(this.m_units[i])
				if (not IsPlayerAlly(owner, whichPlayer)) then
					call this.m_units.erase(i)
				else
					set i = i + 1
				endif
				set owner = null
			endloop
		endmethod

		public method removeEnemiesOfUnit takes unit whichUnit returns nothing
			local player owner = GetOwningPlayer(whichUnit)
			call this.removeEnemiesOfPlayer(owner)
			set owner = null
		endmethod

		public static method create takes nothing returns thistype
			local thistype this = thistype.allocate()
			//members
			set this.m_units = AUnitVector.create()
			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			//members
			call this.m_units.destroy()
		endmethod
	endstruct

endlibrary