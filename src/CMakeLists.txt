# This is the important bit for you, if you want to change the name of your extension in the build system
set( EXTENSION_NAME A3DB )

# Only edit below if you need to
cmake_minimum_required( VERSION 3.8 )

project( ${EXTENSION_NAME} )

SOURCE_GROUP( "Template Files" REGULAR_EXPRESSION ".*tpp" )
SOURCE_GROUP( "Resouce Files" REGULAR_EXPRESSION ".*rc" )

set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /DNDEBUG" )

if( MSVC )
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin/release )

file( GLOB TARGET_SOURCES Framework/*.hpp Framework/*.tpp Framework/*.cpp *.cpp *.hpp )

set( SOURCE_FILES ${TARGET_SOURCES} )

add_library( ${EXTENSION_NAME} SHARED ${SOURCE_FILES} )

#BOOST library - (required by SOCI) ---------------------------------------------------------------------
set( BOOST_INCLUDEDIR "D:/Projekte/C++/_BUILD_MODULES/boost_1_66_0/boost" )
set( BOOST_LYBRARYDIR "D:/Projekte/C++/_BUILD_MODULES/boost_1_66_0/stage_x64/lib" )
target_include_directories( ${EXTENSION_NAME} PUBLIC ${Boost_INCLUDE_DIRS} )

#MySQL C API - MySQL Connector/C (required by SOCI)------------------------------------------------------------
set( MYSQL_C_DIR "D:/Projekte/C++/_BUILD_MODULES/mysql-connector-c-6.1.11/x64" )
include_directories( ${MYSQL_C_DIR}/include )
add_library( mysql-libmysql STATIC IMPORTED )
set_property( TARGET mysql-libmysql PROPERTY IMPORTED_LOCATION ${MYSQL_C_DIR}/libmysql/Release/mysqlclient.lib )
target_link_libraries( ${EXTENSION_NAME} PUBLIC mysql-libmysql )

#SOCI library - C++ database access --------------------------------------------------------------------------
set( SOCI_INCLUDE_DIR "D:/Projekte/C++/_BUILD_MODULES/soci-3.2.3/src" )
set( SOCI_LIBRARY "D:/Projekte/C++/_BUILD_MODULES/soci-3.2.3/src/winbuildx64/lib/Release" )
target_include_directories( ${EXTENSION_NAME} PUBLIC ${SOCI_INCLUDE_DIR}/backends )
target_include_directories( ${EXTENSION_NAME} PUBLIC ${SOCI_INCLUDE_DIR}/core )
add_library( soci-core STATIC IMPORTED )
add_library( soci-mysql STATIC IMPORTED )
set_property( TARGET soci-core PROPERTY IMPORTED_LOCATION ${SOCI_LIBRARY}/libsoci_core_3_2.lib )
set_property( TARGET soci-mysql PROPERTY IMPORTED_LOCATION ${SOCI_LIBRARY}/libsoci_mysql_3_2.lib )
target_link_libraries( ${EXTENSION_NAME} PUBLIC soci-core soci-mysql )

target_include_directories( ${EXTENSION_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/Framework/ThridParty )