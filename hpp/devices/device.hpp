#pragma once
#include <stdint.h>

class Device {
	private:
	public:
		virtual bool is_in_range(uint64_t addr) = 0;
		virtual void handle_mmio_write(uint64_t addr, uint64_t value) = 0;
		virtual uint64_t handle_mmio_read(uint64_t addr) = 0;
		//virtual void run() = 0;
		Device() { };
};
