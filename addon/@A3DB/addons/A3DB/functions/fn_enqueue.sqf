/*
		_    _____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/
params [
    ["_data", [], [[]]],
    ["_type", 0, [0]],
    ["_return", 1, [0]]
];
//waitUntil{!A3DB_INPUT_LOCKED};
private _ticketID = diag_tickTime;
A3DB_INPUT_QUEUE pushBack [_ticketID, _type, _return, _data];

_ticketID
