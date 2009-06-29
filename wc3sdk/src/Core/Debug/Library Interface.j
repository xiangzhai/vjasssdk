library ALibraryCoreDebugInterface requires ALibraryCoreDebugMisc

	globals
		private multiboard array usedMultiboard
	endglobals

	private function MultiboardBarValueFunctionGetValue takes AMultiboardBar multiboardBar returns real
		return 100.0
	endfunction

	private function MultiboardBarValueFunctionGetMaxValue takes AMultiboardBar multiboardBar returns real
		return 100.0
	endfunction

	private function GetMultiboardBarDebug takes player user returns nothing
		local AMultiboardBar multiboardBar
		if (usedMultiboard[GetPlayerId(user)] == null) then
			set usedMultiboard[GetPlayerId(user)] = CreateMultiboard()
			//function ShowMultiboardForPlayer takes player user, multiboard usedMultiboard, boolean show
			call ShowMultiboardForPlayer(user, usedMultiboard[GetPlayerId(user)], true)
			//static method create takes multiboard usedMultiboard, integer column, integer row, integer length, real refreshRate, boolean horizontal, real value, real maxValue, AMultiboardBarValueFunction valueFunction, AMultiboardBarValueFunction maxValueFunction returns AMultiboardBar
			set multiboardBar = AMultiboardBar.create(usedMultiboard[GetPlayerId(user)], 0, 0, 10, 1.0, true, 100.0, 100.0, AMultiboardBarValueFunction.MultiboardBarValueFunctionGetValue, AMultiboardBarValueFunction.MultiboardBarValueFunctionGetMaxValue)
			//method setAllIcons takes string icon, boolean valueIcon
			call multiboardBar.setAllIcons("ReplaceableTextures\\CommandButtons\\BTNAlleriaFlute.blp", true)
			call multiboardBar.setAllIcons("ReplaceableTextures\\CommandButtons\\BTNSpellShieldAmulet.blp", false)
			call multiboardBar.refresh()
		endif
	endfunction

	function AInterfaceDebug takes nothing returns nothing
		local player triggerPlayer = GetTriggerPlayer()
		call GetMultiboardBarDebug(triggerPlayer)
		set triggerPlayer = null
	endfunction

endlibrary