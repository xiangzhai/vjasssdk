/// The fps common.j file is required.
/// Jumping is not implemented yet.
/// Do not use this library, it is unfinished!
library AStructSystemsCharacterMovement requires ALibraryCoreDebugMisc, AStructCoreGeneralAsl, AStructCoreGeneralHashTable, ALibraryCoreInterfaceMisc, AStructSystemsCharacterAbstractCharacterSystem

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
		private trigger array startMovementTrigger[thistype.maxMovementTriggers]
		private trigger fpsTriggerUp
		private trigger fpsTriggerDown
		private boolean array state[thistype.maxStates]
		
		//! runtextmacro A_STRUCT_DEBUG("\"AMovement\"")

		public method enable takes nothing returns nothing
			local integer i
			call super.enable()
			call EnableTrigger(this.movementTrigger)
			set i = 0
			loop
				exitwhen(i == thistype.maxMovementTriggers)
				call EnableTrigger(this.startMovementTrigger[i])
				set i = i + 1
			endloop
		endmethod

		public method disable takes nothing returns nothing
			local integer i
			call super.disable()
			call DisableTrigger(this.movementTrigger)
			set i = 0
			loop
				exitwhen(i == thistype.maxMovementTriggers)
				call DisableTrigger(this.startMovementTrigger[i])
				set i = i + 1
			endloop
		endmethod

		private method moveToPoint takes real x, real y returns nothing
			if (not IsTerrainPathable(x, y, PATHING_TYPE_WALKABILITY)) then
				call this.character().setPosition(x, y)
			endif
		endmethod

		private method moveForward takes nothing returns nothing
			local real facing = GetUnitFacing(this.unit())
			local real x = GetUnitX(this.unit())
			local real y = GetUnitY(this.unit())
			set x = x + thistype.speed * Cos(facing * bj_DEGTORAD)
			set y = y + thistype.speed * Sin(facing * bj_DEGTORAD)
			call this.moveToPoint(x, y)
			debug call Print("move forward")
		endmethod

		private method moveBackward takes nothing returns nothing
			local real facing = GetUnitFacing(this.unit())
			local real x = GetUnitX(this.unit())
			local real y = GetUnitY(this.unit())
			set x = x - thistype.speed * Cos(facing * bj_DEGTORAD)
			set y = y - thistype.speed * Sin(facing * bj_DEGTORAD)
			call this.moveToPoint(x, y)
			debug call Print("move backward")
		endmethod

		private method moveRightForward takes nothing returns nothing
			local real facing = GetUnitFacing(this.unit())
			local real x = GetUnitX(this.unit())
			local real y = GetUnitY(this.unit())
			set x = x + thistype.speed * Cos((facing - thistype.angle) * bj_DEGTORAD)
			set y = y + thistype.speed * Sin((facing - thistype.angle) * bj_DEGTORAD)
			call this.moveToPoint(x, y)
		endmethod

		private method moveRightBackward takes nothing returns nothing
			local real facing = GetUnitFacing(this.unit())
			local real x = GetUnitX(this.unit())
			local real y = GetUnitY(this.unit())
			set x = x - thistype.speed * Cos((facing + thistype.angle) * bj_DEGTORAD)
			set y = y - thistype.speed * Sin((facing + thistype.angle) * bj_DEGTORAD)
			call this.moveToPoint(x, y)
		endmethod

		private method moveLeftForward takes nothing returns nothing
			local real facing = GetUnitFacing(this.unit())
			local real x = GetUnitX(this.unit())
			local real y = GetUnitY(this.unit())
			set x = x + thistype.speed * Cos((facing + thistype.angle) * bj_DEGTORAD)
			set y = y + thistype.speed * Sin((facing + thistype.angle) * bj_DEGTORAD)
			call this.moveToPoint(x, y)
		endmethod

		private method moveLeftBackward takes nothing returns nothing
			local real facing = GetUnitFacing(this.unit())
			local real x = GetUnitX(this.unit())
			local real y = GetUnitY(this.unit())
			set x = x - thistype.speed * Cos((facing - thistype.angle) * bj_DEGTORAD)
			set y = y - thistype.speed * Sin((facing - thistype.angle) * bj_DEGTORAD)
			call this.moveToPoint(x, y)
		endmethod

		private method turnRight takes nothing returns nothing
			local real facingAngle = GetUnitFacing(this.unit()) - thistype.angle
			call SetUnitFacingTimed(this.unit(), facingAngle, thistype.refreshRate + 0.5)
		endmethod
		
		private method turnLeft takes nothing returns nothing
			local real facingAngle = GetUnitFacing(this.unit()) + thistype.angle
			call SetUnitFacingTimed(this.unit(), facingAngle, thistype.refreshRate + 0.5)
		endmethod
		
		private method stop takes nothing returns nothing
			call IssueImmediateOrder(this.unit(), "holdposition")
		endmethod
		
		private static method triggerConditionIsAlive takes nothing returns boolean
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			local boolean result = not IsUnitDeadBJ(this.unit())
			set triggeringTrigger = null
			return result
		endmethod

		private static method triggerActionMovement takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			if (this.state[thistype.stateMoveForward]) then
				if (this.state[thistype.stateTurnRight]) then
					call this.moveRightForward()
				elseif (this.state[thistype.stateTurnLeft]) then
					call this.moveLeftForward()
				else
					call this.moveForward()
				endif
			elseif (this.state[thistype.stateMoveBackward]) then
				if (this.state[thistype.stateTurnRight]) then
					call this.moveRightBackward()
				elseif (this.state[thistype.stateTurnLeft]) then
					call this.moveLeftBackward()
				else
					call this.moveBackward()
				endif
			elseif (this.state[thistype.stateTurnRight]) then
				call this.turnRight()
			elseif (this.state[thistype.stateTurnLeft]) then
				call this.turnLeft()
			elseif (thistype.stopWhileStanding) then
				call this.stop()
			endif //Stop wird in den Start-Aktionen aufgerufen
			set triggeringTrigger = null
		endmethod

		private static method triggerActionMoveForward takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			set this.state[thistype.stateMoveBackward] = false
			set this.state[thistype.stateMoveForward] = true
			set triggeringTrigger = null
		endmethod

		private static method triggerActionStopMovingForward takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			set this.state[thistype.stateMoveForward] = false
			set triggeringTrigger = null
		endmethod

		private static method triggerActionMoveBackward takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			set this.state[thistype.stateMoveForward] = false
			set this.state[thistype.stateMoveBackward] = true
			set triggeringTrigger = null
		endmethod

		private static method triggerActionStopMovingBackward takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			set this.state[thistype.stateMoveBackward] = false
			set triggeringTrigger = null
		endmethod

		private static method triggerActionTurnRight takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			set this.state[thistype.stateTurnLeft] = false
			set this.state[thistype.stateTurnRight] = true
			set triggeringTrigger = null
		endmethod

		private static method triggerActionStopTurningRight takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			set this.state[thistype.stateTurnRight] = false
			set triggeringTrigger = null
		endmethod

		private static method triggerActionTurnLeft takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			set this.state[thistype.stateTurnRight] = false
			set this.state[thistype.stateTurnLeft] = true
			set triggeringTrigger = null
		endmethod

		private static method triggerActionStopTurningLeft takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			set this.state[thistype.stateTurnLeft] = false
			set triggeringTrigger = null
		endmethod

		private method createMovementTriggers takes nothing returns nothing
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction

			set this.movementTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterTimerEvent(this.movementTrigger, thistype.refreshRate, true)
			set conditionFunction = Condition(function thistype.triggerConditionIsAlive)
			set triggerCondition = TriggerAddCondition(this.movementTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.movementTrigger, function thistype.triggerActionMovement)
			call AHashTable.global().setHandleInteger(this.movementTrigger, "this", this)
			set triggerEvent = null
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null

			if (not thistype.useFps) then
				//forward
				set this.startMovementTrigger[0] = CreateTrigger()
				set triggerEvent = TriggerRegisterKeyEventForPlayer(this.user(), this.startMovementTrigger[0], KEY_UP, true)
				set triggerAction = TriggerAddAction(this.startMovementTrigger[0], function thistype.triggerActionMoveForward)
				call AHashTable.global().setHandleInteger(this.startMovementTrigger[0], "this", this)
				set triggerEvent = null
				set triggerAction = null
	
				//stop forward
				set this.startMovementTrigger[1] = CreateTrigger()
				set triggerEvent = TriggerRegisterKeyEventForPlayer(this.user(), this.startMovementTrigger[1], KEY_UP, false)
				set triggerAction = TriggerAddAction(this.startMovementTrigger[1], function thistype.triggerActionStopMovingForward)
				call AHashTable.global().setHandleInteger(this.startMovementTrigger[1], "this", this)
				set triggerEvent = null
				set triggerAction = null
	
				//backward
				set this.startMovementTrigger[2] = CreateTrigger()
				set triggerEvent = TriggerRegisterKeyEventForPlayer(this.user(), this.startMovementTrigger[2], KEY_DOWN, true)
				set triggerAction = TriggerAddAction(this.startMovementTrigger[2], function thistype.triggerActionMoveBackward)
				call AHashTable.global().setHandleInteger(this.startMovementTrigger[2], "this", this)
				set triggerEvent = null
				set triggerAction = null
	
				//stop backward
				set this.startMovementTrigger[3] = CreateTrigger()
				set triggerEvent = TriggerRegisterKeyEventForPlayer(this.user(), this.startMovementTrigger[3], KEY_DOWN, false)
				set triggerAction = TriggerAddAction(this.startMovementTrigger[3], function thistype.triggerActionStopMovingBackward)
				call AHashTable.global().setHandleInteger(this.startMovementTrigger[3], "this", this)
				set triggerEvent = null
				set triggerAction = null
	
				//right
				set this.startMovementTrigger[4] = CreateTrigger()
				set triggerEvent = TriggerRegisterKeyEventForPlayer(this.user(), this.startMovementTrigger[4], KEY_RIGHT, true)
				set triggerAction = TriggerAddAction(this.startMovementTrigger[4], function thistype.triggerActionTurnRight)
				call AHashTable.global().setHandleInteger(this.startMovementTrigger[4], "this", this)
				set triggerEvent = null
				set triggerAction = null
	
				//stop right
				set this.startMovementTrigger[5] = CreateTrigger()
				set triggerEvent = TriggerRegisterKeyEventForPlayer(this.user(), this.startMovementTrigger[5], KEY_RIGHT, false)
				set triggerAction = TriggerAddAction(this.startMovementTrigger[5], function thistype.triggerActionStopTurningRight)
				call AHashTable.global().setHandleInteger(this.startMovementTrigger[5], "this", this)
				set triggerEvent = null
				set triggerAction = null
				
				//left
				set this.startMovementTrigger[6] = CreateTrigger()
				set triggerEvent = TriggerRegisterKeyEventForPlayer(this.user(), this.startMovementTrigger[6], KEY_LEFT, true)
				set triggerAction = TriggerAddAction(this.startMovementTrigger[6], function thistype.triggerActionTurnLeft)
				call AHashTable.global().setHandleInteger(this.startMovementTrigger[6], "this", this)
				set triggerEvent = null
				set triggerAction = null
	
				//stop left
				set this.startMovementTrigger[7] = CreateTrigger()
				set triggerEvent = TriggerRegisterKeyEventForPlayer(this.user(), this.startMovementTrigger[7], KEY_LEFT, false)
				set triggerAction = TriggerAddAction(this.startMovementTrigger[7], function thistype.triggerActionStopTurningLeft)
				call AHashTable.global().setHandleInteger(this.startMovementTrigger[7], "this", this)
				set triggerEvent = null
				set triggerAction = null
			endif
		endmethod

		private static method triggerActionFpsUp takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			local integer triggerKey //= GetTriggerKey() //fpscommon.j
			if(triggerKey == thistype.fpsKeyMoveForward) then
				set this.state[thistype.stateMoveForward] = false
			endif
			if(triggerKey == thistype.fpsKeyMoveBackward) then
				set this.state[thistype.stateMoveBackward] = false
			endif
			if(triggerKey == thistype.fpsKeyTurnRight) then
				set this.state[thistype.stateTurnRight] = false
			endif
			if(triggerKey == thistype.fpsKeyTurnLeft) then
				set this.state[thistype.stateTurnLeft] = false
			endif
			set triggeringTrigger = null
		endmethod

		private method createFpsTriggerUp takes nothing returns nothing
			local triggeraction triggerAction
			if (thistype.useFps) then
				set this.fpsTriggerUp = CreateTrigger()
				//call TriggerRegisterKeyEvent(this.fpsTriggerUp, 0) //fpscommon.j
				set triggerAction = TriggerAddAction(this.fpsTriggerUp, function thistype.triggerActionFpsUp)
				call AHashTable.global().setHandleInteger(this.fpsTriggerUp, "this", this)
				set triggerAction = null
			endif
		endmethod

		private static method triggerActionFpsDown takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			local integer triggerKey //= GetTriggerKey() //fpscommon.j
			if(triggerKey == thistype.fpsKeyMoveForward) then
				set this.state[thistype.stateMoveForward] = true
			endif
			if(triggerKey == thistype.fpsKeyMoveBackward) then
				set this.state[thistype.stateMoveBackward] = true
			endif
			if(triggerKey == thistype.fpsKeyTurnRight) then
				set this.state[thistype.stateTurnRight] = true
			endif
			if(triggerKey == thistype.fpsKeyTurnLeft) then
				set this.state[thistype.stateTurnLeft] = true
			endif
			set triggeringTrigger = null
		endmethod

		private method createFpsTriggerDown takes nothing returns nothing
			local triggeraction triggerAction
			if (thistype.useFps) then
				set this.fpsTriggerDown = CreateTrigger()
				//call TriggerRegisterKeyEvent(this.fpsTriggerDown, 1) //fpscommon.j
				set triggerAction = TriggerAddAction(this.fpsTriggerDown, function thistype.triggerActionFpsDown)
				call AHashTable.global().setHandleInteger(this.fpsTriggerDown, "this", this)
				set triggerAction = null
			endif
		endmethod

		public static method create takes ACharacter character returns thistype
			local thistype this = thistype.allocate(character)

			call this.createMovementTriggers()
			call this.createFpsTriggerUp()
			call this.createFpsTriggerDown()
			return this
		endmethod

		private method destroyKeyMovementTriggers takes nothing returns nothing
			local integer i
			call AHashTable.global().destroyTrigger(this.movementTrigger)
			set this.movementTrigger = null
			if (not thistype.useFps) then
				set i = 0
				loop
					exitwhen(i == thistype.maxMovementTriggers)
					call AHashTable.global().destroyTrigger(this.startMovementTrigger[i])
					set this.startMovementTrigger[i] = null
					set i = i + 1
				endloop
			endif
		endmethod

		private method destroyFpsTriggerUp takes nothing returns nothing
			if (thistype.useFps) then
				call AHashTable.global().destroyTrigger(this.fpsTriggerUp)
				set this.fpsTriggerUp = null
			endif
		endmethod

		private method destroyFpsTriggerDown takes nothing returns nothing
			if (thistype.useFps) then
				call AHashTable.global().destroyTrigger(this.fpsTriggerDown)
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
			debug if (useFps and not Asl.usingRtc()) then
				debug call thistype.staticPrint("FPS is enabled but RtC isn't.")
			debug endif
			//static start members
			set thistype.useFps = useFps
			set thistype.fpsKeyMoveForward = fpsKeyMoveForward
			set thistype.fpsKeyMoveBackward = fpsKeyMoveBackward
			set thistype.fpsKeyTurnRight = fpsKeyTurnRight
			set thistype.fpsKeyTurnLeft = fpsKeyTurnLeft
			set thistype.refreshRate = refreshRate
			set thistype.speed = speed
			set thistype.angle = angle
			set thistype.stopWhileStanding = stopWhileStanding
		endmethod
	endstruct

endlibrary