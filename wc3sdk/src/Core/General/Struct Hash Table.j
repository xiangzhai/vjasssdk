library AStructCoreGeneralHashTable requires ALibraryCoreDebugMisc

	/// Provides access to a single hashtable instance.
	/// @author Tamino Dauth
	struct AHashTable
		//static members
		private static AHashTable m_global
		//members
		private hashtable m_hashTable
		
		//! runtextmacro A_STRUCT_DEBUG("\"AHashTable\"")
		
		//! textmacro AHashTableOperationMacro takes TYPE, TYPENAME, METHODNAME, METHODTYPENAME
			public method set$TYPENAME$ takes string key, string label, $TYPE$ value returns nothing
				call Save$METHODTYPENAME$(this.m_hashTable, StringHash(key), StringHash(label), value)
			endmethod
			
			public method $METHODNAME$ takes string key, string label returns $TYPE$
				return Load$METHODTYPENAME$(this.m_hashTable, StringHash(key), StringHash(label))
			endmethod

			public method has$TYPENAME$ takes string key, string label returns boolean
				return HaveSaved$TYPENAME$(this.m_hashTable, StringHash(key), StringHash(label))
			endmethod

			public method remove$TYPENAME$ takes string key, string label returns nothing
				call RemoveSaved$TYPENAME$(this.m_hashTable, StringHash(key), StringHash(label))
			endmethod

			public method setHandle$TYPENAME$ takes handle usedHandle, string label, $TYPE$ value returns nothing
				call Save$METHODTYPENAME$(this.m_hashTable, GetHandleId(usedHandle), StringHash(label), value)
			endmethod
			
			public method handle$TYPENAME$ takes handle usedHandle, string label returns $TYPE$
				return Load$METHODTYPENAME$(this.m_hashTable, GetHandleId(usedHandle), StringHash(label))
			endmethod

			public method hasHandle$TYPENAME$ takes handle usedHandle, string label returns boolean
				return HaveSaved$TYPENAME$(this.m_hashTable, GetHandleId(usedHandle), StringHash(label))
			endmethod

			public method removeHandle$TYPENAME$ takes handle usedHandle, string label returns nothing
				call RemoveSaved$TYPENAME$(this.m_hashTable, GetHandleId(usedHandle), StringHash(label))
			endmethod
		//! endtextmacro

		//! runtextmacro AHashTableOperationMacro("integer", "Integer", "integer", "Integer")
		//! runtextmacro AHashTableOperationMacro("boolean", "Boolean", "boolean", "Boolean")
		//! runtextmacro AHashTableOperationMacro("real", "Real", "real", "Real")
		//! runtextmacro AHashTableOperationMacro("string", "String", "string", "Str")

		/// Flushes all data of the hashtable.
		public method flush takes nothing returns nothing
			call FlushParentHashtable(this.m_hashTable)
		endmethod

		/// Flushes all data of a hashtable key.
		public method flushKey takes string key returns nothing
			call FlushChildHashtable(this.m_hashTable, StringHash(key))
		endmethod

		public method flushHandle takes handle usedHandle returns nothing
			call FlushChildHashtable(this.m_hashTable, GetHandleId(usedHandle))
		endmethod

		//! textmacro AHashTableDestructionMacro takes TYPE, TYPENAME, DESTRUCTIONNAME
			public method destroy$TYPENAME$ takes $TYPE$ $TYPENAME$ returns nothing
				call this.flushHandle($TYPENAME$)
				call $DESTRUCTIONNAME$$TYPENAME$($TYPENAME$)
			endmethod
		//! endtextmacro

		//! runtextmacro AHashTableDestructionMacro("trigger", "Trigger", "Destroy")
		//! runtextmacro AHashTableDestructionMacro("timer", "Timer", "Destroy")
		//! runtextmacro AHashTableDestructionMacro("unit", "Unit", "Remove")
		//! runtextmacro AHashTableDestructionMacro("item", "Item", "Remove")
		//! runtextmacro AHashTableDestructionMacro("destructable", "Destructable", "Remove")

		public static method create takes nothing returns thistype
			local thistype this = thistype.allocate()
			//members
			set this.m_hashTable = InitHashtable()

			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			//members
			call FlushParentHashtable(this.m_hashTable)
			set this.m_hashTable = null
		endmethod
		
		private static method onInit takes nothing returns nothing
			//static members
			set AHashTable.m_global = 0
		endmethod
		
		public static method global takes nothing returns thistype
			if (thistype.m_global == 0) then
				set thistype.m_global = thistype.create()
			endif
			return thistype.m_global
		endmethod
	endstruct

endlibrary