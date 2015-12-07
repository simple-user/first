#include <iostream>
#include <Windows.h>
#include <string>
#include <conio.h>
#include "comparer.h"

using std::cout;
using std::endl;
using std::string;


#define br cout << endl

#define _EXEPT_START try {
#define _EXEPT_END } \
	catch (std::exception &e) \
{ \
	std::cout << "Error: " << e.what(); \
} \
	catch (string &s) \
{\
	std::cout << s; \
}\
catch (...) { \
	std::cout << "Что-то пошло не так... Chrome.(c) "; \
}



int main(int argc, char*argv[]) 
{
	_EXEPT_START
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	if (argc == 1)
		return 0;
	else if (argc < 3)
		return 0;
	else if (argc > 3)
		return 0;
	
	comparer comp(argv[1], argv[2]);
	if (!comp.load())
	{
		cout << comp.getError() << endl;
		return 0;
	}

	if (!comp.compareToCMD())
	{
		cout << comp.getError() << endl;
		return 0;
	}


	_EXEPT_END
	return 0;
}