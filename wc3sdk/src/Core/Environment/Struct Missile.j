library AStructCoreEnvironmentMissile requires ALibraryCoreDebugMisc, AStructCoreGeneralVector, ALibraryCoreMathsHandle, ALibraryCoreMathsPoint, ALibraryCoreInterfaceSelection

	/// not private! Public static methods return AMissileVectors.
	//! runtextmacro A_VECTOR("", "AMissileVector", "AMissile", "0", "100") /// @todo FIXME - size

	/// OnCollisionFunction functions can be set by method @method setOnCollisionFunction and will be called when missile collides.
	function interface AMissileOnCollisionFunction takes AMissile missile returns nothing

	/// OnDeathFunction functions can by set by method @method setOnDeathFunction and will be called when missile hits target.
	function interface AMissileOnDeathFunction takes AMissile missile returns nothing

	/// Provides the functionality of a single physical missile which is able to cause damage and to have a widget source and target or three coordinate values (x, y and z).
	/// @todo Incompleted!
	/// @todo Add static methods for missile containers.
	/// @todo Collision between missiles?!
	/// @author Draculark
	/// @author Tamino Dauth
	/// @source http://warcraft.ingame.de/forum/showthread.php?s=6f44abe813a621c950b94373b91ed929&threadid=186184
	struct AMissile
		//static start members
		private static real refreshTime
		private static boolean enableCollisions
		//static members
		private static trigger refreshTrigger
		private static AMissileVector missiles
		//dynamic members
		private player m_owner
		private integer m_unitType
		private real m_speed
		private real m_damage
		private real m_damageRange
		private unit m_damageSource
		private attacktype m_attackType
		private damagetype m_damageType
		private weapontype m_weaponType
		private real m_targetX
		private real m_targetY
		private widget m_targetWidget
		private boolean m_collides
		private string m_deathEffectPath
		private string m_startSoundPath
		private string m_deathSoundPath
		private AMissileOnCollisionFunction m_onCollisionFunction
		private AMissileOnDeathFunction m_onDeathFunction
		//members
		private unit usedUnit
		private real cos
		private real sin
		private boolean paused
		
		//! runtextmacro A_STRUCT_DEBUG("\"AMissile\"")
		
		//dynamic member methods
		
		public method setOwner takes player owner returns nothing
			set this.m_owner = owner
		endmethod
		
		public method owner takes nothing returns player
			return this.m_owner
		endmethod
		
		public method setUnitType takes integer unitType returns nothing
			set this.m_unitType = unitType
		endmethod
		
		public method unitType takes nothing returns integer
			return this.m_unitType
		endmethod
		
		public method setSpeed takes real speed returns nothing
			set this.m_speed = speed * AMissile.refreshTime
		endmethod
		
		public method speed takes nothing returns real
			return this.m_speed
		endmethod
		
		public method setDamage takes real damage returns nothing
			set this.m_damage = damage
		endmethod
		
		public method damage takes nothing returns real
			return this.m_damage
		endmethod
		
		public method setDamageRange takes real damageRange returns nothing
			set this.m_damageRange = damageRange
		endmethod
		
		public method damageRange takes nothing returns real
			return this.m_damageRange
		endmethod
		
		public method setDamageSource takes unit damageSource returns nothing
			set this.m_damageSource = damageSource
		endmethod
		
		public method damageSource takes nothing returns unit
			return this.m_damageSource
		endmethod
		
		public method setAttackType takes attacktype attackType returns nothing
			set this.m_attackType = attackType
		endmethod
		
		public method attackType takes nothing returns attacktype
			return this.m_attackType
		endmethod
		
		public method setDamageType takes damagetype damageType returns nothing
			set this.m_damageType = damageType
		endmethod
		
		public method damageType takes nothing returns damagetype
			return this.m_damageType
		endmethod
		
		public method setWeaponType takes weapontype weaponType returns nothing
			set this.m_weaponType = weaponType
		endmethod
		
		public method weaponType takes nothing returns weapontype
			return this.m_weaponType
		endmethod
		
		public method setTargetX takes real targetX returns nothing
			set this.m_targetX = targetX
		endmethod
		
		public method targetX takes nothing returns real
			return this.m_targetX
		endmethod
		
		public method setTargetY takes real targetY returns nothing
			set this.m_targetY = targetY
		endmethod
		
		public method targetY takes nothing returns real
			return this.m_targetY
		endmethod
		
		public method setTargetWidget takes widget targetWidget returns nothing
			set this.m_targetWidget = targetWidget
		endmethod
		
		public method targetWidget takes nothing returns widget
			return this.m_targetWidget
		endmethod
		
		public method setCollides takes boolean collides returns nothing
			set this.m_collides = collides
		endmethod
		
		public method collides takes nothing returns boolean
			return this.m_collides
		endmethod
		
		public method setDeathEffectPath takes string deathEffectPath returns nothing
			set this.m_deathEffectPath = deathEffectPath
		endmethod
		
		public method deathEffectPath takes nothing returns string
			return this.m_deathEffectPath
		endmethod
		
		public method setStartSoundPath takes string startSoundPath returns nothing
			set this.m_startSoundPath = startSoundPath
		endmethod
		
		public method startSoundPath takes nothing returns string
			return this.m_startSoundPath
		endmethod
		
		public method setDeathSoundPath takes string deathSoundPath returns nothing
			set this.m_deathSoundPath = deathSoundPath
		endmethod
		
		public method deathSoundPath takes nothing returns string
			return this.m_deathSoundPath
		endmethod
		
		public method setOnCollisionFunction takes AMissileOnCollisionFunction onCollisionFunction returns nothing
			set this.m_onCollisionFunction = onCollisionFunction
		endmethod
		
		public method onCollisionFunction takes nothing returns AMissileOnCollisionFunction
			return this.m_onCollisionFunction
		endmethod
		
		public method setOnDeathFunction takes AMissileOnDeathFunction onDeathFunction returns nothing
			set this.m_onDeathFunction = onDeathFunction
		endmethod
		
		public method onDeathFunction takes nothing returns AMissileOnDeathFunction
			return this.m_onDeathFunction
		endmethod
		
		//convenience methods
		
		public method startFromUnit takes unit usedUnit returns nothing
			call this.start(GetUnitX(usedUnit), GetUnitY(usedUnit), GetUnitZ(usedUnit), GetUnitFacing(usedUnit)) //ALibraryMathsHandle
		endmethod
		
		/// Makes the missile unpaused which means that it will be moved next time when the periodic trigger moves all unpaused missiles.
		public method continue takes nothing returns nothing
			set this.paused = true
		endmethod

		/// A paused missile won't be moved until it gets unpaused.
		public method pause takes nothing returns nothing
			set this.paused = false
		endmethod
		
		//public methods

		/// Starts the missile from coordinates @param x, @param y, and @param z with angle @param angle.
		public method start takes real x, real y, real z, real angle returns nothing
			debug if (this.usedUnit != null) then
				debug call this.print("Missile has already been started.")
				debug return 
			debug endif

			set this.usedUnit = CreateUnit(this.m_owner, this.m_unitType, x, y, angle)
			call SetUnitInvulnerable(this.usedUnit, true)
			call MakeUnitSelectable(this.usedUnit, false) //ALibraryInterfaceSelection
			call SetUnitZ(this.usedUnit, z) //ALibraryMathsHandle
			set this.paused = false
			if (this.m_startSoundPath != null) then
				call PlaySound(this.m_startSoundPath) /// @todo use coordinates!
			endif
		endmethod

		/// Stops the missile. This means that the missile will be destroyed, damage will be distributed, a death effect will be shown, a death sound will be played and the death function will be executed.
		public method stop takes nothing returns nothing
			call this.causeDamage()
			call this.showDeathEffect()
			if (this.m_deathSoundPath != null) then
				call PlaySound(this.m_deathSoundPath) /// @todo use coordinates!
			endif
			call KillUnit(this.usedUnit)
			call RemoveUnit(this.usedUnit)
			set this.usedUnit = null
			set this.paused = true
			if (this.m_onDeathFunction != 0) then
				call this.m_onDeathFunction.execute(this)
			endif
		endmethod
		
		public method setPaused takes boolean paused returns nothing
			set this.paused = paused
		endmethod
		
		public method isPaused takes nothing returns boolean
			return this.paused
		endmethod
		
		//private methods

		private method move takes nothing returns nothing
			local rect mapRect = GetPlayableMapRect()
			local real currentX = GetUnitX(this.usedUnit)
			local real currentY = GetUnitY(this.usedUnit)
			local real angle
			local real newX
			local real newY
			
			if (this.m_targetWidget != null) then
				set angle = Atan2(GetWidgetY(this.m_targetWidget) - currentY, GetWidgetX(this.m_targetWidget) - currentX)
			else
				set angle = Atan2(this.m_targetY - currentY, this.m_targetX - currentX)
			endif
			set this.cos = Cos(angle)
			set this.sin = Sin(angle)
			call SetUnitFacing(this.usedUnit, angle)
			set newX = currentX + this.m_speed * this.cos
			set newY = currentY + this.m_speed * this.sin
			
			if (this.m_collides) then
				
			endif
			
			if (RectContainsCoords(mapRect, newX, newY) and IsTerrainPathable(newX, newY, PATHING_TYPE_WALKABILITY)) then //not?!
				call SetUnitX(this.usedUnit, newX)
				call SetUnitY(this.usedUnit, newY)
			else
				call this.stop()
			endif
		endmethod
		
		private method causeDamage takes nothing returns nothing
			if (this.m_damage <= 0.0) then
				return
			endif
		
			if (this.m_damageRange > 0.0) then
				call UnitDamagePoint(this.m_damageSource, 0.0, this.m_damageRange, GetUnitX(this.usedUnit), GetUnitY(this.usedUnit), this.m_damage, true, false, this.m_attackType, this.m_damageType, this.m_weaponType)
				//cause area damage to units who aren't allies of the source unit
			elseif (this.m_targetWidget != null) then
				call UnitDamageTarget(this.m_damageSource, this.m_targetWidget, this.m_damage, true, false, this.m_attackType, this.m_damageType, this.m_weaponType)
				//cause single target damage if missile hits widget otherwise show floating text?
			endif
		endmethod
		
		private method showDeathEffect takes nothing returns nothing
			local effect createdEffect
			if (this.m_deathEffectPath != null) then
				set createdEffect = AddSpecialEffect(this.m_deathEffectPath, GetUnitX(this.usedUnit), GetUnitY(this.usedUnit))
				call DestroyEffect(createdEffect)
				set createdEffect = null
			endif
		endmethod

		public static method create takes nothing returns AMissile
			local AMissile this = AMissile.allocate()
			//members
			set this.usedUnit = null
			set this.paused = true

			call AMissile.missiles.pushBack(this)
			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			//dynamic members
			set this.m_damageSource = null
			set this.m_attackType = null
			set this.m_damageType = null
			set this.m_weaponType = null
			//members
			if (this.usedUnit != null) then
				call RemoveUnit(this.usedUnit)
			endif
			set this.usedUnit = null
			
			call AMissile.missiles.remove(this)
		endmethod

		private static method refreshTriggerAction takes nothing returns nothing
			local integer i = 0
			loop
				exitwhen (i == AMissile.missiles.size())
				if (not AMissile.missiles[i].paused) then
					call AMissile.missiles[i].move()
				endif
				set i = i + 1
			endloop
		endmethod

		private static method createRefreshTrigger takes nothing returns nothing
			local event triggerEvent
			local triggeraction triggerAction
			set AMissile.refreshTrigger = CreateTrigger()
			set triggerEvent = TriggerRegisterTimerEvent(AMissile.refreshTrigger, AMissile.refreshTime, true)
			set triggerAction = TriggerAddAction(AMissile.refreshTrigger, function AMissile.refreshTriggerAction)
			set triggerEvent = null 
			set triggerAction = null
		endmethod

		public static method init takes real refreshTime, boolean enableCollisions returns nothing
			debug if (refreshTime <= 0.0) then
				debug call Print("Wrong value refresh time value in AMissile struct initialization: " + R2S(refreshTime) + ".")
			debug endif
			set AMissile.refreshTime = refreshTime
			set AMissile.enableCollisions = enableCollisions
			set AMissile.missiles = AMissileVector.create()
			call AMissile.createRefreshTrigger()
		endmethod

		public static method cleanUp takes nothing returns nothing
			local integer i = 0
			call DestroyTrigger(AMissile.refreshTrigger)
			set AMissile.refreshTrigger = null
			//remove all missiles
			loop
				exitwhen (i == AMissile.missiles.size())
				call AMissile.missiles[i].destroy()
				set i = i + 1
			endloop
			call AMissile.missiles.destroy()
		endmethod

		public static method enable takes nothing returns nothing
			call EnableTrigger(AMissile.refreshTrigger)
		endmethod

		public static method disable takes nothing returns nothing
			call DisableTrigger(AMissile.refreshTrigger)
		endmethod
		
		//convenience methods
		
		public static method createCircle takes real x, real y, real z, real distance, integer count, boolean start, player owner, integer unitType, real speed, real damage, real damageRange, unit damageSource, attacktype attackType, damagetype damageType, weapontype weaponType, boolean collides, string deathEffectPath, string deathSoundPath returns AMissileVector
			local AMissileVector vector = AMissileVector.create()
			local AMissile missile
			local real angle = 0.0
			local real angleValue = 360.0 / count
			local integer i = 0
			loop
				exitwhen (i == count)
				set missile = AMissile.create()
				call missile.setOwner(owner)
				call missile.setUnitType(unitType)
				call missile.setSpeed(speed)
				call missile.setDamage(damage)
				call missile.setDamageRange(damageRange)
				call missile.setDamageSource(damageSource)
				call missile.setAttackType(attackType)
				call missile.setDamageType(damageType)
				call missile.setWeaponType(weaponType)
				call missile.setCollides(collides)
				call missile.setTargetX(GetPolarProjectionX(x, angle, distance))
				call missile.setTargetY(GetPolarProjectionY(y, angle, distance))
				call missile.setDeathEffectPath(deathEffectPath)
				call missile.setDeathSoundPath(deathSoundPath)
				call vector.pushBack(missile)
				if (start) then
					call missile.start(x, y, z, angle)
				endif
				set angle = angle + angleValue
				set i = i + 1
			endloop
			return missile
		endmethod
		
		public static method createCircleFromUnit takes unit usedUnit, real distance, integer count, boolean start, player owner, integer unitType, real speed, real damage, real damageRange, unit damageSource, attacktype attackType, damagetype damageType, weapontype weaponType, boolean collides, string deathEffectPath, string deathSoundPath returns AMissileVector
			return AMissile.createCircle(GetUnitX(usedUnit), GetUnitY(usedUnit), GetUnitZ(usedUnit), distance, count, start, owner, unitType, speed, damage, damageRange, damageSource, attackType, damageType, weaponType, collides, deathEffectPath, deathSoundPath)
		endmethod
	endstruct

endlibrary
