#include "../hpp/devices/virtio.hpp"

void VIRTIO::handle_mmio_write(uint64_t addr, uint64_t value) {
	//dbg() << "UART write at addr: " << addr << " with value " << value;
	address_space[(addr - BASE) / sizeof(uint32_t)] = value;

	switch (addr - BASE) {
	}
}

uint64_t VIRTIO::handle_mmio_read(uint64_t addr) {
	//dbg() << "UART read at addr: " << addr << "/!\\ UNIMPLEMENTED";
	switch (addr - BASE) {
		case VIRTIO_MMIO_MAGIC_VALUE: return 0x74726976;
		case VIRTIO_MMIO_VERSION: return 2;
	    case VIRTIO_MMIO_DEVICE_ID: return 2;
	    case VIRTIO_MMIO_VENDOR_ID: return 0x554d4551;
	    	//							features_ok
	    case VIRTIO_MMIO_STATUS: return 8;
	    case VIRTIO_MMIO_QUEUE_NUM_MAX: return 8;
	}
	return 0;
}

bool VIRTIO::is_in_range(uint64_t addr) {
	// only BASE + 0x3FFFFFC is reserved
	if (addr >= BASE && addr < BASE + SIZE) {
		return 1;
	} else {
		return 0;
	}
}
