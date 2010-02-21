library AStructCoreEnvironmentMissile requires optional ALibraryCoreDebugMisc, AModuleCoreGeneralSystemStruct, AStructCoreGeneralVector, AStructCoreMathsVector3, ALibraryCoreMathsHandle, ALibraryCoreMathsPoint, ALibraryCoreMathsUnit, ALibraryCoreInterfaceSelection

	/// OnCollisionFunction functions can be set by method @method AMissileType.setOnCollisionFunction and will be called when missile collides.
	function interface AMissileTypeOnCollisionFunction takes AMissile missile returns nothing

	/// OnDeathFunction functions can by set by method @method AMissileType.setOnDeathFunction and will be called when missile hits target.
	function interface AMissileTypeOnDeathFunction takes AMissile missile returns nothing

	struct AMissileType
		private player m_owner
		private integer m_unitType
		private real m_speed
		private boolean m_targetSeeking
		private boolean m_collides
		private boolean m_destroyOnDeath
		private string m_deathEffectPath
		private string m_startSoundPath
		private string m_deathSoundPath
		private AMissileTypeOnCollisionFunction m_onCollisionFunction
		private AMissileTypeOnDeathFunction m_onDeathFunction

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

		/**
		* @param speed Distance per second (without gravitational acceleration).
		*/
		public method setSpeed takes real speed returns nothing
			set this.m_speed = speed * AMissile.refreshTime.evaluate()
		endmethod

		public method speed takes nothing returns real
			return this.m_speed
		endmethod

		public method setTargetSeeking takes boolean targetSeeking returns nothing
			set this.m_targetSeeking = targetSeeking
		endmethod

		public method targetSeeking takes nothing returns boolean
			return this.m_targetSeeking
		endmethod

		public method setCollides takes boolean collides returns nothing
			set this.m_collides = collides
		endmethod

		public method collides takes nothing returns boolean
			return this.m_collides
		endmethod

		public method setDestroyOnDeath takes boolean destroyOnDeath returns nothing
			set this.m_destroyOnDeath = destroyOnDeath
		endmethod

		public method destroyOnDeath takes nothing returns boolean
			return this.m_destroyOnDeath
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

		public method setOnCollisionFunction takes AMissileTypeOnCollisionFunction onCollisionFunction returns nothing
			set this.m_onCollisionFunction = onCollisionFunction
		endmethod

		public method onCollisionFunction takes nothing returns AMissileTypeOnCollisionFunction
			return this.m_onCollisionFunction
		endmethod

		public method setOnDeathFunction takes AMissileTypeOnDeathFunction onDeathFunction returns nothing
			set this.m_onDeathFunction = onDeathFunction
		endmethod

		public method onDeathFunction takes nothing returns AMissileTypeOnDeathFunction
			return this.m_onDeathFunction
		endmethod

		public static method create takes nothing returns thistype
			local thistype this = thistype.allocate()
			//dynamic members
			set this.m_owner = null
			set this.m_unitType = 'hfoo'
			set this.m_speed = 100.0
			set this.m_targetSeeking = false
			set this.m_collides = false
			set this.m_destroyOnDeath = false
			set this.m_deathEffectPath = null
			set this.m_startSoundPath = null
			set this.m_deathSoundPath = null
			set this.m_onCollisionFunction = 0
			set this.m_onDeathFunction = 0
			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			//dynamic members
			set this.m_owner = null
		endmethod
	endstruct

	/**
	* Provides the functionality of a single physical missile which can have a specific missile type, a widget source and target or three coordinate values (x, y and z).
	* @todo Incompleted!
	* @todo Add static methods for missile containers.
	* @todo Collision between missiles?!
	* @author Draculark
	* @author Tamino Dauth
	* @link http://warcraft.ingame.de/forum/showthread.php?s=6f44abe813a621c950b94373b91ed929&threadid=186184
	*/
	struct AMissile
		//static start members
		private static real m_refreshTime
		private static real m_gravitationalAcceleration
		private static boolean m_enableCollisions
		//static members
		private static AIntegerVector m_missiles
		private static timer m_refreshTimer
		//dynamic members
		private AMissileType m_missileType
		private real m_targetX
		private real m_targetY
		private real m_targetZ
		private widget m_targetWidget
		private boolean m_isPaused
		//members
		private AVector3 m_speed
		private unit m_unit
		private integer m_index

		implement ASystemStruct

		//! runtextmacro optional A_STRUCT_DEBUG("\"AMissile\"")

		//dynamic members

		public method setMissileType takes AMissileType missileType returns nothing
			set this.m_missileType = missileType
		endmethod

		public method missileType takes nothing returns AMissileType
			return this.m_missileType
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

		public method setTargetZ takes real targetZ returns nothing
			set this.m_targetZ = targetZ
		endmethod

		public method targetZ takes nothing returns real
			return this.m_targetZ
		endmethod

		public method setTargetWidget takes widget targetWidget returns nothing
			set this.m_targetWidget = targetWidget
		endmethod

		public method targetWidget takes nothing returns widget
			return this.m_targetWidget
		endmethod

		public method setPaused takes boolean isPaused returns nothing
			set this.m_isPaused = isPaused
		endmethod

		public method isPaused takes nothing returns boolean
			return this.m_isPaused
		endmethod

		//members

		public method unit takes nothing returns unit
			return this.m_unit
		endmethod

		//convenience methods

		public method startFromUnit takes unit whichUnit returns nothing
			call this.start.evaluate(GetUnitX(whichUnit), GetUnitY(whichUnit), 0.0)
		endmethod

		public method startFromUnitZ takes unit whichUnit returns nothing
			call this.start.evaluate(GetUnitX(whichUnit), GetUnitY(whichUnit), GetUnitZ(whichUnit))
		endmethod

		/// Makes the missile unpaused which means that it will be moved next time when the periodic trigger moves all unpaused missiles.
		public method continue takes nothing returns nothing
			set this.m_isPaused = true
		endmethod

		/// A paused missile won't be moved until it gets unpaused.
		public method pause takes nothing returns nothing
			set this.m_isPaused = false
		endmethod

		//public methods

		public method onTheWay takes nothing returns boolean
			return this.m_speed != 0
		endmethod

		public method x takes nothing returns real
static if (DEBUG_MODE) then
			if (not this.onTheWay()) then
				call this.print("Is not on the way.")
				return 0.0
			endif
endif
			return GetUnitX(this.m_unit)
		endmethod

		public method y takes nothing returns real
static if (DEBUG_MODE) then
			if (not this.onTheWay()) then
				call this.print("Is not on the way.")
				return 0.0
			endif
endif
			return GetUnitY(this.m_unit)
		endmethod

		public method z takes nothing returns real
static if (DEBUG_MODE) then
			if (not this.onTheWay()) then
				call this.print("Is not on the way.")
				return 0.0
			endif
endif
			return GetUnitZ(this.m_unit)
		endmethod

		/// Starts the missile from coordinates @param x, @param y, and @param z with angle @param angle.
		public method start takes real x, real y, real z returns nothing
			local real angle
			debug if (this.onTheWay()) then
				debug call this.print("Missile has already been started.")
				debug return
			debug endif
			debug if (this.m_missileType == 0) then
				debug call this.print("Can't start with missile type 0.")
			debug endif
			if (this.m_targetWidget != null) then

				set angle = GetAngleBetweenPoints(x, y, GetWidgetX(this.m_targetWidget), GetWidgetY(this.m_targetWidget))
			else
				set angle = GetAngleBetweenPoints(x, y, this.m_targetX, this.m_targetY)
			endif
			set this.m_speed = AVector3.create(this.m_missileType.speed(), this.m_missileType.speed(), this.m_missileType.speed())
			call this.m_speed.rotate(angle)
			set this.m_unit = CreateUnit(this.m_missileType.owner(), this.m_missileType.unitType(), x, y, angle)
			call SetUnitInvulnerable(this.m_unit, true)
			call MakeUnitSelectable(this.m_unit, false)
			call SetUnitZ(this.m_unit, z)
			set this.m_isPaused = false
			if (this.m_missileType.startSoundPath() != null) then
				call PlaySoundFileAt(this.m_missileType.startSoundPath(), x, y, z)
			endif
		endmethod

		/**
		* Stops the missile.
		* This means that the missile will be destroyed, damage will be distributed, a death effect will be shown,
		* a death sound will be played and the death function will be executed.
		*/
		public method stop takes nothing returns nothing
			local effect whichEffect
			if (this.m_missileType.deathEffectPath() != null) then
				set whichEffect = AddSpecialEffect(this.m_missileType.deathEffectPath(), GetUnitX(this.m_unit), GetUnitY(this.m_unit)) /// @todo Can't use Z value with special effects.
				call DestroyEffect(whichEffect)
				set whichEffect = null
			endif
			if (this.m_missileType.deathSoundPath() != null) then
				call PlaySoundFileAt(this.m_missileType.deathSoundPath(), GetUnitX(this.m_unit), GetUnitY(this.m_unit), GetUnitZ(this.m_unit))
			endif
			call this.m_speed.destroy()
			set this.m_speed = 0
			call KillUnit(this.m_unit)
			call RemoveUnit(this.m_unit)
			set this.m_unit = null
			set this.m_isPaused = true
			if (this.m_missileType.onDeathFunction() != 0) then
				call this.m_missileType.onDeathFunction().execute(this)
			endif
			if (this.m_missileType.destroyOnDeath()) then
				call this.destroy()
			endif
		endmethod

		//private methods

		private method move takes nothing returns nothing
			local rect mapRect = GetPlayableMapRect()
			local real currentX = GetUnitX(this.m_unit)
			local real currentY = GetUnitY(this.m_unit)
			local real angle
			local real newX
			local real newY
			local real newZ
			local integer i
			local boolean collided = false

			if (this.m_missileType.targetSeeking()) then
				if (this.m_targetWidget != null) then
					//set currentDistance = GetDistanceBetweenPoints(currentX, currentY, 0.0, GetWidgetX(this.m_targetWidget), GetWidgetY(this.m_targetWidget), 0.0)
					set angle = GetAngleBetweenPoints(currentX, currentY, GetWidgetX(this.m_targetWidget), GetWidgetY(this.m_targetWidget))
				else
					//set currentDistance = GetDistanceBetweenPoints(currentX, currentY, 0.0, this.m_targetX, this.m_targetY, 0.0)
					set angle = GetAngleBetweenPoints(currentX, currentY, this.m_targetX, this.m_targetY)
				endif
				call this.m_speed.rotate(angle)
				call SetUnitFacing(this.m_unit, angle)
			endif
			set newX = GetPolarProjectionX(currentX, angle, this.m_missileType.speed())
			set newY = GetPolarProjectionY(currentY, angle, this.m_missileType.speed())

			debug call this.print("Move")

			if (RectContainsCoords(mapRect, newX, newY) and not IsTerrainPathable(newX, newY, PATHING_TYPE_WALKABILITY)) then //not?!
				debug call this.print("Is pathable and on map.")
				/// @todo Doesn't work if it's target seeking, Z value has to be shortend much more if target gets nearer to missile position?
				call this.m_speed.setZ(this.m_speed.z() - thistype.m_gravitationalAcceleration)
				set newZ = GetUnitZ(this.m_unit) + this.m_speed.z()
				debug call this.print("New X " + R2S(newX) + ", new Y " + R2S(newY) + ", new Z " + R2S(newZ) + ".")
				// hits ground
				if (newZ <= GetTerrainZ(newX, newY)) then
					debug call this.print("Hit ground")
					call this.stop()
				elseif (this.m_missileType.collides()) then
					/// @todo Check for things, maybe other missiles?
					set i = 0
					loop
						exitwhen (i == thistype.m_missiles.size())
						// collides with missile
						if (i != this.m_index and thistype(thistype.m_missiles[i]).m_missileType.collides() and newX == thistype(thistype.m_missiles[i]).x() and newY == thistype(thistype.m_missiles[i]).y() and newZ == thistype(thistype.m_missiles[i]).z()) then
							debug call this.print("Collision between " + I2S(this) + " and " + I2S(thistype.m_missiles[i]) + ".")
							set collided = true
							call thistype(thistype.m_missiles[i]).stop()
							call this.stop()
							exitwhen (true)
						endif
						set i = i + 1
					endloop
				endif

				if (not collided) then
					debug call this.print("Not collided.")
					// hits target widget
					if (this.m_targetWidget != null and newX == GetWidgetX(this.m_targetWidget) and newY == GetWidgetY(this.m_targetWidget) and newZ == GetWidgetZ(this.m_targetWidget)) then
						call this.stop()
					// hits target point
					elseif (newX == this.m_targetX and newY == this.m_targetY and newZ == this.m_targetZ) then
						call this.stop()
					else
						call SetUnitX(this.m_unit, newX)
						call SetUnitY(this.m_unit, newY)
						call SetUnitZ(this.m_unit, newZ)
						debug call this.print("2 New X " + R2S(newX) + ", new Y " + R2S(newY) + ", new Z " + R2S(newZ) + ".")
					endif
				endif
			// out of map rect
			else
				call this.stop()
			endif
			set mapRect = null
		endmethod

		public static method create takes nothing returns AMissile
			local thistype this = thistype.allocate()
			//dynamic members
			set this.m_missileType = 0
			set this.m_isPaused = true
			//members
			set this.m_speed = 0
			set this.m_unit = null
			call thistype.m_missiles.pushBack(this)
			set this.m_index = thistype.m_missiles.backIndex()
			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			//static members
			call thistype.m_missiles.erase(this.m_index)
			//members
			if (this.m_speed != 0) then
				call this.m_speed.destroy()
			endif
			if (this.m_unit != null) then
				call RemoveUnit(this.m_unit)
			endif
			set this.m_unit = null
		endmethod

		private static method onInit takes nothing returns nothing
			call thistype.setName("AMissile")
		endmethod

		private static method timerFunctionRefresh takes nothing returns nothing
			local integer i = thistype.m_missiles.backIndex()
			loop
				exitwhen (i < 0)
				if (not thistype(thistype.m_missiles[i]).m_isPaused) then
					call thistype(thistype.m_missiles[i]).move()
				endif
				set i = i - 1
			endloop
		endmethod

		/**
		* @param gravitationalAcceleration Earth average: 9.80665.
		*/
		public static method init takes real refreshTime, real gravitationalAcceleration, boolean enableCollisions returns nothing
			debug if (refreshTime <= 0.0) then
				debug call thistype.staticPrint("Wrong value refresh time value in AMissile struct initialization: " + R2S(refreshTime) + ".")
			debug endif
			//static start members
			set thistype.m_refreshTime = refreshTime
			set thistype.m_gravitationalAcceleration = gravitationalAcceleration * refreshTime
			set thistype.m_enableCollisions = enableCollisions
			//static members
			set thistype.m_missiles = AIntegerVector.create()
			set thistype.m_refreshTimer = CreateTimer()
			call TimerStart(thistype.m_refreshTimer, thistype.m_refreshTime, true, function thistype.timerFunctionRefresh)
			call thistype.initialize()
		endmethod

		public static method cleanUp takes nothing returns nothing
			call thistype.uninitialize()
			call DestroyTimer(thistype.m_refreshTimer)
			set thistype.m_refreshTimer = null
			//remove all missiles
			loop
				exitwhen (thistype.m_missiles.empty())
				call thistype(thistype.m_missiles.back()).destroy()
			endloop
			call thistype.m_missiles.destroy()
		endmethod

		public static method enable takes nothing returns nothing
			call ResumeTimer(thistype.m_refreshTimer)
		endmethod

		public static method disable takes nothing returns nothing
			call PauseTimer(thistype.m_refreshTimer)
		endmethod

		//dynamic static members

		public static method refreshTime takes nothing returns real
			return thistype.m_refreshTime
		endmethod

		//convenience methods
		/*
		public static method createCircle takes real x, real y, real z, real distance, integer count, boolean start, player owner, integer unitType, real speed, real damage, real damageRange, unit damageSource, attacktype attackType, damagetype damageType, weapontype weaponType, boolean collides, string deathEffectPath, string deathSoundPath returns AMissileVector
			local AMissileVector vector = AMissileVector.create()
			local thistype missile
			local real angle = 0.0
			local real angleValue = 360.0 / count
			local integer i = 0
			loop
				exitwhen (i == count)
				set missile = thistype.create()
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
			return thistype.createCircle(GetUnitX(usedUnit), GetUnitY(usedUnit), GetUnitZ(usedUnit), distance, count, start, owner, unitType, speed, damage, damageRange, damageSource, attackType, damageType, weaponType, collides, deathEffectPath, deathSoundPath)
		endmethod
		*/
	endstruct

	struct ADamageMissileType extends AMissileType
		//dynamic members
		private real m_damage
		private real m_damageRange
		private unit m_damageSource
		private attacktype m_attackType
		private damagetype m_damageType
		private weapontype m_weaponType

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

		private static method onDeathFunctionDamage takes AMissile missile returns nothing
			local ADamageMissileType this = ADamageMissileType(missile.missileType())
			if (this.damage() <= 0.0) then
				return
			endif
			if (this.damageRange() > 0.0) then
				call UnitDamagePoint(this.damageSource(), 0.0, this.damageRange(), GetUnitX(missile.unit()), GetUnitY(missile.unit()), this.damage(), true, false, this.attackType(), this.damageType(), this.weaponType())
				//cause area damage to units who aren't allies of the source unit
			elseif (missile.targetWidget.evaluate() != null) then
				call UnitDamageTarget(this.damageSource(), missile.targetWidget(), this.damage(), true, false, this.attackType(), this.damageType(), this.weaponType())
				//cause single target damage if missile hits widget otherwise show floating text?
			endif
		endmethod

		public static method create takes nothing returns thistype
			local thistype this = thistype.allocate()
			//dynamic members
			set this.m_damage = 0.0
			set this.m_damageRange = 0.0
			set this.m_damageSource = null
			set this.m_attackType = ATTACK_TYPE_NORMAL
			set this.m_damageType = DAMAGE_TYPE_NORMAL
			set this.m_weaponType = WEAPON_TYPE_WHOKNOWS

			call this.setOnDeathFunction(thistype.onDeathFunctionDamage)
			return this
		endmethod

		public method onDestroy takes nothing returns nothing
			//dynamic members
			set this.m_damageSource = null
			set this.m_attackType = null
			set this.m_damageType = null
			set this.m_weaponType = null
		endmethod
	endstruct

endlibrary
