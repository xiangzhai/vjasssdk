/// The fps common.j file is required.
/// Jumping is not implemented yet.
/// Do not use this library, it is unfinished!
library AStructSystemsCharacterMovement requires ALibraryCoreInterfaceMisc, AStructSystemsCharacterCharacterHashTable, AStructSystemsCharacterAbstractCharacterSystem

	//Die Blocker-Suche noch weiter ausarbeiten und auch die Entfernung für die Kamera damit berechnen
	//Herausfinden wir man Parameter in die Funktion der Blocker übergeben kann.
	//private function FilterFunctionIsDestructableBlocker takes nothing returns boolean
		//local destructable FilterDestructable = GetFilterDestructable()
		//local boolean Result = false
		//if (GetDestructableTypeId(FilterDestructable) == 'YTpb') then //Verlauf-Blocker Boden
		//set Result = true
		//elseif (GetDestructableTypeId(FilterDestructable) == 'YTpc') then  //Verlauf-Blocker Boden (groß)
		//set Result = true
		//elseif (GetDestructableTypeId(FilterDestructable) == 'YTfb') then  //Verlauf-Blocker Beides (groß)
		//set Result = true
		//elseif (GetDestructableTypeId(FilterDestructable) == 'YTfc') then  //Verlauf-Blocker Beides
		//set Result = true
		//endif
		//set FilterDestructable = null
		//return Result
	//endfunction

	//function IsBlockerInRange takes real X, real Y, real Angle, real AngleWidth, real Distance returns boolean
		//local real RealAngle = (AngleWidth / 2.00)
		//local real X1 = GetPolarProjectionX(X, (Angle + RealAngle), 0.00)
		//local real Y1 = GetPolarProjectionY(Y, (Angle + RealAngle), 0.00)
		//local real X2 = GetPolarProjectionX(X, (Angle - RealAngle), Distance)
		//local real Y2 = GetPolarProjectionY(Y, (Angle - RealAngle), Distance)
		//local rect BlockerRect = Rect(X1, Y1, X2, Y2)
		//call EnumDestructablesInRect(BlockerRect, Filter(function FilterFunctionIsDestructableBlocker), null)
		//call RemoveRect(BlockerRect)
		//set BlockerRect = null
		//return false
	//endfunction

	struct AMovement extends AAbstractCharacterSystem
		//static constant members
		private static constant integer stateMoveForward = 0
		private static constant integer stateMoveBackward = 1
		private static constant integer stateTurnRight = 2
		private static constant integer stateTurnLeft = 3
		private static constant integer maxStates = 4
		private static constant integer maxMovementTriggers = 8
		//static start members
		private static boolean useFps
		private static integer fpsKeyMoveForward
		private static integer fpsKeyMoveBackward
		private static integer fpsKeyTurnRight
		private static integer fpsKeyTurnLeft
		private static real refreshRate
		private static real speed
		private static real angle
		private static boolean stopWhileStanding
		//members
		private trigger movementTrigger
		private trigger array startMovementTrigger[AMovement.maxMovementTriggers]
		private trigger fpsTriggerUp
		private trigger fpsTriggerDown
		private boolean array state[AMovement.maxStates]

		public method enable takes nothing returns nothing
			local integer i
			call EnableTrigger(this.movementTrigger)
			set i = 0
			loop
				exitwhen(i == AMovement.maxMovementTriggers)
				call EnableTrigger(this.startMovementTrigger[i])
				set i = i + 1
			endloop
		endmethod

		public method disable takes nothing returns nothing
			local integer i
			call DisableTrigger(this.movementTrigger)
			set i = 0
			loop
				exitwhen(i == AMovement.maxMovementTriggers)
				call DisableTrigger(this.startMovementTrigger[i])
				set i = i + 1
			endloop
		endmethod

		private method moveToPoint takes real x, real y returns nothing
			if (not IsTerrainPathable(x, y, PATHING_TYPE_WALKABILITY)) then
				call SetUnitPosition(this.getUnit(), x, y)
			endif
		endmethod

		private method moveForward takes nothing returns nothing
			local real facing = GetUnitFacing(this.getUnit())
			local real x = GetUnitX(this.getUnit())
			local real y = GetUnitY(this.getUnit())
			set x = x + AMovement.speed * Cos(facing * bj_DEGTORAD)
            		set y = y + AMovement.speed * Sin(facing * bj_DEGTORAD)
			call this.moveToPoint(x, y)
			debug call Print("move forward")
		endmethod

		private method moveBackward takes nothing returns nothing
			local real facing = GetUnitFacing(this.getUnit())
			local real x = GetUnitX(this.getUnit())
			local real y = GetUnitY(this.getUnit())
			set x = x - AMovement.speed * Cos(facing * bj_DEGTORAD)
			set y = y - AMovement.speed * Sin(facing * bj_DEGTORAD)
			call this.moveToPoint(x, y)
			debug call Print("move backward")
		endmethod

		private method moveRightForward takes nothing returns nothing
			local real facing = GetUnitFacing(this.getUnit())
			local real x = GetUnitX(this.getUnit())
			local real y = GetUnitY(this.getUnit())
			set x = x + AMovement.speed * Cos((facing - AMovement.angle) * bj_DEGTORAD)
            		set y = y + AMovement.speed * Sin((facing - AMovement.angle) * bj_DEGTORAD)
			call this.moveToPoint(x, y)
		endmethod

		private method moveRightBackward takes nothing returns nothing
			local real facing = GetUnitFacing(this.getUnit())
			local real x = GetUnitX(this.getUnit())
			local real y = GetUnitY(this.getUnit())
			set x = x - AMovement.speed * Cos((facing + AMovement.angle) * bj_DEGTORAD)
            		set y = y - AMovement.speed * Sin((facing + AMovement.angle) * bj_DEGTORAD)
			call this.moveToPoint(x, y)
		endmethod

		private method moveLeftForward takes nothing returns nothing
			local real facing = GetUnitFacing(this.getUnit())
			local real x = GetUnitX(this.getUnit())
			local real y = GetUnitY(this.getUnit())
			set x = x + AMovement.speed * Cos((facing + AMovement.angle) * bj_DEGTORAD)
            		set y = y + AMovement.speed * Sin((facing + AMovement.angle) * bj_DEGTORAD)
			call this.moveToPoint(x, y)
		endmethod

		private method moveLeftBackward takes nothing returns nothing
			local real facing = GetUnitFacing(this.getUnit())
			local real x = GetUnitX(this.getUnit())
			local real y = GetUnitY(this.getUnit())
			set x = x - AMovement.speed * Cos((facing - AMovement.angle) * bj_DEGTORAD)
            		set y = y - AMovement.speed * Sin((facing - AMovement.angle) * bj_DEGTORAD)
			call this.moveToPoint(x, y)
		endmethod

		private method turnRight takes nothing returns nothing
			local real facingAngle = GetUnitFacing(this.getUnit()) - AMovement.angle
			call SetUnitFacingTimed(this.getUnit(), facingAngle, AMovement.refreshRate + 0.5)
		endmethod
		
		private method turnLeft takes nothing returns nothing
			local real facingAngle = GetUnitFacing(this.getUnit()) + AMovement.angle
			call SetUnitFacingTimed(this.getUnit(), facingAngle, AMovement.refreshRate + 0.5)
		endmethod
		
		private method stop takes nothing returns nothing
			call IssueImmediateOrder(this.getUnit(), "holdposition")
		endmethod

		private static method triggerActionMovement takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AMovement movement = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "movement") //AClassCharacterCharacterHashTable
			if (movement.state[AMovement.stateMoveForward]) then
				if (movement.state[AMovement.stateTurnRight]) then
					call movement.moveRightForward()
				elseif (movement.state[AMovement.stateTurnLeft]) then
					call movement.moveLeftForward()
				else
					call movement.moveForward()
				endif
			elseif (movement.state[AMovement.stateMoveBackward]) then
				if (movement.state[AMovement.stateTurnRight]) then
					call movement.moveRightBackward()
				elseif (movement.state[AMovement.stateTurnLeft]) then
					call movement.moveLeftBackward()
				else
					call movement.moveBackward()
				endif
			elseif (movement.state[AMovement.stateTurnRight]) then
				call movement.turnRight()
			elseif (movement.state[AMovement.stateTurnLeft]) then
				call movement.turnLeft()
			elseif (AMovement.stopWhileStanding) then
				call movement.stop()
			endif //Stop wird in den Start-Aktionen aufgerufen
			set triggeringTrigger = null
		endmethod

		private static method triggerActionMoveForward takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AMovement movement = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "movement") //AClassCharacterCharacterHashTable
			set movement.state[AMovement.stateMoveBackward] = false
			set movement.state[AMovement.stateMoveForward] = true
			set triggeringTrigger = null
		endmethod

		private static method triggerActionStopMovingForward takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AMovement movement = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "movement") //AClassCharacterCharacterHashTable
			set movement.state[AMovement.stateMoveForward] = false
			set triggeringTrigger = null
		endmethod

		private static method triggerActionMoveBackward takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AMovement movement = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "movement") //AClassCharacterCharacterHashTable
			set movement.state[AMovement.stateMoveForward] = false
			set movement.state[AMovement.stateMoveBackward] = true
			set triggeringTrigger = null
		endmethod

		private static method triggerActionStopMovingBackward takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AMovement movement = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "movement") //AClassCharacterCharacterHashTable
			set movement.state[AMovement.stateMoveBackward] = false
			set triggeringTrigger = null
		endmethod

		private static method triggerActionTurnRight takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AMovement movement = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "movement") //AClassCharacterCharacterHashTable
			set movement.state[AMovement.stateTurnLeft] = false
			set movement.state[AMovement.stateTurnRight] = true
			set triggeringTrigger = null
		endmethod

		private static method triggerActionStopTurningRight takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AMovement movement = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "movement") //AClassCharacterCharacterHashTable
			set movement.state[AMovement.stateTurnRight] = false
			set triggeringTrigger = null
		endmethod

		private static method triggerActionTurnLeft takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AMovement movement = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "movement") //AClassCharacterCharacterHashTable
			set movement.state[AMovement.stateTurnRight] = false
			set movement.state[AMovement.stateTurnLeft] = true
			set triggeringTrigger = null
		endmethod

		private static method triggerActionStopTurningLeft takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AMovement movement = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "movement") //AClassCharacterCharacterHashTable
			set movement.state[AMovement.stateTurnLeft] = false
			set triggeringTrigger = null
		endmethod

		private method createMovementTriggers takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction

			set this.movementTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterTimerEvent(this.movementTrigger, AMovement.refreshRate, true)
			set triggerAction = TriggerAddAction(this.movementTrigger, function AMovement.triggerActionMovement)
			call AGetCharacterHashTable().storeHandleInteger(this.movementTrigger, "movement", this) //AClassCharacterCharacterHashTable
			set triggerEvent = null
			set triggerAction = null

			if (not AMovement.useFps) then
				//forward
				set this.startMovementTrigger[0] = CreateTrigger()
				set triggerEvent = TriggerRegisterKeyEventForPlayer(this.getUser(), this.startMovementTrigger[0], KEY_UP, true) //ALibraryInterfaceMisc
				set triggerAction = TriggerAddAction(this.startMovementTrigger[0], function AMovement.triggerActionMoveForward)
				call AGetCharacterHashTable().storeHandleInteger(this.startMovementTrigger[0], "movement", this) //AClassCharacterCharacterHashTable
				set triggerEvent = null
				set triggerAction = null
	
				//stop forward
				set this.startMovementTrigger[1] = CreateTrigger()
				set triggerEvent = TriggerRegisterKeyEventForPlayer(this.getUser(), this.startMovementTrigger[1], KEY_UP, false) //ALibraryInterfaceMisc
				set triggerAction = TriggerAddAction(this.startMovementTrigger[1], function AMovement.triggerActionStopMovingForward)
				call AGetCharacterHashTable().storeHandleInteger(this.startMovementTrigger[1], "movement", this) //AClassCharacterCharacterHashTable
				set triggerEvent = null
				set triggerAction = null
	
				//backward
				set this.startMovementTrigger[2] = CreateTrigger()
				set triggerEvent = TriggerRegisterKeyEventForPlayer(this.getUser(), this.startMovementTrigger[2], KEY_DOWN, true) //ALibraryInterfaceMisc
				set triggerAction = TriggerAddAction(this.startMovementTrigger[2], function AMovement.triggerActionMoveBackward)
				call AGetCharacterHashTable().storeHandleInteger(this.startMovementTrigger[2], "movement", this) //AClassCharacterCharacterHashTable
				set triggerEvent = null
				set triggerAction = null
	
				//stop backward
				set this.startMovementTrigger[3] = CreateTrigger()
				set triggerEvent = TriggerRegisterKeyEventForPlayer(this.getUser(), this.startMovementTrigger[3], KEY_DOWN, false) //ALibraryInterfaceMisc
				set triggerAction = TriggerAddAction(this.startMovementTrigger[3], function AMovement.triggerActionStopMovingBackward)
				call AGetCharacterHashTable().storeHandleInteger(this.startMovementTrigger[3], "movement", this) //AClassCharacterCharacterHashTable
				set triggerEvent = null
				set triggerAction = null
	
				//right
				set this.startMovementTrigger[4] = CreateTrigger()
				set triggerEvent = TriggerRegisterKeyEventForPlayer(this.getUser(), this.startMovementTrigger[4], KEY_RIGHT, true) //ALibraryInterfaceMisc
				set triggerAction = TriggerAddAction(this.startMovementTrigger[4], function AMovement.triggerActionTurnRight)
				call AGetCharacterHashTable().storeHandleInteger(this.startMovementTrigger[4], "movement", this) //AClassCharacterCharacterHashTable
				set triggerEvent = null
				set triggerAction = null
	
				//stop right
				set this.startMovementTrigger[5] = CreateTrigger()
				set triggerEvent = TriggerRegisterKeyEventForPlayer(this.getUser(), this.startMovementTrigger[5], KEY_RIGHT, false) //ALibraryInterfaceMisc
				set triggerAction = TriggerAddAction(this.startMovementTrigger[5], function AMovement.triggerActionStopTurningRight)
				call AGetCharacterHashTable().storeHandleInteger(this.startMovementTrigger[5], "movement", this) //AClassCharacterCharacterHashTable
				set triggerEvent = null
				set triggerAction = null
				
				//left
				set this.startMovementTrigger[6] = CreateTrigger()
				set triggerEvent = TriggerRegisterKeyEventForPlayer(this.getUser(), this.startMovementTrigger[6], KEY_LEFT, true) //ALibraryInterfaceMisc
				set triggerAction = TriggerAddAction(this.startMovementTrigger[6], function AMovement.triggerActionTurnLeft)
				call AGetCharacterHashTable().storeHandleInteger(this.startMovementTrigger[6], "movement", this) //AClassCharacterCharacterHashTable
				set triggerEvent = null
				set triggerAction = null
	
				//stop left
				set this.startMovementTrigger[7] = CreateTrigger()
				set triggerEvent = TriggerRegisterKeyEventForPlayer(this.getUser(), this.startMovementTrigger[7], KEY_LEFT, false) //ALibraryInterfaceMisc
				set triggerAction = TriggerAddAction(this.startMovementTrigger[7], function AMovement.triggerActionStopTurningLeft)
				call AGetCharacterHashTable().storeHandleInteger(this.startMovementTrigger[7], "movement", this) //AClassCharacterCharacterHashTable
				set triggerEvent = null
				set triggerAction = null
			endif
		endmethod

		private static method triggerActionFpsUp takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AMovement movement = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "movement") //AClassCharacterCharacterHashTable
			local integer triggerKey //= GetTriggerKey() //fpscommon.j
			if(triggerKey == AMovement.fpsKeyMoveForward) then
				set movement.state[AMovement.stateMoveForward] = false
			endif
			if(triggerKey == AMovement.fpsKeyMoveBackward) then
				set movement.state[AMovement.stateMoveBackward] = false
			endif
			if(triggerKey == AMovement.fpsKeyTurnRight) then
				set movement.state[AMovement.stateTurnRight] = false
			endif
			if(triggerKey == AMovement.fpsKeyTurnLeft) then
				set movement.state[AMovement.stateTurnLeft] = false
			endif
			set triggeringTrigger = null
		endmethod

		private method createFpsTriggerUp takes nothing returns nothing
			local triggeraction triggerAction
			if (AMovement.useFps) then
				set this.fpsTriggerUp = CreateTrigger()
				//call TriggerRegisterKeyEvent(this.fpsTriggerUp, 0) //fpscommon.j
				set triggerAction = TriggerAddAction(this.fpsTriggerUp, function AMovement.triggerActionFpsUp)
				call AGetCharacterHashTable().storeHandleInteger(this.fpsTriggerUp, "movement", this) //AClassCharacterCharacterHashTable
				set triggerAction = null
			endif
		endmethod

		private static method triggerActionFpsDown takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local AMovement movement = AGetCharacterHashTable().getHandleInteger(triggeringTrigger, "movement") //AClassCharacterCharacterHashTable
			local integer triggerKey //= GetTriggerKey() //fpscommon.j
			if(triggerKey == AMovement.fpsKeyMoveForward) then
				set movement.state[AMovement.stateMoveForward] = true
			endif
			if(triggerKey == AMovement.fpsKeyMoveBackward) then
				set movement.state[AMovement.stateMoveBackward] = true
			endif
			if(triggerKey == AMovement.fpsKeyTurnRight) then
				set movement.state[AMovement.stateTurnRight] = true
			endif
			if(triggerKey == AMovement.fpsKeyTurnLeft) then
				set movement.state[AMovement.stateTurnLeft] = true
			endif
			set triggeringTrigger = null
		endmethod

		private method createFpsTriggerDown takes nothing returns nothing
			local triggeraction triggerAction
			if (AMovement.useFps) then
				set this.fpsTriggerDown = CreateTrigger()
				//call TriggerRegisterKeyEvent(this.fpsTriggerDown, 1) //fpscommon.j
				set triggerAction = TriggerAddAction(this.fpsTriggerDown, function AMovement.triggerActionFpsDown)
				call AGetCharacterHashTable().storeHandleInteger(this.fpsTriggerDown, "movement", this) //AClassCharacterCharacterHashTable
				set triggerAction = null
			endif
		endmethod

		public static method create takes ACharacter character returns AMovement
			local AMovement this = AMovement.allocate(character)

			call this.createMovementTriggers()
			call this.createFpsTriggerUp()
			call this.createFpsTriggerDown()
			return this
		endmethod

		private method destroyKeyMovementTriggers takes nothing returns nothing
			local integer i
			call AGetCharacterHashTable().destroyTrigger(this.movementTrigger) //AClassCharacterCharacterHashTable
			set this.movementTrigger = null
			if (not AMovement.useFps) then
				set i = 0
				loop
					exitwhen(i == AMovement.maxMovementTriggers)
					call AGetCharacterHashTable().destroyTrigger(this.startMovementTrigger[i]) //AClassCharacterCharacterHashTable
					set this.startMovementTrigger[i] = null
					set i = i + 1
				endloop
			endif
		endmethod

		private method destroyFpsTriggerUp takes nothing returns nothing
			if (AMovement.useFps) then
				call AGetCharacterHashTable().destroyTrigger(this.fpsTriggerUp) //AClassCharacterCharacterHashTable
				set this.fpsTriggerUp = null
			endif
		endmethod

		private method destroyFpsTriggerDown takes nothing returns nothing
			if (AMovement.useFps) then
				call AGetCharacterHashTable().destroyTrigger(this.fpsTriggerDown) //AClassCharacterCharacterHashTable
				set this.fpsTriggerDown = null
			endif
		endmethod

		public method onDestroy takes nothing returns nothing

			call this.destroyKeyMovementTriggers()
			call this.destroyFpsTriggerUp()
			call this.destroyFpsTriggerDown()
		endmethod

		/// @param useFps false //new implementation
		/// @param fpsKeyMoveForward 87 //W
		/// @param fpsKeyMoveBackward 83 //S
		/// @param fpsKeyTurnRight 68 //D
		/// @param fpsKeyTurnLeft 65 //A
		/// @param refreshRate 0.01
		/// @param speed 5.0
		/// @param angle 90.0
		/// @param stopWhileStanding false //If no key is pressed the
		public static method init takes boolean useFps, integer fpsKeyMoveForward, integer fpsKeyMoveBackward, integer fpsKeyTurnRight, integer fpsKeyTurnLeft, real refreshRate, real speed, real angle, boolean stopWhileStanding returns nothing
			//static start members
			set AMovement.useFps = useFps
			set AMovement.fpsKeyMoveForward = fpsKeyMoveForward
			set AMovement.fpsKeyMoveBackward = fpsKeyMoveBackward
			set AMovement.fpsKeyTurnRight = fpsKeyTurnRight
			set AMovement.fpsKeyTurnLeft = fpsKeyTurnLeft
			set AMovement.refreshRate = refreshRate
			set AMovement.speed = speed
			set AMovement.angle = angle
			set AMovement.stopWhileStanding = stopWhileStanding
		endmethod
	endstruct

endlibrary