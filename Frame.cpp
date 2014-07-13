#include "Frame.hpp"

namespace ws
{

bool Frame::IsControl()
{
	return  CLOSE == opcode || PING == opcode || PONG == opcode;
}

bool Frame::IsNonControl()
{
	return  CONTINIUATION == opcode || TEXT == opcode || BINARY == opcode;
}

bool Frame::IsReserved()
{
	return !IsControl() && !IsNonControl();
}

bool Frame::Extended()
{
	return length > PAYLOAD_SIZE_DEFAULT;
}

Frame::Frame(Frame::Type opcode, bool final, bool masked, const char *payload, size_t length)
	: opcode(opcode)
	, final(final)
	, rsv1(0)
	, rsv2(0)
	, rsv3(0)
	, masked(masked)
	, payload(const_cast<char *>(payload))
	, length(length)
{
}

}