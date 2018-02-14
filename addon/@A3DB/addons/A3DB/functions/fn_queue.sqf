/**********************************************************************************************************************\

    DESCRIPTION: Queue that runs on the server handling input and output into/from the extension

------------------------------------------------------------------------------------------------------------------------

    CREATION DATE:  01.01.2018

------------------------------------------------------------------------------------------------------------------------

    Copyright © 2018
    Paul L. (https://github.com/Arkensor)
    Ollie W. (https://github.com/BoGuu)
    All rights reserved!

\**********************************************************************************************************************/

[] spawn
{
    if( !isNil "A3DB_NS" ) exitWith
    {
        diag_log "ERROR: A3DB could not be loaded, as a different extension used the same namespace.";
    };

    A3DB_NS = createLocation [ "Hill", [ -1000, -1000, 0 ], 0, 0 ];

    A3DB_NS setVariable [ "BASE_FRAMETICK", 5 ];
    A3DB_NS setVariable [ "TICKET_ID", 0 ];
    A3DB_NS setVariable [ "INPUT_QUEUE", [] ];
    A3DB_NS setVariable [ "RESULTS_EXPECTED", 0 ];

    private _buffer = "";

    private _debug = isClass( configFile >> "CfgPatches" >> "A3DB_Benchmark" );

    private _parseResult =
    {
        params
        [
            ["_data", "[]", [ "" ]],
            ["_returnCode", -1, [ 0 ]],
            ["_errorCode", 500, [ 0 ]]
        ];

        if ( _debug ) then
        {
            A3DB_NS setVariable [ "DEBUG_EXTENSION_CALLS_COUNT", ( A3DB_NS getVariable [ "DEBUG_EXTENSION_CALLS_COUNT", 0 ] ) + 1 ];
        };

        if ( !( _returnCode isEqualTo 0 ) || !( _errorCode isEqualTo 0 ) ) exitWith
        {
            diag_log format[ "A3DB: %1", _data ];

            A3DB_NS setVariable [ "EXTENSION_FAILURE", true ];
        };

        if ( _data isEqualTo "[]" ) exitWith {};

        _data = parseSimpleArray ( _data );

        if ( !( _data isEqualTo [] ) ) then
        {
            {
                _buffer = _buffer + ( _x param [ 2 ] );

                private _currentIndex = _x param [ 1 ];

                if ( ( _currentIndex isEqualTo 0 ) || { _currentIndex isEqualTo -1 } ) then
                {
                    A3DB_NS setVariable [ str( _x param [ 0 ] ), _buffer ];

                    private _results_exptected = A3DB_NS getVariable [ "RESULTS_EXPECTED", 0 ];

                    if( _results_exptected < 0 ) then
                    {
                        _results_exptected = 0;
                    };

                    A3DB_NS setVariable [ "RESULTS_EXPECTED", _results_exptected ];

                    _buffer = "";
                };

                true
            } count _data;
        };
    };

    for "_i" from 0 to 1 step 0  do
    {
        if ( A3DB_NS getVariable [ "EXTENSION_FAILURE", false ] ) exitWith
        {
            A3DB_NS = nil;
        };

        if !( count( A3DB_NS getVariable [ "INPUT_QUEUE", [] ] ) isEqualTo 0 ) then
        {
            private _data = [];

            isNil
            {
                {
                    _data pushBack _x;

                    private _arr = ( A3DB_NS getVariable [ "INPUT_QUEUE", [] ] );

                    if( count( _arr ) isEqualTo 0 ) exitWith {};

                    _arr deleteAt 0;

                    A3DB_NS setVariable [ "INPUT_QUEUE", _arr ];

                    A3DB_NS setVariable [ "RESULTS_EXPECTED", ( A3DB_NS getVariable [ "RESULTS_EXPECTED", 0 ] ) + 1 ];

                    if ( _forEachIndex isEqualTo 1023 ) exitWith {};

                } forEach +( A3DB_NS getVariable [ "INPUT_QUEUE", [] ] );
            };

            ( "A3DB" callExtension [ "BULK", _data ] ) call _parseResult;
        }
        else
        {
            if !( A3DB_NS getVariable [ "RESULTS_EXPECTED", 0 ] isEqualTo 0 ) then
            {
               ( "A3DB" callExtension [ "CHECK", [] ] ) call _parseResult;
            };

            sleep ( ( A3DB_NS getVariable [ "BASE_FRAMETICK", 5 ] ) / diag_fps );
        };
    };
};