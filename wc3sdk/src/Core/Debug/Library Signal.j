/// @author Tamino Dauth
library ALibraryCoreDebugSignal requires ALibraryCoreDebugMisc, AStructCoreGeneralSignal

	//! runtextmacro A_SIGNAL("private", "MouseMovementSignal", "real x, real y", "x, y", "100")

	private function privateSlot0 takes real x, real y returns nothing
		debug call Print("Private slot 0 with x " + R2S(x) + " and y " + R2S(y) + ".")
	endfunction

	private function privateSlot1 takes real x, real y returns nothing
		debug call Print("Private slot 1 with x " + R2S(x) + " and y " + R2S(y) + ".")
	endfunction

	private struct Window
		private MouseMovementSignal m_signal

		public method signal takes nothing returns MouseMovementSignal
			return this.m_signal
		endmethod

		public method moveMouse takes real x, real y returns nothing
			call this.m_signal.emit(x, y)
		endmethod

		public static method create takes nothing returns Window
			local Window this = Window.allocate()
			set this.m_signal = MouseMovementSignal.create()
			call this.m_signal.connect(privateSlot0)
			call this.m_signal.connect(privateSlot1)
			return this
		endmethod
	endstruct

	private function publicSlot takes real x, real y returns nothing
		debug call Print("Public slot with x " + R2S(x) + " and y " + R2S(y) + ".")
	endfunction

	globals
		private Window window
	endglobals

	function ASignalDebug takes nothing returns nothing
		set window = Window.create()
		call window.signal().connect(publicSlot)
		debug call Print("Created window. Moving mouse to (10.0 | 15.0).")
		call window.moveMouse(10.0, 15.0)
		debug call Print("Finished movement.")
	endfunction

endlibrary