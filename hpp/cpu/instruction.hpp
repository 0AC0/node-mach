#pragma once
#include <stdint.h>

class Rtype {
	public:
		uint32_t three:2;
		uint32_t op:5;
		uint32_t rd:5;
		uint32_t funct3:3;
		uint32_t rs1:5;
		uint32_t rs2:5;
		uint32_t funct7:7;
};

class AMOtype {
	public:
		uint32_t three:2;
		uint32_t op:5;
		uint32_t rd:5;
		uint32_t funct3:3;
		uint32_t rs1:5;
		uint32_t rs2:5;
		uint32_t rl:1;
		uint32_t aq:1;
		uint32_t funct5:5;
};

class SHIFTtype {
	public:
		uint32_t three:2;
		uint32_t op:5;
		uint32_t rd:5;
		uint32_t funct3:3;
		uint32_t rs1:5;
		uint32_t shamt:6;
		uint32_t type:6;
};

class Itype {
	public:
		uint32_t three:2;
		uint32_t op:5;
		uint32_t rd:5;
		uint32_t funct3:3;
		uint32_t rs1:5;
		uint32_t imm:12;
};

class Stype {
	public:
		uint32_t three:2;
		uint32_t op:5;
		uint32_t imm1:5;
		uint32_t funct3:3;
		uint32_t rs1:5;
		uint32_t rs2:5;
		uint32_t imm2:7;
};

class Utype {
	public:
		uint32_t three:2;
		uint32_t op:5;
		uint32_t rd:5;
		uint32_t imm:20;
};

class Btype {
	public:
		uint32_t three:2;
		uint32_t op:5;
		uint32_t imm1:1;
		uint32_t imm2:4;
		uint32_t funct3:3;
		uint32_t rs1:5;
		uint32_t rs2:5;
		uint32_t imm3:6;
		uint32_t imm4:1;
};

class Jtype {
	public:
		uint32_t three:2;
		uint32_t op:5;
		uint32_t rd:5;
		uint32_t imm1:8;
		uint32_t imm2:1;
		uint32_t imm3:10;
		uint32_t imm4:1;
};

class Instruction {
	public:
		uint32_t three:2;
		uint32_t op:5;
		uint32_t data:25;
};

