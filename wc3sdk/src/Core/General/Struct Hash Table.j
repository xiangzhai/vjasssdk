library AStructCoreGeneralHashTable requires ALibraryCoreDebugMisc, ALibraryCoreGeneralConversion

	/// Provides access to a single game cache instance.
	/// The game cache will be replaced by the new data type hashtable which comes with the next patch.
	/// @author Tamino Dauth
	struct AHashTable
		//static members
		private static AHashTable m_global
		debug private static integer hashTables //There is a limit of 256 game caches
		//start members
		private gamecache gameCache
		debug private string name
		//member
		//private hashtable m_hashTable
		
		//! runtextmacro A_STRUCT_DEBUG("\"AHashTable\"")

		debug private static method checkName takes string name returns AHashTable
			debug local integer i = 0
			debug loop
				debug exitwhen (i == AHashTable.hashTables)
				debug if (AHashTable(i).name == name) then
					debug return AHashTable(i)
				debug endif
				debug set i = i + 1
			debug endloop
			debug return 0
		debug endmethod

		//! textmacro AHashTableOperationMacro takes TYPE, TYPENAME
			public method store$TYPENAME$ takes string key, string label, $TYPE$ value returns nothing
				call Store$TYPENAME$(this.gameCache, key, label, value)
				//return Save$TYPENAME$(this.m_hashTable, parentKey, childKey, value)
			endmethod

			public method hasStored$TYPENAME$ takes string key, string label returns boolean
				return HaveStored$TYPENAME$(this.gameCache, key, label)
				//return HaveSaved$TYPENAME$(this.m_hashTable, parentKey, childKey)
			endmethod

			public method flushStored$TYPENAME$ takes string key, string label returns nothing
				call FlushStored$TYPENAME$(this.gameCache, key, label)
				//RemoveSaved$TYPENAME$(this.m_hashTable, parentKey, childKey)
			endmethod

			public method getStored$TYPENAME$ takes string key, string label returns $TYPE$
				return GetStored$TYPENAME$(this.gameCache, key, label)
				//return Load$TYPENAME$(this.m_hashTable, parentKey, childKey)
			endmethod

			public method storeHandle$TYPENAME$ takes handle usedHandle, string label, $TYPE$ value returns nothing
				call this.store$TYPENAME$(I2S(H2I(usedHandle)), label, value) //ALibraryGeneralConversion
				//call this.store$TYPENAME$(GetHandleId(usedHandle), key, value)
			endmethod

			public method handle$TYPENAME$Exists takes handle usedHandle, string label returns boolean
				return this.hasStored$TYPENAME$(I2S(H2I(usedHandle)), label) //ALibraryGeneralConversion
				//return this.hasStored$TYPENAME$(GetHandle(usedHandle), key)
			endmethod

			public method flushHandle$TYPENAME$ takes handle usedHandle, string label returns nothing
				call this.flushStored$TYPENAME$(I2S(H2I(usedHandle)), label) //ALibraryGeneralConversion
				//call this.flushedStored(GetHandleId(usedHandle), key)
			endmethod

			public method getHandle$TYPENAME$ takes handle usedHandle, string label returns $TYPE$
				return this.getStored$TYPENAME$(I2S(H2I(usedHandle)), label) //ALibraryGeneralConversion
				//return this.getStored$TYPENAME$(GetHandleId(usedHandle), key)
			endmethod
		//! endtextmacro

		//! runtextmacro AHashTableOperationMacro("integer", "Integer")
		//! runtextmacro AHashTableOperationMacro("boolean", "Boolean")
		//! runtextmacro AHashTableOperationMacro("real", "Real")
		//! runtextmacro AHashTableOperationMacro("string", "String")

		public method storeUnit takes string key, string label, unit usedUnit returns boolean
			return StoreUnit(this.gameCache, key, label, usedUnit)
		endmethod

		public method hasStoredUnit takes string key, string label returns boolean
			return HaveStoredUnit(this.gameCache, key, label)
		endmethod

		public method flushStoredUnit takes string key, string label returns nothing
			call FlushStoredUnit(this.gameCache, key, label)
		endmethod

		public method getStoredUnit takes string key, string label, player user, real x, real y, real facing returns unit
			return RestoreUnit(this.gameCache, key, label, user, x, y, facing)
		endmethod

		/// Löscht alle Daten der Tabelle.
		public method flush takes nothing returns nothing
			call FlushGameCache(this.gameCache)
			//call FlushParentHashtable(this.m_hashTable)
		endmethod

		/// Speichert die Tabelle auf der Festplatte. Nur im Einzelspielermodus möglich.
		public method save takes nothing returns boolean
			return SaveGameCache(this.gameCache)
		endmethod

		public method flushKey takes string key returns nothing
			call FlushStoredMission(this.gameCache, key)
			//call FlushChildHashtable(this.m_hashTable, key)
		endmethod

		public method flushHandleValues takes handle usedHandle returns nothing
			call this.flushKey(I2S(H2I(usedHandle))) //ALibraryGeneralConversion
			//call this.flushKey(GetHandleId(usedHandle))
		endmethod

		//! textmacro AHashTableDestructionMacro takes TYPE, TYPENAME, DESTRUCTIONNAME
			public method destroy$TYPENAME$ takes $TYPE$ $TYPENAME$ returns nothing
				call this.flushHandleValues($TYPENAME$)
				call $DESTRUCTIONNAME$$TYPENAME$($TYPENAME$)
			endmethod
		//! endtextmacro

		//! runtextmacro AHashTableDestructionMacro("trigger", "Trigger", "Destroy")
		//! runtextmacro AHashTableDestructionMacro("timer", "Timer", "Destroy")
		//! runtextmacro AHashTableDestructionMacro("unit", "Unit", "Remove")
		//! runtextmacro AHashTableDestructionMacro("item", "Item", "Remove")
		//! runtextmacro AHashTableDestructionMacro("destructable", "Destructable", "Remove")

		public static method create takes string name returns AHashTable
			local AHashTable this = AHashTable.allocate()
			debug local integer instance = 0
			debug if (AHashTable.hashTables < 256) then
				debug set instance = AHashTable.checkName(name)
				debug if (instance == 0) then
					debug set AHashTable.hashTables = AHashTable.hashTables + 1
				debug else
					debug set this = 0
					debug call AHashTable.staticPrint("Hash table name " + name + " is already used by instance " + I2S(instance) + ".")
				debug endif
			debug else
				debug set this = 0
				debug call AHashTable.staticPrint("Maximum of game caches reached.")
			debug endif
			//start members
			set this.gameCache = InitGameCache(name)
			debug set this.name = name
			//members
			//set this.m_hashTable = InitHashtable()

			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			//start members
			call FlushGameCache(this.gameCache)
			set this.gameCache = null
			//members
			//call FlushParentHashtable(this.m_hashTable)
			//set this.m_hashTable = null
		endmethod
		
		private static method onInit takes nothing returns nothing
			//static members
			set AHashTable.m_global = 0
			debug set AHashTable.hashTables = 0 //There is a limit of 256 game caches
		endmethod
		
		public static method global takes nothing returns AHashTable
			if (AHashTable.m_global == 0) then
				set AHashTable.m_global = AHashTable.create("ASL")
			endif
			return AHashTable.m_global
		endmethod
	endstruct

endlibrary