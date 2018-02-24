/**********************************************************************************************************************\

    DESCRIPTION: Full request with waiting for the results

------------------------------------------------------------------------------------------------------------------------

    CREATION DATE:  01.01.2018

------------------------------------------------------------------------------------------------------------------------

    Copyright © 2018
    Paul L. (https://github.com/Arkensor)
    Ollie W. (https://github.com/BoGuu)
    All rights reserved!

\**********************************************************************************************************************/

params
[
    [ "_data", "", [ "", 0 ] ],
    [ "_return", 1, [ 0 ] ]
];

if( !canSuspend ) exitWith
{
    diag_log "Suspension is required";
    [ "Suspension is required" ];
};

private _ticketID = [ _data, _return ] call A3DB_fnc_enqueue;

if ( ( _return isEqualTo 1 ) && { !( _ticketID isEqualTo -1 ) } ) exitWith
{
    [ _ticketID ] call A3DB_fnc_dequeue;
};