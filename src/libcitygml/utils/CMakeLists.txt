
add_library( citygmlutils SHARED
  AABB.cpp
  CityGMLtools.cpp
  exportJSON.cpp
  exportOBJ.cpp
  importerASC.cpp
  importerAssimp.cpp
  tile.cpp
  TiledFilesLayout.cpp
  Triangle.cpp
  ConvertTextures.cpp
)

if(MSVC)
  # The build dll goes to the top level build directory.
  # The associated import library goes...out of the way.
  foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )  # Just in case
    set_target_properties( citygmlutils PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
         ${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}
      LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG}
         ${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/Lib
    )
  endforeach()
endif()

target_include_directories( citygmlutils PUBLIC
  ${CITYGML_INCLUDE_DIR}
  ${VCITYCORE_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR} # for generated citygmlutils_export.h
)

target_link_libraries( citygmlutils
   ${CITYGML_LIBRARIES}
   ${VCITYCORE_LIBRARY}
)

install(TARGETS citygmlutils
        LIBRARY DESTINATION lib)

# Dealing with VC++ oddities: generates the export header citygmlutils_exports.h 
GENERATE_EXPORT_HEADER( citygmlutils )
