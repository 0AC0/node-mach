#pragma once
#include <stdint.h>
#include "dbg/dbg.hpp"
#include "memory/memory.hpp"

class ELF {
	private:
		uint8_t     e_ident[16];         /* Magic number and other info */
		[[maybe_unused]] uint16_t    e_type;              /* Object file type */
		[[maybe_unused]] uint16_t    e_machine;           /* Architecture */
		[[maybe_unused]] uint32_t    e_version;           /* Object file version */
		uint64_t    e_entry;             /* Entry point virtual address */
		uint64_t    e_phoff;             /* Program header table file offset */
		[[maybe_unused]] uint64_t    e_shoff;             /* Section header table file offset */
		[[maybe_unused]] uint32_t    e_flags;             /* Processor-specific flags */
		[[maybe_unused]] uint16_t    e_ehsize;            /* ELF header size in bytes */
		[[maybe_unused]] uint16_t    e_phentsize;         /* Program header table entry size */
		uint16_t    e_phnum;             /* Program header table entry count */
		[[maybe_unused]] uint16_t    e_shentsize;         /* Section header table entry size */
		[[maybe_unused]] uint16_t    e_shnum;             /* Section header table entry count */
		[[maybe_unused]] uint16_t    e_shstrndx;          /* Section header string table index */
		uint8_t     data[];
	public:
		uint64_t load(Memory* mem);
		bool check_header();
};

struct ELFPHdr {
               uint32_t   p_type;
               uint32_t   p_flags;
               uint64_t   p_offset;
               uint64_t   p_vaddr;
               uint64_t   p_paddr;
               uint64_t   p_filesz;
               uint64_t   p_memsz;
               uint64_t   p_align;
};

struct ELFSHdr {
               uint32_t   sh_name;
               uint32_t   sh_type;
               uint64_t   sh_flags;
               uint64_t   sh_addr;
               uint64_t   sh_offset;
               uint64_t   sh_size;
               uint32_t   sh_link;
               uint32_t   sh_info;
               uint64_t   sh_addralign;
               uint64_t   sh_entsize;
};

struct ELFSym {
               uint32_t      st_name;
               unsigned char st_info;
               unsigned char st_other;
               uint16_t      st_shndx;
               uint64_t      st_value;
               uint64_t      st_size;
};
