/// @todo In general you should use no map script (Use the map file).
//===========================================================================
//
//	Test Map
//
//	Warcraft III map script
//	Generated by the Warcraft III World Editor
//	Date: Wed Jun 04 15:28:02 2003
//	Map Author: Blizzard Entertainment
//
//===========================================================================

//! import "Import Asl.j"
//! import "Import Dmdf.j"
//! import "Talras/Import.j"

//***************************************************************************
//*
//*  Global Variables
//*
//***************************************************************************

globals
	constant boolean A_RTC = false
	constant boolean A_JAPI = false
	constant boolean A_SYSTEMS = true
	constant boolean A_OBJECTS = false
	constant boolean A_DEBUG_HANDLES = false
	constant boolean A_DEBUG_NATIVES = false
	constant boolean A_NET = false
	constant boolean A_FPS_MOVEMENT = false
    // Generated
endglobals

function InitGlobals takes nothing returns nothing
endfunction

//***************************************************************************
//*
//*  Unit Item Tables
//*
//***************************************************************************


//***************************************************************************
//*
//*  Sounds
//*
//***************************************************************************

function InitSounds takes nothing returns nothing
endfunction

//***************************************************************************
//*
//*  Unit Creation
//*
//***************************************************************************

//===========================================================================
function CreateNeutralHostile takes nothing returns nothing
endfunction

//===========================================================================
function CreateNeutralPassiveBuildings takes nothing returns nothing
endfunction

//===========================================================================
function CreatePlayerBuildings takes nothing returns nothing
endfunction

//===========================================================================
function CreatePlayerUnits takes nothing returns nothing
endfunction

//===========================================================================
function CreateAllUnits takes nothing returns nothing
    call CreateNeutralPassiveBuildings()
    call CreatePlayerBuildings()
    call CreateNeutralHostile()
    call CreatePlayerUnits()
endfunction

//***************************************************************************
//*
//*  Triggers
//*
//***************************************************************************

//===========================================================================
function InitCustomTriggers takes nothing returns nothing
endfunction

//===========================================================================
function RunInitializationTriggers takes nothing returns nothing
endfunction

//***************************************************************************
//*
//*  Players
//*
//***************************************************************************

function InitCustomPlayerSlots takes nothing returns nothing

    // Player 0
    call SetPlayerStartLocation( Player(0), 0 )
    call SetPlayerColor( Player(0), ConvertPlayerColor(0) )
    call SetPlayerRacePreference( Player(0), RACE_PREF_HUMAN )
    call SetPlayerRaceSelectable( Player(0), true )
    call SetPlayerController( Player(0), MAP_CONTROL_USER )

    // Player 1
    call SetPlayerStartLocation( Player(1), 1 )
    call SetPlayerColor( Player(1), ConvertPlayerColor(1) )
    call SetPlayerRacePreference( Player(1), RACE_PREF_ORC )
    call SetPlayerRaceSelectable( Player(1), true )
    call SetPlayerController( Player(1), MAP_CONTROL_USER )

    // Player 2
    call SetPlayerStartLocation( Player(2), 2 )
    call SetPlayerColor( Player(2), ConvertPlayerColor(2) )
    call SetPlayerRacePreference( Player(2), RACE_PREF_UNDEAD )
    call SetPlayerRaceSelectable( Player(2), true )
    call SetPlayerController( Player(2), MAP_CONTROL_USER )

    // Player 3
    call SetPlayerStartLocation( Player(3), 3 )
    call SetPlayerColor( Player(3), ConvertPlayerColor(3) )
    call SetPlayerRacePreference( Player(3), RACE_PREF_NIGHTELF )
    call SetPlayerRaceSelectable( Player(3), true )
    call SetPlayerController( Player(3), MAP_CONTROL_USER )

    // Player 4
    call SetPlayerStartLocation( Player(4), 4 )
    call SetPlayerColor( Player(4), ConvertPlayerColor(4) )
    call SetPlayerRacePreference( Player(4), RACE_PREF_HUMAN )
    call SetPlayerRaceSelectable( Player(4), true )
    call SetPlayerController( Player(4), MAP_CONTROL_USER )

    // Player 5
    call SetPlayerStartLocation( Player(5), 5 )
    call SetPlayerColor( Player(5), ConvertPlayerColor(5) )
    call SetPlayerRacePreference( Player(5), RACE_PREF_ORC )
    call SetPlayerRaceSelectable( Player(5), true )
    call SetPlayerController( Player(5), MAP_CONTROL_USER )

    // Player 6
    call SetPlayerStartLocation( Player(6), 6 )
    call SetPlayerColor( Player(6), ConvertPlayerColor(6) )
    call SetPlayerRacePreference( Player(6), RACE_PREF_UNDEAD )
    call SetPlayerRaceSelectable( Player(6), true )
    call SetPlayerController( Player(6), MAP_CONTROL_USER )

    // Player 7
    call SetPlayerStartLocation( Player(7), 7 )
    call SetPlayerColor( Player(7), ConvertPlayerColor(7) )
    call SetPlayerRacePreference( Player(7), RACE_PREF_NIGHTELF )
    call SetPlayerRaceSelectable( Player(7), true )
    call SetPlayerController( Player(7), MAP_CONTROL_USER )

    // Player 8
    call SetPlayerStartLocation( Player(8), 8 )
    call SetPlayerColor( Player(8), ConvertPlayerColor(8) )
    call SetPlayerRacePreference( Player(8), RACE_PREF_HUMAN )
    call SetPlayerRaceSelectable( Player(8), true )
    call SetPlayerController( Player(8), MAP_CONTROL_USER )

    // Player 9
    call SetPlayerStartLocation( Player(9), 9 )
    call SetPlayerColor( Player(9), ConvertPlayerColor(9) )
    call SetPlayerRacePreference( Player(9), RACE_PREF_ORC )
    call SetPlayerRaceSelectable( Player(9), true )
    call SetPlayerController( Player(9), MAP_CONTROL_USER )

endfunction

function InitCustomTeams takes nothing returns nothing
    // Force: TRIGSTR_020
    call SetPlayerTeam( Player(0), 0 )
    call SetPlayerTeam( Player(1), 0 )
    call SetPlayerTeam( Player(2), 0 )
    call SetPlayerTeam( Player(3), 0 )
    call SetPlayerTeam( Player(4), 0 )
    call SetPlayerTeam( Player(5), 0 )
    call SetPlayerTeam( Player(6), 0 )
    call SetPlayerTeam( Player(7), 0 )
    call SetPlayerTeam( Player(8), 0 )
    call SetPlayerTeam( Player(9), 0 )

endfunction

function InitAllyPriorities takes nothing returns nothing

    call SetStartLocPrioCount( 0, 3 )
    call SetStartLocPrio( 0, 0, 5, MAP_LOC_PRIO_HIGH )
    call SetStartLocPrio( 0, 1, 6, MAP_LOC_PRIO_LOW )
    call SetStartLocPrio( 0, 2, 8, MAP_LOC_PRIO_LOW )

    call SetStartLocPrioCount( 1, 3 )
    call SetStartLocPrio( 1, 0, 2, MAP_LOC_PRIO_LOW )
    call SetStartLocPrio( 1, 1, 7, MAP_LOC_PRIO_LOW )
    call SetStartLocPrio( 1, 2, 9, MAP_LOC_PRIO_HIGH )

    call SetStartLocPrioCount( 2, 3 )
    call SetStartLocPrio( 2, 0, 1, MAP_LOC_PRIO_LOW )
    call SetStartLocPrio( 2, 1, 7, MAP_LOC_PRIO_HIGH )
    call SetStartLocPrio( 2, 2, 9, MAP_LOC_PRIO_LOW )

    call SetStartLocPrioCount( 3, 3 )
    call SetStartLocPrio( 3, 0, 4, MAP_LOC_PRIO_HIGH )
    call SetStartLocPrio( 3, 1, 6, MAP_LOC_PRIO_LOW )
    call SetStartLocPrio( 3, 2, 8, MAP_LOC_PRIO_LOW )

    call SetStartLocPrioCount( 4, 3 )
    call SetStartLocPrio( 4, 0, 3, MAP_LOC_PRIO_HIGH )
    call SetStartLocPrio( 4, 1, 6, MAP_LOC_PRIO_LOW )
    call SetStartLocPrio( 4, 2, 8, MAP_LOC_PRIO_LOW )

    call SetStartLocPrioCount( 5, 3 )
    call SetStartLocPrio( 5, 0, 0, MAP_LOC_PRIO_HIGH )
    call SetStartLocPrio( 5, 1, 6, MAP_LOC_PRIO_LOW )
    call SetStartLocPrio( 5, 2, 8, MAP_LOC_PRIO_LOW )

    call SetStartLocPrioCount( 6, 3 )
    call SetStartLocPrio( 6, 0, 0, MAP_LOC_PRIO_LOW )
    call SetStartLocPrio( 6, 1, 5, MAP_LOC_PRIO_LOW )
    call SetStartLocPrio( 6, 2, 8, MAP_LOC_PRIO_HIGH )

    call SetStartLocPrioCount( 7, 3 )
    call SetStartLocPrio( 7, 0, 1, MAP_LOC_PRIO_LOW )
    call SetStartLocPrio( 7, 1, 2, MAP_LOC_PRIO_HIGH )
    call SetStartLocPrio( 7, 2, 9, MAP_LOC_PRIO_LOW )

    call SetStartLocPrioCount( 8, 3 )
    call SetStartLocPrio( 8, 0, 0, MAP_LOC_PRIO_LOW )
    call SetStartLocPrio( 8, 1, 5, MAP_LOC_PRIO_LOW )
    call SetStartLocPrio( 8, 2, 6, MAP_LOC_PRIO_HIGH )

    call SetStartLocPrioCount( 9, 3 )
    call SetStartLocPrio( 9, 0, 1, MAP_LOC_PRIO_HIGH )
    call SetStartLocPrio( 9, 1, 2, MAP_LOC_PRIO_LOW )
    call SetStartLocPrio( 9, 2, 7, MAP_LOC_PRIO_LOW )
endfunction

//***************************************************************************
//*
//*  Main Initialization
//*
//***************************************************************************

//===========================================================================
function main takes nothing returns nothing
    call SetCameraBounds( -12032.0 + GetCameraMargin(CAMERA_MARGIN_LEFT), -11776.0 + GetCameraMargin(CAMERA_MARGIN_BOTTOM), 11264.0 - GetCameraMargin(CAMERA_MARGIN_RIGHT), 11520.0 - GetCameraMargin(CAMERA_MARGIN_TOP), -12032.0 + GetCameraMargin(CAMERA_MARGIN_LEFT), 11520.0 - GetCameraMargin(CAMERA_MARGIN_TOP), 11264.0 - GetCameraMargin(CAMERA_MARGIN_RIGHT), -11776.0 + GetCameraMargin(CAMERA_MARGIN_BOTTOM) )
    call SetDayNightModels( "Environment\\DNC\\DNCLordaeron\\DNCLordaeronTerrain\\DNCLordaeronTerrain.mdl", "Environment\\DNC\\DNCLordaeron\\DNCLordaeronUnit\\DNCLordaeronUnit.mdl" )
    call NewSoundEnvironment( "Default" )
    call SetAmbientDaySound( "BarrensDay" )
    call SetAmbientNightSound( "BarrensNight" )
    call SetMapMusic( "Music", true, 0 )
    call InitSounds(  )
    call CreateAllUnits(  )
    call InitBlizzard(  )
    call InitGlobals(  )
    call InitCustomTriggers(  )
    call RunInitializationTriggers(  )

endfunction

//***************************************************************************
//*
//*  Map Configuration
//*
//***************************************************************************

function config takes nothing returns nothing
    call SetMapName( "TRIGSTR_000" )
    call SetMapDescription( "TRIGSTR_002" )
    call SetPlayers( 10 )
    call SetTeams( 10 )
    call SetGamePlacement( MAP_PLACEMENT_TEAMS_TOGETHER )

    call DefineStartLocation( 0, 8000.0, -5760.0 )
    call DefineStartLocation( 1, -8896.0, 6080.0 )
    call DefineStartLocation( 2, 3648.0, 6848.0 )
    call DefineStartLocation( 3, -8640.0, -7424.0 )
    call DefineStartLocation( 4, -9088.0, -1664.0 )
    call DefineStartLocation( 5, 8000.0, -1152.0 )
    call DefineStartLocation( 6, -1472.0, -8640.0 )
    call DefineStartLocation( 7, 6336.0, 6656.0 )
    call DefineStartLocation( 8, 1600.0, -7552.0 )
    call DefineStartLocation( 9, -6272.0, 7680.0 )

    // Player setup
    call InitCustomPlayerSlots(  )
    call SetPlayerSlotAvailable( Player(0), MAP_CONTROL_USER )
    call SetPlayerSlotAvailable( Player(1), MAP_CONTROL_USER )
    call SetPlayerSlotAvailable( Player(2), MAP_CONTROL_USER )
    call SetPlayerSlotAvailable( Player(3), MAP_CONTROL_USER )
    call SetPlayerSlotAvailable( Player(4), MAP_CONTROL_USER )
    call SetPlayerSlotAvailable( Player(5), MAP_CONTROL_USER )
    call SetPlayerSlotAvailable( Player(6), MAP_CONTROL_USER )
    call SetPlayerSlotAvailable( Player(7), MAP_CONTROL_USER )
    call SetPlayerSlotAvailable( Player(8), MAP_CONTROL_USER )
    call SetPlayerSlotAvailable( Player(9), MAP_CONTROL_USER )
    call InitGenericPlayerSlots(  )
    call InitAllyPriorities(  )
endfunction

