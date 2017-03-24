/*
		_	_____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/

[] spawn {

    sleep 10;

    arr = [];
    for "_i" from 0 to 999 do {
        [] spawn {
            _test = [10,10,1] call A3DB_fnc_query;
            arr pushBack _test;
        };
    };

    [] spawn {
        _time = diag_tickTime;
        waitUntil {count arr > 999};
        diag_log format["FINISHED IN %1", diag_tickTime - _time];
    };
};