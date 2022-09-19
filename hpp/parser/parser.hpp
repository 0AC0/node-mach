#pragma once
#include <stdint.h>
#include "../dbg/dbg.hpp"
#include "../registers/registers.hpp"
#include "test.hpp"
#include "mov.hpp"
#include "math.hpp"

class Parser {
	private:
		uint16_t index = 0;
		bool interpret(uint8_t* bytes);
	public:
		uint64_t mem_size = 0;
		uint8_t* memory = 0;
		Parser(uint8_t* memory, uint64_t mem_size);

		uint16_t consume() { index++; return memory[index]; }
		uint16_t consume(uint16_t offset) { index += offset; return memory[index]; }
		uint64_t advance() { return Registers::ip(Registers::ip() + 1); }
		uint64_t advance(uint64_t offset) { return Registers::ip(Registers::ip() + offset); }

		bool parse();
};
