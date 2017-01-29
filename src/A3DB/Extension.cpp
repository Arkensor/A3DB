/*
		_    _____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/

#ifdef _WINDOWS
	#include <Windows.h> //Windows specific for console
	#include <iostream>  //For console output
#endif

#include "Extension.h"
#include "mysql.h"

Extension::Extension(std::string n, std::string v) {
	Name = n;
	Version = v;

	#ifdef _WINDOWS
		#ifdef EXTENSION_DEBUG_OUTPUT
			AllocConsole();
			SetConsoleTitle(TEXT("Console output"));
			FILE *stream;
			freopen_s(&stream, "CONOUT$", "w", stdout);
			print(Name + " version " + Version + " has been loaded successfully\n");
		#endif
	#endif
}

Extension::~Extension(){

	//Close down all connections
	//Send shutting down ... to logfile
	//Closing console write stream

	shutDown = true;

	this->print(Name + " shutting down ...");
}

void Extension::print(std::string message) {
	#ifdef _WINDOWS
		#ifdef EXTENSION_DEBUG_OUTPUT
			std::cout << message;
		#endif
	#endif
}

void Extension::setup(){

	if (workerActive) 
		return;

	//Setup the workers - currently one of them
	std::thread workerthread(&Extension::worker, this);
	workerthread.detach();

	workerActive = true;

	this->print("Worker threads have been loaded . . .\n");
}

void Extension::worker() {
	while (!shutDown){
		Workload currentWorkload = queue.dequeue();

		//Debug notice
		this->print("\n=== New Workload is beeing worked on ===\n");
		this->print("Category: "+currentWorkload.WorkloadCategory+"\n");
		for (unsigned int i = 0; i < currentWorkload.WorkloadData.size(); i++){
			this->print("Arguement[" + std::to_string(i) + "]: " + currentWorkload.WorkloadData[i] + "\n");
		}
		this->print("\n");

	}
}

void Extension::addRequest(const char *function, const char **args, int argCnt) {
		
	if (!workerActive){
		this->setup();
	}

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
