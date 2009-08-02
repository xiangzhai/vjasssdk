/// @brief This library is obsolet! Use @function GetHandleId or other native functions.
library ALibraryCoreGeneralConversion
/*
	//! textmacro AGeneralConversionMacro takes TYPE, TYPENAME
		/// There are already some native conversion functions for the integer type.
		function I2$TYPENAME$ takes integer Integer returns $TYPE$
			return Integer
			return null
		endfunction
		
		function $TYPENAME$2I takes $TYPE$ $TYPENAME$ returns integer
			return $TYPENAME$
			return 0
		endfunction
		
		function B2$TYPENAME$ takes boolean Boolean returns $TYPE$
			return Boolean
			return null
		endfunction
		
		function $TYPENAME$2B takes $TYPE$ $TYPENAME$ returns boolean
			return $TYPENAME$
			return false
		endfunction
		
		function R2$TYPENAME$ takes real Real returns $TYPE$
			return Real
			return null
		endfunction
		
		function $TYPENAME$2R takes $TYPE$ $TYPENAME$ returns real
			return $TYPENAME$
			return 0.00
		endfunction
		
		function S2$TYPENAME$ takes string String returns $TYPE$
			return String
			return null
		endfunction
		
		function $TYPENAME$2S takes $TYPE$ $TYPENAME$ returns string
			return $TYPENAME$
			return ""
		endfunction
	//! endtextmacro

	//! runtextmacro AGeneralConversionMacro("handle", "H") //Kurze Variante
	//! runtextmacro AGeneralConversionMacro("event", "Event")
	//! runtextmacro AGeneralConversionMacro("player", "P") //Kurze Variante, da es bereits die Funktion "Player" gibt.
	//! runtextmacro AGeneralConversionMacro("widget", "Widget")
	//! runtextmacro AGeneralConversionMacro("unit", "Unit")
	//! runtextmacro AGeneralConversionMacro("destructable", "Destructable")
	//! runtextmacro AGeneralConversionMacro("item", "Item")
	//! runtextmacro AGeneralConversionMacro("ability", "Ability")
	//! runtextmacro AGeneralConversionMacro("buff", "Buff")
	//! runtextmacro AGeneralConversionMacro("force", "Force")
	//! runtextmacro AGeneralConversionMacro("group", "Group")
	//! runtextmacro AGeneralConversionMacro("trigger", "Trigger")
	//! runtextmacro AGeneralConversionMacro("triggercondition", "TriggerCondition")
	//! runtextmacro AGeneralConversionMacro("triggeraction", "TriggerAction")
	//! runtextmacro AGeneralConversionMacro("timer", "Timer")
	//! runtextmacro AGeneralConversionMacro("location", "L") //Kurze Variante, da es bereits die Funktion "Location" gibt.
	//! runtextmacro AGeneralConversionMacro("region", "RG") //Kurze Variante, da es bereits die Funktion "Region" gibt.
	//! runtextmacro AGeneralConversionMacro("rect", "RC") //Kurze Variante, da es bereits die Funktion "Rect" gibt.
	//! runtextmacro AGeneralConversionMacro("boolexpr", "BooleanExpression")
	//! runtextmacro AGeneralConversionMacro("sound", "Sound")
	//! runtextmacro AGeneralConversionMacro("conditionfunc", "ConditionFunction")
	//! runtextmacro AGeneralConversionMacro("filterfunc", "FilterFunction")
	//! runtextmacro AGeneralConversionMacro("unitpool", "UnitPool")
	//! runtextmacro AGeneralConversionMacro("itempool", "ItemPool")
	//! runtextmacro AGeneralConversionMacro("race", "Race")
	//! runtextmacro AGeneralConversionMacro("alliancetype", "AllianceType")
	//! runtextmacro AGeneralConversionMacro("racepreference", "RacePreference")
	//! runtextmacro AGeneralConversionMacro("gamestate", "GameState")
	//! runtextmacro AGeneralConversionMacro("igamestate", "IGameState")
	//! runtextmacro AGeneralConversionMacro("fgamestate", "FGameState")
	//! runtextmacro AGeneralConversionMacro("playerstate", "PlayerState")
	//! runtextmacro AGeneralConversionMacro("playerscore", "PlayerScore")
	//! runtextmacro AGeneralConversionMacro("playergameresult", "PlayerGameResult")
	//! runtextmacro AGeneralConversionMacro("unitstate", "UnitState")
	//! runtextmacro AGeneralConversionMacro("aidifficulty", "AIDifficulty")
	//! runtextmacro AGeneralConversionMacro("eventid", "EventId")
	//! runtextmacro AGeneralConversionMacro("gameevent", "GameEvent")
	//! runtextmacro AGeneralConversionMacro("playerevent", "PlayerEvent")
	//! runtextmacro AGeneralConversionMacro("playerunitevent", "PlayerUnitEvent")
	//! runtextmacro AGeneralConversionMacro("unitevent", "UnitEvent")
	//! runtextmacro AGeneralConversionMacro("limitop", "LimitOP")
	//! runtextmacro AGeneralConversionMacro("widgetevent", "WidgetEvent")
	//! runtextmacro AGeneralConversionMacro("dialogevent", "DialogEvent")
	//! runtextmacro AGeneralConversionMacro("unittype", "UnitType")
	//! runtextmacro AGeneralConversionMacro("gamespeed", "GameSpeed")
	//! runtextmacro AGeneralConversionMacro("gamedifficulty", "GameDifficulty")
	//! runtextmacro AGeneralConversionMacro("gametype", "GameType")
	//! runtextmacro AGeneralConversionMacro("mapflag", "MapFlag")
	//! runtextmacro AGeneralConversionMacro("mapvisibility", "MapVisibility")
	//! runtextmacro AGeneralConversionMacro("mapsetting", "MapSetting")
	//! runtextmacro AGeneralConversionMacro("mapdensity", "MapDensity")
	//! runtextmacro AGeneralConversionMacro("mapcontrol", "MapControl")
	//! runtextmacro AGeneralConversionMacro("playerslotstate", "PlayerSlotState")
	//! runtextmacro AGeneralConversionMacro("volumegroup", "VolumeGroup")
	//! runtextmacro AGeneralConversionMacro("camerafield", "CameraField")
	//! runtextmacro AGeneralConversionMacro("camerasetup", "CameraSetup")
	//! runtextmacro AGeneralConversionMacro("playercolor", "PlayerColor")
	//! runtextmacro AGeneralConversionMacro("placement", "Placement")
	//! runtextmacro AGeneralConversionMacro("startlocprio", "StartLocationPriority")
	//! runtextmacro AGeneralConversionMacro("raritycontrol", "RarityControl")
	//! runtextmacro AGeneralConversionMacro("blendmode", "BlendMode")
	//! runtextmacro AGeneralConversionMacro("texmapflags", "TextureMapFlags")
	//! runtextmacro AGeneralConversionMacro("effect", "Effect")
	//! runtextmacro AGeneralConversionMacro("effecttype", "EffectType")
	//! runtextmacro AGeneralConversionMacro("weathereffect", "WeatherEffect")
	//! runtextmacro AGeneralConversionMacro("terraindeformation", "TerrainDeformation")
	//! runtextmacro AGeneralConversionMacro("fogstate", "FogState")
	//! runtextmacro AGeneralConversionMacro("fogmodifier", "FogModifier")
	//! runtextmacro AGeneralConversionMacro("dialog", "Dialog")
	//! runtextmacro AGeneralConversionMacro("button", "Button")
	//! runtextmacro AGeneralConversionMacro("quest", "Quest")
	//! runtextmacro AGeneralConversionMacro("questitem", "QuestItem")
	//! runtextmacro AGeneralConversionMacro("defeatcondition", "DefeatCondition")
	//! runtextmacro AGeneralConversionMacro("timerdialog", "TimerDialog")
	//! runtextmacro AGeneralConversionMacro("leaderboard", "Leaderboard")
	//! runtextmacro AGeneralConversionMacro("multiboard", "Multiboard")
	//! runtextmacro AGeneralConversionMacro("multiboarditem", "MultiboardItem")
	//! runtextmacro AGeneralConversionMacro("trackable", "Trackable")
	//! runtextmacro AGeneralConversionMacro("gamecache", "GameCache")
	//! runtextmacro AGeneralConversionMacro("version", "Version")
	//! runtextmacro AGeneralConversionMacro("itemtype", "ItemType")
	//! runtextmacro AGeneralConversionMacro("texttag", "TextTag")
	//! runtextmacro AGeneralConversionMacro("attacktype", "AttackType")
	//! runtextmacro AGeneralConversionMacro("damagetype", "DamageType")
	//! runtextmacro AGeneralConversionMacro("weapontype", "WeaponType")
	//! runtextmacro AGeneralConversionMacro("soundtype", "SoundType")
	//! runtextmacro AGeneralConversionMacro("lightning", "Lightning")
	//! runtextmacro AGeneralConversionMacro("pathingtype", "PathingType")
	//! runtextmacro AGeneralConversionMacro("image", "Image")
	//! runtextmacro AGeneralConversionMacro("ubersplat", "Ubersplat")
*/
endlibrary