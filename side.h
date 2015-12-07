#pragma once
#include <Windows.h>
#include <io.h>
#include <string>
#include "vec.h"
#include <iomanip>
#include <ctime>
#include <iostream>
#include <direct.h>

using std::string;
using std::cout;
using std::endl;





class side
{
public:
	side(string p, size_t x, size_t y);
	void show() const;
	bool mooveDown();
	bool mooveUp();
	bool inactive();
	bool active();
	bool enter();
	inline const string & getPath()
	{
		return path;
	}
	inline const _finddata_t & getActiveFInfo()
	{
		return files[indexFile];
	}
	static HANDLE hand;
	static const size_t countFiles;
	bool fillFiles();
	inline void clearIndex()
	{
		indexFile = 0;
		indexRow = 0;
		upIndexFile = 0;
	}
	void writePath() const;
	size_t getIndexByNameInCurrent(string name) const;
	void makeDir(string name);
private:
	void showTwoLines(size_t row) const;
	void loadFilesPath();
	void makePath();

	void makeLine(const _finddata_t &finfo, string &str) const;

	vec<_finddata_t> files;
	
	vec<string> filesPath;
	string path;
	
	size_t indexFile;
	size_t indexRow;
	size_t upIndexFile;
	
	size_t posX;
	size_t posY;
	
	bool isActive;
};

