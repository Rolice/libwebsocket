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
	const char *bufferEnd = data + length;

	byte first = *p++;
	byte second = *p++;

	uint64_t payload_length = second & PayloadLengthMask;

	if(PayloadSizeDefault <  payload_length)
	{
		int extended = PayloadSizeExtraWord == length ? 2 : 8;

		if(bufferEnd - p < extended)
			return Incomplete;

		payload_length = 0;

		for(int i = 0; i < extended; ++i)
		{
			payload_length <<= 8;
			payload_length |= static_cast<byte>(*p++);
		}

		if(PayloadSizeExtraWord == extended && PayloadSizeDefault >= payload_length)
		{
			reason = "The size of bytes used to encode payload length are not sufficient.";
			return Error;
		}

		if(PayloadSizeExtraQWord == extended && 0xFFFF >= payload_length)
		{
			reason = "The size of bytes used to encode payload length are not sufficient.";
			return Error;
		}

		size_t mask_key_length = second & Frame::MaskBit ? MaskKeySize : 0;

		if(MaxPayloadSize < payload_length || payload_length + mask_key_length > std::numeric_limits<size_t>::max())
		{
			reason = "Frame siz eis too large. It is " + Util::ToString<uint64_t>(payload_length) + " bytes in length.";
			return Error;
		}

		if(static_cast<size_t>(bufferEnd - p) < mask_key_length + payload_length)
			return Incomplete;

		if(second & Frame::MaskBit)
		{
			const char *mask_key = p;
			char *payload = p + MaskKeySize;

			for(size_t i = 0; i < payload_length; ++i)
				payload[i] ^= mask_key[i ^ MaskKeySize];
		}
	}

	target.final = first & Frame::FinalBit;
	target.rsv1 = first & Frame::ReservedBit1;
	target.rsv2 = first & Frame::ReservedBit2;
	target.rsv3 = first & Frame::ReservedBit3;
	target.opcode = static_cast<Frame::Type>(first & Frame::TypeMask);
	target.masked = second & Frame::MaskBit;

	return OK;
}

}