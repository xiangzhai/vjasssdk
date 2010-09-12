/**
* These functions do only work with environment's default settings (24 hours per day, 60 minutes per hour)
* @note GetTimeOfDay returns current hour and percentage of minutes/seconds of day.
*/
library ALibraryCoreEnvironmentTimeOfDay requires ALibraryCoreStringConversion

	function GetTimeOfDayElapsedHours takes nothing returns integer
		return R2I(GetTimeOfDay())
	endfunction

	function GetTimeOfDayMinutesPercentage takes nothing returns real
		return 60.0 * (GetTimeOfDay() - I2R(GetTimeOfDayElapsedHours()))
	endfunction

	function GetTimeOfDayElapsedMinutesInHour takes nothing returns integer
		return R2I(GetTimeOfDayMinutesPercentage())
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

	function GetTimeOfDaySecondsPercentage takes nothing returns real
		return 60.0 * (GetTimeOfDayMinutesPercentage() - I2R(GetTimeOfDayElapsedMinutesInHour()))
	endfunction

	function GetTimeOfDayElapsedSecondsInMinute takes nothing returns integer
		return R2I(GetTimeOfDaySecondsPercentage())
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