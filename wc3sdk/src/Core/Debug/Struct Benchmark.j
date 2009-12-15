library AStructCoreDebugBenchmark requires AModuleCoreGeneralSystemStruct, AStructCoreGeneralAsl, AStructCoreGeneralVector

	/**
	* This struct can be used for time measurement of important code parts.
	* If you're using the japi it uses specific watch natives for exact time measures.
	* Otherwise it uses the default Warcraft @type timer.
	* @todo Debugging handles doesn't work for handles which are created during map initialization since @method ABenchmark.onInit and @method ABenchmark.init are called later.
	*/
	struct ABenchmark
		//static members
		private static AIntegerVector m_benchmarks
//static if (A_DEBUG_HANDLES) then
		private static boolean m_suspend
		private static AUnitVector m_units
		private static AItemVector m_items
		private static ADestructableVector m_destructables
		private static AEffectVector m_effects
//endif
		//dynamic members
		private string m_identifier
		//members
		private boolean m_isRunning
		private real m_time
static if (A_JAPI) then
		private integer m_stopWatch
else
		private timer m_timer
endif
		private integer m_index

		implement ASystemStruct

		//dynamic members

		public method setIdentifier takes string identifier returns nothing
			set this.m_identifier = identifier
		endmethod

		public method identifier takes nothing returns string
			return this.m_identifier
		endmethod

		//members

		public method isRunning takes nothing returns boolean
			return this.m_isRunning
		endmethod

		public method time takes nothing returns real
			return this.m_time
		endmethod

		//methods

		/// @todo When timer ends it should be started again and elapsed time should be added to member variable.
		public method start takes nothing returns nothing
			set this.m_isRunning = true
			set this.m_time = 0.0
static if (A_JAPI) then
			set this.m_stopWatch = StopWatchCreate()
else
			call TimerStart(this.m_timer, 99999.0, false, null)
endif
		endmethod

		public method stop takes nothing returns nothing
			set this.m_isRunning = false
static if (A_JAPI) then
			set this.m_time = 1000 * StopWatchMark(this.m_stopWatch)
			call StopWatchDestroy(this.m_stopWatch)
			set this.m_stopWatch = -1
else
			set this.m_time = TimerGetElapsed(this.m_timer)
			call PauseTimer(this.m_timer)
endif
		endmethod

		public method show takes nothing returns nothing
			debug call Print(this.m_identifier + ": " + R2S(this.m_time))
		endmethod

		public static method create takes string identifier returns thistype
			local thistype this = thistype.allocate()
			//dynamic members
			set this.m_identifier = identifier
			//members
			set this.m_isRunning = false
			set this.m_time = 0
static if (A_JAPI) then
			set this.m_stopWatch = -1 //0?
else
			set this.m_timer = CreateTimer()
endif
			//static members
			call thistype.m_benchmarks.pushBack(this)
			set this.m_index = thistype.m_benchmarks.backIndex()
			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			//static members
			call thistype.m_benchmarks.erase(this.m_index)
			//members
static if (A_JAPI) then
			if (this.m_stopWatch != -1) then
				call StopWatchDestroy(this.m_stopWatch)
			endif
else
			call DestroyTimer(this.m_timer)
			set this.m_timer = null
endif
		endmethod

		private static method onInit takes nothing returns nothing
			call thistype.setName("ABenchmark")
		endmethod

		public static method init takes nothing returns nothing
			//static members
			set thistype.m_benchmarks = AIntegerVector.create()
static if (A_DEBUG_HANDLES) then
			set thistype.m_suspend = false
			set thistype.m_units = AUnitVector.create()
			set thistype.m_items = AItemVector.create()
			set thistype.m_destructables = ADestructableVector.create()
			set thistype.m_effects = AEffectVector.create()
endif
			call thistype.initialize()
		endmethod

		public static method cleanUp takes nothing returns nothing
			call thistype.uninitialize()
			//static members
			loop
				exitwhen (thistype.m_benchmarks.empty())
				call thistype(thistype.m_benchmarks.back()).destroy()
			endloop
			call thistype.m_benchmarks.destroy()
static if (A_DEBUG_HANDLES) then
			call thistype.m_units.destroy()
			call thistype.m_items.destroy()
			call thistype.m_destructables.destroy()
			call thistype.m_effects.destroy()
endif
		endmethod

		public static method showBenchmarks takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == thistype.m_benchmarks.size())
				call thistype(thistype.m_benchmarks[i]).show()
				set i = i + 1
			endloop
		endmethod

static if (A_DEBUG_HANDLES) then
		public static method showUnits takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == thistype.m_units.size())
				debug call Print(GetUnitName(thistype.m_units[i]))
				set i = i + 1
			endloop
			debug call Print("Total count: " + I2S(thistype.m_units.size()) + ".")
		endmethod

		public static method showItems takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == thistype.m_items.size())
				debug call Print(GetItemName(thistype.m_items[i]))
				set i = i + 1
			endloop
			debug call Print("Total count: " + I2S(thistype.m_items.size()) + ".")
		endmethod

		public static method showDestructables takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == thistype.m_destructables.size())
				debug call Print(GetDestructableName(thistype.m_destructables[i]))
				set i = i + 1
			endloop
			debug call Print("Total count: " + I2S(thistype.m_destructables.size()) + ".")
		endmethod
endif

		public static method showAll takes nothing returns nothing
			call thistype.showBenchmarks()
static if (A_DEBUG_HANDLES) then
			call thistype.showUnits()
			call thistype.showItems()
			call thistype.showDestructables()
endif
		endmethod

		public static method clearAll takes nothing returns nothing
			loop
				exitwhen (thistype.m_benchmarks.empty())
				call thistype(thistype.m_benchmarks.back()).destroy()
				call thistype.m_benchmarks.popBack()
			endloop
		endmethod

static if (A_DEBUG_HANDLES) then
		private static method createUnit takes player id, integer unitid, real x, real y, real face returns nothing
			local unit whichUnit
			if (thistype.initialized() and not thistype.m_suspend) then
				set thistype.m_suspend = true
				set whichUnit = CreateUnit(id, unitid, x, y, face)
				call thistype.m_units.pushBack(whichUnit)
				call RemoveUnit(whichUnit)
				set whichUnit = null
				set thistype.m_suspend = false
			endif
		endmethod

		private static method removeUnit takes unit whichUnit returns nothing
			if (thistype.initialized() and not thistype.m_suspend) then
				call thistype.m_units.remove(whichUnit)
			endif
		endmethod

		private static method createItem takes integer itemid, real x, real y returns nothing
			local item whichItem
			if (thistype.initialized() and not thistype.m_suspend) then
				set thistype.m_suspend = true
				set whichItem = CreateItem(itemid, x, y)
				call thistype.m_items.pushBack(whichItem)
				call RemoveItem(whichItem)
				set whichItem = null
				set thistype.m_suspend = false
			endif
		endmethod

		private static method removeItem takes item whichItem returns nothing
			if (thistype.initialized() and not thistype.m_suspend) then
				call thistype.m_items.remove(whichItem)
			endif
		endmethod

		private static method createDestructable takes integer objectid, real x, real y, real face, real scale, integer variation returns nothing
			local destructable whichDestructable
			if (thistype.initialized() and not thistype.m_suspend) then
				set thistype.m_suspend = true
				set whichDestructable = CreateDestructable(objectid, x, y, face, scale, variation)
				call thistype.m_destructables.pushBack(whichDestructable)
				call RemoveDestructable(whichDestructable)
				set whichDestructable = null
				set thistype.m_suspend = false
			endif
		endmethod

		private static method removeDestructable takes destructable d returns nothing
			if (thistype.initialized() and not thistype.m_suspend) then
				call thistype.m_destructables.remove(d)
			endif
		endmethod
endif
	endstruct

static if (A_DEBUG_HANDLES) then
	debug hook CreateUnit ABenchmark.createUnit
	debug hook RemoveUnit ABenchmark.removeUnit
	debug hook CreateItem ABenchmark.createItem
	debug hook RemoveItem ABenchmark.removeItem
	debug hook CreateDestructable ABenchmark.createDestructable
	debug hook RemoveDestructable ABenchmark.removeDestructable
endif

endlibrary