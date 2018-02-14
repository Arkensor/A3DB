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

class CfgPatches
{
    class A3DB_Benchmark
    {
		requiredVersion = 1.67;
		requiredAddons[] = { "A3DB" };
        fileName = "A3DB_Benchmark.pbo";
        author = "Arkensor";
    };
};

class CfgFunctions
{
    class A3DB_Benchmark
    {
        tag = "A3DB_Benchmark";

        class Functions 
        {
            file = "\A3DB_Benchmark\functions";
			class benchmark { postInit = 1; headerType = -1; };
        };
    };
};