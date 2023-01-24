#pragma once
#include <stdint.h>
#include <atomic>
#include <thread>
#include <chrono>
#include <bit>
#include "../dbg/dbg.hpp"
#include "registers.hpp"
#include "instruction.hpp"
#include "../memory/memory.hpp"

class Memory;

class CPU {
	private:
		enum Exception {
			Illegal_Instruction	 = 2,
			Breakpoint		 = 3,
			ECall_User_Mode		 = 8,
			ECall_Supervisor_Mode	 = 9,
			ECall_Machine_Mode	 = 11,
			Instruction_Page_Fault	 = 12,
			Load_Page_Fault		 = 13,
			Store_Page_Fault	 = 15,
		};
		enum Mode {
			User			 = 0,
			Supervisor		 = 1,
			//Hypervisor		 = 2, // Unimplemented
			Machine			 = 3,
		};

		struct VirtAddr {
			uint64_t page_off:12;
			uint64_t vpn0:9;
			uint64_t vpn1:9;
			uint64_t vpn2:9;
		};

		struct PhysAddr {
			uint64_t page_off:12;
			uint64_t ppn0:9;
			uint64_t ppn1:9;
			uint64_t ppn2:26;
		};

		struct PTE {
			uint64_t v:1;
			uint64_t r:1;
			uint64_t w:1;
			uint64_t x:1;
			uint64_t u:1;
			uint64_t g:1;
			uint64_t a:1;
			uint64_t d:1;
			uint64_t rsw:2;
			uint64_t ppn0:9;
			uint64_t ppn1:9;
			uint64_t ppn2:26;
			uint64_t reserved:10;
		};

		uint8_t mode = Mode::Machine;
		uint64_t index = 0;
		std::atomic<bool> running = 1; //pretty sure atomic is unnecessary currently

		bool interpret(uint32_t* bytes);
		void exception(Exception ex);
		void exception_return(Mode m);
		void wait_for_interrupt();
		uint64_t translate_addr(uint64_t addr);
	public:
		Regs regs;
		CSRs csrs;
		uint8_t get_mode() { return this->mode; }
		Memory* memory = 0;
		CPU(Memory* memory, uint8_t hartid);

		uint32_t consume();
		uint32_t consume(uint64_t offset);

		bool step();
		bool parse(uint64_t entry);

		static bool run(CPU* c, uint64_t entry);
		void pause();
		void resume();
};

