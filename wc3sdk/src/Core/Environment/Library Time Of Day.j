/**
* These functions do only work with environment's default settings (24 hours per day, 60 minutes per hour)
*/
library ALibraryCoreEnvironmentTimeOfDay requires ALibraryCoreStringConversion

	function GetTimeOfDayElapsedHours takes nothing returns integer
		return R2I(GetTimeOfDay())
	endfunction

	function GetTimeOfDayElapsedMinutesInHour takes nothing returns integer
		return R2I((GetTimeOfDay() - I2R(GetTimeOfDayElapsedHours())) * 100.0)
	endfunction

	function GetTimeOfDayRemainingHours takes nothing returns integer
		return 24 - GetTimeOfDayElapsedHours()
	endfunction

	globals
		private constant integer dailyMinutes = 1440 // 24 * 60
		private constant integer dailySeconds = 86400 // 24 * 60 * 60
	endglobals

	function GetTimeOfDayElapsedMinutes takes nothing returns integer
		return GetTimeOfDayElapsedHours() * 60 + GetTimeOfDayElapsedMinutesInHour()
	endfunction

	function GetTimeOfDayElapsedSecondsInMinute takes nothing returns integer
		return R2I(((GetTimeOfDay() - I2R(GetTimeOfDayElapsedHours())) * 100.0 - I2R(GetTimeOfDayElapsedMinutes())) * 100.0)
	endfunction

	function GetTimeOfDayRemainingMinutes takes nothing returns integer
		return dailyMinutes - GetTimeOfDayElapsedMinutes()
	endfunction

	function GetTimeOfDayElapsedSeconds takes nothing returns integer
		return GetTimeOfDayElapsedMinutes() * 60 + GetTimeOfDayElapsedSecondsInMinute()
	endfunction

	function GetTimeOfDayRemainingSeconds takes nothing returns integer
		return dailySeconds - GetTimeOfDayElapsedSeconds()
	endfunction

	function GetTimeOfDayString takes nothing returns string
		return GetTimeString(GetTimeOfDayElapsedSeconds())
	endfunction

endlibrary