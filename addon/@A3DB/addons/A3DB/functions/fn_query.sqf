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
    [ "_return", 1, [ 0 ] ],
    [ "_connection", "", [ "" ] ],
    [ "_query", "", [ "" ] ],
    [ "_data", [], [ [] ] ]
];

if( !canSuspend ) exitWith
{
    diag_log "A3DB: Suspension is required to wait for the databse results! To find out more about suspension visit this link: https://community.bistudio.com/wiki/Scheduler#Suspension";
    diag_log format[ "A3DB: The query that should have been executed was: ", _query ];

    if( _return isEqualTo 1 ) exitWith
    {
        [];
    };
};

if( ( _query isEqualTo "" ) || ( _connection isEqualTo "" ) ) exitWith
{
    [];
};

private _request = format[ "%2%1%3", toString [ 29 ], _connection, _query ];

{
    _request = format[ "%2%1%3", toString [ 29 ], _request, _x ];
    false;
} count _data;

private _ticketID = [ _request, _return ] call A3DB_fnc_enqueue;

if ( ( _return isEqualTo 1 ) && { !( _ticketID isEqualTo -1 ) } ) exitWith
{
    private _queryResult = [ _ticketID ] call A3DB_fnc_dequeue;

    parseSimpleArray _queryResult;
};