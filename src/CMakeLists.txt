# This is the important bit for you, if you want to change the name of your extension in the build system
set( EXTENSION_NAME A3DB )

# Only edit below if you need to
cmake_minimum_required( VERSION 3.8 )
SOURCE_GROUP( "Template Files" REGULAR_EXPRESSION ".*tpp" )
SOURCE_GROUP( "Resouce Files" REGULAR_EXPRESSION ".*rc" )

project( ${EXTENSION_NAME} )

# Gather sources -------------------------------------------------------------------------------------------------------
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../bin/release )

file( GLOB TARGET_SOURCES Framework/*.hpp Framework/*.tpp Framework/*.cpp *.cpp *.hpp )

set( SOURCE_FILES ${TARGET_SOURCES} )

add_library( ${EXTENSION_NAME} SHARED ${SOURCE_FILES} )

target_include_directories( ${EXTENSION_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/Framework/ThridParty )

# Linker and compiler settings -----------------------------------------------------------------------------------------
set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /DNDEBUG" )

if( MSVC )
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
#    SET_TARGET_PROPERTIES( ${EXTENSION_NAME} PROPERTIES LINK_FLAGS /LTCG )
    SET_TARGET_PROPERTIES( ${EXTENSION_NAME} PROPERTIES LINK_FLAGS
                          "/LTCG
                           /NODEFAULTLIB:libc.lib
                           /NODEFAULTLIB:libcmt.lib
                           /NODEFAULTLIB:libcd.lib
                           /NODEFAULTLIB:libcmtd.lib
                           /NODEFAULTLIB:msvcrtd.lib"
                          )
endif()

# BOOST library - (required by SOCI) ---------------------------------------------------------------------
set( BOOST_INCLUDEDIR "D:/Projekte/C++/_BUILD_MODULES/boost_1_66_0" )
# set( BOOST_LYBRARYDIR "D:/Projekte/C++/_BUILD_MODULES/boost_1_66_0/stage_x64/lib" ) -- not used atm, as soci mainly uses header only libs
target_include_directories( ${EXTENSION_NAME} PUBLIC ${BOOST_INCLUDEDIR} )

# MySQL Connector/C (required by SOCI) ------------------------------------------------------------
set( MYSQL_C_DIR "D:/Projekte/C++/_BUILD_MODULES/mysql-connector-c-6.1.11/x64" )
include_directories( ${MYSQL_C_DIR}/include )
add_library( soci-libmysql-depend STATIC IMPORTED )
set_property( TARGET soci-libmysql-depend PROPERTY IMPORTED_LOCATION ${MYSQL_C_DIR}/libmysql/Release/mysqlclient.lib )
target_link_libraries( ${EXTENSION_NAME} PUBLIC soci-libmysql-depend )

# SQLITE (required by SOCI) ------------------------------------------------------------
set( SQLITE_DIR "D:/Projekte/C++/_BUILD_MODULES/sqlite3" )
include_directories( ${SQLITE_DIR}/sqlite3 )
add_library( soci-sqlite-depend STATIC IMPORTED )
set_property( TARGET soci-sqlite-depend PROPERTY IMPORTED_LOCATION ${SQLITE_DIR}/x64/Release/sqlite3.lib )
target_link_libraries( ${EXTENSION_NAME} PUBLIC soci-sqlite-depend )

# PostgreSQL (required by SOCI) ------------------------------------------------------------
set( POSTGRESQL_DIR "D:/Projekte/C++/_BUILD_MODULES/postgresql-9.6.7/x64/src" )
include_directories( ${POSTGRESQL_DIR}/include ${POSTGRESQL_DIR}/interfaces/libpq )
add_library( soci-libpq-depend STATIC IMPORTED )
set_property( TARGET soci-libpq-depend PROPERTY IMPORTED_LOCATION ${POSTGRESQL_DIR}/interfaces/libpq/Release/libpq.lib )
target_link_libraries( ${EXTENSION_NAME} PUBLIC soci-libpq-depend )

# SOCI library - C++ database access --------------------------------------------------------------------------
set( SOCI_INCLUDE_DIR "D:/Projekte/C++/_BUILD_MODULES/soci-master/include" )
set( SOCI_BUILD_DIR "D:/Projekte/C++/_BUILD_MODULES/soci-master/winbuildx64/" )
set( SOCI_LIBRARY_DIR "${SOCI_BUILD_DIR}/lib/Release" )
target_include_directories( ${EXTENSION_NAME} PUBLIC ${SOCI_INCLUDE_DIR} ${SOCI_BUILD_DIR}/include )
add_library( soci-core STATIC IMPORTED )
add_library( soci-mysql STATIC IMPORTED )
add_library( soci-sqlite STATIC IMPORTED )
add_library( soci-libpq STATIC IMPORTED )
set_property( TARGET soci-core PROPERTY IMPORTED_LOCATION ${SOCI_LIBRARY_DIR}/libsoci_core_4_0.lib )
set_property( TARGET soci-mysql PROPERTY IMPORTED_LOCATION ${SOCI_LIBRARY_DIR}/libsoci_mysql_4_0.lib )
set_property( TARGET soci-sqlite PROPERTY IMPORTED_LOCATION ${SOCI_LIBRARY_DIR}/libsoci_sqlite3_4_0.lib )
set_property( TARGET soci-libpq PROPERTY IMPORTED_LOCATION ${SOCI_LIBRARY_DIR}/libsoci_postgresql_4_0.lib )
target_link_libraries( ${EXTENSION_NAME} PUBLIC soci-core soci-mysql soci-sqlite soci-libpq )