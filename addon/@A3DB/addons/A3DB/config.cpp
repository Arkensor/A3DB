/*
		_	_____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/

class CfgPatches
{
	class A3DB
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"A3_Functions_F"};
		fileName = "A3DB.pbo";
		author = "Arkensor";
	};
};

class CfgFunctions
{
	class A3DB
	{
		tag = "A3DB";

		class Functions {

			file = "\A3DB\functions";

			class queue {preInit = 1; headerType = -1;};
			class enqueue {headerType = -1;};
			class dequeue {headerType = -1;};
			class query {headerType = -1;};

			//class startexamples {postInit = 1; headerType = -1;};
		};
	};
};