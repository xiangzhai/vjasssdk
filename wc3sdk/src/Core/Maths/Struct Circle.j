library AStructCoreMathsCircle requires AStructCoreMathsPoint, ALibraryCoreMathsPoint

	struct ACircle extends APoint
		//dynamic members
		private real radius

		//dynamic members

		public method setRadius takes real radius returns nothing
			set this.radius = radius
		endmethod

		public method getRadius takes nothing returns real
			return this.radius
		endmethod

		//methods

		public method getArea takes nothing returns real
			return Pow(this.radius, 2.0) * bj_PI
		endmethod

		public method containsPoint takes real x, real y returns boolean
			return (GetDistanceBetweenPoints(x, y, 0.0, this.getX(), this.getY(), 0.0) <= this.radius) //ALibraryMathsPoint
		endmethod

		//instead of getX() and getY() it has to be a protected element
		//but there is no protected keyword...
		public method borderContainsPoint takes real X, real Y returns boolean
			return (GetDistanceBetweenPoints(X, Y, 0.0, this.getX(), this.getY(), 0.0) == this.radius) //ALibraryMathsPoint
		endmethod

		//! textmacro ACircleMacro takes TYPE, TYPENAME
			public method contains$TYPENAME$ takes $TYPE$ circle$TYPENAME$ returns boolean
				return this.containsPoint(Get$TYPENAME$X(circle$TYPENAME$), Get$TYPENAME$Y(circle$TYPENAME$)) //Because of the location type I use the 'Circle' prefix
			endmethod
			
			public method borderContains$TYPENAME$ takes $TYPE$ circle$TYPENAME$ returns boolean
				return this.borderContainsPoint(Get$TYPENAME$X(circle$TYPENAME$), Get$TYPENAME$Y(circle$TYPENAME$))
			endmethod
		//! endtextmacro
	
		//! runtextmacro ACircleMacro("location", "Location")
		//! runtextmacro ACircleMacro("widget", "Widget")
		//! runtextmacro ACircleMacro("unit", "Unit")
		//! runtextmacro ACircleMacro("destructable", "Destructable")
		//! runtextmacro ACircleMacro("item", "Item")

		public method getBorderLocation takes real angle returns location
			return GetPolarProjectionOfPoint(this.getX(), this.getY(), angle, this.radius) //ALibraryMathsPoint
		endmethod
	
		public method getRandomLocation takes nothing returns location
			local real distance = (SquareRoot(GetRandomReal(0.0, 1.0)) * this.radius)
			local real angle = GetRandomReal(0.0, (2.0 * bj_PI))
			return Location((this.getX() + (distance * Cos(angle))), (this.getY() + (distance * Sin(angle))))
		endmethod

		/// @author Tamino Dauth
		/// @return Added special effect.
		public method addSpecialEffect takes string modelName, real angle, real distance returns effect
			return AddSpecialEffect(modelName, this.getPolarProjectionX(angle, distance), this.getPolarProjectionY(angle, distance))
		endmethod

		/// @author Tamino Dauth
		/// @return Added spell effect.
		public method addSpellEffect takes string abilityString, effecttype effectType, real angle, real distance returns effect
			return AddSpellEffect(abilityString, effectType, this.getPolarProjectionX(angle, distance), this.getPolarProjectionY(angle, distance))
		endmethod

		/// @author Tamino Dauth
		/// @return Added spell effect.
		public method addSpellEffectById takes integer abilityId, effecttype effectType, real angle, real distance returns effect
			return AddSpellEffectById(abilityId, effectType, this.getPolarProjectionX(angle, distance), this.getPolarProjectionY(angle, distance))
		endmethod

		/// @author Tamino Dauth
		/// @return Created unit.
		public method createUnit takes player user, integer unitType, real angle, real distance, real face returns unit
			return CreateUnit(user, unitType, this.getPolarProjectionX(angle, distance), this.getPolarProjectionY(angle, distance), face)
		endmethod

		/// @author Tamino Dauth
		/// @return Created destructable.
		public method createDestructable takes integer destructableType, real angle, real distance, real face, real scale, integer variation returns destructable
			return CreateDestructable(destructableType, this.getPolarProjectionX(angle, distance), this.getPolarProjectionY(angle, distance), face, scale, variation)
		endmethod

		/// @author Tamino Dauth
		/// @return Created destructable.
		public method createDestructableZ takes integer destructableType, real angle, real distance, real z, real face, real scale, integer variation returns destructable
			return CreateDestructableZ(destructableType, this.getPolarProjectionX(angle, distance), this.getPolarProjectionY(angle, distance), z, face, scale, variation)
		endmethod

		/// @author Tamino Dauth
		/// @return Created item.
		public method createItem takes integer itemType, real angle, real distance returns item
			return CreateItem(itemType, this.getPolarProjectionX(angle, distance), this.getPolarProjectionY(angle, distance))
		endmethod

		/// @author Tamino Dauth
		/// @return Created trackable.
		public method createTrackable takes string modelPath, real angle, real distance, real facing returns trackable
			return CreateTrackable(modelPath, this.getPolarProjectionX(angle, distance), this.getPolarProjectionY(angle, distance), facing)
		endmethod

		public static method create takes real x, real y, real radius returns ACircle
			local ACircle this = ACircle.allocate(x, y)
			//dynamic members
			set this.radius = radius

			return this
		endmethod
	endstruct

endlibrary