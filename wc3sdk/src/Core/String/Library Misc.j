/// @todo Not tested yet!
/// The SubString functions are concepted that they doesn't need each other.
library ALibraryCoreStringMisc requires ALibraryCoreDebugMisc

	/// Debug for testing over- and underflow.
	debug function StringPositionDebug takes string usedString, integer position returns nothing
		debug if ((position < 0) or (position >= StringLength(usedString))) then
			debug call Print("StringPositionDebug: Wrong position value: " + I2S(position) + ".")
		debug endif
	debug endfunction

	/**
	* Searches for the position of string @param searchedString in string @param usedString.
	* @state checked
	* If @param searchedString is not contained by @param usedString function will return -1 otherwise it will return the position.
	* @param usedString String which should contain the searched string.
	* @param searchedString String which is searched.
	* @return If the string was found it will return its position otherwise it will return -1.
	*/
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

	/**
	* Replaces a part of a string and returns the resulting string.
	* @state checked
	* @param usedString String which contains the sub string.
	* @param position Start position of the sub string.
	* @param replacingString String which should replace the sub string.
	* @return Returns the new string with the replaced sub string.
	*/
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

	/**
	* Replaces string @param replacedString in string @param usedString by string @param replacingString and returns the resulting string.
	* @state checked
	* @param usedString String which contains the sub string @param replacedString.
	* @param replacedString Sub string of string @param usedString which should be replaced.
	* @param replacingString String which should replace @param replacedString.
	* @return Returns the new string with the replaced sub string.
	*/
	function ReplaceString takes string usedString, string replacedString, string replacingString returns string
		local integer position = FindString(usedString, replacedString)
		if (position != -1) then
			return ReplaceSubString(usedString, position, replacingString)
		endif
		return usedString
	endfunction

	/**
	* Removes the sub string at position @param position with length @param length of string @param usedString and returns the resulting string.
	* @state checked
	* @param usedString String which contains the sub string.
	* @param position Position of the sub string.
	* @param length Length of the sub string.
	* @return Returns the new string with the replaced string.
	*/
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

	/**
	* Removes string @param removedString from string @param subString and returns the resulting string. 
	* @state checked
	* @param usedString String @param subString should be removed from this string.
	* @param removedString String which should be removed.
	* @return Returns the new string without the removed string.
	*/
	function RemoveString takes string usedString, string removedString returns string
		local integer position = FindString(usedString, removedString)
		if (position != -1) then
			return RemoveSubString(usedString, position, StringLength(removedString))
		endif
		return usedString
	endfunction

	/**
	* Inserts string @param insertedString into string @param usedString at position @param position and returns the resulting string.
	* @state checked
	* @param usedString String into which string @param insertedString should be inserted.
	* @param position Position where string @param insertedString should be inserted.
	* @param insertedString String which should be inserted into string @param usedString.
	* @return Returns the new string with the inserted string.
	*/
	function InsertString takes string usedString, integer position, string insertedString returns string
		local string result = ""
		debug call StringPositionDebug(usedString, position)
		if (position > 0) then
			set result = SubString(usedString, 0, position)
		endif
		return result + insertedString + SubString(usedString, position, StringLength(usedString))
	endfunction

	/**
	* Moves the sub string of string @param usedString at position @param position and with length @param length to position @param newPosition and returns the resulting string.
	* @state checked
	* @param usedString String in which the sub string should be moved.
	* @param position Start position of the sub string.
	* @param length Length of the sub string.
	* @param newPosition Position to which the sub string should be moved.
	* @return Returns the new string with the moved string.
	*/
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

	/**
	* Moves string @param movedString of string @param usedString to position @param newPosition and returns the resulting string.
	* @state checked
	* @param usedString String in which string @param movedString should be moved.
	* @param movedString String which should be moved.
	* @param newPosition Position to which string @param movedString should be moved.
	* @return Returns the new string with the moved string.
	*/
	function MoveString takes string usedString, string movedString, integer newPosition returns string
		local integer position = FindString(usedString, movedString)
		if (position != -1) then
			return MoveSubString(usedString, position, StringLength(movedString), newPosition) 
		endif
		return usedString
	endfunction

	/**
	* Reverses a string that it will be written backwards and returns the resulting string.
	* @state checked
	* @param usedString String which should be reversed.
	* @return Returns the new reversed string.
	*/
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
	
	/**
	* Basic case (in)sensitive pattern matching.
	* Supported wildcard characters:
	* * - matches 0 or more any characters
	* ? - matches exactly 1 any character
	* # - matches any digit, 0-9
	* [list] - matches any character in <list> (l, i, s and t in this example)
	* [!list] - matches any character that isn't in the list
	* Use \\* or \\? or \\[ to match a * or ? or [ respectively.
	* To get a ] in a list, put it as first character of the list.
	* To get a ! in a list, don't put it first.
	* By common convention, special characters *, ? and # have no meaning when used in a list.
	* If "case" is true, the matching is case sensitive.
	* @author AceHart
	* @link http://www.wc3c.net/showthread.php?t=102026
	*/
	function StringMatch takes string text, string mask, boolean case returns boolean
		local string a
		local string b
		local string x
		local string y
		local integer i = 0
		local integer j = 0
		local boolean m

		if case then
			set a = text
			set b = mask
		else
			set a = StringCase(text, true)
			set b = StringCase(mask, true)
		endif

		set x = SubString(a, 0, 1)
		set y = SubString(b, 0, 1)
		if x == null and y == null then
			return false
		endif

		loop
			if y == null then
				return x == null
			elseif y == "#" then
				exitwhen not (x == "0" or x == "1" or x == "2" or x == "3" or x == "4" or x == "5" or x == "6" or x == "7" or x == "8" or x == "9")
			elseif y == "\\" then
				set j = j + 1
				set y = SubString(b, j, j + 1)
				exitwhen x != y
			elseif y == "?" then
				// nothing to do
			elseif y == "[" then
				set j = j + 1
				set y = SubString(b, j, j + 1)
				exitwhen y == null
				set m = false
				if y == "!" then
					loop
						set j = j + 1
						set y = SubString(b, j, j + 1)
						if y == null then
							return false
						endif
						exitwhen y == "]"
						if x == y then
							set m = true
						endif
					endloop
					exitwhen y != "]"
					exitwhen m == true
				else
					loop
						if y == null then
							return false
						endif
						if x == y then
							set m = true
						endif
						set j = j + 1
						set y = SubString(b, j, j + 1)
						exitwhen y == "]"
					endloop
					exitwhen y != "]"
					exitwhen m == false
				endif
			elseif y == "*" then
				loop
					set j = j + 1
					set y = SubString(b, j, j + 1)
					exitwhen y != "*"
				endloop
		//			if y == null then
				if StringLength(y) < 1 then
					return true
				endif
				set a = SubString(a, i, StringLength(a))
				set b = SubString(b, j, StringLength(b))
				loop
					exitwhen a == null
					if StringMatch(a, b, case) then
					return true
					endif
					set i = i + 1
					set a = SubString(a, 1, StringLength(a))
				endloop
				return false
			else
				exitwhen x != y
			endif

			set i = i + 1
			set j = j + 1
			set x = SubString(a, i, i + 1)
			set y = SubString(b, j, j + 1)
		endloop
		return false
	endfunction

endlibrary