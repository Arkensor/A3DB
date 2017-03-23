#ifndef __RESULT_BASE_H
#define __RESULT_BASE_H

#include <string>

class ResultBase {
public:
	std::string id;
	std::string data;
	int id_length;

protected:
	ResultBase(std::string _id, std::string _data) : id(_id), data(_data), id_length(_id.size()) {}
};

#endif