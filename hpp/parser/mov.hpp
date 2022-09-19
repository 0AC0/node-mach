#pragma once
#include "../dbg/dbg.hpp"
#include "parser.hpp"

class Parser;

class RegMov {
	public:
		static bool movregval(uint8_t* bytes, Parser* p, uint8_t reg1, uint8_t reg2);
		static bool movregmem(uint8_t* bytes, Parser* p, uint8_t reg1, uint8_t reg2);
		static bool movmemreg(uint8_t* bytes, Parser* p, uint8_t reg1, uint8_t reg2);
		static bool movregreg(Parser* p, uint8_t reg1, uint8_t reg2);
};
