library AStructCoreGeneralHashTable requires optional ALibraryCoreDebugMisc

	/// Provides access to a single hashtable instance.
	/// @author Tamino Dauth
	struct AHashTable
		//static members
		private static AHashTable m_global
		//members
		private hashtable m_hashTable

		//! runtextmacro optional A_STRUCT_DEBUG("\"AHashTable\"")

		//! textmacro AHashTableOperationMacro takes TYPE, TYPENAME, METHODTYPENAME
			public method set$TYPENAME$ takes string key, string label, $TYPE$ value returns nothing
				call Save$METHODTYPENAME$(this.m_hashTable, StringHash(key), StringHash(label), value)
			endmethod

			public method $TYPE$ takes string key, string label returns $TYPE$
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

		//! runtextmacro AHashTableOperationMacro("integer", "Integer", "Integer")
		//! runtextmacro AHashTableOperationMacro("boolean", "Boolean", "Boolean")
		//! runtextmacro AHashTableOperationMacro("real", "Real", "Real")
		//! runtextmacro AHashTableOperationMacro("string", "String", "Str")

		//! textmacro AHashTableHandleOperationMacro takes TYPE, TYPENAME, METHODTYPENAME
			public method set$TYPENAME$ takes string key, string label, $TYPE$ value returns nothing
				call Save$TYPENAME$Handle(this.m_hashTable, StringHash(key), StringHash(label), value)
			endmethod

			public method $METHODTYPENAME$ takes string key, string label returns $TYPE$
				return Load$TYPENAME$Handle(this.m_hashTable, StringHash(key), StringHash(label))
			endmethod

			public method has$TYPENAME$ takes string key, string label returns boolean
				return HaveSavedHandle(this.m_hashTable, StringHash(key), StringHash(label))
			endmethod

			public method remove$TYPENAME$ takes string key, string label returns nothing
				call RemoveSavedHandle(this.m_hashTable, StringHash(key), StringHash(label))
			endmethod

			public method setHandle$TYPENAME$ takes handle usedHandle, string label, $TYPE$ value returns nothing
				call Save$TYPENAME$Handle(this.m_hashTable, GetHandleId(usedHandle), StringHash(label), value)
			endmethod

			public method handle$TYPENAME$ takes handle usedHandle, string label returns $TYPE$
				return Load$TYPENAME$Handle(this.m_hashTable, GetHandleId(usedHandle), StringHash(label))
			endmethod

			public method hasHandle$TYPENAME$ takes handle usedHandle, string label returns boolean
				return HaveSavedHandle(this.m_hashTable, GetHandleId(usedHandle), StringHash(label))
			endmethod

			public method removeHandle$TYPENAME$ takes handle usedHandle, string label returns nothing
				call RemoveSavedHandle(this.m_hashTable, GetHandleId(usedHandle), StringHash(label))
			endmethod
		//! endtextmacro

		//! runtextmacro AHashTableHandleOperationMacro("player", "Player", "player")
		//! runtextmacro AHashTableHandleOperationMacro("widget", "Widget", "widget")
		//! runtextmacro AHashTableHandleOperationMacro("destructable", "Destructable", "destructable")
		//! runtextmacro AHashTableHandleOperationMacro("item", "Item", "item")
		//! runtextmacro AHashTableHandleOperationMacro("unit", "Unit", "unit")
		//! runtextmacro AHashTableHandleOperationMacro("ability", "Ability", "ability")
		//! runtextmacro AHashTableHandleOperationMacro("timer", "Timer", "timer")
		//! runtextmacro AHashTableHandleOperationMacro("trigger", "Trigger", "trigger")
		//! runtextmacro AHashTableHandleOperationMacro("triggercondition", "TriggerCondition", "triggerCondition")
		//! runtextmacro AHashTableHandleOperationMacro("triggeraction", "TriggerAction", "triggerAction")
		//! runtextmacro AHashTableHandleOperationMacro("event", "TriggerEvent", "triggerEvent")
		//! runtextmacro AHashTableHandleOperationMacro("force", "Force", "force")
		//! runtextmacro AHashTableHandleOperationMacro("group", "Group", "group")
		//! runtextmacro AHashTableHandleOperationMacro("location", "Location", "location")
		//! runtextmacro AHashTableHandleOperationMacro("rect", "Rect", "rect")
		//! runtextmacro AHashTableHandleOperationMacro("boolexpr", "BooleanExpr", "boolexpr")
		//! runtextmacro AHashTableHandleOperationMacro("sound", "Sound", "sound")
		//! runtextmacro AHashTableHandleOperationMacro("effect", "Effect", "effect")
		//! runtextmacro AHashTableHandleOperationMacro("unitpool", "UnitPool", "unitPool")
		//! runtextmacro AHashTableHandleOperationMacro("itempool", "ItemPool", "itemPool")
		//! runtextmacro AHashTableHandleOperationMacro("quest", "Quest", "quest")
		//! runtextmacro AHashTableHandleOperationMacro("questitem", "QuestItem", "questitem")
		//! runtextmacro AHashTableHandleOperationMacro("defeatcondition", "DefeatCondition", "defeatCondition")
		//! runtextmacro AHashTableHandleOperationMacro("timerdialog", "TimerDialog", "timerDialog")
		//! runtextmacro AHashTableHandleOperationMacro("leaderboard", "Leaderboard", "leaderboard")
		//! runtextmacro AHashTableHandleOperationMacro("multiboard", "Multiboard", "multiboard")
		//! runtextmacro AHashTableHandleOperationMacro("multiboarditem", "MultiboardItem", "multiboardItem")
		//! runtextmacro AHashTableHandleOperationMacro("trackable", "Trackable", "trackable")
		//! runtextmacro AHashTableHandleOperationMacro("dialog", "Dialog", "dialog")
		//! runtextmacro AHashTableHandleOperationMacro("button", "Button", "button")
		//! runtextmacro AHashTableHandleOperationMacro("texttag", "TextTag", "textTag")
		//! runtextmacro AHashTableHandleOperationMacro("lightning", "Lightning", "lightning")
		//! runtextmacro AHashTableHandleOperationMacro("image", "Image", "image")
		//! runtextmacro AHashTableHandleOperationMacro("ubersplat", "Ubersplat", "ubersplat")
		//! runtextmacro AHashTableHandleOperationMacro("region", "Region", "region")
		//! runtextmacro AHashTableHandleOperationMacro("fogstate", "FogState", "fogState")
		//! runtextmacro AHashTableHandleOperationMacro("fogmodifier", "FogModifier", "fogModifier")
		///! runtextmacro AHashTableHandleOperationMacro("agent", "Agent", "agent") /// @todo Missing native
		//! runtextmacro AHashTableHandleOperationMacro("hashtable", "Hashtable", "hashtable")

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
			call this.flush()
			set this.m_hashTable = null
		endmethod

		private static method onInit takes nothing returns nothing
			//static members
			set AHashTable.m_global = 0
		endmethod

		/**
		* Global hash table is used by Advanced Script Library itself.
		*/
		public static method global takes nothing returns thistype
			if (thistype.m_global == 0) then
				set thistype.m_global = thistype.create()
			endif
			return thistype.m_global
		endmethod
	endstruct

endlibrary