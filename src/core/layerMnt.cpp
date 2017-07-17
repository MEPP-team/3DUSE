// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

////////////////////////////////////////////////////////////////////////////////
#include "layerMnt.hpp"
#include "application.hpp"
////////////////////////////////////////////////////////////////////////////////
namespace vcity
{
    ////////////////////////////////////////////////////////////////////////////////
    LayerMnt::LayerMnt(const std::string& name)
        : abstractLayer(name)
    {

    }
    ////////////////////////////////////////////////////////////////////////////////
    LayerMnt::~LayerMnt()
    {

    }
    ////////////////////////////////////////////////////////////////////////////////
    const std::string LayerMnt::getType() const
    {
        return "LayerMnt";
    }
    ////////////////////////////////////////////////////////////////////////////////
    URI LayerMnt::getURI() const
    {
        URI uri;
        uri.append(getName());
        uri.setType(getType());

        return uri;
    }
    ////////////////////////////////////////////////////////////////////////////////
    void LayerMnt::dump()
    {
        log() << "    " << "TODO" << "\n";
    }
    ////////////////////////////////////////////////////////////////////////////////
} // namespace vcity
////////////////////////////////////////////////////////////////////////////////
