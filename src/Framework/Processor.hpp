#ifndef A3DB_PROCESSOR_HPP
#define A3DB_PROCESSOR_HPP
/**********************************************************************************************************************\

    DESCRIPTION: rocessor that handles incoming jobs using multiple threads

------------------------------------------------------------------------------------------------------------------------

    CREATION DATE:  31.12.2017

------------------------------------------------------------------------------------------------------------------------

    Copyright © 2018
    Paul L. (https://github.com/Arkensor)
    Ollie W. (https://github.com/BoGuu)
    All rights reserved!

\**********************************************************************************************************************/

#include "ProcessorQueue.hpp"
#include "ProcessorWorkload.hpp"
#include "ProcessorResult.hpp"

#include <atomic>

namespace A3
{
namespace Extension
{
namespace Processor
{

class CProcessor //Todo not yet exception safe, maybe merge the processor from a standalone class into the extension base
{
public:
    CProcessor( A3::DataTypes::int8 nThreads );
    virtual ~CProcessor();

    void
    Start( std::function< std::vector< CProcessorResult >( CProcessorWorkload ) > oTreadFunction );

    void
    Add( CProcessorWorkload & roWorkload );

    bool
    try_get_results( std::vector<CProcessorResult> & oWorkload,
                     A3::DataTypes::uint64 nCurrentSize,
                     A3::DataTypes::int64 nMaxSize );

public:
    std::atomic_bool m_bActive;
    const A3::DataTypes::int8 m_nProcessorThreads;

protected:
    CProcessorQueue< CProcessorResult > oResultQueue;
    CProcessorQueue< CProcessorWorkload > oWorkloadQueue;

protected:
    void run( std::function< std::vector< CProcessorResult >( CProcessorWorkload ) > oFunction );
};

}; // end namespace Processor
}; // end namespace Extension
}; // end namespace A3

#endif //A3DB_PROCESSOR_HPP
