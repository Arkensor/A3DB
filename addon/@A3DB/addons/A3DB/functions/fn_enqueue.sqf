/*
		_    _____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/
params [
    ["_data", "", ["", 0]],
    ["_type", "", ["", 0]]
];

private "_ticketID";
isNil {
	_ticketID = A3DB_TICKET_ID;
	A3DB_TICKET_ID = A3DB_TICKET_ID + 1;
};

private _sep = toString [31];
private _toAdd = format ["%2%1%3%1%4", _sep, _ticketID, _type, _data];

A3DB_INPUT_QUEUE pushBack format ["%2%1%3%1%4", _sep, _ticketID, _type, _data];

_ticketID
