#ifndef __FRAME_HPP__
#define __FRAME_HPP__

#include <string>
#include <vector>

#define MAX_PAYLOAD_LENGTH             0x7FFFFFFFFFFFFFFF;

#define PAYLOAD_SIZE_DEFAULT           125;
#define PAYLOAD_SIZE_EXTRA_WORD        126;
#define PAYLOAD_SIZE_EXTRA_QWORD       127;

typedef unsigned int                   FrameMask;

namespace ws
{

struct Frame
{
	const byte FinalBit = 0x80;
	const byte ReservedBit1 = 0x40; // Compress Extension
	const byte ReservedBit2 = 0x20;
	const byte ReservedBit3 = 0x10;
	const byte TypeMask = 0x0F;
	const byte MaskBit = 0x80;
	const byte PayloadLengthMask = 0x7F;

	const int FrameMinSize = 2;
	const int MaskKeySize = 4;

	const int PayloadSizeDefault = 125;
	const int PayloadSizeExtraWord = 126;
	const int PayloadSizeExtraQWord = 127;

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

	unsigned short extended            : 16;

	char *payload;


	bool IsControl();
	bool IsNonControl();
	bool IsReserved();

	bool Extended();

	static ParseResult Parse(char *data, size_t length, Frame &target, const char *end, std::string reason);

	Frame();
	Frame(Type opcode, bool final, bool masked, const char *payload, size_t length);
	void MakeData(std::vector<char> &data);

};

}

#endif