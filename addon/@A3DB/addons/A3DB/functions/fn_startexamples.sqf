/*
		_    _____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/

/*
_result = "A3DB" callExtension ["fnc1", [1,"two",true,[4,"five",false]]];
diag_log _result;
//diag_log parseSimpleArray (_result select 0);
*/

[] spawn {

    for "_i" from 0 to 20 do{

        [["playerSelectGear",["playerid", "side", "whatever"]], "MYSQL_SELECT", true] spawn A3DB_fnc_query;

    };

};

/*
"A3DB" callExtension [
	"RAW",[
		"SELECT * from players where playerID = 742323423423",
		"SELECT * from players2 where playerID = 523525"
	]
];
*/

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