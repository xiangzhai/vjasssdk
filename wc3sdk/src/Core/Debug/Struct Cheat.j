/// @author Tamino Dauth
library AStructCoreDebugCheat requires ALibraryCoreDebugMisc, AStructCoreGeneralHashTable, ALibraryCoreGeneralPlayer

	/// @todo Should be a part of @struct ACheat, vJass bug.
	function interface ACheatOnCheatAction takes ACheat cheat returns nothing

	/**
	* ACheat provides a simple cheat functionality. Cheats are string which the player has to enter in chat and which can provide everything the user wants them.
	* Use the function interface @functioninterface ACheatOnCheatAction to specify your cheat action.
	* Note that you can use @function GetEventPlayerChatString() to read the whole entered chat string.
	*/
	struct ACheat
		// construction members
		private string m_cheat
		private boolean m_exactMatch
		private ACheatOnCheatAction m_action
		// members
		private trigger m_cheatTrigger

		//! runtextmacro optional A_STRUCT_DEBUG("\"ACheat\"")

		// construction members

		public method cheat takes nothing returns string
			return this.m_cheat
		endmethod

		public method exactMatch takes nothing returns boolean
			return this.m_exactMatch
		endmethod

		// methods

		public method argument takes nothing returns string
			return SubString(GetEventPlayerChatString(), StringLength(this.m_cheat) + 1, StringLength(GetEventPlayerChatString()))
		endmethod

		private static method triggerActionCheat takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			call this.m_action.execute(this)
			set triggeringTrigger = null
		endmethod

		private method createCheatTrigger takes nothing returns nothing
			local integer i
			local player user
			local event triggerEvent
			local triggeraction triggerAction
			set this.m_cheatTrigger = CreateTrigger()
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then
					set triggerEvent = TriggerRegisterPlayerChatEvent(this.m_cheatTrigger, user, this.m_cheat, this.m_exactMatch)
					set triggerEvent = null
				endif
				set user = null
				set i = i + 1
			endloop
			set triggerAction = TriggerAddAction(this.m_cheatTrigger, function thistype.triggerActionCheat)
			set triggerAction = null
			call AHashTable.global().setHandleInteger(this.m_cheatTrigger, "this", this)
		endmethod

		/**
		* @param cheat The string the player has to enter into the chat.
		* @param exactMatch If this value is false user does not have to enter the exact string of @param cheat to run the cheat. For example if the cheat string is "setlevel" "setlevel 1000" does also work.
		* @param action The function which will be called when player enters cheat string.
		*/
		public static method create takes string cheat, boolean exactMatch, ACheatOnCheatAction action returns thistype
			local thistype this = thistype.allocate()
			debug if (cheat == null) then
				debug call this.print("cheat is empty.")
			debug endif
			debug if (action == 0) then
				debug call this.print("action is 0.")
			debug endif
			// construction members
			set this.m_cheat = cheat
			set this.m_exactMatch = exactMatch
			set this.m_action = action

			call this.createCheatTrigger()
			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_cheatTrigger)
			set this.m_cheatTrigger = null
		endmethod
	endstruct

endlibrary