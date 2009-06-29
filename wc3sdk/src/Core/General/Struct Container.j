library AStructCoreGeneralContainer requires ALibraryCoreDebugMisc

	/// Creates a new AContainer class with type @param ELEMENTTYPE, type name @param ELEMENTTYPENAME and maximum size @param MAXSIZE.
	/// Containers can hold several members of one specified data type like @type unit or @type integer.
	/// Attention: This text macro will be replaced by @textmacro A_VECTOR. Use @textmacro A_VECTOR instead of this!
	//! textmacro A_CONTAINER takes STRUCTPREFIX, NAME, ELEMENTTYPE, NULLVALUE, MAXSIZE

		/// @state untested
		/// An operation function interface with two elements.
		/// The return value is the result of the operation.
		/// Examples are addition and subtraction etc.
		$STRUCTPREFIX$ function interface $NAME$Operation takes $ELEMENTTYPE$ element0, $ELEMENTTYPE$ element1 returns $ELEMENTTYPE$

		/// @state untested
		/// Unary predicate.
		/// A comparision function interface for one element.
		$STRUCTPREFIX$ function interface $NAME$UnaryPredicate takes $ELEMENTTYPE$ element returns boolean

		/// @state untested
		/// Binary predicate.
		/// A comparisan function interface for two elements.
		$STRUCTPREFIX$ function interface $NAME$BinaryPredicate takes $ELEMENTTYPE$ element0, $ELEMENTTYPE$ element1 returns boolean

		/// @state works
		/// Unary function.
		/// Allows the call of an user-definied function for each element.
		$STRUCTPREFIX$ function interface $NAME$UnaryFunction takes $ELEMENTTYPE$ element returns nothing //Rückgabewert wurde vorerst rausgenommen, bis ich weiß, was er bringt

		/// Generator.
		/// Allows filling some elements with the return value.
		$STRUCTPREFIX$ function interface $NAME$Generator takes nothing returns $ELEMENTTYPE$

		$STRUCTPREFIX$ struct $NAME$
			//dynamic members
			private $ELEMENTTYPE$ array element[$MAXSIZE$]
			//Quicksort statics
			private $ELEMENTTYPE$ m
			private integer j

			//dynamic members

			public method setElement takes integer index, $ELEMENTTYPE$ element returns nothing
				debug call this.debugCheckIndex(index)
				set this.element[index] = element
			endmethod

			public method getElement takes integer index returns $ELEMENTTYPE$
				debug call this.debugCheckIndex(index)
				return this.element[index]
			endmethod

			//start members

			public method getSize takes nothing returns integer
				return this.element.size
			endmethod

			//methods

			/// Checks the start position.
			debug private method debugCheckStart takes integer start returns nothing
				debug if ((start < 0) or (start >= $MAXSIZE$)) then
					debug call Print("Wrong start value of container " + I2S(this) + ": start " + I2S(start) + ".")
				debug endif
			debug endmethod

			/// Checks the number.
			debug private method debugCheckNumber takes integer number returns nothing
				debug if ((number > $MAXSIZE$) or (number <= 0)) then
					debug call Print("Wrong number value of container " + I2S(this) + ": number " + I2S(number) + ".")
				debug endif
			debug endmethod

			/// Checks both values.
			debug private method debugCheckStartAndNumber takes integer start, integer number returns nothing
				debug call this.debugCheckStart(start)
				debug call this.debugCheckNumber(number)
				debug if (start + number > $MAXSIZE$) then
					debug call Print("Wrong start or number value of container " + I2S(this) + ": start " + I2S(start) + " number " + I2S(number) + ".")
				debug endif
			debug endmethod

			/// Checks over- and underflow of the size.
			debug private method debugCheckSize takes integer size returns nothing
				debug if (size > $MAXSIZE$) then
					debug call Print("Wrong size of container " + I2S(this) + ": "+ I2S(size) + ".")
				debug endif
			debug endmethod
	
			/// Checks over- and underflow of the index.
			debug private method debugCheckIndex takes integer index returns nothing
				debug if ((index < 0) or (index >= $MAXSIZE$)) then
					debug call Print("Wrong index of container " + I2S(this) + ": "+ I2S(index) + ".")
				debug endif
			debug endmethod

			/// Common quick sort algorithm.
			private method quickSort takes integer left, integer right, $NAME$BinaryPredicate binaryPredicate returns nothing
				local integer i
				local $ELEMENTTYPE$ temp
				if (right > left) then
					set this.m = this.element[right]
					set i = left - 1
					set this.j = right
					loop
						loop
							set i = i + 1
							exitwhen (not binaryPredicate.evaluate(this.element[i], this.m))
						endloop
						
						loop
							set this.j = this.j - 1
							exitwhen (binaryPredicate.evaluate(this.element[this.j], this.m))
						endloop
		
						exitwhen (i >= this.j)
						set temp = this.element[i]
						set this.element[i] = this.element[this.j]
						set this.element[this.j] = temp
					endloop
					
					set temp = this.element[i]
					set this.element[i] = this.element[right]
					set this.element[right] = temp
					
					call this.quickSort(left, i - 1, binaryPredicate)
					call this.quickSort(i + 1, right, binaryPredicate)
				endif
			endmethod

			/// Sorts elements by using the quick sort algorithm in ascending order.
			/// @param start Index of first element.
			/// @param number Number of elements.
			/// @param binaryPredicate Binary comparison function.
			public method sort takes integer start, integer number, $NAME$BinaryPredicate binaryPredicate returns nothing
				debug call this.debugCheckStartAndNumber(start, number)
				call this.quickSort(start, start + number - 1, binaryPredicate)
			endmethod

			/// Reverses elements.
			/// @param start Index of first element.
			/// @param number Number of elements.
			/// @todo I'm not sure if this is best solution.
			public method reverse takes integer start, integer number returns nothing
				local integer i = start
				local integer exitValue = start + (number / 2)
				local $ELEMENTTYPE$ temp
				local integer swapindex
				debug call this.debugCheckStartAndNumber(start, number)
				loop
					exitwhen (i == exitValue)
					set swapindex = ((2 * start) + number - i - 1)
					set temp = this.element[i]
					set this.element[i] = this.element[swapindex]
					set this.element[swapindex] = temp
					set i = i + 1
				endloop
			endmethod

			//C++ algorithm

			/// @state untested
			/// Orientiert sich an accumulate und adjacent_difference.
			/// Man kann allerdings nicht jeden Datentyp addieren oder subtrahieren.
			/// Führt eine Operation mit allen Elementen in einem Bereich durch.
			/// @param start Startposition
			/// @param number Anzahl der Elemente
			/// @param value Der Startwert des Ergebnis
			/// @param operation Operationsfunktion
			/// @return Ergebnis aller Operationen
			public method operation takes integer start, integer number, $ELEMENTTYPE$ value, $NAME$Operation operation returns $ELEMENTTYPE$
				local integer i = start
				local integer exitValue = start + number
				local $ELEMENTTYPE$ result = value
				debug call this.debugCheckStartAndNumber(start, number)
				loop
					exitwhen (i == exitValue)
					set result = operation.evaluate(result, this.element[i]) //Returns something
					set i = i + 1
				endloop
				return result
			endmethod

			/// @state untested
			/// Sucht nach zwei aneinander liegenden gleichen Elementen.
			/// @param start Startposition
			/// @param number Anzahl der Elemente
			/// @return Index des ersten Treffers oder -1
			public method adjacentFind takes integer start, integer number returns integer
				local integer i = start
				local integer exitValue = start + number - 1 //-1 da beim Vergleich + 1 angewendet wird
				debug call this.debugCheckStartAndNumber(start, number)
				loop
					exitwhen (i == exitValue)
					if (this.element[i] == this.element[i + 1]) then
						return i
					endif
					set i = i + 1
				endloop
				return -1
			endmethod

			/// @state untested
			/// Sucht mit Hilfe einer benutzerdefinierten Vergleichsfunktion nach zwei aneinander liegenden gleichen Elementen.
			/// @param start Startposition
			/// @param number Anzahl der Elemente
			/// @param binaryPredicate Vergleichsfunktion
			/// @return Index des ersten Treffers oder -1
			public method adjacentFindIf takes integer start, integer number, $NAME$BinaryPredicate binaryPredicate returns integer
				local integer i = start
				local integer exitValue = start + number - 1 //-1 da beim Vergleich + 1 angewendet wird
				debug call this.debugCheckStartAndNumber(start, number)
				loop
					exitwhen (i == exitValue)
					if (binaryPredicate.evaluate(this.element[i], this.element[i + 1])) then
						return i
					endif
					set i = i + 1
				endloop
				return -1
			endmethod

			/// @state untested
			/// Die binäre Suche funktioniert nur wenn die Elemente in Reihenfolge sortiert sind.
			/// @todo Noch implementieren.
			/// @todo bool binary_search( iterator start, iterator end, const TYPE& val );
			/// @todo bool binary_search( iterator start, iterator end, const TYPE& val, Comp f );
			public method binarySearch takes nothing returns nothing
			endmethod

			/// @state untested
			/// Kopiert einen Elementebereich des Containers in einen Ziel-Container.
			/// @param start0 Index of first element.
			/// @param number0 Number of elements.
			/// @param start1 Index of first element in container @param desitination.
			/// @param destination Container where elements are copied to.
			public method copy takes integer start0, integer number0, integer start1, $NAME$ destination returns nothing
				local integer i = start0
				local integer j = start1
				local integer exitValue = start0 + number0
				debug call this.debugCheckStartAndNumber(start0, number0)
				debug call destination.debugCheckStart(start1)
				loop
					exitwhen (i == exitValue)
					set destination.element[j] = this.element[i]
					set i = i + 1
					set j = j + 1
				endloop
			endmethod

			/// @state untested
			/// Kopiert einen Elementebereich des Containers in einen Ziel-Container.
			/// Im Gegensatz zur copy-Methode fängt diese Methode beim letzten zu kopierenden Wert an.
			/// @param start0 Startposition
			/// @param number0 Anzahl der Elemente
			/// @param start1 Startposition im Ziel-Container
			/// @param destination Ziel-Container
			public method copyBackward takes integer start0, integer number0, integer start1, $NAME$ destination returns nothing
				local integer i = start0 + number0 - 1
				local integer j = start1 + number0 - 1
				local integer exitValue = start0 - 1
				debug call this.debugCheckStartAndNumber(start0, number0)
				debug call destination.debugCheckStart(start1)
				loop
					exitwhen (i == exitValue)
					set destination.element[j] = this.element[i]
					set i = i - 1
					set j = j - 1
				endloop
			endmethod

			/// Diese Methode tut das, was die Methoden dieses Containers von C++-Containern unterscheidet.
			/// Sie verwendet als zweiten Parameter die Anzahl der Elemente und keinen Ziel-Iterator.
			/// Sie ist daher überflüssig.
			/// iterator copy_n( iterator from, size_t num, iterator to );
			public method copyN takes nothing returns nothing
			endmethod

			/// @state untested
			/// Vergleicht einen Elementebereich mit einem Wert.
			/// @param start Startposition
			/// @param number Anzahl der Elemente
			/// @param value Vergleichswert
			/// @return Anzahl der Treffer
			public method count takes integer start, integer number, $ELEMENTTYPE$ value returns integer
				local integer i = start
				local integer exitValue = start + number
				local integer result = 0
				debug call this.debugCheckStartAndNumber(start, number)
				loop
					exitwhen (i == exitValue)
					if (this.element[i] == value) then
						set result = result + 1
					endif
					set i = i + 1
				endloop
				return result
			endmethod

			/// @state untested
			/// Vergleicht einen Elementebereich mit Hilfe einer benutzerdefinierten Funktion.
			/// @param start Startposition
			/// @param number Anzahl der Elemente
			/// @param unaryPredicate Vergleichsfunktion
			/// @return Anzahl der Treffer
			public method countIf takes integer start, integer number, $NAME$UnaryPredicate unaryPredicate returns integer
				local integer i = start
				local integer exitValue = start + number
				local integer result = 0
				debug call this.debugCheckStartAndNumber(start, number)
				loop
					exitwhen (i == exitValue)
					if (unaryPredicate.evaluate(this.element[i])) then
						set result = result + 1
					endif
					set i = i + 1
				endloop
				return result
			endmethod

			/// @state untested
			/// Prüft ob ein Elementebereich einem anderen entspricht.
			/// Verwendet dabei den Standard ==-Operator, der normalerweise bei allen Datentypen anwendbar ist.
			/// @param start1 Startposition
			/// @param number1 Anzahl der Elemente
			/// @param start2 Startposition im Vergleichs-Container
			/// @param destination Vergleichs-Container
			/// @return Übereinstimmung aller Elemente
			public method equal takes integer start1, integer number1, integer start2, $NAME$ destination returns boolean
				local integer i = start1
				local integer j = start2
				local integer exitValue = start1 + number1
				debug call this.debugCheckStartAndNumber(start1, number1)
				debug call destination.debugCheckStart(start2)
				loop
					exitwhen (i == exitValue)
						if (not (this.element[i] == destination.element[j])) then
							return false
						endif
					set i = i + 1
					set j = j + 1
				endloop
				return true
			endmethod

			/// @state untested
			/// Prüft mit Hilfe einer benutzerdefinierten Vergleichsfunktion ob ein Elementebereich einem anderen entspricht.
			/// @param start0 Startposition
			/// @param number0 Anzahl der Elemente
			/// @param start1 Startposition im Vergleichs-Container
			/// @param destination Vergleichs-Container
			/// @param binaryPredicate Vergleichsfunktion
			/// @return Übereinstimmung aller Elemente
			public method equalIf takes integer start0, integer number0, integer start1, $NAME$ destination, $NAME$BinaryPredicate binaryPredicate returns boolean
				local integer i = start0
				local integer j = start1
				local integer exitValue = start0 + number0
				debug call this.debugCheckStartAndNumber(start0, number0)
				debug call destination.debugCheckStart(start1)
				loop
					exitwhen (i == exitValue)
						if (not binaryPredicate.evaluate(this.element[i], destination.element[j])) then
							return false
						endif
					set i = i + 1
					set j = j + 1
				endloop
				return true
			endmethod

			/// @state untested
			/// Füllt einen Elementebereich mit einem Wert.
			/// @param start Startposition
			/// @param number Anzahl der Elemente
			/// @param value Zu füllender Wert
			public method fill takes integer start, integer number, $ELEMENTTYPE$ value returns nothing
				local integer i = start
				local integer exitValue = start + number
				debug call this.debugCheckStartAndNumber(start, number)
				loop
					exitwhen (i == exitValue)
						set this.element[i] = value
					set i = i + 1
				endloop
			endmethod

			/// Diese Methode tut das, was die Methoden dieses Containers von C++-Containern unterscheidet.
			/// Sie verwendet als zweiten Parameter die Anzahl der Elemente und keinen Ziel-Iterator.
			/// Sie ist daher überflüssig.
			/// iterator fill_n( iterator start, size_t n, const TYPE& val );
			public method fillN takes nothing returns nothing
			endmethod

			/// @state untested
			/// Sucht im Container nach einem Element mit einem bestimmten Wert.
			/// @param start Index of first element which is checked for the searched value.
			/// @param number Number of elements which are checked for the searched value.
			/// @param value Searched value.
			/// @return Returns the index of the first hit. If there was no hit it returns -1.
			public method find takes integer start, integer number, $ELEMENTTYPE$ value returns integer
				local integer i = start
				local integer exitValue = start + number
				debug call this.debugCheckStartAndNumber(start, number)
				loop
					exitwhen (i == exitValue)
					if (this.element[i] == value) then
						return i
					endif
					set i = i + 1
				endloop
				return -1
			endmethod

			/// @todo iterator find_end( iterator start, iterator end, iterator seq_start, iterator seq_end );
			/// @todo iterator find_end( iterator start, iterator end, iterator seq_start, iterator seq_end, BinPred bp );
			public method findEnd takes nothing returns nothing
			endmethod

			/// @todo iterator find_first_of( iterator start, iterator end, iterator find_start, iterator find_end );
			/// @todo iterator find_first_of( iterator start, iterator end, iterator find_start, iterator find_end, BinPred bp );
			public method findFirstOf takes nothing returns nothing
			endmethod

			/// @state untested
			/// Sucht mit Hilfe einer benutzerdefinierten Funktion ein Element im Container.
			/// @param start Startposition
			/// @param number Anzahl der Elemente
			/// @param unaryPredicate Vergleichsfunktion
			/// @return Index des ersten Treffers oder -1
			public method findIf takes integer start, integer number, $NAME$UnaryPredicate unaryPredicate returns integer
				local integer i = start
				local integer exitValue = start + number
				debug call this.debugCheckStartAndNumber(start, number)
				loop
					exitwhen (i == exitValue)
						if (unaryPredicate.evaluate(this.element[i])) then
							return i
						endif
					set i = i + 1
				endloop
				return -1
			endmethod

			/// @state untested
			/// Führt eine benutzerdefinierte Funktion für einen Elementebereich aus.
			/// @param start Startposition
			/// @param number Anzahl der Elemente
			/// @param unaryFunction Benutzerdefinierte Funktion
			public method forEach takes integer start, integer number, $NAME$UnaryFunction unaryFunction returns nothing
				local integer i = start
				local integer exitValue = start + number
				debug call this.debugCheckStartAndNumber(start, number)
				loop
					exitwhen (i == exitValue)
						call unaryFunction.execute(this.element[i])
					set i = i + 1
				endloop
			endmethod

			/// void generate( iterator start, iterator end, Generator g );
			/// @state untested
			/// Füllt einen Elementebereich mit dem Rückgabewert einer benutzerdefinierten Funktion.
			/// Funktion wurde so in Quell-Code des GCCs nachgeschlagen.
			/// @param start Startposition
			/// @param number Anzahl der Elemente
			/// @param generator Benutzerdefinierte Funktion
			public method generate takes integer start, integer number, $NAME$Generator generator returns nothing
				local integer i = start
				local integer exitValue = start + number
				debug call this.debugCheckStartAndNumber(start, number)
				loop
					exitwhen (i == exitValue)
						set this.element[i] = generator.evaluate()
					set i = i + 1
				endloop
			endmethod

			/// Diese Methode tut das, was die Methoden dieses Containers von C++-Containern unterscheidet.
			/// Sie verwendet als zweiten Parameter die Anzahl der Elemente und keinen Ziel-Iterator.
			/// Sie ist daher überflüssig.
			/// iterator generate_n( iterator result, size_t num, Generator g );
			public method generateN takes nothing returns nothing
			endmethod

			/// @todo bool includes( iterator start1, iterator end1, iterator start2, iterator end2 );
			/// @todo bool includes( iterator start1, iterator end1, iterator start2, iterator end2, StrictWeakOrdering cmp );
			/// @todo The includes() algorithm returns true if every element in [start2,end2) is also in [start1,end1). Both of the given ranges must be sorted in ascending order.
			/// @todo By default, the < operator is used to compare elements. If the strict weak ordering function object cmp is given, then it is used instead.
			public method includes takes nothing returns nothing
			endmethod

			/// @todo TYPE inner_product( iterator start1, iterator end1, iterator start2, TYPE val );
			/// @todo TYPE inner_product( iterator start1, iterator end1, iterator start2, TYPE val, BinaryFunction f1, BinaryFunction f2 );
			/// @todo The inner_product() function computes the inner product of [start1,end1) and a range of the same size starting at start2.
			public method innerProduct takes nothing returns nothing
			endmethod

			/// @todo inline void inplace_merge( iterator start, iterator middle, iterator end );
			/// @todo inline void inplace_merge( iterator start, iterator middle, iterator end, StrictWeakOrdering cmp );
			/// @todo The inplace_merge() function is similar to the merge() function, but instead of creating a new sorted range of elements, inplace_merge() alters the existing ranges to perform the merge in-place.
			public method inplaceMerge takes nothing returns nothing
			endmethod

			/// @state untested
			/// iterator max_element( iterator start, iterator end );
			/// iterator max_element( iterator start, iterator end, BinPred p );
			/// Sucht mit Hilfe einer benutzerdefinierten Vergleichsfunktion nach dem größten Element in einem Elementebereich.
			/// Das Prädikat stellt in diesem Fall den <-Operator dar.
			/// @param start Startposition
			/// @param number Anzahl der Elemente
			/// @param binaryPredicate Benutzerdefinierte Vergleichsfunktion
			/// @return Index des größten Elements
			public method maxElement takes integer start, integer number, $NAME$BinaryPredicate binaryPredicate returns $ELEMENTTYPE$
				local integer i = start + 1
				local integer exitValue = start + number
				local integer result = start
				debug call this.debugCheckStartAndNumber(start, number)
				loop
					exitwhen (i == exitValue)
						//x < y
						if (binaryPredicate.evaluate(this.element[result], this.element[i])) then
							set result = i
						endif
					set i = i + 1
				endloop
				return result
			endmethod

			//iterator min_element( iterator start, iterator end );
			//iterator min_element( iterator start, iterator end, BinPred p );
			/// Sucht mit Hilfe einer benutzerdefinierten Vergleichsfunktion nach dem kleinsten Element in einem Elementebereich.
			/// Das Prädikat stellt in diesem Fall den <-Operator dar.
			/// @param start Startposition
			/// @param number Anzahl der Elemente
			/// @param binaryPredicate Benutzerdefinierte Vergleichsfunktion
			/// @return Index des kleinsten Elements
			public method minElement takes integer start, integer number, $NAME$BinaryPredicate binaryPredicate returns $ELEMENTTYPE$
				local integer i = start + 1
				local integer exitValue = start + number
				local integer result = start
				debug call this.debugCheckStartAndNumber(start, number)
				loop
					exitwhen (i == exitValue)
						//x < y
						if (binaryPredicate.evaluate(this.element[i], this.element[result])) then
							set result = i
						endif
					set i = i + 1
				endloop
				return result
			endmethod
			
			public stub method clear takes nothing returns nothing
				local integer i = 0
				loop
					exitwhen (i == $MAXSIZE$)
					set this.element[i] = $NULLVALUE$
					set i = i + 1
				endloop
			endmethod

			/// Mit dem []-Operator kann man auf ein Element des Containers zugreifen.
			/// @param index Index des Elements
			/// @return Das Element
			public method operator[] takes integer index returns $ELEMENTTYPE$
				return this.getElement(index)
			endmethod

			/// Mit dem []=-Operator kann man einem Element des Containers einen Wert zuweisen.
			/// @param index Index des Elements
			/// @param value Zuzuweisender Wert
			public method operator[]= takes integer index, $ELEMENTTYPE$ value returns nothing
				call this.setElement(index, value)
			endmethod

			/// Mit dem <-Operator kann man prüfen, ob der Container eine kleinere Größe als ein anderer hat.
			/// @param destination Vergleichs-Container
			/// @return Gibt wahr zurück, wenn der Container eine kleinere Größe als der Ziel-Container hat.
			public method operator< takes $NAME$ destination returns boolean
				return this.element.size < destination.element.size
			endmethod

			// Mit dem >-Operator kann man prüfen, ob der Container eine größere Größe als ein anderer hat.
			// @param destination Vergleichs-Container
			// @return Gibt wahr zurück, wenn der Container eine kleinere Größe als der Ziel-Container hat.
			//public method operator> takes A$ELEMENTTYPENAME$Container destination returns boolean
				//return this.size > destionation.size
			//endmethod
		endstruct

		//Basic algorithm

		//The binary predicate represents the < operator.
		//function AMax takes $ELEMENTTYPE$ x, $ELEMENTTYPE$ y, A$ELEMENTTYPENAME$ContainerBinaryPredicate binaryPredicate returns $ELEMENTTYPE
		//	if (binaryPredicate.evaluate(x, y)) then
		//		return y
		//	endif
		//	return x
		//endfunction

		//function AMin takes $ELEMENTTYPE$ x, $ELEMENTTYPE$ y, A$ELEMENTTYPENAME$ContainerBinaryPredicate binaryPredicate returns $ELEMENTTYPE
		//	if (binaryPredicate.evaluate(x, y)) then
		//		return x
		//	endif
		//	return y
		//endfunction
	//! endtextmacro

endlibrary