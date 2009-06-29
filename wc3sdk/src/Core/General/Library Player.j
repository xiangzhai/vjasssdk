library ALibraryCoreGeneralPlayer

	/// Checks whether a player is a playing user.
	/// @author Tamino Dauth
	function IsPlayerPlayingUser takes player user returns boolean
		return ((GetPlayerController(user) == MAP_CONTROL_USER) and (GetPlayerSlotState(user) == PLAYER_SLOT_STATE_PLAYING))
	endfunction

endlibrary