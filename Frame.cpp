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
	return PayloadSizeDefault < length;
}

Frame::Frame()
{
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

Frame::ParseResult Frame::Parse(char *data, size_t length, Frame &target, const char *end, std::string reason)
{
	if(FrameMinSize >= length)
		return Incomplete;

	char *p = data;

	byte first = *p++;
	byte second = *p++;

	frame.final = first & FirstBit;
	frame.rsv1 = first & ReservedBit1;
	frame.rsv2 = first & ReservedBit2;
	frame.rsv3 = first & ReservedBit3;
	frame.opcode = static_cast<Frame::Type>(first & TypeMask);
	frame.masked = second & MaskBit;

	return OK;
}

}