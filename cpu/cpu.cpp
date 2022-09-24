#include "../hpp/cpu/cpu.hpp"

std::atomic<bool> CPU::running = 0;
std::atomic<bool> CPU::stopping = 0;

bool CPU::run(CPU* c, uint64_t entry) {
	running = 1;
	while (!stopping) {
		if (running) {
			if (c->parse(entry)) return 1;
		} else {
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
	return 0;
}

void CPU::pause() {
	running = 0;
}

void CPU::resume() {
	running = 1;
}

void CPU::stop() {
	stopping = 1;
}

CPU::CPU(Memory* memory) {
	this->memory = memory;
}

bool CPU::parse(uint64_t entry) {
	index = entry;
	regs.pc = index;
	dbg() << "Starting hart at entry point: " << regs.pc;
	while (index < Memory::MEM_SIZE + Memory::MEM_START) {
		csrs.set_csr(0xC00, csrs.get_csr(0xC00) + 1);
		csrs.set_csr(0xB00, csrs.get_csr(0xB00) + 1);
		if (interpret((memory->read_instruction(index)))) {
			dbg("[2;30;1m[[0;31;2m  DEAD  [30;1m][m ") << "pc: " << regs.pc;

			for (uint8_t j = 0; j < 32; j++) {
				dbg("[2;30;1m[[0;31;2m  DEAD  [30;1m][m ") << regs.name[j] << ": " << regs.x[j];
			}

			#if 0
			dbg* d = new dbg("[2;30;1m[[0;31;2m  DEAD  [30;1m][m ");
			uint32_t buffer[5] = {0};
			*d << "Memory dump: " << '\n';
				for (uint64_t i = 1; i < Memory::MEM_SIZE + 1; i++) {
					if (buffer[0] == 0
					&& buffer[1] == 0
					&& buffer[2] == 0
					&& buffer[3] == 0
					&& buffer[4] == 0
					&& memory->read32(i - 1) == 0)
						;
					else {
						std::cout << std::uppercase << std::setfill('0') << std::setw(8) << memory->read32((i - 1) * 4);
						std::cout << '[' << std::uppercase << std::setfill('0') << std::setw(4) << (i - 1) * 4 << ']';
						*d << ' ';

						if (i % 4 == 0) {
							buffer[i % 4] = memory->read32(i - 1);
							*d << '\n';
						}
					}
				}
			d->~dbg();
			#endif
		       	return 1;
		}
	}
	return 0;
}
