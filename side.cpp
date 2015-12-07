#include "side.h"

HANDLE side::hand = GetStdHandle(STD_OUTPUT_HANDLE);
const size_t side::countFiles = 20;

side::side(string p, size_t x, size_t y) 
: indexFile(0), upIndexFile(0), indexRow(0), path(std::move(p)), posX(x), posY(y), isActive(true)
{
	if (!fillFiles())
		throw string("Can't read the directory: " + path);
	
	loadFilesPath();
}

void side::loadFilesPath()
{
	size_t lastPos = 0;
	size_t findedPos = 0;
	filesPath.clear();
	do
	{
		findedPos = path.find('\\', lastPos);
		filesPath.emplace_back(path, lastPos, findedPos - lastPos);
		if (findedPos == path.size()-1)
			break;
		lastPos = findedPos + 1;
	} while (findedPos != string::npos);

}

bool side::fillFiles()
{
	files.clear();
	_finddata_t finfo;
	intptr_t done = _findfirst((path + "*.*").c_str(), &finfo);
	if (done == -1)
	{
		_findclose(done);
		return false;
	}
	if (_stricmp(finfo.name, ".")) files.push_back(finfo);

	while (_findnext(done, &finfo) != -1)
		if (_stricmp(finfo.name, ".")) files.push_back(finfo);

	_findclose(done);

	return true;
}


void side::show() const
{
	if (!files.size())
		throw string("Nothing to show in func show()");

	string bufLine;
	for (size_t a = 0; a < countFiles; ++a)
	{
		SetConsoleCursorPosition(hand, { posX, posY + a });
		if (a+upIndexFile < files.size())
		{
			SetConsoleTextAttribute(hand, a + upIndexFile == indexFile ? 7 | 2 << 4 : 7);
			makeLine(files[a+upIndexFile], bufLine);
			cout << bufLine;
		}
		else
		{
			SetConsoleTextAttribute(hand, 7);
			printf_s("%c                   %c       %c          %c", 179, 179, 179, 179);
		}
	}

}

void side::showTwoLines(size_t row) const
{
	if (row + 1 + upIndexFile >= files.size())
		throw myException<4, string, size_t>{{ string("out of range in showTwoLines"), 0 }, 
		                                     { "Max index", files.size() - 1 }, 
											 { "upIndexFile: ", upIndexFile },
											 { "Argument row+1: ", row+1} };
	string bufLine;
	for (size_t a = 0; a < 2; ++a)
	{
		SetConsoleCursorPosition(hand, { posX, posY + a + row });
		SetConsoleTextAttribute(hand, a + row + upIndexFile == indexFile ? 7 | 2 << 4 : 7);
		makeLine(files[a + row + upIndexFile], bufLine);
		cout << bufLine;
	}
}

void side::makeLine(const _finddata_t &finfo, string &str) const
{
	str.clear();
	str += char(179);

	/*name*/
	if (strlen(finfo.name) > 19)
		str += string(finfo.name, 19);
	else
		str += finfo.name + string(19 - strlen(finfo.name), ' ');
	str += char(179);

	/*size*/

	string size;
	if (finfo.size >= 1000 * 1000 * 1000)  // > 1 gb
		size = std::to_string(finfo.size / (1024 * 1024 * 1024)) + ',' + std::to_string(finfo.size % (1024 * 1024 * 1024) / 1024 / 1024 *100 / 1024) + 'G';
	else if (finfo.size >= 1000 * 1000)  // > 1 mb < 1 gb
		size = std::to_string(finfo.size / (1024 * 1024)) + ',' + std::to_string(finfo.size % (1024 * 1024) * 100 / (1024 * 1024)) + 'M';
	else if (finfo.size >= 1000)  // > 1 kb < 1 mb
		size = std::to_string(finfo.size / 1024) + ',' + std::to_string(finfo.size % 1024 * 100 / 1024) + 'K';
	else // < 1 kb
		size = std::to_string(finfo.size) + 'B';

	if (size.size() > 7)
		throw string("lenght of str:size > 7 : ") + size;

	if (size.size() < 7)
		size.insert(0, 7 - size.size(), ' ');

	str += size + char(179);

	/*date*/
	tm t;
	localtime_s(&t, &finfo.time_write);
	if (t.tm_mday < 10)
		str += '0';
	str += std::to_string(t.tm_mday) + '.';
	if (t.tm_mon < 9)
		str += '0';
	str += std::to_string(t.tm_mon + 1) + '.' + std::to_string(t.tm_year + 1900);
	str += char(179);


}


bool side::mooveDown()
{
	if (indexFile == files.size() - 1)
		return false;
	else if (indexRow == countFiles - 1) // у самому нижньому положенні
	{
		++upIndexFile;
		++indexFile;
		show();      // треба переписати весь список
	}
	else  // просто спустити курсор на 1
	{
		++indexRow;
		++indexFile;
		showTwoLines(indexRow - 1);
	}
	return true;
}

bool side::mooveUp()
{
	if (indexFile == 0)
		return false;
	else if (indexRow == 0)
	{
		--upIndexFile;
		--indexFile;
		show();
	}
	else
	{
		--indexRow;
		--indexFile;
		showTwoLines(indexRow);
	}
	return true;
}

bool side::enter()
{
	if (filesPath.size() < 2 && !(_stricmp(files[indexFile].name, "..")))
		return false;
	else if (!(_stricmp(files[indexFile].name, "..")))  //..
	{
		string tName(filesPath.back());
		filesPath.pop_back();
		makePath();
		if (!fillFiles())
			throw string("Can't read the directory: " + path);
		indexFile = getIndexByNameInCurrent(tName);
		if (indexFile >= side::countFiles)  // не вміщається в першу (значить хз :) десь посерединці поставимо :))
		{
			indexRow = 10;
			upIndexFile = indexFile - 10;
		}
		else // ваміщається в перший комлект
		{
			upIndexFile = 0;
			indexRow = indexFile;
		}
		show();
		writePath();
		return true;
	}
	else if ((_stricmp(files[indexFile].name, "..")) && !(files[indexFile].attrib & _A_SUBDIR))
		return false;
	else   // not .. but folder
	{
		filesPath.emplace_back(files[indexFile].name);
		makePath();
		if (!fillFiles())
			throw string("Can't read the directory: " + path);
		clearIndex();
		show();
		writePath();
		return true;
	}
	
}

void side::makeDir(string name)
{
	_mkdir((path + name).c_str());
	fillFiles();
	indexFile = getIndexByNameInCurrent(name);
	if (indexFile >= side::countFiles)  // не вміщається в першу (значить хз :) десь посерединці поставимо :))
	{
		indexRow = 10;
		upIndexFile = indexFile - 10;
	}
	else // ваміщається в перший комлект
	{
		upIndexFile = 0;
		indexRow = indexFile;
	}
	show();

}


size_t side::getIndexByNameInCurrent(string name) const
{
	/* _finddata_t finfo;
	size_t counter = 0;
	intptr_t done = _findfirst((path + "*.*").c_str(), &finfo);
	if (done == -1)
		return false;
	if (_stricmp(finfo.name, name.c_str())) return 0; // на нульовому індексі
	

	while (_findnext(done, &finfo) != -1)
	{
		++counter;
		if (_stricmp(finfo.name, name.c_str())) return counter;
	}
	_findclose(done);
	throw string("We didn't find dir: " + name + "in: " + path);
	*/
	for (size_t a = 0; a < files.size(); ++a)
	{
		if (!_stricmp(files[a].name, name.c_str())) // якщо ідентичні 
			return a;
	}
	throw string("We didn't find dir: " + name + "in: " + path);
}

void side::makePath()
{
	path.clear();
	for (auto &a : filesPath)
		path += (a + '\\');
}

bool side::inactive()
{
	if (!isActive)
		return false;
	
	SetConsoleCursorPosition(hand, { posX, posY + indexRow });
	SetConsoleTextAttribute(hand, 7 | 1 << 4);
	string buf;
	makeLine(files[indexFile], buf);
	cout << buf;
	isActive = false;
	return true;
}

bool side::active()
{
	if (isActive)
		return false;

	SetConsoleCursorPosition(hand, { posX, posY + indexRow });
	SetConsoleTextAttribute(hand, 7 | 2 << 4);
	string buf;
	makeLine(files[indexFile], buf);
	cout << buf;
	isActive = true;
	return true;
}

void side::writePath() const
{
	SetConsoleCursorPosition(side::hand, { posX, posY - 1 });
	SetConsoleTextAttribute(side::hand, 7);
	for (size_t a = 39; a; --a) cout << ' '; 
	cout << char(179);
	SetConsoleCursorPosition(side::hand, { posX, posY - 1 });
	// ширина у нас 40
	// |c:\...  - 7
	//		  33 - залишку
	cout << char(179);
	if (path.size() <= 39)
		cout << path;
	else
		cout << path.substr(0, 3) << "..." << path.substr(path.size() - 31 - 1);

}


