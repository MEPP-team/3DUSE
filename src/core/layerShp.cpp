// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

////////////////////////////////////////////////////////////////////////////////
#include "layerShp.hpp"
#include "application.hpp"
////////////////////////////////////////////////////////////////////////////////
namespace vcity
{
////////////////////////////////////////////////////////////////////////////////
LayerShp::LayerShp( const std::string& name )
   : abstractLayer( name ), m_shp( nullptr )
{

}
////////////////////////////////////////////////////////////////////////////////
LayerShp::~LayerShp()
{
   GDALClose( m_shp );
}
////////////////////////////////////////////////////////////////////////////////
const std::string LayerShp::getType() const
{
   return "LayerShp";
}
////////////////////////////////////////////////////////////////////////////////
URI LayerShp::getURI() const
{
   URI uri;
   uri.append( getName(), getType() );
   uri.setType( getType() );

   return uri;
}
////////////////////////////////////////////////////////////////////////////////
void LayerShp::dump()
{
   log() << "    " << "TODO" << "\n";
}
////////////////////////////////////////////////////////////////////////////////
} // namespace vcity
////////////////////////////////////////////////////////////////////////////////
