#include "../hpp/cpu/cpu.hpp"

void CPU::exception(Exception ex) {
	switch (ex) {
		case Exception::Illegal_Instruction:
			dbg("[2;30;1m[[0;31;2m  DEAD  [30;1m][m ") << "Tried to execute invalid instruction "
				<< ((Instruction*)(memory->read_instruction(index)))->op << " at "
				<< regs.pc;
			break;
		case Exception::Breakpoint:
			break;
		case Exception::ECall_Machine_Mode:
			dbg("[2;30;1m[[0;31;2m  DEAD  [30;1m][m ") << "ECall "
				<< ((Instruction*)(memory->read_instruction(index)))->op << " at "
				<< regs.pc;
			while (1) {}
			break;
	}
}

void CPU::exception_return() {
	switch (mode) {
		// return from machine mode
		case Mode::Machine: {
					    CSRs::mstatus* stat = (CSRs::mstatus*)csrs.get_csr_ptr(0x300);

					    stat->mie = stat->mpie;
					    mode = stat->mpp; // set to previous mode
					    dbg() << "Switched to mode: " << mode;
					    stat->mpie = 1;
					    stat->mpp = Mode::User;

					    regs.pc = csrs.get_csr(0x341); //mepc
					    index = (regs.pc - Memory::MEM_START) / 4 + Memory::MEM_START;
					    return;
				    }
		default:
			while (1) {}
	}
}

void CPU::wait_for_interrupt() {
	while (1) {}
}
