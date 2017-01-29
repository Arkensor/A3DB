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

#include "safeQueue.h"
#include "Workload.h"

class Extension {
public:
	std::string Name;
	std::string Version;

	//Constructor
	Extension(std::string n, std::string v);
	~Extension();

	//Public functions for extension class interaction
	void addRequest(const char *function, const char **args, int argCnt);
	int  checkResults(char *output, int outputSize);

private:
	//Console Interaction
	void print(std::string message);

	// Request Mangement
	bool workerActive = false;
	bool shutDown = false;
	SafeQueue<Workload> queue;
	void setup();
	void worker();

	//void MySQLquery(std::string WorkloadCategory, std::vector<std::string> WorkloadData);
};