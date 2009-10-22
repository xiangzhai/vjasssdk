/// Provides a simple signal template struct (oriented on Qt and the Boost signal library).
/// Does not support signal return values.
/// @author Tamino Dauth
library AStructCoreGeneralSignal requires optional ALibraryCoreDebugMisc

	//! textmacro A_SIGNAL takes STRUCTPREFIX, NAME, PARAMETERS, PARAMETERNAMES, SIZE
		function interface $NAME$FunctionInterface takes $PARAMETERS$ returns nothing

		$STRUCTPREFIX$ struct $NAME$
			private static constant integer maxSlots = $SIZE$
			private integer m_slotCount = 0
			private $NAME$FunctionInterface array m_slots[$NAME$.maxSlots]
			private boolean array m_slotIsBlocked[$NAME$.maxSlots]

			debug private method checkIndex takes integer index returns nothing
				debug if (index < 0 or index > this.m_slotCount) then
					debug call Print("Wrong index: " + I2S(index) + ".")
				debug endif
			debug endmethod

			public method slotIndex takes $NAME$FunctionInterface slot returns integer
				local integer i = 0
				loop
					exitwhen (i == this.m_slotCount)
					if (this.m_slots[i] == slot) then
						return i
					endif
					set i = i + 1
				endloop
				return -1
			endmethod

			public method connect takes $NAME$FunctionInterface slot returns integer
				local integer index = this.m_slotCount
				call this.connectWithIndex(slot, index)
				return index
			endmethod

			public method connectWithIndex takes $NAME$FunctionInterface slot, integer index returns nothing
				debug if (this.m_slots[index] != 0) then
					debug call Print("Slot " + I2S(index) + " has already been taken.")
				debug elseif (this.m_slotCount == thistype.maxSlots) then
					debug call Print("Reached slot maximum: " + I2S(thistype.maxSlots) + ".")
				debug endif
				set this.m_slots[index] = slot
				set this.m_slotIsBlocked[index] = false
				if (this.m_slotCount < index + 1) then
					set this.m_slotCount = index + 1
				endif
			endmethod

			public method disconnect takes $NAME$FunctionInterface slot returns boolean
				local integer index = this.slotIndex(slot)
				if (index != -1) then
					call this.disconnectByIndex(index)
					return true
				endif
				return false
			endmethod

			public method disconnectByIndex takes integer index returns nothing
				local integer i
				debug call this.checkIndex(index)
				set this.m_slots[index] = 0
				//move list
				loop
					set i = index + 1
					exitwhen (i == this.m_slotCount)
					set this.m_slots[index] = this.m_slots[i]
					set index = i
				endloop
				set this.m_slotCount = this.m_slotCount - 1
			endmethod

			public method isConnected takes $NAME$FunctionInterface slot returns boolean
				return this.slotIndex(slot) != -1
			endmethod

			public method isBlocked takes $NAME$FunctionInterface slot returns boolean
				local integer index = this.slotIndex(slot)
				if (index == -1) then
					debug call Print("Can't check if slot is blocked because it does not exist.")
					return true
				endif
				return this.m_slotIsBlocked[index]
			endmethod

			public method isBlockedByIndex takes integer index returns boolean
				debug call this.checkIndex(index)
				return this.m_slotIsBlocked[index]
			endmethod

			public method emit takes $PARAMETERS$ returns nothing
				local integer i = 0
				loop
					exitwhen (i == this.m_slotCount)
					if (not this.m_slotIsBlocked[i]) then
						call $NAME$FunctionInterface(this.m_slots[i]).execute($PARAMETERNAMES$)
					endif
					set i = i + 1
				endloop
			endmethod
		endstruct
	//! endtextmacro

endlibrary