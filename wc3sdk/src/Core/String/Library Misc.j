library ALibraryCoreStringMisc requires optional ALibraryCoreDebugMisc

	debug function StringPositionDebug takes string whichString, integer position returns nothing
		debug if ((position < 0) or (position >= StringLength(whichString))) then
			debug call Print("StringPositionDebug: Wrong position value: " + I2S(position) + ".")
		debug endif
	debug endfunction

	/**
	* Searches for position of string @param searchedString in string @param whichString.
	* If @param searchedString is not contained by @param whichString function will return -1 otherwise it will return its position.
	* @param whichString String which should contain searched string.
	* @param searchedString String which is searched.
	* @return If the string was found it returns its position otherwise it will return -1.
	*/
	function FindString takes string whichString, string searchedString returns integer
		local integer i
		debug if (StringLength(whichString) < StringLength(searchedString)) then
			debug call Print("FindString: Used string is lesser than searched string.")
		debug endif
		set i = 0
		loop
			exitwhen (i + StringLength(searchedString) > StringLength(whichString))
			if (SubString(whichString, i, i + StringLength(searchedString)) == searchedString) then
				return i
			endif
			set i = i + 1
		endloop
		return -1
	endfunction

	/**
	* Replaces a part of a string and returns the resulting string.
	* @param whichString String which is used for replacement operation.
	* @param position Position where replacement should start.
	* @param replacingString String which should replace a sub string of string @param whichString.
	* @return Returns the new string with the replaced sub string.
	*/
	function ReplaceSubString takes string whichString, integer position, string replacingString returns string
		local string result = ""
		debug call StringPositionDebug(whichString, position)
		set result = SubString(whichString, 0, position) + replacingString
		if (position + StringLength(replacingString) < StringLength(whichString)) then
			set result = result + SubString(whichString, position + StringLength(replacingString), StringLength(whichString))
		endif
		return result
	endfunction

	/**
	* Replaces string @param replacedString in string @param whichString by string @param replacingString and returns the resulting string.
	* Note that @param replacedString and @replacingString do not have to have the same size. The function removes @param replacedString and inserts @param replacingString afterwards at @param replacedString's old position.
	* @param whichString String which contains the sub string @param replacedString.
	* @param replacedString Sub string of string @param whichString which should be replaced.
	* @param replacingString String which should replace @param replacedString.
	* @return Returns the new string with the replaced sub string.
	*/
	function ReplaceString takes string whichString, string replacedString, string replacingString returns string
		local integer position = FindString(whichString, replacedString)
		if (position == -1) then
			return whichString
		endif
		return SubString(whichString, 0, position) + replacingString + SubString(whichString, position + StringLength(replacedString), StringLength(whichString))
	endfunction

	/**
	* Removes the sub string at position @param position with length @param length of string @param whichString and returns the resulting string.
	* @param whichString String which is used for removing the sub string.
	* @param position Position of the sub string which should be removed.
	* @param length Length of the sub string which should be removed.
	* @return Returns the new string without the sub string string.
	*/
	function RemoveSubString takes string whichString, integer position, integer length returns string
		debug call StringPositionDebug(whichString, position)
		debug call StringPositionDebug(whichString, position + length - 1)
		return SubString(whichString, 0, position) + SubString(whichString, position + length, StringLength(whichString))
	endfunction

	/**
	* Removes string @param removedString from string @param whichString and returns the resulting string.
	* @param whichString String from which @param subString should be removed.
	* @param removedString String which should be removed.
	* @return Returns the new string without the removed string.
	*/
	function RemoveString takes string whichString, string removedString returns string
		local integer position = FindString(whichString, removedString)
		if (position == -1) then
			debug call Print("RemoveString: String \"" + whichString + "\" does not contain string \"" + removedString + "\".")
			return whichString
		endif
		return RemoveSubString(whichString, position, StringLength(removedString))
	endfunction

	/**
	* Inserts string @param insertedString into string @param whichString at position @param position and returns the resulting string.
	* @param whichString String into which string @param insertedString should be inserted.
	* @param position Position where string @param insertedString should be inserted.
	* @param insertedString String which should be inserted into string @param whichString at position @param position.
	* @return Returns the new string with the inserted string.
	*/
	function InsertString takes string whichString, integer position, string insertedString returns string
		local string result = ""
		debug call StringPositionDebug(whichString, position)
		return SubString(whichString, 0, position) + insertedString + SubString(whichString, position, StringLength(whichString))
	endfunction

	/**
	* Moves the sub string of string @param whichString at position @param position and with length @param length to position @param newPosition and returns the resulting string.
	* @param whichString String in which the sub string should be moved.
	* @param position Start position of the sub string.
	* @param length Length of the sub string.
	* @param newPosition Position to which the sub string should be moved.
	* @return Returns the new string with the moved string.
	*/
	function MoveSubString takes string whichString, integer position, integer length, integer newPosition returns string
		local string subString = ""
		local string result = ""
		debug call StringPositionDebug(whichString, position)
		debug call StringPositionDebug(whichString, position + length - 1)
		debug call StringPositionDebug(whichString, newPosition)
		debug call StringPositionDebug(whichString, newPosition + length - 1)
		set subString = SubString(whichString, position, position + length)
		set result = RemoveSubString(whichString, position, position + length)
		return InsertString(result, newPosition, subString)
	endfunction

	/**
	* Moves string @param movedString of string @param whichString to position @param newPosition and returns the resulting string.
	* @param whichString String in which string @param movedString should be moved.
	* @param movedString String which should be moved.
	* @param newPosition Position to which string @param movedString should be moved.
	* @return Returns the new string with the moved string.
	* @todo Doesn't work correctly. Missing rest of string when moving something.
	*/
	function MoveString takes string whichString, string movedString, integer newPosition returns string
		local integer position = FindString(whichString, movedString)
		if (position == -1) then
			debug call Print("MoveString: String \"" + whichString + "\" does not contain string \"" + movedString + "\".")
			return whichString
		endif
		return MoveSubString(whichString, position, StringLength(movedString), newPosition)
	endfunction

	/**
	* Reverses a string that it will be written backwards and returns the resulting string.
	* @param whichString String which should be reversed.
	* @return Returns the new reversed string.
	*/
	function ReverseString takes string whichString returns string
		local integer i = StringLength(whichString)
		local string result = ""
		loop
			exitwhen (i == 0)
			set result = result + SubString(whichString, i - 1, i)
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