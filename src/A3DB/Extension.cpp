/*
		_    _____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/

//#include "mariadb\mysql.h"
#include "Extension.hpp"
#include "Processor.hpp"

Extension::Extension(std::string _name, std::string _version) {
	Name = _name;
	Version = _version;

	#ifdef CONSOLE_DEBUG
		AllocConsole();
		SetConsoleTitle(TEXT("Console output"));
		FILE *stream;
		freopen_s(&stream, "CONOUT$", "w", stdout);

		auto console_logger = spdlog::stdout_logger_mt("Console");
		console.swap(console_logger);
		console->info("{0} version {1} initialized ...", Name, Version);
	#endif
}

Extension::~Extension(){

	//Close down all connections
	//Send shutting down ... to logfile

	shutDown = true;

	#ifdef CONSOLE_DEBUG
		console->info("{0} shutting down ...", Name);
	#endif
}

void Extension::setup() {

	if (workerActive) return;
	
	processor = new Processor<Workload, Result>(this);
	processor->start(std::bind(&Extension::process, this, std::placeholders::_1), -1);

	workerActive = true;

	//Everything should be good now
	allGood = true;
}

std::vector<Result> Extension::process(Workload request)
{
	std::vector<Result> results;

	//TODO
	//process request query
	//check length of return
	//if > max size, split into multiple results, mark result as multi-part, supply index

	return results;
}

int Extension::call(char *output, int outputSize, const char *function, const char **args, int argsCnt) {

	if (!allGood) {
		strncpy(output, "There was an error when loading the extension please see logfiles for more information ...", outputSize);
		return 500;
	}

	if (!workerActive) {
		this->setup();
		if (!workerActive){allGood = false;}
	}

	if (!strcmp(function, "version")) {
		strncpy(output, Version.c_str(), outputSize);
		return 1;
	} else {
		auto addedIDs = this->addRequest(function, args, argsCnt);
		//add IDs to ret, and check for new results
		auto results = this->checkResults(output, outputSize);
	}
	return 0;
}

std::vector<int> Extension::addRequest(const char *function, const char **args, int argCnt) {
	std::vector<int> ids;
	for (int i = 0; i < argCnt; i++) {
		int id = ++ticketID;
		ids.push_back(id);
		auto req = Workload(id, function, args[i]);
		processor->add(req);
	}
	return ids;
}

int Extension::checkResults(char *output, int outputSize) {

	//processor->try_get_result
	//Keep trying to get results, while checking return size < max size
	//TODO
	strncpy(output, "There were not ready results. Greetings Doggo", outputSize);
	return 404;
}