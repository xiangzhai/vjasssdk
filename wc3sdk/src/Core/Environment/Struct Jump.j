library AStructCoreEnvironmentJump requires ALibraryCoreDebugMisc, AStructCoreGeneralVector, ALibraryCoreMathsReal, ALibraryCoreMathsPoint, ALibraryCoreMathsUnit

	//! runtextmacro A_VECTOR("private", "AJumpVector", "AJump", "0", "100")

	/// @todo Should be a part of @struct AJump, vJass bug.
	function interface AJumpAlightAction takes unit usedUnit returns nothing

	struct AJump
		//static start members
		private static real m_refreshRate
		private static string m_jumpAnimation
		//static members
		private static timer m_timer
		private static AJumpVector m_jumps
		//start members
		private unit m_unit
		private real m_maxHeight
		private real m_targetX
		private real m_targetY
		private AJumpAlightAction m_alightAction
		//members
		private integer m_index
		private real m_startX
		private real m_startY
		private real m_distance
		private real m_x
		
		//! runtextmacro A_STRUCT_DEBUG("\"AJump\"")
		
		private method refreshPosition takes nothing returns boolean
			set this.m_x = this.m_x + this.m_distance / 100.0
			debug call this.print("Refresh. Distance is " + R2S(this.m_distance) + " refresh rate is " + R2S(thistype.m_refreshRate))
			call SetUnitX(this.m_unit, GetPolarProjectionX(this.m_startX, GetUnitFacing(this.m_unit), this.m_x))
			call SetUnitY(this.m_unit, GetPolarProjectionY(this.m_startY, GetUnitFacing(this.m_unit), this.m_x))
			call SetUnitZ(this.m_unit, ParabolaZ(this.m_maxHeight, this.m_distance, this.m_x))
			debug call this.print("X is " + R2S(this.m_x))
			debug call this.print("New x is " + R2S(this.m_x))
			return this.m_x >= this.m_distance
		endmethod
		
		public static method create takes unit usedUnit, real maxHeight, real targetX, real targetY, AJumpAlightAction alightAction returns AJump
			local AJump this = AJump.allocate()
			//start members
			set this.m_unit = usedUnit
			set this.m_maxHeight = maxHeight
			set this.m_targetX = targetX
			set this.m_targetY = targetY
			set this.m_alightAction = alightAction
			//members
			call thistype.m_jumps.pushBack(this)
			set this.m_index = thistype.m_jumps.backIndex()
			set this.m_startX = GetUnitX(usedUnit)
			set this.m_startY = GetUnitY(usedUnit)
			set this.m_distance = GetDistanceBetweenPoints(this.m_startX, this.m_startY, 0.0, targetX, targetY, 0)
			set this.m_x = 0.0
			
			call PauseUnit(usedUnit, true)
			call SetUnitFacing(usedUnit, GetAngleBetweenPoints(this.m_startX, this.m_startY, targetX, targetY))
			
			if (thistype.m_jumpAnimation != null) then
				call SetUnitAnimation(usedUnit, thistype.m_jumpAnimation)
			endif
			
			return this
		endmethod
		
		public method onDestroy takes nothing returns nothing
			call PauseUnit(this.m_unit, false)
			
			if (thistype.m_jumpAnimation != null) then
				call ResetUnitAnimation(this.m_unit)
			endif
			
			//start members
			set this.m_unit = null
			
			call thistype.m_jumps.erase(this.m_index)
		endmethod
		
		private static method timerFunction takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == thistype.m_jumps.size())
				if (thistype.m_jumps[i].refreshPosition()) then
					if (thistype.m_jumps[i].m_alightAction != 0) then
						call thistype.m_jumps[i].m_alightAction.execute(thistype.m_jumps[i].m_unit)
					endif
					call thistype.m_jumps[i].destroy()
					//do not increase i, jump was removed from vector
				elseif (IsUnitDeadBJ(thistype.m_jumps[i].m_unit)) then
					call thistype.m_jumps[i].destroy()
					//do not increase i, jump was removed from vector
					debug call thistype.staticPrint("Is Dead!")
				else
					set i = i + 1
				endif
			endloop
		endmethod
		
		public static method init takes real refreshRate, string jumpAnimation returns nothing
			//static start members
			set thistype.m_refreshRate = refreshRate
			set thistype.m_jumpAnimation = jumpAnimation
			//static members
			set thistype.m_timer = CreateTimer()
			call TimerStart(thistype.m_timer, thistype.m_refreshRate, true, function thistype.timerFunction)
			set thistype.m_jumps = AJumpVector.create()
		endmethod
		
		public static method cleanUp takes nothing returns nothing
			local integer i = thistype.m_jumps.backIndex()
			call DestroyTimer(thistype.m_timer)
			set thistype.m_timer = null
			loop
				exitwhen (i < 0)
				call thistype.m_jumps[i].destroy()
				set i = i - 1
			endloop
		endmethod
		
		public static method enable takes nothing returns nothing
			call ResumeTimer(thistype.m_timer)
		endmethod
		
		public static method disable takes nothing returns nothing
			call PauseTimer(thistype.m_timer)
		endmethod
	endstruct
	
endlibrary