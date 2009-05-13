library ALibraryCoreEnvironmentSpecialEffect

	/// Creates a single special effect which only can be seen by player @param user.
	/// @author Tamino Dauth
	function CreateSpecialEffectForPlayer takes player user, string modelPath, real x, real y returns effect
		local player localPlayer = GetLocalPlayer()
		local string localPath = ""
		if (user == localPlayer) then
			set localPath = modelPath 
		endif
		set localPlayer = null
		return AddSpecialEffect(localPath, x, y) 
	endfunction

	/// Creates a single special effect on widget's @param target attachement point @param attachPoint with model file @param model path.
	/// The created special effect is only visible to player @param user.
	function CreateSpecialEffectOnTargetForPlayer takes player user, string modelPath, widget target, string attachPoint returns effect
		local player localPlayer = GetLocalPlayer()
		local string localPath = ""
		if (user == localPlayer) then
			set localPath = modelPath
		endif
		set localPlayer = null
		return AddSpecialEffectTarget(localPath, target, attachPoint) 
	endfunction

endlibrary