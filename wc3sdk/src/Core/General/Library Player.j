library ALibraryCoreGeneralPlayer

	/// You have not to set GetTriggerPlayer() to null.
	/// @author Tamino Dauth
	function GetTriggerPlayerId takes nothing returns integer
		local player user = GetTriggerPlayer()
		local integer id = GetPlayerId(user)
		set user = null
		return id
	endfunction

	/// You have not to set GetTriggerUnit() and GetOwningPlayer to null.
	/// @author Tamino Dauth
	function GetOwningPlayerId takes nothing returns integer
		local unit triggerunit = GetTriggerUnit()
		local player owner = GetOwningPlayer(triggerunit)
		local integer id = GetPlayerId(owner)
		set triggerunit = null
		set owner = null
		return id
	endfunction

	/// Checks whether a player is a playing user.
	/// @author Tamino Dauth
	function IsPlayerPlayingUser takes player user returns boolean
		return ((GetPlayerController(user) == MAP_CONTROL_USER) and (GetPlayerSlotState(user) == PLAYER_SLOT_STATE_PLAYING))
	endfunction

endlibrary