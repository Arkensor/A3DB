/*
		_    _____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/

#ifndef __RESULT_H_
#define __RESULT_H_

#include <string>
#include "ResultBase.hpp"

class Result : public ResultBase
{
public:
	bool IsMultiPart = false;
	int PartIndex;
	Result(std::string _id, std::string result, bool multipart = false, int index = 0) : IsMultiPart(multipart), PartIndex(index), ResultBase(_id, result) {}
};
#endif