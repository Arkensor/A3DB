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
    A3DB_INPUT_LOCKED = false;
    A3DB_DELETE_LOCKED = false;
    A3DB_NEXT_TICKET = 1;
    A3DB_BASE_FRAMETICK = 5;

    while {true} {

        //Cleanup the output queue and remove things that belong to the ticketids that shall be deleted //Todo make a better script than this array = array - [thingy].
        if(count(A3DB_DELETE_QUEUE) != 0) then {
            A3DB_DELETE_LOCKED = true;
            {
                private _toDelete = _x;
                {
                    if((_x select 0) isEqualTo _toDelete) then {
                        A3DB_OUTPUT_QUEUE = A3DB_OUTPUT_QUEUE - [_x];
                    };
                } forEach A3DB_OUTPUT_QUEUE;
            } forEach A3DB_DELETE_QUEUE;
            A3DB_DELETE_QUEUE = [];
            A3DB_DELETE_LOCKED = false;
        }

        //Check if there should be new input
        if(count(A3DB_INPUT_QUEUE) != 0) then {

            A3DB_INPUT_LOCKED = true;
            private _data = [];
            {
                _data pushBack _x;
            } forEach A3DB_INPUT_QUEUE;
            A3DB_INPUT_QUEUE = []; //Make it emtpy again, since we input it all
            A3DB_INPUT_LOCKED = false;

            //Now call the extension and fetch results
            private _result = "A3DB" callExtension ["INPUT",_data];

            {
                A3DB_OUTPUT_QUEUE pushBack _x;
            } forEach (parseSimpleArray _result);

        } else {
            //No new input, ok but still check once in a while and start it all over again then
            "A3DB" callExtension ["CHECK",[]]

            sleep (A3DB_BASE_FRAMETICK/diag_fps);
        };

    };
};