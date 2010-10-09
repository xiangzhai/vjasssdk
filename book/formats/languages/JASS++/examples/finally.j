// Jass and vJass

// We need a temporary variable or we will have to call RemoveUnit in each if statement.
function GetUnitTypeStringAndRemove takes unit whichUnit returns string
	local string result
	if (GetUnitTypeId(whichUnit) == 'h000') then
		set result = "It's cool!"
	elseif (GetUnitTypeId(whichUnit) == 'h001') then
		set result = "It's almost cool!"
	elseif (GetUnitTypeId(whichUnit) == 'h002') then
		set result = "It's amazing"
	else
		set result = "Unknown"
	endif
	call RemoveUnit(whichUnit)
	return result
endfunction

// JASS++

// We don't need any local variable since finally statement will be called when function returns something.
string GetUnitTypeStringAndRemove(unit whichUnit)
	switch (GetUnitTypeId(whichUnit))
		case ('h000')
			return "It's cool!"

		case ('h001')
			return  "It's almost cool!"

		case ('h002')
			return "It's amazing"

	return "Unknown"

	finally
		RemoveUnit(whichUnit)