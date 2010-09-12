/**
* @author Tamino Dauth
* @todo Test!
*/
library ALibraryObjectsCreatures

	/// Creatures.mpq - Models/Creatures/Animals/*.mdx
	function APreloadCreaturesAnimalsModels takes nothing returns nothing
		call Preload("Models\\Creatures\\Animals\\Baerenform.mdx")
		call Preload("Models\\Creatures\\Animals\\Baer.mdx")
		call Preload("Models\\Creatures\\Animals\\Bear.mdx")
		call Preload("Models\\Creatures\\Animals\\Bee.mdx")
		call Preload("Models\\Creatures\\Animals\\Credits.txt")
		call Preload("Models\\Creatures\\Animals\\Deer.mdx")
		call Preload("Models\\Creatures\\Animals\\DireWolf.mdx")
		call Preload("Models\\Creatures\\Animals\\Drache.mdx")
		call Preload("Models\\Creatures\\Animals\\Duck.mdx")
		call Preload("Models\\Creatures\\Animals\\ElkFemale.mdx")
		call Preload("Models\\Creatures\\Animals\\ElkMale.mdx")
		call Preload("Models\\Creatures\\Animals\\Giraffe.mdx")
		call Preload("Models\\Creatures\\Animals\\Gorilla.mdx")
		call Preload("Models\\Creatures\\Animals\\Horse.mdx")
		call Preload("Models\\Creatures\\Animals\\Katze.mdx")
		call Preload("Models\\Creatures\\Animals\\Krabbe.mdx")
		call Preload("Models\\Creatures\\Animals\\Kraehe.mdx")
		call Preload("Models\\Creatures\\Animals\\Kuh.mdx")
		call Preload("Models\\Creatures\\Animals\\LindormrDragon.mdx")
		call Preload("Models\\Creatures\\Animals\\LindormrDragon_Portrait.mdx")
		call Preload("Models\\Creatures\\Animals\\Ratte.mdx")
		call Preload("Models\\Creatures\\Animals\\Sheep.mdx")
		call Preload("Models\\Creatures\\Animals\\Spinne.mdx")
		call Preload("Models\\Creatures\\Animals\\Spinne_Portrait.mdx")
		call Preload("Models\\Creatures\\Animals\\Tiger.mdx")
		call Preload("Models\\Creatures\\Animals\\Vogelspinne.mdx")
		call Preload("Models\\Creatures\\Animals\\Widder.mdx")
		call Preload("Models\\Creatures\\Animals\\Wildschwein.mdx")
		call Preload("Models\\Creatures\\Animals\\Wolfsform.mdx")
		call Preload("Models\\Creatures\\Animals\\Wolfsform_Portrait.mdx")
	endfunction

	/// Creatures.mpq - Models/Creatures/Humans/*.mdx
	function APreloadCreaturesHumansModels takes nothing returns nothing
		//call Preload("Models\\Ambient\\Grassland\\Altar.mdx")
	endfunction

	/// Creatures.mpq - Textures/Creatures/Humans/*.mdx
	function APreloadCreaturesHumansTextures takes nothing returns nothing
	endfunction

endlibrary