library AStructCoreEnvironmentDamageRecorder requires optional ALibraryCoreDebugMisc, AModuleCoreGeneralSystemStruct, AStructCoreGeneralHashTable

	/// @todo Should be a part of struct @struct ADamageRecorder, vJass bug.
	function interface ADamageRecorderOnDamageAction takes ADamageRecorder damageRecorder returns nothing

	/**
	* Provides damage recording functionality for a single unit target.
	* The user is able to get all incurred damage of the unit target.
	* Usually only the total incured damage is stored by using a simple real value.
	* Consider that you can store the single damage sources and their given damage, too.
	* To provide this functionality two vectors are used. Since their maximum could be reached very fast old sources at the beginning of the vectors are discarded when the maximum is reached. Total damage will still be correct!
	* Use ADamageRecorder.saveData to enable source and source values storing.
	*/
	struct ADamageRecorder
		// static construction members
		private static boolean m_useGlobalDamageDetection
		private static ADamageRecorderOnDamageAction m_globalDamageDetectionOnDamageAction
		private static boolean m_saveDataByDefault
		// static members
		private static trigger m_globalDamageDetectionEnterTrigger
		private static trigger m_globalDamageDetectionLeaveTrigger
		// dynamic members
		private ADamageRecorderOnDamageAction m_onDamageAction
		private boolean m_saveData
		// construction members
		private unit m_target
		// members
		private AUnitVector m_damageSources
		private ARealVector m_damageAmounts
		private real m_totalDamage
		private trigger m_damageTrigger

		implement ASystemStruct

		//! runtextmacro optional A_STRUCT_DEBUG("\"ADamageRecorder\"")

		// dynamic members

		public method setOnDamageAction takes ADamageRecorderOnDamageAction onDamageAction returns nothing
			set this.m_onDamageAction = onDamageAction
		endmethod

		public method onDamageAction takes nothing returns ADamageRecorderOnDamageAction
			return this.m_onDamageAction
		endmethod

		/**
		* @param saveData If this value is true damage sources and amounts will be saved.
		*/
		public method setSaveData takes boolean saveData returns nothing
			set this.m_saveData = saveData
		endmethod

		/**
		* @return This value is true by default.
		*/
		public method saveData takes nothing returns boolean
			return this.m_saveData
		endmethod

		// construction members

		public method target takes nothing returns unit
			return this.m_target
		endmethod

		// members

		public method damageSource takes integer index returns unit
			debug if (not this.checkIndex.evaluate(index)) then
				debug return null
			debug endif
			return this.m_damageSources[index]
		endmethod

		public method damageAmount takes integer index returns real
			debug if (not this.checkIndex.evaluate(index)) then
				debug return 0.0
			debug endif
			return this.m_damageAmounts[index]
		endmethod

		/**
		* @return Returns the number of stored damage sources.
		* @see ADamageRecorder.damageSource, ADamageRecorder.damageAmount
		*/
		public method damageCount takes nothing returns integer
			return this.m_damageSources.size()
		endmethod

		/**
		* @return Returns the total incurred damage of the unit target.
		*/
		public method totalDamage takes nothing returns real
			return this.m_totalDamage
		endmethod

		// methods

		/**
		* Use @function GetEventDamageSource and @function GetEventDamage to get event properties.
		* By default this method calls the user defined on damage action (@method ADamageRecorder.onDamageAction).
		*/
		public stub method onSufferDamage takes nothing returns nothing
			if (this.m_onDamageAction != 0) then
				call this.m_onDamageAction.execute(this)
			endif
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

		debug private method checkIndex takes integer index returns boolean
			debug if (index < 0 or index >= this.m_damageSources.size()) then
				debug call this.print("Wrong index: " + I2S(index) + ".")
				debug return false
			debug endif
			debug return true
		debug endmethod

		private static method triggerActionDamaged takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			if (this.m_saveData) then
				if (this.m_damageSources.size() == AIntegerVector.maxSize()) then
					call this.m_damageSources.popFront()
					call this.m_damageAmounts.popFront()
				endif
				call this.m_damageSources.pushBack(GetEventDamageSource())
				call this.m_damageAmounts.pushBack(GetEventDamage())
				set this.m_totalDamage = this.m_totalDamage + GetEventDamage()
			endif
			call this.onSufferDamage()
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
			// dynamic members
			set this.m_onDamageAction = 0
			set this.m_saveData = true
			// construction members
			set this.m_target = target
			// members
			set this.m_damageSources = AUnitVector.create()
			set this.m_damageAmounts = ARealVector.create()

			call this.createDamageTrigger()
			return this
		endmethod

		private method destroyDamageTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_damageTrigger)
			set this.m_damageTrigger = null
		endmethod

		public method onDestroy takes nothing returns nothing
			// construction members
			set this.m_target = null
			// members
			call this.m_damageSources.destroy()
			call this.m_damageAmounts.destroy()

			call this.destroyDamageTrigger()
		endmethod

		private static method onInit takes nothing returns nothing
			call thistype.setName("ADamageRecorder")
		endmethod

		private static method groupFunctionRegister takes nothing returns nothing
			call thistype.registerGlobalUnit.evaluate(GetEnumUnit())
		endmethod

		private static method registerAllUnitsInPlayableMap takes nothing returns nothing
			local group whichGroup = CreateGroup()
			call GroupEnumUnitsInRect(whichGroup, bj_mapInitialPlayableArea, null)
			call ForGroup(whichGroup, function thistype.groupFunctionRegister)
			call DestroyGroup(whichGroup)
			set whichGroup = null
		endmethod

		private static method triggerActionEnter takes nothing returns nothing
			call thistype.registerGlobalUnit.evaluate(GetTriggerUnit())
		endmethod

		private static method triggerActionLeave takes nothing returns nothing
			call thistype.unregisterGlobalUnit.evaluate(GetTriggerUnit())
		endmethod

		/**
		* @param useGlobalDamageDetection If this value is true there will be a global damage detection system which allows you acessing a damage recorder of every unit in map.
		* @param globalDamageDetectionOnDamageAction Use this value to specify a default action which is set for every created global damage recorder.
		* @param saveDataByDefault If this value is true data will be saved by default, otherwise it will be discared.
		* @todo What's about dying units (should be removed from global damage detection? Heroes?!)
		*/
		public static method init takes boolean useGlobalDamageDetection, ADamageRecorderOnDamageAction globalDamageDetectionOnDamageAction, boolean saveDataByDefault returns nothing
			// static construction members
			set thistype.m_useGlobalDamageDetection = useGlobalDamageDetection
			set thistype.m_globalDamageDetectionOnDamageAction = globalDamageDetectionOnDamageAction
			set thistype.m_saveDataByDefault = saveDataByDefault

			if (thistype.m_useGlobalDamageDetection) then
				call thistype.registerAllUnitsInPlayableMap()

				set thistype.m_globalDamageDetectionEnterTrigger = CreateTrigger()
				call TriggerRegisterEnterRectSimple(thistype.m_globalDamageDetectionEnterTrigger, bj_mapInitialPlayableArea) /// @todo Leak
				call TriggerAddAction(thistype.m_globalDamageDetectionEnterTrigger, function thistype.triggerActionEnter)
				set thistype.m_globalDamageDetectionLeaveTrigger = CreateTrigger()
				call TriggerRegisterLeaveRectSimple(thistype.m_globalDamageDetectionLeaveTrigger, bj_mapInitialPlayableArea) /// @todo Leak
				call TriggerRegisterAnyUnitEventBJ(thistype.m_globalDamageDetectionLeaveTrigger, EVENT_PLAYER_UNIT_DEATH)
				call TriggerAddAction(thistype.m_globalDamageDetectionLeaveTrigger, function thistype.triggerActionLeave)
			endif


			call thistype.initialize()
		endmethod

		public static method isGlobalUnitRegistered takes unit whichUnit returns boolean
			debug if (not thistype.m_useGlobalDamageDetection) then
				debug call thistype.staticPrintMethodError("isGlobalUnitRegistered", "Global damage detection is not enabled.")
				debug return false
			debug endif
			return AHashTable.global().hasHandleInteger(whichUnit, "GlobalDamageRecorder")
		endmethod

		public static method registerGlobalUnit takes unit whichUnit returns thistype
			local thistype this
			debug if (not thistype.m_useGlobalDamageDetection) then
				debug call thistype.staticPrintMethodError("registerGlobalUnit", "Global damage detection is not enabled.")
				debug return 0
			debug endif
			if (thistype.isGlobalUnitRegistered(whichUnit)) then
				return 0
			endif
			set this = thistype.create(whichUnit)
			call this.setOnDamageAction(thistype.m_globalDamageDetectionOnDamageAction)
			call this.setSaveData(thistype.m_saveDataByDefault)
			call AHashTable.global().setHandleInteger(whichUnit, "GlobalDamageRecorder", this)
			return this
		endmethod

		public static method unregisterGlobalUnit takes unit whichUnit returns boolean
			debug if (not thistype.m_useGlobalDamageDetection) then
				debug call thistype.staticPrintMethodError("unregisterGlobalUnit", "Global damage detection is not enabled.")
				debug return false
			debug endif
			if (not thistype.isGlobalUnitRegistered(whichUnit)) then
				return false
			endif
			call thistype(AHashTable.global().handleInteger(whichUnit, "GlobalDamageRecorder")).destroy()
			call AHashTable.global().removeHandleInteger(whichUnit, "GlobalDamageRecorder")
			return true
		endmethod

		public static method globalUnitDamageRecorder takes unit whichUnit returns thistype
			debug if (not thistype.m_useGlobalDamageDetection) then
				debug call thistype.staticPrintMethodError("globalUnitDamageRecorder", "Global damage detection is not enabled.")
				debug return 0
			debug endif
			return thistype(AHashTable.global().handleInteger(whichUnit, "GlobalDamageRecorder"))
		endmethod
	endstruct

	hook RemoveUnit ADamageRecorder.unregisterGlobalUnit

endlibrary