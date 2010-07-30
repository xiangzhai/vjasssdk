/**
* Required image file properties (listed by PitzerMike):
* <ul>
* <li>Image files must have an invisible border of 1 pixel</li>
* <li>You have to create new image files with their original size and a transparent background</li>
* <li>Image file sizes have to be a power of 2</li>
* <li>You have to insert your bordered image file into the transparent one</li>
* <li>Image files may only have RGB channels (no alpha channels)</li>
* <li>Image files have to be saved as 32 Bit TGA</li>
* <li>Image files should be converted into BLP format</li>
* </ul>
*/
library ALibraryCoreInterfaceImage requires optional ALibraryCoreDebugMisc

	/**
	* Creates an image being visible for only one player.
	* @author PitzerMike
	* Local variable is being needed because of rendering.
	* @todo Crashes game when showing image!
	*/
	function CreateImageForPlayer takes player whichPlayer, string file, real x, real y, real z, real sizeX, real sizeY returns image
		debug call PrintFunctionError("CreateImageForPlayer", "Is bugged!")
		return null
		/*
		local image whichImage
		local string localFile = ""
		if (whichPlayer == GetLocalPlayer()) then
			set localFile = file
		endif
		set whichImage = CreateImage(localFile, sizeX, sizeY, 0.0, (x - (sizeX / 2.0)), (y - (sizeY / 2.0)), z, 0.0, 0.0, 0.0, 2) // image is placed in centre
		call SetImageRenderAlways(whichImage, true)

		return whichImage
		*/
	endfunction

	/**
	* Shows or hides image @param whichImage for player @param whichPlayer.
	* @param whichPlayer Player who the image is shown or hidden to.
	* @param whichImage Image handle which should be shown or hidden.
	* @param show If this value is true image @param whichImage will be shown otherwise it will be hidden.
	*/
	function ShowImageForPlayer takes player whichPlayer, image whichImage, boolean show returns nothing
		local player localPlayer = GetLocalPlayer()
		if (whichPlayer == localPlayer) then
			call ShowImage(whichImage, show)
		endif
		set localPlayer = null
	endfunction

endlibrary