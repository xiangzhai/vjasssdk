library AStructCoreInterfacePlayerSelection

	struct APlayerSelection
		//start members
		private player m_player
		//members
		private group m_group
		
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
			if (this.m_group != null) then
				call DestroyGroup(this.m_group)
				set this.m_group = null
			endif
			set this.m_group = GetUnitsSelectedAll(this.m_player)
		endmethod
		
		public method restore takes nothing returns nothing
			if (this.m_group == null) then
				call ClearSelectionForPlayer(this.m_player)
			else
				call SelectGroupForPlayerBJ(this.m_group, this.m_player)
			endif
		endmethod
		
		public static method create takes player user returns thistype
			local thistype this = thistype.allocate()
			//start members
			set this.m_player = user
			//members
			set this.m_group = null
			return this
		endmethod
		
		public method onDestroy takes nothing returns nothing
			//start members
			set this.m_player = null
			//members
			if (this.m_group != null) then
				call DestroyGroup(this.m_group)
				set this.m_group = null
			endif
		endmethod
	endstruct

endlibrary