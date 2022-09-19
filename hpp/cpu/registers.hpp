#pragma once
#include <stdint.h>
#include "../dbg/dbg.hpp"

class CSRs {
	private:
		uint64_t csrs[4096] = {0};
		struct CSRAddr {
			uint8_t low :2;
			uint8_t mid :2;
			uint8_t high:2;
		};
	public:
		struct misa {
			uint64_t ext:26;
			uint64_t ignore:36;
			uint64_t base:2;
		};
		struct mstatus {
			uint8_t uie:1;
			uint8_t sie:1;
			uint8_t hie:1;
			uint8_t mie:1;
			uint8_t upie:1;
			uint8_t spie:1;
			uint8_t hpie:1;
			uint8_t mpie:1;
			uint8_t spp:1;
			uint8_t hpp:2;
			uint8_t mpp:2;
			uint8_t fs:2;
			uint8_t xs:2;
			uint8_t mpriv:1;
			uint8_t pum:1;
			uint8_t mxr:1;
			uint8_t ignore:4;
			uint8_t vm:5;
			uint64_t ignore1:34;
			uint8_t sd:1;
		};
		void set_csr(uint16_t addr, uint64_t value, bool force = 0);
		uint64_t get_csr(uint16_t addr);
		uint64_t* get_csr_ptr(uint16_t addr);
};

class Regs {
	private:

	public:
		Regs() {}
		//static constexpr uint8_t XLEN = 64;
		const char* name[32] = { "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2"
					, "s0/fp", "s1", "a0", "a1", "a2", "a3", "a4", "a5"
					, "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7"
					, "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6" };
		uint64_t x[32] = {0};
		uint64_t pc;
};
