library ALibraryCoreInterfaceCamera requires ALibraryCoreMathsPoint

	/// Similar to @function RotateCameraAroundLocBJ but does not use location.
	/// @author Tamino Dauth
	/// @state untested
	function RotateCameraAroundPointForPlayer takes player user, real x, real y, real degrees, real duration returns nothing
		local player localPlayer = GetLocalPlayer()
		if (localPlayer == user) then
			call SetCameraRotateMode(x, y, bj_DEGTORAD * degrees, duration)
		endif
		set localPlayer = null
	endfunction

	/// Similar to @function SmartCameraPanBJ but does not use location and uses Z value.
	/// Does not use z!
	/// @author Tamino Dauth
	/// @state untested
	function SmartCameraPanWithZForPlayer takes player user, real x, real y, real zOffset, real duration returns nothing
		local player localPlayer = GetLocalPlayer()
		local real distance
		if (localPlayer == user) then
			set distance = GetDistanceBetweenPoints(x, y, 0.0, GetCameraTargetPositionX(), GetCameraTargetPositionY(), 0.0)
			if (distance >= bj_SMARTPAN_TRESHOLD_SNAP) then
				//If the user is too far away, snap the camera.
				call PanCameraToTimedWithZ(x, y, zOffset, 0.0)
			elseif (distance >= bj_SMARTPAN_TRESHOLD_PAN) then
				//If the user is moderately close, pan the camera.
				call PanCameraToTimedWithZ(x, y, zOffset, duration)
			//else
				// User is close enough, so don't touch the camera.
			endif
		endif
		set localPlayer = null
	endfunction

	/// Similar to @function SmartCameraPanBJ but does not use location.
	/// @author Tamino Dauth
	/// @state untested
	function SmartCameraPanForPlayer takes player user, real x, real y, real duration returns nothing
		local player localPlayer = GetLocalPlayer()
		local real distance
		if (localPlayer == user) then
			set distance = GetDistanceBetweenPoints(x, y, 0.0, GetCameraTargetPositionX(), GetCameraTargetPositionY(), 0.0)
			if (distance >= bj_SMARTPAN_TRESHOLD_SNAP) then
				//If the user is too far away, snap the camera.
				call PanCameraToTimed(x, y, 0.0)
			elseif (distance >= bj_SMARTPAN_TRESHOLD_PAN) then
				//If the user is moderately close, pan the camera.
				call PanCameraToTimed(x, y, duration)
			//else
				// User is close enough, so don't touch the camera.
			endif
		endif
		set localPlayer = null 
	endfunction

	/// Similar to @function PanCameraToTimedLocWithZForPlayer but does not use location.
	/// @author Tamino Dauth
	/// @state untested
	function PanCameraToTimedWithZForPlayer takes player user, real x, real y, real zOffset, real duration returns nothing
		local player localPlayer = GetLocalPlayer()
		if (localPlayer == user) then
			call PanCameraToTimedWithZ(x, y, zOffset, duration)
		endif
		set localPlayer = null
	endfunction

	function SetCameraBoundsForPlayer takes player user, real x1, real y1, real x2, real y2, real x3, real y3, real x4, real y4 returns nothing
		local player localPlayer = GetLocalPlayer()
		if (user == localPlayer) then
			call SetCameraBounds(x1, y1, x2, y2, x3, y3, x4, y4)
		endif
		set localPlayer = null
	endfunction

	function SetCameraBoundsToPointForPlayer takes player user, real x, real y returns nothing
		call SetCameraBoundsForPlayer(user, x, y, x, y, x, y, x, y)
	endfunction

	function ResetCameraBoundsToMapRectForPlayer takes player user returns nothing
		local real minX = GetRectMinX(bj_mapInitialCameraBounds)
    	local real minY = GetRectMinY(bj_mapInitialCameraBounds)
    	local real maxX = GetRectMaxX(bj_mapInitialCameraBounds)
    	local real maxY = GetRectMaxY(bj_mapInitialCameraBounds)
		call SetCameraBoundsForPlayer(user, minX, minY, minX, maxY, maxX, maxY, maxX, minY)
	endfunction

	function GetCameraTargetPositionOfPlayer takes player user returns location
		local player localPlayer = GetLocalPlayer()
		local location cameraTargetPosition
		if (user == localPlayer) then
			set cameraTargetPosition = GetCameraTargetPositionLoc()
		endif
		set localPlayer = null
		return cameraTargetPosition
	endfunction

	function GetCameraTargetPositionXOfPlayer takes player user returns real
		local player localPlayer = GetLocalPlayer()
		local real value
		if (user == localPlayer) then
			set value = GetCameraTargetPositionX()
		endif
		set localPlayer = null
		return value
	endfunction

	function GetCameraTargetPositionYOfPlayer takes player user returns real
		local player localPlayer = GetLocalPlayer()
		local real value
		if (user == localPlayer) then
			set value = GetCameraTargetPositionY()
		endif
		set localPlayer = null
		return value
	endfunction

	function GetCameraTargetPositionZOfPlayer takes player user returns real
		local player localPlayer = GetLocalPlayer()
		local real value
		if (user == localPlayer) then
			set value = GetCameraTargetPositionZ()
		endif
		set localPlayer = null
		return value
	endfunction

	function GetCameraFieldOfPlayer takes player user, camerafield cameraField returns real
		local player localPlayer = GetLocalPlayer()
		local real cameraFieldValue
		if (user == localPlayer) then
			set cameraFieldValue = GetCameraField(cameraField)
		endif
		set localPlayer = null
		return cameraFieldValue
	endfunction

endlibrary