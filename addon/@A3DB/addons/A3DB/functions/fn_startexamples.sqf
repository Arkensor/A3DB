/*
		_	_____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/

arr = [];
for "_i" from 0 to 99 do {
    [] spawn {
        _test = [10,10,1] call A3DB_fnc_query;
        arr pushBack _test;
    };
};
[] spawn {
    _time = diag_tickTime;
    waitUntil {count arr > 99};
    diag_log format["FINISHED IN %1", diag_tickTime - _time];
    {
        diag_log _x;
        true;
    } count arr;
};

/*

	private["_start", "_stop", "_result"];

	_start = diag_tickTime;
	_result = "A3DB" callExtension ["testcall",["par1", "par88", 1337]];
	_stop = diag_tickTime;
	diag_log format["Single -- Result: %1 - took %2ms", _result, (_stop - _start)];

	[] spawn {
		_start = diag_tickTime;
		_result = "";
			for "_i" from 1 to 10000 do {
				_result = "A3DB" callExtension ["testcall",["par1", "par88", 1337]];
			};

		_stop = diag_tickTime;
		diag_log format["10000 runs -- Result: %1 - took %2ms (%2 / 10000 = %3)", _result, (_stop - _start), ((_stop - _start) / 10000)];
	};
*/