library ALibraryCoreMathsLocation requires ALibraryCoreMathsPoint

	/// @return Returns the distance between location @param location0 and @param location1.
	/// Benutzt im Gegensatz zur originalen Funktion den Z-Wert.
	function GetDistanceBetweenLocations takes location location0, location location1 returns real
		return GetDistanceBetweenPoints(GetLocationX(location0), GetLocationY(location0), GetLocationZ(location0), GetLocationX(location1), GetLocationY(location1), GetLocationZ(location1))
	endfunction

	/// @return Returns the centre location between location @param location0 and @param location1.
	/// Achtung: Z muss ignoriert werden, da sich kein Punkt mit einem Z-Wert erstellen lässt.
	function GetCentreBetweenLocations takes location location0, location location1 returns location
		return GetCentreBetweenPoints(GetLocationX(location0), GetLocationY(location0), GetLocationX(location1), GetLocationY(location1))
	endfunction

	/// @return Returns the angle between location @param location0 and @param location1 from centre location @param centre.
	function GetAngleBetweenLocationsFromCentre takes location centre, location location0, location location1 returns real
		return GetAngleBetweenPointsFromCentre(GetLocationX(centre), GetLocationY(centre), GetLocationX(location0), GetLocationY(location0), GetLocationX(location1), GetLocationY(location1))
	endfunction

endlibrary