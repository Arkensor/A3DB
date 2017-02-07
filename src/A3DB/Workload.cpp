/*
		_    _____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/

#include "Workload.hpp"

Workload::Workload(int _id, const char *category, const char *request) {
	id = _id;
	WorkloadCategory = category;
	WorkloadData = std::string(request);
}