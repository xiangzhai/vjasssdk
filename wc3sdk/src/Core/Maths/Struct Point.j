library AStructCoreMathsPoint requires ALibraryCoreMathsPoint

	struct APoint
		//dynamic members
		private real m_x //should be protected
		private real m_y

		//dynamic members

		public method setX takes real x returns nothing
			set this.m_x = x
		endmethod

		public method x takes nothing returns real
			return this.m_x
		endmethod

		public method setY takes real y returns nothing
			set this.m_y = y
		endmethod

		public method y takes nothing returns real
			return this.m_y
		endmethod

		//methods

		public method polarProjectionX takes real angle, real distance returns real
			return GetPolarProjectionX(this.m_x, angle, distance)
		endmethod

		public method polarProjectionY takes real angle, real distance returns real
			return GetPolarProjectionY(this.m_y, angle, distance)
		endmethod

		public method location takes real angle, real distance returns location
			return GetPolarProjectionOfPoint(this.m_x, this.m_y, angle, distance)
		endmethod

		public static method create takes real x, real y returns APoint
			local APoint this = APoint.allocate()
			//dynamic members
			set this.m_x = x
			set this.m_y = y

			return this
		endmethod
	endstruct

endlibrary
