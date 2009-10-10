library AStructSystemsCharacterKillQuestItem requires ALibraryCoreStringConversion, AStructSystemsCharacterQuest

	private function stateEventCompleted takes AKillQuestItem questItem, trigger usedTrigger returns nothing
		call TriggerRegisterAnyUnitEventBJ(usedTrigger, EVENT_PLAYER_UNIT_DEATH)
	endfunction
	
	private function stateConditionCompleted takes AKillQuestItem questItem returns boolean
		local unit killer
		local unit triggerUnit
		local boolean result = true
		if (questItem.unitTypeId() != 0) then
			set killer = GetKillingUnit()
			set result = GetUnitTypeId(killer) == questItem.unitTypeId()
			set killer = null
		endif
		if (result and questItem.targetUnitTypeId() != 0) then
			set triggerUnit = GetTriggerUnit()
			set result = GetUnitTypeId(triggerUnit) == questItem.targetUnitTypeId()
			set triggerUnit = null
		endif
		if (result) then
			call questItem.addKill()
			call questItem.quest().displayUpdateMessage(IntegerArg(IntegerArg("%i von i% getötet.", questItem.killed()), questItem.count()))
		endif
		return questItem.killed() == questItem.count()
	endfunction

	/// @state untested
	struct AKillQuestItem extends AQuestItem
		//static start members
		private static string message
		//start members
		private integer m_unitTypeId
		private integer m_targetUnitTypeId
		private integer m_count
		//members
		private integer m_killed
		
		//start members
		
		public method unitTypeId takes nothing returns integer
			return this.m_unitTypeId
		endmethod
		
		public method targetUnitTypeId takes nothing returns integer
			return this.m_targetUnitTypeId
		endmethod
		
		public method count takes nothing returns integer
			return this.m_count
		endmethod
		
		//members
		
		public method killed takes nothing returns integer
			return this.m_killed
		endmethod
		
		public method displayMessage takes nothing returns nothing
			call this.quest().displayUpdateMessage(IntegerArg(IntegerArg(thistype.message, this.m_killed), this.m_count))
		endmethod
		
		/// Friend relation to library functions, do not use!
		public method addKill takes nothing returns nothing
			set this.m_killed = this.m_killed + 1
		endmethod
		
		/**
		* @param unitTypeId If this value is 0 all unit types can kill the target unit type.
		* @param targetUnitTypeId If this value is 0 all unit types can be killed by an unit-type unit.
		*/
		public static method create takes AQuest usedQuest, string description, integer unitTypeId, integer targetUnitTypeId, integer count returns thistype
			local thistype this = thistype.allocate(usedQuest, description)
			//start members
			set this.m_unitTypeId = unitTypeId
			set this.m_targetUnitTypeId = targetUnitTypeId
			set this.m_count = count
			//members
			set this.m_killed = 0
			
			call this.setStateEvent(AAbstractQuest.stateCompleted, stateEventCompleted)
			call this.setStateCondition(AAbstractQuest.stateCompleted, stateConditionCompleted)
			return this
		endmethod
		
		public static method init0 takes string message returns nothing
			//static start members
			set thistype.message = message
		endmethod
	endstruct

endlibrary