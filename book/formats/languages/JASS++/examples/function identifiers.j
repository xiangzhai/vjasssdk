// Jass and vJass
function ShowTextTagPlayerBJ takes boolean show, texttag tt, player whichPlayer returns nothing
	if (whichPlayer == GetLocalPlayer()) then
		call SetTextTagVisibility(tt, show)
	endif
endfunction

call ShowTextTagPlayerBJ(true, tt, whichPlayer)
call ShowTextTagForceBJ(true, tt, whichForce)

// Jass++
void ShowTextTag(boolean show, texttag tt, player const whichPlayer)
{
	local (whichPlayer)
	{
		SetTextTagVisibility(tt, show);
	}
}

alias ShowTextTagForceBJ(ShowTextTag);

ShowTextTag(true, tt, whichPlayer);
ShowTextTag(true, tt, whichForce);