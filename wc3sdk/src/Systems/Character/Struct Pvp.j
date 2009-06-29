library AStructSystemsCharacterPvp requires AStructCoreGeneralVector

	//! runtextmacro A_VECTOR("private", "ARealVector", "real", "0", "100")
	//! runtextmacro A_VECTOR("private", "AUnitVector", "unit", "null", "100")

	function interface APvpOnEnterAction takes APvp pvp, unit usedUnit returns nothing
	
	function interface APvpOnLeaveAction takes APvp pvp, unit usedUnit returns nothing
	
	struct APvp
		//static start members
		private static boolean m_useLeaderboard
		//dynamic members
		private ARealVector m_startX
		private ARealVector m_startY
		private ARealVector m_startFacing
		//members
		private AUnitVector m_units
		private leaderboard m_leaderboard
		
		public method unitEnters takes unit usedUnit returns integer
		endmethod
		
		public method unitLeaves takes unit usedUnit returns nothing
		endmethod
		
		public method unitLeavesByIndex takes integer index returns nothing
		endmethod
		
		
	endstruct

endlibrary