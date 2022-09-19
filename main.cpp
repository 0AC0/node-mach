#include <stdint.h>
#include <fstream>
#include <filesystem>
#include <thread>
#include <chrono>

#include "hpp/dbg/dbg.hpp"
#include "hpp/cpu/cpu.hpp"
#include "hpp/elf.hpp"

//std::this_thread::sleep_for(std::chrono::seconds(1));

bool hart(CPU* c, uint64_t entry) {
	if(c->parse(entry)) {
		dbg("[2;30;1m[[0;31;2m  STOP  [30;1m][m ") << " ----------- ";
		return 1;
	} else
		dbg("[2;30;1m[[0;32;2m  STOP  [30;1m][m ") << " ----------- ";
	return 0;
}


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

	dbg() << "Allocating hart0 resources";
	CPU* c = new CPU(memory);
	dbg::regs = &c->regs;
	dbg() << "Initializing hart0";
	// Check registers.cpp for read only registers
	//              I = standard isa; S = supervisor; U = usermode
	//              misa     10 = 64 bit                            ABCDEFGHIJKLMNOPQRSTUVWXZ
	//c->csrs.set_csr(0x301, 0b1000000000000000000000000000000000000000000000010000000001010000, 1);

	dbg("[2;30;1m[[0;32;2m  START [30;1m][m ") << "Hart 0 stating up... ";
	std::thread hart0(hart, c, entry);

	hart0.join();

	return 0;
}
