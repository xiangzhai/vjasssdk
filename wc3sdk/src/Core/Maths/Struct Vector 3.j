library AStructCoreMathsVector3 requires ALibraryCoreMathsHandle, ALibraryCoreMathsPoint

	/**
	* Provides access to three-dimensional vector.
	* Three-dimensional vectors are often used in 3d graphics to define positions or speed.
	* They can simplify calculations.
	*/
	struct AVector3
		// dynamic members
		private real m_x
		private real m_y
		private real m_z

		// dynamic members

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

		// convenience methods

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

		public method setItem takes item whichItem returns nothing
			set this.m_x = GetItemX(whichItem)
			set this.m_y = GetItemY(whichItem)
			set this.m_z = GetItemZ(whichItem)
		endmethod

		public method setDestructable takes destructable whichDestructable returns nothing
			set this.m_x = GetDestructableX(whichDestructable)
			set this.m_y = GetDestructableY(whichDestructable)
			set this.m_z = GetDestructableZ(whichDestructable)
		endmethod

		// methods

		/**
		* Multiplies all values with the value of \p factor.
		* If \p factor is smaller than 1 and bigger or equal to 0 the vector will shorten otherwise it will lengthen.
		*/
		public method scale takes real factor returns nothing
			set this.m_x = this.m_x * factor
			set this.m_y = this.m_y * factor
			set this.m_z = this.m_z * factor
		endmethod

		public method length takes nothing returns real
			return SquareRoot(Pow(this.m_x, 2.0) + Pow(this.m_y, 2.0) + Pow(this.m_z, 2.0))
		endmethod

		public method setLength takes real length returns nothing
			call this.scale((length / this.length()))
		endmethod

		/**
		* Similar to \ref AVector3.setLength.
		*/
		public method setNorm takes real norm returns nothing
			call this.setLength(norm)
		endmethod

		/**
		* Similar to \ref AVector3.length.
		*/
		public method norm takes nothing returns real
			return this.length()
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

		/**
		* Multiplies each value from vector \p vector and returns the sum of all results.
		* Doesn't change any vector data!
		* If the result is 0 there would be a right angle if you put both vectors together.
		* If the result is bigger than 0 there would be an acute angle if you put both vectors together.
		* If the result is smaller than 0 there would be an obtuse angle if you put both vectors together.
		*/
		public method multiply takes thistype vector returns real
			return ((this.m_x * vector.m_x) + (this.m_y * vector.m_y) + (this.m_z * vector.m_z))
		endmethod

		/**
		* Similar to \ref AVector3.multiply.
		*/
		public method scalarProduct takes thistype vector returns real
			return this.multiply(vector)
		endmethod

		/**
		* Adds scaled vector \p vector which is scaled by value \p factor.
		* Note that vector \p vector won't be changed!
		* @author peq
		*/
		public method addScaled takes thistype vector, real factor returns nothing
			set this.m_x = this.m_x + vector.m_x * factor
			set this.m_y = this.m_y + vector.m_y * factor
			set this.m_z = this.m_z + vector.m_z * factor
		endmethod

		/**
		* Projects vector on vector \p vector.
		* Doesn't change the length.
		*/
		public method project takes thistype vector returns nothing
			local real factor = this.length() / vector.length()
			set this.m_x = vector.m_x * factor
			set this.m_y = vector.m_y * factor
			set this.m_z = vector.m_z * factor
		endmethod

		/**
		* Rotates a vector through \p angle.
		* Only x and y will be changed. Rotation on z axis is not possible yet.
		*/
		public method rotate takes real angle returns nothing
			local real length = this.length()
			local real beta = (Asin(this.m_y / length) - (angle * bj_DEGTORAD))
			set this.m_x = length * Cos(beta)
			set this.m_y = length * Sin(beta)
		endmethod

		/**
		* Sets vector's values to terrain point with x value \p x and y value \p y.
		* Z value is calculated automatically by using function \ref GetTerrainZ.
		* @author Tamino Dauth
		*/
		public method terrainPoint takes real x, real y returns nothing
			set this.m_x = x
			set this.m_y = y
			set this.m_z = GetTerrainZ(x, y)
		endmethod

		/**
		* Gets the normal vector of the terrain at given coordinates. @param sampleRadius defines
		* how far apart the reference points will be, if they are further apart, the result will
		* be an impression of smoother terrain; normaly the value should be between 0 and 128.
		* @todo Not tested yet.
		*/
		public method terrainNormal takes real x, real y, real sampleRadius returns nothing
			local real array z
			local thistype vectorX = thistype.create.evaluate(0.0, 0.0, 0.0)
			local thistype vectorY = thistype.create.evaluate(0.0, 0.0, 0.0)
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
			call vectorX.add(vectorY)
			call thistype.destroy(vectorY)
			call this.copy(vectorX)
			call thistype.destroy(vectorX)
		endmethod

		public static method create takes real x, real y, real z returns thistype
			local thistype this = thistype.allocate()
			// dynamic members
			set this.m_x = x
			set this.m_y = y
			set this.m_z = z

			return this
		endmethod

		/**
		* @see AVector3.terrainNormal
		* @author Tamino Dauth
		*/
		public static method createOnTerrain takes real x, real y returns thistype
			local thistype this = thistype.allocate()
			call this.terrainPoint(x, y)
			return this
		endmethod

		/**
		* @see AVector3.terrainNormal
		* @author Tamino Dauth
		*/
		public static method createOnTerrainNormal takes real x, real y, real sampleRadius returns thistype
			local thistype this = thistype.allocate()
			call this.terrainNormal(x, y, sampleRadius)
			return this
		endmethod

		/**
		* \return Returns sum of the two vectors \p vector0 and \p vector1 in a new vector instance.
		*/
		public static method sum takes thistype vector0, thistype vector1 returns thistype
			local thistype result = thistype.create(vector0.m_x, vector0.m_y, vector0.m_z)
			call result.add(vector1)
			return result
		endmethod

		/**
		* @return Returns difference of the two vectors \p vector0 and \p vector1 in a new vector instance.
		*/
		public static method difference takes thistype vector0, thistype vector1 returns thistype
			local thistype result = thistype.create(vector0.m_x, vector0.m_y, vector0.m_z)
			call result.subtract(vector1)
			return result
		endmethod

		/**
		* @return Returns scalar product of the two vectors \p vector0 and \p vector1.
		* @author Tamino Dauth
		* \todo untested
		*/
		public static method multiplication takes thistype vector0, thistype vector1 returns real
			return vector0.multiply(vector1)
		endmethod

		/**
		* @see AVector3.multiplication
		*/
		public static method scalarProduct2 takes thistype vector0, thistype vector1 returns real
			return thistype.multiplication(vector0, vector1)
		endmethod

		/**
		* @return Returns projection of the two vectors \p vector0 and \p vector1 in a new vector instance.
		* @author Tamino Dauth
		* @todo untested
		*/
		public method projection takes thistype vector0, thistype vector1 returns thistype
			local thistype result = thistype.create(vector0.m_x, vector0.m_y, vector0.m_z)
			call result.project(vector1)
			return result
		endmethod
	endstruct

endlibrary