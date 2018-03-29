/**********************************************************************************************************************\

    DESCRIPTION: A3DB - A database extension for ArmA 3

------------------------------------------------------------------------------------------------------------------------

    CREATION DATE:  14.02.2018

------------------------------------------------------------------------------------------------------------------------

    Copyright © 2018
    Paul L. (https://github.com/Arkensor)
    Ollie W. (https://github.com/BoGuu)
    All rights reserved!

\**********************************************************************************************************************/

class CfgPatches
{
    class A3DB
    {
        requiredVersion = 1.67;
        fileName = "A3DB.pbo";
        author = "Arkensor";
    };
};

class CfgFunctions
{
    class A3DB
    {
        tag = "A3DB";

        class Functions 
        {
            file = "\A3DB\functions";

            class queue { preInit = 1; headerType = -1; };
            class enqueue { headerType = -1; };
            class dequeue { headerType = -1; };
            class query { headerType = -1; };
        };
    };
};