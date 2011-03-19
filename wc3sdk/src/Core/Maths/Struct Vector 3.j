library AStructCoreMathsVector3 requires optional ALibraryCoreDebugMisc, ALibraryCoreMathsHandle, ALibraryCoreMathsPoint

	/**
	* Provides access to three-dimensional vector.
	* Three-dimensional vectors are often used in 3d graphics to define positions or speed.
	* They can simplify calculations.
	* Mainly influenced by Anitarf's system (\link http://www.wc3c.net/showthread.php?t=87027).
	* \note This system has not been tested extensively!
	*/
	struct AVector3
		// dynamic members
		private real m_x
		private real m_y
		private real m_z

		//! runtextmacro A_STRUCT_DEBUG("\"AVector3\"")
		
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
			call this.scale(length / this.length())
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


		/// Subtracts all data from vector @param vector.
		/// Das Ergebnis ist ein Vektor, der von der Spitze des Vektors "vector" zur Spitze des Vektors "this" verläuft.
		/// Achtung: Der Vektor verläuft immer vom 2. Wert zum 1. Wert der Subtraktion.
		public method subtract takes thistype vector returns nothing
			set this.m_x = this.m_x - vector.m_x
			set this.m_y = this.m_y - vector.m_y
			set this.m_z = this.m_z - vector.m_z
		endmethod
		
		/**
		* Substracts scaled vector \p vector which is scaled by value \p factor.
		* Note that vector \p vector won't be changed!
		*/
		public method substractScaled takes thistype vector, real factor returns nothing
			set this.m_x = this.m_x - vector.m_x * factor
			set this.m_y = this.m_y - vector.m_y * factor
			set this.m_z = this.m_z - vector.m_z * factor
		endmethod

		/**
		* Multiplies each value from vector \p vector and returns the sum of all results.
		* Doesn't change any vector data!
		* If the result is 0 there would be a right angle if you put both vectors together.
		* If the result is bigger than 0 there would be an acute angle if you put both vectors together.
		* If the result is smaller than 0 there would be an obtuse angle if you put both vectors together.
		* \sa thistype.dot, thistype.multiplication
		*/
		public method multiply takes thistype vector returns real
			return ((this.m_x * vector.m_x) + (this.m_y * vector.m_y) + (this.m_z * vector.m_z))
		endmethod

		/**
		* Similar to \ref thistype.multiply.
		* \sa thistype.dotProduct
		*/
		public method dot takes thistype vector returns real
			return thistype.dotProduct(this, vector)
		endmethod

		/**
		* \sa thistype.crossProduct
		*/
		public method cross takes thistype vector returns thistype
			return thistype.crossProduct(this, vector)
		endmethod

		public method distance takes thistype vector returns real
			local thistype temp = thistype.createCopy(vector)
			local real result
			call temp.subtract(vector)
			set result = temp.length()
			call temp.destroy()
			return result
		endmethod
		
		/**
		* Normalizes given vector.
		* Normal of vector should be orthogonal.
		*/
		public method normalize takes nothing returns nothing
			local real length = this.length()
			debug if (length == 0) then
				debug call this.printMethodError("normalize", "0 vector.")
				debug return
			debug endif
			set this.m_x = this.m_x / length
			set this.m_y = this.m_y / length
			set this.m_z = this.m_z / length
		endmethod
		
		/**
		* Projects vector on vector \p vector.
		* Doesn't change the length.
		*/
		public method project takes thistype vector returns nothing
			local real factor = this.multiply(vector) / vector.length()
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
		* \return Result is returned in degrees.
		* \author Anitarf
		*/
		public method angle takes thistype vector returns real
			local real length = this.length() * vector.length()
			debug if length == 0 then
				debug call this.printMethodError("angle", "0 vector.")
				debug return 0.0
			debug endif
			return Acos(this.dot(vector) / length) * bj_RADTODEG
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
		* Gets the normal vector of the terrain at given coordinates. \p sampleRadius defines
		* how far apart the reference points will be, if they are further apart, the result will
		* be an impression of smoother terrain; normaly the value should be between 0 and 128.
		* \author Anitarf
		*/
		public method terrainNormal takes real x, real y, real sampleRadius returns nothing
			local real zx = GetTerrainZ(x - sampleRadius, y) - GetTerrainZ(x + sampleRadius, y)
			local real zy = GetTerrainZ(x, y - sampleRadius) - GetTerrainZ(x, y + sampleRadius)
			set sampleRadius= 2 * sampleRadius
			set this.m_x = zx * sampleRadius
			set this.m_y = zy * sampleRadius
			set this.m_z = Pow(sampleRadius, 2.0)
		endmethod

		public static method create takes real x, real y, real z returns thistype
			local thistype this = thistype.allocate()
			// dynamic members
			set this.m_x = x
			set this.m_y = y
			set this.m_z = z

			return this
		endmethod
		
		public static method createCopy takes thistype vector returns thistype
			local thistype this = thistype.allocate()
			call this.copy(vector)

			return this
		endmethod

		/**
		* @see thistype.terrainPoint
		* @author Tamino Dauth
		*/
		public static method createOnTerrain takes real x, real y returns thistype
			local thistype this = thistype.allocate()
			call this.terrainPoint(x, y)
			return this
		endmethod

		/**
		* @see thistype.terrainNormal
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
			local thistype result = thistype.createCopy(vector0)
			call result.add(vector1)
			return result
		endmethod

		/**
		* @return Returns difference of the two vectors \p vector0 and \p vector1 in a new vector instance.
		*/
		public static method difference takes thistype vector0, thistype vector1 returns thistype
			local thistype result = thistype.createCopy(vector0)
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

		public static method dotProduct takes thistype vector0, thistype vector1 returns real
			return thistype.multiplication(vector0, vector1)
		endmethod

		public static method crossProduct takes thistype vector0, thistype vector1 returns thistype
			return thistype.create(vector0.m_y * vector1.m_z - vector0.m_z * vector1.m_y, vector0.m_z * vector1.m_x - vector0.m_x * vector1.m_z, vector0.m_x * vector1.m_y - vector0.m_y * vector1.m_x)
		endmethod
		
		/**
		* @return Returns projection of the two vectors \p vector0 and \p vector1 in a new vector instance.
		* @author Tamino Dauth
		* @todo untested
		*/
		public static method projection takes thistype vector0, thistype vector1 returns thistype
			local thistype result = thistype.createCopy(vector0)
			call result.project(vector1)
			return result
		endmethod
	endstruct

endlibrary