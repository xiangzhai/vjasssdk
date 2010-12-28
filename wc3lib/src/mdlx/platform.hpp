/***************************************************************************
 *   Copyright (C) 2009 by Tamino Dauth                                    *
 *   tamino@cdauth.eu                                                      *
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

#ifndef WC3LIB_MDLX_PLATFORM_HPP
#define WC3LIB_MDLX_PLATFORM_HPP

#include <string>

#include <boost/cstdint.hpp>

namespace wc3lib
{

namespace mdlx
{

/// @todo Check signed and unsigned!
/**
* MDL notes:
* Floating point numbers are in IEEE scientific notation format with 6 significant
* figures. Exponent is not shown for exponents of ±4. If the number is an integral
* float, decimal point is not shown.
*/
typedef float float32;
typedef uint16_t short16; /// @todo undefined length?!
typedef uint32_t long32;
typedef char ascii;
typedef char byte;
typedef std::basic_string<ascii> string; /// Required for MDL format.

/**
* MDLX format supports interpolation for scalings, translations and rotations.
* If interpolation is not used value should be DontInterpolate.
* If line type is Hermite or Bezier additional interpolation data is used (see structures).
* Line type is stored as long32 in MDX files.
* @see InterpolationData, InterpolationRotationData
*/
enum LineType
{
	DontInterpolate = 0,
	Linear = 1,
	Hermite = 2,
	Bezier = 3
};

/**
* @todo Finish enum.
* @see Texture, ParticleEmitter2
*/
enum ReplaceableId
{
	None = 0,
	TeamColor = 1,
	TeamGlow = 2,
	Cliff = 11,
	LordaeronTree = 31,
	AshenvaleTree = 32,
	BarrensTree = 33,
	NorthrendTree = 34,
	MushroomTree = 35
};

struct VertexData
{
	VertexData() : x(0), y(0), z(0) { };
	VertexData(float32 x, float32 y, float32 z) : x(x), y(y), z(z)
	{
	}

	float32 operator[](uint8_t index) const
	{
		if (index == 0)
			return x;
		else if (index == 1)
			return y;

		return z;
	}

	float32 x, y, z;
};

struct QuaternionData
{
	float32 a, b, c, d;
};

struct InterpolationData
{
	float32 inTanX, inTanY, inTanZ;
	float32	outTanX, outTanY, outTanZ;
};

struct InterpolationRotationData
{
	float32 inTanA, inTanB, inTanC, inTanD;
	float32 outTanA, outTanB, outTanC, outTanD;
};

}

}

#endif
