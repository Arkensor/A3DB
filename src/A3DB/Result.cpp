/*
		_    _____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/

#include "Result.hpp"

Result::Result(int uid, std::string result, bool multipart, int index) {
	UniqueID = uid;
	ResultData = result;
	IsMultiPart = multipart;
	PartIndex = index;
}