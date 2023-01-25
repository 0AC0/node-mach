#include "../hpp/cpu/cpu.hpp"


uint32_t CPU::consume() {
	index++;
	regs.pc += sizeof(uint32_t);
	return memory->read32(index);
}

uint32_t CPU::consume(uint64_t offset) {
	index += offset;
	regs.pc += offset;
	return memory->read32(index);
}

bool CPU::step() {
	return interpret((memory->read_instruction(translate_addr(index))));
}

// TODO: page fault
// TODO: separate read and write functions so we can page fault accordingly
// TODO: check for misaligned superpages
// TODO: set access and dirty bits
uint64_t CPU::translate_addr(uint64_t addr) {
	CSRs::satp* satp = (CSRs::satp*)csrs.get_csr_ptr(0x180);

	VirtAddr va = __builtin_bit_cast(VirtAddr, addr);
	if (satp->mode == 0) return addr;
	if (satp->mode == 8) {
		uint64_t dir = satp->ppn * Memory::PAGE_SIZE;
		PTE pte = __builtin_bit_cast(PTE, memory->read64(dir + va.vpn2 * sizeof(PTE)));
		if (!pte.v || (!pte.r && pte.w)) {
			dbg() << "page fault";
			while (1) { }
		} else if (pte.r || pte.x) {
			dbg() << "superpage lv2 leaf found";
			while (1) { }
		} else {
			// TODO: ignore reserved bits which may be 1s
			dir = (__builtin_bit_cast(uint64_t, pte) >> 10) * Memory::PAGE_SIZE;
			pte = __builtin_bit_cast(PTE, memory->read64(dir + va.vpn1 * sizeof(PTE)));
			if (!pte.v || (!pte.r && pte.w)) {
				dbg() << "page lv1 fault";
				while (1) { }
			} else if (pte.r || pte.x) {
				dbg() << "superpage lv1 leaf found";
				while (1) { }
			} else {
				dir = (__builtin_bit_cast(uint64_t, pte) >> 10) * Memory::PAGE_SIZE;
				pte = __builtin_bit_cast(PTE, memory->read64(dir + va.vpn0 * sizeof(PTE)));
				if (!pte.v || (!pte.r && pte.w)) {
					dbg() << "page lv0 fault";
					while (1) { }
				} else if (pte.r || pte.x) {
					PhysAddr pa;
					pa.page_off = va.page_off;
					pa.ppn0 = pte.ppn0;
					pa.ppn1 = pte.ppn1;
					pa.ppn2 = pte.ppn2;
					return __builtin_bit_cast(uint64_t, pa) & 0x0fffffffffff;
				}
			}
		}
		dbg() << __builtin_bit_cast(uint64_t, pte);
		return addr;
	}
	dbg() << "unsupported paging type";
	while (1) {}
}

bool CPU::run(CPU* c, uint64_t entry) {
	if (c->parse(entry)) return 1;
	return 0;
}

void CPU::pause() {
	running = 0;
}

void CPU::resume() {
	running = 1;
	cv.notify_all();
}

CPU::CPU(Memory* memory, uint8_t hartid) {
	this->memory = memory;
	this->csrs.hartid = hartid;
}

bool CPU::parse(uint64_t entry) {
	index = entry;
	regs.pc = index;
	dbg() << "Starting hart " << csrs.hartid << " at entry point: " << regs.pc;
	while (index < Memory::MEM_SIZE + Memory::MEM_START) {
		csrs.set_csr(0xC00, csrs.get_csr(0xC00) + 1);
		csrs.set_csr(0xB00, csrs.get_csr(0xB00) + 1);

		std::unique_lock<std::mutex> lock(mutex);
		cv.wait(lock, [this] { return running; });

		if (step()) {
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
