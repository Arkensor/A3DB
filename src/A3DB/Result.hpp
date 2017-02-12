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

class Result {
public:
	float id;
	std::string ResultData;
	bool IsMultiPart = false;
	int PartIndex;
	Result(float _id, std::string result, bool multipart = false, int index = 0) : id(_id), ResultData(result), IsMultiPart(multipart), PartIndex(index) {}
};
#endif