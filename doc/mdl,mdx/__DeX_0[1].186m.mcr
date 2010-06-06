----------------------------------------------------------------------------------------------------------------------------
--MDL Toolkit: 'DeX
--This is the MDL exporter component of the MDL Toolkit for Warcraft III
--
--Note to prospective coders, PLEASE READ:
--To anyone considering adding any features or bug fixes to this code please comment every important change you make
--as well as identify the purpose of any variables defined. Please comment EVERY line that you edit or add at least with
--a small identifier so it is possible to identify the edited line. For example you could make a comment that uses your
--first initial such as "--b". Also please add an identifier to the dexversion so that your version can be uniquely
--identified. This could be a concatenation of your name or first
--initial, it really matters little what the identifier is so long as it's unique. The purpose of this is to avoid
--confusion with your editted version and the official versioning.
--
--Please section functions using a horizontal line of dashes and restrict additions of code to the block headers. For
--example put all new functions into "Function and global definitions">>"New functions".
--
--Whenever possible use swrite for all stream output and update that function as necessary rather than
--directly writing to a file within other areas of the code. The reason for this is to ensure that the gmax
--functionality remains reliable by ensuring all stream output is processed by the swrite stream interface.
--When using swrite sometimes the strings in the argument can get quite long so please use the escape character "\"
--to break the lines up. Please do the same with other lines of code that get very long so the code is kept clean
--and easy to read.
--
--'DeX version global variable: This variable is used globally for consistent version references. If any strings are to
--contain the version of 'DeX please do not use the version directly but reference the dexversion variable below.
global dexversion = "0.186m"
----------------------------------------------------------------------------------------------------------------------------
--Todo List
--fix rotations
--camera animation - rotational (positional animation done)
--fix scale key issue
--added ParticleEmitters, CS fixed and Ambient lights added by blinkboy

global DeX = macroscript DeX
	category:"MDL Toolkit"
	toolTip:("'DeX " + dexversion + ", MDL exporter")
	Icon:#("'DeX",1)
(

-----------------------------------
--Function and global definitions--
-----------------------------------


	global MDLObjects = #()
	global DefaultMDLBone = undefined
	global sel = #()
	global idcount = -1
	global meshes = #()
	global cam = undefined
	global streamtype
		--1:file
		--2:listener
		--3:memory
	global foutstream --outstream for file writing
	global gmaxstringlength = 15000 --maximum gmax string length
	global gmaxlistenerlength = 10000 --maximum gmax listener selection length
	
	global listenertemp = "" as stringstream --temporary listener stringstream

	--Every memory mdl block has the following string at the beginning so it can be searched
	--for in memory. The number of MDL blocks X, and block index Y are also concatenated
	--to the string in the following manner MemoryMDLIdentifierXXXXYYYY (length 27)
	global MDLsearchstring = "MemoryMDLIdentifierXXXXYYYY"
	global MemoryMDL = #(MDLsearchstring) --ouput for write to memory
	global memgrabpath = "memgrab.exe"

	struct MDLObj (o, objid, parent, type, ex)			-- type: 0 - unknown, 1 - bone, 2 - helper, 3 - attachment, 4 - ParticleEmitter2,
														--  5 - light, 6 - EventObject, 7 - CollisionShape
	struct BoneEx (geosid, geoaid)
	
	struct Key (time, value)				-- keyframe for animation
	struct TanKey (time, value, intan, outtan)
	
	-----------------
	--New Functions--
	-----------------
	fn swrite outstring =
	(
		if streamtype == 1 then --filestream
		(
			format outstring to:foutstream
		)

		if streamtype == 2 then --listener(incomplete)
		(
			format outstring to:listener
		)
		
		if streamtype == 3 then --memory
		(
			cb = MemoryMDL.count
			slength = outstring.count
			diff = gmaxstringlength - MemoryMDL[cb].count
			if diff <= slength then
			(
				halfoutstring = substring outstring 1 diff
				MemoryMDL[cb] = MemoryMDL[cb] + halfoutstring
				MemoryMDL[cb + 1] = MDLsearchstring
				MemoryMDL[cb + 1] = MemoryMDL[cb + 1] + (substring outstring (diff + 1) (slength - diff))
			)
			else
			(
				MemoryMDL[cb] = MemoryMDL[cb] + outstring
			)

		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn numberMemMDL =
	(
		blocks = MemoryMDL.count
		blockssl = (blocks as string).count
		bcount = replace "0000" (5 - blockssl) blockssl (blocks as string)
		for n = 1 to blocks do
		(
			MemoryMDL[n] = replace MemoryMDL[n] 20 4 bcount
			indexsl = (n as string).count
			bindex = replace "0000" (5 - indexsl) indexsl (n as string)
			MemoryMDL[n] = replace MemoryMDL[n] 24 4 bindex
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn isgmax =
	(
		return cylgizmo == undefined
	)
---------------------
--End New Functions--
---------------------
----------------------------------------------------------------------------------------------------------------------------
	fn ResetGlobals =
	(
		MDLObjects = #()
		DefaultMDLBone = undefined
		sel = #()
		idcount = -1
		meshes = #()
		cam = undefined
	)
----------------------------------------------------------------------------------------------------------------------------
	fn CompareMDLObjects a b =
	(
		if a.objid < b.objid then return -1
		else if a.objid > b.objid then return 1
		else return 0
	)
----------------------------------------------------------------------------------------------------------------------------
	fn IsSkinned o = (return o.modifiers[#Skin] != undefined)
----------------------------------------------------------------------------------------------------------------------------
	fn IsMesh o = (return classOf o == Editable_mesh)
----------------------------------------------------------------------------------------------------------------------------
	fn IsCamera o = (return classOf o == Targetcamera)
----------------------------------------------------------------------------------------------------------------------------
	fn IsCollisionShape o = (return classOf o == Box or classOf o == Sphere or classOf o == GeoSphere)
----------------------------------------------------------------------------------------------------------------------------
	fn IsAttachment o = (return (substring o.name 1 4) == "ATT:")
----------------------------------------------------------------------------------------------------------------------------
	fn IsEventObject o = (return (substring o.name 1 4) == "Obj:")
----------------------------------------------------------------------------------------------------------------------------
	fn IsParticleEmitter o = (return classOf o == Blizzard)
----------------------------------------------------------------------------------------------------------------------------
	fn IsLight o = (return classOf o == OmniLight)
----------------------------------------------------------------------------------------------------------------------------
	fn IsInSkin o =			-- unused
	(
		local modpanelobj = modPanel.getCurrentObject
		max modify mode
		for m in meshes do
		(
			if IsSkinned m then
			(
				sk = m.Skin
				modPanel.setCurrentObject m.modifiers[#Skin]
				for i = 1 to (skinOps.getNumberBones sk) do if (skinOps.getBoneName sk i) == o.name then return true
			)
		)
		return false
		if modpanelobj != undefined then modPanel.setCurrentObject modpanelobj
	)
----------------------------------------------------------------------------------------------------------------------------
	fn IsBone o = ( return IsInSkin o )
----------------------------------------------------------------------------------------------------------------------------
	fn IsHelper o = ( return (classOf o) == helper )
----------------------------------------------------------------------------------------------------------------------------
	fn ArrayCompare a b =				-- returns true if two arrays are exactly the same
	(
		if a == b then return 0			-- == operator for arrays only returns true if they are at same address, as in c
		if (classOf a) != Array and (classOf b) != Array then if a < b then return -1 else if a > b then return 1
		for i = 1 to a.count do
		(
			if b.count < i then return 1
			local c = ArrayCompare a[i] b[i]
			if c < 0 then return -1
			else if c > 0 then return 1
		)
		if a.count < b.count then return -1
		return 0
	)
----------------------------------------------------------------------------------------------------------------------------
	fn ArrayFindItem array value =
	(
		for i = 1 to array.count do
		(
			if (ArrayCompare array[i] value) == 0 then return i
		)
		return 0
	)
----------------------------------------------------------------------------------------------------------------------------
	fn QuatMult q s =
	(
		local p = q
		p.angle *= s
		return p
	)
----------------------------------------------------------------------------------------------------------------------------
	fn QuatSub q p =
	(
		return q * inverse(p)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn PositionKeys o =
	(
		local keys = #()
		
		local c = o.pos.controller
		
		local p = o.pos
		if o.parent != undefined then
		(
			p -= o.parent.pos
			pr = o.parent.rotation
		)
		
		case (classOf c) of
		(
		bezier_position: (
			for k in c.keys do
			(
				at time k.time local v = o.pos - p
				if o.parent != undefined then at time k.time v = ((o.pos - o.parent.pos) * inverse(o.parent.rotation * inverse(pr))) - p
				append keys (TanKey k.time v (v + k.inTangent) (v + k.outTangent))
			)
			return keys
		)
		tcb_position: (
			for k in c.keys do 
			(
				at time k.time local v = o.pos - p
				if o.parent != undefined then at time k.time v = ((o.pos - o.parent.pos) * inverse(o.parent.rotation * inverse(pr))) - p
				append keys (TanKey k.time v undefined undefined)
			)
			keys[1].intan = keys[1].outtan = keys[1].value
			keys[keys.count].intan = keys[keys.count].outtan = keys[keys.count].value
			for k = 2 to (c.keys.count - 1) do
			(
				local t = (c.keys[k].tension - 25) / 25
				local c_ = (c.keys[k].continuity - 25) / 25				-- "c" is taken by the position controller
				local b = (c.keys[k].bias - 25) / 25
				
				keys[k].intan = ((((1 - t) * (1 + c_) * (1 - b)) / 2) * (keys[k + 1].value - keys[k].value) + \
					(((1 - t) * (1 - c_) * (1 + b)) / 2) * (keys[k].value - keys[k - 1].value)) + keys[k].value
				keys[k].outtan = ((((1 - t) * (1 - c_) * (1 - b)) / 2) * (keys[k + 1].value - keys[k].value) + \
					(((1 - t) * (1 + c_) * (1 + b)) / 2) * (keys[k].value - keys[k - 1].value)) + keys[k].value
			)
			return keys
		)
		default: (
			for k in c.keys do 
			(
				at time k.time local v = o.pos - p
				if o.parent != undefined then at time k.time v = ((o.pos - o.parent.pos) * inverse(o.parent.rotation * inverse(pr))) - p
				append keys (Key k.time v)
			)
			return keys
		)
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn RotationKeys o =
	(
		local keys = #()
		
		local c = o.rotation.controller
		
		local r = o.rotation
		local pr = (quat 0 0 0 1)
		if o.parent != undefined then pr = o.parent.rotation
		
		case (classOf c) of
		(
		tcb_rotation: (
			for k in c.keys do
			(
				at time k.time local v = o.rotation * inverse(r)
				if o.parent != undefined then at time k.time v = (o.rotation * inverse(r)) / (o.parent.rotation * inverse(pr))
				append keys (TanKey k.time v undefined undefined)
			)
--			keys[1].intan = keys[1].outtan = keys[1].value
--			keys[keys.count].intan = keys[keys.count].outtan = keys[keys.count].value

			-- Jul's smooth rotation code ( Catmull-Rom Spline - T = C = B = 0 )
			keys[1].inTan = quat 0 0 0 1
			keys[1].outTan = qcompA (keys[1].value as quat) (keys[1].value as quat) (keys[2].value as quat)
			keys[keys.count].inTan = qcompA (keys[keys.count-1].value as quat) (keys[keys.count].value as quat) (keys[keys.count].value as quat)
			keys[keys.count].outTan = quat 0 0 0 1
			
			for k = 2 to (c.keys.count - 1) do
			(
--				local t = (c.keys[k].tension - 25) / 25
--				local c_ = (c.keys[k].continuity - 25) / 25
--				local b = (c.keys[k].bias - 25) / 25
				
--				keys[k].intan = ((QuatMult (QuatSub keys[k + 1].value  keys[k].value) (((1 - t) * (1 + c_) * (1 - b)) / 2)) * \
--					(QuatMult (QuatSub keys[k].value keys[k - 1].value) (((1 - t) * (1 - c_) * (1 + b)) / 2))) * keys[k].value
--				keys[k].outtan = ((QuatMult (QuatSub keys[k + 1].value  keys[k].value) (((1 - t) * (1 - c_) * (1 - b)) / 2)) * \
--					(QuatMult (QuatSub keys[k].value keys[k - 1].value) (((1 - t) * (1 + c_) * (1 + b)) / 2))) * keys[k].value
				
				keys[k].intan = qcompA (keys[k-1].value as quat) (keys[k].value as quat) (keys[k + 1].value as quat)
				keys[k].outtan = keys[k].intan
			)
			return keys
		)
		default: (
			for k in c.keys do 
			(
				at time k.time local v = o.rotation * inverse(r)
				if o.parent != undefined then at time k.time v = (o.rotation * inverse(r)) / (o.parent.rotation * inverse(pr))
				append keys (Key k.time v)
			)
			return keys
		)
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn ScaleKeys o =
	(
		local keys = #()
		
		local c = o.scale.controller
		
		local p = o.scale
		if o.parent != undefined then
		(
			p /= o.parent.scale
			pr = o.parent.rotation
		)
		
		case (classOf c) of
		(
		bezier_scale: (
			for k in c.keys do
			(
				at time k.time local v = o.scale / p
				if o.parent != undefined then at time k.time v = ((o.scale / o.parent.scale) * inverse(o.parent.rotation * inverse(pr))) / p
				append keys (TanKey k.time v (v * k.inTangent) (v * k.outTangent))
			)
			return keys
		)
		tcb_scale: (
			for k in c.keys do 
			(
				at time k.time local v = o.scale / p
				if o.parent != undefined then at time k.time v = ((o.scale / o.parent.scale) * inverse(o.parent.rotation * inverse(pr))) / p
				append keys (TanKey k.time v undefined undefined)
			)
			keys[1].intan = keys[1].outtan = keys[1].value
			keys[keys.count].intan = keys[keys.count].outtan = keys[keys.count].value
			for k = 2 to (c.keys.count - 1) do
			(
				local t = (c.keys[k].tension - 25) / 25
				local c_ = (c.keys[k].continuity - 25) / 25				-- "c" is taken by the position controller
				local b = (c.keys[k].bias - 25) / 25
				
				keys[k].intan = ((((1 - t) * (1 + c_) * (1 - b)) / 2) * (keys[k + 1].value / keys[k].value) * \
					(((1 - t) * (1 - c_) * (1 + b)) / 2) * (keys[k].value / keys[k - 1].value)) * keys[k].value
				keys[k].outtan = ((((1 - t) * (1 - c_) * (1 - b)) / 2) * (keys[k + 1].value / keys[k].value) * \
					(((1 - t) * (1 + c_) * (1 + b)) / 2) * (keys[k].value / keys[k - 1].value)) * keys[k].value
			)
			return keys
		)
		default: (
			for k in c.keys do 
			(
				at time k.time local v = o.scale / p
				if o.parent != undefined then at time k.time v = ((o.scale / o.parent.pos) * inverse(o.parent.rotation * inverse(pr))) / p
				append keys (Key k.time v)
			)
			return keys
		)
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputPositionAnimation o =
	(
		if o.pos.isAnimated then
		(
			
			local keys = PositionKeys o
			
			if (classOf keys[1]) == TanKey then
			(
				swrite ("\tTranslation " + (keys.count as string))
				if (classOf o.pos.controller) == bezier_position then swrite " {\n\t\tBezier,\n"
				else swrite " {\n\t\tHermite,\n"
				for k in keys do swrite ("\t\t" + ((k.time as integer / 5) as string) + \
					": { " + ((k.value.x) as string) + \
					", " + ((k.value.y) as string) + \
					", " + ((k.value.z) as string) + \
					" },\n\t\t\tInTan { " + (k.intan.x as string) + \
					", " + (k.intan.y as string) + \
					", " + (k.intan.z as string) + \
					" },\n\t\t\tOutTan { " + (k.outtan.x as string) + \
					", " + (k.outtan.y as string) + \
					", " + (k.outtan.z as string) + " },\n")
				swrite "\t}\n"
			)
			else
			(
				swrite ("\tTranslation " + (keys.count as string) + " {\n\t\tLinear,\n")
				for k in keys do swrite ("\t\t" + ((k.time as integer / 5) as string) + \
					": { " + (k.value.x as string) + \
					", " + (k.value.y as string) + \
					", " + (k.value.z as string) + " },\n")
				swrite "\t}\n"
			)
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputRotationAnimation o =
	(
		if o.rotation.isAnimated then
		(
		
			local keys = RotationKeys o
			
			if (classOf keys[1]) == TanKey then
			(
				swrite ("\tRotation " + (keys.count as string) + " {\n\t\tHermite,\n")
				for k in keys do swrite (\
					"\t\t" + ((k.time as integer / 5) as string) + \
					": { " + (k.value.x as string) + \
					", " + (k.value.y as string) + \
					", " + (k.value.z as string) + \
					", " + (k.value.w as string) + \
					" },\n\t\t\tInTan { " + (k.intan.x as string) + \
					", " + (k.intan.y as string) + \
					", " + (k.intan.z as string) + \
					", " + (k.intan.w as string) + \
					" },\n\t\t\tOutTan { " + (k.outtan.x as string) + \
					", " + (k.outtan.y as string) + \
					", " + (k.outtan.z as string) + \
					", " + (k.outtan.w as string) + \
					" },\n")
				swrite "\t}\n"
			)
			else
			(
				swrite ("\tRotation " + (keys.count as string) + " {\n\t\tLinear,\n")
				for k in keys do swrite (\
					"\t\t" + ((k.time as integer / 5) as string) + \
					": { " + (k.value.x as string) + \
					", " + (k.value.y as string) + \
					", " + (k.value.z as string) + \
					", " + (k.value.w as string) + \
					" },\n")
				swrite "\t}\n"
			)
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputScaleAnimation o =
	(
		if o.pos.isAnimated then
		(
			
			local keys = ScaleKeys o
			
			if (classOf keys[1]) == TanKey then
			(
				swrite ("\tScaling " + (keys.count as string))
				if (classOf o.pos.controller) == bezier_scale then swrite " {\n\t\tBezier,\n"
				else swrite " {\n\t\tHermite,\n"
				for k in keys do swrite ("\t\t" + ((k.time as integer / 5) as string) + \
					": { " + ((k.value.x) as string) + \
					", " + ((k.value.y) as string) + \
					", " + ((k.value.z) as string) + \
					" },\n\t\t\tInTan { " + (k.intan.x as string) + \
					", " + (k.intan.y as string) + \
					", " + (k.intan.z as string) + \
					" },\n\t\t\tOutTan { " + (k.outtan.x as string) + \
					", " + (k.outtan.y as string) + \
					", " + (k.outtan.z as string) + " },\n")
				swrite "\t}\n"
			)
			else
			(
				swrite ("\tScaling " + (keys.count as string) + " {\n\t\tLinear,\n")
				for k in keys do swrite ("\t\t" + ((k.time as integer / 5) as string) + \
					": { " + (k.value.x as string) + \
					", " + (k.value.y as string) + \
					", " + (k.value.z as string) + " },\n")
				swrite "\t}\n"
			)
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputVisibilityAnimation o =
	(
		if o.visibility.isAnimated then
		(
			c = o.visibility.controller
			swrite ("\tVisibility " + (c.keys.count as string) + " {\n\t\tDontInterp,\n")
			at time k.time for k in c.keys do swrite ("\t\t" + ((k.time as integer / 5) as string) + \
				": " + (if o.visibility then "1" else "0") + ",\n")
		    swrite "\t}\n"
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputNoteTrackAnimation o =
	(
		if o.Note Track.isAnimated then
		(
			c = o.Note Track.controller
			swrite ("\tEventTrack " + (c.keys.count as string) + "{\n")
			at time k.time for k in c.keys do swrite ("\t\t" + ((k.time as integer / 5) as string) + ",\n")
		    swrite "\t}\n"
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn GetVertFaces m v =
	(
		local adj = #()
		for i = 1 to m.numfaces do
		(
			local f = getFace m i
			for j = 1 to 3 do if f[j] == v then append adj #(i, j)
		)
		return adj
	)
----------------------------------------------------------------------------------------------------------------------------
	fn GetCamera =
	(
		for i in sel do if IsCamera i then return i
		return undefined
	)
----------------------------------------------------------------------------------------------------------------------------
	fn UniqueID =
	(
		idcount += 1
		return idcount
	)
----------------------------------------------------------------------------------------------------------------------------
	fn BuildObject o type ex =
	(
		local index = undefined     -- check if object already exists
		for i = 1 to MDLObjects.count do
		(
			if MDLObjects[i].o == o then
			(
				index = i
				exit
			)
		)
		if index != undefined then
		(
			if MDLObjects[index].type == 1 then			-- if object is a bone make changes
			(
				if ex != undefined then
				(
					if ex.geosid != undefined then
					(
						if MDLObjects[index].ex.geosid == undefined then MDLObjects[index].ex.geosid = ex.geosid
						else if MDLObjects[index].ex.geosid != ex.geosid then MDLObjects[index].ex.geosid = "Multiple"
					)
					if ex.geoaid != undefined then
					(
						if MDLObjects[index].ex.geoaid == undefined or MDLObjects[index].ex.geoaid == "None" then \
							MDLObjects[index].ex.geoaid = ex.geoaid
					)
				)
			)
			else if ex != undefined then		-- if its a not a bone, but should be, make it so
			(
				MDLObjects[index].type = 1
				MDLObjects[index].ex = ex
			)
			return MDLObjects[index].objid
		)
		else
		(
			local par = undefined
			if o.parent != undefined then par = BuildObject o.parent 2 undefined
			append MDLObjects (MDLObj o (UniqueID()) par type ex)
			return MDLObjects[MDLObjects.count].objid
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn DefaultBone ex =
	(
		if DefaultMDLBone == undefined then			-- not created yet
		(
			DefaultMDLBone = (MDLObj undefined (UniqueID()) undefined 1 ex)
			return DefaultMDLBone.objid
		)
		else										-- already exists
		(
			if ex != undefined then
			(
				if ex.geosid != undefined then
				(
					if DefaultMDLBone.ex.geosid == undefined then DefaultMDLBone.ex.geosid = ex.geosid
					else if DefaultMDLBone.ex.geosid != ex.geosid then DefaultMDLBone.ex.geosid = "Multiple"
				)
				if ex.geoaid != undefined then
				(
					if DefaultMDLBone.ex.geoaid == undefined or DefaultMDLBone.ex.geoaid == "None" then \
						DefaultMDLBone.ex.geoaid = ex.geoaid
				)
			)
			return DefaultMDLBone.objid
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	struct Geoset (
		o = undefined,					-- mesh object
		verts = #(),					-- array of point3s
		normals = #(),
		tverts = #(),
		matrixindices = #(),
		faces = #(),
		matrices = #(),
		alpha = #(),
		geosid = 0,
		geoaid = 0,
		fn CopyMesh m =					-- sets the BreakMesh to mesh m
		(
			if (IsMesh m) then
			(
				o = m
				local tex = (getNumTVerts m) > 0
				local s = undefined
				if IsSkinned m then
				(
					s = m.skin
					modPanel.setCurrentObject s
				)
				
				for f = 1 to (getNumFaces m) do append faces (getFace m f)
				
				if m.visibility.isAnimated then for k in m.visibility.controller.keys do append alpha (Key k.time k.value)
				
				for v = 1 to m.numverts do
				(
				
					-- matrix finding
					local matrixindex = undefined
					local bonegroup = #()
					if s != undefined and ((skinOps.GetVertexWeightCount s v) > 0) then
					(
						local bonenodes = #()
						local weights = #()
						for w = 1 to (skinOps.GetVertexWeightCount s v) do
						(
							append bonenodes (getNodeByName (skinOps.GetBoneName s (skinOps.GetVertexWeightBoneID s v w) 0) exact:true)
							append weights #((skinOps.GetVertexWeight s v w), w)
						)
						
						fn cmp a b = (if a[1] < b[1] then return -1 else if a[1] > b[1] then return 1 else return 0)
						qsort weights cmp
						
						while (weights[1][1] * 2) < weights[(mod 1 weights.count) + 1][1] do
						(
							bonenodes[weights[1][2]] = undefined
							deleteItem weights 1
						)
						
						for o in bonenodes do if o != undefined then append bonegroup (BuildObject o 1 (BoneEx geosid geoaid))
						
						sort bonegroup
					)
					else bonegroup = #(DefaultBone (BoneEx geosid geoaid))
					
					local found = ArrayFindItem matrices bonegroup
					if found == 0 then
					(
						append matrices bonegroup
						matrixindex = matrices.count - 1
					)
					else matrixindex = found - 1
					
					if tex then
					(
						-- UV breaking
					 	local facegroups = #()
						adj = GetVertFaces m v
						for f in adj do
						(
							tv = (getTVFace m f[1])[f[2]]
							local new = true
							for fg in facegroups do
							(
								if tv == fg[1] then
								(
									append fg f
									new = false
									exit
								)
							)
							if new == true then append facegroups #(tv, f)
						)
						for fg in facegroups do
						(
							append verts (getVert m v)
							append normals (getNormal m v)
							append tverts (getTVert m (getTVFace m fg[2][1])[fg[2][2]])
							append matrixindices matrixindex
							for f = 2 to fg.count do faces[fg[f][1]][fg[f][2]] = verts.count
						)
					)
					else
					(
						adj = GetVertFaces m v
						if adj.count > 0 then
						(
							append verts (getVert m v)
							append normals (getNormal m v)
							append tverts [0,0]
							append matrixindices matrixindex
							for f in adj do faces[f[1]][f[2]] = verts.count
						)
					)
				)
			)
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn GetMeshes =
	(
		local rmeshes = #()
		max modify mode
		a = 0
		for i = 1 to sel.count do
		(
			if IsMesh sel[i] then
			(
				local geos = (Geoset sel[i])
				geos.geosid = (i - 1) as string
				geos.geoaid = "None"
				if sel[i].visibility.isAnimated then
				(
					geos.geoaid = a
					a += 1
				)
				geos.CopyMesh sel[i]
				append rmeshes geos
			)
		)
		return rmeshes
	)
----------------------------------------------------------------------------------------------------------------------------
	fn InitializeMDLObjects =
	(
		-- Bone and helper gathering   Done by Geoset::CopyMesh
		-- Attachment gathering
		for o in sel do if IsAttachment o then BuildObject o 3 undefined
		-- Particle gathering
		for o in sel do if IsParticleEmitter o then BuildObject o 4 undefined
		-- Light gathering
		for o in sel do if IsLight o then BuildObject o 5 undefined
		-- EventObject gathering
		for o in sel do if IsEventObject o then BuildObject o 6 undefined
		-- ColitionShape gathering
		for o in sel do if IsCollisionShape o then BuildObject o 7 undefined
		-- Add Other Object Gathering Here
		-- Configure Default Bone
		if DefaultMDLBone != undefined then
		(
			DefaultMDLBone.o = box prefix:"default_bone"
			append MDLObjects DefaultMDLBone
		)
		qsort MDLObjects CompareMDLObjects
	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputExtents arr start =			-- inaccurate but quick bounds finding
	(
		if arr.count > 0 then
		(
			oldtime = sliderTime
			sliderTime = start

			local oldsel = selection as array
			clearSelection()
			for i in arr do selectmore i.o
			swrite ("\tMinimumExtent { " + (selection.min.x as string) + ", " + (selection.min.y as string) + \
				", " + (selection.min.z as string) + " },\n")
			swrite ("\tMaximumExtent { " + (selection.max.x as string) + ", " + (selection.max.y as string) + \
				", " + (selection.max.z as string) + " },\n")
			if (dot selection.max selection.max) > (dot selection.min selection.min) then \
				swrite ("\tBoundsRadius " + ((length selection.max) as string) + ",\n")
			else swrite ("\tBoundsRadius " + ((length selection.min) as string) + ",\n")
			clearSelection()
			for i in oldsel do selectmore i
			sliderTime = oldtime
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputMDLHeader name comments =
	(
		if comments != "" then
		(
			print comments to:listener
			commentstemp = filterstring comments "\x0a\x0d"
			for e in commentstemp do print e to:listener
			for e in commentstemp do (if e != "" then swrite ("// " + e + "\n"))
		)
		swrite ("Version {\n\tFormatVersion 800,\n}\nModel \"" + (name as string) + "\" {\n")
		if meshes.count > 0 then swrite ("\tNumGeosets " + (meshes.count as string) + ",\n")
		local n = 0
		for i in meshes do if i.alpha.count > 0 then n +=1
		if n > 0 then swrite ("\tNumGeosetAnims " + (n as string) + ",\n")
		local numobjs = #(0, 0, 0, 0, 0, 0)
		for i in MDLObjects do if i.type > 0 and i.type < 7 then numobjs[i.type] += 1
		if numobjs[2] > 0 then swrite ("\tNumHelpers " + (numobjs[2] as string) + ",\n")
		if numobjs[5] > 0 then swrite ("\tNumLights " + (numobjs[5] as string) + ",\n")
		if numobjs[1] > 0 then swrite ("\tNumBones " + (numobjs[1] as string) + ",\n")
		if numobjs[3] > 0 then swrite ("\tNumAttachments " + (numobjs[3] as string) + ",\n")
		if numobjs[4] > 0 then swrite ("\tNumParticleEmitters " + (numobjs[4] as string) + ",\n")
		if numobjs[6] > 0 then swrite ("\tNumEvents " + (numobjs[6] as string) + ",\n")
		swrite ("\tBlendTime 150,\n")
		OutputExtents meshes animationRange.start
		swrite "}\n"
	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputMDLSequences animitems =
	(
		if animitems.count > 0 then
		(
			swrite ("Sequences " + (animitems.count as string) + " {\n")
			for a in animitems do
			(
				local fs = filterString a "/"
				swrite ("\tAnim \"" + fs[1] + "\" {\n")
				swrite ("\t\tInterval { " + fs[2] + ", " + fs[3] + " },\n")
				OutputExtents meshes (fs[2] as integer)
				swrite "\t}\n"
			)
			swrite "}\n"
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputMDLTexturesMaterials =
	(
		swrite ("Textures 1 {\n\tBitmap {\n\t\tImage \"\",\n\t\tReplaceableId 1,\n\t}" + \
			"\n}\nMaterials 1 {\n\tMaterial {\n\t\tLayer " + \
			"{\n\t\t\tFilterMode None,\n\t\t\tstatic TextureID 0,\n\t	}\n\t}\n}\n")
	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputMDLGeosets animitems =
	(
		for i = 1 to meshes.count do
		(
			m = meshes[i]
			swrite ("Geoset " + " {\n")
			local numverts = m.verts.count
			
			swrite ("\tVertices " + (numverts as string) + " {\n")
			for v in m.verts do swrite ("\t\t{ " + (v.x as string) + ", " + (v.y as string) + ", " + (v.z as string) + " },\n")
			swrite "\t}\n"
			
			swrite ("\tNormals " + (numverts as string) + " {\n")
			for v in m.normals do swrite ("\t\t{ " + (v.x as string) + ", " + (v.y as string) + ", " + (v.z as string) + " },\n")
			swrite "\t}\n"
			
			swrite ("\tTVertices " + (numverts as string) + " {\n")
			for v in m.tverts do swrite ("\t\t{ " + (v.x as string) + ", " + ((1 - v.y) as string) + " },\n")
			swrite "\t}\n"
			
			swrite "\tVertexGroup {\n"
			for v in m.matrixindices do swrite ("\t\t" + (v as string) + ",\n")
			swrite "\t}\n"
			
			swrite ("\tFaces 1 " + ((m.faces.count * 3) as string) + " {\n")
			swrite ("\t\tTriangles {\n\t\t\t{ " + ((m.faces[1].x as integer - 1) as string) + ", " + \
				((m.faces[1].y as integer - 1) as string) + \
				", " + ((m.faces[1].z as integer - 1) as string))
			for f = 2 to m.faces.count do swrite (", " + ((m.faces[f].x as integer - 1) as string) + ", " + \
				((m.faces[f].y as integer - 1) as string) + ", " + ((m.faces[f].z as integer - 1) as string))
			swrite " },\n\t\t}\n\t}\n"
			
			local total = 0
			for a in m.matrices do total += a.count
			swrite ("\tGroups " + (m.matrices.count as string) + " " + (total as string) + " {\n")
			for g = 1 to m.matrices.count do
			(
				swrite "\t\tMatrices { "
				for b = 1 to (m.matrices[g].count - 1) do swrite ((m.matrices[g][b] as string) + ", ")
				swrite (m.matrices[g][m.matrices[g].count] as string)
				swrite " },\n"
			)
			swrite "\t}\n"
			
			OutputExtents #(m) animationRange.start
			for a in animitems do
			(
				swrite "\tAnim {\n"
				local fs = filterString a "/"
				OutputExtents #(m) (fs[2] as integer)
				swrite "\t}\n"
			)
			swrite "\tMaterialID 0,\n\tSelectionGroup 0,\n}\n"
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputMDLGeosetAnims =
	(
		for m in meshes do
		(
			if m.alpha.count > 0 then
			(
				swrite ("GeosetAnim {\n\tAlpha " + (m.alpha.count as string) + " {\n\t\tDontInterp,\n")
				for k in m.alpha do swrite ("\t\t" + ((k.time as integer / 5) as string) + \
					": " + (k.value as string) + ",\n")
				swrite ("\t}\n\tGeosetId " + (m.geosid as string) + ",\n}\n")
			)
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputMDLBones =
	(
		for o in MDLObjects do
		(
			if o.type == 1 then
			(
				swrite ("Bone \"" + o.o.name + "\" {\n\tObjectId " + (o.objid as string) + ",\n")
				if o.parent != undefined then swrite ("\tParent " + (o.parent as string) + ",\n")
				swrite ("\tGeosetId " + (o.ex.geosid as string) + ",\n\tGeosetAnimId " + (o.ex.geoaid as string) + ",\n")
				OutputPositionAnimation o.o
				OutputRotationAnimation o.o
				OutputScaleAnimation o.o
				swrite "}\n"
			)
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputMDLLights =
	(
		for o in MDLObjects do
		(
			if o.type == 5 then
			(
				local color = (o.o.color as point3) / 255
				local ambcolor = (ambientColor as point3) / 255
				swrite ("Light \"" + o.o.name + "\" {\n\tObjectId " + (o.objid as string) + ",\n")
				if o.parent != undefined then swrite ("\tParent " + (o.parent as string) + ",\n")
				swrite ("\tOmnidirectional,\n\tstatic AttenuationStart " + (o.o.farAttenStart as string) + \
					",\n\tstatic AttenuationEnd " + (o.o.farAttenEnd as string) + \
					",\n\tstatic Intensity " + (o.o.multiplier as string) + \
					",\n\tstatic Color { " + (color.x as string) + \
					", " + (color.y as string) + \
					", " + (color.z as string) + \
					" },\n\tstatic AmbIntensity " + (lightLevel as string) + \
					",\n\tstatic AmbColor { " + (ambcolor.x as string) + \
					", " + (ambcolor.y as string) + \
					", " + (ambcolor.z as string) + " },\n")
				OutputVisibilityAnimation o.o
				OutputPositionAnimation o.o
				OutputRotationAnimation o.o
				OutputScaleAnimation o.o
				swrite "}\n"
			)
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputMDLHelpers =
	(
		for o in MDLObjects do
		(
			if o.type == 2 then
			(
				swrite ("Helper \"" + o.o.name + "\" {\n\tObjectId " + (o.objid as string) + ",\n")
				if o.parent != undefined then swrite ("\tParent " + (o.parent as string) + ",\n")
				OutputPositionAnimation o.o
				OutputRotationAnimation o.o
				OutputScaleAnimation o.o
				swrite "}\n"
			)
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputMDLAttachments =
	(
		for o in MDLObjects do
		(
			if o.type == 3 then
			(
				swrite ("Attachment \"" + (substring o.o.name 5 o.o.name.count) + "\" {\n\tObjectId " + (o.objid as string) + ",\n")
				if o.parent != undefined then swrite ("\tParent " + (o.parent as string) + ",\n")
				OutputPositionAnimation o.o
				OutputRotationAnimation o.o
				OutputScaleAnimation o.o
				OutputVisibilityAnimation o.o
				swrite "}\n"
			)
		)

	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputMDLPivotPoints =
	(
		if MDLObjects.count > 0 then
		(
			swrite ("PivotPoints " + (MDLObjects.count as string) + " {\n")
			for o in MDLObjects do
			(
				swrite ("\t{ " + (o.o.pos.x as string) + \
					", " + (o.o.pos.y as string) + \
					", " + (o.o.pos.z as string) + \
					" },\n")
			)
			swrite "}\n"
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputMDLParticleEmitter =
	(
		for o in MDLObjects do
		(
			if o.type == 4 then
			(
				 swrite ("ParticleEmitter \"" + o.o.name + "\" {\n\tObjectId " + (o.objid as string) + ",\n")
				 if o.parent != undefined then swrite ("\tParent " + (o.parent as string) + ",\n")
				 swrite ("    EmitterUsesTGA" + \
				     ",\n\tstatic EmissionRate " + (o.o.Birth_Rate as string) + \
				     ",\n\tstatic Gravity 0" + \
				     ",\n\tstatic Longitude " + (o.o.Emitter_Width as string) + \
				     ",\n\tstatic Latitude " + (o.o.Emitter_Length as string) + ",\n")
				 OutputVisibilityAnimation o.o
				 swrite ("    Particle { " + \
				     " \n\t        static LifeSpan " + (o.o.life as string) + \
				     ",\n\t        static InitVelocity " + (o.o.speed as string) + \
				     ",\n\t        Path " + "n\t\tTextures\White.blp" + ",\n")
				 OutputPositionAnimation o.o
				 OutputRotationAnimation o.o
				 OutputScaleAnimation o.o
				 swrite "}\n"
			)
		)
	)			
----------------------------------------------------------------------------------------------------------------------------
	fn OutputMDLEventObjects =
	(
		for o in MDLObjects do
		(
			if o.type == 6 then
			(
			swrite ("EventObject \"" + (substring o.o.name 5 o.o.name.count) + "\" {\n\tObjectId " + (o.objid as string) + ",\n")
			if o.parent != undefined then swrite ("\tParent " + (o.parent as string) + ",\n")
			OutputPositionAnimation o.o
			OutputRotationAnimation o.o
			OutputScaleAnimation o.o
			OutputVisibilityAnimation o.o
			swrite "}\n"
			)
		)
	)	
----------------------------------------------------------------------------------------------------------------------------
	fn OutputMDLCollisionShape =
	(
		for o in MDLObjects do
		(
			if o.type == 7 then
			(
				if o == Box then
				(
				swrite ("CollisionShape \"" + o.name + "\" {\n\tObjectId " + (o.objid as string) + \
					",\n\tBox,\n\tVertices 2 {\n\t\t{ " + (o.min.x as string) + \
					", " + (o.min.y as string) + \
					", " + (o.min.z as string) + \
					" },\n\t\t{ " + (o.max.x as string) + \
					", " + (o.max.y as string) + \
					", " + (o.max.z as string) + " },\n")
				)
				else if classOf o == Sphere or classOf o == GeoSphere then
				(
				swrite ("CollisionShape \"" + o.name + "\" {\n\tObjectId " + (o.objid as string) + \
					",\n\tSphere,\n\tVertices 1 {\n\t\t{ " + (o.pos.x as string) + \
					", " + (o.pos.y as string) + \
					", " + (o.pos.z as string) + \
					" },\n\t}\n\tBoundsRadius " + (o.radius as string) + \
					",\n}\n")
				)
			)
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputMDLObjects =
	(
		OutputMDLBones()
		OutputMDLLights()
		OutputMDLHelpers()
		OutputMDLAttachments()
		OutputMDLPivotPoints()
		OutputMDLParticleEmitter()
		OutputMDLEventObjects()
		OutputMDLCollisionShape()
	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputMDLCamera =
	(
		if cam != undefined then
		(
			swrite ("Camera \"" + cam.name + \	
				"\" {\n\tPosition { " + (cam.pos.x as string) + \
				", " + (cam.pos.y as string) + \
				", " + (cam.pos.z as string) + \
				" },\n")
			OutputPositionAnimation cam
			swrite (\
				"\tFieldOfView " + (cam.fov as string) + \
				",\n\tFarClip " + (cam.farclip as string) + \
				",\n\tNearClip " + (cam.nearclip as string) + \
				",\n\tTarget {\n\t\tPosition { " + (cam.target.pos.x as string) + \
				", " + (cam.target.pos.y as string) + \
				", " + (cam.target.pos.z as string) + \
				" },\n")
			OutputPositionAnimation cam.target
			swrite "\t}\n}\n"
		)
	)
----------------------------------------------------------------------------------------------------------------------------
	fn OutputMDL name comments animitems =
	(
		disableSceneRedraw()
		
		ResetGlobals()
		
		local newcom = ("'DeX Version: " + dexversion + "\n" + comments)
		
		local prevsettings = #()
		
		prevsettings[1] = animationRange
		prevsettings[2] = sliderTime
		
		if animitems.count > 0 then animationRange = interval ((filterString animitems[1] "/")[2] as integer) \
				((filterString animitems[animitems.count] "/")[3] as integer)
		else animationRange = interval 0 1
		sliderTime = animationRange.start
		
		prevsettings[3] = framerate
		framerate = 960
		
--		prevsettings[4] = set coordsys (rotateZMatrix -90)
		
		sel = selection as array
		
		meshes = GetMeshes()
		cam = GetCamera()

		InitializeMDLObjects()
		OutputMDLHeader name newcom
		OutputMDLSequences animitems
		OutputMDLTexturesMaterials()
		OutputMDLGeosets animitems
		OutputMDLGeosetAnims()
		OutputMDLObjects()
		OutputMDLCamera()
		
		if DefaultMDLBone != undefined then if DefaultMDLBone.o != undefined then delete DefaultMDLBone.o
		
		animationRange = prevsettings[1]
		sliderTime = prevsettings[2]
		framerate = prevsettings[3]
--		set coordsys prevsettings[4]
		
		clearSelection()
		for o in sel do selectMore o
		
		format "Export Successful!\n" to:listener
		
		enableSceneRedraw()
	)
----------------------------------------------------------------------------------------------------------------------------
----------------------------------------
-- End Function and global definitions--
----------------------------------------

------------------
--UI definitions--
------------------
	if mt_floater != undefined then closeRolloutFloater mt_floater
	rollout mt_exportparams "Export Settings" width:320 height:224
	(
		GroupBox grp1 "General Info" pos:[8,8] width:148 height:160
		edittext model_name "" pos:[16,40] width:128 height:17
		GroupBox grp2 "Animation" pos:[168,8] width:144 height:160
		listbox anims "Animations" pos:[176,27] width:128 height:6
		button add_anim "Add" pos:[192,145] width:39 height:15 across:2
		button delete_anim "Delete" pos:[248,144] width:39 height:16
		button load_settings "Load" pos:[112,192] width:41 height:16
		button save_settings "Save" pos:[168,192] width:41 height:16
		GroupBox grpsettings "Settings" pos:[88,176] width:144 height:40
		edittext edtComments "" pos:[16,88] width:128 height:54
		label lblcomments "Comments" pos:[20,72] width:80 height:16
		label lbl5 "Model Name" pos:[16,24] width:128 height:16
	--------------------------------------------------------------------------------	
	--mt_exportparams events
	
		on add_anim pressed do
		(
			rollout add_anim_rollout "Add Animation"
			(
				edittext add_anim_name "Name" pos:[10, 5]
				spinner add_anim_start "Start" range:[0, 1000000000, 0] type:#integer pos:[20, 27]
				spinner add_anim_end "End" range:[0, 1000000000, framerate] type:#integer pos:[23, 48]
				button add_anim_go "Add" pos:[39, 69]
				on add_anim_go pressed do
				(
					if add_anim_start.value < add_anim_end.value then 
					(
						local s = stringStream ""
						tempstart	= (((add_anim_start.value as float)/framerate)*960) as integer
						tempend		= (((add_anim_end.value as float)/framerate)*960) as integer
						format "%/%/%" add_anim_name.text (tempstart as string) (tempend as string) to:s
						local temparr = mt_exportparams.anims.items
						append temparr (s as string)
						mt_exportparams.anims.items = temparr
					)
					else messageBox "Start time must be less than end time!"
				)
			)
			CreateDialog add_anim_rollout pos:[300, 300] width:200 height:100
		)
		on delete_anim pressed do
		(
			if mt_exportparams.anims.selection > 0 then
			(
				local temparr = mt_exportparams.anims.items
				deleteItem temparr mt_exportparams.anims.selection
				mt_exportparams.anims.items = temparr
			)
		)
		on load_settings pressed do
		(
			if fileproperties.getnumproperties #custom > 0 then
			(
				start = fileproperties.findproperty #custom "'DeX-version"
				if start != 0 then
				(
					animcount = fileproperties.getpropertyvalue #custom (start+1)
					mt_exportparams.model_name.text = fileproperties.getpropertyvalue #custom (start + 2)
					mt_exportparams.edtcomments.text = fileproperties.getpropertyvalue #custom (start + 3)
					temp = #()
					for i = 1 to animcount do
					(
						append temp (fileproperties.getpropertyvalue #custom (start + 3 + i))
					)
					mt_exportparams.anims.items = temp
				)
			)
		)
		on save_settings pressed do
		(
			if fileproperties.getnumproperties #custom > 0 then
			(
				start = fileproperties.findproperty #custom "'DeX-version"
				animcount = fileproperties.getpropertyvalue #custom (start+1)
				for i = 1 to animcount do fileproperties.deleteproperty #custom ("'DeX-animation" + (i as string))
				
				fileproperties.addproperty #custom "'DeX-version" dexversion
				fileproperties.addproperty #custom "'DeX-MDL animation count" mt_exportparams.anims.items.count
				fileproperties.addproperty #custom "'DeX-model name" mt_exportparams.model_name.text
				fileproperties.addproperty #custom "'DeX-MDL comment" mt_exportparams.edtcomments.text
				
				for i = 1 to mt_exportparams.anims.items.count do fileproperties.addproperty #custom ("'DeX-animation" + (i as string)) \
					mt_exportparams.anims.items[i]
			)
			else
			(
				fileproperties.addproperty #custom "'DeX-version" dexversion
				fileproperties.addproperty #custom "'DeX-MDL animation count" mt_exportparams.anims.items.count		
				fileproperties.addproperty #custom "'DeX-model name" mt_exportparams.model_name.text
				fileproperties.addproperty #custom "'DeX-MDL comment" mt_exportparams.edtcomments.text
				
				for i = 1 to mt_exportparams.anims.items.count do fileproperties.addproperty #custom ("'DeX-animation" + (i as string)) \
					mt_exportparams.anims.items[i]
			)
		)
	)	
	
	rollout mt_go "Export" width:320 height:109
	(
		GroupBox grp1 "Go!" pos:[80,7] width:154 height:98
		button ExpMDL "Export To File" pos:[117,26] width:81 height:21
		button MemMDL "Write to Memory" pos:[111,52] width:93 height:21
		button PrintMDL "Print to Listener" pos:[113,78] width:89 height:21
		
		on ExpMDL pressed do
		(
			local outfile_name = undefined
			if isgmax() then messagebox "gmax users must export using Write to Memory or Print to Listener."
			else outfile_name = getSaveFileName types:"MDL File (*.mdl)|*.mdl|All Files (*.*)|*.*|"
			if outfile_name != undefined then
			(
				streamtype = 1
				gc()
				global foutstream = createfile outfile_name					
				
				OutputMDL mt_exportparams.model_name.text mt_exportparams.edtComments.text mt_exportparams.anims.items

				close foutstream
				edit outfile_name
			)
		)--on close
		on MemMDL pressed do
		(
			messagebox "This feature is not yet implemented."
--			global streamtype = 3
--			OutputMDL mt_exportparams.model_name.text mt_exportparams.edtComments.text mt_exportparams.anims.items
--			numberMemMDL()
--			for i = 1 to MemoryMDL.count do MemoryMDL[i] = MemoryMDL[i] as stringstream
--			gc()
--			messagebox "Run MDL Grabber and click Ok once memory grab is succesful." title: "Write to memory succesful"
		)--on close
		on PrintMDL pressed do
		(
			global streamtype = 2
			if (querybox "All information currently in the listener will be cleared.") == true then
			(
				clearlistener()
				OutputMDL mt_exportparams.model_name.text mt_exportparams.edtComments.text mt_exportparams.anims.items
			)
		)--on close
	)
	rollout mt_about "About" width:320 height:250
	(
		bitmap b1 "" pos:[8,8] width:150 height:154 fileName:"DeX.jpg"
		label lbl14 "'DeX, MDL exporter for the Warcraft 3 MDL Toolkit www.wc3campaigns.com" pos:[168,8] width:144 height:40
		label lbl15 "Original Author: Max Crane (aka republicola)\nModifications written by William Talmadge\n(aka CitizenSnips)\nand Julien Lardy\n(aka Jul) and Blinkboy" \
			pos:[168,54] width:144 height:92
		label lbl16 "MDL editing gurus that made this script possible:\nCookie,Oinkerwinkle , Drax, Kdub, KMK\nIf I've left someone out please let me know." \
			pos:[168,148] width:144 height:64
	)
	mt_floater = newrolloutfloater ("'DeX " + dexversion) 330 630
	addrollout mt_exportparams mt_floater
	addrollout mt_go mt_floater
	addrollout mt_about mt_floater

----------------------
--End UI definitions--
----------------------
)--macroscript close


macros.run DeX
--End of script