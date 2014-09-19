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

	if(PayloadSizeDefault < payload_length)
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
			reason = "The bytes used to encode payload length are not sufficient.";
			return Error;
		}

		if(PayloadSizeExtraQWord == extended && 0xFFFF >= payload_length)
		{
			reason = "The bytes used to encode payload length are not sufficient.";
			return Error;
		}

		target.extended = extended;
	}

	size_t mask_key_length = second & Frame::MaskBit ? MaskKeySize : 0;

	if(MaxPayloadSize < payload_length || payload_length + mask_key_length > std::numeric_limits<size_t>::max())
	{
		reason = "Frame size is too large. It is " + Util::ToString<uint64_t>(payload_length) + " bytes in length.";
		return Error;
	}

	if(static_cast<size_t>(bufferEnd - p) < mask_key_length + payload_length)
		return Incomplete;

	//if(!target.payload)
	target.payload = (char *) malloc(payload_length + 1);
	memset(target.payload, 0, payload_length);

	if(second & Frame::MaskBit)
	{
		const char *mask_key = p;
		char *payload = p + MaskKeySize;

		for(size_t i = 0; i < payload_length; ++i) {
			payload[i] ^= mask_key[i % MaskKeySize];
			target.payload[i] = payload[i];
		}
	}

	target.payload[payload_length] = '\0';

	target.final = first & Frame::FinalBit;
	target.rsv1 = first & Frame::ReservedBit1;
	target.rsv2 = first & Frame::ReservedBit2;
	target.rsv3 = first & Frame::ReservedBit3;
	target.opcode = static_cast<Frame::Type>(first & Frame::TypeMask);
	target.masked = second & Frame::MaskBit;
	target.length = payload_length;

	end = p + mask_key_length + payload_length;

	return OK;
}

void Frame::Data(std::vector<byte> &data)
{
	assert(!(opcode & ~TypeMask));

	data.resize(2);
	data.at(0) = (final ? FinalBit : 0) | (rsv1 ? ReservedBit1 : 0) | opcode;
	data.at(1) = masked ? MaskBit : 0;

	if(PayloadSizeDefault >= length)
		data.at(1) |= length;
	else if(0xFFFF >= length)
	{
		data.at(1) |= PayloadSizeExtraWord;

		data.push_back((length & 0xFFFF) >> 8);
		data.push_back((length & 0xFF));
	}
	else
	{
		data.at(1) |= PayloadSizeExtraQWord;
		byte ext[8];
		size_t remaining = length;

		for(size_t i = 0; i < sizeof(ext); i++)
		{
			ext[7- i] = remaining & 0xFF;
			remaining >>= 8;
		}

		assert(!remaining);

		data.push_back(*ext);
	}

	Append(*this, data);
}

void Frame::Append(const Frame &target, std::vector<byte> &data)
{
	size_t mask_key_start = 0;

	if(target.masked)
	{
		mask_key_start = data.size();
		data.resize(data.size() + MaskKeySize);
	}

	size_t payload_start = data.size();

	for(size_t i = 0; i < target.length; i++)
		data.push_back(*(target.payload + i));

	if(target.masked)
	{
		Util::Random(data.data() + mask_key_start, MaskKeySize);

		for(size_t i = 0; i < target.length; i++)
			data[payload_start + i] ^= data[mask_key_start + i % MaskKeySize];
	}
}

}
