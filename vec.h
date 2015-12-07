#pragma once
#include <vector>
#include <allocators>
#include "myException.h"

#define GetStringVal(var) #var


template <typename type, typename alloc = std::allocator<type>> 
class vec : public std::vector<type, alloc>
{
public:
	vec() : vector() {} // construct empty vector
	explicit vec(const alloc &al) : vector(al) {}
	explicit vec(size_t count) : vector(count) {}
	vec(size_t count, const type &val) : vector(count, val) {}
	vec(size_t count, const type &val, const alloc &al) : vector(count, val, al) {}

	vec(const vec<type, alloc> &src) : vector(src) {}
	vec(const vec<type, alloc> &src, const alloc &al) : vector(src, al) {}
	vec(const vector<type, alloc> &src) : vector(src) {}
	vec(const vector<type, alloc> &src, const alloc &al) : vector(src, al) {}

	template<typename iter>
	vec(iter first, iter last) : vector(first, last) {}
	template<typename iter>
	vec(iter first, iter last, alloc &al) : vector(first, last, al) {}

	vec(vec<type, alloc> &&src) : vector(stc) {}
	vec(vec<type, alloc> &&src, const alloc &al) : vector(src, al) {}
	vec(vector<type, alloc> &&src) : vector(stc) {}
	vec(vector<type, alloc> &&src, const alloc &al) : vector(src, al) {}
	
	vec(std::initializer_list<type> ilList, const alloc &al = alloc()) : vector(ilList, al) {}


	

	const_reference operator[](size_type pos) const
	{
		if (pos >= size())
			throw myException<3, string, size_t>{{ string("out of range in ") + typeid(vec).name() + ", const_reference operator[]", 0 }, { "Max index", size() - 1 }, { "Cur index", pos } };
		const vector<type, alloc> &_base = *this;
		return _base[pos];
	}

	reference operator[](size_type pos)
	{
		if (pos >= size())
			throw myException<3, string, size_t>{{ string("out of range in ") + typeid(vec).name() + ", reference operator[]", 0 }, { "Max index", size() - 1 }, { "Cur index", pos } };
		vector<type, alloc> &_base = *this;
		return _base[pos];
	}

	const_reference at(size_type pos) const
	{
		if (pos >= size())
			throw myException<3, string, size_t>{{ string("out of range in ") + typeid(vec).name() + ", const_reference at()", 0 }, { "Max index", size() - 1 }, { "Cur index", pos } };
		return vector::at(pos);
	}

	reference at(size_type pos)
	{
		if (pos >= size())
			throw myException<3, string, size_t>{{ string("out of range in ") + typeid(vec).name() + ", reference at()", 0 }, { "Max index", size() - 1 }, { "Cur index", pos } };
		return vector::at(pos);
	}


	


};

