library AStructCoreGeneralGroup requires AStructCoreGeneralVector, ALibraryCoreGeneralUnit

	struct AGroup
		//members
		private AUnitVector m_units
		
		//members
		
		public method units takes nothing returns AUnitVector
			return this.m_units
		endmethod
		
		//methods

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