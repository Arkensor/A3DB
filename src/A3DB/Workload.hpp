/*
	    _    _____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor and BoGuu
*/

#ifndef __WORKLOAD_H_
#define __WORKLOAD_H_

#include <string>
#include <vector>

class Workload {
public:
	std::string id;
	int id_length;
	std::string Type;
	std::string WorkloadData;
	std::vector<std::string> Arguments;
	Workload(std::string _id, int _id_len, std::string _type, std::string _workloaddata) : id(_id), id_length(_id.size()), Type(_type), WorkloadData(_workloaddata) {}
	Workload(std::string _id, int _id_len, std::string _type, std::string _workloaddata, std::vector<std::string> _arguments) : id(_id), id_length(_id.size()), Type(_type), WorkloadData(_workloaddata), Arguments(_arguments) {}
};
#endif