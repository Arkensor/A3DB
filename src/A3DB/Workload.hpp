/*
		_    _____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/

#ifndef __WORKLOAD_H_
#define __WORKLOAD_H_

#include <string>
#include <vector>

class Workload {
public:
	float id;
	std::string WorkloadCategory;
	std::string WorkloadData;
	Workload(int _id, const char *category, std::string request) : id(_id), WorkloadCategory(category), WorkloadData(request) {}
};
#endif