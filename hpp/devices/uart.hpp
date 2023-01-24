#pragma once
#include <stdint.h>
#include "device.hpp"
#include "../dbg/dbg.hpp"

class UART : public Device {
	private:
		static constexpr uint64_t SIZE = 0x1000;
		uint64_t BASE;

		uint32_t address_space[SIZE / sizeof(uint32_t)] = {0};

	public:
		void handle_mmio_write(uint64_t addr, uint64_t value);
		uint64_t handle_mmio_read(uint64_t addr);
		bool is_in_range(uint64_t addr);
		//static void run() { dbg() << "PLIC starting... "; };
		UART(const uint64_t base) { this->BASE = base; }
};
