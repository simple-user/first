#pragma once
#include <string>
#include "vec.h"

using std::string;

class comparer
{
public:
	typedef vec<string> vecS;
	comparer(string tfile, string folder);

	bool load();
	bool isUniqueLists();
	bool compareToCMD();
	void printContainersToCMD();
	string getError()
	{
		return errorLoading;
	}
private:
	string tFile;
	string folder;
	
	size_t countInList;
	size_t countInFolder;
	
	string errorLoading;
	
	vec<string> vecTextFile;
	vec<string> vecFolder;
	vec<string> vecRes;

	void myIntersection();
	bool isUnique(vecS &test);
};

