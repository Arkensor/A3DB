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

if (_ticketID isEqualTo -1) exitWith {diag_log "A call to a non existing ticket was performed. Check of possible errors and data loss!";};

private _stringTicket = str _ticketID;

waitUntil {!isNil {A3DB_NS getVariable _stringTicket}};

private _result = A3DB_NS getVariable _stringTicket;

A3DB_NS setVariable [_stringTicket, nil];

_result;