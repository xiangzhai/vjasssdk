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

	/// Eine unsichtbare Plattform wird unter dem Trackable erzeugt.
	/// Diese Funktion ist extra, da sie mehr Speicher verbraucht und nicht immer benötigt wird.
	/// @author KaTTaNa
	/// From: http://www.wc3jass.com/
	/// This creates a trackable at the given coordinates that floats above the ground with the height specified by z.
	//It works by creating an invisible platform, creating the trackable and removing the platform again.
	function CreateTrackableForPlayerZ takes player user, string modelPath, real x, real y, real z, real facingAngle returns trackable
		local destructable heightDestructable = CreateDestructableZ('OTip', x, y, z, 0.0, 1.0, 0)
		local trackable usedTrackable = CreateTrackableForPlayer(user, modelPath, x, y, facingAngle)
		call RemoveDestructable(heightDestructable)
		set heightDestructable = null
		return usedTrackable
	endfunction

endlibrary