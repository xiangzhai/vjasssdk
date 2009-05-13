library AStructCoreMathsVector requires ALibraryCoreMathsHandle, ALibraryCoreMathsPoint

	struct AVector
		//dynamic members
		private real x
		private real y
		private real z

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

		public method setZ takes real z returns nothing
			set this.z = z
		endmethod

		public method getZ takes nothing returns real
			return this.z
		endmethod
		
		//convenience methods
		
		public method setWidget takes widget usedWidget returns nothing
			set this.x = GetWidgetX(usedWidget)
			set this.y = GetWidgetY(usedWidget)
			set this.z = GetWidgetZ(usedWidget)
		endmethod
		
		public method setUnit takes unit usedUnit returns nothing
			set this.x = GetUnitX(usedUnit)
			set this.y = GetUnitY(usedUnit)
			set this.z = GetUnitZ(usedUnit)
		endmethod

		//methods

		public method setLength takes real length returns nothing
			call this.scale((length / this.getLength()))
		endmethod

		public method getLength takes nothing returns real
			return SquareRoot((this.x * this.x) + (this.y * this.y) + (this.z * this.z))
		endmethod

		/// Copys all data from vector @param vector.
		public method copy takes AVector vector returns nothing
			set this.x = vector.x
			set this.y = vector.y
			set this.z = vector.z
		endmethod

		/// Adds all data from vector @param vector.
		/// Das Ergebnis ist ein Vektor, der vom Punkt 0 zur Spitze vom Vektor "this" verläuft, wenn man diesen an die Spitze des Vektors "vector" legt.
		public method add takes AVector vector returns nothing
			set this.x = this.x + vector.x
			set this.y = this.y + vector.y
			set this.z = this.z + vector.z
		endmethod


		/// Subtracts all data from vector @param vector.
		/// Das Ergebnis ist ein Vektor, der von der Spitze des Vektors "vector" zur Spitze des Vektors "this" verläuft.
		/// Achtung: Der Vektor verläuft immer vom 2. Wert zum 1. Wert der Subtraktion.
		public method subtract takes AVector vector returns nothing
			set this.x = this.x - vector.x
			set this.y = this.y - vector.y
			set this.z = this.z - vector.z
		endmethod

		/// Multiplies each value from vector @param vector and returns the sum of all results.
		/// Ist das Ergebnis 0, so entsteht ein rechter Winkel, wenn man die beiden Vektoren mit ihren Spitzen aneinander legt.
		public method multiply takes AVector vector returns real
			return ((this.x * vector.x) + (this.y * vector.y) + (this.z * vector.z))
		endmethod

		/// Multiplies all values with the value of @param factor.
		/// Ist der Betrag des Wertes grer oder gleich 1 oder kleiner als 0, so verlängert sich der Vektor.
		/// Anderenfalls wird der Vektor kürzer.
		public method scale takes real factor returns nothing
			set this.x = this.x * factor
			set this.y = this.y * factor
			set this.z = this.z * factor
		endmethod

		/// Adds scaled vector @param vector which is scaled by value @param factor.
		/// Note that vector @param vector won't be changed!
		/// @author peq
		public method addScaled takes AVector vector, real factor returns nothing
			set this.x = this.x + vector.x * factor
			set this.y = this.y + vector.y * factor
			set this.z = this.z + vector.z * factor
		endmethod

		/// Projects vector @param vector.
		/// Passt die Richtung des Vektors "this" an die Richtung des Vektors "vector" an ohne dabei die Länge des Vektors "this" zu verändern. 
		public method project takes AVector vector returns nothing
			local real factor = this.getLength() / vector.getLength()
			set this.x = vector.x * factor
			set this.y = vector.y * factor
			set this.z = vector.z * factor
		endmethod

		/// Rotates a vector through @param angle.
		/// Only x and y will be changed. Rotation on z axis is not possible yet.		
		public method rotate takes real angle returns nothing
			local real beta = (Asin(this.y / this.getLength()) - (angle * bj_DEGTORAD))
			set this.x = this.getLength() * Cos(beta)
			set this.y = this.getLength() * Sin(beta)
		endmethod


		// Gets the normal vector of the terrain at given coordinates. @param sampleRadius defines
		// how far apart the reference points will be, if they are further apart, the result will
		// be an impression of smoother terrain; normaly the value should be between 0 and 128.
		/// @todo Not tested yet.
		public method getTerrainNormal takes real x, real y, real sampleRadius returns nothing
			local real array z
			local AVector vectorX = AVector.create(0.0, 0.0, 0.0)
			local AVector vectorY = AVector.create(0.0, 0.0, 0.0)
			//Z
			set z[0] = GetTerrainZ((x - sampleRadius), y)
			set z[1] = GetTerrainZ((x + sampleRadius), y)
			set z[2] = GetTerrainZ((y - sampleRadius), y)
			set z[3] = GetTerrainZ((y + sampleRadius), y)
			//Vector X
			set vectorX.x = (2.0 * sampleRadius)
			//set VectorX.Y = 0.00
			set vectorX.z =  (z[1] - z[0])
			//Vector Y
			//set VectorY.X = 0.00
			set vectorY.y = (2.0 * sampleRadius)
			set vectorY.z =  (z[3] - z[2])
			//Add
			call vectorX.add(vectorY)
			//Copy
			call this.copy(vectorX)
			//Destroy
			call AVector.destroy(vectorX)
			call AVector.destroy(vectorY)
		endmethod

		public static method create takes real x, real y, real z returns AVector
			local AVector this = AVector.allocate()
			//dynamic members
			set this.x = x
			set this.y = y
			set this.z = z

			return this
		endmethod

		/// Returns the product of the two vectors @param vector0 and @vector1 in a new vector instance.
		public static method product takes AVector vector0, AVector vector1 returns AVector
			local AVector result = AVector.create(0.0, 0.0, 0.0)
			call result.copy(vector0)
			call result.add(vector1)
			return result
		endmethod

		/// Returns the difference of the two vectors @param vector0 and @vector1 in a new vector instance.
		public static method difference takes AVector vector0, AVector vector1 returns AVector
			local AVector result = AVector.create(0.0, 0.0, 0.0)
			call result.copy(vector0)
			call result.subtract(vector1)
			return result
		endmethod

		/// Returns the multiplication of the two vectors @param vector0 and @vector1 in a new vector instance.
		/// @author Tamino Dauth
		/// @state untested
		public method multiplication takes AVector vector0, AVector vector1 returns AVector
			local AVector result = AVector.create(0.0, 0.0, 0.0)
			call result.copy(vector0)
			call result.multiply(vector1)
			return result
		endmethod

		/// Returns the projection of the two vectors @param vector0 and @vector1 in a new vector instance.
		/// @author Tamino Dauth
		/// @state untested
		public method projection takes AVector vector0, AVector vector1 returns AVector
			local AVector result = AVector.create(0.0, 0.0, 0.0)
			call result.copy(vector0)
			call result.project(vector1)
			return result
		endmethod
	endstruct

endlibrary