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
#include <vector>

class Result {
public:
	int UniqueID;
	std::string ResultData;
	bool IsMultiPart = false;
	int PartIndex;
	Result(int uid, std::string result, bool multipart = false, int index = 0);
};
#endif