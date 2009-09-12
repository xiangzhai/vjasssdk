/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
 *   tamino@cdauth.de                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef WC3LIB_MDLX_MDLX_HPP
#define WC3LIB_MDLX_MDLX_HPP

#include <fstream>
#include <list>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Object;

/**
* long/float size: 32-bit
* @todo OBJ == ?!
* @todo KATV == float?!
*/
class Mdlx
{
	public:
		struct Bones
		{
			//long nbytes;
			std::list<struct Bone*> bones; //[nbons]
		};

		struct PrimitiveVertices
		{
			//long	ntris;
			std::list<short32> triangles; //[ntris];
		};

		struct VertexGroups
		{
			//long	nvgrps;
			std::list<byte>	vertexGroups; //[nvgrps];
		};

		struct GeosetAnimations
		{
			//long	nbytes;
			std::list<struct GeosetAnimation*> geosetAnimations;
		};

		struct MatriceGroupCounts
		{
			//--MTGC // [Groups]
			//long	nmtrcs; //   GroupCount is an array of lengths
			std::list<long32> groupCounts; //[nmtrcs]; //   for each group.
		};

		struct Geoset
		{
			struct Vertices *vertices; //VRTX
			struct Normals *normals; //NRMS
			struct PrimitiveType *primitveType; //PTYP
			struct PrimitiveSizes *primitiveSizes; //PCNT
			struct PrimitiveVertices *primitiveVertices; //PVTX
			struct VertexGroups *vertexGroups; //GNDX
			struct MatriceGroupCounts *matriceGroupCounts; //MTGC
			struct Matrices *matrices; //MATS
			long32 materialId;
			long32 selectionGroup;
			long32 selectable; //(0:none;4:Unselectable)
			float32 boundsRadius;
			float32 minExtx, minExty, minExtz;
			float32 maxExtx, maxExty, maxExtz;
			//long32 nanim;

			struct GAnimation
			{
				float32 boundsRadius;
				float32 minExtx, minExty, minExtz;
				float32 maxExtx, maxExty, maxExtz;
			};

			std::list<struct GAnimation*> ganimations;
			//struct GAnimation *ganimations; //[nganim], naim?!?! Tamino Dauth
			struct TexturePatches *texturePatches; //UVAS
			struct Tvertices *tvertices; //UVBS
		};

		struct Vertice
		{
			float32 x, y, z;
		};

		struct Vertices
		{
			//long	nvrts;
			std::list<struct Vertice*> vertices; //[nvrts]
		};

		struct Normal
		{
			float32 x, y, z;
		};

		struct Normals
		{
			std::list<struct Normal*> normals; //[nnrms];
		};

		//--UVBS // [TVertices] (nonexistant in v1300)
		struct Tvertice
		{
			float32 x, y;
		};

		struct Tvertices
		{
			std::list<struct Tvertice*> tvertices;
		};

		struct Rotation
		{
			long32 frame;
			float32 a, b, c, d;
			//if (LineType > 1)
			//{
			float32 inTana, inTanb, inTanc, inTand;
			float32 outTana, outTanb, outTanc, outTand;
			//}
		};

		struct Rotations
		{
			//Rotations
			//long nunks;
			long32 lineType; //(0:don't interp;1:linear;2:hermite;3:bezier)
			long32 globalSequenceId; // 0xFFFFFFFF if none
			std::list<struct Rotation*> m_rotations; //[nunks]
		};

		struct Scaling
		{
			long32 frame;
			float32 x, y, z;
			//if (LineType > 1) {
			float32 inTanx, inTany, inTanz;
			float32 outTanx, outTany, outTanz;
			//}
		};


		struct Scalings
		{
			long32 m_scalingLineType; //(0:don't interp;1:linear;2:hermite;3:bezier)
			long32 m_scalingGlobalSequenceId; // 0xFFFFFFFF if none
			std::list<struct Scaling*> m_scalings; //[nunks];
		};

		struct Alpha
		{
			long32 frame;
			float32 state; //(0 or 1)
			//if (LineType > 1) {
			float32 inTan;
			float32 outTan;
			//}
		};

		struct Alphas //KMTA
		{
			//long nunks;
			long32 m_alphaLineType; //(0:don't interp;1:linear;2:hermite;3:bezier)
			long32 m_alphaGlobalSequenceId; // 0xFFFFFFFF if none
			std::list<struct Alpha*> m_alphas; //[nunks];
		};

		struct Layer
		{
			//long nbytesi;
			long32 filterMode; //(0:none;1:transparent;2:blend;3:additive;4:addalpha;5:modulate)
			long32 shading; //+1:unshaded;+2:SphereEnvMap;+16:twosided;
			long32 textureId; //  +32:unfogged;+64:NoDepthTest;+128:NoDepthSet)
			long32 tVertexAnimationId; // 0xFFFFFFFF if none
			long32 coordId;
			float32 alpha; //(0(transparent)->1(opaque))
			//(KMTA)
			//(KMTF) // state is long not float
		};

		struct Light
		{
			//long nbytesi;
			//OBJ
			long32 type; //(0:Omnidirectional;1:Directional;2:Ambient)
			float32 attStart, attEnd;
			float32 colorRed, colorGreen, colorBlue;
			float32 intensity;
			float32 ambColorRed, ambColorGreen, ambColorBlue;
			float32 ambIntensity;
			//(KLAI)
			//(KLAV)
			//(KLAC)
			//(KLBC)
			//(KLBI)
		};

		struct Model
		{
			//long nbytes;
			ascii m_name[0x150]; //(0x150 bytes)
			long32 m_unkown0; //(0)
			float32 m_boundsRadius;
			float32 m_minExtX, m_minExtY, m_minExtZ;
			float32 m_maxExtX, m_maxExtY, m_maxExtZ;
			long32 m_blendTime;
		};

		struct Material
		{
			//long nbytesi;
			long32 priorityPlane;
			long32 renderMode; //(+1:ConstantColor;+16:SortPrimsFarZ;+32:FullResolution)
			std::list<struct Layer*> layers;
		};

		struct PivotPoint
		{
			float32 x, y, z;
		};

		struct ParticleEmitter
		{
			long32 nbytesi;
			long32 nbytesikg; // inclusive bytecount including KGXXs
			ascii name[0x50]; //(0x50 bytes)
			long32 objectID;
			long32 parent; //(0xFFFFFFFF if none)
			long32 flags; //(bit20)	// +bit23(EmitterUsesMDL) +bit8(EmitterUsesTGA)
			//(KGTR)
			//(KGRT)
			//(KGSC)
			float32 emissionRate;
			float32 gravity;
			float32 longitude;
			float32 latitidue;
			ascii modelPath[0x100]; //(0x100 bytes)
			long32 unknown0; //(0)
			float32 lifeSpan;
			float32 initVelocity;
			//(KPEV)
		};

		struct ParticleEmitter2
		{
			long32 nbytesi;
			long32 nbytesikg; // inclusive bytecount including KGXXs
			ascii name[0x50]; //(0x50 bytes)
			long32 objectID;
			long32 parent; //(0xFFFFFFFF if none)
			long32 flags; //(bit20)	// +bit26(DontInherit Rotation)
			//(KGTR) // +bit23(Unshaded)	+bit10(Unfogged)
			//(KGRT) // +bit12(XYQuad)	+bit9(LineEmitter)
			//(KGSC) // +bit11(ModelSpace)	+bit8(SortPrimsFarZ)
			float32 speed;
			float32 variation;
			float32 latitidue;
			float32 gravity;
			float32 lifespan;
			float32 emissionRate;
			float32 length;
			float32 width;
			long32 filterMode; //(0:Blend;1:Additive;2:Modulate;4:AlphaKey)
			long32 rows;
			long32 columns;
			long32 flag2; //(0:Head;1:Tail;2:Both)
			float32 tailLength;
			float32 time;

			struct SegmentColor
			{			// SegmentColor usually 3 segments
				float red, green, blue; // Inverse order from MDL
			};//color[numsegments]

			byte alpha1, alpha2, alpha3; //byte
			float32 scalingX, scalingY, scalingZ;
			long32 lifeSpanUvAnim1, lifeSpanUvAnim2, lifeSpanUvAnim3;
			long32 decayUvAnim1, decayUvAnim2, decayUvAnim3;
			long32 tailUvAnim1, tailUvAnim2, tailUvAnim3;
			long32 tailDecayUvAnim1, tailDecayUvAnim2, tailDecayUvAnim3;
			long32 textureID;
			long32 squirt; //(1:Squirt)
			long32 priorityPlane;
			long32 replaceableId;
			//(KP2S)
			//(KP2L)
			//(KP2E)
			//(KP2V)
			//(KP2N)
			//(KP2W)
		};

		struct RibbonEmitter
		{
			//long nbytesi;
			//long nbytesikg; // inclusive bytecount including KGXXs
			ascii name[0x50]; //(0x50 bytes)
			long32 objectId;
			long32 parent; //(0xFFFFFFFF if none)
			long32 flags; //(0x00400000)
			//(KGTR)
			//(KGRT)
			//(KGSC)
			float32 heightAbove;
			float32 heightBelow;
			float32 alpha;
			float32 colorRed, colorGreen, colorBlue;
			float32 lifeSpan;
			long32 unknown0; //(0)
			long32 emissionRate;
			long32 rows;
			long32 columns;
			long32 materialId;
			float32 gravity;
			//(KRVS)
			//(KRHA)
			//(KRHB)
		};

		struct Sequence
		{
			ascii name[0x50]; //(0x50 bytes)
			long32 intervalStart, intervalEnd;
			float32 moveSpeed;
			long32 noLooping; //(0:loop; 1:no loop)
			float32 rarity;
			long32 unknown0; //(0)
			float32 boundsRadius;
			float32 minExtX, minExtY, minExtZ;
			float32 maxExtX, maxExtY, maxExtZ;
		};

		struct Texture
		{
			long32 replaceableId;
			ascii texturePath[0x100]; //(0x100 bytes)
			long32 unknown0; //(0)
			long32 wrapping; //(1:WrapWidth;2:WrapHeight;3:Both)
		};

		struct TextureAnimation
		{
			//long32 nbytesi;
			struct Translation *translation; //(KTAT) // Might be optional
			struct Rotation *rotation; //(KTAR)
			struct Scaling *scaling; //(KTAS)
		};


		struct Helpers
		{
			//HELP // [Helper]
			//long nbytes;
			std::list<struct Object*> helpers; //[nhlprs]; std::list<OBJ> m_helpers; //[nhlprs];
		};



		void readMdx(std::fstream &fstream) throw (Exception);
		void readMdl(std::fstream &fstream) throw (Exception);
		void readBlend(std::fstream &fstream) throw (Exception);
		void read3ds(std::fstream &fstream) throw (Exception);

		void writeMdx(std::fstream &fstream) throw (Exception);
		void writeMdl(std::fstream &fstream) throw (Exception);
		void writeBlend(std::fstream &fstream);
		void write3ds(std::fstream &fstream);

		static const long32 currentVersion = 0x20030000;
	private:
		void readMdlVersion(std::fstream &fstream) throw (Exception);
		void readMdlModel(std::fstream &fstream) throw (Exception);

		void writeMdlVersion(std::fstream &fstream) throw (Exception);
		void writeMdlModel(std::fstream &fstream) throw (Exception);
		void writeMdlSequences(std::fstream &fstream) throw (Exception);
		void writeMdlGlobalSequences(std::fstream &fstream) throw (Exception);
		void writeMdlMaterials(std::fstream &fstream) throw (Exception);
		void writeMdlTextureAnimations(std::fstream &fstream) throw (Exception);
		void writeMdlGeoset(std::fstream &fstream) throw (Exception);
		void writeMdlGeosetAnimations(std::fstream &fstream) throw (Exception);
		void writeMdlBones(std::fstream &fstream) throw (Exception);

		//long nbytes;
		std::list<struct Attachment*> m_attachments; //[natts]
		struct Bones *m_bones;
		//long nbytes;
		std::list<struct Camera*> m_cameras;
		//long nbytes;
		std::list<struct CollisionShape*> m_collisionShapes;
		//long nbytes;
		std::list<struct Event*> m_events;
		//long nbytes;
		struct GeosetAnimations *m_geosetAnimations;
		//long nbytes;
		std::list<struct Geoset*> m_geosets;
		//long nvrts;
		std::list<struct Vertice*> m_vertices; //[nvrts];
		//long nnrms;
		struct Normals *m_normals;
		// Not sure of the function of these.
		//long m_nptyps; //   PTYP seems to be a way to indicate the type 
		std::list<long32> m_primTypes; //[nptyps]; //   of primitive (4) for each group indicated
		//   in PCNT. And each value in PCNT is how 
		//long m_npcnts; //   many verts in a face (3).
		std::list<long32> m_primCounts; //[npcnts];
		//--PVTX // [Faces]
		//long ntris;
		std::list<short32> m_faceTriangles; //[ntris];
		//--GNDX // [VertexGroup]
		//long nvgrps;
		std::list<byte> m_vertexGroups; //[nvgrps]; //byte
		//--MTGC // [Groups]
		//long nmtrcs; //   GroupCount is an array of lengths
		std::list<long32> m_groupCounts; //[nmtrcs]; //   for each group.
		//--MATS // [Matrices]
		//long nmats; //   Each group is composed of the next
		std::list<long32> m_matrices; //[nmats]; //   groupCount[i] items of matrices.
		//--(BIDX) ???
		//--(BWGT) ???
		//--UVAS (repositioned over PTYP)
		//long ntvrts;
		//std::list<UVA> m_uvas; //UVBS[ntvrts]
		//--UVBS // [TVertices] (nonexistant in v1300)
		//long nvrts;
		std::list<struct TVertice*> m_tvertices; //[nvrts];
		//GLBS // [GlobalSequences]
		//long nbytes;
		std::list<long32> m_globalSequencesDurations; //[ndurs]; // ndurs = nbytes/4;
		struct Helpers *m_helpers;
		//Rotations
		struct Rotations *m_rotations;
	
		//KGSC // [Scaling]
		struct Scalings *m_scalings;

		//KMTA // [Alpha]
		struct Alphas *m_alphas;
		//LAYS // [Layer} (ID may have been removed, extra bytes...)
		//long nlays;
		std::list<struct Layer*> m_layers; //[nlays];
		//LITE // [Light]
		//long nbytes;
		std::list<struct Light*> m_lights; //[nlits];
		//MODL // [Model] (extra byte before blendTIme (0))
		struct Model *m_model;
		//MTLS // [Materials]
		//long nbytes;
		std::list<struct Material*> m_materials;
		//OBJ
		struct Object *m_object;
		//PIVT // [PivotPoints]
		//long nbytes;
		std::list<struct PivotPoint*> m_pivontPoints;
		//PREM // [ParticleEmitter]
		//long nbytes;
		std::list<struct ParticleEmitter*> m_particleEmitters;
		//PRE2 // [ParticleEmitter2]
		//long nbytes;
		std::list<struct ParticleEmitter2*> m_particleEmitters2;
		//RIBB
		//long nbytes;
		std::list<struct RibbonEmitter*> m_ribbonEmitters;
		//SEQS // [Sequences] (v13: +8 bytes?)
		//long nbytes;
		std::list<struct Sequence*> m_sequences;
		//TEXS // [Textures] (same as v800)
		//long nbytes;
		std::list<struct Texture*> m_textures;
		//TXAN // [Texture Animations]
		//long nbytes;
		std::list<struct TextureAnimation*> m_textureAnimations;
		//VERS // [Version]
		//long	nbytes;
		long32 m_version; // Currently 0x20030000 (800)
};

}

}

#endif
