/*
		_	_____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/
[] spawn {

    A3DB_INPUT_QUEUE = [];
    A3DB_TICKET_ID = 0;
    A3DB_BASE_FRAMETICK = 5;

    //Create NS
    A3DB_NS = createLocation ["Hill", [-1000, -1000, 0], 0, 0];

    private _parseResult = {
        params [
            ["_data", [], [[]]]
        ];

        if (_data isEqualTo []) exitWith {};
        _data = parseSimpleArray (_data select 0);

        if !(_data isEqualTo []) then {

            private _buffer = "";

            {
                _buffer = _buffer + (_x select 2);

                private _currentIndex = _x select 1;
                if (_currentIndex isEqualTo 0 || {_currentIndex isEqualTo -1}) then {
                    A3DB_NS setVariable [str (_x select 0), _buffer];
                    _buffer = "";
                };
                true;
            } count _data;
        };
    };

    while {true} do {

        if !((count A3DB_INPUT_QUEUE) isEqualTo 0) then {

			private _data = [];

			isNil {

				{
					_data pushBack _x;
					true;
				} count A3DB_INPUT_QUEUE;

				A3DB_INPUT_QUEUE = [];
			};

            private _result = "A3DB" callExtension ["INPUT",_data];
			[_result] call _parseResult;

        } else {

            private _result = "A3DB" callExtension ["CHECK",[]];
            [_result] call _parseResult;

            sleep (A3DB_BASE_FRAMETICK/diag_fps);
        };
    };
};