#include "fileMan.h"


fileMan::fileMan(string l, string r, size_t x, size_t y) 
: activeLeft(true), posX(x), posY(y), 
left(new side(l, x, y)), right(new side(r, x+40, y )), active(nullptr)
{
	active = left;
	ramka();
	left->show();
	right->show();
	right->writePath();
	left->writePath();
	right->inactive();
}

void fileMan::keyLisener()
{
	int key[2] = {0};
	do
	{
		key[0] = _getch();
		clearStatus();
		if (_kbhit())
			key[1] = _getch();
		switch (key[0])
		{
		case ENTER:
			active->enter();
			break;
		case TAB:
			active->inactive();
			active = activeLeft ? right : left;
			activeLeft = activeLeft ? false : true;
			active->active();
			break;
		case SKIP:
			switch (key[1])
			{
			case UP:
				active->mooveUp();
				break;
			case DOWN:
				active->mooveDown();
				break;
			}
			break;
		case FSCIP:
			switch (key[1])
			{
			case F5:
				copy();
				break;
			case F7:
				makeDir();
				break;
			}
			break;
		}

	} while (key[0]!=ESC);
}

bool fileMan::copyFile(string pathSrc,	string pathDst, string fileName)
{
	if (!pathSrc.compare(pathDst))
	{
		sMessage("We can't cpy in the same folder.");
		return false;
	}

	
	SetConsoleCursorPosition(side::hand, { posX, posY + side::countFiles +1 });
	for (size_t a = 80; a; --a) cout << ' ';
	SetConsoleCursorPosition(side::hand, { posX, posY + side::countFiles +1 });
	cout << "Copying: " << fileName;

	if (findExistedItem(pathDst, fileName)) // то такий файл вже є
	{
		SetConsoleCursorPosition(side::hand, { posX, posY + side::countFiles +1});
		SetConsoleTextAttribute(side::hand, 7);
		cout << "File " << fileName << "already exists! rwrite? (enter/esc)";
		int key = 0;
		do
		{
			key = _getch();
		} while (key != ENTER && key != ESC);
		if (key == ESC)
			return false;
	}

#ifdef DEBUG_FILEMAN_COPY_FILE
	string s1(pathSrc + fileName);
	string s2(pathDst + fileName);
#endif

	bool res = CopyFileA((pathSrc+fileName).c_str(), (pathDst+fileName).c_str(), 0);

	return res;
}

bool fileMan::findExistedItem(string path, string name)
{
	_finddata_t finfo;
	
	_set_errno(0);
	intptr_t done = _findfirst((path + name).c_str(), &finfo);
	if (done == -1 && (errno & ENOENT))   // файл не знайдено
	{
		_findclose(done);
		return false;
	}
	
	//файл знайдено
	_findclose(done);
	return true;
}

bool fileMan::copyDir(string pathSrc, string pathDst, string dirName)
{
	if (!pathSrc.compare(pathDst))
	{
		sMessage("We can't cpy in the same folder.");
		return false;
	}
	else if (pathDst.find_first_of(pathSrc) != string::npos)
		return false;

	if (findExistedItem(pathDst, dirName)) // то така папка вже є
	{

		sMessage(string("Dir ") + dirName + "already exists! rwrite? (enter/esc)");
		//SetConsoleCursorPosition(side::hand, { posX, posY + side::countFiles });
		//SetConsoleTextAttribute(side::hand, 7);
		//cout << "Dir " << dirName << "already exists! rwrite? (enter/esc)";
		int key = 0;
		do
		{
			key = _getch();
		} while (key != ENTER && key != ESC);
		if (key == ESC)
			return false;
	}



#ifdef DEBUG_FILEMAN_COPY_DIR
	string newdir(pathDst + dirName);
#endif
	_mkdir((pathDst + dirName).c_str());
	
	_finddata_t finfo;
	intptr_t done = _findfirst((pathSrc + dirName + '\\' + "*.*").c_str(), &finfo);
	if (done == -1)
	{
		_findclose(done);
		return false;
	}
#ifdef DEBUG_FILEMAN_COPY_DIR
	bool isDir = finfo.attrib & _A_SUBDIR;
	string src(pathSrc + dirName + '\\');
	string dst(pathDst + dirName + '\\');
	string name(finfo.name);
#endif

	if (_stricmp(finfo.name, ".") && _stricmp(finfo.name, ".."))
	{
		if (finfo.attrib & _A_SUBDIR) // якщо папка - рекурсія
			copyDir(pathSrc + dirName + '\\', pathDst + dirName + '\\', finfo.name);
		else  // копіюємо файл
			copyFile(pathSrc + dirName + '\\', pathDst + dirName + '\\', finfo.name);
	}

	while (_findnext(done, &finfo) != -1) // те ж саме тільки в циклі
	{
#ifdef DEBUG_FILEMAN_COPY_DIR
		bool isDir = finfo.attrib & _A_SUBDIR;
		string src(pathSrc + dirName + '\\');
		string dst(pathDst + dirName + '\\');
		string name(finfo.name);
#endif

		if (_stricmp(finfo.name, ".") && _stricmp(finfo.name, ".."))
		{
			if (finfo.attrib & _A_SUBDIR) // якщо папка - рекурсія
				copyDir(pathSrc + dirName + '\\', pathDst + dirName + '\\', finfo.name);
			else  // копіюємо файл
				copyFile(pathSrc + dirName + '\\', pathDst + dirName + '\\', finfo.name);
		}
	}

	_findclose(done);
	return true;
}

bool fileMan::copy()
{
	SetConsoleCursorPosition(side::hand, { posX, posY + side::countFiles +1});
	SetConsoleTextAttribute(side::hand, 7);
	cout << "Do you want to copy: " << active->getActiveFInfo().name << "(enter/esc)";
	int key = 0;
	do
	{
		key = _getch();
	} while (key != ENTER && key != ESC);
	if (key == ESC)
		return false;
	
	bool res = 0;

#ifdef DEBUG_FILEMAN_COPY
	string src(active->getPath());	
	string dst(getInactive()->getPath());
	string name(active->getActiveFInfo().name);
	bool isDir = active->getActiveFInfo().attrib & _A_SUBDIR;
#endif

	if (active->getActiveFInfo().attrib & _A_SUBDIR) // якщо папка - рекурсія
		res = copyDir(active->getPath(), getInactive()->getPath(), active->getActiveFInfo().name);
	else  // копіюємо файл
		res = copyFile(active->getPath(), getInactive()->getPath(), active->getActiveFInfo().name);


	if (!res)
	{
		SetConsoleCursorPosition(side::hand, { posX, posY + side::countFiles +1});
		for (size_t a = 80; a; --a) cout << ' ';
		SetConsoleCursorPosition(side::hand, { posX, posY + side::countFiles +1});
		cout << "Copy fail!";
		return false;
	}

	getInactive()->fillFiles();
	getInactive()->clearIndex();
	getInactive()->show();

	SetConsoleCursorPosition(side::hand, { posX, posY + side::countFiles +1});
	for (size_t a = 80; a; --a) cout << ' ';
	SetConsoleCursorPosition(side::hand, { posX, posY + side::countFiles +1});
	cout << "Copy complete.";
	return true;
}

void fileMan::makeDir()
{
	sMessage("Enter the name of new dir: ");
	string name;
	std::cin >> name;
	active->makeDir(name);
}

fileMan::~fileMan()
{
	delete left;
	delete right;
}

void fileMan::ramka() const
{
	// head
	SetConsoleCursorPosition(side::hand, { 36, 0 });
	cout << "fileman :)";
	
	// top
	SetConsoleCursorPosition(side::hand, { posX, posY - 2 });
	printf_s("%c", 218);
	for (size_t a = 1; a <= 38; ++a) printf_s("%c", 196);
	printf_s("%c%c", 194,194);
	for (size_t a = 1; a <= 38; ++a) printf_s("%c", 196);
	printf_s("%c", 191);

	// bottom
	SetConsoleCursorPosition(side::hand, { posX, posY + side::countFiles});
	printf_s("%c", 192);
	for (size_t a = 1; a <= 38; ++a) printf_s("%c", 196);
	printf_s("%c%c", 193, 193);
	for (size_t a = 1; a <= 38; ++a) printf_s("%c", 196);
	printf_s("%c", 217);

}
