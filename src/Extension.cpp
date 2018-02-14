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

#include "Framework/ExtensionBase.hpp"

namespace A3
{
namespace Extension
{

/** Setup is called once before the first workload is processed
 * @Return: void
 */
void
CExtensionBase::Setup()
{

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

    return "A3EXT rocks!";
}

}; // end namespace Extension
}; // end namespace A3