/*
		_	_____  ____  ____
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
	_ticketID = (A3DB_NS getVariable ["TICKET_ID", 0]);
	A3DB_NS setVariable ["TICKET_ID", _ticketID+1];
};
private _arr = (A3DB_NS getVariable ["A3DB_INPUT_QUEUE", []]);
_arr pushBack format ["%2%1%3%1%4", toString [31], _ticketID, _type, _data];
A3DB_NS setVariable ["A3DB_INPUT_QUEUE", _arr];

_ticketID;