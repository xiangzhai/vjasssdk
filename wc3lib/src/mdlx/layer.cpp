#include "layer.hpp"

namespace wc3lib
{

namespace mdlx
{

Layer::Layer(class Mdlx *mdlx) : m_mdlx(mdlx)
{
}

Layer::~Layer()
{
}

void Layer::readMdl(std::fstream &fstream) throw (class Exception)
{
}

void Layer::readMdx(std::fstream &fstream) throw (class Exception)
{
}

void Layer::writeMdl(std::fstream &fstream) throw (class Exception)
{
}

void Layer::writeMdx(std::fstream &fstream) throw (class Exception)
{
}

}

}
