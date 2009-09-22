library AStructCoreMathsVector3 requires ALibraryCoreMathsHandle, ALibraryCoreMathsPoint

	struct AVector3
		//dynamic members
		private real m_x
		private real m_y
		private real m_z

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

		public method setZ takes real z returns nothing
			set this.m_z = z
		endmethod

		public method z takes nothing returns real
			return this.m_z
		endmethod
		
		//convenience methods
		
		public method setWidget takes widget usedWidget returns nothing
			set this.m_x = GetWidgetX(usedWidget)
			set this.m_y = GetWidgetY(usedWidget)
			set this.m_z = GetWidgetZ(usedWidget)
		endmethod
		
		public method setUnit takes unit usedUnit returns nothing
			set this.m_x = GetUnitX(usedUnit)
			set this.m_y = GetUnitY(usedUnit)
			set this.m_z = GetUnitZ(usedUnit)
		endmethod

		//methods

		public method setLength takes real length returns nothing
			call this.scale((length / this.length()))
		endmethod

		public method length takes nothing returns real
			return SquareRoot((this.m_x * this.m_x) + (this.m_y * this.m_y) + (this.m_z * this.m_z))
		endmethod

		/// Copys all data from vector @param vector.
		public method copy takes thistype vector returns nothing
			set this.m_x = vector.m_x
			set this.m_y = vector.m_y
			set this.m_z = vector.m_z
		endmethod

		/// Adds all data from vector @param vector.
		/// Das Ergebnis ist ein Vektor, der vom Punkt 0 zur Spitze vom Vektor "this" verläuft, wenn man diesen an die Spitze des Vektors "vector" legt.
		public method add takes thistype vector returns nothing
			set this.m_x = this.m_x + vector.m_x
			set this.m_y = this.m_y + vector.m_y
			set this.m_z = this.m_z + vector.m_z
		endmethod


		/// Subtracts all data from vector @param vector.
		/// Das Ergebnis ist ein Vektor, der von der Spitze des Vektors "vector" zur Spitze des Vektors "this" verläuft.
		/// Achtung: Der Vektor verläuft immer vom 2. Wert zum 1. Wert der Subtraktion.
		public method subtract takes thistype vector returns nothing
			set this.m_x = this.m_x - vector.m_x
			set this.m_y = this.m_y - vector.m_y
			set this.m_z = this.m_z - vector.m_z
		endmethod

		/// Multiplies each value from vector @param vector and returns the sum of all results.
		/// Ist das Ergebnis 0, so entsteht ein rechter Winkel, wenn man die beiden Vektoren mit ihren Spitzen aneinander legt.
		public method multiply takes thistype vector returns real
			return ((this.m_x * vector.m_x) + (this.m_y * vector.m_y) + (this.m_z * vector.m_z))
		endmethod

		/// Multiplies all values with the value of @param factor.
		/// Ist der Betrag des Wertes grer oder gleich 1 oder kleiner als 0, so verlängert sich der Vektor.
		/// Anderenfalls wird der Vektor kürzer.
		public method scale takes real factor returns nothing
			set this.m_x = this.m_x * factor
			set this.m_y = this.m_y * factor
			set this.m_z = this.m_z * factor
		endmethod

		/// Adds scaled vector @param vector which is scaled by value @param factor.
		/// Note that vector @param vector won't be changed!
		/// @author peq
		public method addScaled takes thistype vector, real factor returns nothing
			set this.m_x = this.m_x + vector.m_x * factor
			set this.m_y = this.m_y + vector.m_y * factor
			set this.m_z = this.m_z + vector.m_z * factor
		endmethod

		/// Projects vector @param vector.
		/// Passt die Richtung des Vektors "this" an die Richtung des Vektors "vector" an ohne dabei die Länge des Vektors "this" zu verändern. 
		public method project takes thistype vector returns nothing
			local real factor = this.length() / vector.length()
			set this.m_x = vector.m_x * factor
			set this.m_y = vector.m_y * factor
			set this.m_z = vector.m_z * factor
		endmethod

		/// Rotates a vector through @param angle.
		/// Only x and y will be changed. Rotation on z axis is not possible yet.		
		public method rotate takes real angle returns nothing
			local real beta = (Asin(this.m_y / this.length()) - (angle * bj_DEGTORAD))
			set this.m_x = this.length() * Cos(beta)
			set this.m_y = this.length() * Sin(beta)
		endmethod


		// Gets the normal vector of the terrain at given coordinates. @param sampleRadius defines
		// how far apart the reference points will be, if they are further apart, the result will
		// be an impression of smoother terrain; normaly the value should be between 0 and 128.
		/// @todo Not tested yet.
		public method terrainNormal takes real x, real y, real sampleRadius returns nothing
			local real array z
			local thistype vectorX = thistype.create(0.0, 0.0, 0.0)
			local thistype vectorY = thistype.create(0.0, 0.0, 0.0)
			//Z
			set z[0] = GetTerrainZ((x - sampleRadius), y)
			set z[1] = GetTerrainZ((x + sampleRadius), y)
			set z[2] = GetTerrainZ((y - sampleRadius), y)
			set z[3] = GetTerrainZ((y + sampleRadius), y)
			//Vector X
			set vectorX.m_x = (2.0 * sampleRadius)
			//set VectorX.Y = 0.00
			set vectorX.m_z =  (z[1] - z[0])
			//Vector Y
			//set VectorY.X = 0.00
			set vectorY.m_y = (2.0 * sampleRadius)
			set vectorY.m_z =  (z[3] - z[2])
			//Add
			call vectorX.add(vectorY)
			//Copy
			call this.copy(vectorX)
			//Destroy
			call thistype.destroy(vectorX)
			call thistype.destroy(vectorY)
		endmethod

		public static method create takes real x, real y, real z returns thistype
			local thistype this = thistype.allocate()
			//dynamic members
			set this.m_x = x
			set this.m_y = y
			set this.m_z = z

			return this
		endmethod

		/// Returns the product of the two vectors @param vector0 and @vector1 in a new vector instance.
		public static method product takes thistype vector0, thistype vector1 returns thistype
			local thistype result = thistype.create(0.0, 0.0, 0.0)
			call result.copy(vector0)
			call result.add(vector1)
			return result
		endmethod

		/// Returns the difference of the two vectors @param vector0 and @vector1 in a new vector instance.
		public static method difference takes thistype vector0, thistype vector1 returns thistype
			local thistype result = thistype.create(0.0, 0.0, 0.0)
			call result.copy(vector0)
			call result.subtract(vector1)
			return result
		endmethod

		/// Returns the multiplication of the two vectors @param vector0 and @vector1 in a new vector instance.
		/// @author Tamino Dauth
		/// @state untested
		public method multiplication takes thistype vector0, thistype vector1 returns thistype
			local thistype result = thistype.create(0.0, 0.0, 0.0)
			call result.copy(vector0)
			call result.multiply(vector1)
			return result
		endmethod

		/// Returns the projection of the two vectors @param vector0 and @vector1 in a new vector instance.
		/// @author Tamino Dauth
		/// @state untested
		public method projection takes thistype vector0, thistype vector1 returns thistype
			local thistype result = thistype.create(0.0, 0.0, 0.0)
			call result.copy(vector0)
			call result.project(vector1)
			return result
		endmethod
	endstruct

endlibrary