#include <iostream> //debug
#include <string>
#include <cstring>

#include <boost/tokenizer.hpp>

#include "mdlx.hpp"

namespace wc3lib
{

namespace mdlx
{

/**
* The first 12 bytes of every blend-file is the file-header.
* The file-header has information on Blender (version-number) and the PC the blend-file was saved on (pointer-size and endianness).
* This is required as all data inside the blend-file is ordered in that way, because no translation or transformation is done during saving.
*/
struct BlendHeader
{
	char identifier[7]; ///	File identifier (always 'BLENDER')
	char pointerSize; /// Size of a pointer; all pointers in the file are stored in this format. '_' means 4 bytes or 32 bit and '-' means 8 bytes or 64 bits.
	char endianness; /// Type of byte ordering used; 'v' means little endian and 'V' means big endian.
	char versionNumber[3];	/// Version of Blender the file was created in; '248' means version 2.48
};

struct BlendFileBlock
{
	char code[4]; /// Identifier of the file-block
	int size; /// Total length of the data after the file-block-header
	void *oldMemoryAddress; /// Memory address the structure was located when written to disk
	int sdnaIndex; /// Index of the SDNA structure
	int count; /// Number of structure located in this file-block
};

void Mdlx::readMdx(std::fstream &fstream) throw (class Exception)
{
	this->m_version->readMdx(fstream);
	this->m_model->readMdx(fstream);
	this->m_sequences->readMdx(fstream);
	this->m_globalSequences->readMdx(fstream);
	this->m_materials->readMdx(fstream);
	this->m_textures->readMdx(fstream);
	this->m_textureAnimations->readMdx(fstream);
	this->m_geosets->readMdx(fstream);
	this->m_geosetAnimations->readMdx(fstream);
	this->m_bones->readMdx(fstream);
	this->m_lights->readMdx(fstream);
	this->m_helpers->readMdx(fstream);
	this->m_attachments->readMdx(fstream);
	this->m_pivotPoints->readMdx(fstream);
	this->m_particleEmitters->readMdx(fstream);
	this->m_particleEmitter2s->readMdx(fstream);
	this->m_ribbonEmitters->readMdx(fstream);
	this->m_cameras->readMdx(fstream);
	this->m_events->readMdx(fstream);
	this->m_collisionShapes->readMdx(fstream);
}

void Mdlx::readMdl(std::fstream &fstream) throw (Exception)
{
	this->m_version->readMdl(fstream);
	this->m_model->readMdl(fstream);
	this->m_sequences->readMdl(fstream);
	this->m_globalSequences->readMdl(fstream);
	this->m_materials->readMdl(fstream);
	this->m_textures->readMdl(fstream);
	this->m_textureAnimations->readMdl(fstream);
	this->m_geosets->readMdl(fstream);
	this->m_geosetAnimations->readMdl(fstream);
	this->m_bones->readMdl(fstream);
	this->m_lights->readMdl(fstream);
	this->m_helpers->readMdl(fstream);
	this->m_attachments->readMdl(fstream);
	this->m_pivotPoints->readMdl(fstream);
	this->m_particleEmitters->readMdl(fstream);
	this->m_particleEmitter2s->readMdl(fstream);
	this->m_ribbonEmitters->readMdl(fstream);
	this->m_cameras->readMdl(fstream);
	this->m_events->readMdl(fstream);
	this->m_collisionShapes->readMdl(fstream);
}

void Mdlx::writeMdl(std::fstream &fstream) throw (Exception)
{
	this->writeMdlVersion(fstream);
	this->writeMdlModel(fstream);
	this->writeMdlSequences(fstream);
	this->writeMdlGlobalSequences(fstream);
	this->writeMdlMaterials(fstream);
	this->writeMdlTextureAnimations(fstream);
	this->writeMdlGeoset(fstream);
	this->writeMdlGeosetAnimations(fstream);
	this->writeMdlBones(fstream);
	this->writeMdlHelpers(fstream);
}

void Mdlx::readMdlVersion(std::fstream &fstream) throw (Exception)
{
	std::string line;
	
	while (std::getline(fstream, line))
	{
		boost::tokenizer<> tokenizer(line);
		boost::tokenizer<>::iterator iterator = tokenizer.begin();

		if (iterator == tokenizer.end())
			throw Exception("Syntax error.");

		if ((*iterator) == "//")
			continue;
		else if ((*iterator) == "Version")
		{
			++iterator;

			if ((*iterator) != "{" || iterator == tokenizer.end())
				throw Exception("Syntax error.");
		}
		else if ((*iterator) == "FormatVersion")
		{
			++iterator;

			if (iterator == tokenizer.end())
				throw Exception("Syntax error.");

			std::stringstream sstream;
			sstream << *iterator;
			sstream >> this->m_version;
		}
		else if ((*iterator) == "}")
			break;
	}
}

void Mdlx::writeMdlVersion(std::fstream &fstream) throw (Exception)
{
	fstream << "// Current FormatVersion is 800\n"
	"Version {\n"
	"\tFormatVersion" << this->m_version << ",\n"
	"}\n";
}

void Mdlx::writeMdlModel(std::fstream &fstream) throw (Exception)
{
	fstream << "Model " << this->m_name << "{\n";

	if (this->m_geosets.size() > 0)
		fstream << "\tNumGeosets " << this->m_geosets.size() << ",\n";

	if (this->m_geosetAnimations.size() > 0)
		fstream << "\tNumGeosetAnims " << this->m_geosetAnimations.size() << ",\n";
	
	if (this->m_helpers.size() > 0)
		fstream << "\tNumHelpers " << this->m_helpers.size() << ",\n";
	
	if (this->m_lights.size() > 0)
		fstream << "\tNumLights " << this->m_lights.size() << ",\n";
	
	if (this->m_bones.size() > 0)
		fstream << "\tNumBones " << this->m_bones.size() << ",\n";
	
	if (this->m_attachments.size() > 0)
		fstream << "\tNumAttachements " << this->m_attachments.size() << ",\n";
	
	if (this->m_particleEmitters.size() > 0)
		fstream << "\tNumParticleEmitters " << this->m_particleEmitters.size() << ",\n";

	if (this->m_particleEmitters2.size() > 0)
		fstream << "\tNumParticleEmitters2 " << this->m_particleEmitters2.size() << ",\n";

	if (this->m_ribbonEmitters.size() > 0)
		fstream << "\tNumRibbonEmitters " << this->m_ribbonEmitters.size() << ",\n";

	if (this->m_events.size() > 0)
		fstream << "\tNumEvents " << this->m_events.size() << ",\n";

	fstream << "\tBlendTime " << this->m_blendTime << ",\n";

	if (this->m_minExtX != 0.0 || this->m_minExtY != 0.0 || this->m_minExtZ != 0.0)
		fstream << "MinimumExtent { " << this->m_minExtX << ", " << this->m_minExtY << ", " << this->m_minExtZ << " },\n";

	if (this->m_maxExtX != 0.0 || this->m_maxExtY != 0.0 || this->m_maxExtZ != 0.0)
		fstream << "MaxmimumExtent { " << this->m_maxExtX << ", " << this->m_maxExtY << ", " << this->m_maxExtZ << " },\n";

	if (this->m_boundsRadius != 0.0)
		fstream << "BoundsRadius " << this->m_boundsRadius << ",\n";

	fstream << "}\n";
}

void Mdlx::writeMdlSequences(std::fstream &fstream) throw (Exception)
{
	fstream
	<< "Sequences " << this->m_sequences.size() << " {\n";
	
	for (std::list<class Sequence*>::iterator iterator = this->m_sequences.begin(); iterator != this->m_sequences.end(); ++iterator)
	{
		fstream
		<< "\tAnim " << (*iterator)->name << " {\n"
		<< "\t\tInterval { " << (*iterator)->intervalStart << ", " << (*iterator)->intervalEnd << " },\t"
		;

		if ((*iterator)->noLooping == 1)
			fstream << "\t\tNonLooping,\n";

		if ((*iterator)->moveSpeed != 0.0)
			fstream << "\t\tMoveSpeed " << (*iterator)->moveSpeed << ",\n";
		
		if ((*iterator)->rarity != 0.0)
			fstream << "\t\tRarity " << (*iterator)->rarity << ",\n";

		if ((*iterator)->minExtX != 0.0 || (*iterator)->minExtY != 0.0 || (*iterator)->minExtZ != 0.0)
			fstream << "\t\tMinimumExtent { " << (*iterator)->minExtX << ", " << (*iterator)->minExtY << ", " << (*iterator)->minExtZ << " },\n";
		
		if ((*iterator)->maxExtX != 0.0 || (*iterator)->maxExtY != 0.0 || (*iterator)->maxExtZ != 0.0)
			fstream << "\t\tMaxmimumExtent { " << (*iterator)->maxExtX << ", " << (*iterator)->maxExtY << ", " << (*iterator)->maxExtZ << " },\n";
	
		if ((*iterator)->boundsRadius != 0.0)
			fstream << "\t\tBoundsRadius " << (*iterator)->boundsRadius << ",\n";
	
		fstream << "\t}\n";
	}

	fstream << "}\n";
}

void Mdlx::writeMdlGlobalSequences(std::fstream &fstream) throw (Exception)
{
	fstream << "GlobalSequences " << this->m_globalSequencesDurations.size() << " {\n";
	
	for (std::list<long32>::iterator iterator = this->m_globalSequencesDurations.begin(); iterator != this->m_globalSequencesDurations.end(); ++iterator)
		fstream << "\tDuration " << *iterator << ",\n";

	fstream << "}\n";

	// textures
	fstream << "Textures " << this->m_textures.size() << " {\n";

	for (std::list<struct Texture*>::iterator iterator = this->m_textures.begin(); iterator != this->m_textures.end(); ++iterator)
	{
		fstream
		<< "\tBitmap {\n"
		<< "\t\tImage " << (*iterator)->texturePath << ",\n";

		if ((*iterator)->replaceableId != 0)
			fstream << "\t\tReplaceableId " << (*iterator)->replaceableId << ",\n";

		if ((*iterator)->wrapping == 1 || (*iterator)->wrapping == 3)
			fstream << "\t\tWrapWidth,\n";

		if ((*iterator)->wrapping == 2 || (*iterator)->wrapping == 3)
			fstream << "\t\tWrapHeight,\n";

		fstream << "\t}\n";
	}

	fstream << "}\n";
}

void Mdlx::writeMdlMaterials(std::fstream &fstream) throw (Exception)
{
	fstream << "Materials " << this->m_materials.size() << " {\n";

	for (std::list<struct Material*>::iterator iterator = this->m_materials.begin(); iterator != this->m_materials.end(); ++iterator)
	{
		fstream << "\tMaterial {\n";

		if ((*iterator)->renderMode & 1)
			fstream << "\t\tConstantColor,\n";

		if ((*iterator)->renderMode & 16)
			fstream << "\t\tSortPrimsFarZ,\n";

		if ((*iterator)->renderMode & 32)
			fstream << "\t\tFullResolution,\n";
		
		if ((*iterator)->priorityPlane != 0)
			fstream << "\t\tPriorityPlane " << (*iterator)->priorityPlane << ",\n";

		bool showCoordId = true;

		for (std::list<struct Layer*>::iterator iterator1 = (*iterator)->layers.begin(); iterator1 != (*iterator)->layers.end(); ++iterator1)
		{
			if ((*iterator1)->coordId == 0)
			{
				showCoordId = false;
				break;
			}
		}

		for (std::list<struct Layer*>::iterator iterator1 = (*iterator)->layers.begin(); iterator1 != (*iterator)->layers.end(); ++iterator1)
		{
			fstream << "\t\tLayer {\n"
			<< "\t\t\tFilterMode ";

			switch ((*iterator1)->filterMode)
			{
				case 0:
					fstream << "None";
					break;

				case 1:
					fstream << "Transparent";
					break;

				case 2:
					fstream << "Blend";
					break;

				case 3:
					fstream << "Additive";
					break;

				case 4:
					fstream << "AddAlpha";
					break;

				case 5:
					fstream << "Modulate";
					break;

				case 6:
					fstream << "Modulate2x";
					break;
			}

			fstream << ",\n";

			if ((*iterator1)->shading & 1)
				fstream << "\t\t\tUnshaded,\n";

			if ((*iterator1)->shading & 2)
				fstream << "\t\t\tSphereEnvMap,\n";

			if ((*iterator1)->shading & 16)
				fstream << "\t\t\tTwoSided,\n";

			if ((*iterator1)->shading & 32)
				fstream << "\t\t\tUnfogged,\n";

			if ((*iterator1)->shading & 64)
				fstream << "\t\t\tNoDepthTest,\n";

			if ((*iterator1)->shading & 128)
				fstream << "\t\t\tNoDepthSet,\n";

			fstream << "\t\t\tstatic (TextureID " << (*iterator1)->textureId << "),\n";

			if ((*iterator1)->tVertexAnimationId != 0xFFFFFFFF)
				fstream << "\t\t\tTVertexAnimId " << (*iterator1)->tVertexAnimationId << ",\n";

			if (showCoordId)
				fstream << "\t\t\tCoordId " << (*iterator1)->coordId << ",\n";
			
			fstream << "\t\t\tstatic (Alpha " << (*iterator1)->alpha << "),\n"
			<< "\t\t}\n";
		}

		fstream << "\t}\n";
	}
	
	fstream << "}\n";
}

void Mdlx::writeMdlTextureAnimations(std::fstream &fstream) throw (Exception)
{
	fstream << "TextureAnims " << this->m_textureAnimations.size() << " {\n";

	for (std::list<struct TextureAnimation*>::iterator iterator = this->m_textureAnimations.begin(); iterator != this->m_textureAnimations.end(); ++iterator)
	{
		fstream << "\tTVertexAnim {\n";
		
		if ((*iterator)->translation != 0)
			fstream << "\t\t(Translation { " << (*iterator)->translation->x << ", " << (*iterator)->translation->y << ", " << (*iterator)->translation->y << " })\n";
		
		/// @todo InTan and OutTan only appear when Hermite or Bezier. GlobalSeqId only appears when its value is not 0xFFFFFFFF.
		if ((*iterator)->rotation != 0)
			fstream << "\t\t(Rotation { " << (*iterator)->rotation->a << ", " << (*iterator)->rotation->b << ", " << (*iterator)->rotation->c << ", " << (*iterator)->rotation->d << " })\n";

		if ((*iterator)->scaling != 0)
			fstream << "\t\t(Scaling { " << (*iterator)->scaling->x << ", " << (*iterator)->scaling->y << ", " << (*iterator)->scaling->y << " })\n";
	
		fstream << "\t}\n";
	}

	fstream << "}\n";
}

void Mdlx::writeMdlGeoset(std::fstream &fstream) throw (Exception)
{
	fstream
	<< "Geoset {\n"
	<< "\tVertices " << this->m_geoset->vertices->vertices.size() << " {\n";

	for (std::list<struct Vertice*>::iterator iterator = this->m_geoset->vertices->vertices.begin(); iterator != this->m_geoset->vertices->vertices.end(); ++iterator)
		fstream << "\t\t{ " << (*iterator)->x << ", " << (*iterator)->y << ", " << (*iterator)->z << " },\n";

	fstream << "\t}\n";

	if (this->m_geoset->normals->normals.size() > 0)
	{
		fstream << "\tNormals " << this->m_geoset->normals->normals.size() << " {\n";

		for (std::list<struct Normal*>::iterator iterator = this->m_geoset->normals->normals.begin(); iterator != this->m_geoset->normals->normals.end(); ++iterator)
			fstream << "\t\t{ " << (*iterator)->x << ", " << (*iterator)->y << ", " << (*iterator)->z << " },\n";

		fstream << "\t}\n";
	}

	fstream << "\tTVertices " << this->m_geoset->tvertices->tvertices.size() << " {\n";

	for (std::list<struct Tvertice*>::iterator iterator = this->m_geoset->tvertices->tvertices.begin(); iterator != this->m_geoset->tvertices->tvertices.end(); ++iterator)
		fstream << "\t\t{ " << (*iterator)->x << ", " << (*iterator)->y << "},\n";

	fstream << "\t}\n";
	
	fstream << "\tVertexGroup {\n";

	for (std::list<byte::iterator iterator = this->m_geoset->vertexGroups->vertexGroups.begin(); iterator != this->m_geoset->vertexGroups->vertexGroups.end(); ++iterator)
		fstream << "\t\t" << *iterator << ",\n";
	
	fstream << "\t}\n";
	/// @todo Group size this->m_geoset->vertexGroups->vertexGroups.size() as before?!
	fstream << "\tFaces " << this->m_geoset->vertexGroups->vertexGroups.size() << " " << this->m_geoset->primitiveVertices->triangles.size() << " {\n"
	<< "\t\tTriangles {\n"
	<< "\t\t\t{ "

	for (std::list<short32>::iterator iterator = this->m_geoset->primitiveVertices->triangles.begin(); iterator != this->m_geoset->primitiveVertices->triangles.end(); ++iterator)
		fstream << *iterator << ", ";

	fstream
	<< "\t\t\t},\n"
	<< "\t\t}\n"
	<< "\t}\n";

	fstream
	<< "\tGroups " << this->m_geoset->matriceGroupCounts->groupCounts.size() << " " << this->m_geoset->matrices.size() << " {\n"
	<< "\t\tMatrices { ";

	for (std::list<long32>::iterator iterator = this->m_geoset->matrices.begin(); iterator != this->m_geoset->matrices.end(); ++iterator)
		fstream << *iterator << ", ";

	fstream
	<< "},\n"
	<< "\t}\n";

	if (this->m_geoset->m_minExtX != 0.0 || this->m_geoset->m_minExtY != 0.0 || this->m_geoset->m_minExtZ != 0.0)
		fstream << "\tMinimumExtent { " << this->m_geoset->m_minExtX << ", " << this->m_geoset->m_minExtY << ", " << this->m_geoset->m_minExtZ << " },\n";
	
	if (this->m_geoset->m_maxExtX != 0.0 || this->m_geoset->m_maxExtY != 0.0 || this->m_geoset->m_maxExtZ != 0.0)
		fstream << "\tMaximumExtent { " << this->m_geoset->m_minExtX << ", " << this->m_geoset->m_minExtY << ", " << this->m_geoset->m_minExtZ << " },\n";

	if (this->m_geoset->boundsRadius != 0.0)
		fstream << "\tBoundsRadius " << this->m_geoset->boundsRadius << ",\n";

	for (std::list<struct GAnimation*>::iterator iterator = this->m_geoset->ganimations.begin(); iterator != this->m_geoset->ganimations.end(); ++iterator)
	{
		fstream << "\tAnim {\n";

		if ((*iterator)->minExtX != 0.0 || (*iterator)->minExtY != 0.0 || (*iterator)->minExtZ != 0.0)
			fstream << "\t\tMinimumExtent { " << (*iterator)->minExtX << ", " << (*iterator)->minExtY << ", " << (*iterator)->minExtZ << " },\n";

		if ((*iterator)->maxExtX != 0.0 || (*iterator)->maxExtY != 0.0 || (*iterator)->maxExtZ != 0.0)
			fstream << "\t\tMaximumExtent { " << (*iterator)->maxExtX << ", " << (*iterator)->maxExtY << ", " << (*iterator)->maxExtZ << " },\n";

		if ((*iterator)->boundsRadius != 0.0)
			fstream << "\t\tBoundsRadius " << (*iterator)->boundsRadius << ",\n";

		fstream << "\t}\n";
	}
	
	fstream
	<< "\tMaterialID " << this->m_geoset->materialId << ",\n"
	<< "\tSelectionGroup " << this->m_geoset->selectionGroup << ",\n";

	if (this->m_geoset->selectable == 4)
		fstream << "\tUnselectable,\n";

	fstream << "}\n";
}

void Mdlx::writeMdlGeosetAnimations(std::fstream &fstream) throw (Exception)
{
	for (std::list<struct GeosetAnimation*>::iterator iterator = this->m_geosetAnimations->geosetAnimations.begin(); iterator != this->m_geosetAnimations->geosetAnimations.end(); ++iterator)
	{
		fstream << "GeosetAnim {\n";

		if ((*iterator)->colorAnimation == 1 || (*iterator)->colorAnimation == 3)
			fstream << "\tDropShadow,\n";
	
		fstream << "\tstatic (Alpha " << (*iterator)->staticAlpha << "),\n";
	
		/// @todo This flag?
		if ((*iterator)->colorAnimation > 1)
			fstream << "\tstatic (Color { " << (*iterator)->colorBlue << ", " << (*iterator)->colorGreen << ", " << (*iterator)->colorRed << " }),\n";

		fstream
		<< "\tGeosetId " << (*iterator)->geosetId << ",\n"
		<< "}\n";
	}
}

void Mdlx::writeMdlBones(std::fstream &fstream) throw (Exception)
{
	for (std::list<struct Bone*>::iterator iterator = this->m_bones->bones.begin(); iterator != this->m_bones->bones.end(); ++iterator)
	{
		fstream
		<< "Bone " << (*iterator)->object->name << " {\n"
		<< "\tObjectId " << (*iterator)->object->objectId << ",\n"
		<< "\tParent " << (*iterator)->object->parent << ",\n"
		;
		/* << "\tBillboardedLockZ,
		BillboardedLockY,
		BillboardedLockX,
		Billboarded,
		CameraAnchored,
		DontInherit { <Rotation|Translation|Scaling> },
		*/

		if ((*iterator)->geosetId == -1)
			fstream << "\tGeosetId Multiple,\n";
		else
			fstream << "\tGeosetId " << (*iterator)->geosetId << ",\n";

		if ((*iterator)->geosetAnimationId == -1)
			fstream << "\tGeosetAnimId None,\n";
		else
			fstream << "\tGeosetAnimId " << (*iterator)->geosetAnimationId << ",\n";

		/*
		(Translation { <float_x>, <float_y>, <float_z> })
		(Rotation { <float_a>, <float_b>, <float_c>, <float_d> })
		(Scaling { <float_x>, <float_y>, <float_z> })
		(Visibility <float>)
		*/
	}
}

void Mdlx::writeMdlHelpers(std::fstream &fstream) throw (Exception)
{
	//helpers
	// Observe properties of an Object.
	for (std::list<struct Object*>::iterator iterator = this->m_helpers->helpers.begin(); iterator != this->m_helpers->helpers.end(); ++iterator)
	{
		fstream
		<< "Helper " << (*iterator)->name << " {\n"
		<< "\tObjectId " << (*iterator)->objectId << ",\n"
		<< "\tParent " << (*iterator)->parent << ",\n";
		/*
		<< BillboardedLockZ,
		BillboardedLockY,
		BillboardedLockX,
		Billboarded,
		CameraAnchored,
		DontInherit { <Rotation|Translation|Scaling> },
		(Translation { <float_x>, <float_y>, <float_z> })
		(Rotation { <float_a>, <float_b>, <float_c>, <float_d> })
		(Scaling { <float_x>, <float_y>, <float_z> })
		(Visibility <float>)
		*/
	}
}

}

}
