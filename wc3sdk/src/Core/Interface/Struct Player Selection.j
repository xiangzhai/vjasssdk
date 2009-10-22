library AStructCoreInterfacePlayerSelection requires optional ALibraryCoreDebugMisc

	struct APlayerSelection
		//start members
		private player m_player
		//members
		private group m_group
		
		//! runtextmacro optional A_STRUCT_DEBUG("\"APlayerSelection\"")
		
		//start members
		
		public method player takes nothing returns player
			return this.m_player
		endmethod
		
		//members
		
		public method group takes nothing returns group
			return this.m_group
		endmethod
		
		//methods
		
		public method save takes nothing returns nothing
			debug call this.print("Saving player selection.")
			call GroupClear(this.m_group)
			call SyncSelections()
			call GroupEnumUnitsSelected(this.m_group, this.m_player, null)
			debug call this.print("Group size " + I2S(CountUnitsInGroup(this.m_group)))
		endmethod
		
		/// @todo Desync
		public method restore takes nothing returns nothing
			debug call this.print("Restoring for player " + GetPlayerName(this.m_player))
			//call SelectGroupForPlayerBJ(this.m_group, this.m_player) /// @todo Desync
		endmethod
		
		public static method create takes player user returns thistype
			local thistype this = thistype.allocate()
			//start members
			set this.m_player = user
			//members
			set this.m_group = CreateGroup()
			return this
		endmethod
		
		public method onDestroy takes nothing returns nothing
			//start members
			set this.m_player = null
			//members
			call DestroyGroup(this.m_group)
			set this.m_group = null
		endmethod
	endstruct

endlibrary