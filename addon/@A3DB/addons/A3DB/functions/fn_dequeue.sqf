/*
		_	_____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/
params [
	["_ticketID", -1, [0]]
];

if (_ticketID isEqualTo -1) exitWith {diag_log "********_tickedID was -1!********"};

private _stringTicket = str _ticketID;

waitUntil {!isNil {A3DB_NS getVariable _stringTicket}};

_result = A3DB_NS getVariable _stringTicket;

A3DB_NS setVariable [_stringTicket, nil];

_result;