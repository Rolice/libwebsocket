#ifndef __FRAME_HPP__
#define __FRAME_HPP__

#include "include/global.hpp"

#include "Util.hpp"

#include <string>
#include <vector>
#include <limits>
#include <cstring>

#include <inttypes.h>

#include <assert.h>

namespace ws
{

struct Frame
{
	static const byte FinalBit = 0x80;
	static const byte ReservedBit1 = 0x40; // Compress Extension
	static const byte ReservedBit2 = 0x20;
	static const byte ReservedBit3 = 0x10;
	static const byte TypeMask = 0x0F;
	static const byte MaskBit = 0x80;
	static const byte PayloadLengthMask = 0x7F;

	static const int FrameMinSize = 2;
	static const int MaskKeySize = 4;

	static const int PayloadSizeDefault = 125;
	static const int PayloadSizeExtraWord = 126;
	static const int PayloadSizeExtraQWord = 127;

	static const uint64_t MaxPayloadSize = 0x7FFFFFFFFFFFFFFF;

	// RFC 6455 OpCodes
	enum Type
	{
		CONTINIUATION                  = 0x0,
		TEXT                           = 0x1,
		BINARY                         = 0x2,

		// Reserved for further non-control Frames (RFC 6455)
		NCF1                           = 0x3,
		NCF2                           = 0x4,
		NCF3                           = 0x5,
		NCF4                           = 0x6,
		NCF5                           = 0x7,

		CLOSE                          = 0x8,
		PING                           = 0x9,
		PONG                           = 0xA,

		// Reserved for further control frames (RFC 6455)
		CF1                            = 0xB,
		CF2                            = 0xC,
		CF3                            = 0xD,

		INCOMPLETE                     = 0xE,

		// Reserved for further control frames (RFC 6455)
		CF4                            = 0xF,
	};

	enum ParseResult
	{
		OK,
		Incomplete,
		Error
	};

	bool final                         : 1;
	bool rsv1                          : 1;
	bool rsv2                          : 1;
	bool rsv3                          : 1;

	Type opcode                        : 4;

	bool masked                        : 1;
	unsigned short length              : 7;

	int extended;

	char *payload;


	bool IsControl();
	bool IsNonControl();
	bool IsReserved();

	bool Extended();

	static ParseResult Parse(char *data, size_t length, Frame &target, const char *end, std::string reason);

	Frame();
	Frame(Type opcode, bool final, bool masked, const char *payload, size_t length);

	void Data(std::vector<byte> &data);
	
	static void Append(const Frame &target, std::vector<byte> &data);
};

}

#endif