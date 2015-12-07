#pragma once
#include <utility>
#include <array>
#include <initializer_list>
#include <exception>

using std::array;
using std::initializer_list;
using std::pair;

template <size_t count, typename t1, typename t2>
class myException : public std::exception
{
public:
	myException() : resChar(nullptr) {};
	myException(initializer_list<pair<t1, t2>> inList) : resChar(nullptr)
	{
		for (size_t a = 0; a < inList.size(); ++a)
		{
			arr[a].first = (inList.begin() + a)->first;
			arr[a].second = (inList.begin() + a)->second;
		}
	}
	
	virtual const char* what() const override 
	{
		string res;
		for (size_t a = 0; a < arr.size(); ++a)
		{
			res += string(arr[a].first) + " - " + std::to_string(arr[a].second) + "\n";
		}
		resChar = new char[res.size() + 1];
		memcpy_s(resChar, res.size() + 1, res.c_str(), res.size());
		resChar[res.size()] = '\0';

		return resChar;
	}

	bool setPair(size_t index, const pair<t1, t2> &p)
	{
		if (index >= arr.size())
			return false;
		arr[index].first = p.first;
		arr[index].second = p.second;
		return true;
	}

	bool setPair(size_t index, const t1 &val1, const t2 &val2)
	{
		if (index >= arr.size())
			return false;
		arr[index].first = val1;
		arr[index].second = val2;
		return true;

	}

	const pair<t1, t2> * getPtrPair(size_t index) const
	{
		if (index >= arr.size())
			return nullptr;
		
		return &arr[index];

	}

	const size_t getSize() const
	{
		return arr.size();
	}

	~myException()
	{
		delete[] resChar;
	}
private:
	array<pair<t1, t2>, count> arr;
	mutable char * resChar;


};

