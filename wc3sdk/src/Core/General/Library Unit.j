library ALibraryCoreGeneralUnit

	/// Sicheres iterieren per FirstOfGroup
	///
	/// Heyhey,
	///
	/// dieser Bug (oder Feature, je nachdem wie mans nimmt) hat mich etwa 4 Stunden debuggen 
	/// gekostet.
	///
	/// Hintergrund: Wie ihr wisst kann man ja über eine Unitgroup iterieren (und diese dabei leeren) 
	/// (Die gruppe nenne ich einfach mal g):
	///
	/// code:
	/// local unit u
	/// loop
	/// 	set u = FirstOfGroup(g)
	/// 	exitwhen u == null
	///
	/// 	//HIER das mit der unit machen was man machen will
	///
	/// 	call GroupRemoveUnit(g,u)
	/// endloop
	///
	/// Tja, nur blöd dass das buggy ist, sobald einmal eine unit in der Gruppe aus dem Spiel 
	/// entfernt wurde ohne sie vorher ordnungsgemäß aus der Gruppe zu removen. Das wusste ich bisher 
	/// auch nicht.
	///
	/// Beispiel:
	/// Wir adden in eine Gruppe 4 units. Dann entfernen wir die erste geaddete unit per 
	/// RemoveUnit(...) aus dem Spiel. Nun ist die Gruppe kaputt (zumindest für das Iterieren per 
	/// FirstOfGroup()). Denn FirstOfGroup(g) wird nun null liefern, die schleife endet also sofort 
	/// obwohl drei units in der Group sind. Denn die "Lücke" die die unit hinterlassen hat bleibt in 
	/// der Group. Und das blödeste: Diese lücke können wir auch nicht mehr per call 
	/// GroupRemoveUnit(FirstOfGroup()) entfernen, da ja FoG nun null zurückliefert und 
	/// GroupRemoveUnit(null) nichts macht.
	///
	///
	/// ALSO VORSICHT! Das FirstOfGroup(...) null zurückliefert heisst nicht unbedingt, dass die 
	/// Gruppe leer ist!
	///
	/// Deshalb hab ich mir eine kleine Funktion geschrieben die äquivalent zu einem Aufruf von 
	/// FirstOfGroup ist, aber solche Lücken erkennt und die Gruppe automatisch repariert sobald sie 
	/// auf so eine Lücke trifft, hier ist sie:
	///
	/// Die Idee:
	/// Erstmal führt die Funktion ein normales FirstOfGroup auf die gewünschte Gruppe aus. Liefert 
	/// dieser aufruf NICHT null, klappt ja alles perfekt, dann gibt sie einfach die unit zurück. 
	/// Liefert der Aufruf aber null können zwei Fälle eingetreten sein:
	///
	/// 1.) Die Group ist tatsächlich empty
	/// 2.) Wir sind auf eine Lücke gestoßen die repariert werden muss.
	///
	/// Das überprüfen wir einfach indem wir ein GroupIsEmpty-derivat aufrufen. Liefert es true /// 
	/// zurück war die gruppe wirklich leer und wir können null returnen. sonst reparieren wir sie, 
	/// das geht so:
	/// Wir kopieren per ForGroup alle units in eine swap gruppe. Dabei werden lücken nicht 
	/// mitkopiert, da ForGroup so schlau ist und solche lücken beim iterieren nicht beachtet. Dann 
	/// leeren wir die gruppe und kopieren einfach die units aus der swapgruppe zurück und fertig ist 
	/// die reparierte Gruppe auf die wir nun ein erneutes FirstOfGroup aufrufen um die wirkliche 
	/// first unit zu ermitteln die wir zurück geben.
	///
	/// Ich empfehle euch dringend diese Funktion zu benutzen wenn ihr über eine Group per 
	/// FirstOfGroup iterieren wollt, aber nicht sicher sein könnt, dass keine unit jemals aus dem 
	/// Spiel entfernt wurde (was auch nach dem normalen tod und dekay automatisch passiert) die in 
	/// der Gruppe war.
	///
	/// Hoffe ich kann euch damit die Stunden des Bugfixen die ich hatte ersparen...
	/// @author gexxo
	/// @param g Used unit group.
	/// @return Returns the first unit of group g. If the group is empty it will return null.
	function FirstOfGroupSave takes group g returns unit
		local unit u = FirstOfGroup(g) //Try a normal first of group
		local group swap
		
		// If the result is null there may be gaps in the group
		if u == null then
			//Check if the group is empty. If it is not, then there must be gaps -> repair
			set bj_isUnitGroupEmptyResult = true
			call ForGroup(g, function IsUnitGroupEmptyBJEnum)
			if not bj_isUnitGroupEmptyResult then
				//** Repair the group **
				set swap = CreateGroup()
				call GroupAddGroup(g,swap) //Add all units to a swapping group
				call GroupClear(g) //Clear the buggy group hence removing the gaps
				call GroupAddGroup(swap,g) //Put the units back in from the swapping group
				
				//Collect garbage
				call DestroyGroup(swap)
				set swap = null
				
				//Do another FirstOfGroup to gain the real first unit
				set u = FirstOfGroup(g)
			endif
		endif
		
		return u //Return the unit we wanted   
	endfunction
	
	/// @author Tamino Dauth
	/// @return Missing life of unit @param usedUnit.
	function GetUnitMissingLife takes unit usedUnit returns real
		return GetUnitState(usedUnit, UNIT_STATE_MAX_LIFE) -  GetUnitState(usedUnit, UNIT_STATE_LIFE)
	endfunction
	
	/// @author Tamino Dauth
	/// @return Missing life of unit @param usedUnit in percent.
	function GetUnitMissingLifePercent takes unit usedUnit returns real
		return 100.0 - GetUnitStatePercent(usedUnit, UNIT_STATE_LIFE, UNIT_STATE_MAX_LIFE)
	endfunction

	/// @author Tamino Dauth
	/// @return Missing mana of unit @param usedUnit.
	function GetUnitMissingMana takes unit usedUnit returns real
		return GetUnitState(usedUnit, UNIT_STATE_MAX_MANA) -  GetUnitState(usedUnit, UNIT_STATE_MANA)
	endfunction
	
	/// @author Tamino Dauth
	/// @return Missing mana of unit @param usedUnit in percent.
	function GetUnitMissingManaPercent takes unit usedUnit returns real
		return 100.0 - GetUnitStatePercent(usedUnit, UNIT_STATE_MANA, UNIT_STATE_MAX_MANA)
	endfunction

endlibrary