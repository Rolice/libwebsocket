#ifndef __UTIL_HPP__
#define __UTIL_HPP__

#include "include/global.hpp"

#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

namespace ws
{

class Util
{
public:
	template<typename T>
	static std::string ToString(const T &value)
	{
		std::ostringstream stream;
		stream << value;
		return stream.str();
	}

	template <typename T>
	static std::vector<T> ToVector(const T &value)
	{
		std::string buffer = Util::ToString(value);
		std::vector<T> result(buffer.begin(), buffer.end());

		return result;
	}

	static byte *Random(unsigned int length);
	static void *RandomRaw(unsigned int length);
	static void Random(byte *target, unsigned int length);
	static void RandomRaw(void *target, unsigned int length);
};

}

#endif