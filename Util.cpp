#include "Util.hpp"

namespace ws
{

// template<typename T>
// std::string Util::ToString(const T &value)
// {
// 	std::ostringstream stream;
// 	stream << value;
// 	return stream.str();
// }

byte *Util::Random(unsigned int length)
{
	byte *result = (byte *) malloc(length);

	if(!result)
		return NULL;

	for(unsigned int i = 0; i < length; i++)
		result[i] = rand();

	return result;
}

void *Util::RandomRaw(unsigned int length)
{
	byte *result = (byte *) malloc(length);

	if(!result)
		return NULL;

	for(unsigned int i = 0; i < length; i++)
		result[i] = rand();

	return (void *) result;
}

void Util::Random(byte *target, unsigned int length)
{
	if(!target)
		return;

	for(unsigned int i = 0; i < length; i++)
		target[i] = rand();
}

void Util::RandomRaw(void *target, unsigned int length)
{
	if(!target)
		return;

	byte *buffer = (byte *) target;

	for(unsigned int i = 0; i < length; i++)
		buffer[i] = rand();
}

}