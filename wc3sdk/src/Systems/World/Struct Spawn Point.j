library AStructSystemsWorldSpawnPoint requires ALibraryCoreDebugMisc, ALibraryCoreGeneralPlayer, ALibraryCoreMathsPoint, ALibraryCoreStringConversion, AStructCoreGeneralHashTable

	/// @struct ASpawnPoint provides the functionality of common creep spawn points, mostly used in RPG maps.
	struct ASpawnPoint
		//static start members
		private static real m_time
		private static string m_effectPath
		private static integer m_dropChance
		private static boolean m_distributeItems
		private static player m_owner
		private static string m_textDistributeItem
		//static member
		private static integer m_dropOwnerId
		//dynamic members
		private integer m_count
		//start members
		private real m_x
		private real m_y
		private real m_range
		//members
		private unitpool m_unitPool
		private itempool m_itemPool
		private trigger m_deathTrigger
		private timer m_spawnTimer
		private group m_unitGroup

		//! runtextmacro A_STRUCT_DEBUG("\"ASpawnPoint\"")

		//dynamic members
		
		public method setCount takes integer count returns nothing
			set this.m_count = count
		endmethod

		public method count takes nothing returns integer
			return this.m_count
		endmethod

		//convenience methods
		
		public method addExistingUnit takes unit existingUnit returns integer
			local integer id = this.m_count
			call GroupAddUnit(this.m_unitGroup, existingUnit)
			set this.m_count = id + 1
			return id
		endmethod
		
		/// Note that after unit @param existingUnit has died there will be spawned a new RANDOM unit from unit pool.
		/// @param weight Weight of added unit type. This value has no effects if @param addType is false.
		/// @return Returns the number of the added unit in the unit group.
		public method addExistingUnitWithType takes unit existingUnit, real weight returns integer
			local integer id = this.addExistingUnit(existingUnit)
			call UnitPoolAddUnitType(this.m_unitPool, GetUnitTypeId(existingUnit), weight)
			return id
		endmethod
		
		//removeExistingUnit
		
		//methods
		
		public method addUnitType takes integer unitTypeId, real weight returns nothing
			call UnitPoolAddUnitType(this.m_unitPool, unitTypeId, weight)
		endmethod
		
		public method removeUnitType takes integer unitTypeId returns nothing
			call UnitPoolRemoveUnitType(this.m_unitPool, unitTypeId)
		endmethod
		
		public method addItemType takes integer itemTypeId, real weight returns nothing
			call ItemPoolAddItemType(this.m_itemPool, itemTypeId, weight)
		endmethod
		
		public method removeItemType takes integer itemTypeId returns nothing
			call ItemPoolRemoveItemType(this.m_itemPool, itemTypeId)
		endmethod

		/// If you want to start a video and there are some spawn points near the scene you can disable them during the video.
		public method enable takes nothing returns nothing
			call EnableTrigger(this.m_deathTrigger)
			if (TimerGetRemaining(this.m_spawnTimer) > 0.0) then
				call PauseTimerBJ(false, this.m_spawnTimer)
			endif
		endmethod

		public method disable takes nothing returns nothing
			call DisableTrigger(this.m_deathTrigger)
			if (TimerGetRemaining(this.m_spawnTimer) > 0.0) then
				call PauseTimerBJ(true, this.m_spawnTimer)
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
			debug if (this.m_unitPool == null) then
				debug call this.print("Unit pool is null.")
				debug return
			debug endif
			debug if (not IsUnitGroupDeadBJ(this.m_unitGroup)) then
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
				set createdUnit = PlaceRandomUnit(this.m_unitPool, thistype.m_owner, x, y, facingAngle)
				call GroupAddUnit(this.m_unitGroup, createdUnit)
				set createdUnit = null
				//Need global, faster?
				if (thistype.m_effectPath != null) then
					set createdEffect = AddSpecialEffect(thistype.m_effectPath, x, y)
					call DestroyEffect(createdEffect)
					set createdEffect = null
				endif
				set i = i + 1
			endloop
		endmethod

		private method dropItem takes unit diedUnit returns nothing
			local integer change
			local item droppedItem
			local player itemOwner
			local integer i
			local player user
			if (this.m_itemPool != null) then
				set change = GetRandomInt(0, 100)
				if (change <= thistype.m_dropChance) then
					set droppedItem = PlaceRandomItem(this.m_itemPool, GetUnitX(diedUnit), GetUnitY(diedUnit))
					call SetItemDropID(droppedItem, GetUnitTypeId(diedUnit))
					if (thistype.m_distributeItems) then
						call thistype.distributeDroppedItem(droppedItem)
					endif
					set droppedItem = null
				endif
			endif
		endmethod

		private static method timerFunctionSpawn takes nothing returns nothing
			local timer expiredTimer = GetExpiredTimer()
			local ASpawnPoint this = AHashTable.global().handleInteger(expiredTimer, "this")
			call this.spawn()
			set expiredTimer = null
		endmethod

		private method startTimer takes nothing returns nothing
			debug if (TimerGetRemaining(this.m_spawnTimer) > 0.0) then
				debug call this.print("Timer has arleady been started.")
				debug return
			debug endif
			call TimerStart(this.m_spawnTimer, thistype.m_time, false, function thistype.timerFunctionSpawn)
		endmethod

		private static method triggerConditionDeath takes nothing returns boolean
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local unit triggerUnit = GetTriggerUnit()
			local ASpawnPoint this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			local boolean result = IsUnitInGroup(triggerUnit, this.m_unitGroup)
			set triggeringTrigger = null
			set triggerUnit = null
			return result
		endmethod

		private static method triggerActionDeath takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local unit triggerUnit = GetTriggerUnit()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			call this.dropItem(triggerUnit)
			if (IsUnitGroupDeadBJ(this.m_unitGroup)) then /// @todo maybe you should check it without ForGroup or this bj fucntion
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
			set this.m_deathTrigger = CreateTrigger()
			set user = Player(PLAYER_NEUTRAL_AGGRESSIVE)
			set triggerEvent = TriggerRegisterPlayerUnitEvent(this.m_deathTrigger, user, EVENT_PLAYER_UNIT_DEATH, null)
			set user = null
			set conditionFunction = Condition(function thistype.triggerConditionDeath)
			set triggerCondition = TriggerAddCondition(this.m_deathTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.m_deathTrigger, function thistype.triggerActionDeath)
			call AHashTable.global().setHandleInteger(this.m_deathTrigger, "this", this)
			set triggerEvent = null
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
		endmethod

		private method createSpawnTimer takes nothing returns nothing
			set this.m_spawnTimer = CreateTimer()
			call AHashTable.global().setHandleInteger(this.m_spawnTimer, "this", this)
		endmethod

		public static method create takes real x, real y, real range returns thistype
			local thistype this = thistype.allocate()
			//start members
			set this.m_x = x
			set this.m_y = y
			set this.m_range = range
			//members
			set this.m_unitPool = CreateUnitPool()
			set this.m_itemPool = CreateItemPool()
			set this.m_unitGroup = CreateGroup()

			call this.createDeathTrigger()
			call this.createSpawnTimer()
			return this
		endmethod
		
		public static method createByExistingUnitWithType takes unit usedUnit, real weight, real x, real y, real range returns thistype
			local thistype this = thistype.create(x, y, range)
			call this.addExistingUnitWithType(usedUnit, weight)
			return this
		endmethod
		
		public static method createByExistingUnitWithTypeAndRect takes unit usedUnit, real weight, rect usedRect, real range returns thistype
			return thistype.createByExistingUnitWithType(usedUnit, weight, GetRectCenterX(usedRect), GetRectCenterY(usedRect), range)
		endmethod
		
		public static method createByExistingUnitWithTypeAndPosition takes unit usedUnit, real weight, real range returns thistype
			local thistype this = thistype.create(GetUnitX(usedUnit), GetUnitY(usedUnit), range)
			call this.addExistingUnitWithType(usedUnit, weight)
			return this
		endmethod

		private method destroyDeathTrigger takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_deathTrigger)
			set this.m_deathTrigger = null
		endmethod

		private method destroySpawnTimer takes nothing returns nothing
			call AHashTable.global().destroyTimer(this.m_spawnTimer)
			set this.m_spawnTimer = null
		endmethod
		
		private static method removeUnit takes nothing returns nothing
			local unit enumUnit = GetEnumUnit()
			call RemoveUnit(enumUnit)
			set enumUnit = null
		endmethod

		/// Removes all contained units.
		public method onDestroy takes nothing returns nothing
			//members
			call DestroyUnitPool(this.m_unitPool)
			set this.m_unitPool = null
			call DestroyItemPool(this.m_itemPool)
			set this.m_itemPool = null
			call ForGroup(this.m_unitGroup, function thistype.removeUnit)
			call DestroyGroup(this.m_unitGroup)
			set this.m_unitGroup = null

			call this.destroyDeathTrigger()
			call this.destroySpawnTimer()
		endmethod

		/**
		* Initializes the spawn point system. Please call this method before using anything of this system.
		* @param time The time which has to elapse before the unit respawns.
		* @param effectPath The path of the effect which is shown when the unit respawns. If this value is null there won't be shown any effect.
		* @param dropChance The chance (percentaged) for dropping items.
		* @param owner The player who owns all spawn point units.
		*/
		public static method init takes real time, string effectPath, integer dropChance, boolean distributeItems, player owner, string textDistributeItem returns nothing
			//static start members
			set thistype.m_time = time
			set thistype.m_effectPath = effectPath
			set thistype.m_dropChance = dropChance
			set thistype.m_distributeItems = distributeItems
			set thistype.m_owner = owner
			set thistype.m_textDistributeItem = textDistributeItem
			//static members
			set thistype.m_dropOwnerId = 0
		endmethod
		
		public static method distributeDroppedItem takes item usedItem returns nothing
			local player itemOwner = thistype.getRandomItemOwner()
			local player user
			local integer i = 0
			call SetItemPlayer(usedItem, itemOwner, true)
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then
					call DisplayTimedTextToPlayer(user, 0.0, 0.0, 6.0, StringArg(StringArg(thistype.m_textDistributeItem, GetItemName(usedItem)), GetPlayerName(itemOwner)))
				endif
				set user = null
				set i = i + 1
			endloop
			set itemOwner = null
		endmethod
		
		private static method getRandomItemOwner takes nothing returns player
			local player user
			local integer oldDropId = thistype.m_dropOwnerId
			set thistype.m_dropOwnerId = thistype.m_dropOwnerId + 1
			loop
				if (thistype.m_dropOwnerId == bj_MAX_PLAYERS) then
					set thistype.m_dropOwnerId = 0
				endif
				set user = Player(thistype.m_dropOwnerId)
				if (IsPlayerPlayingUser(user) or thistype.m_dropOwnerId == oldDropId) then
					return user
				endif
				set user = null
				set thistype.m_dropOwnerId = thistype.m_dropOwnerId + 1
			endloop
			return user
		endmethod
	endstruct

endlibrary