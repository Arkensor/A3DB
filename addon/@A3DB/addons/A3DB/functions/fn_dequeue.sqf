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

private _allResultsFetched = false;
private _parts = [];
private _result = "";
private _last_index = -2;
while {!_allResultsFetched} do {
	{
		if((_x select 0) isEqualTo _ticketID) then {

			private _index = _x select 1;

			if (_last_index isEqualTo (_index + 1) || {_last_index isEqualTo -2}) then {
				_last_index = _index;
				_parts pushBack (_x select 2);
			};

			if(_index isEqualTo 0 || {_index isEqualTo -1}) then {
				_allResultsFetched = true;
			};
		};
		if (_allResultsFetched) exitWith {};
		true
	} count A3DB_OUTPUT_QUEUE;
};

{
	_result = _result + _x;
	true
} count _parts;

A3DB_DELETE_QUEUE pushBack _ticketID;

_result;
