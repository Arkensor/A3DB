/**********************************************************************************************************************\

    DESCRIPTION: A3DB - A database extension for ArmA 3

------------------------------------------------------------------------------------------------------------------------

    CREATION DATE:  01.01.2018

------------------------------------------------------------------------------------------------------------------------

    Copyright © 2018
    Paul L. (https://github.com/Arkensor)
    Ollie W. (https://github.com/BoGuu)
    All rights reserved!

\**********************************************************************************************************************/

waitUntil{ !isNil "A3DB_NS" };

diag_log "A3DB: Benchmark starting in 5 seconds ...";

uiSleep 5;

g_nCallingThreads = 1;//254; //Simulating n requests from different sources like remoteexec on your server which all run in parallel
g_nCallsEach      = 2;//3;   //How many calls does each of the threads do for the simulation

g_oResults = [];

[] spawn
{
    private _time = diag_tickTime;

    waitUntil { ( count g_oResults ) >= ( g_nCallingThreads * g_nCallsEach ) };

    private _result = format [ "A3DB: %1 request(s) (%2 threads with %3 requests each) in %4 seconds(s) - %5 seconds per request -> The extension was called %6 times.",
                         ( count g_oResults ),
                         g_nCallingThreads,
                         g_nCallsEach,
                         ( diag_tickTime - _time ),
                         ( diag_tickTime - _time ) / ( g_nCallingThreads * g_nCallsEach ),
                         ( A3DB_NS getVariable [ "DEBUG_EXTENSION_CALLS_COUNT", "undefined" ] ) ];

    diag_log _result;
    diag_log format[ "A3DB: Return results: %1", g_oResults ];
};

for "_i" from 1 to g_nCallingThreads do
{
    [] spawn
    {
        for "_i" from 1 to g_nCallsEach do
        {
            private _result = [ "this is some sample data ...", 1 ] call A3DB_fnc_request;
            g_oResults pushBack _result;
        };
    };
};