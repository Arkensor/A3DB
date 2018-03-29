/**********************************************************************************************************************\

    DESCRIPTION: Waiting for the data to be returned

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
    [ "_ticketID", -1, [ 0 ] ]
];

if ( _ticketID isEqualTo -1 ) exitWith
{
    diag_log format[ "A3DB: A call to a non existing ticket was performed. Check of possible errors and data loss! -> TicketID: %1", _ticketID ];
};

private _stringTicket = str _ticketID;

waitUntil { !isNil{ A3DB_NS getVariable _stringTicket } };

private _result = A3DB_NS getVariable _stringTicket;

A3DB_NS setVariable [ _stringTicket, nil ];

_result;