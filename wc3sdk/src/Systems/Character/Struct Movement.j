/// The rtc.j file is required.
/// Jumping is not implemented yet.
/// Do not use this library, it is unfinished!
library AStructSystemsCharacterMovement requires optional ALibraryCoreDebugMisc, AStructCoreGeneralAsl, AStructCoreGeneralHashTable, ALibraryCoreInterfaceMisc, AStructSystemsCharacterAbstractCharacterSystem

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
		// static constant members
		private static constant integer stateMoveForward = 0
		private static constant integer stateMoveBackward = 1
		private static constant integer stateTurnRight = 2
		private static constant integer stateTurnLeft = 3
		private static constant integer maxStates = 4
		private static constant integer maxMovementTriggers = 8
		// static construction members
static if (A_RTC and A_FPS_MOVEMENT) then
		private static integer fpsKeyMoveForward
		private static integer fpsKeyMoveBackward
		private static integer fpsKeyTurnRight
		private static integer fpsKeyTurnLeft
endif
		private static real refreshRate
		private static real speed
		private static real angle
		private static boolean stopWhileStanding
		// members
		private trigger m_movementTrigger
static if (A_RTC and A_FPS_MOVEMENT) then
		private trigger m_fpsTriggerUp
		private trigger m_fpsTriggerDown
else
		private trigger array m_startMovementTrigger[thistype.maxMovementTriggers]
endif
		private boolean array m_state[thistype.maxStates]

		//! runtextmacro optional A_STRUCT_DEBUG("\"AMovement\"")

		public method enable takes nothing returns nothing
//static if (not A_RTC or not A_FPS_MOVEMENT) then
static if (not A_FPS_MOVEMENT) then
			local integer i
endif
			call super.enable()
			call EnableTrigger(this.m_movementTrigger)
//static if (not A_RTC or not A_FPS_MOVEMENT) then
static if (not A_FPS_MOVEMENT) then
			set i = 0
			loop
				exitwhen(i == thistype.maxMovementTriggers)
				call EnableTrigger(this.m_startMovementTrigger[i])
				set i = i + 1
			endloop
endif
		endmethod

		public method disable takes nothing returns nothing
//static if (not A_RTC or not A_FPS_MOVEMENT) then
static if (not A_FPS_MOVEMENT) then
			local integer i
endif
			call super.disable()
			call DisableTrigger(this.m_movementTrigger)
//static if (not A_RTC or not A_FPS_MOVEMENT) then
static if (not A_FPS_MOVEMENT) then
			set i = 0
			loop
				exitwhen(i == thistype.maxMovementTriggers)
				call DisableTrigger(this.m_startMovementTrigger[i])
				set i = i + 1
			endloop
endif
		endmethod

		private method moveToPoint takes real x, real y returns nothing
			if (not IsTerrainPathable(x, y, PATHING_TYPE_WALKABILITY)) then
				call this.character().setPosition(x, y)
			endif
		endmethod

		private method moveForward takes nothing returns nothing
			local real facing = GetUnitFacing(this.character().unit())
			local real x = GetUnitX(this.character().unit())
			local real y = GetUnitY(this.character().unit())
			set x = x + thistype.speed * Cos(facing * bj_DEGTORAD)
			set y = y + thistype.speed * Sin(facing * bj_DEGTORAD)
			call this.moveToPoint(x, y)
			debug call Print("move forward")
		endmethod

		private method moveBackward takes nothing returns nothing
			local real facing = GetUnitFacing(this.character().unit())
			local real x = GetUnitX(this.character().unit())
			local real y = GetUnitY(this.character().unit())
			set x = x - thistype.speed * Cos(facing * bj_DEGTORAD)
			set y = y - thistype.speed * Sin(facing * bj_DEGTORAD)
			call this.moveToPoint(x, y)
			debug call Print("move backward")
		endmethod

		private method moveRightForward takes nothing returns nothing
			local real facing = GetUnitFacing(this.character().unit())
			local real x = GetUnitX(this.character().unit())
			local real y = GetUnitY(this.character().unit())
			set x = x + thistype.speed * Cos((facing - thistype.angle) * bj_DEGTORAD)
			set y = y + thistype.speed * Sin((facing - thistype.angle) * bj_DEGTORAD)
			call this.moveToPoint(x, y)
		endmethod

		private method moveRightBackward takes nothing returns nothing
			local real facing = GetUnitFacing(this.character().unit())
			local real x = GetUnitX(this.character().unit())
			local real y = GetUnitY(this.character().unit())
			set x = x - thistype.speed * Cos((facing + thistype.angle) * bj_DEGTORAD)
			set y = y - thistype.speed * Sin((facing + thistype.angle) * bj_DEGTORAD)
			call this.moveToPoint(x, y)
		endmethod

		private method moveLeftForward takes nothing returns nothing
			local real facing = GetUnitFacing(this.character().unit())
			local real x = GetUnitX(this.character().unit())
			local real y = GetUnitY(this.character().unit())
			set x = x + thistype.speed * Cos((facing + thistype.angle) * bj_DEGTORAD)
			set y = y + thistype.speed * Sin((facing + thistype.angle) * bj_DEGTORAD)
			call this.moveToPoint(x, y)
		endmethod

		private method moveLeftBackward takes nothing returns nothing
			local real facing = GetUnitFacing(this.character().unit())
			local real x = GetUnitX(this.character().unit())
			local real y = GetUnitY(this.character().unit())
			set x = x - thistype.speed * Cos((facing - thistype.angle) * bj_DEGTORAD)
			set y = y - thistype.speed * Sin((facing - thistype.angle) * bj_DEGTORAD)
			call this.moveToPoint(x, y)
		endmethod

		private method turnRight takes nothing returns nothing
			local real facingAngle = GetUnitFacing(this.character().unit()) - thistype.angle
			call SetUnitFacingTimed(this.character().unit(), facingAngle, thistype.refreshRate + 0.5)
		endmethod

		private method turnLeft takes nothing returns nothing
			local real facingAngle = GetUnitFacing(this.character().unit()) + thistype.angle
			call SetUnitFacingTimed(this.character().unit(), facingAngle, thistype.refreshRate + 0.5)
		endmethod

		private method stop takes nothing returns nothing
			call IssueImmediateOrder(this.character().unit(), "holdposition")
		endmethod

		private static method triggerConditionIsAlive takes nothing returns boolean
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			local boolean result = not IsUnitDeadBJ(this.character().unit())
			set triggeringTrigger = null
			return result
		endmethod

		private static method triggerActionMovement takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			if (this.m_state[thistype.stateMoveForward]) then
				if (this.m_state[thistype.stateTurnRight]) then
					call this.moveRightForward()
				elseif (this.m_state[thistype.stateTurnLeft]) then
					call this.moveLeftForward()
				else
					call this.moveForward()
				endif
			elseif (this.m_state[thistype.stateMoveBackward]) then
				if (this.m_state[thistype.stateTurnRight]) then
					call this.moveRightBackward()
				elseif (this.m_state[thistype.stateTurnLeft]) then
					call this.moveLeftBackward()
				else
					call this.moveBackward()
				endif
			elseif (this.m_state[thistype.stateTurnRight]) then
				call this.turnRight()
			elseif (this.m_state[thistype.stateTurnLeft]) then
				call this.turnLeft()
			elseif (thistype.stopWhileStanding) then
				call this.stop()
			endif //Stop wird in den Start-Aktionen aufgerufen
			set triggeringTrigger = null
		endmethod
static if (not A_FPS_MOVEMENT) then
//static if (not A_RTC or not A_FPS_MOVEMENT) then
		private static method triggerActionMoveForward takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			set this.m_state[thistype.stateMoveBackward] = false
			set this.m_state[thistype.stateMoveForward] = true
			set triggeringTrigger = null
		endmethod

		private static method triggerActionStopMovingForward takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			set this.m_state[thistype.stateMoveForward] = false
			set triggeringTrigger = null
		endmethod

		private static method triggerActionMoveBackward takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			set this.m_state[thistype.stateMoveForward] = false
			set this.m_state[thistype.stateMoveBackward] = true
			set triggeringTrigger = null
		endmethod

		private static method triggerActionStopMovingBackward takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			set this.m_state[thistype.stateMoveBackward] = false
			set triggeringTrigger = null
		endmethod

		private static method triggerActionTurnRight takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			set this.m_state[thistype.stateTurnLeft] = false
			set this.m_state[thistype.stateTurnRight] = true
			set triggeringTrigger = null
		endmethod

		private static method triggerActionStopTurningRight takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			set this.m_state[thistype.stateTurnRight] = false
			set triggeringTrigger = null
		endmethod

		private static method triggerActionTurnLeft takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			set this.m_state[thistype.stateTurnRight] = false
			set this.m_state[thistype.stateTurnLeft] = true
			set triggeringTrigger = null
		endmethod

		private static method triggerActionStopTurningLeft takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			set this.m_state[thistype.stateTurnLeft] = false
			set triggeringTrigger = null
		endmethod
endif
		private method createMovementTriggers takes nothing returns nothing
			local event triggerEvent
			local conditionfunc conditionFunction
			local triggercondition triggerCondition
			local triggeraction triggerAction
			set this.m_movementTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterTimerEvent(this.m_movementTrigger, thistype.refreshRate, true)
			set conditionFunction = Condition(function thistype.triggerConditionIsAlive)
			set triggerCondition = TriggerAddCondition(this.m_movementTrigger, conditionFunction)
			set triggerAction = TriggerAddAction(this.m_movementTrigger, function thistype.triggerActionMovement)
			call AHashTable.global().setHandleInteger(this.m_movementTrigger, "this", this)
			set triggerEvent = null
			set conditionFunction = null
			set triggerCondition = null
			set triggerAction = null
//static if (not A_RTC or not A_FPS_MOVEMENT) then
static if (not A_FPS_MOVEMENT) then
			//forward
			set this.m_startMovementTrigger[0] = CreateTrigger()
			set triggerEvent = TriggerRegisterKeyEventForPlayer(this.character().player(), this.m_startMovementTrigger[0], AKeyUp, true)
			set triggerAction = TriggerAddAction(this.m_startMovementTrigger[0], function thistype.triggerActionMoveForward)
			call AHashTable.global().setHandleInteger(this.m_startMovementTrigger[0], "this", this)
			set triggerEvent = null
			set triggerAction = null

			//stop forward
			set this.m_startMovementTrigger[1] = CreateTrigger()
			set triggerEvent = TriggerRegisterKeyEventForPlayer(this.character().player(), this.m_startMovementTrigger[1], AKeyUp, false)
			set triggerAction = TriggerAddAction(this.m_startMovementTrigger[1], function thistype.triggerActionStopMovingForward)
			call AHashTable.global().setHandleInteger(this.m_startMovementTrigger[1], "this", this)
			set triggerEvent = null
			set triggerAction = null

			//backward
			set this.m_startMovementTrigger[2] = CreateTrigger()
			set triggerEvent = TriggerRegisterKeyEventForPlayer(this.character().player(), this.m_startMovementTrigger[2], AKeyDown, true)
			set triggerAction = TriggerAddAction(this.m_startMovementTrigger[2], function thistype.triggerActionMoveBackward)
			call AHashTable.global().setHandleInteger(this.m_startMovementTrigger[2], "this", this)
			set triggerEvent = null
			set triggerAction = null

			//stop backward
			set this.m_startMovementTrigger[3] = CreateTrigger()
			set triggerEvent = TriggerRegisterKeyEventForPlayer(this.character().player(), this.m_startMovementTrigger[3], AKeyDown, false)
			set triggerAction = TriggerAddAction(this.m_startMovementTrigger[3], function thistype.triggerActionStopMovingBackward)
			call AHashTable.global().setHandleInteger(this.m_startMovementTrigger[3], "this", this)
			set triggerEvent = null
			set triggerAction = null

			//right
			set this.m_startMovementTrigger[4] = CreateTrigger()
			set triggerEvent = TriggerRegisterKeyEventForPlayer(this.character().player(), this.m_startMovementTrigger[4], AKeyRight, true)
			set triggerAction = TriggerAddAction(this.m_startMovementTrigger[4], function thistype.triggerActionTurnRight)
			call AHashTable.global().setHandleInteger(this.m_startMovementTrigger[4], "this", this)
			set triggerEvent = null
			set triggerAction = null

			//stop right
			set this.m_startMovementTrigger[5] = CreateTrigger()
			set triggerEvent = TriggerRegisterKeyEventForPlayer(this.character().player(), this.m_startMovementTrigger[5], AKeyRight, false)
			set triggerAction = TriggerAddAction(this.m_startMovementTrigger[5], function thistype.triggerActionStopTurningRight)
			call AHashTable.global().setHandleInteger(this.m_startMovementTrigger[5], "this", this)
			set triggerEvent = null
			set triggerAction = null

			//left
			set this.m_startMovementTrigger[6] = CreateTrigger()
			set triggerEvent = TriggerRegisterKeyEventForPlayer(this.character().player(), this.m_startMovementTrigger[6], AKeyLeft, true)
			set triggerAction = TriggerAddAction(this.m_startMovementTrigger[6], function thistype.triggerActionTurnLeft)
			call AHashTable.global().setHandleInteger(this.m_startMovementTrigger[6], "this", this)
			set triggerEvent = null
			set triggerAction = null

			//stop left
			set this.m_startMovementTrigger[7] = CreateTrigger()
			set triggerEvent = TriggerRegisterKeyEventForPlayer(this.character().player(), this.m_startMovementTrigger[7], AKeyLeft, false)
			set triggerAction = TriggerAddAction(this.m_startMovementTrigger[7], function thistype.triggerActionStopTurningLeft)
			call AHashTable.global().setHandleInteger(this.m_startMovementTrigger[7], "this", this)
			set triggerEvent = null
			set triggerAction = null
endif
		endmethod
static if (A_RTC and A_FPS_MOVEMENT) then
		private static method triggerActionFpsUp takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			local integer triggerKey = GetTriggerKey() //rtc.j
			if(triggerKey == thistype.fpsKeyMoveForward) then
				set this.m_state[thistype.stateMoveForward] = false
			endif
			if(triggerKey == thistype.fpsKeyMoveBackward) then
				set this.m_state[thistype.stateMoveBackward] = false
			endif
			if(triggerKey == thistype.fpsKeyTurnRight) then
				set this.m_state[thistype.stateTurnRight] = false
			endif
			if(triggerKey == thistype.fpsKeyTurnLeft) then
				set this.m_state[thistype.stateTurnLeft] = false
			endif
			set triggeringTrigger = null
		endmethod

		private method createFpsTriggerUp takes nothing returns nothing
			local triggeraction triggerAction
			set this.m_fpsTriggerUp = CreateTrigger()
			call TriggerRegisterKeyEvent(this.m_fpsTriggerUp, 0) //rtc.j
			set triggerAction = TriggerAddAction(this.m_fpsTriggerUp, function thistype.triggerActionFpsUp)
			call AHashTable.global().setHandleInteger(this.m_fpsTriggerUp, "this", this)
			set triggerAction = null
		endmethod

		private static method triggerActionFpsDown takes nothing returns nothing
			local trigger triggeringTrigger = GetTriggeringTrigger()
			local thistype this = AHashTable.global().handleInteger(triggeringTrigger, "this")
			local integer triggerKey = GetTriggerKey() //rtc.j
			if(triggerKey == thistype.fpsKeyMoveForward) then
				set this.m_state[thistype.stateMoveForward] = true
			endif
			if(triggerKey == thistype.fpsKeyMoveBackward) then
				set this.m_state[thistype.stateMoveBackward] = true
			endif
			if(triggerKey == thistype.fpsKeyTurnRight) then
				set this.m_state[thistype.stateTurnRight] = true
			endif
			if(triggerKey == thistype.fpsKeyTurnLeft) then
				set this.m_state[thistype.stateTurnLeft] = true
			endif
			set triggeringTrigger = null
		endmethod

		private method createFpsTriggerDown takes nothing returns nothing
			local triggeraction triggerAction
			set this.m_fpsTriggerDown = CreateTrigger()
			call TriggerRegisterKeyEvent(this.m_fpsTriggerDown, 1) //rtc.j
			set triggerAction = TriggerAddAction(this.m_fpsTriggerDown, function thistype.triggerActionFpsDown)
			call AHashTable.global().setHandleInteger(this.m_fpsTriggerDown, "this", this)
			set triggerAction = null
		endmethod
endif

		public static method create takes ACharacter character returns thistype
			local thistype this = thistype.allocate(character)

			call this.createMovementTriggers()
static if (A_RTC and A_FPS_MOVEMENT) then
			call this.createFpsTriggerUp()
			call this.createFpsTriggerDown()
endif
			return this
		endmethod

		private method destroyKeyMovementTriggers takes nothing returns nothing
//static if (not A_RTC or not A_FPS_MOVEMENT) then
static if (not A_FPS_MOVEMENT) then
			local integer i
endif
			call AHashTable.global().destroyTrigger(this.m_movementTrigger)
			set this.m_movementTrigger = null
//static if (not A_RTC or not A_FPS_MOVEMENT) then
static if (not A_FPS_MOVEMENT) then
			set i = 0
			loop
				exitwhen(i == thistype.maxMovementTriggers)
				call AHashTable.global().destroyTrigger(this.m_startMovementTrigger[i])
				set this.m_startMovementTrigger[i] = null
				set i = i + 1
			endloop
endif
		endmethod
static if (A_RTC and A_FPS_MOVEMENT) then
		private method destroyFpsTriggerUp takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_fpsTriggerUp)
			set this.m_fpsTriggerUp = null
		endmethod

		private method destroyFpsTriggerDown takes nothing returns nothing
			call AHashTable.global().destroyTrigger(this.m_fpsTriggerDown)
			set this.m_fpsTriggerDown = null
		endmethod
endif
		public method onDestroy takes nothing returns nothing

			call this.destroyKeyMovementTriggers()
static if (A_RTC and A_FPS_MOVEMENT) then
			call this.destroyFpsTriggerUp()
			call this.destroyFpsTriggerDown()
endif
		endmethod
//static if (not A_RTC or not A_FPS_MOVEMENT) then
static if (not A_FPS_MOVEMENT) then
		/**
		* @param refreshRate 0.01
		* @param speed 5.0
		* @param angle 90.0
		* @param stopWhileStanding false Character stops if no key is pressed.
		*/
		public static method init takes real refreshRate, real speed, real angle, boolean stopWhileStanding returns nothing
			// static construction members
			set thistype.refreshRate = refreshRate
			set thistype.speed = speed
			set thistype.angle = angle
			set thistype.stopWhileStanding = stopWhileStanding
		endmethod
else
		/**
		* @param fpsKeyMoveForward 87 - W
		* @param fpsKeyMoveBackward 83 - S
		* @param fpsKeyTurnRight 68 - D
		* @param fpsKeyTurnLeft 65 - A
		* @param refreshRate 0.01
		* @param speed 5.0
		* @param angle 90.0
		* @param stopWhileStanding false Character stops if no key is pressed.
		*/
		public static method init takes integer fpsKeyMoveForward, integer fpsKeyMoveBackward, integer fpsKeyTurnRight, integer fpsKeyTurnLeft, real refreshRate, real speed, real angle, boolean stopWhileStanding returns nothing
			// static construction members
			set thistype.fpsKeyMoveForward = fpsKeyMoveForward
			set thistype.fpsKeyMoveBackward = fpsKeyMoveBackward
			set thistype.fpsKeyTurnRight = fpsKeyTurnRight
			set thistype.fpsKeyTurnLeft = fpsKeyTurnLeft
			set thistype.refreshRate = refreshRate
			set thistype.speed = speed
			set thistype.angle = angle
			set thistype.stopWhileStanding = stopWhileStanding
		endmethod
endif
	endstruct

endlibrary