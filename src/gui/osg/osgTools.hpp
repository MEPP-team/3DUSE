// -*-c++-*- VCity project, 3DUSE, Liris, 2013, 2014
////////////////////////////////////////////////////////////////////////////////
#ifndef __OSGTOOLS_HPP__
#define __OSGTOOLS_HPP__
////////////////////////////////////////////////////////////////////////////////
#include "core/URI.hpp"
#include <osg/Geode>
////////////////////////////////////////////////////////////////////////////////
namespace osgTools
{
    /// Build a bbox
    osg::ref_ptr<osg::Geode> buildBBox(osg::Vec3 lowerBound, osg::Vec3 upperBound);

    /// \brief getURI Compute the uri of an osg node
    /// \param node The osg node
    /// \return URI
    vcity::URI getURI(osg::Node* node);
}
////////////////////////////////////////////////////////////////////////////////
#endif // __OSGTOOLS_HPP__