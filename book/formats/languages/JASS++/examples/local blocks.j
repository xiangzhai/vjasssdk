// Jass and vJass
function ShowImageToPlayer takes player whichPlayer, image whichImage, boolean show returns nothing
	if (whichPlayer == GetLocalPlayer()) then
		call ShowImage(whichImage, show)
	endif
endfunction

function ShowImageToForce takes force whichForce, image whichImage, boolean show returns nothing
	if (IsPlayerInForce(GetLocalPlayer(), whichForce)) then
		call ShowImage(whichImage, show)
	endif
endfunction

// JASS++

void ShowImageToPlayer(player const whichPlayer, image whichImage, boolean show)
	local (whichPlayer)
		ShowImage(whichImage, show)

void ShowImageToForce(force const whichForce, image whichImage, boolean show)
	local (whichForce)
		ShowImage(whichImage, show)

