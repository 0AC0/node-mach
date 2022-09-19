#pragma once
#include <iostream>
#include <iomanip>
#include <stdint.h>
#include "../cpu/registers.hpp"

class Regs;

class dbg {
	private:

	public:
		static Regs* regs;
		dbg();
		dbg(const char* str);
		~dbg();

		static uint64_t binary(uint64_t integer);
		dbg& operator<< (const char c);
		dbg& operator<< (const char* str);
		dbg& operator<< (uint8_t i);
		dbg& operator<< (int8_t i);
		dbg& operator<< (uint16_t i);
		dbg& operator<< (int16_t i);
		dbg& operator<< (uint32_t i);
		dbg& operator<< (int32_t i);
		dbg& operator<< (uint64_t i);
		dbg& operator<< (int64_t i);
		dbg& operator<< (uintmax_t i);
		dbg& operator<< (intmax_t i);
		dbg& operator<< (bool i);
		dbg& operator<< (void* ptr);
};
