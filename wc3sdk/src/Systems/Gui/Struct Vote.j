library AStructSystemsGuiVote requires AStructCoreGeneralVector, ALibraryCoreGeneralPlayer, AStructSystemsGuiGui

	function interface AVoteResultAction takes AVote vote returns nothing

	/// @todo Should be a part of @struct AVote, vJass bug.
	private function ADialogButtonActionVote takes ADialogButton dialogButton returns nothing
		call AVote.activeVote().addVote(dialogButton.dialog().player(), dialogButton.index())
	endfunction

	/// @todo Maybe votes shouldn't be started for all playing players (player groups).
	struct AVote
		//static members
		private static thistype m_activeVote
		//start members
		private string m_message
		private AVoteResultAction m_resultAction
		//members
		private AStringVector m_choices
		private AIntegerVector m_choiceVotes
		private boolean array m_playerHasVoted[12] /// @todo bj_MAX_PLAYERS
		
		//start members
		
		public method message takes nothing returns string
			return this.m_message
		endmethod
		
		//method
		
		public method start takes nothing returns nothing
			local player user
			local integer i = 0
			local integer j
			set thistype.m_activeVote = this
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set user = Player(i)
				if (IsPlayerPlayingUser(user)) then
					set this.m_playerHasVoted[GetPlayerId(user)] = false
					call AGui.playerGui(user).dialog().clear()
					call AGui.playerGui(user).dialog().setMessage(this.m_message)
					set j = 0
					loop
						exitwhen (j == this.m_choices.size())
						call AGui.playerGui(user).dialog().addDialogButton(this.m_choices[j], 0, ADialogButtonActionVote)
						set j = j + 1
					endloop
					call AGui.playerGui(user).dialog().show()
				endif
				set i = i + 1
			endloop
		endmethod
		
		public method addChoice takes string choice returns integer
			call this.m_choices.pushBack(choice)
			call this.m_choiceVotes.pushBack(0)
			return this.m_choices.backIndex()
		endmethod
		
		public method addVote takes player user, integer choice returns boolean
			local integer i
			local player playingPlayer
			set this.m_choiceVotes[choice] = this.m_choiceVotes[choice] + 1
			/// @todo Show vote message
			set this.m_playerHasVoted[GetPlayerId(user)] = true
			set i = 0
			loop
				exitwhen (i == bj_MAX_PLAYERS)
				set playingPlayer = Player(i)
				if (not this.m_playerHasVoted[i] and IsPlayerPlayingUser(playingPlayer)) then
					return false
				endif
				set playingPlayer = null
				set i = i + 1
			endloop
			call this.m_resultAction.execute(this)
			return true
		endmethod
		
		public method result takes nothing returns integer
			local integer result = 0
			local integer i = 1
			loop
				exitwhen (i == this.m_choiceVotes.size())
				if (this.m_choiceVotes[i] > this.m_choiceVotes[result]) then
					set result = i
				endif
				set i = i + 1
			endloop
			return result
		endmethod
		
		public static method create takes string message, AVoteResultAction resultAction returns thistype
			local thistype this = thistype.allocate()
			//start members
			set this.m_message = message
			set this.m_resultAction = resultAction
			//members
			set this.m_choices = AStringVector.create()
			set this.m_choiceVotes = AIntegerVector.create()
			
			return this
		endmethod
		
		public method onDestroy takes nothing returns nothing
			//members
			call this.m_choices.destroy()
			call this.m_choiceVotes.destroy()
		endmethod
		
		private static method onInit takes nothing returns nothing
			//static members
			set thistype.m_activeVote = 0
		endmethod
		
		public static method activeVote takes nothing returns thistype
			return thistype.m_activeVote
		endmethod
	endstruct

endlibrary