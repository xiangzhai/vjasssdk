library AStructSystemsWorldSpawnPoint requires ALibraryCoreDebugMisc, ALibraryCoreMathsPoint, AStructSystemsWorldWorldHashTable

	/// @struct ASpawnPoint provides the functionality of common creep spawn points, mostly used in RPG maps.
	struct ASpawnPoint
		//static start members
		private static real time
		private static string effectPath
		private static integer dropChance
		private static player owner
		//dynamic members
		private integer m_count
		//start members
		private real m_x
		private real m_y
		private real m_range
		//members
		private unitpool unitPool
		private itempool itemPool
		private trigger deathTrigger
		private timer spawnTimer
		private group unitGroup

		//! runtextmacro A_STRUCT_DEBUG("\"ASpawnPoint\"")

		//dynamic members
		
		public method setCount takes integer count returns nothing
			set this.m_count = count
		endmethod

		public method count takes nothing returns integer
			return this.m_count
		endmethod

		//convenience methods
		
		/// Note that after unit @param existingUnit has died there will be spawned a new RANDOM unit from unit pool.
		/// @param addType If this value is true unit type of unit @param existingUnit will be added to unit pool.
		/// @param weight Weight of added unit type. This value has no effects if @param addType is false.
		/// @return Returns the number of the added unit in the unit group.
		public method addExistingUnit takes unit existingUnit, boolean addType, real weight returns integer
			local integer id = this.m_count
			call GroupAddUnit(this.unitGroup, existingUnit)
			set this.m_count = id + 1
			if (addType) then
				call UnitPoolAddUnitType(this.unitPool, GetUnitTypeId(existingUnit), weight)
			endif
			return id
		endmethod
		
		//removeExistingUnit
		
		//methods
		
		public method addUnitType takes integer unitTypeId, real weight returns nothing
			call UnitPoolAddUnitType(this.unitPool, unitTypeId, weight)
		endmethod
		
		public method removeUnitType takes integer unitTypeId returns nothing
			call UnitPoolRemoveUnitType(this.unitPool, unitTypeId)
		endmethod
		
		public method addItemType takes integer itemTypeId, real weight returns nothing
			call ItemPoolAddItemType(this.itemPool, itemTypeId, weight)
		endmethod
		
		public method removeItemType takes integer itemTypeId returns nothing
			call ItemPoolRemoveItemType(this.itemPool, itemTypeId)
		endmethod

		/// If you want to start a video and there are some spawn points near the scene you can disable them during the video.
		public method enable takes nothing returns nothing
			call EnableTrigger(this.deathTrigger)
			if (TimerGetRemaining(this.spawnTimer) > 0.0) then
				call PauseTimerBJ(false, this.spawnTimer)
			endif
		endmethod

		public method disable takes nothing returns nothing
			call DisableTrigger(this.deathTrigger)
			if (TimerGetRemaining(this.spawnTimer) > 0.0) then
				call PauseTimerBJ(true, this.spawnTimer)
			endif
		endmethod

		public method spawn takes nothing returns nothing
			local integer i
			local real angle
			local real range
			local real x
			local real y
			local real facingAngle
			local unit createdUnit
			local effect createdEffect
			debug if (this.unitPool == null) then
				debug call this.print("Unit pool is null.")
				debug return
			debug endif
			debug if (not IsUnitGroupDeadBJ(this.unitGroup)) then
				debug call this.print("Unit group is not dead yet.")
			debug endif
			set i = 0
			loop
				exitwhen (i == this.m_count)
				set angle = GetRandomReal(0.0, 360.0)
				set range = GetRandomReal(0.0, this.m_range)
				set x = GetPolarProjectionX(this.m_x, angle, range)
				set y = GetPolarProjectionY(this.m_y, angle, range)
				set facingAngle = GetRandomReal(0.0, 360.0)
				set createdUnit = PlaceRandomUnit(this.unitPool, ASpawnPoint.owner, x, y, facingAngle)
				call GroupAddUnit(this.unitGroup, createdUnit)
				set createdUnit = null
				//Need global, faster?
				if (ASpawnPoint.effectPath != null) then
					set createdEffect = AddSpecialEffect(ASpawnPoint.effectPath, x, y)
					call DestroyEffect(createdEffect)
					set createdEffect = null
				endif
				set i = i + 1
			endloop
		endmethod

		private method dropItem takes unit diedUnit returns nothing
			local integer change
			local unit triggerUnit
			local item droppedItem
			if (this.itemPool != null) then
				set change = GetRandomInt(0, 100)
				if (change <= ASpawnPoint.dropChance) then
					set droppedItem = PlaceRandomItem(this.itemPool, GetUnitX(diedUnit), GetUnitY(diedUnit))
					set droppedItem = null
				endif
			endif
		endmethod

		private static method timerFunctionSpawn takes nothing returns nothing
			local timer expiredTimer = GetExpiredTimer()
			local ASpawnPoint this = AGetWorldHashTable().getHandleInteger(expiredTimer, "this") //AClassWorldWorldHashTable
			call this.spawn()
			set expiredTimer = null
		endmethod

		private method startTimer takes nothing returns nothing
			debug if (TimerGetRemaining(this.spawnTimer) > 0.0) then
				debug call this.print("Timer has arleady been started.")
				debug return
			debug endif
			call TimerStart(this.spawnTimer, ASpawnPoint.time, false, function ASpawnPoint.timerFunctionSpawn)
		endmethod

		private static method triggerConditionDeath takes nothing returns boolean
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local unit triggerUnit = GetTriggerUnit()
			local ASpawnPoint this = AGetWorldHashTable().getHandleInteger(triggeringTrigger, "this")
			local boolean result = IsUnitInGroup(triggerUnit, this.unitGroup)
			set triggeringTrigger = null
			set triggerUnit = null
			return result
		endmethod

		private static method triggerActionDeath takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local unit triggerUnit = GetTriggerUnit()
			local ASpawnPoint this = AGetWorldHashTable().getHandleInteger(triggeringTrigger, "this")
			call this.dropItem(triggerUnit)
			if (IsUnitGroupDeadBJ(this.unitGroup)) then /// @todo maybe you should check it without ForGroup or this bj fucntion
				call this.startTimer()
			endif
			set triggeringTrigger = null
			set triggerUnit = null
		endmethod

		private method createDeathTrigger takes nothing returns nothing
			local player user
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.deathTrigger = CreateTrigger()
			set user = Player(PLAYER_NEUTRAL_AGGRESSIVE)
			set triggerEvent = TriggerRegisterPlayerUnitEvent(this.deathTrigger, user, EVENT_PLAYER_UNIT_DEATH, null)
			set user = null
			set conditionFunction = Condition(function ASpawnPoint.triggerConditionDeath)
			set triggerCondition = TriggerAddCondition(this.deathTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.deathTrigger, function ASpawnPoint.triggerActionDeath)
			call AGetWorldHashTable().storeHandleInteger(this.deathTrigger, "this", this) //AClassWorldWorldHashTable
			set triggerEvent = null
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		private method createSpawnTimer takes nothing returns nothing
			set this.spawnTimer = CreateTimer()
			call AGetWorldHashTable().storeHandleInteger(this.spawnTimer, "this", this) //AClassWorldWorldHashTable
		endmethod

		public static method create takes real x, real y, real range returns ASpawnPoint
			local ASpawnPoint this = ASpawnPoint.allocate()
			//start members
			set this.m_x = x
			set this.m_y = y
			set this.m_range = range
			//members
			set this.unitPool = CreateUnitPool()
			set this.itemPool = CreateItemPool()
			set this.unitGroup = CreateGroup()

			call this.createDeathTrigger()
			call this.createSpawnTimer()
			return this
		endmethod

		private method destroyDeathTrigger takes nothing returns nothing
			call AGetWorldHashTable().destroyTrigger(this.deathTrigger) //AClassWorldWorldHashTable
			set this.deathTrigger = null
		endmethod

		private method destroySpawnTimer takes nothing returns nothing
			call AGetWorldHashTable().destroyTimer(this.spawnTimer) //AClassWorldWorldHashTable
			set this.spawnTimer = null
		endmethod
		
		private static method removeUnit takes nothing returns nothing
			local unit enumUnit = GetEnumUnit()
			call RemoveUnit(enumUnit)
			set enumUnit = null
		endmethod

		/// Removes all contained units.
		public method onDestroy takes nothing returns nothing
			//members
			call DestroyUnitPool(this.unitPool)
			set this.unitPool = null
			call DestroyItemPool(this.itemPool)
			set this.itemPool = null
			call ForGroup(this.unitGroup, function ASpawnPoint.removeUnit)
			call DestroyGroup(this.unitGroup)
			set this.unitGroup = null

			call this.destroyDeathTrigger()
			call this.destroySpawnTimer()
		endmethod

		/// Initializes the spawn point system. Please call this method before using anything of this system.
		/// @param time The time which has to elapse before the unit respawns.
		/// @param effectPath The path of the effect which is shown when the unit respawns. If this value is null there won't be shown any effect.
		/// @param dropChance The chance (percentaged) for dropping items.
		/// @param owner The player who owns all spawn point units.
		public static method init takes real time, string effectPath, integer dropChance, player owner returns nothing
			//static start members
			set ASpawnPoint.time = time
			set ASpawnPoint.effectPath = effectPath
			set ASpawnPoint.dropChance = dropChance
			set ASpawnPoint.owner = owner
		endmethod
		
		public static method createByExistingUnit takes unit usedUnit, real range, real weight returns ASpawnPoint
			local ASpawnPoint spawnPoint = ASpawnPoint.create(GetUnitX(usedUnit), GetUnitY(usedUnit), range)
			call spawnPoint.addExistingUnit(usedUnit, true, weight)
			return spawnPoint
		endmethod
	endstruct

endlibrary
