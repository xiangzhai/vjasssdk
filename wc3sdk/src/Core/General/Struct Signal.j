/// Provides a simple signal template struct (oriented on Qt and the Boost signal library).
/// Does not support signal return values.
/// @author Tamino Dauth
library AStructCoreGeneralSignal requires ALibraryCoreDebugMisc

	//! textmacro A_SIGNAL takes PREFIX, PARAMETERS, PARAMETERNAMES, SIZE
		function interface A$PREFIX$SignalFunctionInterface takes $PARAMETERS$ returns nothing

		struct A$PREFIX$Signal
			private static constant integer maxSlots = $SIZE$
			private integer slotCount = 0
			private A$PREFIX$SignalFunctionInterface array slots[A$PREFIX$Signal.maxSlots]
			private boolean array slotIsBlocked[A$PREFIX$Signal.maxSlots]

			debug private method checkIndex takes integer index returns nothing
				debug if (index < 0 or index > this.slotCount) then
					debug call this.print("Wrong index: " + I2S(index) + ".")
				debug endif
			debug endmethod

			public method slotIndex takes A$PREFIX$SignalFunctionInterface slot returns integer
				local integer i = 0
				loop
					exitwhen (i == this.slotCount)
					if (this.slots[i] == slot) then
						return i
					endif
					set i = i + 1
				endloop
				return -1
			endmethod

			public method connect takes A$PREFIX$SignalFunctionInterface slot returns integer
				local integer index = this.slotCount
				call this.connectWithIndex(slot, index)
				return index
			endmethod

			public method connectWithIndex takes A$PREFIX$SignalFunctionInterface slot, integer index returns nothing
				debug if (this.slots[index] != 0) then
					debug call this.print("Slot " + I2S(index) + " has already been taken.")
				debug elseif (this.slotCount == thistype.maxSlots) then
					debug call this.print("Reached slot maximum: " + I2S(thistype.maxSlots) + ".")
				debug endif
				set this.slots[index] = slot
				set this.slotIsBlocked[index] = false
				if (this.slotCount < index + 1) then
					set this.slotCount = index + 1
				endif
			endmethod

			public method disconnect takes A$PREFIX$SignalFunctionInterface slot returns boolean
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
				set this.slots[index] = 0
				//move list
				loop
					set i = index + 1
					exitwhen (i == this.slotCount)
					set this.slots[index] = this.slots[i]
					set index = i
				endloop
				set this.slotCount = this.slotCount - 1
			endmethod

			public method isConnected takes A$PREFIX$SignalFunctionInterface slot returns boolean
				return this.slotIndex(slot) != -1
			endmethod

			public method isBlocked takes A$PREFIX$SignalFunctionInterface slot returns boolean
				local integer index = this.slotIndex(slot)
				if (index == -1) then
					debug call this.print("Can't check if slot is blocked because it does not exist.")
					return true
				endif
				return this.slotIsBlocked[index]
			endmethod

			public method isBlockedByIndex takes integer index returns boolean
				debug call this.checkIndex(index)
				return this.slotIsBlocked[index]
			endmethod

			public method emit takes $PARAMETERS$ returns nothing
				local integer i = 0
				loop
					exitwhen (i == this.slotCount)
					if (not this.slotIsBlocked[i]) then
						call A$PREFIX$SignalFunctionInterface(this.slots[i]).execute($PARAMETERNAMES$)
					endif
					set i = i + 1
				endloop
			endmethod
		endstruct
	//! endtextmacro

endlibrary

//library AStructCoreDebugWindow initializer init

//	//! runtextmacro A_SIGNAL("MouseMovement", "real x, real y", "x, y", "100")

//	private function privateSlot0 takes real x, real y returns nothing
//	endfunction

//	private function privateSlot1 takes real x, real y returns nothing
//	endfunction

//	struct AWindow
//		private AMouseMovementSignal m_signal

//		public method signal takes nothing returns AMouseMovementSignal
//			return this.m_signal
//		endmethod

//		public method moveMouse takes real x, real y returns nothing
//			call this.m_signal.emit(x, y)
//		endmethod

//		public static method create takes nothing returns AWindow
//			local AWindow this = AWindow.allocate()
//			set this.m_signal = AMouseMovementSignal.create()
//			call this.m_signal.connect(privateSlot0)
//			call this.m_signal.connect(privateSlot1)
//			return this
//		endmethod
//	endstruct

//	function publicSlot takes real x, real y returns nothing
//	endfunction

//	globals
//		private AWindow window
//	endglobals

//	private function init takes nothing returns nothing
//		set window = AWindow.create()
//		call window.signal().connect(publicSlot)
//	endfunction

//endlibrary