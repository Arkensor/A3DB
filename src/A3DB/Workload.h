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

class Workload {
public:
	//Attributes
	std::string WorkloadCategory;
	std::vector<std::string> WorkloadData;

	//Constructor, no destructor overload needed here
	Workload(const char *function, const char **args, int argCnt);

	//Utils
	std::string Trim(std::string s);
};