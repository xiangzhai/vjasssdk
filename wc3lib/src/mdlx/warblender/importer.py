import sys
import os
import math

import Blender
#from Blender.Armature.Bone import SKINNABLE, UNSKINNABLE
from Blender.Mathutils import Quaternion
#from Blender.Armature.Bone import LOC, ROT, SIZE

from mdx import mdx
from converter import Converter


def add(x, y):
	return x+y

def sub(x, y):
	return x-y

class Importer(Converter):
	def __init__(self, scene=None, fps=None):
		Converter.__init__(self, scene, fps)
		self.model = None
		self.textures = None
		self.materials = None
		self.meshes = None
		self.armature = None

	def toBlender(self, model):
		self.model = model
		self.meshes = []
		self.armature = Blender.Armature.Armature(self.model.MODL.Name + ".Armature")

		sys.stderr.write("Converting model %s to Blender data structures..\n" % (repr(self.model.MODL.Name)))
		self.loadTextures()
		self.loadMaterials()
		self.loadMeshes()
		self.loadBones()
		self.loadSequences()
		sys.stderr.write("Done converting model %s.\n" % (repr(self.model.MODL.Name)))

		self.armature = None
		self.meshes = None
		self.scene = None
		self.model = None

	def loadTextures(self):
		(filepath, dummy) = os.path.split(self.model.filename)
		self.textures = []
		for texnr in range(self.model.TEXS.ntexs):
			tex = Blender.Texture.New(self.model.MODL.Name + '.Tex' + str(texnr))
			tex.setType('Image')
			tex.setImageFlags('InterPol', 'UseAlpha', 'MipMap')
			texturefile = self.model.TEXS.textures[texnr].TexturePath;
			if texturefile!='':
				if os.name!='nt':
					texturefile = texturefile.replace("\\", "/")
				(texturefile, dummy) = os.path.splitext(texturefile)
				texturefile = os.path.join(filepath, texturefile + ".png")
				sys.stderr.write("texture %d: PNG texture file (%s)\n" % (texnr, texturefile))
				try:
					tex.setImage(Blender.Image.Load(texturefile))
				except IOError:
					pass
				self.textures.append(tex)
			else:
				sys.stderr.write("texture %d: None\n" % texnr)
				self.textures.append(None)


	def loadMaterials(self):
		self.materials = []
		for matnr in range(self.model.MTLS.nmtls):
			mat = Blender.Material.New(self.model.MODL.Name + ".Mat" + str(matnr))
			mat.setRGBCol(self.teamcolour)
			mat.setMode('Shadeless', 'ZTransp')
			setTexFace = 1
			for laynr in range(self.model.MTLS.materials[matnr].LAYS.nlays):
				mapto = 0
				lay = self.model.MTLS.materials[matnr].LAYS.layers[laynr]
				if lay.FilterMode == 1: # the usual kind of texture
					mapto = mapto | Blender.Texture.MapTo.ALPHA | Blender.Texture.MapTo.COL
				elif lay.FilterMode == 2: # the kind of tecture that shows team colour through the alpha
					mapto = mapto | Blender.Texture.MapTo.COL
					setTexFace = 0
				elif lay.FilterMode == 3:
					# set 'texture blending mode' to 'add'
					pass
				elif lay.FilterMode == 4:
					# set 'texture blending mode' to 'add'
					mapto = mapto | Blender.Texture.MapTo.ALPHA
				elif lay.FilterMode == 5:
					# modulate ??					
					pass
				tex = self.textures[self.model.MTLS.materials[matnr].LAYS.layers[laynr].TextureID]
				if tex!=None:
					mat.setTexture(laynr, tex, Blender.Texture.TexCo.UV, mapto)
					# FIXME we also need a way to specify the other kind of ALPHA setting
					# FIXME as well as the DVar option in the MapTo tab of Shading->Material Buttons
			if setTexFace==1:
				mat.setMode('TexFace')
			self.materials.append(mat)

	def loadMeshes(self):
		for geoset_i in range(len(self.model.GEOS.geosets)):
			geoset = self.model.GEOS.geosets[geoset_i]
			myMeshObject = Blender.Object.New('Mesh', self.model.MODL.Name + ".Mesh" + str(geoset_i))
			#mymesh = Blender.NMesh.New(self.model.MODL.Name + str(geoset_i))
			mymesh = myMeshObject.getData()
			sys.stderr.write("mesh %d: %d NRMS\n" % (geoset_i, geoset.NRMS.nvrts))

			mymesh.setMaterials([self.materials[geoset.MaterialID]]);
			mymesh.hasFaceUV(1);

			# vertices
			nrverts = 0
			for vert_i in range(len(geoset.VRTX.vertices)):
				vert = geoset.VRTX.vertices[vert_i]
				x = vert.x
				y = vert.y
				z = vert.z
				v = Blender.NMesh.Vert(x, y, z)
				v.no[0] = geoset.NRMS.vertices[vert_i].x
				v.no[1] = geoset.NRMS.vertices[vert_i].y
				v.no[2] = geoset.NRMS.vertices[vert_i].z
				#if HAS_TEX==1:
				#	v.uvco[0] = geoset.UVAS.UVBS[0].vertices[vert_i].x;
				#	v.uvco[1] = geoset.UVAS.UVBS[0].vertices[vert_i].y;
				mymesh.verts.append(v)
				nrverts += 1
			sys.stderr.write("mesh %d: %d verts\n" % (geoset_i, nrverts))
			sys.stderr.write("Mesh added to Blender.\n")
		
			# faces
			nrfaces = 0
			for i in range(geoset.PVTX.nvrts / 3):
				v1 = geoset.PVTX.vertices[3 * i]
				v2 = geoset.PVTX.vertices[3 * i + 1]
				v3 = geoset.PVTX.vertices[3 * i + 2]
				face = Blender.NMesh.Face([mymesh.verts[v1], mymesh.verts[v2], mymesh.verts[v3]])
	
				uv_v1 = (geoset.UVAS.UVBS[0].vertices[v1].x, 1-geoset.UVAS.UVBS[0].vertices[v1].y);
				uv_v2 = (geoset.UVAS.UVBS[0].vertices[v2].x, 1-geoset.UVAS.UVBS[0].vertices[v2].y);
				uv_v3 = (geoset.UVAS.UVBS[0].vertices[v3].x, 1-geoset.UVAS.UVBS[0].vertices[v3].y);
				face.uv = [uv_v1, uv_v2, uv_v3];
				face.mode = face.mode | Blender.NMesh.FaceModes['TEX'];
				face.mat = 0;
				mymesh.faces.append(face)
				nrfaces += 1
			sys.stderr.write("mesh %d: %d faces\n" % (geoset_i, nrfaces))
			mymesh.update()
			self.scene.link(myMeshObject)
			self.meshes.append(myMeshObject)

	def loadBones(self):
		#sys.stderr.write("Ordering bones..\n")
		#boneOrder = []
		#for bone_id in range(wbNrBones):
			#if bone_id not in boneOrder:
				#self.boneInsert(bone_id, boneOrder)
		#sys.stderr.write("Bone order: %s\n" % str(boneOrder))

		wbBoneVertices = [[] for x in self.model.bones]
		wbBoneChildren = [[] for x in self.model.bones]
		blMeshObjArray = []
		# populate wbBoneVertices for bone tail calculations,
		#   and blMeshObjArray for armature parenting
		for geoset_i in range(len(self.model.GEOS.geosets)):
			blMeshObj = Blender.Object.Get(self.model.MODL.Name + ".Mesh" + str(geoset_i))
			blMeshObjArray.append(blMeshObj)
			blMesh = blMeshObj.getData()
			wbGeoset = self.model.GEOS.geosets[geoset_i]
			wbGroups = [[] for x in range(wbNrBones)]
			# construct the group arrays
			for vert_id in range(wbGeoset.GNDX.nvgrps):
				groupid = wbGeoset.GNDX.vertexGroups[vert_id]
				group = wbGeoset.MATS.getGroup(wbGeoset, groupid)
				for boneingroup in group:
					wbGroups[boneingroup].append(vert_id)
			# assign vertices to Blender's vertex groups
			for bone_id in range(wbNrBones):
				if len(wbGroups[bone_id]) > 0:
					blMesh.addVertGroup(self.model.bones[bone_id].Name)
					blMesh.assignVertsToGroup(self.model.bones[bone_id].Name, wbGroups[bone_id], 1.0, 'replace')
					wbBoneVertices[bone_id].extend(
						[[wbGeoset.VRTX.vertices[i].x,
						wbGeoset.VRTX.vertices[i].y,
						wbGeoset.VRTX.vertices[i].z] for i in wbGroups[bone_id]])
			blMesh.update()

		#bones = {}
		wbBoneHeads = [[] for x in self.model.bones]
		index = 0
		# create the bones and set the bone head
		for wbBone in self.model.bones:
			wbPivt = self.model.PIVT.pivpts[wbBone.ObjectID]

			if wbBone.Parent != -1:
				self.armature.bones[index].setParent(self.armature.bones[wbBone.Parent])
				wbBoneChildren[wbBone.Parent].append(index)
			else:
				parentTail = [0, 0, 0]
			head = [wbPivt.x, wbPivt.y, wbPivt.z]
			self.armature.bones[index].setHead(head[0], head[1], head[2])
			self.armature.bones[index].hasIK(1)
			if wbBone.skinnable:
				self.armature.bones[index].setBoneclass((SKINNABLE))
			else:
				self.armature.bones[index].setBoneclass((UNSKINNABLE))
			sys.stderr.write("Adding Blender bone '" + name + "\n")
			wbBoneHeads[index] = head
			index += 1
		# set the bone tails
		index = 0
		for wbBone in self.model.bones:
			if len(wbBoneChildren[index])>0:
				# average the heads of all children
				avg = [0.0, 0.0, 0.0]
				for child in wbBoneChildren[index]:
					avg = map(add, avg, wbBoneHeads[child])
				avg = [x/len(wbBoneChildren[index]) for x in avg]
				self.armature.bones[index].setTail(avg[0], avg[1], avg[2])
			elif len(wbBoneVertices[index]) > 0:
				# average all the points belonging to this bone and
				# tail = mirror of head through this middle point
				avg = [0.0, 0.0, 0.0]
				for vert in wbBoneVertices[index]:
					avg = map(add, avg, vert)
				avg = [x/len(wbBoneVertices[index]) for x in avg]
				self.armature.bones[index].setTail(avg[0], avg[1], avg[2])
			index += 1


		# create scene object
		blArmObj = Blender.Object.New(self.armature.name)
		blArmObj.link(self.armature)
		blArmObj.makeParent(blMeshObjArray)
		self.scene.link(blArmObj)

	def loadVertexGroups(self):
		self.groups = {}
		for vrt_i in range(len(geoset.GNDX.vertexGroups)):
			objList = geoset.MATS.matrices[geoset.MTGC.groupCount[geoset.GNDX.vertexGroups[vrt_i]]]
			sys.stderr.write("objList: %s\n" % type(objList))
			boneNames = [self.BONE.bones[obj_i].OBJ.Name for obj_i in objList]
			for name in boneNames:
				if GROUPS.has_key(name):
					self.groups[name].append(vrt_i)
				else:
					self.groups[name] = [vrt_i]
		for grp_name, grp in self.groups:
			mymesh.addVertGroup(grp_name)
			mymesh.assignVertsToGroup(grp_name, grp, 1.0, 'replace')

	def boneInsert(self, boneIndex, boneOrder):
		#sys.stderr.write("boneIndex: %s\n" % str(boneIndex))
		parentID = self.model.bones[boneIndex].Parent
		#sys.stderr.write("parentID: %s\n" % str(parentID))
		if parentID != -1 and parentID not in boneOrder:
			self.boneInsert(parentID, boneOrder)
		boneOrder.append(boneIndex)
	
	def loadSequences(self):
		# <DEBUG>
		file = open(Blender.Get('datadir') + "/warblender.modelKeyFrames.txt", 'w')
		keys = self.model.keyFrames.keys()
		keys.sort()
		for item in keys:
			file.write(str(item) + ": " + str([obj.Name for obj in self.model.keyFrames[item]]) + "\n")
		file.close()
		# </DEBUG>
		for wbSeq in self.model.SEQS.seqs:
			blAct = Blender.Armature.NLA.NewAction(wbSeq.Name)
			sys.stderr.write("New action from sequence '" + wbSeq.Name + "'\n")
			startTime = wbSeq.IntStart # start time of action in MDX time scale
			endTime = wbSeq.IntEnd
			wbActBones = self.model.keyFrames[startTime]
			#for name in [bone.Name for bone in self.model.keyFrames[end]]:
			#	sys.stderr.write("Bone in list: " + name + "\n")
			for wbBone in wbActBones:
				sys.stderr.write("bone name: " + wbBone.Name + "\n")
				self.makeBoneIpo(blAct, wbBone, startTime, endTime, wbSeq.NoLooping)

	def wbTime2blFrame(self, animTime, startTime):
		""" translates animTime from MDX time scale to Blender time scale, wrt to origin startTime """
		timeScale = 0.001
		return float(animTime - startTime) * self.fps * timeScale + 1

	def makeBoneIpo(self, blAct, wbBone, startTime, endTime, NoLooping):
		if NoLooping:
			Xpo = 'Constant'
		else:
			Xpo = 'Cyclic'

		index = len(self.armature.bones)
		blBone = self.armature.bones[index] # add new

		translation = wbBone.KGTR.present and wbBone.KGTR.keyFrames.has_key(startTime) and wbBone.KGTR.keyFrames.has_key(endTime)
		rotation = wbBone.KGRT.present and wbBone.KGRT.keyFrames.has_key(startTime) and wbBone.KGRT.keyFrames.has_key(endTime)
		scaling = wbBone.KGSC.present and wbBone.KGSC.keyFrames.has_key(startTime) and wbBone.KGSC.keyFrames.has_key(endTime)

		self.armature.bones[index].setPose([], blAct)
		ipo = blAct.getChannelIpo(wbBone.Name)

		# FIXME we probably still have to set the ones that are not specified to some
		#	default value! Perhaps 0 rotation, 0 translation, 1 scale? Add elses to the ifs to do this

		k = wbBone.KGTR
		if translation:
			xcurve = ipo.addCurve('LocX')
			xcurve.setInterpolation('Linear')
			xcurve.setExtrapolation(Xpo)

			ycurve = ipo.addCurve('LocY')
			ycurve.setInterpolation('Linear')
			ycurve.setExtrapolation(Xpo)

			zcurve = ipo.addCurve('LocZ')
			zcurve.setInterpolation('Linear')
			zcurve.setExtrapolation(Xpo)

			startFrame = k.keyFrames[startTime] # index of the starting frame in the wb bone's KG(TR|RT|SC) data
			endFrame = k.keyFrames[endTime]
			for framei in range(startFrame, endFrame + 1):
				frame = k.array[framei] # the struct containing the time (.Frame) and the data (.x, .y, .z, etc)
				blFramei = self.wbTime2blFrame(frame.Frame, startTime)
				sys.stderr.write("\tFrame "+str(blFramei)+": TR("+str(frame.x)+","+str(-1*frame.z)+","+str(frame.y)+")\n")
				xcurve.addBezier((blFramei, frame.x))
				ycurve.addBezier((blFramei, -1*frame.z))
				zcurve.addBezier((blFramei, frame.y))

		k = wbBone.KGRT
		if rotation:
			#self.makeRotationCurve_quat(ipo, k, startTime, endTime, Xpo)
			#def makeRotationCurve_quat(self, ipo, k, startTime, endTime, Xpo):

			xcurve = ipo.addCurve('QuatX')
			xcurve.setInterpolation('Linear')
			xcurve.setExtrapolation(Xpo)
        
			ycurve = ipo.addCurve('QuatY')
			ycurve.setInterpolation('Linear')
			ycurve.setExtrapolation(Xpo)
        
			zcurve = ipo.addCurve('QuatZ')
			zcurve.setInterpolation('Linear')
			zcurve.setExtrapolation(Xpo)
        
			wcurve = ipo.addCurve('QuatW')
			wcurve.setInterpolation('Linear')
			wcurve.setExtrapolation(Xpo)
        
			startFrame = k.keyFrames[startTime] # index of the starting frame in the wb bone's KG(TR|RT|SC) data
			endFrame = k.keyFrames[endTime]
			for framei in range(startFrame, endFrame + 1):
				frame = k.array[framei] # the struct containing the time (.Frame) and the data (.x, .y, .z, etc)
				blFramei = self.wbTime2blFrame(frame.Frame, startTime)
				#s_t2 = math.sin(math.acos(frame.d)) # sin_theta2: sine of half the rotation angle
				#if s_t2==0:
				#	axis = [0, 0, 0]
				#else:
				#	axis = [frame.a/s_t2, -1*frame.c/s_t2, frame.b/s_t2]
				if wbBone.Parent == -1:
					axis = [frame.a, frame.b, frame.c]
				else:
					axis = [frame.a, frame.c, frame.b]
					
				# Past this point, axis contains the right-handed axis, compensated for the Blender bug
				# FIXME When the Blender bug is fixed, axis = [a, b, -c]
        
				# FIXME Do stuff here to translate the quaternion from left-handedness to right-handedness
				quat_vals = [frame.d]
				quat_vals[len(quat_vals):] = axis
				q = Blender.Mathutils.Quaternion(quat_vals)
				q.normalize()
				xcurve.addBezier((blFramei, q.x))
				ycurve.addBezier((blFramei, q.y))
				zcurve.addBezier((blFramei, q.z))
				wcurve.addBezier((blFramei, q.w))

		k = wbBone.KGSC
		if scaling:
			xc = ipo.getCurve('SizeX')
			print [i.getPoints() for i in xc.getPoints()]

		
			xcurve = ipo.addCurve('SizeX')
			xcurve.setInterpolation('Linear')
			xcurve.setExtrapolation(Xpo)

			ycurve = ipo.addCurve('SizeY')
			ycurve.setInterpolation('Linear')
			ycurve.setExtrapolation(Xpo)

			zcurve = ipo.addCurve('SizeZ')
			zcurve.setInterpolation('Linear')
			zcurve.setExtrapolation(Xpo)

			startFrame = k.keyFrames[startTime] # index of the starting frame in the wb bone's KG(TR|RT|SC) data
			endFrame = k.keyFrames[endTime]
			for framei in range(startFrame, endFrame + 1):
				frame = k.array[framei] # the struct containing the time (.Frame) and the data (.x, .y, .z, etc)
				blFramei = self.wbTime2blFrame(frame.Frame, startTime)
				sys.stderr.write("\tFrame "+str(blFramei)+": SC("+str(frame.x)+","+str(-1*frame.z)+","+str(frame.y)+")\n")
				xcurve.addBezier((blFramei, frame.x))
				ycurve.addBezier((blFramei, -1*frame.z))
				zcurve.addBezier((blFramei, frame.y))
		return ipo

	def makeRotationCurve_euler(self, ipo, k, start, end, Xpo):
		xcurve = ipo.addCurve('RotX')
		xcurve.setInterpolation('Linear')
		xcurve.setExtrapolation(Xpo)

		ycurve = ipo.addCurve('RotY')
		ycurve.setInterpolation('Linear')
		ycurve.setExtrapolation(Xpo)

		zcurve = ipo.addCurve('RotZ')
		zcurve.setInterpolation('Linear')
		zcurve.setExtrapolation(Xpo)

		fStart = k.keyFrames[start]
		fEnd = k.keyFrames[end]
		for framei in range(fStart, fEnd + 1):
			frame = k.rotation[framei]
			quat = Quaternion([frame.a, frame.c, frame.b, frame.d])
			mat = quat.toEuler()
			xcurve.addBezier((self.wbTime2blFrame(frame.Frame, start), mat.x))
			ycurve.addBezier((self.wbTime2blFrame(frame.Frame, start), -1*mat.z))
			zcurve.addBezier((self.wbTime2blFrame(frame.Frame, start), mat.y))
	
	# new
	# @author Tamino Dauth
	def loadLights(self):
		sys.stderr.write("loading %i lights\n" % len(self.model.LITE.lights))
		
		for wbLamp in self.model.LITE.lights
			blLamp = Blender.Lamp.New()
			blLamp.setName(wbLamp.OBJ.Name)
			#(0:Omnidirectional;1:Directional;2:Ambient)
			if (wbLamp.Type == 0)
				blLamp.setType('Lamp')
			else if (wbLamp.Type == 1)
				blLamp.setType('Spot')
			else if (wbLamp.Type == 2)
				blLamp.setType('Area')
			else
				# todo throw exception
				
			#light.AttStart 
			#light.AttEnd
			blLamp.R = wbLamp.ColR
			blLamp.G = wbLamp.ColB
			blLamp.B = wbLamp.ColB
			blLamp.setEnergy(wbLamp.Intensity)
			#light.AmbColR
			#light.AmbColG
			#light.AmbColB
			#light.AmbColIntensity
			


	def loadParticleEmitters(self):
	
	def loadParticleEmitters2(self):
