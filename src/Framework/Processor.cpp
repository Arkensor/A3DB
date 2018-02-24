/**********************************************************************************************************************\

    DESCRIPTION: Processor that handles incoming jobs using multiple threads

------------------------------------------------------------------------------------------------------------------------

    CREATION DATE:  31.12.2017

------------------------------------------------------------------------------------------------------------------------

    Copyright © 2018
    Paul L. (https://github.com/Arkensor)
    Ollie W. (https://github.com/BoGuu)
    All rights reserved!

\**********************************************************************************************************************/

#include "Processor.hpp"

namespace A3
{
namespace Extension
{
namespace Processor
{

CProcessor::CProcessor( A3::DataTypes::int8 nThreads )
    : m_bActive( false )
    , m_nProcessorThreads( nThreads )
{
}

CProcessor::~CProcessor()
{
}

void
CProcessor::Start( std::function< std::vector< CProcessorResult >( CProcessorWorkload ) > oTreadFunction )
{
    if ( m_bActive )
    {
        return;
    }

    m_bActive = true;

    for ( int nThread = 0; nThread < m_nProcessorThreads; ++nThread )
    {
        std::thread oThread( [=] { run( oTreadFunction ); } );

        oThread.detach();
    }
}

void
CProcessor::Add( CProcessorWorkload & roWorkload )
{
    oWorkloadQueue.push( roWorkload );
}

bool
CProcessor::try_get_results( std::vector< CProcessorResult > & oWorkload,
                             A3::DataTypes::uint64 nCurrentSize,
                             A3::DataTypes::int64 nMaxSize )
{
    return oResultQueue.try_pop_results( oWorkload, nCurrentSize, nMaxSize );
}

void
CProcessor::run( std::function< std::vector< CProcessorResult >( CProcessorWorkload ) > oFunction )
{
    while ( m_bActive )
    {
        CProcessorWorkload oWorkLoad = oWorkloadQueue.pop();

        std::vector< CProcessorResult > oResults = oFunction( oWorkLoad );

        for ( CProcessorResult & result : oResults )
        {
            oResultQueue.push( result );
        }
    }
}

}; // end namespace Processor
}; // end namespace Extension
}; // end namespace A3