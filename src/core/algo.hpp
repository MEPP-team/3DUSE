#ifndef __ALGO_HPP__
#define __ALGO_HPP__
////////////////////////////////////////////////////////////////////////////////
#include "URI.hpp"

#include <geos/geom/GeometryFactory.h>
////////////////////////////////////////////////////////////////////////////////
namespace vcity
{
class Algo
{
public:
	void generateLOD0(const URI& uri);
    void generateLOD0Scene(geos::geom::Geometry ** ShapeGeo);

private:

};
// entry points for LODs ?
// split algos in multiple files ?
////////////////////////////////////////////////////////////////////////////////
} // namespace vcity
////////////////////////////////////////////////////////////////////////////////
#endif // __ALGO_HPP__

