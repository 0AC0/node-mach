#include "../hpp/cpu/cpu.hpp"

void CPU::interrupt(uint64_t irq) {
	if (csrs.get_csr(0x303) & (1 << irq)) {
		if constexpr (DBG_IRQ) dbg() << "IRQ S mode: " << irq;
		//				scause  interrupt bit | interrupt type
		csrs.set_csr(0x142, ((uint64_t)1 << 63) | irq); // TODO: WARNING THIS SHOULD BE THE TYPE NOT THE NUMBER
		// sstatus
		CSRs::mstatus sstat = __builtin_bit_cast(CSRs::mstatus, csrs.get_csr(0x100));
		// previous interrupt enable
		sstat.spie = 1;
		// set previous mode
		sstat.spp = mode;
		// disable interrupts
		sstat.sie = 0;
		csrs.set_csr(0x100, __builtin_bit_cast(uint64_t, sstat));
		// sip
		//csrs.set_csr(0x144, 1 << irq);
		mode = Mode::Supervisor;
		// sepc
		csrs.set_csr(0x141, regs.pc);
		if (csrs.get_csr(0x105) & 3) {
			//vectored mode
			regs.pc = (csrs.get_csr(0x105) & ~0x3) + irq * 4;
		} else {
			//direct mode
			regs.pc = csrs.get_csr(0x105) & ~0x3;
		}
	} else {
		if constexpr (DBG_IRQ) dbg() << "IRQ M mode: " << irq;
		//				mcause  interrupt bit | interrupt type
		csrs.set_csr(0x342, ((uint64_t)1 << 63) | irq); // TODO: WARNING THIS SHOULD BE THE TYPE NOT THE NUMBER
		//																	mstatus
		CSRs::mstatus mstat = __builtin_bit_cast(CSRs::mstatus, csrs.get_csr(0x300));
		// previous interrupt enable
		mstat.mpie = 1;
		// set previous mode
		mstat.mpp = mode;
		// disable interrupts
		mstat.mie = 0;
		csrs.set_csr(0x300, __builtin_bit_cast(uint64_t, mstat));
		// mip
		//csrs.set_csr(0x344, 1 << irq);
		mode = Mode::Machine;
		// mepc
		csrs.set_csr(0x341, regs.pc);
		if (csrs.get_csr(0x305) & 3) {
			//vectored mode
			regs.pc = (csrs.get_csr(0x305) & ~0x3) + irq * 4;
		} else {
			//direct mode
			regs.pc = csrs.get_csr(0x305) & ~0x3;
		}
	}
}


void CPU::exception(Exception ex, uint64_t info) {
	switch (ex) {
		case Exception::Illegal_Instruction:
			dbg("[2;30;1m[[0;31;2m  DEAD  [30;1m][m ") << "Tried to execute invalid instruction "
			<< regs.pc;
			break;
		case Exception::Breakpoint:
			break;
		case ECall_User_Mode:
		case ECall_Supervisor_Mode:
		case Exception::ECall_Machine_Mode:
			dbg("[2;30;1m[[0;31;2m  DEAD  [30;1m][m ") << "ECall ";
			break;

			// TODO: 
		case Instruction_Page_Fault:
		case Load_Page_Fault:

		case Store_Page_Fault:
			if (csrs.get_csr(0x302) & (1 << Exception::Store_Page_Fault)) {
				// TODO: check deleg

				if constexpr (DBG_IRQ) dbg() << "Exception: page fault, handler: " << (csrs.get_csr(0x105) & ~0x3);
				// mcause
				csrs.set_csr(0x142, Exception::Load_Page_Fault);
				// mstatus
				CSRs::mstatus sstat = __builtin_bit_cast(CSRs::mstatus, csrs.get_csr(0x100));
				// previous interrupt enable
				sstat.spie = sstat.sie;
				// set previous mode
				sstat.spp = mode;
				// disable interrupts
				sstat.sie = 0;

				csrs.set_csr(0x100, __builtin_bit_cast(uint64_t, sstat));
				// mepc
				csrs.set_csr(0x141, regs.pc);
				// mtval
				csrs.set_csr(0x143, info);
				// mip
				//csrs.set_csr(0x344, 1 << Exception::Load_Page_Fault);
				mode = Mode::Supervisor;
				//direct mode only
				regs.pc = csrs.get_csr(0x105);
			} else {
				if constexpr (DBG_IRQ) dbg() << "Exception: page fault, handler: " << (csrs.get_csr(0x305) & ~0xf);
				// mcause
				csrs.set_csr(0x342, Exception::Load_Page_Fault);
				// mstatus
				CSRs::mstatus mstat = __builtin_bit_cast(CSRs::mstatus, csrs.get_csr(0x300));
				// previous interrupt enable
				mstat.mpie = mstat.mie;
				// set previous mode
				mstat.mpp = mode;
				// disable interrupts
				mstat.mie = 0;

				csrs.set_csr(0x300, __builtin_bit_cast(uint64_t, mstat));
				// mepc
				csrs.set_csr(0x341, regs.pc);
				// mtval
				csrs.set_csr(0x343, info);
				// mip
				//csrs.set_csr(0x344, 1 << Exception::Load_Page_Fault);
				mode = Mode::Machine;
				//direct mode only
				regs.pc = csrs.get_csr(0x305);
			}
			break;
	}
}

void CPU::exception_return(Mode m) {
	switch (m) {
		case Mode::User: {
			dbg() << "user ret";
			while (1) {}
		}
		case Mode::Supervisor: {
					    CSRs::mstatus* stat = (CSRs::mstatus*)csrs.get_csr_ptr(0x100);

					    stat->sie = stat->spie;
					    mode = stat->spp; // set to previous mode
					    dbg() << "Switched to mode: " << (uint64_t)mode;
					    //stat->spie = 1;
					    stat->spp = Mode::User;

					    regs.pc = csrs.get_csr(0x141); //sepc
					    return;
				    }
		case Mode::Machine: {
					    CSRs::mstatus* stat = (CSRs::mstatus*)csrs.get_csr_ptr(0x300);

					    stat->mie = stat->mpie;
					    mode = stat->mpp; // set to previous mode
					    dbg() << "Switched to mode: " << (uint64_t)mode;
					    //stat->mpie = 1;
					    stat->mpp = Mode::User;

					    regs.pc = csrs.get_csr(0x341); //mepc
					    return;
				    }
		default:
			dbg() << "bad return";
			while (1) {}
	}
}

void CPU::wait_for_interrupt() {
	dbg() << "wait_for_interrupt";
	while (1) {}
}
