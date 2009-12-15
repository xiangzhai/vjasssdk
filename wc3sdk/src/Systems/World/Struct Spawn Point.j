library AStructSystemsWorldSpawnPoint requires optional ALibraryCoreDebugMisc, ALibraryCoreGeneralPlayer, ALibraryCoreStringConversion, AStructCoreGeneralGroup, AStructCoreGeneralHashTable, AStructCoreGeneralVector

	private struct ASpawnPointMember
		// dynamic members
		private unitpool m_unitPool
		private itempool m_itemPool
		private real m_x
		private real m_y
		private real m_facing
		// members
		private unit m_unit
		private item m_item

		// dynamic members

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

		public method setX takes real x returns nothing
			set this.m_x = x
		endmethod

		public method x takes nothing returns real
			return this.m_x
		endmethod

		public method setY takes real y returns nothing
			set this.m_y = y
		endmethod

		public method y takes nothing returns real
			return this.m_y
		endmethod

		public method setFacing takes real facing returns nothing
			set this.m_facing = facing
		endmethod

		public method facing takes nothing returns real
			return this.m_facing
		endmethod

		// members

		public method unit takes nothing returns unit
			return this.m_unit
		endmethod

		public method item takes nothing returns item
			return this.m_item
		endmethod

		// methods

		public method placeUnit takes player whichPlayer returns nothing
			local real facing
			if (this.m_facing < 0.0) then
				set facing = GetRandomReal(0.0, 360.0)
			else
				set facing = this.m_facing
			endif
			set this.m_unit = PlaceRandomUnit(this.m_unitPool, whichPlayer, this.m_x, this.m_y, facing)
		endmethod

		public method placeItem takes nothing returns nothing
			set this.m_item = PlaceRandomItem(this.m_itemPool, this.m_x, this.m_y)
		endmethod

		/**
		* @param facing If this value is smaller than 0 it will be random (between 0 and 360 degrees).
		*/
		public static method create takes real x, real y, real facing returns thistype
			local thistype this = thistype.allocate()
			set this.m_unitPool = CreateUnitPool()
			set this.m_itemPool = CreateItemPool()
			set this.m_x = x
			set this.m_y = y
			set this.m_facing = facing
			set this.m_unit = null
			set this.m_item = null

			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			call DestroyUnitPool(this.m_unitPool)
			set this.m_unitPool = null
			call DestroyItemPool(this.m_itemPool)
			set this.m_itemPool = null
			if (this.m_unit != null) then
				call RemoveUnit(this.m_unit)
				set this.m_unit = null
			endif
			if (this.m_item != null) then
				call RemoveItem(this.m_item)
				set this.m_item = null
			endif
		endmethod
	endstruct


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
		//members
		private AIntegerVector m_members
		private AGroup m_group
		private trigger m_deathTrigger
		private timer m_spawnTimer

		//! runtextmacro optional A_STRUCT_DEBUG("\"ASpawnPoint\"")

		// convenience methods

		public method setLocation takes integer memberIndex, location whichLocation returns nothing
			call this.setX(memberIndex, GetLocationX(whichLocation))
			call this.setY(memberIndex, GetLocationY(whichLocation))
		endmethod

		public method setRect takes integer memberIndex, rect whichRect returns nothing
			call this.setX(memberIndex, GetRectCenterX(whichRect))
			call this.setY(memberIndex, GetRectCenterX(whichRect))
		endmethod

		// methods

		public method addMember takes real x, real y, real facing returns integer
			local ASpawnPointMember member = ASpawnPointMember.create(x, y, facing)
			call this.m_members.pushBack(member)
			return this.m_members.backIndex()
		endmethod

		public method removeMember takes integer memberIndex returns nothing
			call this.m_members.erase(memberIndex)
		endmethod

		public method addUnitType takes integer memberIndex, integer unitTypeId, real weight returns nothing
			call ASpawnPointMember(this.m_members[memberIndex]).addUnitType(unitTypeId, weight)
		endmethod

		public method removeUnitType takes integer memberIndex, integer unitTypeId returns nothing
			call ASpawnPointMember(this.m_members[memberIndex]).removeUnitType(unitTypeId)
		endmethod

		public method addItemType takes integer memberIndex, integer itemTypeId, real weight returns nothing
			call ASpawnPointMember(this.m_members[memberIndex]).addItemType(itemTypeId, weight)
		endmethod

		public method removeItemType takes integer memberIndex, integer itemTypeId returns nothing
			call ASpawnPointMember(this.m_members[memberIndex]).removeItemType(itemTypeId)
		endmethod

		public method setX takes integer memberIndex, real x returns nothing
			call ASpawnPointMember(this.m_members[memberIndex]).setX(x)
		endmethod

		public method x takes integer memberIndex returns real
			return ASpawnPointMember(this.m_members[memberIndex]).x()
		endmethod

		public method setY takes integer memberIndex, real y returns nothing
			call ASpawnPointMember(this.m_members[memberIndex]).setY(y)
		endmethod

		public method y takes integer memberIndex returns real
			return ASpawnPointMember(this.m_members[memberIndex]).y()
		endmethod

		public method setFacing takes integer memberIndex, real facing returns nothing
			call ASpawnPointMember(this.m_members[memberIndex]).setFacing(facing)
		endmethod

		public method facing takes integer memberIndex returns real
			return ASpawnPointMember(this.m_members[memberIndex]).facing()
		endmethod

		/**
		* Adds an existing unit to group. If all units of the group has been died, new units will be spawned.
		*/
		public method addUnit takes unit whichUnit returns nothing
			call this.m_group.units().pushBack(whichUnit)
		endmethod

		/**
		* Note that this only works before the group was respawned first time since respawning
		* units means that the old ones will be removed from game (not like hero revivals).
		*/
		public method removeUnit takes unit whichUnit returns nothing
			call this.m_group.units().remove(whichUnit)
		endmethod

		public method clearUnits takes nothing returns nothing
			call this.m_group.units().clear()
		endmethod

		/**
		* Note that after unit @param whichUnit has died there will be spawned a new RANDOM unit from unit pool.
		* @param weight Weight of added unit type. This value has no effects if @param addType is false.
		* @return Returns the index of the added member.
		*/
		public method addUnitWithType takes unit whichUnit, real weight returns integer
			local integer index = this.addMember(GetUnitX(whichUnit), GetUnitY(whichUnit), GetUnitFacing(whichUnit))
			call this.addUnitType(index, GetUnitTypeId(whichUnit), weight)
			call this.addUnit(whichUnit)
			return index
		endmethod

		public method removeUnitWithType takes unit whichUnit returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.m_members.size())
				call this.removeUnitType(i, GetUnitTypeId(whichUnit))
				set i = i + 1
			endloop
			call this.removeUnit(whichUnit)
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

		public method remainingTime takes nothing returns real
			return TimerGetRemaining(this.m_spawnTimer)
		endmethod

		public method spawn takes nothing returns nothing
			local integer i
			local effect whichEffect
			debug if (not this.m_group.units().empty()) then
				debug call this.print("Warning: Unit group is not dead yet.")
			debug endif
			set i = 0
			loop
				exitwhen (i == this.m_members.size())
				call ASpawnPointMember(this.m_members[i]).placeUnit(thistype.m_owner)

				if (ASpawnPointMember(this.m_members[i]).unit() != null) then
					call this.m_group.units().pushBack(ASpawnPointMember(this.m_members[i]).unit())
					// need global, faster?
					if (thistype.m_effectPath != null) then
						set whichEffect = AddSpecialEffect(thistype.m_effectPath, ASpawnPointMember(this.m_members[i]).x(), ASpawnPointMember(this.m_members[i]).y())
						call DestroyEffect(whichEffect)
						set whichEffect = null
					endif
				debug else
					debug call this.print("Warning: Couldn't place unit.")
				endif
				set i = i + 1
			endloop
		endmethod

		private method dropItem takes unit diedUnit returns nothing
			local integer i
			if (GetRandomInt(0, 100) <= thistype.m_dropChance) then
				set i = 0
				loop
					exitwhen (i == this.m_members.size())
					if (diedUnit == ASpawnPointMember(this.m_members[i]).unit()) then
						call ASpawnPointMember(this.m_members[i]).placeItem()
						if (ASpawnPointMember(this.m_members[i]).item() != null and thistype.m_distributeItems) then // item can be null if member has no item types to place
							call thistype.distributeDroppedItem(ASpawnPointMember(this.m_members[i]).item())
						debug else
							debug call this.print("Warning: Couldn't place item.")
						endif
						exitwhen (true)
					endif
					set i = i + 1
				endloop
			endif
		endmethod

		private static method timerFunctionSpawn takes nothing returns nothing
			local timer expiredTimer = GetExpiredTimer()
			local thistype this = AHashTable.global().handleInteger(expiredTimer, "this")
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

		private static method filterFunctionDeath takes nothing returns boolean
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local unit triggerUnit = GetTriggerUnit()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			local boolean result = this.m_group.units().contains(triggerUnit)
			debug if (result) then
				debug call this.print("Contains")
			debug else
				debug call this.print("Does not contain")
			debug endif
			set triggeringTrigger = null
			set triggerUnit = null
			return result
		endmethod

		private static method triggerActionDeath takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local unit triggerUnit = GetTriggerUnit()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			call this.m_group.units().remove(triggerUnit)
			call this.dropItem(triggerUnit)
			debug call this.print("Unit " + GetUnitName(triggerUnit) + " was contained by spawn point.")
			if (this.m_group.units().empty()) then
				debug call this.print("Starting spawn point timer.")
				call this.startTimer()
			endif
			set triggeringTrigger = null
			set triggerUnit = null
		endmethod

		private method createDeathTrigger takes nothing returns nothing
			local filterfunc filterFunction
			local integer i
			local player user
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_deathTrigger = CreateTrigger()
			set filterFunction = Filter(function thistype.filterFunctionDeath)
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				set triggerEvent = TriggerRegisterPlayerUnitEvent(this.m_deathTrigger, user, EVENT_PLAYER_UNIT_DEATH, filterFunction)
				set user = null
				set triggerEvent = null
				set i = i + 1
			endloop
			set triggerAction = TriggerAddAction(this.m_deathTrigger, function thistype.triggerActionDeath)
			call AHashTable.global().setHandleInteger(this.m_deathTrigger, "this", this)
			set filterFunction = null
			set triggerAction = null
		endmethod

		private method createSpawnTimer takes nothing returns nothing
			set this.m_spawnTimer = CreateTimer()
			call AHashTable.global().setHandleInteger(this.m_spawnTimer, "this", this)
		endmethod

		public static method create takes nothing returns thistype
			local thistype this = thistype.allocate()
			//members
			set this.m_members = AIntegerVector.create()
			set this.m_group = AGroup.create()

			call this.createDeathTrigger()
			call this.createSpawnTimer()
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

		/// Removes all contained units.
		public method onDestroy takes nothing returns nothing
			//members
			loop
				exitwhen (this.m_members.empty())
				call ASpawnPointMember(this.m_members.back()).destroy()
				call this.m_members.popBack()
			endloop
			call this.m_members.destroy()
			call this.m_group.destroy()

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

		public static method distributeDroppedItem takes item whichItem returns nothing
			local player itemOwner = thistype.getRandomItemOwner()
			local player user
			local integer i = 0
			call SetItemPlayer(whichItem, itemOwner, true)
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then
					call DisplayTimedTextToPlayer(user, 0.0, 0.0, 6.0, StringArg(StringArg(thistype.m_textDistributeItem, GetItemName(whichItem)), GetPlayerName(itemOwner)))
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