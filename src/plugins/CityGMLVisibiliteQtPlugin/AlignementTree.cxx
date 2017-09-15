// Copyright University of Lyon, 2012 - 2017
// Distributed under the GNU Lesser General Public License Version 2.1 (LGPLv2)
// (Refer to accompanying file LICENSE.md or copy at
//  https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html )

#include <qstring.h>
#include <qfiledialog.h>
#include <ogrsf_frmts.h>

#include "AlignementTree.hpp"
#include "core/application.hpp"
#include "libcitygml/export/exportCityGML.hpp"
#include "filters/raytracing/Hit.hpp"
#include "AABB.hpp"

typedef std::vector<TVec3d> LRing;

// FIXME: this file has some comments in french.

/**
*	@brief Convert a degree float to radian
*/
inline float DegToRad( float x )
{
   return x / 180 * M_PI;
}

/**
*	@brief Convert a radian float to degree
*/
inline float RadToDeg( float x )
{
   return x / M_PI * 180;
}

citygml::Geometry* BuildCube( std::string name, TVec3d pos, float radius, float height )
{
   float offsetPoint = radius / sqrt( 2 );

   citygml::Geometry* cityGeom = new citygml::Geometry( name + "_Geometry", citygml::GT_Unknown, 2 );

   {citygml::LinearRing* ring = new citygml::LinearRing( name + "_ringFutBot", true );
   ring->addVertex( TVec3d( pos.x - offsetPoint, pos.y - offsetPoint, pos.z ) );
   ring->addVertex( TVec3d( pos.x + offsetPoint, pos.y - offsetPoint, pos.z ) );
   ring->addVertex( TVec3d( pos.x + offsetPoint, pos.y + offsetPoint, pos.z ) );
   ring->addVertex( TVec3d( pos.x - offsetPoint, pos.y + offsetPoint, pos.z ) );
   citygml::Polygon* poly = new citygml::Polygon( name + "_polyFutBot" );
   poly->addRing( ring );
   cityGeom->addPolygon( poly ); }

   {citygml::LinearRing* ring = new citygml::LinearRing( name + "_ringFutTop", true );
   ring->addVertex( TVec3d( pos.x - offsetPoint, pos.y - offsetPoint, pos.z + height ) );
   ring->addVertex( TVec3d( pos.x + offsetPoint, pos.y - offsetPoint, pos.z + height ) );
   ring->addVertex( TVec3d( pos.x + offsetPoint, pos.y + offsetPoint, pos.z + height ) );
   ring->addVertex( TVec3d( pos.x - offsetPoint, pos.y + offsetPoint, pos.z + height ) );
   citygml::Polygon* poly = new citygml::Polygon( name + "_polyFutTop" );
   poly->addRing( ring );
   cityGeom->addPolygon( poly ); }

   {citygml::LinearRing* ring = new citygml::LinearRing( name + "_ringFutFront", true );
   ring->addVertex( TVec3d( pos.x - offsetPoint, pos.y - offsetPoint, pos.z ) );
   ring->addVertex( TVec3d( pos.x + offsetPoint, pos.y - offsetPoint, pos.z ) );
   ring->addVertex( TVec3d( pos.x + offsetPoint, pos.y - offsetPoint, pos.z + height ) );
   ring->addVertex( TVec3d( pos.x - offsetPoint, pos.y - offsetPoint, pos.z + height ) );
   citygml::Polygon* poly = new citygml::Polygon( name + "_polyFutFront" );
   poly->addRing( ring );
   cityGeom->addPolygon( poly ); }

   {citygml::LinearRing* ring = new citygml::LinearRing( name + "_ringFutBack", true );
   ring->addVertex( TVec3d( pos.x + offsetPoint, pos.y + offsetPoint, pos.z ) );
   ring->addVertex( TVec3d( pos.x - offsetPoint, pos.y + offsetPoint, pos.z ) );
   ring->addVertex( TVec3d( pos.x - offsetPoint, pos.y - offsetPoint, pos.z + height ) );
   ring->addVertex( TVec3d( pos.x + offsetPoint, pos.y - offsetPoint, pos.z + height ) );
   citygml::Polygon* poly = new citygml::Polygon( name + "_polyFutBack" );
   poly->addRing( ring );
   cityGeom->addPolygon( poly ); }

   {citygml::LinearRing* ring = new citygml::LinearRing( name + "_ringFutLeft", true );
   ring->addVertex( TVec3d( pos.x - offsetPoint, pos.y + offsetPoint, pos.z ) );
   ring->addVertex( TVec3d( pos.x - offsetPoint, pos.y - offsetPoint, pos.z ) );
   ring->addVertex( TVec3d( pos.x - offsetPoint, pos.y - offsetPoint, pos.z + height ) );
   ring->addVertex( TVec3d( pos.x - offsetPoint, pos.y + offsetPoint, pos.z + height ) );
   citygml::Polygon* poly = new citygml::Polygon( name + "_polyFutLeft" );
   poly->addRing( ring );
   cityGeom->addPolygon( poly ); }

   {citygml::LinearRing* ring = new citygml::LinearRing( name + "_ringFutRight", true );
   ring->addVertex( TVec3d( pos.x + offsetPoint, pos.y - offsetPoint, pos.z ) );
   ring->addVertex( TVec3d( pos.x + offsetPoint, pos.y + offsetPoint, pos.z ) );
   ring->addVertex( TVec3d( pos.x + offsetPoint, pos.y + offsetPoint, pos.z + height ) );
   ring->addVertex( TVec3d( pos.x + offsetPoint, pos.y - offsetPoint, pos.z + height ) );
   citygml::Polygon* poly = new citygml::Polygon( name + "_polyFutRight" );
   poly->addRing( ring );
   cityGeom->addPolygon( poly ); }

   return cityGeom;
}

/**
*	Create a cylinder in a citymodel
*	@param pos 3D position of the cylinder
*	@param ModelOut CityModel where to put the cylinder
*/
void GenCylindre( TVec3d pos, OGRFeature* feature, citygml::CityModel* ModelOut )
{
   static int cpt = 0;
   float radiusFut = 1.0;
   float heightFut = 6.0;
   float heightLeaf = 6.0;
   float radiusLeaf = 3.0;


   std::string name = "noid";
   if ( feature->GetFieldIndex( "identifian" ) != -1 )
      name = feature->GetFieldAsString( "identifian" );// "codefuv"/"gid"

   name += "_tree";

   if ( feature->GetFieldIndex( "circonfere" ) != -1 )
      radiusFut = ( ( feature->GetFieldAsDouble( "circonfere" ) / M_PI ) / 100 ) / 2;//On a une ciconference en cm...

   if ( feature->GetFieldIndex( "hauteurfut" ) != -1 )
      heightFut = feature->GetFieldAsDouble( "hauteurfut" );

   if ( feature->GetFieldIndex( "hauteurtot" ) != -1 )
      heightLeaf = feature->GetFieldAsDouble( "hauteurtot" ) - heightFut;

   if ( feature->GetFieldIndex( "rayoncouro" ) != -1 )
      radiusLeaf = feature->GetFieldAsDouble( "rayoncouro" );


   std::cout << name << " " << radiusFut << " " << heightFut << " " << heightLeaf << " " << radiusLeaf << std::endl;

   citygml::CityObject* BuildingCO = new citygml::SolitaryVegetationObject( name );

   BuildingCO->addGeometry( BuildCube( name, pos, radiusFut, heightFut ) );
   BuildingCO->addGeometry( BuildCube( name, TVec3d( pos.x, pos.y, pos.z + heightFut ), radiusLeaf, heightLeaf ) );

   ModelOut->addCityObject( BuildingCO );
   ModelOut->addCityObjectAsRoot( BuildingCO );

   cpt++;
}

/**
*	@brief Put a set of point a the height of the terrain according to the tiled files of this terrain
*/
void PutLRingOnTiledTerrain( LRing ring, std::vector<OGRFeature*> pointsFeatures, std::string dir )
{
   //Load all terrain bounding box that are above the points
   AABBCollection boxes = LoadLayersAABBs( dir );

   std::cout << "Creation des arbres d'alignement sur les " << boxes.terrain.size() << " tuiles du terrain." << std::endl;

   for ( AABB box : boxes.terrain )
   {
      TVec3d min = box.min;
      TVec3d max = box.max;

      std::string path = dir + box.name;
      TriangleList* TrianglesTile = BuildTriangleList( path, citygml::CityObjectsType::COT_TINRelief );

      // Contiendra les points de la box courante
      LRing PtsBox;
      // Contiendra les donnees semantiques de ces points
      std::vector<OGRFeature*> PtsBoxFeatures;

      for ( int i = 0; i < ring.size(); ++i )
      {
         TVec3d vec = ring.at( i );
         // Le point vec est dans la box courante
         if ( vec.x >= min.x && vec.x <= max.x && vec.y >= min.y && vec.y <= max.y )
         {
            Ray ray( vec, TVec3d( 0.0, 0.0, 1.0 ) );

            for ( unsigned int j = 0; j < TrianglesTile->triangles.size(); j++ )
            {
               Hit hit;
               // Check if the ray hit the triangle 
               if ( ray.Intersect( TrianglesTile->triangles[j], &hit ) )
               {
                  vec.z = hit.point.z;
                  PtsBox.push_back( vec );
                  PtsBoxFeatures.push_back( pointsFeatures.at( i ) );
                  break;
               }
            }

            // Pour ne plus avoir a tester ce point dans les prochaines box
            LRing::iterator it = ring.begin() + i;
            ring.erase( it );
            std::vector<OGRFeature*>::iterator it2 = pointsFeatures.begin() + i;
            pointsFeatures.erase( it2 );
            i--;
         }
      }

      if ( PtsBox.size() == 0 )
         continue;

      citygml::CityModel* ModelOut = new citygml::CityModel;

      for ( unsigned int i = 0; i < PtsBox.size(); i++ )
      {
         GenCylindre( PtsBox[i], PtsBoxFeatures[i], ModelOut );
      }

      std::cout << "Exporting citygml for box " << box.name << std::endl;
      ModelOut->computeEnvelope();

      QDir dirArbres;
      dirArbres.mkdir( QString::fromStdString( dir ) + "/_VEGET/" );

      citygml::ExporterCityGML exporter( dir + "/_VEGET/" + box.name.substr( box.name.find_last_of( "/" ), box.name.length() ) );
      exporter.exportCityModel( *ModelOut );
      std::cout << "Done exporting box " << box.name << std::endl;
   }

   std::cout << std::endl << "FIN : Tous les arbres des tuiles correspondantes ont ete generes." << std::endl;
}

void ExtrudeAlignementTree( std::string dir )
{
   QString filepath = QFileDialog::getOpenFileName( nullptr, "Load shp file" );

   QFileInfo file( filepath );

   QString ext = file.suffix().toLower();

   if ( ext == "shp" )
   {
      GDALDataset* poDS = (GDALDataset*)GDALOpenEx( filepath.toStdString().c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL );

      std::cout << "Shp loaded" << std::endl;
      std::cout << "Processing..." << std::endl;

      OGRLayer *poLayer;

      if ( poDS->GetLayerCount() == 0 )
      {
         std::cout << "Erreur, fichier Shapefile vide." << std::endl;
         return;
      }

      // Contiendra tous les points representant les positions des arbres d'alignement
      LRing points;
      // Contiendra les informations semantiques liees a ces points
      std::vector<OGRFeature*> pointsFeatures;

      poLayer = poDS->GetLayer( 0 );

      OGRFeature *poFeature;

      poLayer->ResetReading();

      while ( ( poFeature = poLayer->GetNextFeature() ) != NULL )
      {
         OGRGeometry* poGeometry = poFeature->GetGeometryRef();

         if ( poGeometry != NULL && poGeometry->getGeometryType() == wkbPoint )
         {
            OGRPoint* poP = (OGRPoint*)poGeometry;

            points.push_back( TVec3d( poP->getX(), poP->getY(), 0.0 ) );
            pointsFeatures.push_back( poFeature );
         }
      }

      PutLRingOnTiledTerrain( points, pointsFeatures, dir );

      GDALClose( poDS );
   }
}
