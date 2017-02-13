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

private _ticketID = [_data,_type,_return] call A3DB_fnc_enqueue;

//We want to have a return value
if (_return isEqualTo 1) then {

    _result = [_ticketID] call A3DB_fnc_dequeue;

    diag_log format["RESULT: %1", _result];

    _result;
};