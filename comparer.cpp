#include "comparer.h"

#include <fstream>
#include <io.h>
#include <iostream>
#include <algorithm>

using std::ifstream;
using std::cout;
using std::endl;

comparer::comparer(string tfile, string folder) : tFile(tfile), folder(folder), countInFolder(0), countInList(0)
{	
}

bool comparer::load()
{
	ifstream file(tFile.c_str(), std::ios::in);
	if (!file.is_open())
	{
		errorLoading = "ERROR with open file: " + tFile + '\n';
		return false;
	}

	file.seekg(0);
	char buff[200] = {};
	
	while (!file.eof())
	{
		file.getline(buff, 199);
		if (strlen(buff))
			vecTextFile.emplace_back(buff);
	}
	if (vecTextFile.size()==0)
	{
		errorLoading = "ERROR. We've opened the file, but it's empty!";
		return false;
	}
	file.close();
	// vecTextFile loaded!


	_finddata_t finfo;
	intptr_t done = _findfirst((folder + "\\*.*").c_str(), &finfo);
	if (done == -1)
	{
		_findclose(done);
		errorLoading = "ERROR with grabing files, full path:" + folder + "\\*.*";
		return false;
	}
	
	if (_stricmp(finfo.name, ".") && _stricmp(finfo.name, "..")) 
		vecFolder.emplace_back(finfo.name);

	while (_findnext(done, &finfo) != -1)
	{
		if (_stricmp(finfo.name, ".") && _stricmp(finfo.name, ".."))
			vecFolder.emplace_back(finfo.name);
	}
	_findclose(done);
	
	return true;
}

void comparer::printContainersToCMD()
{
	cout << "Text file strings:\n";
	for (auto a : vecTextFile)
		cout << a << endl;
	cout << "-----------------";

	cout << "\n\nList of files:\n";
	for (auto a : vecFolder)
		cout << a << endl;
	cout << "-----------------";

}

bool comparer::compareToCMD()
{
	if (!isUniqueLists())
		return false;

	 myIntersection();

	cout << "--------Intersection:---------\n";
	for (auto a : vecRes)
		cout << a << endl;
	cout << "------------------------\n";

	cout << "--------TextFile problem:---------\n";
	for (auto a : vecTextFile)
		cout << a << endl;
	cout << "------------------------\n";

	cout << "--------Folder problem:---------\n";
	for (auto a : vecFolder)
		cout << a << endl;
	cout << "------------------------\n";

	return true;

}

void comparer::myIntersection()
{

	vecRes.clear();
	for (size_t a = 0; a < vecTextFile.size(); ++a)
	{
		for (size_t b = 0; b < vecFolder.size(); ++b)
		{
			if (!vecTextFile.at(a).compare(vecFolder.at(b)))
			{
				vecRes.push_back(vecTextFile.at(a));
				vecTextFile.erase(vecTextFile.cbegin() + a);
				vecFolder.erase(vecFolder.cbegin() + a);
				--a;
				--b;
				break;
			}
		}
	}


	
}

bool comparer::isUnique(vecS &test)
{
	if (test.size() == 0)
	{
		errorLoading = "List is not unique!";
		return false;
	}
	else if (test.size() == 1)
		return true;

	for (size_t a = 0; a < test.size()-1; ++a)
	{
		for (size_t b = a + 1; b < test.size(); ++b)
		{
			if (!test.at(a).compare(test.at(b)))
			{
				errorLoading = "List is not unique. problem with: " + test.at(a) + " in " + std::to_string(a)
					+ " pos and in " + std::to_string(b) + " pos!\n";
				return false;
			}
		}
	}
	return true;
}

bool comparer::isUniqueLists()
{
	if (isUnique(vecTextFile) && isUnique(vecFolder))
		return true;
	else
		return false;
}

