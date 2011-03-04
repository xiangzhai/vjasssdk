library AStructSystemsCharacterBuff requires AModuleCoreGeneralSystemStruct, AStructCoreGeneralHashTable, AStructCoreGeneralVector

	/**
	* Provides acces to buff type which can be added as buff instance several times to one single unit.
	* Buff id should be a custom ability id of an aura which only affects the caster himself to create real buff effect in Warcraft 3.
	*/
	struct ABuff
		implement ASystemStruct

		// static members
		private static AIntegerVector m_buffs
		// construction members
		private integer m_buffId
		// members
		private AUnitVector m_targets
		private integer m_index

		/**
		* Use this method to check whether specific buff effects should be removed from unit.
		*/
		public method count takes unit whichUnit returns integer
			return AHashTable.global().handleInteger(whichUnit, "ABuff" + I2S(this) + "Count")
		endmethod

		public method add takes unit whichUnit returns integer
			local integer count = this.count(whichUnit)
			call this.m_targets.pushBack(whichUnit)
			if (count == 0) then
				call UnitAddAbility(whichUnit, this.m_buffId)
				call UnitMakeAbilityPermanent(whichUnit, true, this.m_buffId) // bleibt auch bei Verwandlungen
			endif
			set count = count + 1
			call AHashTable.global().setHandleInteger(whichUnit, "ABuff" + I2S(this) + "Count", count)
			return this.m_targets.backIndex()
		endmethod

		public method remove takes unit whichUnit returns nothing
			local integer count = this.count(whichUnit)
			call this.m_targets.remove(whichUnit)
			set count = count - 1
			call AHashTable.global().setHandleInteger(whichUnit, "ABuff" + I2S(this) + "Count", count)
			if (count == 0) then
				call AHashTable.global().removeHandleInteger(whichUnit, "ABuff" + I2S(this) + "Count")
				call UnitRemoveAbility(whichUnit, this.m_buffId)
			endif
		endmethod

		public static method create takes integer buffId returns thistype
			local thistype this = thistype.allocate()
			set this.m_buffId = buffId
			set this.m_targets = AUnitVector.create()
			call thistype.m_buffs.pushBack(this)
			set this.m_index = thistype.m_buffs.backIndex()
			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			call this.m_targets.destroy()
			call thistype.m_buffs.erase(this.m_index)
		endmethod

		private static method onInit takes nothing returns nothing
			call thistype.setName("ABuff")
		endmethod

		public static method init takes nothing returns nothing
			call thistype.initialize()
			// static members
			set thistype.m_buffs = AIntegerVector.create()
		endmethod

		public static method cleanUp takes nothing returns nothing
			call thistype.uninitialize()
			// static members
			loop
				exitwhen (thistype.m_buffs.empty())
				call thistype(thistype.m_buffs.back()).destroy()
				call thistype.m_buffs.erase(thistype.m_buffs.backIndex())
			endloop
			call thistype.m_buffs.destroy()
		endmethod

		public static method hookRemoveUnit takes unit whichUnit returns nothing
			local integer i
			if (thistype.initialized()) then
				set i = 0
				loop
					exitwhen (i == thistype.m_buffs.size())
					if (AHashTable.global().hasHandleInteger(whichUnit, "ABuff" + I2S(thistype.m_buffs[i]) + "Count")) then
						call AHashTable.global().removeHandleInteger(whichUnit, "ABuff" + I2S(thistype.m_buffs[i]) + "Count")
					endif
					set i = i + 1
				endloop
			endif
		endmethod
	endstruct

	hook RemoveUnit ABuff.hookRemoveUnit

endlibrary