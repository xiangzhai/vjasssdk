/// @todo Not tested yet!
/// The SubString functions are concepted that they doesn't need each other.
library ALibraryCoreStringMisc requires ALibraryCoreDebugMisc

	/// Debug for testing over- and underflow.
	debug function StringPositionDebug takes string usedString, integer position returns nothing
		debug if ((position < 0) or (position >= StringLength(usedString))) then
			debug call Print("StringPositionDebug: Wrong position value: " + I2S(position) + ".") //ALibraryGeneralDebug
		debug endif
	debug endfunction

	/// Searches for the position of string @param searchedString in string @param usedString.
	/// @state checked
	/// If @param searchedString is not contained by @param usedString function will return -1 otherwise it will return the position.
	/// @param usedString String which should contain the searched string.
	/// @param searchedString String which is searched.
	/// @return If the string was found it will return its position otherwise it will return -1.
	function FindString takes string usedString, string searchedString returns integer
		local integer i
		local integer exitvalue
		debug if (StringLength(usedString) < StringLength(searchedString)) then
			debug call Print("FindString: Used string is lesser than searched string.")
		debug endif
		//Gleicher Inhalt
		if (usedString == searchedString) then
			return 0
		endif
		set exitvalue = StringLength(usedString) - StringLength(searchedString)
		set i = 0
		loop
			exitwhen (i > exitvalue) //i ist nicht der zweite Positionsparameter
			if (SubString(usedString, i, (i + StringLength(searchedString))) == searchedString) then //added -1
				return i
			endif
			set i = i + 1
		endloop
		return -1
	endfunction

	/// Replaces a part of a string and returns the resulting string.
	/// @state checked
	/// @param usedString String which contains the sub string.
	/// @param position Start position of the sub string.
	/// @param replacingString String which should replace the sub string.
	/// @return Returns the new string with the replaced sub string.
	function ReplaceSubString takes string usedString, integer position, string replacingString returns string
		local string result = ""
		debug call StringPositionDebug(usedString, position)
		if (position > 0) then
			set result = SubString(usedString, 0, position)
		endif
		set result = result + replacingString
		if (StringLength(result) < StringLength(usedString)) then
			set result = result + SubString(usedString, StringLength(result), StringLength(usedString))
		endif
		return result
	endfunction

	/// Replaces string @param replacedString in string @param usedString by string @param replacingString and returns the resulting string.
	/// @state checked
	/// @param usedString String which contains the sub string @param replacedString.
	/// @param replacedString Sub string of string @param usedString which should be replaced.
	/// @param replacingString String which should replace @param replacedString.
	/// @return Returns the new string with the replaced sub string.
	function ReplaceString takes string usedString, string replacedString, string replacingString returns string
		local integer position = FindString(usedString, replacedString)
		if (position != -1) then
			return ReplaceSubString(usedString, position, replacingString)
		endif
		return usedString
	endfunction

	/// Removes the sub string at position @param position with length @param length of string @param usedString and returns the resulting string.
	/// @state checked
	/// @param usedString String which contains the sub string.
	/// @param position Position of the sub string.
	/// @param length Length of the sub string.
	/// @return Returns the new string with the replaced string.
	function RemoveSubString takes string usedString, integer position, integer length returns string
		local string result = "" //Has to be set otherwise usedString + ... doesn't work
		debug call StringPositionDebug(usedString, position)
		debug call StringPositionDebug(usedString, position + length - 1)
		if (position > 0) then
			set result = SubString(usedString, 0, position)
		endif
		if ((position + length) < StringLength(usedString)) then
			set result = result + SubString(usedString, position + length, StringLength(usedString))
		endif
		return result
	endfunction

	/// Removes string @param removedString from string @param subString and returns the resulting string. 
	/// @state checked
	/// @param usedString String @param subString should be removed from this string.
	/// @param removedString String which should be removed.
	/// @return Returns the new string without the removed string.
	function RemoveString takes string usedString, string removedString returns string
		local integer position = FindString(usedString, removedString)
		if (position != -1) then
			return RemoveSubString(usedString, position, StringLength(removedString))
		endif
		return usedString
	endfunction

	/// Inserts string @param insertedString into string @param usedString at position @param position and returns the resulting string.
	/// @state checked
	/// @param usedString String into which string @param insertedString should be inserted.
	/// @param position Position where string @param insertedString should be inserted.
	/// @param insertedString String which should be inserted into string @param usedString.
	/// @return Returns the new string with the inserted string.
	function InsertString takes string usedString, integer position, string insertedString returns string
		local string result = ""
		debug call StringPositionDebug(usedString, position)
		if (position > 0) then
			set result = SubString(usedString, 0, position)
		endif
		return result + insertedString + SubString(usedString, position, StringLength(usedString))
	endfunction

	/// Moves the sub string of string @param usedString at position @param position and with length @param length to position @param newPosition and returns the resulting string.
	/// @state checked
	/// @param usedString String in which the sub string should be moved.
	/// @param position Start position of the sub string.
	/// @param length Length of the sub string.
	/// @param newPosition Position to which the sub string should be moved.
	/// @return Returns the new string with the moved string.
	function MoveSubString takes string usedString, integer position, integer length, integer newPosition returns string
		local string result = ""
		debug call StringPositionDebug(usedString, position + length)
		debug call StringPositionDebug(usedString, newPosition)
		if (position > 0) then
			set result = SubString(usedString, 0, position)
		endif
		set result = result + SubString(usedString, newPosition, newPosition + length)
		set result = result + SubString(usedString, position, position + length)
		if (StringLength(result) < StringLength(usedString)) then
			set result = result + SubString(usedString, StringLength(result), StringLength(usedString))
		endif
		return result
	endfunction

	/// Moves string @param movedString of string @param usedString to position @param newPosition and returns the resulting string.
	/// @state checked
	/// @param usedString String in which string @param movedString should be moved.
	/// @param movedString String which should be moved.
	/// @param newPosition Position to which string @param movedString should be moved.
	/// @return Returns the new string with the moved string.
	function MoveString takes string usedString, string movedString, integer newPosition returns string
		local integer position = FindString(usedString, movedString)
		if (position != -1) then
			return MoveSubString(usedString, position, StringLength(movedString), newPosition) 
		endif
		return usedString
	endfunction

	/// Reverses a string that it will be written backwards and returns the resulting string.
	/// @state checked
	/// @param usedString String which should be reversed.
	/// @return Returns the new reversed string.
	function ReverseString takes string usedString returns string
		local integer i
		local string result
		set i = StringLength(usedString)
		loop
			exitwhen (i == 1)
			set result = result + SubString(usedString, i - 1, i)
			set i = i - 1
		endloop
		return result
	endfunction

endlibrary