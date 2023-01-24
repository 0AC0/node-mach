#include <stdint.h>
#include <fstream>
#include <filesystem>
#include <thread>
#include <signal.h>

#include "hpp/dbg/dbg.hpp"
#include "hpp/dbg/debugger.hpp"
#include "hpp/cpu/cpu.hpp"
#include "hpp/devices/uart.hpp"
#include "hpp/elf.hpp"
#include "hpp/threads.hpp"

#include "hpp/devices/plic.hpp"
#include "hpp/devices/uart.hpp"
#include "hpp/devices/virtio.hpp"


int main(int, char* argv[]) {
	std::filesystem::path payloadname = argv[1];

	std::ifstream payload(payloadname, std::ios::in | std::ios::binary);
	Memory* memory;
	uint32_t mem_size;

	dbg() << "Getting payload stats";
	mem_size = std::filesystem::file_size(payloadname);
	dbg() << "Allocating memory of size: " << Memory::MEM_SIZE;
	dbg() << "Exec size: " << mem_size;
	if (mem_size < Memory::MEM_SIZE)
		memory = new Memory;
	else
		return 2;

	ELF* elf = (ELF*)new uint8_t[mem_size];
	payload.read((char*)elf, mem_size);
	payload.close();

	if(elf->check_header()) return 3;
	uint64_t entry = elf->load(memory);
	free(elf);

	dbg() << "Initializing debugger";
	debugger();

	dbg() << "Starting Device threads";
	PLIC* plic = new PLIC(0xC000000);
	Threads::start_device(plic);

	UART* uart = new UART(0x10000000);
	Threads::start_device(uart);

	VIRTIO* virtio = new VIRTIO(0x10001000);
	Threads::start_device(virtio);

	dbg() << "Starting CPU threads";
	Threads::start_all_cpus(memory, entry);

	Threads::join_all_cpus();

	return 0;
}
