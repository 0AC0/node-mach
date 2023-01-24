#include "../hpp/devices/plic.hpp"

void PLIC::handle_mmio_write(uint64_t addr, uint64_t value) {
	dbg() << "PLIC write at addr: " << addr << " with value " << value;
	address_space[(addr - BASE) / sizeof(uint32_t)] = value;

	/*
	switch (addr - BASE) {
		case 0x000 ... 0xFFC:
			//priority
			break;
		case 0x1000 ... 0x107C:
			//pending
			break;
		case 0x2000 ... 0x1F1FFC:
			if (addr & 4) {
			} else {
				//enable
			}
			break;
		case 0x200000 ... 0x3FFF000:
			if (addr & 4) {
			} else {
			}
			break;
	}
	*/
}

uint64_t PLIC::handle_mmio_read(uint64_t addr) {
	dbg() << "PLIC read at addr: " << addr << "/!\\ UNIMPLEMENTED";
	return 0;
}

bool PLIC::is_in_range(uint64_t addr) {
	// only BASE + 0x3FFFFFC is reserved
	if (addr >= BASE && addr < BASE + SIZE) {
		return 1;
	} else {
		return 0;
	}
}
