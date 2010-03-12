library AStructSystemsCharacterPvp requires ALibraryCoreInterfaceLeaderboard, ALibraryCoreStringConversion, AStructCoreGeneralHashTable, AStructCoreGeneralList, AStructCoreGeneralVector

	/// @todo Add config options for scoring.
	struct APvp
		// static initialization members
		private static boolean m_useLeaderboard
		private static string m_leaderboardLabelText
		private static string m_enterText
		private static string m_leaveText
		private static string m_killText
		// dynamic members
		private ARealVector m_startX
		private ARealVector m_startY
		private ARealVector m_startFacing
		// members
		private AUnitVector m_units
		private AIntegerVector m_unitScores
		private leaderboard m_leaderboard
		private trigger m_killTrigger

		public method addStartPosition takes real x, real y, real facing returns integer
			call this.m_startX.pushBack(x)
			call this.m_startY.pushBack(y)
			call this.m_startFacing.pushBack(facing)
			return this.m_startFacing.backIndex()
		endmethod

		public method removeStartPosition takes integer index returns boolean
			if (index < 0 or index >= this.m_startX.size()) then
				return false
			endif
			call this.m_startX.erase(index)
			call this.m_startY.erase(index)
			call this.m_startFacing.erase(index)
			return true
		endmethod

		/// @todo Use of player vector would be much easier.
		public method showMessage takes string message returns nothing
			local integer i = 0
			local APlayerList owners = APlayerList.create()
			local player owner
			loop
				exitwhen (i == this.m_units.size())
				set owner = GetOwningPlayer(this.m_units[i])
				if (not owners.contains(owner)) then
					call owners.pushBack(owner)
				endif
				set i = i + 1
			endloop
			loop
				exitwhen (owners.empty())
				call DisplayTimedTextToPlayer(owners.back(), 0.0, 0.0, 6.0, message)
				call owners.popBack()
			endloop
			call owners.destroy()
		endmethod

		/**
		* Note that there has to be a seperated start position for each entered unit.
		*/
		public stub method unitEnters takes unit whichUnit returns integer
			if (this.m_units.size() == this.m_startX.size()) then
				return -1
			endif
			call this.m_units.pushBack(whichUnit)
			call SetUnitX(whichUnit, this.m_startX[this.m_units.backIndex()])
			call SetUnitY(whichUnit, this.m_startY[this.m_units.backIndex()])
			call SetUnitFacing(whichUnit, this.m_startFacing[this.m_units.backIndex()])
			if (thistype.m_useLeaderboard) then
				call LeaderboardAddItem(this.m_leaderboard, GetUnitName(whichUnit), 0, GetOwningPlayer(whichUnit))
				call ShowLeaderboardForPlayer(GetOwningPlayer(whichUnit), this.m_leaderboard, true)
			endif
			call this.showMessage(StringArg(thistype.m_enterText, GetUnitName(whichUnit)))
			return this.m_units.backIndex()
		endmethod

		public method unitLeavesByIndex takes integer index returns nothing
			local string name = GetUnitName(this.m_units[index])
			local player owner = GetOwningPlayer(this.m_units[index])
			local integer i
			local boolean hideLeaderboard
			call this.m_units.erase(index)
			if (thistype.m_useLeaderboard) then
				call LeaderboardRemoveItem(this.m_leaderboard, index)
				set hideLeaderboard = true
				set i = 0
				loop
					exitwhen (i == this.m_units.size())
					if (GetOwningPlayer(this.m_units[i]) == owner) then
						set hideLeaderboard = false
						exitwhen (true)
					endif
					set i = i + 1
				endloop
				if (hideLeaderboard) then
					call ShowLeaderboardForPlayer(owner, this.m_leaderboard, false)
				endif
			endif
			call this.showMessage(StringArg(thistype.m_leaveText, name))
			set owner = null
		endmethod

		public stub method unitLeaves takes unit whichUnit returns nothing
			local integer index = this.m_units.find(whichUnit)
			if (index != -1) then
				call this.unitLeavesByIndex(index)
			endif
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

		/**
		* Virtual method. Reimplement to handle kills.
		*/
		public stub method kill takes unit killingUnit, unit killedUnit returns nothing
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
					if (thistype.m_useLeaderboard) then
						call LeaderboardSetItemValue(this.m_leaderboard, killerIndex, this.m_unitScores[killerIndex])
						call LeaderboardSortItemsByValue(this.m_leaderboard, true)
					endif
					call this.showMessage(StringArg(StringArg(thistype.m_killText, GetUnitName(killingUnit)), GetUnitName(killedUnit)))
					call this.kill(killingUnit, killedUnit)
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
			// dynamic members
			set this.m_startX = ARealVector.create()
			set this.m_startY = ARealVector.create()
			set this.m_startFacing = ARealVector.create()
			// members
			set this.m_units = AUnitVector.create()
			set this.m_unitScores = AIntegerVector.create()
			if (thistype.m_useLeaderboard) then
				set this.m_leaderboard = CreateLeaderboard()
				call LeaderboardSetLabel(this.m_leaderboard, thistype.m_leaderboardLabelText)
				call LeaderboardDisplay(this.m_leaderboard, false)
			endif
			call this.createKillTrigger()

			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			// dynamic members
			call this.m_startX.destroy()
			call this.m_startY.destroy()
			call this.m_startFacing.destroy()
			// members
			call this.m_units.destroy()
			call this.m_unitScores.destroy()
			if (thistype.m_useLeaderboard) then
				call DestroyLeaderboard(this.m_leaderboard)
				set this.m_leaderboard = null
			endif
			call AHashTable.global().destroyTrigger(this.m_killTrigger)
			set this.m_killTrigger = null
		endmethod

		/**
		* @param enterText tr("%s hat die Arena betreten.")
		* @param leaveText tr("%s hat die Arena verlassen.")
		* @param killText tr("%s hat %s getötet.")
		*/
		public static method init takes boolean useLeaderboard, string leaderboardLabelText, string enterText, string leaveText, string killText returns nothing
			// static initialization members
			set thistype.m_useLeaderboard = useLeaderboard
			set thistype.m_leaderboardLabelText = leaderboardLabelText
			set thistype.m_enterText = enterText
			set thistype.m_leaveText = leaveText
			set thistype.m_killText = killText
		endmethod
	endstruct

endlibrary