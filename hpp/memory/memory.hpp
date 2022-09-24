#pragma once
#include <stdint.h>
#include "../dbg/dbg.hpp"

class Memory {
	public:
		static constexpr uint32_t MB = 1 * 0x100000;
		static constexpr uint32_t MEM_SIZE = 132 * MB;
		static constexpr uint32_t MEM_START = 0x80000000;
	private:
		template <typename T> T readT(uint64_t addr);
		template <typename T> void writeT(uint64_t addr, T value);
		uint8_t mem[MEM_SIZE];
	public:
		void write8(uint64_t addr, uint8_t value);
		void write16(uint64_t addr, uint16_t value);
		void write32(uint64_t addr, uint32_t value);
		void write64(uint64_t addr, uint64_t value);

		uint8_t read8(uint64_t addr);
		uint16_t read16(uint64_t addr);
		uint32_t read32(uint64_t addr);
		uint32_t* read_instruction(uint64_t addr);
		uint64_t read64(uint64_t addr);

};
