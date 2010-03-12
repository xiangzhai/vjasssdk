library ALibraryCoreInterfaceTextTag

	/// @author Tamino Dauth
	/// We do not want to use player forces ftw!!!
	function ShowTextTagForPlayer takes player user, texttag textTag, boolean show returns nothing
		local player localPlayer = GetLocalPlayer()
		if (user == localPlayer) then
			call SetTextTagVisibility(textTag, show)
		endif
		set localPlayer = null
	endfunction

	/// @author Tamino Dauth
	function ShowFadingTextTagForPlayer takes player user, string text, real size, real x, real y, integer red, integer green, integer blue, integer alpha, real velocity, real fadepoint, real lifespan returns nothing
		local player localPlayer
		local texttag textTag = CreateTextTag()
		call SetTextTagText(textTag, text, size)
		call SetTextTagPos(textTag, x, y, 0.0)
		call SetTextTagColor(textTag, red, green, blue, alpha)
		call SetTextTagVelocity(textTag, 0.0, velocity)
		if (user == null) then
			call SetTextTagVisibility(textTag, true)
		else
			call ShowTextTagForPlayer(user, textTag, true)
		endif
		call SetTextTagFadepoint(textTag, fadepoint)
		call SetTextTagLifespan(textTag, lifespan)
		call SetTextTagPermanent(textTag, false)
		set textTag = null
	endfunction

	/// @author Tamino Dauth
	function ShowGeneralFadingTextTagForPlayer takes player user, string text, real x, real y, integer red, integer green, integer blue, integer alpha returns nothing
		call ShowFadingTextTagForPlayer(user, text, 0.025, x, y, red, green, blue, alpha, 0.03, 1.0, 2.0)
	endfunction

	// The following parameters have been token from the UI/Misc.txt file of the War3X.mpq archive.

	/// @author Tamino Dauth
	function ShowGoldTextTagForPlayer takes player user, real x, real y, integer gold returns nothing
		call ShowFadingTextTagForPlayer(user, "+" + I2S(gold), 0.025, x, y, 255, 220, 0, 255, 0.03, 1.0, 2.0)
	endfunction

	/// @author Tamino Dauth
	function ShowLumberTextTagForPlayer takes player user, real x, real y, integer lumber returns nothing
		call ShowFadingTextTagForPlayer(user, "+" + I2S(lumber), 0.025, x, y, 0, 200, 80, 255, 0.03, 1.0, 2.0)
	endfunction

	/// @author Tamino Dauth
	function ShowBountyTextTagForPlayer takes player user, real x, real y, integer bounty returns nothing
		call ShowFadingTextTagForPlayer(user, "+" + I2S(bounty), 0.025, x, y, 255, 220, 0, 255, 0.03, 2.0, 3.0)
	endfunction

	/// @author Tamino Dauth
	function ShowMissTextTagForPlayer takes player user, real x, real y returns nothing
		call ShowFadingTextTagForPlayer(user, GetLocalizedString("MISS") + "!", 0.025, x, y, 255, 0, 0, 255, 0.03, 1.0, 3.0)
	endfunction

	/// @author Tamino Dauth
	function ShowCriticalStrikeTextTagForPlayer takes player user, real x, real y, integer damage returns nothing
		call ShowFadingTextTagForPlayer(user, I2S(damage) + "!", 0.025, x, y, 255, 0, 0, 255, 0.04, 2.0, 5.0)
	endfunction

	/// @author Tamino Dauth
	function ShowShadowStrikeTextTagForPlayer takes player whichPlayer, real x, real y, integer damage returns nothing
		call ShowFadingTextTagForPlayer(whichPlayer, I2S(damage) + "!", 0.025, x, y, 160, 255, 0, 255, 0.04, 2.0, 5.0)
	endfunction

	/// @author Tamino Dauth
	function ShowManaBurnTextTagForPlayer takes player user, real x, real y, integer damage returns nothing
		call ShowFadingTextTagForPlayer(user, "-" + I2S(damage), 0.025, x, y, 82, 82 ,255 ,255, 0.04, 2.0, 5.0)
	endfunction

	/// @author Tamino Dauth
	/// @todo Look for bash.
	function ShowBashTextTagForPlayer takes player user, real x, real y, integer damage returns nothing
		call ShowFadingTextTagForPlayer(user, I2S(damage) + "!", 0.025, x, y, 255, 0, 0, 255, 0.04, 2.0, 5.0)
	endfunction

	/**
	* @author Vexorian
	* @author Tamino Dauth
	*/
	function Bounty takes player whichPlayer, real x, real y, integer bounty returns nothing
		local string effectPath = ""
		local effect whichEffect
		call SetPlayerState(whichPlayer, PLAYER_STATE_RESOURCE_GOLD, GetPlayerState(whichPlayer, PLAYER_STATE_RESOURCE_GOLD) + bounty)
		if (GetLocalPlayer() == whichPlayer) then
			set effectPath = "UI\\Feedback\\GoldCredit\\GoldCredit.mdl"
		endif
		set whichEffect = AddSpecialEffect(effectPath, x, y)
		call DestroyEffect(whichEffect)
		set whichEffect = null
		call ShowBountyTextTagForPlayer(whichPlayer, x, y, bounty)
	endfunction

endlibrary