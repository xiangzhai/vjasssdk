library ALibraryCoreMathsPoint

	/// @return Returns the z value of point with coordinates @param x and @param y.
	function GetTerrainZ takes real x, real y returns real
		local location usedLocation = Location(x, y)
		local real z = GetLocationZ(usedLocation)
		call RemoveLocation(usedLocation)
		set usedLocation = null		
		return z
	endfunction

	/// Copied from the new FPS mod.
	function GetPointZ takes real x, real y returns real
		local real x1 = x - ModuloReal(x, 128.0) //top left corner
		local real y1 = y - ModuloReal(x, 128.0) + 128.0
		local real z1 = GetTerrainZ(x1, y1)
		local real x3 = x1 + 128.0 //bottom right
		local real y3 = y1 - 128.0
		local real z3 = GetTerrainZ(x3, y3)
		local real x2
		local real y2
		local real z2
		local real A
		local real B
		local real C
		local real D

		if ((x-x1) < (y1-y)) then //in bottom left section
			set x2 = x1
			set y2 = y3
		else
			set x2 = x3 //top right corner
			set y2 = y1
		endif
		set z2 = GetTerrainZ(x2, y2)

		set A = y1*(z2 - z3) + y2*(z3 - z1) + y3*(z1 - z2) //
	
		//http://local.wasp.uwa.edu.au/~pbourke/geometry/planeeq/ ftw
		set B = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2)
		set C = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)
		set D = -(x1 * (y2 * z3 - y3 * z2) + x2 * (y3 * z1 - y1 * z3) + x3 * (y1 * z2 - y2 * z1))
	
		return -(A * x + B * y + D) / C
	endfunction

	/**
	* Uses z values, too.
	* @return Returns the distance between two points.
	*/
	function GetDistanceBetweenPoints takes real x0, real y0, real z0, real x1, real y1, real z1 returns real
		local real distanceX = (x1 - x0)
		local real distanceY = (y1 - y0)
		local real distanceZ = (z1 - z0)
		return SquareRoot((distanceX * distanceX) + (distanceY * distanceY) + (distanceZ * distanceZ))
	endfunction

	/**
	* Z has to be ignored since you're unable to create a location with its z value.
	* @return Returns the centre between two points by using their coordinates.
	*/
	function GetCentreBetweenPoints takes real x0, real y0, real x1, real y1 returns location
		return Location(((x0 + x1) / 2.0), ((y0 + y1) / 2.0))
	endfunction

	/**
	* @return Returns x value of a polar projection.
	* @see GetPolarProjectionY
	* @see GetPolarProjectionOfPoint
	*/
	function GetPolarProjectionX takes real x, real angle, real distance returns real
		return (x + distance * Cos(angle * bj_DEGTORAD))
	endfunction

	/**
	* @return Returns y value of a polar projection.
	* @see GetPolarProjectionX
	* @see GetPolarProjectionOfPoint
	*/
	function GetPolarProjectionY takes real Y, real angle, real distance returns real
		return (Y + distance * Sin(angle * bj_DEGTORAD))
	endfunction

	/**
	* @return Returns the polar projection location of a point by using its coordinates.
	* @see GetPolarProjectionX
	* @see GetPolarProjectionY
	* @see PolarProjectionBJ
	*/
	function GetPolarProjectionOfPoint takes real x, real y, real angle, real distance returns location
		local real resultX = (x + distance * Cos(angle * bj_DEGTORAD))
		local real resultY = (y + distance * Sin(angle * bj_DEGTORAD))
		return Location(resultX, resultY)
	endfunction

	/**
	* @return Returns angle between to points by using their coordinates.
	* @see GetAngleBetweenPointsFromCentre
	* @see AngleBetweenPoints
	*/
	function GetAngleBetweenPoints takes real x0, real y0, real x1, real y1 returns real
		return Atan2BJ((y1 - y0), (x1 - x0))
	endfunction

	/**
	* @return Returns angle between to points by using their coordinates and a centre.
	* @see GetAngleBetweenPoints
	*/
	function GetAngleBetweenPointsFromCentre takes real centreX, real centreY, real x0, real y0, real x1, real y1 returns real
		local real ax = (x0 - centreX)
		local real ay = (y0 - centreY)
		local real bx = (x1 - centreX)
		local real by = (y1 - centreY)
		local real a = SquareRoot((ax * ax) + (ax * ax))
		local real b = SquareRoot((bx * bx) + (by * by))
		return AcosBJ(((ax * bx) + (ay * by)) / (a * b))
	endfunction

endlibrary