library ALibraryCoreEnvironmentTerrainFog

	/**
	* @author dataangel
	* @link http://www.wc3jass.com/
	* @todo Did not test desync yet.
	*/
	function ResetTerrainFogForPlayer takes player usedPlayer returns nothing
		local player localPlayer = GetLocalPlayer()
		if (localPlayer == usedPlayer) then
			call ResetTerrainFog()
		endif
		set localPlayer = null
	endfunction

	/**
	* @author Tamino Dauth
	* @link http://www.mappedia.de/wiki/Tutorial:Gel%C3%A4ndenebel
	*/
	function SetTerrainFogNight takes nothing returns nothing
		call SetTerrainFogEx(0, 0, 5000.0, 0.0, 0.0, 0.0, 0.1961)
	endfunction

	/**
	* @author Tamino Dauth
	* @link http://www.mappedia.de/wiki/Tutorial:Gel%C3%A4ndenebel
	*/
	function SetTerrainFogSunny takes nothing returns nothing
		call SetTerrainFogEx(0, 0, 10000.0, 0.0, 1.0, 1.0, 0.3922)
	endfunction

	/**
	* @author Tamino Dauth
	* @link http://www.mappedia.de/wiki/Tutorial:Gel%C3%A4ndenebel
	*/
	function SetTerrainFogForest takes nothing returns nothing
		call SetTerrainFogEx(0, 0, 5000.0, 0.0, 0.0, 0.3137, 0.1961)
	endfunction

	/**
	* @author Tamino Dauth
	* @link http://www.mappedia.de/wiki/Tutorial:Gel%C3%A4ndenebel
	*/
	function SetTerrainFogFoggy takes nothing returns nothing
		call SetTerrainFogEx(0, 0, 4000.0, 0.0, 0.7843, 0.7843, 0.7843)
	endfunction

endlibrary