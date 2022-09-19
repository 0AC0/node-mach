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
			return 0;
	}
	return csrs[addr];
}

void CSRs::set_csr(uint16_t addr, uint64_t value, bool force) {
	CSRAddr* address = (CSRAddr*)&addr; //TODO: this is broken

	if (address->high == 0b11) {
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
		case 0x104:
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to supervisor interrupt enable: " << value;
			csrs[addr] = value;
			break;
		case 0x180:
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to page table base address: " << value;
			csrs[addr] = value;
			break;
		case 0x300:
			/*
			(*(mstatus*)&csrs[addr]).uie	= (*(mstatus*)&value).uie;
			(*(mstatus*)&csrs[addr]).sie	= (*(mstatus*)&value).sie;
			(*(mstatus*)&csrs[addr]).hie	= (*(mstatus*)&value).hie;
			(*(mstatus*)&csrs[addr]).mie	= (*(mstatus*)&value).mie;
			(*(mstatus*)&csrs[addr]).upie	= (*(mstatus*)&value).upie;
			(*(mstatus*)&csrs[addr]).spie	= (*(mstatus*)&value).spie;
			(*(mstatus*)&csrs[addr]).hpie	= (*(mstatus*)&value).hpie;
			(*(mstatus*)&csrs[addr]).mpie	= (*(mstatus*)&value).mpie;
			(*(mstatus*)&csrs[addr]).spp	= (*(mstatus*)&value).spp;
			(*(mstatus*)&csrs[addr]).hpp	= (*(mstatus*)&value).hpp;
			(*(mstatus*)&csrs[addr]).mpp	= (*(mstatus*)&value).mpp;
			(*(mstatus*)&csrs[addr]).fs	= (*(mstatus*)&value).fs;
			(*(mstatus*)&csrs[addr]).xs	= (*(mstatus*)&value).xs;
			(*(mstatus*)&csrs[addr]).mpriv	= (*(mstatus*)&value).mpriv;
			(*(mstatus*)&csrs[addr]).pum	= (*(mstatus*)&value).pum;
			(*(mstatus*)&csrs[addr]).mxr	= (*(mstatus*)&value).mxr;
			(*(mstatus*)&csrs[addr]).ignore	= 0;
			(*(mstatus*)&csrs[addr]).vm	= (*(mstatus*)&value).vm;
			(*(mstatus*)&csrs[addr]).ignore1= 0;
			(*(mstatus*)&csrs[addr]).sd	= (*(mstatus*)&value).sd;
			*/
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to mstatus: " << value << '/' << csrs[addr];
			csrs[addr] = value;
			break;
		case 0x302:
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to medeleg: " << value;
			csrs[addr] = value;
			break;
		case 0x303:
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to mideleg: " << value;
			csrs[addr] = value;
			break;
		case 0x304:
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to machine interrupt enable: " << value;
			csrs[addr] = value;
			break;
		case 0x305:
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to machine trap handler base address: " << value;
			csrs[addr] = value;
			break;
		case 0x340:
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to mscratch: " << value;
			csrs[addr] = value;
			break;
		case 0x341:
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to mepc: " << value;
			csrs[addr] = value;
			break;
		case 0x3A0:
		case 0x3A2:
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to pmpcfg" << (addr & 0xf) << ": " << value;
			csrs[addr] = value;
			break;
		case 0x3B0 ... 0x3BF:
			dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
				<< "Wrote to pmpaddr" << (addr & 0xf) << ": " << value;
			csrs[addr] = value;
			break;
		case 0xB00:
		case 0xC00:
			//dbg() << "Set cycles to " << value;
			csrs[addr] = value;
			break;
		case 0x301:
		case 0xF14:
			if (force) {
				csrs[addr] = value;
				dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
					<< "Force writing to CSR: " << addr;
			} else
				dbg("[2;30;1m[[0;35;2m  CSR   [30;1m][m ")
					<< "Can't write to this CSR: " << addr;

			break;
		default:
			dbg("[2;30;1m[[0;34;2m  WARN  [30;1m][m ")
				<< "Setting unknown CSR at " << addr << " to " << value;
			csrs[addr] = value;
			break;
	}
}
