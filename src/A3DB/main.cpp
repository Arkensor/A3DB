/*
		_    _____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor and BoGuu
*/

#include "Extension.hpp"

Extension *instance;

#ifdef __GNUC__

	//For linux use GCC compiler syntax to for lib attach and detach
	static void __attribute__((constructor)) extension_init(void) {instance = new Extension("A3DB", VERSION);}
	static void __attribute__((destructor))	extension_destroy(void)	{delete instance;}

	extern "C"
	{
		void RVExtension(char *output, int outputSize, const char *function);
		int  RVExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argCnt);
		void RVExtensionVersion(char *output, int outputSize);
	}

	void __stdcall RVExtensionVersion(char *output, int outputSize)
	{
		strncpy_s(output, outputSize, VERSION, --outputSize);
	}

	void RVExtension(char *output, int outputSize, const char *function) {
		strncpy(output, "Please use the Array Input Variant", --outputSize);
	}

	int RVExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argCnt) {
		return instance->call(output, outputSize, function, args, argsCnt);
	}

#elif _MSC_VER

	int __stdcall DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
		switch (ul_reason_for_call)
		{
			case DLL_PROCESS_ATTACH:
				instance = new Extension("A3DB", VERSION);
			break;

			case DLL_PROCESS_DETACH:
				delete instance;
			break;
		}
		return 1;
	}

	extern "C" {
		__declspec (dllexport) void __stdcall RVExtension(char *output, int outputSize, const char *function);
		__declspec (dllexport) int  __stdcall RVExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argsCnt);
		__declspec (dllexport) void __stdcall RVExtensionVersion(char *output, int outputSize);
	}

	void __stdcall RVExtensionVersion(char *output, int outputSize)
	{
		strncpy(output, VERSION, --outputSize);
	}

	void __stdcall RVExtension(char *output, int outputSize, const char *function) {
		strncpy(output, "Please use the Array Input Variant", --outputSize);
	}

	int __stdcall RVExtensionArgs(char *output, int outputSize, const char *function, const char **args, int argsCnt) {
		return instance->call(output, outputSize, function, args, argsCnt);
	}

#endif