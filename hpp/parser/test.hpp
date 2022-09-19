#pragma once
#include "../dbg/dbg.hpp"
#include "parser.hpp"
#include <stdio.h>

class Parser;

class RegTest {
	public:
		static bool greater(uint8_t* bytes, Parser* p);
		static bool lesser(uint8_t* bytes, Parser* p);
		static bool eq(uint8_t* bytes, Parser* p);
		static bool greatereq(uint8_t* bytes, Parser* p);
		static bool lessereq(uint8_t* bytes, Parser* p);
		static bool noteq(uint8_t* bytes, Parser* p);
};
