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
#include <memory>

#ifdef _WINDOWS
	#include <Windows.h>
	#ifdef _DEBUG
		#define CONSOLE_DEBUG
		#include "spdlog/spdlog.h"
	#endif
#endif

#include "Workload.hpp"
#include "Result.hpp"

template<class T, class U>
class Processor;

class Extension {
public:
	std::string Name;
	std::string Version;

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

	int ticketID = 0;

	//Block all requets if there was an error in the extension
	bool allGood = true;

	// Request Mangement
	bool workerActive = false;
	bool shutDown = false;

	void setup();

	std::vector<int> addRequest(const char *function, const char **args, int argCnt);
	int checkResults(char *output, int outputSize);
	std::vector<Result> process(Workload request);

	//void MySQLquery(std::string WorkloadCategory, std::vector<std::string> WorkloadData);
};
#endif