#ifndef WC3LIB_MDLX_LAYER_HPP
#define WC3LIB_MDLX_LAYER_HPP

#include <fstream>

#include "platform.hpp"
#include "../exception.hpp"

namespace wc3lib
{

namespace mdlx
{

class Mdlx;

class Layer
{
	public:
		Layer(class Mdlx *mdlx);
		virtual ~Layer();

		class Mdlx* mdlx() const;
		long32	filterMode() const;
		long32	shading() const;
		long32	textureId() const;
		long32 	tvertexAnimationId() const;
		long32	coordinatesId() const;
		float32	alpha() const;
		class Alphas* alphas() const;
		class Alpha2s* alpha2s() const;

		virtual void readMdl(std::fstream &fstream) throw (class Exception);
		virtual void readMdx(std::fstream &fstream) throw (class Exception);
		virtual void writeMdl(std::fstream &fstream) throw (class Exception);
		virtual void writeMdx(std::fstream &fstream) throw (class Exception);

	protected:
		class Mdlx *m_mdlx;
		long32	m_filterMode; //(0:none;1:transparent;2:blend;3:additive;4:addalpha;5:modulate)
		long32	m_shading; //+1:unshaded;+2:SphereEnvMap;+16:twosided;
		long32	m_textureId; //  +32:unfogged;+64:NoDepthTest;+128:NoDepthSet)
		long32 	m_tvertexAnimationId; // 0xFFFFFFFF if none
		long32	m_coordinatesId;
		float32	m_alpha; //(0(transparent)->1(opaque))
		class Alphas *m_alphas; //(KMTA)
		class Alpha2s *m_alpha2s; //(KMTF) // state is long not float
};

inline class Mdlx* Layer::mdlx() const
{
	return this->m_mdlx;
}

inline long32 Layer::filterMode() const
{
	return this->m_filterMode;
}

inline long32 Layer::shading() const
{
	return this->m_shading;
}

inline long32 Layer::textureId() const
{
	return this->m_textureId;
}

inline long32 Layer::tvertexAnimationId() const
{
	return this->m_tvertexAnimationId;
}

inline long32 Layer::coordinatesId() const
{
	return this->m_coordinatesId;
}

inline float32 Layer::alpha() const
{
	return this->m_alpha;
}

inline class Alphas* Layer::alphas() const
{
	return this->m_alphas;
}

inline class Alpha2s* Layer::alpha2s() const
{
	return this->m_alpha2s;
}

}

}

#endif
