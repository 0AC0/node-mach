#include "../hpp/memory/memory.hpp"

void Memory::write8(uint64_t addr, uint8_t value) {
	switch (addr) {
		case 0 ... MEM_SIZE:
			mem[addr] = value;
			return;
		// UART HACK
		case 0x10000000:
			if (value)
				std::cerr << (char)value;
			else
				std::cerr << "\\0";
			return;
		default:
			return;
	}
}
void Memory::write16(uint64_t addr, uint16_t value) {
	switch (addr) {
		case 0 ... MEM_SIZE:
			*(uint16_t*)&(mem[addr]) = value;
		default:
			return;
	}
}
void Memory::write32(uint64_t addr, uint32_t value) {
	switch (addr) {
		case 0 ... MEM_SIZE:
			*(uint32_t*)&(mem[addr]) = value;
		default:
			return;
	}
}
void Memory::write64(uint64_t addr, uint64_t value) {
	switch (addr) {
		case 0 ... MEM_SIZE:
			*(uint64_t*)&(mem[addr]) = value;
		default:
			return;
	}
}

uint8_t Memory::read8(uint64_t addr) {
	switch (addr) {
		case 0 ... MEM_SIZE:
			return mem[addr];
		// UART HACK
		case 0x10000000 + 5:
			return (1<<5);
		default:
			return 0;
	}
}

uint16_t Memory::read16(uint64_t addr) {
	switch (addr) {
		case 0 ... MEM_SIZE:
			return *(uint16_t*)&(mem[addr]);
		default:
			return 0;
	}
}

uint32_t* Memory::read_instruction(uint64_t addr) {
	return &((uint32_t*)&mem)[addr];
}

uint32_t Memory::read32(uint64_t addr) {
	switch (addr) {
		case 0 ... MEM_SIZE:
			return *(uint32_t*)&(mem[addr]);
		default:
			return 0;
	}
}

uint64_t Memory::read64(uint64_t addr) {
	switch (addr) {
		case 0 ... MEM_SIZE:
			return *(uint64_t*)&(mem[addr]);
		default:
			return 0;
	}
}
