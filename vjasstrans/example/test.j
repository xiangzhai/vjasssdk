library StructTalksTalkTellborn requires Asl, Quests

	private function startPageAction takes ATalk talk returns nothing
		call talk.showInfo(0)
		call talk.showInfo(1)
		call talk.showInfo(2)
		call talk.showInfo(3)
		call talk.showInfo(4)
		call talk.showInfo(5)
		call talk.show()
	endfunction

	//Hallo.
	private function buttonAction0 takes AInfo info returns nothing
		call speech(info, false, tr("STRING_0"), null)
		call speech(info, true, tr("STRING_1"), null)
		call speech(info, false, tr("STRING_2"), null)
		call speech(info, true, tr("STRING_3"), null)
		call speech(info, true, tr("STRING_4"), null)
		call speech(info, true, tr("STRING_5"), null)
		call speech(info, true, tr("STRING_6"), null)
		if (true) then //Fulco hats erzählt
			call speech(info, false, tr("STRING_7"), null)
			call speech(info, true, tr("STRING_8"), null)
		else
			call speech(info, false, tr("STRING_9"), null)
			call speech(info, true, tr("STRING_10"), null)
			call speech(info, true, tr("STRING_11"), null)
			call speech(info, true, tr("STRING_12"), null)
			//Charakter lacht/grinst
		endif
		call info.talk().showStartPage()
	endfunction
	
	//(Charakter hat vom Missgeschick erfahren)
	private function buttonCondition1 takes AInfo info returns boolean
		return true
	endfunction

	//Kann ich dir irgendwie bei deinem Missgeschick helfen?
	private function buttonAction1 takes AInfo info returns nothing
		call speech(info, false, tr("STRING_13"), null)
		call speech(info, true, tr("STRING_14"), null)
		call speech(info, true, tr("STRING_15"), null)
		call speech(info, true, tr("STRING_16"), null)
		call info.talk().showInfo(6)
		call info.talk().showInfo(7)
		call info.talk().showStartPage()
	endfunction
	
	//(Charakter hat alle Zutaten im Inventar)
	private function buttonCondition2 takes AInfo info returns boolean
		return true
	endfunction

	//Hier hast du deine Zutaten.
	private function buttonAction2 takes AInfo info returns nothing
		call speech(info, false, tr("STRING_17"), null)
		call speech(info, true, tr("STRING_18"), null)
		call speech(info, true, tr("STRING_19"), null)
		call speech(info, true, tr("STRING_20"), null)
		call QuestMyFriendTheBear.characterQuest(info.talk().character()).setState(AAbstractQuest.stateCompleted)
		call info.talk().showStartPage()
	endfunction

	//Was trägst du da für Kleidung?
	private function buttonAction3 takes AInfo info returns nothing
		call speech(info, false, tr("STRING_21"), null)
		call speech(info, true, tr("STRING_22"), null)
		call speech(info, true, tr("STRING_23"), null)
		call info.talk().showStartPage()
	endfunction
	
	//(Charakter hat negative Zaubereffekte an sich)
	private function buttonCondition4 takes AInfo info returns boolean
		local unit characterUnit = info.talk().character().unit()
		local boolean result = UnitCountBuffsEx(characterUnit, true, false, true, false, false, false, false) > 0
		set characterUnit = null
		return result
	endfunction
	
	//Böse Geister plagen mich!
	private function buttonAction4 takes AInfo info returns nothing
		local unit npc = info.talk().unit()
		local unit characterUnit = info.talk().character().unit()
		call speech(info, false, tr("STRING_24"), null)
		call SetUnitAnimation(npc, "Spell")
		call ResetUnitAnimation(npc)
		call speech(info, true, tr("STRING_25"), null)
		call UnitRemoveBuffsEx(characterUnit, false, true, true, false, false, false, false)
		set npc = null
		set characterUnit = null
		call info.talk().showStartPage()
	endfunction
	
	//Immer doch. Nur her mit der Liste!
	private function buttonAction1_0 takes AInfo info returns nothing
		local unit characterUnit = info.talk().character().unit()
		local item createdItem
		call speech(info, false, tr("STRING_26"), null)
		call speech(info, true, tr("STRING_27"), null)
		set createdItem = UnitAddItemById(characterUnit, 'I00X')
		set characterUnit = null
		set createdItem = null
		call QuestMyFriendTheBear.characterQuest(info.talk().character())
		call info.talk().showStartPage()
	endfunction
	
	//Eigentlich gefällt mir dein Freund ganz gut, so wie er ist. Schön flauschig, der Bär.
	private function buttonAction1_1 takes AInfo info returns nothing
		call speech(info, false, tr("STRING_28"), null)
		call speech(info, true, tr("STRING_29"), null)
		call info.talk().showStartPage()
	endfunction

	struct TalkTellborn extends ATalk

		public static method create takes nothing returns thistype
			local thistype this = thistype.allocate(gg_unit_n011_0113, startPageAction)

			//start page
			call this.addInfo(false, false, 0, buttonAction0, tr("STRING_30")) //0
			call this.addInfo(false, false, buttonCondition1, buttonAction1, tr("STRING_31")) //1
			call this.addInfo(false, false, buttonCondition2, buttonAction2, tr("STRING_32")) //2
			call this.addInfo(false, false, 0, buttonAction3, tr("STRING_33")) //3
			call this.addInfo(true, false, buttonCondition4, buttonAction4, tr("STRING_34")) //4
			call this.addExitButton() //5
			
			//info 1
			call this.addInfo(false, false, 0, buttonAction1_0, tr("STRING_35")) //6
			call this.addInfo(false, false, 0, buttonAction1_1, tr("STRING_36")) //7

			return this
		endmethod
	endstruct

endlibrary
