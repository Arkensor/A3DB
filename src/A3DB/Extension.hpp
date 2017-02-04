/*
		_    _____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/

#include <string>
#include <vector>
#include <memory>

#ifdef EXTENSION_DEBUG_OUTPUT_WINDOWS
	#include "spdlog/spdlog.h"
#endif

#include "SafeQueue.hpp"
#include "Workload.hpp"

class Extension {
public:
	std::string Name;
	std::string Version;

	//Constructor
	Extension(std::string _name, std::string _version);
	~Extension();

	//Public functions for extension class interaction

	int call(char *output, int outputSize, const char *function, const char **args, int argsCnt);

private:

	#ifdef EXTENSION_DEBUG_OUTPUT_WINDOWS
		std::shared_ptr<spdlog::logger> console;
	#endif

	//Block all requets if there was an error in the extension
	bool allGood = true;

	// Request Mangement
	bool workerActive = false;
	bool shutDown = false;
	SafeQueue<Workload> queue;

	void setup();
	void worker();

	void addRequest(const char *function, const char **args, int argCnt);
	int  checkResults(char *output, int outputSize);

	//void MySQLquery(std::string WorkloadCategory, std::vector<std::string> WorkloadData);
};