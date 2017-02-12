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
	int id_length;
	std::string WorkloadCategory;
	std::string WorkloadData;
	Workload(float _id, int _id_len, const char *category, std::string request) : id(_id), id_length(_id_len), WorkloadCategory(category), WorkloadData(request) {}
};
#endif