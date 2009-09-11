library ALibraryCoreGeneralPlayer

	/// Checks whether a player is a playing user.
	/// @author Tamino Dauth
	function IsPlayerPlayingUser takes player user returns boolean
		return ((GetPlayerController(user) == MAP_CONTROL_USER) and (GetPlayerSlotState(user) == PLAYER_SLOT_STATE_PLAYING))
	endfunction
	
	/**
	* @author Tamino Dauth
	* @return Returns the number of playing users in game.
	*/
	function CountPlayingUsers takes nothing returns integer
		local integer result = 0
		local player user
		local integer i = 0
		loop
			exitwhen (i == bj_MAX_PLAYERS)
			set user = Player(i)
			if (IsPlayerPlayingUser(user)) then
				set result = result + 1
			endif
			set user = null
			set i= i + 1
		endloop
		return result
	endfunction

	/**
	* @author Tamino Dauth
	* @return Returns the number of playing players in game (without neutral players).
	*/
	function CountPlayingPlayers takes nothing returns integer
		local integer result = 0
		local player whichPlayer
		local integer i = 0
		loop
			exitwhen (i == bj_MAX_PLAYERS)
			set whichPlayer = Player(i)
			if (GetPlayerController(whichPlayer) == MAP_CONTROL_COMPUTER or IsPlayerPlayingUser(whichPlayer)) then
				set result = result + 1
			endif
			set whichPlayer = null
			set i= i + 1
		endloop
		return result
	endfunction

endlibrary