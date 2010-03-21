library AStructSystemsWorldItemSpawnPoint requires AInterfaceSystemsWorldSpawnPointInterface, optional ALibraryCoreDebugMisc, AModuleCoreGeneralSystemStruct, AStructCoreGeneralHashTable, AStructCoreGeneralList, ALibraryCoreMathsPoint

	/**
	* Unfortunately there is no known possibility to register generic item death or pickup events.
	* For this reason there is being created a single timer which checks if any item was moved away from its respawn position.
	* Instead of having a group of members which have all to be dead or removed, item spawn points have to be created for each item uniquely.
	* @see ASpawnPoint
	*/
	struct AItemSpawnPoint extends ASpawnPointInterface
		private static real m_time
		private static real m_removalRange
		private static AIntegerList m_itemSpawnPoints
		private static timer m_respawnTimer
		private real m_x
		private real m_y
		private itempool m_itemPool
		private item m_item
		private timer m_timer
		private boolean m_isEnabled

		implement ASystemStruct

		//! runtextmacro A_STRUCT_DEBUG("\"AItemSpawnPoint\"")

		public method x takes nothing returns real
			return this.m_x
		endmethod

		public method y takes nothing returns real
			return this.m_y
		endmethod

		public method item takes nothing returns item
			return this.m_item
		endmethod

		public method isEnabled takes nothing returns boolean
			return this.m_isEnabled
		endmethod

		public method addItemType takes integer itemTypeId, real weight returns nothing
			call ItemPoolAddItemType(this.m_itemPool, itemTypeId, weight)
		endmethod

		public method removeItemType takes integer itemTypeId returns nothing
			call ItemPoolRemoveItemType(this.m_itemPool, itemTypeId)
		endmethod

		public method remainingTime takes nothing returns real
			if (this.m_timer == null) then
				return 0.0
			endif
			return TimerGetRemaining(this.m_timer)
		endmethod

		public method runs takes nothing returns boolean
			return this.remainingTime() > 0.0
		endmethod

		public method pause takes nothing returns boolean
			if (not this.runs()) then
				return false
			endif
			call PauseTimer(this.m_timer)
			return true
		endmethod

		public method resume takes nothing returns boolean
			if (not this.runs()) then
				return false
			endif
			call ResumeTimer(this.m_timer)
			return true
		endmethod

		public method enable takes nothing returns nothing
			set this.m_isEnabled = true
			call this.resume()
		endmethod

		public method disable takes nothing returns nothing
			set this.m_isEnabled = false
			call this.pause()
		endmethod

		public method spawn takes nothing returns boolean
			if (this.m_item != null) then
				return false
			endif
			debug call this.print("Spawning item")
			set this.m_item = PlaceRandomItem(this.m_itemPool, this.m_x, this.m_y)
			return true
		endmethod

		private static method timerFunctionRespawn takes nothing returns nothing
			local thistype this = AHashTable.global().handleInteger(GetExpiredTimer(), "this")
			call this.spawn()
		endmethod

		private method respawn takes nothing returns nothing
			debug call this.print("Starting item respawn")
			set this.m_item = null
			if (this.m_timer == null) then
				set this.m_timer = CreateTimer()
				call AHashTable.global().setHandleInteger(this.m_timer, "this", this)
			endif
			call TimerStart(this.m_timer, thistype.m_time, false, function thistype.timerFunctionRespawn)
		endmethod

		public static method create takes item whichItem, real weight returns thistype
			local thistype this = thistype.allocate()
			set this.m_x = GetItemX(whichItem)
			set this.m_y = GetItemY(whichItem)
			set this.m_itemPool = CreateItemPool()
			call this.addItemType(GetItemTypeId(whichItem), weight)
			set this.m_item = whichItem
			set this.m_timer = null
			set this.m_isEnabled = true
			call thistype.m_itemSpawnPoints.pushBack(this)

			return this
		endmethod

		/**
		* Respawn will be started immediately after creation.
		*/
		public static method createWithoutItem takes real x, real y returns thistype
			local thistype this = thistype.allocate()
			set this.m_x = x
			set this.m_y = y
			set this.m_itemPool = CreateItemPool()
			set this.m_item = null
			set this.m_timer = null
			set this.m_isEnabled = true
			call thistype.m_itemSpawnPoints.pushBack(this)
			call this.respawn()

			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			call DestroyItemPool(this.m_itemPool)
			set this.m_itemPool = null
			set this.m_item = null
			if (this.m_timer != null) then
				call PauseTimer(this.m_timer)
				call AHashTable.global().destroyTimer(this.m_timer)
				set this.m_timer = null
			endif
			call thistype.m_itemSpawnPoints.remove(this)
		endmethod

		private static method onInit takes nothing returns nothing
			call thistype.setName("AItemSpawnPoint")
		endmethod

		private static method timerFunctionRespawnCheck takes nothing returns nothing
			local AIntegerListIterator iterator = thistype.m_itemSpawnPoints.begin()
			loop
				exitwhen (not iterator.isValid())
				if (not thistype(iterator.data()).runs() and thistype(iterator.data()).m_isEnabled and (thistype(iterator.data()).m_item == null or IsItemOwned(thistype(iterator.data()).m_item) or GetWidgetLife(thistype(iterator.data()).m_item) <= 0.0 or GetDistanceBetweenPoints(GetItemX(thistype(iterator.data()).m_item), GetItemY(thistype(iterator.data()).m_item), 0.0, thistype(iterator.data()).m_x, thistype(iterator.data()).m_y, 0.0) > thistype.m_removalRange)) then
					call thistype(iterator.data()).respawn()
				endif
				call iterator.next()
			endloop
		endmethod

		public static method init takes real checkRate, real time, real removalRange returns nothing
			set thistype.m_time = time
			set thistype.m_removalRange = removalRange
			set thistype.m_itemSpawnPoints = AIntegerList.create()
			set thistype.m_respawnTimer = CreateTimer()
			call TimerStart(thistype.m_respawnTimer, checkRate, true, function thistype.timerFunctionRespawnCheck)
			call thistype.initialize()
		endmethod

		public static method pauseAll takes nothing returns nothing
			local AIntegerListIterator iterator
			debug if (not thistype.check("pauseAll")) then
				debug return
			debug endif
			set iterator = thistype.m_itemSpawnPoints.begin()
			loop
				exitwhen (not iterator.hasNext())
				call thistype(iterator.data()).pause()
				call iterator.next()
			endloop
		endmethod

		public static method resumeAll takes nothing returns nothing
			local AIntegerListIterator iterator
			debug if (not thistype.check("resumeAll")) then
				debug return
			debug endif
			set iterator = thistype.m_itemSpawnPoints.begin()
			loop
				exitwhen (not iterator.hasNext())
				call thistype(iterator.data()).resume()
				call iterator.next()
			endloop
		endmethod

		// do not resume or stop the global timer since user could enable or disable single instances afterwards
		public static method enableAll takes nothing returns nothing
			local AIntegerListIterator iterator
			debug if (not thistype.check("pauseAll")) then
				debug return
			debug endif
			set iterator = thistype.m_itemSpawnPoints.begin()
			loop
				exitwhen (not iterator.hasNext())
				call thistype(iterator.data()).enable()
				call iterator.next()
			endloop
		endmethod

		// do not resume or stop the global timer since user could enable or disable single instances afterwards
		public static method disableAll takes nothing returns nothing
			local AIntegerListIterator iterator
			debug if (not thistype.check("disableAll")) then
				debug return
			debug endif
			set iterator = thistype.m_itemSpawnPoints.begin()
			loop
				exitwhen (not iterator.hasNext())
				call thistype(iterator.data()).disable()
				call iterator.next()
			endloop
		endmethod

		public static method cleanUp takes nothing returns nothing
			local AIntegerListIterator iterator
			debug if (not thistype.check("cleanUp")) then
				debug return
			debug endif
			call PauseTimer(thistype.m_respawnTimer)
			call DestroyTimer(thistype.m_respawnTimer)
			set iterator = thistype.m_itemSpawnPoints.begin()
			loop
				exitwhen (not iterator.hasNext())
				call thistype(iterator.data()).destroy()
				call thistype.m_itemSpawnPoints.popFront()
				call iterator.next()
			endloop
			call thistype.m_itemSpawnPoints.destroy()
			call thistype.uninitialize()
		endmethod
	endstruct
endlibrary