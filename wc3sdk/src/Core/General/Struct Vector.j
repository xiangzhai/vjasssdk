library AStructCoreGeneralVector requires ALibraryCoreDebugMisc

	/// @author Tamino Dauth
	/// @source http://www.cplusplus.com/reference/stl/list/
	//! textmacro A_VECTOR takes STRUCTPREFIX, NAME, ELEMENTTYPE, NULLVALUE, MAXSIZE
		
		/// @todo Should be a part of @struct $NAME$, vJass bug.
		$STRUCTPREFIX$ function interface $NAME$UnaryPredicate takes $ELEMENTTYPE$ value returns boolean
	
		/// @todo Should be a part of @struct $NAME$, vJass bug.
		$STRUCTPREFIX$ function interface $NAME$BinaryPredicate takes $ELEMENTTYPE$ value0, $ELEMENTTYPE$ value1 returns boolean
		
		/// @todo Should be a part of @struct $NAME$, vJass bug.
		$STRUCTPREFIX$ function interface $NAME$UnaryFunction takes $ELEMENTTYPE$ element returns nothing //Rückgabewert wurde vorerst rausgenommen, bis ich weiß, was er bringt

		/// Generator.
		/// Allows filling some elements with the return value.
		/// @todo Should be a part of @struct $NAME$, vJass bug.
		$STRUCTPREFIX$ function interface $NAME$Generator takes nothing returns $ELEMENTTYPE$
	
		$STRUCTPREFIX$ struct $NAME$
			//members
			private $ELEMENTTYPE$ array m_element[$MAXSIZE$]
			private integer m_size
			//Quicksort statics
			private $ELEMENTTYPE$ m
			private integer j
			
			public method size takes nothing returns integer
				return this.m_size
			endmethod
		
			public method front takes nothing returns $ELEMENTTYPE$
				return this.m_element[0]
			endmethod
			
			public method back takes nothing returns $ELEMENTTYPE$
				return this.m_element[this.m_size - 1]
			endmethod
			
			public method backIndex takes nothing returns integer
				return this.m_size - 1
			endmethod
			
			public method at takes integer index returns $ELEMENTTYPE$
				debug if (index < 0 or index >= this.m_size) then
					debug call Print("Invalid index: " + I2S(index) + ".")
					debug return $NULLVALUE$
				debug endif
				return this.m_element[index]
			endmethod
			
			public method empty takes nothing returns boolean
				return this.m_size == 0
			endmethod
			
			public method resize takes integer size, $ELEMENTTYPE$ newContent returns nothing
				local integer i
				if (size < this.m_size) then
					set i = this.m_size
					loop
						exitwhen (i == size)
						call this.popBack()
						set i = i - 1
					endloop
				elseif (size > this.m_size) then
					set i = size
					loop
						exitwhen (i == this.m_size)
						call this.pushBack(newContent)
						set i = i + 1
					endloop
				//else
					//do nothing
				endif
			endmethod
			
			/// Assigns new content to the container, dropping all the elements contained in the container object before the call and replacing them by those specified by the parameters:
			public method assign takes integer number, $ELEMENTTYPE$ content returns nothing
				//push back number times with content content
				local integer i = 0
				call this.clear()
				loop
					exitwhen (i == number)
					call this.pushBack(content)
					set i = i + 1
				endloop
			endmethod
			
			/// Inserts a new element at the beginning of the list, right before its current first element. The content of this new element is initialized to @param value.
			/// This effectively increases the list size by one.
			public method pushFront takes $ELEMENTTYPE$ value returns nothing
				call this.insert(0, value)
			endmethod
			
			/// Removes the first element in the list container, effectively reducing the list size by one.
			/// This doesn't call the removed element's destructor!
			public method popFront takes nothing returns nothing
				call this.erase(0)
			endmethod
			
			/// Adds a new element at the end of the list, right after its current last 
			/// element. The content of this new element is initialized to @param value.
			/// This effectively increases the list size by one.
			public method pushBack takes $ELEMENTTYPE$ value returns nothing
				set this.m_element[this.m_size] = value
				set this.m_size = this.m_size + 1
			endmethod
			
			/// Removes the last element in the list container, effectively reducing the list size by one.
			/// This calls the removed element's destructor.
			public method popBack takes nothing returns nothing
				set this.m_element[this.m_size - 1] = $NULLVALUE$
				set this.m_size = this.m_size - 1
			endmethod
			
			/// The list container is extended by inserting new elements before the element at position @param position with value @param value.
			/// This effectively increases the container size by @param number.
			public method insertNumber takes integer position, integer number, $ELEMENTTYPE$ value returns nothing
				local integer i = this.m_size - 1
				local integer firstExitValue = position + number - 1
				local integer secondExitValue = position + number
				debug if (position < 0 or position >= this.m_size) then
					debug call Print("Wrong position: " + I2S(position) + ".")
					debug return
				debug elseif (number <= 0 or position + number > this.m_size) then
					debug call Print("Wrong number: " + I2S(number) + ".")
					debug return
				debug elseif (this.m_size + number > thistype.maxSize()) then
					debug call Print("Size would be too high: " + I2S(this.m_size + number) + ". Maximum size is: " + I2S(thistype.maxSize()) + ".")
					debug return
				debug endif
				loop
					exitwhen (i == firstExitValue)
					set this.m_element[i + number] = this.m_element[i]
					set i = i - 1
				endloop
				set i = position
				loop
					exitwhen (i == secondExitValue)
					set this.m_element[i] = value
					set i = i + 1
				endloop
				set this.m_size = this.m_size + number
			endmethod
			
			public method insert takes integer position, $ELEMENTTYPE$ value returns nothing
				call this.insertNumber(position, 1, value)
			endmethod
			
			/// Removes from the list @param number elements at position @param position.
			/// This effectively reduces the list size by @param number.
			public method eraseNumber takes integer position, integer number returns nothing
				local integer i = position + number
				debug if (position < 0 or position >= this.m_size) then
					debug call Print("Wrong position: " + I2S(position) + ".")
					debug return
				debug elseif (number <= 0 or position + number > this.m_size) then
					debug call Print("Wrong number: " + I2S(number) + ".")
					debug return
				debug endif
				loop
					exitwhen (i == this.m_size)
					set this.m_element[i - number] = this.m_element[i]
					set this.m_element[i] = $NULLVALUE$ //clear
					set i = i + 1
				endloop
				set this.m_size = this.m_size - number
			endmethod
			
			public method erase takes integer position returns nothing
				call this.eraseNumber(position, 1)
			endmethod
			
			/// Exchanges the content of the vector by the content of @param vector, which is another vector object containing elements of the same type. Sizes may differ.
			/// After the call to this member function, the elements in this container are those which were in @param vector before the call, and the elements of @param vector are those which were in this.
			public method swap takes thistype vector returns nothing
				local $ELEMENTTYPE$ tempValue
				local integer i = 0
				debug if (this == vector) then
					debug call Print("Same vector.")
					debug return
				debug endif
				loop
					exitwhen (i == this.m_size or i == vector.m_size)
					set tempValue = this.m_element[i]
					set this.m_element[i] = vector.m_element[i]
					set vector.m_element[i] = tempValue
					set i = i + 1
				endloop
			endmethod
			
			/// All the elements in the vector container are dropped: they are removed from the vector container, leaving it with a size of 0.
			public method clear takes nothing returns nothing
				local integer i = 0
				loop
					exitwhen (i == this.m_size)
					set this.m_element[i] = $NULLVALUE$
					set i = i + 1
				endloop
				set this.m_size = 0
			endmethod
			
			/// Moves @param vectorNumber elements from vector @param vector at position @param vectorPosition into the vector container at the specified position @param position, effectively inserting the specified elements into the container and removing them from @param vector.
			/// This increases the container size by the amount of elements inserted, and reduces the size of @param vector by the same amount.
			public method splice takes integer position, thistype vector, integer vectorPosition, integer vectorNumber returns nothing
				local integer i = vectorPosition
				local integer exitValue = vectorPosition + vectorNumber
				local integer secondIndex
				debug if (position < 0 or position >= this.m_size) then
					debug call Print("Wrong position: " + I2S(position) + ".")
					debug return
				debug elseif (this == vector) then
					debug call Print("Same vector.")
					debug return
				debug elseif (vectorPosition < 0 or vectorPosition >= vector.m_size) then
					debug call Print("Wrong vector position: " + I2S(vectorPosition) + ".")
					debug return
				debug elseif (vectorNumber <= 0 or vectorNumber + vectorPosition > vector.m_size) then
					debug call Print("Wrong vector number: " + I2S(vectorNumber) + ".")
					debug return
				debug endif
				loop
					exitwhen (i == exitValue)
					set secondIndex = i + position
					if (secondIndex >= this.m_size) then
						call this.pushBack(vector.m_element[i])
					else
						call this.insert(secondIndex, vector.m_element[i])
					endif
					set i = i + 1
				endloop
				call vector.eraseNumber(vectorPosition, vectorNumber)
			endmethod
			
			/// Removes from the list all the elements with the specific value @param value. This reduces the list size by the amount of elements removed.
			/// Unlike method @method erase, which erases elements by their position, this method removes elements by their value.
			/// A similar method, @method removeIf, exists, which allows for a condition other than a plain value comparison to be performed on each element in order to determine the elements to be removed.
			public method remove takes $ELEMENTTYPE$ value returns nothing
				//backwards should be faster
				local integer i = this.m_size - 1
				loop
					exitwhen (i < 0)
					if (this.m_element[i] == value) then
						call this.erase(i)
					endif
					set i = i - 1
				endloop
			endmethod
			
			/// Removes from the list all the elements for which predicate @param unaryPredicate returns true. This reduces the list size by the amount of elements removed.
			/// Predicate @param unaryPredicate can be implemented as a function taking one argument of the same type as the list and returning a @type boolean.
			/// The function calls unaryPredicate.evaluate(x) for each element (where i is the element). Any of the elements in the list for which this returns true, is removed from the container.
			public method removeIf takes $NAME$UnaryPredicate unaryPredicate returns nothing
				//backwards should be faster
				local integer i = this.m_size - 1
				debug if (unaryPredicate == 0) then
					debug call Print("Unary predicate is 0.")
					debug return
				debug endif
				loop
					exitwhen (i < 0)
					if (unaryPredicate.evaluate(this.m_element[i])) then
						call this.erase(i)
					endif
					set i = i - 1
				endloop
			endmethod
			
			/// The first version, with no parameters, removes all but the first element from every consecutive group of equal elements in the vector container.
			/// Notice that an element is only removed from the vector if it is equal to the element immediately preceding it. Thus, this function is specially useful for sorted lists.
			public method unique takes nothing returns nothing
				//backwards should be faster
				local integer i = this.m_size - 2
				loop
					exitwhen (i < 0)
					if (this.m_element[i] == this.m_element[i + 1]) then
						call this.erase(i + 1) //remove rear value
					endif
					set i = i - 1
				endloop
			endmethod
			
			/// For the second version, accepting a binary predicate, a specific comparison function to determine the "uniqueness" of an element can be specified. In fact, any behavior can be implemented (and not only a plain comparison), but notice that the function will call binaryPredicate.evaluate(x, x - 1)) for all pairs of elements (where x is an element) and remove x from the list if the predicate returns true.
			public method uniqueIf takes $NAME$BinaryPredicate binaryPredicate returns nothing
				//backwards should be faster
				local integer i = this.m_size - 2
				debug if (binaryPredicate == 0) then
					debug call Print("Binary predicate is 0.")
					debug return
				debug endif
				loop
					exitwhen (i < 0)
					if (binaryPredicate.evaluate(this.m_element[i + 1], this.m_element[i])) then
						call this.erase(i + 1) //remove rear value
					endif
					set i = i - 1
				endloop
			endmethod
			
			/// Merges @param vector into the vector, inserting all the elements of @param vector into the vector object at their respective ordered positions. This empties @param vector and increases the vector size.
			/// @todo insert sorted?!
			public method merge takes thistype vector returns nothing
				local integer i = 0
				call this.insertNumber(0, vector.m_size, $NULLVALUE$)
				loop
					exitwhen (i == vector.m_size)
					set this.m_element[i] = vector.m_element[i]
					set i = i + 1
				endloop
				call vector.clear()
			endmethod
			
			/// The second version (template function), has the same behavior, but takes a specific function to perform the comparison operation in charge of determining the insertion points. The comparison function has to perform weak strict ordering (which basically means the comparison operation has to be transitive and irreflexive).
			/// The merging is performed using two iterators: one to iterate through x and another one to keep the insertion point in the vector object; During the iteration of x, if the current element in x compares less than the element at the current insertion point in the list object, the element is removed from x and inserted into that location, otherwise the insertion point is advanced. This operation is repeated until either end is reached, in which moment the remaining elements of x (if any) are moved to the end of the list object and the function returns (this last operation is performed in constant time).
			/// @todo implement mergeIf pls.
			/// template <class Compare>
			/// void merge ( list<T,Allocator>& x, Compare comp );
			
			/// Common quick sort algorithm.
			private method quickSort takes integer left, integer right, $NAME$BinaryPredicate binaryPredicate returns nothing
				local integer i
				local $ELEMENTTYPE$ temp
				if (right > left) then
					set this.m = this.m_element[right]
					set i = left - 1
					set this.j = right
					loop
						loop
							set i = i + 1
							exitwhen (not binaryPredicate.evaluate(this.m_element[i], this.m))
						endloop
						
						loop
							set this.j = this.j - 1
							exitwhen (binaryPredicate.evaluate(this.m_element[this.j], this.m))
						endloop
		
						exitwhen (i >= this.j)
						set temp = this.m_element[i]
						set this.m_element[i] = this.m_element[this.j]
						set this.m_element[this.j] = temp
					endloop
					
					set temp = this.m_element[i]
					set this.m_element[i] = this.m_element[right]
					set this.m_element[right] = temp
					
					call this.quickSort(left, i - 1, binaryPredicate)
					call this.quickSort(i + 1, right, binaryPredicate)
				endif
			endmethod

			/// Sorts the elements in the container from lower to higher. The sorting is performed by comparing the elements in the container in pairs using a sorting algorithm.
			/// The comparisons are perfomed using function @param binaryPredicate, which performs weak strict ordering (this basically means the comparison operation has to be transitive and irreflexive).
			public method sortNumber takes integer position, integer number, $NAME$BinaryPredicate binaryPredicate returns nothing
				debug if (not this.debugCheckPositionAndNumber(position, number)) then
					debug return
				debug endif
				call this.quickSort(position, position + number - 1, binaryPredicate)
			endmethod
			
			public method sort takes $NAME$BinaryPredicate binaryPredicate returns nothing
				call this.sortNumber(0, this.m_size, binaryPredicate)
			endmethod

			/// Reverses the order of the elements in the list container.
			/// @todo I'm not sure if this is best solution.
			public method reverseNumber takes integer position, integer number returns nothing
				local integer i = position
				local integer exitValue = position + (number / 2)
				local $ELEMENTTYPE$ temp
				local integer swapindex
				debug if (not this.debugCheckPositionAndNumber(position, number)) then
					debug return
				debug endif
				loop
					exitwhen (i == exitValue)
					set swapindex = ((2 * position) + number - i - 1)
					set temp = this.m_element[i]
					set this.m_element[i] = this.m_element[swapindex]
					set this.m_element[swapindex] = temp
					set i = i + 1
				endloop
			endmethod
			
			public method reverse takes nothing returns nothing
				call this.reverseNumber(0, this.m_size)
			endmethod
			
			/// @todo Implement the following methods:
			/// adjacentFind
			/// adjacentFindIf
			/// binarySearch
			/// copy
			/// copyBackward
			/// copyN
			/// countIf
			/// equal
			/// equalIf
			/// fill
			/// fillN
			
			public method findNumber takes integer position, integer number, $ELEMENTTYPE$ value returns integer
				local integer i = position
				local integer exitValue = position + number
				debug call this.debugCheckPositionAndNumber(position, number)
				loop
					exitwhen (i == exitValue)
					if (this.m_element[i] == value) then
						return i
					endif
					set i = i + 1
				endloop
				return -1
			endmethod
			
			public method find takes $ELEMENTTYPE$ value returns integer
				return this.findNumber(0, this.m_size, value)
			endmethod
			
			public method findEndNumber takes integer position, integer number, $ELEMENTTYPE$ value returns integer
				local integer i = position + number - 1
				local integer exitValue = position - 1
				debug call this.debugCheckPositionAndNumber(position, number)
				loop
					exitwhen (i == exitValue)
					if (this.m_element[i] == value) then
						return i
					endif
					set i = i - 1
				endloop
				return -1
			endmethod
			
			public method findEnd takes $ELEMENTTYPE$ value returns integer
				return this.findEndNumber(0, this.m_size, value)
			endmethod

			/// findFirstOf
			
			public method findIfNumber takes integer position, integer number, $NAME$UnaryPredicate unaryPredicate returns integer
				local integer i = position
				local integer exitValue = position + number
				debug call this.debugCheckPositionAndNumber(position, number)
				loop
					exitwhen (i == exitValue)
					if (unaryPredicate.evaluate(this.m_element[i])) then
						return i
					endif
					set i = i + 1
				endloop
				return -1
			endmethod
			
			public method findIf takes $NAME$UnaryPredicate unaryPredicate returns integer
				return this.findIfNumber(0, this.m_size, unaryPredicate)
			endmethod
			
			public method contains takes $ELEMENTTYPE$ value returns boolean
				return this.find(value) != -1
			endmethod

			/// Calls user-defined function @param unaryFunction for each element in range beginning at position @param position and ending at position @param position + @param number - 1.
			/// Function @param unaryFunction is called like this: call unaryFunction.execute(x) where x is the current element of iteration.
			public method forEachNumber takes integer position, integer number, $NAME$UnaryFunction unaryFunction returns nothing
				local integer i = position
				local integer exitValue = position + number
				debug if (not this.debugCheckPositionAndNumber(position, number)) then
					debug return
				debug endif
				loop
					exitwhen (i == exitValue)
						call unaryFunction.execute(this.m_element[i])
					set i = i + 1
				endloop
			endmethod
			
			public method forEach takes $NAME$UnaryFunction unaryFunction returns nothing
				call this.forEachNumber(0, this.m_size, unaryFunction)
			endmethod
			
			/// Füllt einen Elementebereich mit dem Rückgabewert einer benutzerdefinierten Funktion.
			public method generateNumber takes integer position, integer number, $NAME$Generator generator returns nothing
				local integer i = position
				local integer exitValue = position + number
				debug if (not this.debugCheckPositionAndNumber(position, number)) then
					debug return
				debug endif
				loop
					exitwhen (i == exitValue)
						set this.m_element[i] = generator.evaluate()
					set i = i + 1
				endloop
			endmethod
			
			public method generate takes $NAME$Generator generator returns nothing
				call this.generateNumber(0, this.m_size, generator)
			endmethod
			
			/// includes
			/// innerProduct
			/// inplaceMerge
			/// maxElement
			/// minElement
			
			public method operator[] takes integer index returns $ELEMENTTYPE$
				debug if (index < 0 or index >= this.m_size) then
					debug call Print("Invalid index: " + I2S(index) + ".")
					debug return $NULLVALUE$
				debug endif
				return this.m_element[index]
			endmethod

			public method operator[]= takes integer index, $ELEMENTTYPE$ value returns nothing
				debug if (index < 0 or index >= this.m_size) then
					debug call Print("Invalid index: " + I2S(index) + ".")
					debug return
				debug endif
				set this.m_element[index] = value
			endmethod

			public method operator< takes thistype vector returns boolean
				debug if (this == vector) then
					debug call Print("Same vector.")
				debug endif
				return this.m_size < vector.m_size
			endmethod
			
			debug private method debugCheckPositionAndNumber takes integer position, integer number returns boolean
				debug if (position < 0 or position >= this.m_size) then
					debug call Print("Wrong position: " + I2S(position) + ".")
					debug return false
				debug elseif (number <= 0 or position + number > this.m_size) then
					debug call Print("Wrong number: " + I2S(number) + ".")
					debug return false
				debug endif
				debug return true
			debug endmethod
			
			public static method create takes nothing returns thistype
				local thistype this = thistype.allocate()
				//members
				set this.m_size = 0
				return this
			endmethod
			
			public static method createWithSize takes integer size, $ELEMENTTYPE$ content returns thistype
				local thistype this = thistype.allocate()
				call this.resize(size, content)
				return this
			endmethod
			
			/// Creates a vector by filling it with elements of vector @param other.
			public static method createByOther takes thistype other returns thistype
				local thistype this = thistype.allocate()
				local integer i = 0
				loop
					exitwhen (i == other.m_size)
					call this.pushBack(other.m_element[i])
					set i = i + 1
				endloop
				return this
			endmethod
			
			
			/// Vector will be cleared before destruction.
			public method onDestroy takes nothing returns nothing
				call this.clear()
			endmethod
			
			public static constant method maxSize takes nothing returns integer
				return $MAXSIZE$
			endmethod
		endstruct
	//! endtextmacro
	
	///default vectors, Jass data types
	//! runtextmacro A_VECTOR("", "AIntegerVector", "integer", "0", "8192") /// @todo JASS_MAX_ARRAY_SIZE
	//! runtextmacro A_VECTOR("", "AStringVector", "string", "null", "8192") /// @todo JASS_MAX_ARRAY_SIZE
	///! runtextmacro A_VECTOR("", "ABooleanVector", "boolean", "false", "8192") /// @todo JASS_MAX_ARRAY_SIZE
	//! runtextmacro A_VECTOR("", "ARealVector", "real", "0.0", "8192") /// @todo JASS_MAX_ARRAY_SIZE
	
	//! runtextmacro A_VECTOR("", "AEffectVector", "effect", "null", "8192") /// @todo JASS_MAX_ARRAY_SIZE
	//! runtextmacro A_VECTOR("", "AUnitVector", "unit", "null", "8192") /// @todo JASS_MAX_ARRAY_SIZE
	
	
	///default vectors, ASL data types
	//! runtextmacro A_VECTOR("", "ADamageRecorderVector", "ADamageRecorder", "0", "8192") /// @todo JASS_MAX_ARRAY_SIZE
endlibrary