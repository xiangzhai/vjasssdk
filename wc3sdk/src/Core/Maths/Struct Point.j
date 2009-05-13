library AStructCoreMathsPoint requires ALibraryCoreMathsPoint

	struct APoint
		//dynamic members
		private real x //should be protected
		private real y

		//dynamic members

		public method setX takes real x returns nothing
			set this.x = x
		endmethod

		public method getX takes nothing returns real
			return this.x
		endmethod

		public method setY takes real y returns nothing
			set this.y = y
		endmethod

		public method getY takes nothing returns real
			return this.y
		endmethod

		//methods

		public method getPolarProjectionX takes real angle, real distance returns real
			return GetPolarProjectionX(this.x, angle, distance)
		endmethod

		public method getPolarProjectionY takes real angle, real distance returns real
			return GetPolarProjectionY(this.y, angle, distance)
		endmethod

		public method getLocation takes real angle, real distance returns location
			return GetPolarProjectionOfPoint(this.getX(), this.getY(), angle, distance)
		endmethod

		public static method create takes real x, real y returns APoint
			local APoint this = APoint.allocate()
			//dynamic members
			set this.x = x
			set this.y = y

			return this
		endmethod
	endstruct

endlibrary
