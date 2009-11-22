library ALibraryCoreInterfaceImage

	/**
	* @author PitzerMike
	* - Bilder brauchen einen unsichtbaren Rand von 1 Pixel
	* - Man macht ein neues Bild mit der originalen Größe und transparentem Hintergrund
	* - Bildergrößen müssen eine 2er Potenz sein!!!
	* - Dann fügt man das Bild mit Rand in das neue Bild und hat einen transparenten Rand
	* - Das Bild darf nur RGB-Kanäle und keine Alpha-Kanäle enthalten
	* - Dann speichert man es als 32-Bit-TGA
	* - Dann muss man es mit dem Wc3Viewer zu einem BLP-Bild konvertieren
	* Hier braucht man eine lokale Variable für das Image, wegen des Renderns
	*/
	function CreateImageForPlayer takes player whichPlayer, string file, real x, real y, real z, real sizeX, real sizeY returns image
		local player localPlayer = GetLocalPlayer()
		local image whichImage	
		local string localFile = ""
		if (whichPlayer == localPlayer) then
			set localFile = file
		endif
		set whichImage = CreateImage(localFile, sizeX, sizeY, 0.0, (x - (sizeX / 2.0)), (y - (sizeY / 2.0)), z, 0.0, 0.0, 0.0, 2) //Bild wird in der Mitte platziert
		call SetImageRenderAlways(whichImage, true) //Hierbei testen, ob es dadurch dann doch für alle Spieler sichtbar ist
		set localPlayer = null
		return whichImage
	endfunction

	/**
	* Shows or hides image @param shownImage for player @param user.
	* @param user Player who the image is shown or hidden to.
	* @param shownImage Image handle which should be shown or hidden.
	* @param show If this value is true image @param shownImage will be shown otherwise it will be hidden.
	*/
	function ShowImageForPlayer takes player whichPlayer, image whichImage, boolean show returns nothing
		local player localPlayer = GetLocalPlayer()
		if (whichPlayer == localPlayer) then 
			call ShowImage(whichImage, show)
		endif
		set localPlayer = null
	endfunction

endlibrary