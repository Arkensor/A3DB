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
		strcpy_s(output, outputSize - 1, "There was an error when loading the extension please see logfiles for more information ...");
		return 500;
	}

	if (!workerActive) {
		this->setup();
		if (!workerActive){allGood = false;}
	}

	if (!strcmp(function, "version")) {
		strcpy_s(output, outputSize - 1, Version.c_str());
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
	strcpy_s(output, outputSize - 1, "There were not ready results. Greetings Doggo");
	return 404;
}

/*
	void Extension::MySQLquery(std::string WorkloadCategory, std::vector<std::string> WorkloadData){
	//mysql testing first

	MYSQL       *connection = mysql_init(NULL);
	MYSQL_RES	*result;
	MYSQL_ROW	row;

	if (!mysql_real_connect(connection, "local.pi", "root", "Gammel003GG", "doggo_db", 3306, 0, 0)){
	this->print("Connection to the server failed\n");
	return;
	}

	mysql_query(connection, "SELECT * FROM duggos");
	result = mysql_store_result(connection);


	// get the number of the columns
	int num_fields = mysql_num_fields(result);
	// Fetch all rows from the result
	while ((row = mysql_fetch_row(result)))
	{
	// Print all columns
	for (int i = 0; i < num_fields; i++)
	{
	// Make sure row[i] is valid!
	if (row[i] != NULL){
	this->print(row[i]);
	this->print("\n");
	}
	else {
	this->print("Null");
	this->print("\n");
	}

	// Also, you can use ternary operator here instead of if-else
	// cout << row[i] ? row[i] : "NULL" << endl;
	}
	}

	// DON'T FORGET TO CLEAN RESULT AFTER YOU DON'T NEED IT
	// ANYMORE

	if (result != NULL) {
	mysql_free_result(result);
	}

	// disconnect from server
	mysql_close(connection);
	}
*/