#include "hpp/elf.hpp"

bool ELF::check_header() {
	if (e_ident[0] != 0x7F
	&& e_ident[1] != 0x45
	&& e_ident[2] != 0x4c
	&& e_ident[3] != 0x46) {
		dbg("[2;30;1m[[0;31;2m  DEAD  [30;1m][m ") << "Bad ELF header magic";
		return 1;
	} else {
		dbg() << "ELF magic correct";
	}
	dbg() << "ELF type: " << e_type;
	if (e_machine != 0xF3) {
		dbg("[2;30;1m[[0;31;2m  DEAD  [30;1m][m ") << "Bad ELF machine type";
		return 1;
	}
	dbg() << "ELF entry: " << e_entry;
	
	/*
	*/
	dbg() << "ELF e_phnum: " << e_phnum;
	dbg() << "ELF e_phoff: " << e_phoff;
	for (uint64_t i = 0; i < e_phnum; i++) {
		if (((ELFPHdr*)(e_phoff + (uintptr_t)data))[i].p_type != 0) {
			dbg() << "type " << ((ELFPHdr*)(e_phoff + (uintptr_t)data))[i].p_type  ;
			dbg() << "flags " << ((ELFPHdr*)(e_phoff + (uintptr_t)data))[i].p_flags ;
			dbg() << "off " << ((ELFPHdr*)(e_phoff + (uintptr_t)data))[i].p_offset;
			dbg() << "vaddr " << ((ELFPHdr*)(e_phoff + (uintptr_t)data))[i].p_vaddr ;
			dbg() << "paddr " << ((ELFPHdr*)(e_phoff + (uintptr_t)data))[i].p_paddr ;
			dbg() << "fsize " << ((ELFPHdr*)(e_phoff + (uintptr_t)data))[i].p_filesz;
			dbg() << "msize " << ((ELFPHdr*)(e_phoff + (uintptr_t)data))[i].p_memsz ;
			dbg() << "align " << ((ELFPHdr*)(e_phoff + (uintptr_t)data))[i].p_align ;
		}
	}

	/*
	dbg() << "ELF e_shnum: " << e_shnum;
	dbg() << "ELF e_shoff: " << e_shoff;
	char* strtab = 0;
	for (uint64_t i = 0; i < e_shnum; i++) {
		dbg d;
		ELFSHdr entry = ((ELFSHdr*)(e_shoff + (uintptr_t)data))[i];
		if (entry.sh_type == 0x3)
			strtab = (char*)(entry.sh_offset + (uintptr_t)data);
		if (strtab) {
			dbg() << strtab + entry.sh_name;
		}
		d << " name:"<<entry.sh_name;
		d << " type:"<<entry.sh_type;
		d << " flags:"<<entry.sh_flags;
		d << " addr:"<<entry.sh_addr;
		d << " off:"<<entry.sh_offset;
		d << " size:"<<entry.sh_size;
		d << " link:"<<entry.sh_link;
		d << " info:"<<entry.sh_info;
		d << " align:"<<entry.sh_addralign;
		d << " entsize:"<<entry.sh_entsize;
	}
	*/
	return 0;
}

uint64_t ELF::load(Memory* mem) {
	dbg() << "ELF entry: " << e_entry;
	
	/*
	*/
	dbg() << "ELF e_phnum: " << e_phnum;
	dbg() << "ELF e_phoff: " << e_phoff;
	ELFPHdr* phdr = ((ELFPHdr*)(e_phoff + (uintptr_t)(this))); // 
	for (uint64_t i = 0; i < e_phnum; i++) {
		if (phdr[i].p_type == 1) {
			for (uint64_t j = 0; j < phdr[i].p_filesz; j++) {
					mem->write8(j + Memory::MEM_START, *(uint8_t*)(((uint8_t*)this) + j + phdr[i].p_offset));
			}
			dbg() << "type " << phdr[i].p_type  ;
			dbg() << "flags " << phdr[i].p_flags ;
			dbg() << "off " << phdr[i].p_offset;
			dbg() << "vaddr " << phdr[i].p_vaddr ;
			dbg() << "paddr " << phdr[i].p_paddr ;
			dbg() << "fsize " << phdr[i].p_filesz;
			dbg() << "msize " << phdr[i].p_memsz ;
			dbg() << "align " << phdr[i].p_align ;
		}
	}
	return e_entry;
}
