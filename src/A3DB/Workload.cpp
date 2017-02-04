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

//Todo das geht nicht so recht
std::string Workload::Trim(std::string s) {
	size_t p = s.find_first_not_of(" \t\"");
	s.erase(0, p);

	p = s.find_last_not_of(" \t\"");
	if (std::string::npos != p)
		s.erase(p + 1);

	return s;
}