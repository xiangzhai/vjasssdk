set(wc3lib_MDLX_SRC
	alpha2s.cpp                                     
	alpha.cpp
	alphas.cpp                                      
	attachment.cpp
	attachments.cpp
	bone.cpp
	bones.cpp
	camera.cpp
	cameras.cpp
	collisionshape.cpp
	collisionshapes.cpp
	event.cpp
	events.cpp
	ganimation.cpp
	geosetanimation.cpp
	geosetanimations.cpp
	geoset.cpp
	geosets.cpp
	globalsequence.cpp
	globalsequences.cpp
	helper.cpp
	helpers.cpp
	layer.cpp
	light.cpp
	lights.cpp
	material.cpp
	materials.cpp
	mdlx.cpp
	mdxblock.cpp
	model.cpp
	normal.cpp
	object.cpp
	particleemitter2.cpp
	particleemitter2s.cpp
	particleemitter.cpp
	particleemitters.cpp
	pivotpoint.cpp
	pivotpoints.cpp
	ribbonemitter.cpp
	ribbonemitters.cpp
	rotation.cpp
	scaling.cpp
	scalings.cpp
	segmentcolor.cpp
	sequence.cpp
	sequences.cpp
	textureanimation.cpp
	textureanimations.cpp
	texture.cpp
	textures.cpp
	texturevertex.cpp
	translation0.cpp
	translation0s.cpp
	version.cpp
	vertex.cpp
	visibility0.cpp
	visibility1.cpp
	visibility2.cpp
	visibility3.cpp
	visibility4.cpp
)

set(wc3lib_MDLX_H
	attachment.hpp
	bone.hpp
	camera.hpp
	collisionshape.hpp
	event.hpp
	ganimation.hpp
	geoset.hpp
	geosetanimation.hpp
	mdlx.hpp
	object.hpp
	visibility0.hpp
	visibility1.hpp
	visibility2.hpp
	visibility3.hpp
	visibility4.hpp
)

add_library(wc3libmdlx SHARED ${wc3lib_MDLX_SRC})
#target_link_libraries(wc3libblp gettextlib)

#install

install(TARGETS wc3libmdlx DESTINATION lib/wc3lib/)
install(FILES ${wc3lib_MDLX_H} PUBLIC_HEADER DESTINATION lib/wc3lib/mdlx)