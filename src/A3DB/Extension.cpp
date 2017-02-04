/*
		_    _____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/

#include "Extension.hpp"
#include "mysql.h"

Extension::Extension(std::string _name, std::string _version) {
	Name = _name;
	Version = _version;

	#ifdef EXTENSION_DEBUG_OUTPUT_WINDOWS
		AllocConsole();
		SetConsoleTitle(TEXT("Console output"));
		FILE *stream;
		freopen_s(&stream, "CONOUT$", "w", stdout);

		auto console_temp = spdlog::stdout_logger_mt("Console");
		console.swap(console_temp);
		console->info("{0} version {1} initialized ...", Name, Version);
	#endif
}

Extension::~Extension(){

	//Close down all connections
	//Send shutting down ... to logfile

	shutDown = true;

	#ifdef EXTENSION_DEBUG_OUTPUT_WINDOWS
		console->info("{0} shutting down ...", Name);
	#endif
}

void Extension::setup(){

	if (workerActive){
		return;
	}
	
	//Setup the workers - currently one of them
	std::thread workerthread(&Extension::worker, this);
	workerthread.detach();
	workerActive = true;

	//Everything should be good now
	allGood = true;

	#ifdef EXTENSION_DEBUG_OUTPUT_WINDOWS
		console->info("Worker threads have been loaded . . .");
	#endif
}

void Extension::worker() {
	while (!shutDown){
		Workload currentWorkload = queue.dequeue();

		#ifdef EXTENSION_DEBUG_OUTPUT_WINDOWS
			//Debug notice
			console->info("\n=== New Workload is beeing worked on ===");
			console->info("Category: " + currentWorkload.WorkloadCategory);
			for (unsigned int i = 0; i < currentWorkload.WorkloadData.size(); i++){
				console->info("Arguement[{0}]: {1}", i, currentWorkload.WorkloadData[i]);
			}
		#endif
	}
}

int Extension::call(char *output, int outputSize, const char *function, const char **args, int argsCnt){

	if (!allGood){
		strcpy_s(output, outputSize, "There was an error when loading the extension please see logfiles for more information ...");
		return 500;
	}

	if (!workerActive){
		this->setup();
		if (!workerActive){allGood = false;}
	}

	if (strcmp(function, "version")){
		strcpy_s(output, outputSize, Version.c_str());
		return 1;
	}
	else {
		this->addRequest(function, args, argsCnt);
		return this->checkResults(output, outputSize);
	}
}

void Extension::addRequest(const char *function, const char **args, int argCnt) {
	queue.enqueue(Workload(function, args, argCnt));
}

int Extension::checkResults(char *output, int outputSize) {
	strcpy_s(output, outputSize, "There were not ready results. Greetings Doggo");
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
