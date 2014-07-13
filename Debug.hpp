#ifndef __DEBUG_HPP__
#define __DEBUG_HPP__

#include <stdio.h>
#include <iostream>
#include <string.h>

#include "include/global.hpp"
#include "exinclude/console.hpp"

class Debug {

public:
	static void Write(const char *text);
	static void WriteLn(const char *text);

	static void Tag(const char *tag, const char *text);
	static void Tag(const char *tag, const char *text, int code);
	static void ColorTag(const char *tag, const char *text, const char *color);
	static void ColorTag(const char *tag, const char *text, const char *color, int code);

	static void Info(const char *text);
	static void Warn(const char *text);
	static void Error(const char *text);
	static void Error(const char *text, int code);
};

#endif