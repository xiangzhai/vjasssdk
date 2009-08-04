library ALibraryCoreEnvironmentLightning

	/**
	* Creates a single lightning which only can be seen by the player "user".
	* @author Tamino Dauth
	* @param user Player who can see the lightning.
	* @param usedCode For furhter information look into the "Splats/LightningData.slk" file of the original Warcraft 3 The Frozen Throne MPQ archives.
	* @param x0 Start x coordinate.
	* @param y0 Start y coordinate.
	* @param z0 Start z coordinate.
	* @param x1 End x coordinate.
	* @param y1 End y coordinate.
	* @param z1 End z coordinate.
	* @return Returns the created lightning.
	*/
	function CreateLightningForPlayer takes player user, string usedCode, real x0, real y0, real z0, real x1, real y1, real z1 returns lightning
		local player localPlayer = GetLocalPlayer()
		local string localCode = ""
		if (user == localPlayer) then
			set localCode = usedCode
		endif
		set localPlayer = null
		return AddLightningEx(localCode, false, x0, y0, z0, x1, y1, z1)
	endfunction

endlibrary