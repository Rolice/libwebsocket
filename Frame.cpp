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

	target.final = first & Frame::FinalBit;
	target.rsv1 = first & Frame::ReservedBit1;
	target.rsv2 = first & Frame::ReservedBit2;
	target.rsv3 = first & Frame::ReservedBit3;
	target.opcode = static_cast<Frame::Type>(first & Frame::TypeMask);
	target.masked = second & Frame::MaskBit;

	return OK;
}

}