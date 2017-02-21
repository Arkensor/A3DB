/*
		_	_____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/
//if (!canSuspend) exitWith {diag_log "Wrong context"};
params [
	["_data", "", ["", 0]],
	["_type", "", ["", 0]],
	["_return", 1, [0]]
];

private _ticketID = [_data,_type] call A3DB_fnc_enqueue;

if (_return isEqualTo 1) exitWith {

	private _result = [_ticketID] call A3DB_fnc_dequeue;
	_result;
};
