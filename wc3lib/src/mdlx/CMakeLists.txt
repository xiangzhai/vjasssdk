set(wc3lib_MDLX_SRC                            
	alpha.cpp
	alphas.cpp
	ambientcolor.cpp
	ambientcolors.cpp
	ambientintensities.cpp
	ambientintensity.cpp
	attachment.cpp
	attachments.cpp
	bone.cpp
	bones.cpp
	camera.cpp
	cameras.cpp
	collisionshape.cpp
	collisionshapes.cpp
	color0.cpp
	color0s.cpp
	color1.cpp
	color1s.cpp
	emissionrate.cpp
	emissionrates.cpp
	event.cpp
	events.cpp
	ganimation.cpp
	geoset.cpp
	geosetanimation.cpp
	geosetanimations.cpp
	geosets.cpp
	globalsequence.cpp
	globalsequences.cpp
	groupvertex.cpp
	groupvertices.cpp
	helper.cpp
	helpers.cpp
	intensities.cpp
	intensity.cpp
	kgao.cpp
	layer.cpp
	layers.cpp
	lengths.cpp
	light.cpp
	lights.cpp
	material.cpp
	materials.cpp
	matrices.cpp
	matrix.cpp
	matrixgroupcount.cpp
	matrixgroupcounts.cpp
	mdlx.cpp
	mdxblock.cpp
	model.cpp
	normal.cpp
	normals.cpp
	object.cpp
	particleemitter2.cpp
	particleemitter2s.cpp
	particleemitter.cpp
	particleemitters.cpp
	pivotpoint.cpp
	pivotpoints.cpp
	primitivesize.cpp
	primitivesizes.cpp
	primitivetype.cpp
	primitivetypes.cpp
	primitivevertex.cpp
	primitivevertices.cpp
	ribbonemitter.cpp
	ribbonemitters.cpp
	rotation0.cpp
	rotation0s.cpp
	rotation1.cpp
	rotation1s.cpp
	scaling0.cpp
	scaling0s.cpp
	scaling1.cpp
	scaling1s.cpp
	segmentcolor.cpp
	sequence.cpp
	sequences.cpp
	texture.cpp
	textureanimation.cpp
	textureanimations.cpp
	textureid.cpp
	textureids.cpp
	textures.cpp
	texturepatch.cpp
	texturepatches.cpp
	texturevertex.cpp
	texturevertices.cpp
	translation0.cpp
	translation0s.cpp
	translation1.cpp
	translation1s.cpp
	translation2.cpp
	translation2s.cpp
	translation3.cpp
	translation3s.cpp
	version.cpp
	vertex.cpp
	vertices.cpp
	visibility0.cpp
	visibility0s.cpp
	visibility1.cpp
	visibility1s.cpp
	visibility2.cpp
	visibility2s.cpp
	visibility3.cpp
	visibility3s.cpp
	visibility4.cpp
	visibility4s.cpp
)

set(wc3lib_MDLX_H
	attachment.hpp
	ambientcolor.hpp
	ambientcolors.hpp
	ambientintensity.hpp
	ambientintensities.hpp
	bone.hpp
	camera.hpp
	collisionshape.hpp
	color0.hpp
	color0s.hpp
	color1.hpp
	color1s.hpp
	event.hpp
	ganimation.hpp
	geoset.hpp
	geosetanimation.hpp
	mdlx.hpp
	object.hpp
	visibility0.hpp
	visibility0s.hpp
	visibility1.hpp
	visibility1s.hpp
	visibility2.hpp
	visibility2s.hpp
	visibility3.hpp
	visibility3s.hpp
	visibility4.hpp
	visibility4s.hpp
)

#if (BLEND)
	add_subdirectory(readblend)
#endif (BLEND)

# custom include path, Gentoo Linux
include_directories(/usr/include/boost-1_35)

add_library(wc3libmdlx SHARED ${wc3lib_MDLX_SRC})
#target_link_libraries(wc3libblp gettextlib)
#if (BLEND)
	target_link_libraries(wc3libmdlx wc3libmdlxreadblend)
#endif (BLEND)

#install

install(TARGETS wc3libmdlx DESTINATION lib/wc3lib/)
install(FILES ${wc3lib_MDLX_H} PUBLIC_HEADER DESTINATION lib/wc3lib/mdlx)