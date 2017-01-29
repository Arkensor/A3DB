/*
		_    _____  ____  ____
	   / \  |___ / |  _ \| __ )
	  / _ \   |_ \ | | | |  _ \
	 / ___ \ ___)  | |_| | |_) |
	/_/   \_\____/ |____/|____/

	a cross database extension for Arma 3 by Arkensor
*/

#include <string>
#include <vector>

class Result {
public:
	std::string UniqueID;
	std::vector<std::vector<std::string>> ResultData;
	Result(std::string uid, std::vector<std::vector<std::string>> result);
};