#!BPY

"""
Name: 'Blizzard Warcraft III model file (.mdx)...'
Blender: 249a
Group: 'Import'
Tooltip: "Import a model file from Blizzard's Warcraft III"
"""

import Blender
from warblender.mdx import mdx
from warblender.importer import Importer


def loadMDX(filename):
	r = mdx.Reader()
	mdxModel = None
	
	try:
		mdxModel = r.loadFile(filename)
	except mdx.MDXFileFormatError:
		Blender.Draw.Text("Could not read model file")
		raise
	
	imp = Importer()
	imp.toBlender(mdxModel)

print "Warblender"
cfgdir = Blender.Get('datadir');
if cfgdir != None:
	try:
		file = open(cfgdir + "/warblender.autoload", 'r')
	except:
		file = None
	if not file:
		Blender.Window.FileSelector(loadMDX, "Import MDX")
	else:
		filename = file.readline();
		filename = filename.strip()
		file.close()
		if filename:
			loadMDX(filename)
		else:
			Blender.Window.FileSelector(loadMDX, "Import MDX")
else:
	Blender.Window.FileSelector(loadMDX, "Import MDX")
