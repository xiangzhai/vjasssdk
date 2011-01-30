library ALibraryCoreMathsUnit requires ALibraryCoreGeneralUnit, ALibraryCoreMathsHandle, ALibraryCoreMathsPoint

	/// Die Entfernung zwischen zwei Einheiten.
	/// Da es die nativen Funktionen GetUnitX() und GetUnitY() gibt, dürfte es schneller sein, als erst Locations der Einheiten zu erzeugen und diese zu vergleichen.
	/// Achtung: Nur verwenden, wenn man auch wirklich den Z-Wert braucht.
	/// @return Returns the distance between units.
	function GetDistanceBetweenUnitsWithZ takes unit unit0, unit unit1 returns real
		local location unitLocation0 = GetUnitLoc(unit0)
		local location unitLocation1 = GetUnitLoc(unit1)
		local real distance =  GetDistanceBetweenUnits(unit0, unit1, GetLocationZ(unitLocation0), GetLocationZ(unitLocation1)) //ALibraryMathsHandle
		call RemoveLocation(unitLocation0)
		set unitLocation0 = null
		call RemoveLocation(unitLocation1)
		set unitLocation1 = null
		return distance
	endfunction

	/// Does not create any locations.
	function SetUnitPolarProjectionPosition takes unit usedUnit, real angle, real distance returns nothing
		local real x = GetUnitPolarProjectionX(usedUnit, angle, distance)
		local real y = GetUnitPolarProjectionY(usedUnit, angle, distance)
		call SetUnitPosition(usedUnit, x, y)
		call SetUnitFacing(usedUnit, angle)
	endfunction

	/// Makes a unit flyable by adding and removing the ability Crow Form.
	/// Thus you can change its high.
	function MakeUnitFlyable takes unit usedUnit returns nothing
		if (GetUnitAbilityLevel(usedUnit, 'Amrf') == 0) then
			call UnitAddAbility(usedUnit, 'Amrf')
			call UnitRemoveAbility(usedUnit, 'Amrf')
		endif
	endfunction

	/// Ändert die Höhe einer Einheit.
	/// Die neue Höhe muss >= der Bodenhöhe an dem Punkt, an dem sich die Einheit befindet, sein.
	/// @author WaterKnight
	function SetUnitZ takes unit usedUnit, real z returns nothing
		call MakeUnitFlyable(usedUnit)
		call SetUnitFlyHeight(usedUnit, z - GetUnitZ(usedUnit), 0.0) //ALibraryMathsHandle
	endfunction

	/**
	* Beschreibung: Die Funktionen setzen eine Einheit auf angegebene Koordinaten, falls die Einheit
	* auf diesen stehen könnte. Dabei wird die Betrachtung für jede Koordinate separat ausgeführt.
	* Das heißt, die Einheit könnte in eine Achsenrichtung bewegt werden, auch wenn sie es in die
	* Zweite nicht kann. Dadurch slidet sie an Grenzen, wenn die Bewegung in eine Richtung möglich
	* ist. Die Funktionen returnen, ob die Einheit erfolgreich an die gegebenen Koordinaten bewegt
	* wurde.
	* Hinweise: Um zu entscheiden, ob die Einheit geblockt wird, wird ein kleiner Toleranzbereich
	* genommen. Auf Bewegung bezogene Ereignisse werden ausgeführt, auch wenn die Einheit im
	* Endeffekt zurückgesetzt wurde. Sowohl das Prüfen als auch das Zurücksetzen der Position
	* erfolgt über Bewegungsaktionen, die auf die Einheit angewandt werden. Da die Funktionen wohl
	* im Zusammenhang mit rekursiven Fortbewegungssystemen einer Einheit gebraucht werden (die
	* vorigen Koordinaten einer Einheit werden ausgelesen, um sie zum Beispiel für die Ermittlung
	* der Nächsten zu verarbeiten), muss man bei allen drei Funktionen die alten
	* Positionsinformationen der Einheit mitliefern. Das ist besser für die
	* Schnelligkeitsperformance, als wenn man es mehrfach ausliest.
	* @author WaterKnight
	*/
	function SetUnitXIfNotBlocked takes unit usedUnit, real oldX, real oldY, real x returns boolean
		call SetUnitPosition(usedUnit, x, oldY)
		if ((RAbsBJ(GetUnitX(usedUnit) - x) > 1) or (RAbsBJ(GetUnitY(usedUnit) - oldY) > 1)) then
			call SetUnitX(usedUnit, oldX)
			call SetUnitY(usedUnit, oldY)
			return false
		endif
		return true
	endfunction

	/// @author WaterKnight
	function SetUnitYIfNotBlocked takes unit usedUnit, real oldX, real oldY, real y returns boolean
		call SetUnitPosition(usedUnit, oldX, y)
		if ((RAbsBJ(GetUnitX(usedUnit) - oldX) > 1) or (RAbsBJ(GetUnitY(usedUnit) - y) > 1)) then
			call SetUnitX(usedUnit, oldX)
			call SetUnitY(usedUnit, oldY)
			return false
		endif
		return true
	endfunction

	/// @author WaterKnight
	function SetUnitXYIfNotBlocked takes unit usedUnit, real oldX, real oldY, real x, real y returns boolean
		if (SetUnitXIfNotBlocked(usedUnit, oldX, oldY, x)) then
			if (SetUnitYIfNotBlocked(usedUnit, x, oldY, y )) then
				return true
			endif
		else
			call SetUnitYIfNotBlocked(usedUnit, oldX, oldY, y)
		endif
		return false
	endfunction

	/// @author Grater
	/// @link http://www.wc3jass.com/
	function FindClosestUnit takes group g, real x, real y returns unit
		local real dx
		local real dy
		local group tempGroup
		local real maxDist = 999999.0
		local real dist
		local unit u = null
		local unit closest = null
		if (bj_wantDestroyGroup == true) then
			set tempGroup = g
		else
			set tempGroup = CreateGroup()
			call GroupAddGroup(g, tempGroup)
		endif
		set bj_wantDestroyGroup = false

		loop
			set u = FirstOfGroupSave(tempGroup)
			call GroupRemoveUnit(tempGroup, u)
			exitwhen (u == null)
			set dx = GetUnitX(u) - x
			set dy = GetUnitY(u) - y
			set dist = SquareRoot(dx*dx+dy*dy)
			if (dist < maxDist) then
				set closest = u
				set maxDist = dist
			endif
			set u = null
		endloop
		call DestroyGroup(tempGroup)
		set tempGroup = null
		return closest
	endfunction

	/// @author Grater
	/// @link http://www.wc3jass.com/
	function FindClosestUnitByLocation takes group g, location loc returns unit
		return FindClosestUnit(g, GetLocationX(loc), GetLocationY(loc))
	endfunction

	/// @author Tamino Dauth
	function FindClosestUnitByRect takes group usedGroup, rect usedRect returns unit
		return FindClosestUnit(usedGroup, GetRectCenterX(usedRect), GetRectCenterY(usedRect))
	endfunction

	/**
	* IsUnitOnRect returns true if the unit's collision circle
	* ------------ interesects with a rect.
	*
	* Useful for example, in a "enter rect" event
	* it will return true, unlike blizz' RectContainsUnit
	*
	* probably slower than RectContainsUnit
	* @author Vexorian
	*/
	function IsUnitOnRect takes unit u, rect r returns boolean
		local real x = GetUnitX(u)
		local real y = GetUnitY(u)
		local real mx = GetRectMaxX(r)
		local real nx = GetRectMinX(r)
		local real my = GetRectMaxY(r)
		local real ny = GetRectMinY(r)
		if (nx <= x) and (x <= mx) and (ny <= y) and (y <= my) then
			return true
		endif
		if(x>mx) then
			set x=mx
		elseif(x<nx) then
			set x=nx
		endif
		if(y>my) then
			set y=my
		elseif(y<ny) then
			set y=ny
		endif
		return IsUnitInRangeXY(u,x,y,0.0)
	endfunction

	/// @author Tamino Dauth
	function SetUnitFacingToFaceRectTimed takes unit whichUnit, rect whichRect, real duration returns nothing
		call SetUnitFacingTimed(whichUnit, GetAngleBetweenPoints(GetUnitX(whichUnit), GetUnitY(whichUnit), GetRectCenterX(whichRect), GetRectCenterY(whichRect)), duration)
	endfunction

	/**
	* @author Tamino Dauth
	* @see SetUnitFacingToFaceUnitTimed
	*/
	function SetUnitFacingToFaceUnit takes unit whichUnit, unit target returns nothing
		call SetUnitFacing(whichUnit, GetAngleBetweenUnits(whichUnit, target))
	endfunction

	/**
	* @author Tamino Dauth
	* @see SetUnitPositionLoc
	*/
	function SetUnitPositionRect takes unit whichUnit, rect whichRect returns nothing
		call SetUnitPosition(whichUnit, GetRectCenterX(whichRect), GetRectCenterY(whichRect))
	endfunction

	/**
	* \author Tamino Dauth
	* \sa SetUnitPositionLocFacingBJ
	*/
	function SetUnitPositionRectFacing takes unit whichUnit, rect whichRect, real facing returns nothing
		call SetUnitPositionRect(whichUnit, whichRect)
		call SetUnitFacing(whichUnit, facing)
	endfunction

	/// @author Tamino Dauth
	function SetUnitToRandomPointOnRect takes unit whichUnit, rect whichRect returns nothing
		local real minX = RMinBJ(GetRectMinX(whichRect), GetRectMaxX(whichRect))
		local real maxX = RMaxBJ(GetRectMinX(whichRect), GetRectMaxX(whichRect))
		local real minY = RMinBJ(GetRectMinY(whichRect), GetRectMaxY(whichRect))
		local real maxY = RMaxBJ(GetRectMinY(whichRect), GetRectMaxY(whichRect))
		call SetUnitPosition(whichUnit, GetRandomReal(minX, maxX), GetRandomReal(minY, maxY))
	endfunction

	/**
	* Issues unit \p whichUnit order \p order at center of rect \p whichRect.
	* \return Returns true if order was issued successfully.
	* \author Tamino Dauth
	* \sa IssuePointOrder, IssueRectOrderById
	*/
	function IssueRectOrder takes unit whichUnit, string order, rect whichRect returns boolean
		return IssuePointOrder(whichUnit, order, GetRectCenterX(whichRect), GetRectCenterY(whichRect))
	endfunction

	/**
	* \copydoc IssueRectOrder
	* \author Tamino Dauth
	* \sa IssuePointOrderById, IssueRectOrder
	*/
	function IssueRectOrderById takes unit whichUnit, integer order, rect whichRect returns boolean
		return IssuePointOrderById(whichUnit, order, GetRectCenterX(whichRect), GetRectCenterY(whichRect))
	endfunction

endlibrary