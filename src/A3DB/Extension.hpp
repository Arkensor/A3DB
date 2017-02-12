/*
		_    _____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/

#ifndef __EXTENSION_H_
#define __EXTENSION_H_

#include <string>
#include <vector>

#ifdef _WINDOWS
	#include <Windows.h>
	#ifdef _DEBUG
		#define CONSOLE_DEBUG
	#endif
#elif __GNUC__
	#include <string.h>
#endif

#include "spdlog/spdlog.h"

#include "Workload.hpp"
#include "Result.hpp"

template<class T, class U>
class Processor;

class Extension {
public:
	std::string Name;
	std::string Version;

	bool allGood = true;
	bool shutDown = false;

	#ifdef CONSOLE_DEBUG
		std::shared_ptr<spdlog::logger> console;
	#endif

	//Constructor
	Extension(std::string _name, std::string _version);
	~Extension();

	//Public functions for extension class interaction
	int call(char *output, int outputSize, const char *function, const char **args, int argsCnt);

private:
	Processor<Workload, Result> *processor;

	// Request Mangement
	bool workerActive = false;
	int max_size;

	void setup(int outputSize);

	std::vector<int> addRequest(const char *function, const char **args, int argCnt);
	bool checkResults(std::vector<Result>& results, int current_size);
	std::string result_to_string(Result res);
	std::vector<Result> worker(Workload request);
	unsigned int digitsCnt(unsigned int i);
};
#endif