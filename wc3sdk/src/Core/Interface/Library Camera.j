library ALibraryCoreInterfaceCamera requires optional ALibraryCoreDebugMisc, ALibraryCoreGeneralPlayer, ALibraryCoreMathsHandle, ALibraryCoreMathsPoint, ALibraryCoreMathsRect

	/**
	* @author Tamino Dauth
	* @todo Test!
	* @see PanCameraToWithZ, PanCameraToTimedLocForPlayer
	*/
	function PanCameraToWithZForPlayer takes player whichPlayer, real x, real y, real zOffset returns nothing
		if (whichPlayer == GetLocalPlayer()) then
			call PanCameraToWithZ(x, y, zOffset)
		endif
	endfunction

	/**
	* @author Tamino Dauth
	* @todo Test!
	* @see PanCameraToTimedWithZ, PanCameraToTimedLocForPlayer
	*/
	function PanCameraToTimedWithZForPlayer takes player whichPlayer, real x, real y, real zOffset, real duration returns nothing
		if (whichPlayer == GetLocalPlayer()) then
			call PanCameraToTimedWithZ(x, y, zOffset, duration)
		endif
	endfunction

	/**
	* @author Tamino Dauth
	* @todo Test!
	* @see PanCameraToTimed, SmartCameraPanForPlayer
	*/
	function SmartCameraPan takes real x, real y, real duration returns nothing
		local real dist = GetDistanceBetweenPoints(x, y, 0.0, GetCameraTargetPositionX(), GetCameraTargetPositionY(), 0.0)
		if (dist >= bj_SMARTPAN_TRESHOLD_SNAP) then
			// If the user is too far away, snap the camera.
			call PanCameraToTimed(x, y, 0)
		elseif (dist >= bj_SMARTPAN_TRESHOLD_PAN) then
			// If the user is moderately close, pan the camera.
			call PanCameraToTimed(x, y, duration)
		// else
			// User is close enough, so don't touch the camera.
		endif
	endfunction

	/**
	* Similar to SmartCameraPanBJ but does not use location.
	* @author Tamino Dauth
	* @todo Test!
	* @see SmartCameraPanBJ, PanCameraToTimed, SmartCameraPan
	*/
	function SmartCameraPanForPlayer takes player whichPlayer, real x, real y, real duration returns nothing
		local real dist
		if (GetLocalPlayer() == whichPlayer) then
			call SmartCameraPan(x, y, duration)
		endif
	endfunction

	/**
	* @author Tamino Dauth
	* @todo Test!
	* @see PanCameraToTimedWithZ, SmartCameraPanWithZForPlayer
	*/
	function SmartCameraPanWithZ takes real x, real y, real zOffset, real duration returns nothing
		local real distance = GetDistanceBetweenPoints(x, y, 0.0, GetCameraTargetPositionX(), GetCameraTargetPositionY(), 0.0)
		if (distance >= bj_SMARTPAN_TRESHOLD_SNAP) then
			// If the user is too far away, snap the camera.
			call PanCameraToTimedWithZ(x, y, zOffset, 0.0)
		elseif (distance >= bj_SMARTPAN_TRESHOLD_PAN) then
			// If the user is moderately close, pan the camera.
			call PanCameraToTimedWithZ(x, y, zOffset, duration)
		// else
			// User is close enough, so don't touch the camera.
		endif
	endfunction

	/**
	* Similar to SmartCameraPanBJ but does not use location and uses Z value.
	* @author Tamino Dauth
	* @todo Test!
	* @see SmartCameraPanBJ, PanCameraToTimedWithZ, SmartCameraPanWithZ
	*/
	function SmartCameraPanWithZForPlayer takes player whichPlayer, real x, real y, real zOffset, real duration returns nothing
		if (GetLocalPlayer() == whichPlayer) then
			call SmartCameraPanWithZ(x, y, zOffset, duration)
		endif
	endfunction

	/**
	* Similar to SetCameraRotateMode but uses degree not radian.
	* @author Tamino Dauth
	* @todo Test!
	* @see SetCameraRotateMode
	*/
	function RotateCameraAround takes real x, real y, real degrees, real duration returns nothing
		call SetCameraRotateMode(x, y, bj_DEGTORAD * degrees, duration)
	endfunction

	/**
	* Similar to RotateCameraAroundLocBJ but does not use location.
	* @author Tamino Dauth
	* @todo Test!
	* @see RotateCameraAroundLocBJ
	*/
	function RotateCameraAroundForPlayer takes player whichPlayer, real x, real y, real degrees, real duration returns nothing
		local player localPlayer = GetLocalPlayer()
		if (localPlayer == whichPlayer) then
			call SetCameraRotateMode(x, y, bj_DEGTORAD * degrees, duration)
		endif
		set localPlayer = null
	endfunction


	/**
	* Generic camera functions for data types with location property.
	* @see SetCameraPosition, SetCameraPositionForPlayer, SetCameraPositionLocForPlayer, SetCameraQuickPosition, SetCameraQuickPositionLoc, SetCameraQuickPositionForPlayer, SetCameraQuickPositionLocForPlayer, PanCameraTo, PanCameraToTimed, PanCameraToWithZ, PanCameraToTimedWithZ, PanCameraToForPlayer, PanCameraToLocForPlayer, PanCameraToTimedForPlayer, PanCameraToTimedLocForPlayer, PanCameraToTimedLocWithZForPlayer, SmartCameraPanBJ,  SetCameraRotateMode, RotateCameraAroundLocBJ
	*/
	//! textmacro ACameraTextMacro takes TYPE, TYPENAME
		function SetCameraPosition$TYPENAME$ takes $TYPE$ which$TYPENAME$ returns nothing
			call SetCameraPosition(Get$TYPENAME$X(which$TYPENAME$), Get$TYPENAME$Y(which$TYPENAME$))
		endfunction

		function SetCameraPosition$TYPENAME$ForPlayer takes player whichPlayer, $TYPE$ which$TYPENAME$ returns nothing
			call SetCameraPositionForPlayer(whichPlayer, Get$TYPENAME$X(which$TYPENAME$), Get$TYPENAME$Y(which$TYPENAME$))
		endfunction

		function SetCameraQuickPosition$TYPENAME$ takes $TYPE$ which$TYPENAME$ returns nothing
			call SetCameraQuickPosition(Get$TYPENAME$X(which$TYPENAME$), Get$TYPENAME$Y(which$TYPENAME$))
		endfunction

		function SetCameraQuickPosition$TYPENAME$ForPlayer takes player whichPlayer, $TYPE$ which$TYPENAME$ returns nothing
			call SetCameraQuickPositionForPlayer(whichPlayer, Get$TYPENAME$X(which$TYPENAME$), Get$TYPENAME$Y(which$TYPENAME$))
		endfunction

		function PanCameraTo$TYPENAME$ takes $TYPE$ which$TYPENAME$ returns nothing
			call PanCameraTo(Get$TYPENAME$X(which$TYPENAME$), Get$TYPENAME$Y(which$TYPENAME$))
		endfunction

		function PanCameraTo$TYPENAME$ForPlayer takes player whichPlayer, $TYPE$ which$TYPENAME$ returns nothing
			call PanCameraToForPlayer(whichPlayer, Get$TYPENAME$X(which$TYPENAME$), Get$TYPENAME$Y(which$TYPENAME$))
		endfunction

		function PanCameraTo$TYPENAME$WithZ takes $TYPE$ which$TYPENAME$, real zOffset returns nothing
			call PanCameraToWithZ(Get$TYPENAME$X(which$TYPENAME$), Get$TYPENAME$Y(which$TYPENAME$), zOffset)
		endfunction

		function PanCameraTo$TYPENAME$WithZForPlayer takes player whichPlayer, $TYPE$ which$TYPENAME$, real zOffset returns nothing
			call PanCameraToWithZForPlayer(whichPlayer, Get$TYPENAME$X(which$TYPENAME$), Get$TYPENAME$Y(which$TYPENAME$), zOffset)
		endfunction

		function PanCameraToTimed$TYPENAME$ takes $TYPE$ which$TYPENAME$, real duration returns nothing
			call PanCameraToTimed(Get$TYPENAME$X(which$TYPENAME$), Get$TYPENAME$Y(which$TYPENAME$), duration)
		endfunction

		function PanCameraToTimed$TYPENAME$ForPlayer takes player whichPlayer, $TYPE$ which$TYPENAME$, real duration returns nothing
			call PanCameraToTimedForPlayer(whichPlayer, Get$TYPENAME$X(which$TYPENAME$), Get$TYPENAME$Y(which$TYPENAME$), duration)
		endfunction

		function PanCameraToTimed$TYPENAME$WithZ takes $TYPE$ which$TYPENAME$, real zOffset, real duration returns nothing
			call PanCameraToTimedWithZ(Get$TYPENAME$X(which$TYPENAME$), Get$TYPENAME$Y(which$TYPENAME$), zOffset, duration)
		endfunction

		function PanCameraToTimed$TYPENAME$WithZForPlayer takes player whichPlayer, $TYPE$ which$TYPENAME$, real zOffset, real duration returns nothing
			call PanCameraToTimedWithZForPlayer(whichPlayer, Get$TYPENAME$X(which$TYPENAME$), Get$TYPENAME$Y(which$TYPENAME$), zOffset, duration)
		endfunction

		function SmartCameraPan$TYPENAME$ takes $TYPE$ which$TYPENAME$, real duration returns nothing
			call SmartCameraPan(Get$TYPENAME$X(which$TYPENAME$), Get$TYPENAME$Y(which$TYPENAME$), duration)
		endfunction

		function SmartCameraPan$TYPENAME$ForPlayer takes player whichPlayer, $TYPE$ which$TYPENAME$, real duration returns nothing
			call SmartCameraPanForPlayer(whichPlayer, Get$TYPENAME$X(which$TYPENAME$), Get$TYPENAME$Y(which$TYPENAME$), duration)
		endfunction

		function SmartCameraPan$TYPENAME$WithZ takes $TYPE$ which$TYPENAME$, real zOffset, real duration returns nothing
			call SmartCameraPanWithZ(Get$TYPENAME$X(which$TYPENAME$), Get$TYPENAME$Y(which$TYPENAME$), zOffset, duration)
		endfunction

		function SmartCameraPan$TYPENAME$WithZForPlayer takes player whichPlayer, $TYPE$ which$TYPENAME$, real zOffset, real duration returns nothing
			call SmartCameraPanWithZForPlayer(whichPlayer, Get$TYPENAME$X(which$TYPENAME$), Get$TYPENAME$Y(which$TYPENAME$), zOffset, duration)
		endfunction

		function RotateCameraAround$TYPENAME$ takes $TYPE$ which$TYPENAME$, real degrees, real duration returns nothing
			call RotateCameraAround(Get$TYPENAME$X(which$TYPENAME$), Get$TYPENAME$Y(which$TYPENAME$), degrees, duration)
		endfunction

		function RotateCameraAround$TYPENAME$ForPlayer takes player whichPlayer, $TYPE$ which$TYPENAME$, real degrees, real duration returns nothing
			call RotateCameraAroundForPlayer(whichPlayer, Get$TYPENAME$X(which$TYPENAME$), Get$TYPENAME$Y(which$TYPENAME$), degrees, duration)
		endfunction
	//! endtextmacro

	/**
	* There are already some location related camera functions which use identifier "Loc" instead of "Location".
	* However, some implementations are missing so we just create a new set of functions.
	*/
	//! runtextmacro ACameraTextMacro("location", "Location")
	//! runtextmacro ACameraTextMacro("rect", "Rect")
	//! runtextmacro ACameraTextMacro("widget", "Widget")
	//! runtextmacro ACameraTextMacro("unit", "Unit")
	//! runtextmacro ACameraTextMacro("item", "Item")
	//! runtextmacro ACameraTextMacro("destructable", "Destructable")

	function SetCameraBoundsForPlayer takes player whichPlayer, real x1, real y1, real x2, real y2, real x3, real y3, real x4, real y4 returns nothing
		local player localPlayer = GetLocalPlayer()
		if (whichPlayer == localPlayer) then
			call SetCameraBounds(x1, y1, x2, y2, x3, y3, x4, y4)
		endif
		set localPlayer = null
	endfunction

	function SetCameraBoundsToPointForPlayer takes player whichPlayer, real x, real y returns nothing
		call SetCameraBoundsForPlayer(whichPlayer, x, y, x, y, x, y, x, y)
	endfunction

	function ResetCameraBoundsToMapRectForPlayer takes player user returns nothing
		local real minX = GetRectMinX(bj_mapInitialCameraBounds)
		local real minY = GetRectMinY(bj_mapInitialCameraBounds)
		local real maxX = GetRectMaxX(bj_mapInitialCameraBounds)
		local real maxY = GetRectMaxY(bj_mapInitialCameraBounds)
		call SetCameraBoundsForPlayer(user, minX, minY, minX, maxY, maxX, maxY, maxX, minY)
	endfunction

	/// @todo Desynchronization!
	function GetCameraTargetPositionOfPlayer takes player user returns location
		local player localPlayer = GetLocalPlayer()
		local location cameraTargetPosition = null
		/*
		if (user == localPlayer) then
			set cameraTargetPosition = GetCameraTargetPositionLoc()
		endif
		*/
		set localPlayer = null
		debug call PrintFunctionError("GetCameraTargetPositionOfPlayer", "Broken function!")
		return cameraTargetPosition
	endfunction

	/// @todo Desynchronization!
	function GetCameraTargetPositionXOfPlayer takes player user returns real
		local player localPlayer = GetLocalPlayer()
		local real value = 0.0
		/*
		if (user == localPlayer) then
			set value = GetCameraTargetPositionX()
		endif
		*/
		set localPlayer = null
		debug call PrintFunctionError("GetCameraTargetPositionXOfPlayer", "Broken function!")
		return value
	endfunction

	/// @todo Desynchronization!
	function GetCameraTargetPositionYOfPlayer takes player user returns real
		local player localPlayer = GetLocalPlayer()
		local real value = 0.0
		/*
		if (user == localPlayer) then
			set value = GetCameraTargetPositionY()
		endif
		*/
		set localPlayer = null
		debug call PrintFunctionError("GetCameraTargetPositionYOfPlayer", "Broken function!")
		return value
	endfunction

	/// @todo Desynchronization!
	function GetCameraTargetPositionZOfPlayer takes player user returns real
		local player localPlayer = GetLocalPlayer()
		local real value = 0.0
		/*
		if (user == localPlayer) then
			set value = GetCameraTargetPositionZ()
		endif
		set localPlayer = null
		*/
		debug call PrintFunctionError("GetCameraTargetPositionZOfPlayer", "Broken function!")
		return value
	endfunction

	/// @todo Desynchronization!
	function GetCameraFieldOfPlayer takes player user, camerafield cameraField returns real
		local player localPlayer = GetLocalPlayer()
		local real cameraFieldValue = 0.0
		/*
		if (user == localPlayer) then
			set cameraFieldValue = GetCameraField(cameraField)
		endif
		set localPlayer = null
		*/
		debug call PrintFunctionError("GetCameraFieldOfPlayer", "Broken function!")
		return cameraFieldValue
	endfunction

endlibrary