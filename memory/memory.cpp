#include "../hpp/memory/memory.hpp"

template <typename T>
void Memory::writeT(uint64_t addr, T value) {
	switch (addr) {
		case MEM_START ... MEM_START + MEM_SIZE - 1:
			*(T*)&(mem[addr - MEM_START]) = value;
			return;
		default:
			for (auto d : Threads::instance().devices) {
				if (d.device->is_in_range(addr)) {
					d.device->handle_mmio_write(addr, value);
					return;
				}
			}
			dbg() << "MMU: Ignoring write to: " << addr << " out of range";
	}
}

void Memory::write8(uint64_t addr, uint8_t value) {
	writeT<uint8_t>(addr, value);
}

void Memory::write16(uint64_t addr, uint16_t value) {
	writeT<uint16_t>(addr, value);
}

void Memory::write32(uint64_t addr, uint32_t value) {
	writeT<uint32_t>(addr, value);
}

void Memory::write64(uint64_t addr, uint64_t value) {
	writeT<uint64_t>(addr, value);
}


template <typename T>
T Memory::readT(uint64_t addr) {
	switch (addr) {
		case MEM_START ... MEM_START + MEM_SIZE - 1:
			return *(T*)&(mem[addr - MEM_START]);
		default:
			for (auto d : Threads::instance().devices) {
				if (d.device->is_in_range(addr)) {
					return d.device->handle_mmio_read(addr);
				}
			}
			dbg() << "MMU: Ignoring read from: " << addr << " out of range";
			return 0;
	}
}

uint8_t Memory::read8(uint64_t addr) {
	return readT<uint8_t>(addr);
}

uint16_t Memory::read16(uint64_t addr) {
	return readT<uint16_t>(addr);
}

uint32_t* Memory::read_instruction(uint64_t addr) {
	return &((uint32_t*)&mem)[addr - MEM_START];
}

uint32_t Memory::read32(uint64_t addr) {
	return readT<uint32_t>(addr);
}

uint64_t Memory::read64(uint64_t addr) {
	return readT<uint64_t>(addr);
}
