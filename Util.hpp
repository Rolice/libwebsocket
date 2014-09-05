#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include <string>
#include <sstream>

namespace ws
{

class Util
{
public:
	template<typename T>
	static std::string ToString(const T &value);
};

}

#endif