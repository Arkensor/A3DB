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

void Extension::setup(int outputSize) {

	if (workerActive) return;
	
	processor = new Processor<Workload, Result>(this);
	processor->start(std::bind(&Extension::process, this, std::placeholders::_1), -1);

	workerActive = true;
	max_size = outputSize - 2;

	//Everything should be good now
	allGood = true;
}

std::vector<Result> Extension::process(Workload request)
{
	std::vector<Result> results;

	std::string res = "Doggo";

	//Need to find the most optimal max size to store, to if there is lots of big results, little results get returned too..
	unsigned int max_scaled_size = (unsigned int)(0.8 * max_size);
	unsigned int i = 0;
	while ((unsigned int)res.length() > max_scaled_size)
	{
		Result r(request.id, res.substr(0, max_scaled_size), true, i);
		results.push_back(r);
		res = res.substr(max_scaled_size);
		i++;
	}
	Result r(request.id, res, i != 0, i);
	results.push_back(r);

	return results;
}

int Extension::call(char *output, int outputSize, const char *function, const char **args, int argsCnt) {

	outputSize--;

	if (!allGood) {
		strncpy(output, "There was an error when loading the extension please see logfiles for more information ...", outputSize);
		return 500;
	}

	if (!workerActive) {
		this->setup(outputSize);
		if (!workerActive){allGood = false;}
	}

	console->info("The function {0} was called", function);

	if (!strcmp(function, "version")) {
		strncpy(output, Version.c_str(), outputSize);
		return 1;
	} else {

		std::vector<int> addedIDs;
		if (argsCnt)
			addedIDs = this->addRequest(function, args, argsCnt);

		int current_size = 2;
		bool has_returned_id = false;
		std::string ret = "[";

		//Check for ID returns
		if (!addedIDs.empty())
		{
			for (int id : addedIDs)
			{
				current_size += number_of_digits(id) + 1;
				ret += std::to_string(id);
				ret += ",";
			}
			current_size--;
			has_returned_id = true;
		}

		//Check for results
		std::vector<Result> results;
		if (this->checkResults(results, current_size))
		{
			for (Result r : results)
			{
				ret += result_to_string(r);
				ret += ",";
			}
			ret.pop_back();
		} else {
			if (has_returned_id)
			{
				ret.pop_back();
			}
		}
		ret += "]";
		strncpy(output, ret.c_str(), outputSize);
	}
	return 0;
}

//http://stackoverflow.com/a/1489928
unsigned int Extension::number_of_digits(unsigned int i)
{
	return i > 0 ? (int)log10((double)i) + 1 : 1;
}

std::string Extension::result_to_string(Result res)
{
	std::string ret = "[";
	ret += std::to_string(res.UniqueID);
	ret += ",";
	ret += res.IsMultiPart ? std::to_string(res.PartIndex) : "-1";
	ret += ",";
	ret += res.ResultData;
	ret += "]";
	return ret;
}

std::vector<int> Extension::addRequest(const char *function, const char **args, int argCnt) {
	std::vector<int> ids;
	for (int i = 0; i < argCnt; i++) {
		int id = ++ticketID;
		ids.push_back(id);

		console->info("\n");
		console->info("A new Workload incoming:");
		console->info("Function: {0}", function);
		console->info("Arguement: {0}", args[i]);
		console->info("Assigned ID: {0}", id);

		processor->add(Workload(id, function, args[i]));
	}
	return ids;
}

bool Extension::checkResults(std::vector<Result>& results, int current_size) {
	return processor->try_get_results(results, current_size, this->max_size);
}