library AStructCoreEnvironmentDamageRecorder requires ALibraryCoreDebugMisc, AStructCoreGeneralHashTable

	/// @todo Should be a part of struct @struct ADamageRecorder, vJass bug.
	function interface ADamageRecorderOnDamageAction takes ADamageRecorder damageRecorder returns nothing

	/**
	* Provides damage recording functionality for a single target.
	* The user is able to get all damage.
	*/
	struct ADamageRecorder
		//dynamic members
		private ADamageRecorderOnDamageAction m_onDamageAction
		//start members
		private unit m_target
		//members
		private AUnitVector m_damageSources
		private ARealVector m_damageAmounts
		private trigger m_damageTrigger
		
		//! runtextmacro A_STRUCT_DEBUG("\"ADamageRecorder\"")
		
		//dynamic members
		
		public method setOnDamageAction takes ADamageRecorderOnDamageAction onDamageAction returns nothing
			set this.m_onDamageAction = onDamageAction
		endmethod
		
		public method onDamageAction takes nothing returns ADamageRecorderOnDamageAction
			return this.m_onDamageAction
		endmethod
		
		//start members
		
		public method target takes nothing returns unit
			return this.m_target
		endmethod
		
		//members
		
		public method damageSource takes integer index returns unit
			debug call this.checkIndex(index)
			return this.m_damageSources[index]
		endmethod
		
		public method damageAmount takes integer index returns real
			debug call this.checkIndex(index)
			return this.m_damageAmounts[index]
		endmethod
		
		public method damageCount takes nothing returns integer
			return this.m_damageSources.size()
		endmethod
		
		//methods
		
		/// Adds all damage amounts and returns the result.
		public method totalDamage takes nothing returns real
			local real result = 0.0
			local integer i = 0
			loop
				exitwhen (i == this.m_damageSources.size())
				set result = result + this.m_damageAmounts[i]
				set i = i + 1
			endloop
			return result
		endmethod
		
		public method enable takes nothing returns nothing
			debug if (IsTriggerEnabled(this.m_damageTrigger)) then
				debug call this.print("Damage trigger has already been enabled before.")
				debug return
			debug endif
			call EnableTrigger(this.m_damageTrigger)
		endmethod
		
		public method disable takes nothing returns nothing
			debug if (not IsTriggerEnabled(this.m_damageTrigger)) then
				debug call this.print("Damage trigger has already been disabled before.")
				debug return
			debug endif
			call DisableTrigger(this.m_damageTrigger)
		endmethod
		
		public method isEnabled takes nothing returns boolean
			return IsTriggerEnabled(this.m_damageTrigger)
		endmethod
		
		public method setEnabled takes boolean enabled returns nothing
			if (enabled) then
				call this.enable()
			else
				call this.disable()
			endif
		endmethod
		
		debug private method checkIndex takes integer index returns nothing
			debug if (index < 0 or index >= this.m_damageSources.size()) then
				debug call this.print("Wrong index: " + I2S(index) + ".")
			debug endif
		debug endmethod
		
		private static method triggerActionDamaged takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			debug if (this.m_damageSources.size() >= AIntegerVector.maxSize()) then
				debug call this.print("Damage source maximum has already been reached.")
			debug endif
			call this.m_damageSources.pushBack(GetEventDamageSource())
			call this.m_damageAmounts.pushBack(GetEventDamage())
			if (this.m_onDamageAction != 0) then
				call this.m_onDamageAction.execute(this)
			endif
			set triggeringTrigger = null
		endmethod
		
		private method createDamageTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_damageTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterUnitEvent(this.m_damageTrigger, this.m_target, EVENT_UNIT_DAMAGED)
			set triggerAction = TriggerAddAction(this.m_damageTrigger, function thistype.triggerActionDamaged)
			call AHashTable.global().setHandleInteger(this.m_damageTrigger, "this", this)
			set triggerEvent = null
			set triggerAction = null
		endmethod
		
		public static method create takes unit target returns thistype
			local thistype this = thistype.allocate()
			debug if (target == null) then
				debug call this.print("Target is null.")
			debug endif
			//dynamic members
			set this.m_onDamageAction = 0
			//start members
			set this.m_target = target
			//members
			set this.m_damageSources = AIntegerVector.create()
			set this.m_damageAmounts = AIntegerVector.create()
			
			call this.createDamageTrigger()
			return this
		endmethod
		
		private method destroyDamageTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_damageTrigger)
			set this.m_damageTrigger = null
		endmethod
		
		public method onDestroy takes nothing returns nothing
			//start members
			set this.m_target = null
			//members
			call this.m_damageSources.destroy()
			call this.m_damageAmounts.destroy()
			
			call this.destroyDamageTrigger()
		endmethod
	endstruct

endlibrary
