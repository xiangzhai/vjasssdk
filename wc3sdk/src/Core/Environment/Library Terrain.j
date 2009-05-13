library ALibraryCoreEnvironmentTerrain

	/// Original description:
	/// The function SetTerrainPathable sets a single 32x32 space on the pathing grid. However, SetTerrainType sets a
	/// 128x128 space on the terrain grid, and does not change the pathing for that space. If you want to change the
	/// pathing, you'd have to change the pathing for 16 individual points per terrain space. This function rectifies 
	/// the situation by allowing you to change the pathing for a single space on the terrain grid, instead of a single 
	/// space on the pathing grid.
	/// Note: In my tests, the alignment was correct. However, there might be cases where this isn't true, so if there 
	/// is a problem I will change the numbers as needed.
	/// @author Shvegait
	/// @source http://www.wc3jass.com/
	function SetTerrainSpacePathable takes real x, real y, pathingtype pathingType, boolean flag returns nothing
		local real newX = x + 64.0
		local real newY = y + 64.0
		local integer i
		local integer j
		set newX = (newX - ModuloReal(newX, 128.0) - 64.0)
		set newY = (newY - ModuloReal(newY, 128.0) + 32.0)
		set i = 0
		loop
			exitwhen (i > 3)
			set j = 0
			loop
				exitwhen (j > 3)
				call SetTerrainPathable((newX + i * 32.0) , (newY - j * 32.0), pathingType, flag)
				set j = j + 1
			endloop
			set i = i + 1
		endloop
	endfunction

endlibrary