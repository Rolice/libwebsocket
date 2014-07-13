#ifndef __FRAME_HPP__
#define __FRAME_HPP__

#define MAX_PAYLOAD_LENGTH              0x7FFFFFFFFFFFFFFF;

namespace ws
{

struct Frame
{
	// RFC 6455 OpCodes
	enum Type
	{
		CONTINUE                       = 0x0,
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

	bool fin                           : 1;
	bool rsv1                          : 1;
	bool rsv2                          : 1;
	bool rsv3                          : 1;

	Type opcode                        : 4;

	unsigned bool mask                 : 1;
	unsigned short length              : 7;

	unsigned short extended            : 16;
};

}

#endif