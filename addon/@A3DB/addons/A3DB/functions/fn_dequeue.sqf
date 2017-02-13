/*
		_    _____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/
params [
    ["_ticketID", 0, [0]]
];

if (_ticketID isEqualTo 0) exitWith {diag_log "********_tickedID was 0!"};

//Fetch all results
private _allResultsFetched = false;
private _parts = [];
private _result = "";
while {!_allResultsFetched} do {
    {
        if((_x select 0) isEqualTo _ticketID) then {
            _parts pushBack (_x select 2);

            private _index = _x select 1;
            if(_index isEqualTo 0 || {_index isEqualTo -1}) then {
                _allResultsFetched = true;
            };
        }
        if (_allResultsFetched) exitWith {};
        true
    } count +A3DB_OUTPUT_QUEUE;
};

//Put the parts together
{
    _result = _result + _x;
    true
} count _parts;

//Tell the man in the middle to clean up this ticket id now
//waitUntil{!A3DB_DELETE_LOCKED};
A3DB_DELETE_QUEUE pushBack _ticketID;

call compile _result;
