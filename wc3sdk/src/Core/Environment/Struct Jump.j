library AStructCoreEnvironmentJump requires optional ALibraryCoreDebugMisc, AStructCoreGeneralVector, ALibraryCoreMathsReal, ALibraryCoreMathsPoint, ALibraryCoreMathsUnit

	/// @todo Should be a part of @struct AJump, vJass bug.
	function interface AJumpAlightAction takes unit usedUnit returns nothing

	/**
	* @todo Test and fix @struct AMissile first and use vectors for this struct, too afterwards.
	*/
	struct AJump
		//static start members
		private static real m_refreshRate
		private static string m_jumpAnimation
		//static members
		private static timer m_timer
		private static AIntegerVector m_jumps
		//dynamic members
		private real m_speed
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

		//! runtextmacro optional A_STRUCT_DEBUG("\"AJump\"")

		//dynamic members

		public method setSpeed takes real speed returns nothing
			set this.m_speed = speed * thistype.m_refreshRate
		endmethod

		public method speed takes nothing returns real
			return this.m_speed
		endmethod

		//start members

		private method refreshPosition takes nothing returns boolean
			set this.m_x = this.m_x + this.m_speed
			//debug call this.print("Refresh. Distance is " + R2S(this.m_distance) + " refresh rate is " + R2S(thistype.m_refreshRate))
			call SetUnitX(this.m_unit, GetPolarProjectionX(this.m_startX, GetUnitFacing(this.m_unit), this.m_x))
			call SetUnitY(this.m_unit, GetPolarProjectionY(this.m_startY, GetUnitFacing(this.m_unit), this.m_x))
			call SetUnitZ(this.m_unit, ParabolaZ(this.m_maxHeight, this.m_distance, this.m_x))
			//debug call this.print("X is " + R2S(this.m_x))
			//debug call this.print("New x is " + R2S(this.m_x))
			return this.m_x >= this.m_distance
		endmethod

		public static method create takes unit usedUnit, real maxHeight, real targetX, real targetY, AJumpAlightAction alightAction returns thistype
			local thistype this = thistype.allocate()
			//dynamic members
			call this.setSpeed(100.0)
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
			call thistype.m_jumps.erase(this.m_index)
			if (not IsUnitDeadBJ(this.m_unit) and this.m_unit != null) then //could be removed by user function
				call PauseUnit(this.m_unit, false)

				if (thistype.m_jumpAnimation != null) then
					call ResetUnitAnimation(this.m_unit)
				endif
			endif
			//start members
			set this.m_unit = null
		endmethod

		/// @todo fast creation can cause crashes. behaviour is not change if vector members are erased in this method and not in destructor.
		private static method timerFunction takes nothing returns nothing
			local thistype jump
			local integer i = thistype.m_jumps.backIndex()
			loop
				exitwhen (i < 0)
				set jump = thistype.m_jumps[i]
				if (jump.refreshPosition()) then
					if (jump.m_alightAction != 0) then
						call jump.m_alightAction.execute(jump.m_unit)
					endif
					call jump.destroy()
					//do not increase i, jump was removed from vector
				elseif (IsUnitDeadBJ(jump.m_unit)) then
					call jump.destroy()
					//do not increase i, jump was removed from vector
					//debug call thistype.staticPrint("Is Dead!")
				endif
				set i = i - 1
			endloop
		endmethod

		public static method init takes real refreshRate, string jumpAnimation returns nothing
			//static start members
			set thistype.m_refreshRate = refreshRate
			set thistype.m_jumpAnimation = jumpAnimation
			//static members
			set thistype.m_timer = CreateTimer()
			call TimerStart(thistype.m_timer, thistype.m_refreshRate, true, function thistype.timerFunction)
			set thistype.m_jumps = AIntegerVector.create()
		endmethod

		public static method cleanUp takes nothing returns nothing
			call PauseTimer(thistype.m_timer)
			call DestroyTimer(thistype.m_timer)
			set thistype.m_timer = null
			loop
				exitwhen (thistype.m_jumps.empty())
				call thistype(thistype.m_jumps.back()).destroy()
			endloop
			call thistype.m_jumps.destroy()
		endmethod

		public static method enable takes nothing returns nothing
			call ResumeTimer(thistype.m_timer)
		endmethod

		public static method disable takes nothing returns nothing
			call PauseTimer(thistype.m_timer)
		endmethod
	endstruct

endlibrary