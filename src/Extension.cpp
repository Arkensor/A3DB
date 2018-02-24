/**********************************************************************************************************************\

    DESCRIPTION: A3DB - A database extension for ArmA 3

------------------------------------------------------------------------------------------------------------------------

    CREATION DATE:  02.01.2018

------------------------------------------------------------------------------------------------------------------------

    Copyright © 2018
    Paul L. (https://github.com/Arkensor)
    Ollie W. (https://github.com/BoGuu)
    All rights reserved!

\**********************************************************************************************************************/

#include <soci/soci-config.h>
#include <soci/soci.h>
#include <soci/mysql/soci-mysql.h>
#include <soci/sqlite3/soci-sqlite3.h>
#include <soci/postgresql/soci-postgresql.h>

#include "Framework/ExtensionBase.hpp"

#include <iostream>

namespace A3
{
namespace Extension
{

std::shared_ptr< std::map< std::string, std::shared_ptr< soci::connection_pool > > > g_poConnectionPool;

enum EBackendType
{
    e_Emtpy,
    e_MySQL,
    e_SQLite,
    e_PostgreSQL,
};

/** Setup is called once before the first workload is processed
 * @Return: void
 */
void
CExtensionBase::Setup()
{
    m_poConsoleLogger->info( "I am still alive!" );

    //Setup the connection pool, each desired core gets one available connection to each supported database server
    auto poConnectionPool = std::make_shared< std::map< std::string, std::shared_ptr< soci::connection_pool > > >();
    g_poConnectionPool.swap( poConnectionPool );

    for( auto & oSection : m_poConfiguration->Sections() )
    {
        std::string strSectionLower = oSection;
        std::transform( strSectionLower.begin(), strSectionLower.end(), strSectionLower.begin(), ::tolower );

        std::string strDriver = m_poConfiguration->Get( oSection, "Driver", "" );
        std::string strDatabase = m_poConfiguration->Get( oSection, "Database", "" );
        std::string strHost = m_poConfiguration->Get( oSection, "Host", "" );
        std::string strPort = m_poConfiguration->Get( oSection, "Port", "" );
        std::string strUser = m_poConfiguration->Get( oSection, "User", "" );
        std::string strPassword = m_poConfiguration->Get( oSection, "Password", "" );

        std::transform( strDriver.begin(), strDriver.end(), strDriver.begin(), ::tolower );

        //Validations
        if( strDriver.empty() )
        {
            throw std::runtime_error( fmt::format( "No driver specified for connection: {0}", oSection ) );
        }

        if( strDatabase.empty() )
        {
            throw std::runtime_error( fmt::format( "No database name specified for connection: {0}", oSection ) );
        }

        //Build connection string
        std::string strConnectionString = fmt::format( "dbname={0}", strDatabase );

        if( !strHost.empty() )
        {
            strConnectionString += fmt::format( " host={0}", strHost );
        }

        if( !strPort.empty() )
        {
            strConnectionString += fmt::format( " port={0}", strPort );
        }

        if( !strUser.empty() )
        {
            strConnectionString += fmt::format( " user={0}", strUser );
        }

        if( !strPassword.empty() )
        {
            strConnectionString += fmt::format( " password={0}", strPassword );
        }

        //Find backend type
        EBackendType eBackend = EBackendType::e_Emtpy;

        if( strDriver.find( "mysql" ) != std::string::npos )
        {
            eBackend = EBackendType::e_MySQL;
        }
        else if ( strDriver.find( "sqlite" ) != std::string::npos )
        {
            eBackend = EBackendType::e_SQLite;

            if( !std::experimental::filesystem::exists( strDatabase ) )
            {
                if( !std::experimental::filesystem::create_directories( std::experimental::filesystem::path( strDatabase ).parent_path() ) )
                {
                    throw std::runtime_error( fmt::format( "Could not create database storage directory for connection: {0}", oSection ) );
                }
            }
        }
        else if ( strDriver.find( "postgresql" ) != std::string::npos )
        {
            eBackend = EBackendType::e_PostgreSQL;
        }

        //Init pools
        auto oPool = std::make_shared< soci::connection_pool >( m_poProcessor->m_nProcessorThreads );
        g_poConnectionPool->operator[](strSectionLower).swap( oPool );

        try
        {
            for( A3::DataTypes::uint8 nConnection = 0; nConnection < m_poProcessor->m_nProcessorThreads; ++nConnection )
            {
                soci::session & oConnection = g_poConnectionPool->at(strSectionLower)->at( nConnection );

                switch ( eBackend )
                {
                    case EBackendType::e_MySQL:
                    {
                        oConnection.open( soci::mysql, strConnectionString );
                        break;
                    }
                    case EBackendType::e_SQLite:
                    {
                        oConnection.open( soci::sqlite3, strConnectionString );
                        break;
                    }
                    case EBackendType::e_PostgreSQL:
                    {
                        oConnection.open( soci::postgresql, strConnectionString );
                        break;
                    }
                    default:
                    {
                        throw std::runtime_error( fmt::format( "No backend driver was found for connection: {0}", oSection ) );
                    }
                }
            }
        }
        catch( const std::exception & oException )
        {
            throw std::runtime_error( fmt::format( "Error '{0}' while opening connection for: {1}. Check your A3DB.ini!", oException.what(), oSection ) );
        }
    }
}

/** Execute is called for each workload that is processed
 * @Return: Returns a string as result to the processed workload. This string
 *          is then returned to the sqf side function.
 *          Returns could be prepared for easy parsing in ArmA via parseSimpleArray.
 *          -> https://community.bistudio.com/wiki/parseSimpleArray
 */
std::string
CExtensionBase::Execute( A3::Extension::Processor::CProcessorWorkload oWorkload )
{
    int nCount;

    soci::session sql( *g_poConnectionPool->at( "arma_mysql_or_mariadb" ) );

    sql << "select count(*) from test", soci::into( nCount );

    std::cout << "We have " << nCount << " entries in the test table." << std::endl;;

    soci::session sql3( *g_poConnectionPool->at( "arma_postgresql" ) );

    sql3 << "select count(*) from test", soci::into( nCount );

    std::cout << "We have " << nCount << " entries in the test table." << std::endl;;

    soci::session sql2( *g_poConnectionPool->at( "arma_sqlite" ) );

    sql2 << "drop table test1";

    sql2 <<
         "create table test1 ("
                 "    id integer,"
                 "    name varchar(100)"
                 ")";

    sql2 << "insert into test1(id, name) values(7, \'John\')";

    soci::rowid rid(sql2);

    sql2 << "select oid from test1 where id = 7", soci::into(rid);

    int id;
    std::string name;

    sql2 << "select id, name from test1 where oid = :rid",
            soci::into(id), soci::into(name), use(rid);


    std::cout << "ID: " << id << " - Name: " << name << std::endl;

    sql2 << "drop table test1";

    return "A3EXT rocks!";
}

}; // end namespace Extension
}; // end namespace A3