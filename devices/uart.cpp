#include "../hpp/devices/uart.hpp"

void UART::handle_mmio_write(uint64_t addr, uint64_t value) {
	//dbg() << "UART write at addr: " << addr << " with value " << value;
	address_space[(addr - BASE) / sizeof(uint32_t)] = value;

	switch (addr - BASE) {
		case 0x000 ... 0x1000:
			if (value)
				std::cerr << (char)value;
			else
				std::cerr << "\\0";
			break;
	}
}

uint64_t UART::handle_mmio_read(uint64_t addr) {
	//dbg() << "UART read at addr: " << addr << "/!\\ UNIMPLEMENTED";
	switch (addr - BASE) {
		case 0x5:
			return (1<<5);
	}
	return 0;
}

bool UART::is_in_range(uint64_t addr) {
	// only BASE + 0x3FFFFFC is reserved
	if (addr >= BASE && addr < BASE + SIZE) {
		return 1;
	} else {
		return 0;
	}
}
