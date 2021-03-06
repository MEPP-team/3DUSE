// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

////////////////////////////////////////////////////////////////////////////////
#ifndef OSGINFODATATYPE_HPP
#define OSGINFODATATYPE_HPP

////////////////////////////////////////////////////////////////////////////////
/// \brief
////////////////////////////////////////////////////////////////////////////////
#include "osgInfo.hpp"
#include "vecs.hpp"
#include <osg/Switch>
#include <osg/MatrixTransform>


class InfoDataType : public osg::Referenced
{
public:
    /// \brief InfoDataType constructor
    InfoDataType();

    /// \brief InfoDataType constructor
    /// \param osg::Node
    InfoDataType(osg::Node* n);

    /// \brief Get switchroot node member
    /// \return osg::ref_ptr<osg::Switch> pointer to switchroot
    osg::ref_ptr<osg::Switch> getSwitchRoot();

    /// \brief Display document in map as stairs
    /// Each document is higher than the one in front
    /// \param sorted map with osgInfo and its DCAM
    void stairedDisplay(std::map<float, osgInfo*> m_info);

    /// \brief Display document by OVa
    /// \param sorted map with osgInfo and its DCAM
    void OVaDisplay(int screenX, int screenY, std::map<float, osgInfo *> m_info);

    /// \brief Turn on switches with displayable documents
    void display();


    void computeDepthMap(int screenX, int screenY, std::map<float, osgInfo *> m_info);


protected:
    osg::ref_ptr<osg::Switch> switchRoot;

};

#endif // OSGINFODATATYPE_HPP
