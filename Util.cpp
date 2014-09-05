#include "Util.hpp"

namespace ws
{

template<typename T>
std::string Util::ToString(const T &value)
{
	std::ostringstream stream;
	stream << value;
	return stream.str();
}

}