/// Do not use this library.
/// @author Tamino Dauth
/// @state untested
library ALibraryObjectsDoodads

	//Tileset List
	//* A Ashenvale
	//* B Barrens
	//* C Felwood
	//* D Dungeon
	//* F Lordaeron Fall
	//* G Underground
	//* L Lordaeron Summer
	//* N Northrend
	//* Q Village Fall
	//* V Village
	//* W Lordaeron Winter
	//* X Dalaran
	//* Y Cityscape
	//* Z Sunken Ruins
	//* I Icecrown
	//* J Dalaran Ruins
	//* O Outland
	//* K Black Citadel

	//example: //! runtextmacro A_DOODAD("Z000", "Pilz", "Pilz", "Grasland", "1.0")
	
	//File parameter without the mdx postfix
	
	//! textmacro A_AMBIENT_DOODAD takes ID, NAME, FILE, TERRAIN, DEFAULTSCALE
		//! external OBJMERGE w3d APms $ID$ category "E" tilesets "_" tilesetSpecific 1 dir "Models\Ambient\$TERRAIN$" file "$FILE$" comment "Test comment" name "$NAME$" doodClass "_" soundLoop "_" selSize 0 defScale $DEFAULTSCALE$ minScale 0.01 maxScale 100 canPlaceRandScale 1 useClickHelper 0 maxPitch -1 maxRoll -1 visRadius 50 walkable 0 numVar 1 onCliffs 1 onWater 1 floats 0 shadow 1 showInFog 1 animInFog 1 fixedRot -1 pathTex "none" showInMM 0 useMMColor 0 MMRed 0 MMGreen 0 MMBlue 0 InBeta 0
	//! endtextmacro
	
	//Grasland
	////! runtextmacro A_AMBIENT_DOODAD("Z000", "Amboss", "Amboss", "Grasland", "1")
	
	//Island
	
	//Mountains
	
	//Special - Extra
	
	//Swamp
	
	function APreloadGraslandModels takes nothing returns nothing
	endfunction
	
	function APreloadGraslandTextures takes nothing returns nothing
	endfunction

endlibrary