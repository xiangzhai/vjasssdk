library ALibraryCoreMathsReal

	/**
	* @author WaterKnight
	* @link http://www.inwarcraft.de/
	* Siehe RoundTo.
	*/
	private function RoundToGetDifference takes real dividend, real divisor returns real
		return RAbsBJ(dividend - R2I(dividend / divisor) * divisor)
	endfunction

	/**
	* @author WaterKnight
	* @return Gibt das von base nächstgelegene Vielfache von interval zurück. Also praktisch ein allgemeines Runden. Ist \p interval 0, so gibt die Funktion 0 zurück.
	* Sollte der Abstand gleich sein, hat das Ergebnis mit dem kleineren Betrag Vorrang. RoundToGetDifference ist für RoundTo gedacht, deswegen habe ich eine Sicherheitsmanahme, die eine Nulldivision verhindern würde, ausgelassen, da diese schon bei RoundTo stattfindet.
	*/
	function RoundTo takes real base, real interval returns real
		local real difference1
		local real difference2
		if (interval == 0) then
			return 0.0
		endif
		set difference1 = RoundToGetDifference(base, interval)
		set difference2 = RAbsBJ(interval) - difference1
		if (difference2 < difference1) then
			return (base + RSignBJ(interval) * difference2)
		endif
		return (base - RSignBJ(interval) * difference1)
	endfunction

	/**
	* @author Shadow1500
	* @link http://www.hiveworkshop.com/
	* @return Returns the height.
	*/
	function JumpParabola takes real distance, real maxDistance, real curve returns real
		local real t = (distance * 2) / maxDistance - 1
		return (- t * t + 1) * (maxDistance / curve)
	endfunction

	/**
	* This function takes as a parameters the distance that the parabolic movement should move (d)
	* and the maximum height the projectile will flight (h).
	* So x is a value between 0 and d, and the function will return the height for that value.
	* As a side note, if x < 0 or x > d then this function will return negative values.
	* @author moyack
	* @author Spec
	* @link http://www.wc3c.net/showthread.php?t=102077
	*/
	function ParabolaZ takes real maxHeight, real distance, real x returns real
		return (4 * maxHeight / distance) * (distance - x) * (x / distance)
	endfunction

	/**
	* Called ALog since RtC has its own Log function.
	* @see Log, Logarithm
	* @author Vexorian
	* @link http://www.wc3campaigns.net/
	*/
	function ALog takes real x, integer iterations returns real
		local real min = -88.0
		local real max = 88.0
		local real mid
		local integer i = iterations
		loop
			set mid = (min + max) / 2
			exitwhen (i <= 0)
			set i = i - 1
			if (Pow(bj_E, mid) >= x) then
				set max = mid
			else
				set min = mid
			endif
		endloop
		return mid
	endfunction

	/**
	* @see Log, ALog
	* @author Vexorian
	* @link http://www.wc3campaigns.net/
	*/
	function Logarithm takes real base, real x, integer iterations returns real
		local real min = -88.0
		local real max = 88.0
		local real mid
		local integer i = iterations
		loop
			set mid = (min + max) / 2
			exitwhen (i <= 0)
			set i = i-1
			if (Pow(base, mid) >= x) then
				set max = mid
			else
				set min= mid
			endif
		endloop
		return mid
	endfunction

	/// @author Tamino Dauth
	function GetRandomFacing takes nothing returns real
		return GetRandomReal(0.0, 360.0)
	endfunction

endlibrary