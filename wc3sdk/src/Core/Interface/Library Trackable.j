library ALibraryCoreInterfaceTrackable

	/// Auf WC3Jass.com wurde die lokale Erzeugung ebenfalls so angewandt.
	function CreateTrackableForPlayer takes player user, string modelPath, real x, real y, real facingAngle returns trackable
		local player localPlayer = GetLocalPlayer()
		local string localPath = ""
		if (user == localPlayer) then
			set localPath = modelPath
		endif
		set localPlayer = null
		return CreateTrackable(localPath, x, y, facingAngle)
	endfunction

	/**
	* This creates a trackable at the given coordinates that floats above the ground with the height specified by z.
	* It works by creating an invisible platform, creating the trackable and removing the platform again.
	* This function is extra since it uses more memory and isn't always required.
	* @author KaTTaNa
	* @source http://www.wc3jass.com/
	*/
	function CreateTrackableForPlayerZ takes player whichPlayer, string modelPath, real x, real y, real z, real facingAngle returns trackable
		local destructable heightDestructable = CreateDestructableZ('OTip', x, y, z, 0.0, 1.0, 0)
		local trackable whichTrackable = CreateTrackableForPlayer(whichPlayer, modelPath, x, y, facingAngle)
		call RemoveDestructable(heightDestructable)
		set heightDestructable = null
		return whichTrackable
	endfunction

endlibrary