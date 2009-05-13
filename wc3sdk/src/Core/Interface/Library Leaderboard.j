library ALibraryCoreInterfaceLeaderboard

	/// Shows or hides leaderboard @param usedLeaderboard for player @param user.
	function ShowLeaderboardForPlayer takes player user, leaderboard usedLeaderboard, boolean show returns nothing
		local player localPlayer = GetLocalPlayer()
		call PlayerSetLeaderboard(user, usedLeaderboard)
		if (user == localPlayer) then
			call LeaderboardDisplay(usedLeaderboard, show)
		endif
		set localPlayer = null
	endfunction

endlibrary