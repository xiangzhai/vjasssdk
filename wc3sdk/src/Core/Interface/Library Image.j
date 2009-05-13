library ALibraryCoreInterfaceImage

	/// @author PitzerMike
	/// - Bilder brauchen einen unsichtbaren Rand von 1 Pixel
	/// - Man macht ein neues Bild mit der originalen Größe und transparentem Hintergrund
	/// - Bildergrößen müssen eine 2er Potenz sein!!!
	/// - Dann fügt man das Bild mit Rand in das neue Bild und hat einen transparenten Rand
	/// - Das Bild darf nur RGB-Kanäle und keine Alpha-Kanäle enthalten
	/// - Dann speichert man es als 32-Bit-TGA
	/// - Dann muss man es mit dem Wc3Viewer zu einem BLP-Bild konvertieren
	/// Hier braucht man eine lokale Variable für das Image, wegen des Renderns
	function CreateImageForPlayer takes player user, string imagePath, real x, real y, real z, real sizeX, real sizeY returns image
		local player localPlayer = GetLocalPlayer()
		local image usedImage	
		local string localPath = ""
		if (user == localPlayer) then
			set localPath = imagePath
		endif
		set usedImage = CreateImage(localPath, sizeX, sizeY, 0.0, (x - (sizeX / 2.0)), (y - (sizeY / 2.0)), z, 0.0, 0.0, 0.0, 2) //Bild wird in der Mitte platziert
		call SetImageRenderAlways(usedImage, true) //Hierbei testen, ob es dadurch dann doch für alle Spieler sichtbar ist
		set localPlayer = null
		return usedImage
	endfunction

	/// Shows or hides image @param shownImage for player @param user.
	/// @param user Player who the image is shown or hidden to.
	/// @param shownImage Image handle which should be shown or hidden.
	/// @param show If this value is true image @param shownImage will be shown otherwise it will be hidden.
	function ShowImageForPlayer takes player user, image shownImage, boolean show returns nothing
		local player localPlayer = GetLocalPlayer()
		if (user == localPlayer) then 
			call ShowImage(shownImage, show)
		endif
		set localPlayer = null
	endfunction

endlibrary