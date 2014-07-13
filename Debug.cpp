#include "Debug.hpp"

namespace ws
{

void Debug::Write(const char *text)
{
	#ifdef __DEBUG__
	std::cout << text;
	#endif
}

void Debug::WriteLn(const char *text)
{
	#ifdef __DEBUG__
	std::cout << text << std::endl;
	#endif
}

void Debug::Tag(const char *tag, const char *text)
{
	#ifdef __DEBUG__
	printf("[%s] %s", tag, text);
	#endif
}

void Debug::Tag(const char *tag, const char *text, int code)
{
	#ifdef __DEBUG__
	printf("[%s] %d: %s", tag, code, text);
	#endif
}

void Debug::ColorTag(const char *tag, const char *text, const char *color)
{
	#ifdef __DEBUG__
	printf("[%s%s%s] %s\n", color, tag, CONSOLE_COLOR_RESET, text);
	#endif
}

void Debug::ColorTag(const char *tag, const char *text, const char *color, int code)
{
	#ifdef __DEBUG__
	printf("[%s%s%s] %d: %s\n", color, tag, CONSOLE_COLOR_RESET, code, text);
	#endif
}

void Debug::Info(const char *text)
{
	Debug::ColorTag("INFO", text, CONSOLE_COLOR_BOLDCYAN);
}

void Debug::Warn(const char *text)
{
	Debug::ColorTag("WARN", text, CONSOLE_COLOR_BOLDYELLOW);
}

void Debug::Error(const char *text)
{
	Debug::ColorTag("ERROR", text, CONSOLE_COLOR_BOLDRED);
}

void Debug::Error(const char *text, int code)
{
	Debug::ColorTag("ERROR", text, CONSOLE_COLOR_BOLDRED, code);
}

}