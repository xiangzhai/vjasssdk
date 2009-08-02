library AStructSystemsCharacterPvp requires AStructCoreGeneralHashTable, AStructCoreGeneralVector, ALibraryCoreStringConversion

	//! runtextmacro A_VECTOR("private", "AUnitVector", "unit", "null", "100")

	function interface APvpOnEnterAction takes APvp pvp, unit usedUnit returns nothing
	
	function interface APvpOnLeaveAction takes APvp pvp, unit usedUnit returns nothing
	
	function interface APvpOnScoreAction takes APvp pvp, unit scoringUnit, unit dyingUnit, integer newScores returns nothing
	
	/// @todo Finish this struct. Strings should be constant values, add init method, add config options for scoring.
	struct APvp
		//static start members
		private static boolean m_useLeaderboard
		//dynamic members
		private ARealVector m_startX
		private ARealVector m_startY
		private ARealVector m_startFacing
		//members
		private AUnitVector m_units
		private AIntegerVector m_unitScores
		private leaderboard m_leaderboard
		private trigger m_killTrigger
		
		public method unitEnters takes unit usedUnit returns integer
			local player owner
			if (this.m_units.size() == this.m_startX.size()) then
				return -1
			endif
			call this.m_units.pushBack(usedUnit)
			call SetUnitX(usedUnit, this.m_startX[this.m_units.backIndex()])
			call SetUnitY(usedUnit, this.m_startY[this.m_units.backIndex()])
			call SetUnitFacing(usedUnit, this.m_startFacing[this.m_units.backIndex()])
			set owner = GetOwningPlayer(usedUnit)
			call LeaderboardAddItem(this.m_leaderboard, GetUnitName(usedUnit), 0, owner)
			set owner = null
			call this.showMessage(StringArg(tr("%s hat die Arena betreten."), GetUnitName(usedUnit)))
			return this.m_units.backIndex()
		endmethod
		
		public method unitLeaves takes unit usedUnit returns nothing
			local integer index = this.m_units.find(usedUnit)
			call this.m_units.erase(index)
			call LeaderboardRemoveItem(this.m_leaderboard, index)
			call this.showMessage(StringArg(tr("%s hat die Arena verlassen."), GetUnitName(usedUnit)))
		endmethod
		
		/// Unnecessary since @method unitLeaves uses index, too.
		public method unitLeavesByIndex takes integer index returns nothing
		endmethod
		
		public method pause takes boolean pause returns nothing
			local integer i = 0
			loop
				exitwhen (i == this.m_units.size())
				call PauseUnit(this.m_units[i], pause)
				call SetUnitInvulnerable(this.m_units[i], pause)
				set i = i + 1
			endloop
		endmethod
		
		/// @todo Use of player vector would be much easier.
		public method showMessage takes string message returns nothing
			local integer i = 0
			local integer j
			local player array owners
			local integer ownerSize
			local player owner
			loop
				exitwhen (i == this.m_units.size())
				set owner = GetOwningPlayer(this.m_units[i])
				set j = 0
				loop
					exitwhen (j == ownerSize or owners[j] == owner)
					set j = j + 1
				endloop
				if (j == ownerSize) then
					set owners[ownerSize] = owner
					set ownerSize = ownerSize + 1
				endif
				set owner = null
				set i = i + 1
			endloop
			set i = 0
			loop
				exitwhen (i == ownerSize)
				call DisplayTimedTextToPlayer(owners[i], 0.0, 0.0, 6.0, message)
				set owners[i] = null
				set i = i + 1
			endloop
		endmethod
		
		private static method triggerActionKill takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			local unit killedUnit
			local unit killingUnit = GetKillingUnit()
			local integer killerIndex = this.m_units.find(killingUnit)
			if (killerIndex != -1) then
				set killedUnit = GetTriggerUnit()
				if (this.m_units.contains(killedUnit)) then
					set this.m_unitScores[killerIndex] = this.m_unitScores[killerIndex] + 1
					call LeaderboardSetItemValue(this.m_leaderboard, killerIndex, this.m_unitScores[killerIndex])
					call LeaderboardSortItemsByValue(this.m_leaderboard, true)
					call this.showMessage(StringArg(StringArg(tr("%s hat %s getötet."), GetUnitName(killingUnit)), GetUnitName(killedUnit)))
				endif
				set killedUnit = null
			endif
			set killingUnit = null
			set triggeringTrigger = null
		endmethod
		
		private method createKillTrigger takes nothing returns nothing
			local triggeraction triggerAction
			set this.m_killTrigger = CreateTrigger()
			call TriggerRegisterAnyUnitEventBJ(this.m_killTrigger, EVENT_PLAYER_UNIT_DEATH)
			set triggerAction = TriggerAddAction(this.m_killTrigger, function thistype.triggerActionKill)
			call AHashTable.global().setHandleInteger(this.m_killTrigger, "this", this)
			set triggerAction = null
		endmethod
		
		public static method create takes nothing returns thistype
			local thistype this = thistype.allocate()
			//members
			set this.m_units = AUnitVector.create()
			set this.m_unitScores = AIntegerVector.create()
			set this.m_leaderboard = CreateLeaderboard()
			call this.createKillTrigger()
			
			return this
		endmethod
		
		public method onDestroy takes nothing returns nothing
			//members
			call this.m_units.destroy()
			call this.m_unitScores.destroy()
			call DestroyLeaderboard(this.m_leaderboard)
			set this.m_leaderboard = null
			call AHashTable.global().destroyTrigger(this.m_killTrigger)
			set this.m_killTrigger = null
		endmethod
	endstruct

endlibrary