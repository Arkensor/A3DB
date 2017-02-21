/*
_    _____  ____  ____
/ \  |___ / |  _ \| __ )
/ _ \   |_ \ | | | |  _ \
/ ___ \ ___)  | |_| | |_) |
/_/   \_\____/ |____/|____/

a cross database extension for Arma 3 by Arkensor
*/

#include <sstream>

#include "Extension.hpp"
#include "Processor.hpp"


Extension::Extension(std::string _name, std::string _version) : Name(_name), Version(_version) {

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

Extension::~Extension() {

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
	processor->start(std::bind(&Extension::worker, this, std::placeholders::_1), -1);

	workerActive = true;
	max_size = outputSize - 12; //Account for the extra characters in the array ([] , codes) etc

								//Everything should be good now
	allGood = true;
}

std::vector<Result> Extension::worker(Workload request)
{
	std::vector<Result> results;

	std::string res;

	//proof of concept, adds 1's to a string equal to the input int count (e.g 5000 1's)
	//int size = stoi(request.WorkloadData);
	//for (int i = 0; i < size; i++)
		//res += "1";
	res = "1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";
	//Need to find the most optimal max size to store, to if there is lots of big results, little results get returned too..
	unsigned int max_scaled_size = (unsigned int)(0.8 * max_size);
	unsigned int i = res.length() / max_scaled_size;
	bool multi_part = i != 0;
	while ((unsigned int)res.length() > max_scaled_size)
	{
		Result r(request.id, request.id_length, res.substr(0, max_scaled_size), true, i);
		results.push_back(r);
		res = res.substr(max_scaled_size);
		i--;
	}
	Result r(request.id, request.id_length, res, multi_part, multi_part ? 0 : -1);
	results.push_back(r);

	return results;
}

int Extension::call(char *output, int outputSize, const char *function, const char **args, int argsCnt) {

	//my really fucking cool variable
	int g = 0;

	outputSize--;

	if (!allGood) {
		//Block all requets if there was an error in the extension
		strncpy(output, "There was an error when loading the extension please see logfiles for more information ...", outputSize);
		return 500;
	}

	if (!workerActive) {
		this->setup(outputSize);
		if (!workerActive) { allGood = false; }
	}

	int t = g + 1;

#ifdef CONSOLE_DEBUG
	//console->info("The function '{0}' was called", function);
#endif

	if (!strcmp(function, "version")) {
		strncpy(output, Version.c_str(), outputSize);
		return 1;
	}
	else {

		//std::vector<int> addedIDs;
		if (argsCnt)
			this->addRequest(function, args, argsCnt);
		//addedIDs = this->addRequest(function, args, argsCnt);

		int current_size = 2;
		//bool has_returned_id = false;
		std::string ret = "[";

		/*
		//Check for ID returns
		if (!addedIDs.empty())
		{
		for (int id : addedIDs)
		{
		current_size += digitsCnt(id) + 1;
		ret += std::to_string(id);
		ret += ",";
		}
		current_size--;
		has_returned_id = true;
		}
		*/

		//Check for results
		std::vector<Result> results;
		if (this->checkResults(results, current_size))
		{
#ifdef CONSOLE_DEBUG
			console->info("Outputting {0} results", results.size());
#endif
			for (Result r : results)
			{
				ret += result_to_string(r);
				ret += ",";
			}
			ret.pop_back();
		}
		/*
		} else {
		if (has_returned_id)
		{
		ret.pop_back();
		}
		}
		*/
		ret += "]";
		strncpy(output, ret.c_str(), outputSize);
	}
	return 2;
}

void Extension::addRequest(const char *function, const char **args, int argCnt) {
	//std::vector<int> ids;
	for (int i = 0; i < argCnt; i++) {
#ifdef CONSOLE_DEBUG
		console->info(args[i]);
#endif
		//int id = ++ticketID;
		//ids.push_back(id);

		std::string request_string(args[i]);
		request_string = request_string.substr(1, request_string.size() - 2);
		std::vector<std::string> parts = delimiter_split(request_string, 31);

		if (parts.size() < 3)
		{
			//Exit with an error (needs atleast ID/type/query)
		}

		std::string ticket_id(parts[0]);
		std::string type(parts[1]);
		std::string query(parts[2]);
		
		int ticket_id_len = ticket_id.size();

		int ticket_id_len = ticket_id.size();
/*
#ifdef CONSOLE_DEBUG
		console->info("A new Workload incoming:");
		console->info("Type: {0}", type);
		console->info("Query: {0}", query);
		console->info("ID: {0}", ticket_id);
#endif
*/

		if (parts.size() > 3)
		{
			std::vector<std::string> arguments(parts.begin() + 3, parts.end());

#ifdef CONSOLE_DEBUG
			/*
			console->info("Arguments: ");
			for (auto s : arguments)
				console->info(s);
			*/
#endif

			processor->add(Workload(ticket_id, ticket_id_len, type, query, arguments));
		}
		else
		{
			processor->add(Workload(ticket_id, ticket_id_len, type, query));
		}

#ifdef CONSOLE_DEBUG
		console->info("\n");
#endif

	}
	//return ids;
}

bool Extension::checkResults(std::vector<Result>& results, int current_size) {
	return processor->try_get_results(results, current_size, this->max_size);
}

std::vector<std::string> Extension::delimiter_split(const std::string &str, char delimiter) {

	std::vector<std::string> ret;
	std::stringstream string_stream(str);
	std::string next_string;

	while (std::getline(string_stream, next_string, delimiter))
		ret.push_back(std::move(next_string));

	return ret;
}

std::string Extension::result_to_string(Result res)
{
	std::string ret = "[";
	ret += res.id;
	ret += ",";
	ret += res.IsMultiPart ? std::to_string(res.PartIndex) : "-1";
	ret += ",";
	ret += "\"";
	ret += res.ResultData;
	ret += "\"";
	ret += "]";
	return ret;
}