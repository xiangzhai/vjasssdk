library AStructCoreGeneralGroup requires AStructCoreGeneralVector, ALibraryCoreGeneralUnit

	//! runtextmacro A_VECTOR("private", "AUnitVector", "unit", "null", "100")

	struct AGroup
		//members
		private AUnitVector m_units
		
		//members
		
		public method units takes nothing returns AUnitVector
			return this.m_units
		endmethod
		
		//methods
		
		public method addGroup takes group usedGroup returns nothing
			local unit firstOfGroup
			loop
				exitwhen (IsUnitGroupEmptyBJ(usedGroup))
				set firstOfGroup = FirstOfGroupSave(usedGroup)
				call this.m_units.pushBack(firstOfGroup)
				call GroupRemoveUnit(usedGroup, firstOfGroup)
				set firstOfGroup = null
			endloop
			call DestroyGroup(usedGroup)
			set usedGroup = null
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