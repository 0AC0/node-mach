#pragma once
#include <stdint.h>
#include "parser.hpp"

class Parser;

class RegMath {
	public:
		static bool addregreg(uint8_t* bytes, Parser* p, uint8_t reg1, uint8_t reg2, uint8_t reg3);
		static bool subtractregreg(uint8_t* bytes, Parser* p, uint8_t reg1, uint8_t reg2, uint8_t reg3);
		static bool multiplyregreg(uint8_t* bytes, Parser* p, uint8_t reg1, uint8_t reg2, uint8_t reg3);
		static bool divideregreg(uint8_t* bytes, Parser* p, uint8_t reg1, uint8_t reg2, uint8_t reg3);
		static bool uaddregreg(uint8_t* bytes, Parser* p, uint8_t reg1, uint8_t reg2, uint8_t reg3);
		static bool usubtractregreg(uint8_t* bytes, Parser* p, uint8_t reg1, uint8_t reg2, uint8_t reg3);
		static bool umultiplyregreg(uint8_t* bytes, Parser* p, uint8_t reg1, uint8_t reg2, uint8_t reg3);
		static bool udivideregreg(uint8_t* bytes, Parser* p, uint8_t reg1, uint8_t reg2, uint8_t reg3);
		static bool lshiftregreg(uint8_t* bytes, Parser* p, uint8_t reg1, uint8_t reg2, uint8_t reg3);
		static bool rshiftregreg(uint8_t* bytes, Parser* p, uint8_t reg1, uint8_t reg2, uint8_t reg3);
		static bool orregreg(uint8_t* bytes, Parser* p, uint8_t reg1, uint8_t reg2, uint8_t reg3);
		static bool xorregreg(uint8_t* bytes, Parser* p, uint8_t reg1, uint8_t reg2, uint8_t reg3);
		static bool andregreg(uint8_t* bytes, Parser* p, uint8_t reg1, uint8_t reg2, uint8_t reg3);
};
