/*
		_    _____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/
[] spawn {

    A3DB_INPUT_QUEUE = [];
    A3DB_OUTPUT_QUEUE = [];
    A3DB_DELETE_QUEUE = [];
    A3DB_TICKET_ID = 0;
    A3DB_BASE_FRAMETICK = 5;
    A3DB_LOOP_COUNT = 0;

    while {true} do {

        if (A3DB_LOOP_COUNT >= 35) then {
            if !((count A3DB_DELETE_QUEUE) isEqualTo 0) then {

			    isNil {
				    {
					    private _toDelete = _x;
					    {
						    if((_x select 0) isEqualTo _toDelete) then {
							    A3DB_OUTPUT_QUEUE = A3DB_OUTPUT_QUEUE - [_x];
						    };
						    true
					    } count +A3DB_OUTPUT_QUEUE;
					    true
				    } count A3DB_DELETE_QUEUE;
				    A3DB_DELETE_QUEUE = [];
			    };
            };
            A3DB_LOOP_COUNT = 0;
        };

        if !((count A3DB_INPUT_QUEUE) isEqualTo 0) then {

			private _data = [];

			isNil {

				{
					_data pushBack _x;
					true
				} count A3DB_INPUT_QUEUE;

				A3DB_INPUT_QUEUE = [];

			};

            private _result = "A3DB" callExtension ["INPUT",_data];

			_result = parseSimpleArray (_result select 0);

			if !(_result isEqualTo []) then {
                {
                    A3DB_OUTPUT_QUEUE pushBack _x;
				    true
                } count _result;
			};

        } else {
            private _result = "A3DB" callExtension ["CHECK",[]];

            _result = parseSimpleArray (_result select 0);

            if !(_result isEqualTo []) then {
                {
                    A3DB_OUTPUT_QUEUE pushBack _x;
                    true
                } count _result;
            };

            //sleep (A3DB_BASE_FRAMETICK/diag_fps);
        };
        A3DB_LOOP_COUNT = A3DB_LOOP_COUNT + 1;
    };
};
