#include "../hpp/cpu/registers.hpp"

uint64_t* CSRs::get_csr_ptr(uint16_t addr) {
	return &csrs[addr];
}

uint64_t CSRs::get_csr(uint16_t addr) {
	CSRAddr* address = (CSRAddr*)&addr;
	switch (address->mid) {
		case 0b00: // User
			break;
		case 0b01: // Supervisor
			break;
		case 0b10: // Hypervisor
			break;
		case 0b11: // Machine
			break;
	}
	switch (addr) {
		case 0xF11:
			// Machine Vendor ID = NodeMach
			return 0x4e6f64654d616368;
		case 0xF14:
			// Hart ID       //TODO: for multihart return a different id
			return hartid;
	}
	return csrs[addr];
}

void CSRs::set_csr(uint16_t addr, uint64_t value, bool force) {
	CSRAddr* address = (CSRAddr*)&addr; //TODO: this is broken

	if (address->high == 0b11) {
		// this doesn't work
		//dbg("[2;30;1m[[0;34;2m  WARN  [30;1m][m ") << "Tried writing " << value << " to read only CSR " << addr;
	}
	// TODO: check for privilage level first
	switch (address->mid) {
		case 0b00: // User
			break;
		case 0b01: // Supervisor
			break;
		case 0b10: // Hypervisor
			break;
		case 0b11: // Machine
			break;
	}
	switch (addr) {
		case 0x100:
			if constexpr (DBG_CSR)
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to sstatus: " << value;
			csrs[addr] = value;
			break;
		case 0x104:
			if constexpr (DBG_CSR)
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to supervisor interrupt enable: " << value;
			csrs[addr] = value;
			break;
		case 0x180:
			if constexpr (DBG_CSR)
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to satp: " << value;
			csrs[addr] = value;
			break;
		case 0x300:
			if constexpr (DBG_CSR)
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to mstatus: " << value << '/' << csrs[addr];
			csrs[addr] = value;
			break;
		case 0x302:
			if constexpr (DBG_CSR)
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to medeleg: " << value;
			csrs[addr] = value;
			break;
		case 0x303:
			if constexpr (DBG_CSR)
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to mideleg: " << value;
			csrs[addr] = value;
			break;
		case 0x304:
			if constexpr (DBG_CSR)
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to machine interrupt enable: " << value;
			csrs[addr] = value;
			break;
		case 0x305:
			if constexpr (DBG_CSR)
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to machine trap handler base address: " << value;
			csrs[addr] = value;
			break;
		case 0x340:
			if constexpr (DBG_CSR)
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to mscratch: " << value;
			csrs[addr] = value;
			break;
		case 0x341:
			if constexpr (DBG_CSR)
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to mepc: " << value;
			csrs[addr] = value;
			break;
		case 0x3A0:
		case 0x3A2:
			if constexpr (DBG_CSR)
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to pmpcfg" << (addr & 0xf) << ": " << value;
			csrs[addr] = value;
			break;
		case 0x3B0 ... 0x3BF:
			if constexpr (DBG_CSR)
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to pmpaddr" << (addr & 0xf) << ": " << value;
			csrs[addr] = value;
			break;
		case 0xB00:
		case 0xC00:
			if constexpr (DBG_CSR)
			//dbg() << "Set cycles to " << value;
			csrs[addr] = value;
			break;
		case 0x301:
		case 0xF14:
			if (force) {
				csrs[addr] = value;
				if constexpr (DBG_CSR)
				dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
					<< "Force writing to CSR: " << addr;
			} else
				if constexpr (DBG_CSR)
				dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
					<< "Can't write to this CSR: " << addr;

			break;
		default:
			if constexpr (DBG_CSR)
			dbg("[2;30;1m[[0;34;2m  WARN  [30;1m][m ")
				<< "Setting unknown CSR at " << addr << " to " << value;
			csrs[addr] = value;
			break;
	}
}
