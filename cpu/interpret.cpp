#include "../hpp/cpu/cpu.hpp"

int64_t sign_extend8(uint8_t i) {
	int64_t j;
	if (i & 0x80)
		j = i | (((uint64_t)-1) << 8);
	else
		j = i;

	//std::cout << "[sign extend8: " << std::dec << j << "]" << std::hex;
	return j;
}

int64_t sign_extend12(uint16_t i) {
	int64_t j;
	if (i & 0x800)
		j = i | (((uint64_t)-1) << 12);
	else
		j = i;

	//std::cout << "[sign extend12: " << std::dec << j << "]" << std::hex;
	return j;
}

int64_t sign_extend16(uint16_t i) {
	int64_t j;
	if (i & 0x8000)
		j = i | (((uint64_t)-1) << 16);
	else
		j = i;

	//std::cout << "[sign extend16: " << std::dec << j << "]" << std::hex;
	return j;
}

int64_t sign_extend20(uint32_t i) {
	int64_t j;
	if (i & 0x80000)
		j = i | (((uint64_t)-1) << 20);
	else
		j = i;

	//std::cout << "[sign extend20: " << std::dec << j << "]" << std::hex;
	return j;
}

int64_t sign_extend32(uint32_t i) {
	int64_t j;
	if (i & 0x80000000)
		j = i | (((uint64_t)-1) << 32);
	else
		j = i;

	//std::cout << "[sign extend32: " << std::dec << j << "]" << std::hex;
	return j;
}

uint64_t arithmetic_right_shift(uint64_t x, uint64_t n) {
	return x >> n | ~(~0ULL >> n);
}

bool CPU::interpret(uint32_t* bytes) {
	if (regs.x[0] != 0) regs.x[0] = 0;

	if (((Instruction*)bytes)->three != 3) {
		dbg("[2;30;1m[[0;31;2m  DEAD  [30;1m][m ") << "NOT AN INSTRUCTION: "
			<< *bytes << " at "
			<< regs.pc;
		
		return 0;
	}
	switch(((Instruction*)bytes)->op) {
		case 0x0:
			switch(((Itype*)bytes)->funct3) {
				case 0b000:
					dbg() << "lb x[" << ((Itype*)bytes)->rd << "](" << regs.x[((Itype*)bytes)->rd] << ')';
					regs.x[((Itype*)bytes)->rd]
						= sign_extend8(memory->read8(regs.x[((Itype*)bytes)->rs1]
									+ sign_extend12(((Itype*)bytes)->imm)));
					break;
				case 0b001:
					dbg() << "lh x[" << ((Itype*)bytes)->rd << "](" << regs.x[((Itype*)bytes)->rd] << ')';
					regs.x[((Itype*)bytes)->rd]
						= sign_extend16(memory->read16(regs.x[((Itype*)bytes)->rs1]
									+ sign_extend12(((Itype*)bytes)->imm)));
					break;
				case 0b010:
					dbg() << "lw x[" << ((Itype*)bytes)->rd << "](" << regs.x[((Itype*)bytes)->rd] << ')';
					regs.x[((Itype*)bytes)->rd]
						= sign_extend32(memory->read32(regs.x[((Itype*)bytes)->rs1]
									+ sign_extend12(((Itype*)bytes)->imm)));
					break;
				case 0b011:
					dbg() << "ld x[" << ((Itype*)bytes)->rd << "](" << regs.x[((Itype*)bytes)->rd] << ')';
					regs.x[((Itype*)bytes)->rd]
						= memory->read64(regs.x[((Itype*)bytes)->rs1]
									+ sign_extend12(((Itype*)bytes)->imm));
					break;
				case 0b100:
					dbg() << "lbu x[" << ((Itype*)bytes)->rd << "](" << regs.x[((Itype*)bytes)->rd] << ')';
					regs.x[((Itype*)bytes)->rd]
						= memory->read8(regs.x[((Itype*)bytes)->rs1]
									+ sign_extend12(((Itype*)bytes)->imm));
					break;
				case 0b101:
					dbg() << "lhu x[" << ((Itype*)bytes)->rd << "](" << regs.x[((Itype*)bytes)->rd] << ')';
					regs.x[((Itype*)bytes)->rd]
						= memory->read16(regs.x[((Itype*)bytes)->rs1]
									+ sign_extend12(((Itype*)bytes)->imm));
					break;
				case 0b110:
					dbg() << "lwu x[" << ((Itype*)bytes)->rd << "](" << regs.x[((Itype*)bytes)->rd] << ')';
					regs.x[((Itype*)bytes)->rd]
						= memory->read32(regs.x[((Itype*)bytes)->rs1]
									+ sign_extend12(((Itype*)bytes)->imm));
					break;
				default:
					exception(Exception::Illegal_Instruction);
					return 1;
			}
			consume();
			break;
		case 0x3:
			dbg() << "Fence/Fence.i";
			//Uninplemented? This VM doesn't have any memory cache
			consume();
			break;
		case 0x4:
			switch(((Itype*)bytes)->funct3) {
				case 0b000:
					regs.x[((Itype*)bytes)->rd]
						= regs.x[((Itype*)bytes)->rs1] + sign_extend12(((Itype*)bytes)->imm);
					dbg() << "addi x[" << ((Itype*)bytes)->rs1 << "]("
						<< regs.x[((Itype*)bytes)->rs1] << ") + imm("
						<< sign_extend12(((Itype*)bytes)->imm) << ") = x["
						<< ((Itype*)bytes)->rd << "]("
						<< regs.x[((Itype*)bytes)->rd] << ')';
					break;
				case 0b001:
					regs.x[((SHIFTtype*)bytes)->rd]
						= regs.x[((SHIFTtype*)bytes)->rs1] << sign_extend12(((SHIFTtype*)bytes)->shamt);
					dbg() << "slli x[" << ((SHIFTtype*)bytes)->rd << "] << "
						<< sign_extend12(((SHIFTtype*)bytes)->shamt) << " = "
						<< regs.x[((SHIFTtype*)bytes)->rd];
					break;
				case 0b010:
					dbg() << "slti x[" << ((Itype*)bytes)->rd << "] = "
						<< ((int64_t)regs.x[((Itype*)bytes)->rs1]) << " < "
						<< ((int64_t)sign_extend12(((Itype*)bytes)->imm)) << " ? "
						<< (bool)(((int64_t)regs.x[((Itype*)bytes)->rs1])
						< ((int64_t)sign_extend12(((Itype*)bytes)->imm)) ? 1 : 0);
					regs.x[((Itype*)bytes)->rd]
						= ((int64_t)regs.x[((Itype*)bytes)->rs1])
						< ((int64_t)sign_extend12(((Itype*)bytes)->imm)) ? 1 : 0;
					break;
				case 0b011:
					dbg() << "sltiu x[" << ((Itype*)bytes)->rd << "] = "
						<< ((uint64_t)regs.x[((Itype*)bytes)->rs1]) << " < "
						<< ((uint64_t)sign_extend12(((Itype*)bytes)->imm)) << " ? "
						<< (bool)(((uint64_t)regs.x[((Itype*)bytes)->rs1])
						< ((uint64_t)sign_extend12(((Itype*)bytes)->imm)) ? 1 : 0);
					regs.x[((Itype*)bytes)->rd]
						= ((uint64_t)regs.x[((Itype*)bytes)->rs1])
						< ((uint64_t)sign_extend12(((Itype*)bytes)->imm)) ? 1 : 0;
					break;
				case 0b100:
					regs.x[((Itype*)bytes)->rd]
						= regs.x[((Itype*)bytes)->rs1] ^ sign_extend12(((Itype*)bytes)->imm);
					dbg() << "xori x[" << ((Itype*)bytes)->rd << "] ^ "
						<< sign_extend12(((Itype*)bytes)->imm) << " = "
						<< regs.x[((Itype*)bytes)->rd];
					break;
				case 0b101:
					switch (((SHIFTtype*)bytes)->type) {
						case 0b0000000:
							regs.x[((SHIFTtype*)bytes)->rd]
								= regs.x[((SHIFTtype*)bytes)->rs1] >> sign_extend12(((SHIFTtype*)bytes)->shamt);
							dbg() << "srli x[" << ((SHIFTtype*)bytes)->rd << "] >>l "
								<< sign_extend12(((SHIFTtype*)bytes)->shamt) << " = "
								<< regs.x[((SHIFTtype*)bytes)->rd];
							break;
						case 0b0010000:
							regs.x[((SHIFTtype*)bytes)->rd]
								= arithmetic_right_shift
								(regs.x[((SHIFTtype*)bytes)->rs1]
								 ,  sign_extend12(((SHIFTtype*)bytes)->shamt));
							dbg() << "srai x[" << ((SHIFTtype*)bytes)->rd << "] >>a "
								<< sign_extend12(((SHIFTtype*)bytes)->shamt) << " = "
								<< regs.x[((SHIFTtype*)bytes)->rd];
							break;
						default:
							exception(Exception::Illegal_Instruction);
							return 1;
					}
					break;
				case 0b110:
					regs.x[((Itype*)bytes)->rd]
						= regs.x[((Itype*)bytes)->rs1] | sign_extend12(((Itype*)bytes)->imm);
					dbg() << "ori x[" << ((Itype*)bytes)->rd << "] | "
						<< sign_extend12(((Itype*)bytes)->imm) << " = "
						<< regs.x[((Itype*)bytes)->rd];
					break;
				case 0b111:
					regs.x[((Itype*)bytes)->rd]
						= regs.x[((Itype*)bytes)->rs1] & sign_extend12(((Itype*)bytes)->imm);
					dbg() << "andi x[" << ((Itype*)bytes)->rd << "] & "
						<< sign_extend12(((Itype*)bytes)->imm) << " = "
						<< regs.x[((Itype*)bytes)->rd];
					break;
				default:
					exception(Exception::Illegal_Instruction);
					return 1;
			}
			consume();
			break;
		case 0x6:
			switch(((Itype*)bytes)->funct3) {
				case 0b000:
					regs.x[((Itype*)bytes)->rd]
						= sign_extend32(regs.x[((Itype*)bytes)->rs1] + sign_extend12(((Itype*)bytes)->imm));
					dbg() << "addiw x[" << ((Itype*)bytes)->rd << "] + "
						<< sign_extend12(((Itype*)bytes)->imm) << " = "
						<< regs.x[((Itype*)bytes)->rd];
					break;
				case 0b001:
					regs.x[((SHIFTtype*)bytes)->rd]
						= sign_extend32(regs.x[((SHIFTtype*)bytes)->rs1] << sign_extend12(((SHIFTtype*)bytes)->shamt));
					dbg() << "slliw x[" << ((SHIFTtype*)bytes)->rd << "] << "
						<< sign_extend12(((SHIFTtype*)bytes)->shamt) << " = "
						<< regs.x[((SHIFTtype*)bytes)->rd];
					break;
				case 0b101:
					switch (((SHIFTtype*)bytes)->type) {
						case 0b0000000:
							regs.x[((SHIFTtype*)bytes)->rd]
								= sign_extend32(regs.x[((SHIFTtype*)bytes)->rs1] >> sign_extend12(((SHIFTtype*)bytes)->shamt));
							dbg() << "srliw x[" << ((SHIFTtype*)bytes)->rd << "] >>l "
								<< sign_extend12(((SHIFTtype*)bytes)->shamt) << " = "
								<< regs.x[((SHIFTtype*)bytes)->rd];
							break;
						case 0b0010000:
							regs.x[((SHIFTtype*)bytes)->rd]
								= sign_extend32(arithmetic_right_shift
								(regs.x[((SHIFTtype*)bytes)->rs1]
								 ,  sign_extend12(((SHIFTtype*)bytes)->shamt)));
							dbg() << "sraiw x[" << ((SHIFTtype*)bytes)->rd << "] >>a "
								<< sign_extend12(((SHIFTtype*)bytes)->shamt) << " = "
								<< regs.x[((SHIFTtype*)bytes)->rd];
							break;
						default:
							exception(Exception::Illegal_Instruction);
							return 1;
					}
					break;
				default:
					exception(Exception::Illegal_Instruction);
					return 1;
			}
			consume();
			break;
		case 0x8:

			switch(((Stype*)bytes)->funct3) {
				case 0b000:
					dbg() << "sb " << sign_extend12((((Stype*)bytes)->imm2 << 5)
							| ((Stype*)bytes)->imm1) + regs.x[((Stype*)bytes)->rs1] << " = "
						<< regs.x[((Stype*)bytes)->rs2];
					memory->write8(regs.x[((Stype*)bytes)->rs1]
							+ sign_extend12((((Stype*)bytes)->imm2 << 5)
							| ((Stype*)bytes)->imm1)
							, regs.x[((Stype*)bytes)->rs2] & 0xFF);
					break;
				case 0b001:
					dbg() << "sh " << sign_extend12((((Stype*)bytes)->imm2 << 5)
							| ((Stype*)bytes)->imm1) + regs.x[((Stype*)bytes)->rs1] << " = "
						<< regs.x[((Stype*)bytes)->rs2 & 0xFFFF];
					memory->write16(regs.x[((Stype*)bytes)->rs1]
							+ sign_extend12((((Stype*)bytes)->imm2 << 5)
							| ((Stype*)bytes)->imm1)
							, regs.x[((Stype*)bytes)->rs2]);
					break;
				case 0b010:
					dbg() << "sw " << sign_extend12((((Stype*)bytes)->imm2 << 5)
							| ((Stype*)bytes)->imm1) + regs.x[((Stype*)bytes)->rs1] << " = "
						<< regs.x[((Stype*)bytes)->rs2 & 0xFFFFFFFF];
					memory->write32(regs.x[((Stype*)bytes)->rs1]
							+ sign_extend12((((Stype*)bytes)->imm2 << 5)
							| ((Stype*)bytes)->imm1)
							, regs.x[((Stype*)bytes)->rs2]);
					break;
				case 0b011:
					dbg() << "sd " << sign_extend12((((Stype*)bytes)->imm2 << 5)
							| ((Stype*)bytes)->imm1) + regs.x[((Stype*)bytes)->rs1] << " = "
						<< regs.x[((Stype*)bytes)->rs2];
					memory->write64(regs.x[((Stype*)bytes)->rs1]
							+ sign_extend12((((Stype*)bytes)->imm2 << 5)
							| ((Stype*)bytes)->imm1)
							, regs.x[((Stype*)bytes)->rs2]);
					break;
				default:
					exception(Exception::Illegal_Instruction);
					return 1;
			}
			consume();
			break;
		case 0x5:
			dbg() << "auipc x[" << ((Utype*)bytes)->rd << "] = pc + "
				<< sign_extend32(((Utype*)bytes)->imm << 12);
			regs.x[((Utype*)bytes)->rd] = regs.pc + sign_extend32(((Utype*)bytes)->imm << 12);
			consume();
			break;
		case 0xB:
			switch (((AMOtype*)bytes)->funct3) {
				case 0b010:
					switch (((AMOtype*)bytes)->funct5) {
						case 0b1: {
								  // TODO: this may be wrong
								  uint32_t t = (int32_t)memory->read32(regs.x[((AMOtype*)bytes)->rs1]);
								  memory->write32(regs.x[((AMOtype*)bytes)->rs1]
										  , regs.x[((AMOtype*)bytes)->rs2]);
								  regs.x[((AMOtype*)bytes)->rs2] = t;
								  dbg() << "amoswap.w x[" << ((AMOtype*)bytes)->rd << "] = "
									  << regs.x[((AMOtype*)bytes)->rd] << " addr: "
									  << regs.x[((AMOtype*)bytes)->rs1] << " val: "
									  << regs.x[((AMOtype*)bytes)->rs2];
								  regs.x[((AMOtype*)bytes)->rd] = t;
									  break;
							  }
						case 0b0:
							dbg() << "amoadd.w";
							while (1) {}
							break;
					}
					break;
			}
			consume();
			break;
		case 0xC:
			switch (((Rtype*)bytes)->funct3) {
				case 0b000:
					switch (((Rtype*)bytes)->funct7) {
						case 0b0000000:
							dbg() << "add x[" <<((Rtype*)bytes)->rd
								<< "] = x[" << ((Rtype*)bytes)->rs1
								<< "](" << regs.x[((Rtype*)bytes)->rs1]
								<< ") + x[" << ((Rtype*)bytes)->rs2
								<< "](" << regs.x[((Rtype*)bytes)->rs2] << ')';
							regs.x[((Rtype*)bytes)->rd]
								= regs.x[((Rtype*)bytes)->rs1]
								+ regs.x[((Rtype*)bytes)->rs2];
							break;
						case 0b0100000:
							dbg() << "sub x[" <<((Rtype*)bytes)->rd
								<< "] = x[" << ((Rtype*)bytes)->rs1
								<< "](" << regs.x[((Rtype*)bytes)->rs1]
								<< ") - x[" << ((Rtype*)bytes)->rs2
								<< "](" << regs.x[((Rtype*)bytes)->rs2] << ')';
							regs.x[((Rtype*)bytes)->rd]
								= regs.x[((Rtype*)bytes)->rs1]
								- regs.x[((Rtype*)bytes)->rs2];
							break;
						case 0b0000001:
							dbg() << "mul x[" <<((Rtype*)bytes)->rd
								<< "] = x[" << ((Rtype*)bytes)->rs1
								<< "](" << regs.x[((Rtype*)bytes)->rs1]
								<< ") * x[" << ((Rtype*)bytes)->rs2
								<< "](" << regs.x[((Rtype*)bytes)->rs2] << ')';
							regs.x[((Rtype*)bytes)->rd]
								= (int64_t)regs.x[((Rtype*)bytes)->rs1]
								* (int64_t)regs.x[((Rtype*)bytes)->rs2];
							break;
						default:
							exception(Exception::Illegal_Instruction);
							return 1;
					}
					break;
				case 0b001:
					dbg() << "sll x[" <<((Rtype*)bytes)->rd
						<< "] = x[" << ((Rtype*)bytes)->rs1
						<< "](" << regs.x[((Rtype*)bytes)->rs1]
						<< ") << x[" << ((Rtype*)bytes)->rs2
						<< "](" << regs.x[((Rtype*)bytes)->rs2] << ')';
					regs.x[((Rtype*)bytes)->rd]
						= regs.x[((Rtype*)bytes)->rs1]
						<< regs.x[((Rtype*)bytes)->rs2];
					break;
				case 0b010:
					dbg() << "slt x[" <<((Rtype*)bytes)->rd
						<< "] < x[" << ((Rtype*)bytes)->rs1
						<< "](" << (int64_t)regs.x[((Rtype*)bytes)->rs1]
						<< ") < x[" << ((Rtype*)bytes)->rs2
						<< "](" << (int64_t)regs.x[((Rtype*)bytes)->rs2] << ")?";
					regs.x[((Rtype*)bytes)->rd]
						= (int64_t)regs.x[((Rtype*)bytes)->rs1]
						< (int64_t)regs.x[((Rtype*)bytes)->rs2] ? 1 : 0;
					break;
				case 0b011:
					dbg() << "sltu x[" <<((Rtype*)bytes)->rd
						<< "] < x[" << ((Rtype*)bytes)->rs1
						<< "](" << regs.x[((Rtype*)bytes)->rs1]
						<< ") < x[" << ((Rtype*)bytes)->rs2
						<< "](" << regs.x[((Rtype*)bytes)->rs2] << ")?";
					regs.x[((Rtype*)bytes)->rd]
						= regs.x[((Rtype*)bytes)->rs1]
						< regs.x[((Rtype*)bytes)->rs2] ? 1 : 0;
					break;
				case 0b100:
					dbg() << "xor x[" <<((Rtype*)bytes)->rd
						<< "] = x[" << ((Rtype*)bytes)->rs1
						<< "](" << regs.x[((Rtype*)bytes)->rs1]
						<< ") ^ x[" << ((Rtype*)bytes)->rs2
						<< "](" << regs.x[((Rtype*)bytes)->rs2] << ')';
					regs.x[((Rtype*)bytes)->rd]
						= regs.x[((Rtype*)bytes)->rs1]
						^ regs.x[((Rtype*)bytes)->rs2];
					break;
				case 0b101:
					switch (((Rtype*)bytes)->funct7) {
						case 0b0000000:
							dbg() << "srl x[" <<((Rtype*)bytes)->rd
								<< "] = x[" << ((Rtype*)bytes)->rs1
								<< "](" << regs.x[((Rtype*)bytes)->rs1]
								<< ") >>l x[" << ((Rtype*)bytes)->rs2
								<< "](" << regs.x[((Rtype*)bytes)->rs2] << ')';
							regs.x[((Rtype*)bytes)->rd]
								= regs.x[((Rtype*)bytes)->rs1]
								>> regs.x[((Rtype*)bytes)->rs2];
							break;
						case 0b0100000:
							dbg() << "sra x[" <<((Rtype*)bytes)->rd
								<< "] = x[" << ((Rtype*)bytes)->rs1
								<< "](" << regs.x[((Rtype*)bytes)->rs1]
								<< ") >>a x[" << ((Rtype*)bytes)->rs2
								<< "](" << regs.x[((Rtype*)bytes)->rs2] << ')';
							regs.x[((Rtype*)bytes)->rd]
								=arithmetic_right_shift(regs.x[((Rtype*)bytes)->rs1]
										,regs.x[((Rtype*)bytes)->rs2]);
							break;
						default:
							exception(Exception::Illegal_Instruction);
							return 1;
					}
				case 0b110:
					dbg() << "or x[" <<((Rtype*)bytes)->rd
						<< "] = x[" << ((Rtype*)bytes)->rs1
						<< "](" << regs.x[((Rtype*)bytes)->rs1]
						<< ") | x[" << ((Rtype*)bytes)->rs2
						<< "](" << regs.x[((Rtype*)bytes)->rs2] << ')';
					regs.x[((Rtype*)bytes)->rd]
						= regs.x[((Rtype*)bytes)->rs1]
						| regs.x[((Rtype*)bytes)->rs2];
					break;
				case 0b111:
					dbg() << "and x[" <<((Rtype*)bytes)->rd
						<< "] = x[" << ((Rtype*)bytes)->rs1
						<< "](" << regs.x[((Rtype*)bytes)->rs1]
						<< ") & x[" << ((Rtype*)bytes)->rs2
						<< "](" << regs.x[((Rtype*)bytes)->rs2] << ')';
					regs.x[((Rtype*)bytes)->rd]
						= regs.x[((Rtype*)bytes)->rs1]
						& regs.x[((Rtype*)bytes)->rs2];
					break;
				default:
					exception(Exception::Illegal_Instruction);
					return 1;
			}
			consume();
			break;
		case 0xD:
			dbg() << "lui x[" << ((Utype*)bytes)->rd << "] = "
				<< sign_extend32(((Utype*)bytes)->imm << 12);
			regs.x[((Utype*)bytes)->rd] = sign_extend32(((Utype*)bytes)->imm << 12);
			consume();
			break;
		case 0x18: {
			switch (((Btype*)bytes)->funct3) {
				case 0b000: {
					if (regs.x[((Btype*)bytes)->rs1] == regs.x[((Btype*)bytes)->rs2]) {
						regs.pc += sign_extend12(((Btype*)bytes)->imm1 << 10
							| ((Btype*)bytes)->imm2 << 1
							| ((Btype*)bytes)->imm3 << 5
							| ((Btype*)bytes)->imm4 << 11);
						dbg() << "beq x[" << ((Btype*)bytes)->rs1 << "]("
							<< regs.x[((Btype*)bytes)->rs1] 
							<< ") == x[" << ((Btype*)bytes)->rs2 << "]("
							<< regs.x[((Btype*)bytes)->rs2]
							<< ") ? returned true, jumping to " << regs.pc;
						index = regs.pc / 4;
						return 0;
					} else {
						dbg() << "beq false";
						consume();
					}
					break;
					    }
				case 0b001:
					if (regs.x[((Btype*)bytes)->rs1] != regs.x[((Btype*)bytes)->rs2]) {
						regs.pc += sign_extend12(((Btype*)bytes)->imm1 << 10
							| ((Btype*)bytes)->imm2 << 1
							| ((Btype*)bytes)->imm3 << 5
							| ((Btype*)bytes)->imm4 << 11);
						dbg() << "bne x[" << ((Btype*)bytes)->rs1 << "]("
							<< regs.x[((Btype*)bytes)->rs1] 
							<< ") != x[" << ((Btype*)bytes)->rs2 << "]("
							<< regs.x[((Btype*)bytes)->rs2]
							<< ") ? returned true, jumping to " << regs.pc;
						index = regs.pc / 4;
					} else {
						dbg() << "bne false";
						consume();
					}
					break;
				case 0b100:
					if ((int64_t)regs.x[((Btype*)bytes)->rs1] < (int64_t)regs.x[((Btype*)bytes)->rs2]) {
						regs.pc += sign_extend12(((Btype*)bytes)->imm1 << 10
							| ((Btype*)bytes)->imm2 << 1
							| ((Btype*)bytes)->imm3 << 5
							| ((Btype*)bytes)->imm4 << 11);
						dbg() << "blt to " << regs.pc;
						index = regs.pc / 4;
					} else {
						dbg() << "blt false";
						consume();
					}
					break;
				case 0b101:
					if ((int64_t)regs.x[((Btype*)bytes)->rs1] >= (int64_t)regs.x[((Btype*)bytes)->rs2]) {
						regs.pc += sign_extend12(((Btype*)bytes)->imm1 << 10
							| ((Btype*)bytes)->imm2 << 1
							| ((Btype*)bytes)->imm3 << 5
							| ((Btype*)bytes)->imm4 << 11);
						dbg() << "bge to " << regs.pc;
						index = regs.pc / 4;
					} else {
						dbg() << "bge false";
						consume();
					}
					break;
				case 0b110:
					if (regs.x[((Btype*)bytes)->rs1] < regs.x[((Btype*)bytes)->rs2]) {
						regs.pc += sign_extend12(((Btype*)bytes)->imm1 << 10
							| ((Btype*)bytes)->imm2 << 1
							| ((Btype*)bytes)->imm3 << 5
							| ((Btype*)bytes)->imm4 << 11);
						dbg() << "bltu to " << regs.pc;
						index = regs.pc / 4;
					} else {
						dbg() << "bltu false";
						consume();
					}
					break;
				case 0b111:
					if (regs.x[((Btype*)bytes)->rs1] <= regs.x[((Btype*)bytes)->rs2]) {
						regs.pc += sign_extend12(((Btype*)bytes)->imm1 << 10
							| ((Btype*)bytes)->imm2 << 1
							| ((Btype*)bytes)->imm3 << 5
							| ((Btype*)bytes)->imm4 << 11);
						dbg() << "bgeu to " << regs.pc;
						index = regs.pc / 4;
					} else {
						dbg() << "bgeu false";
						consume();
					}
					break;
			}
			break;
			   }
		case 0x19: {
				   dbg() << "jalr to " << ((regs.x[((Itype*)bytes)->rs1]
							   + sign_extend12(((Itype*)bytes)->imm)) & ~1);
				   uint64_t t = regs.pc + 4; // TODO: we might be skipping one instruction here
				   regs.pc = (regs.x[((Itype*)bytes)->rs1] + sign_extend12(((Itype*)bytes)->imm)) & ~1;
				   index = ((regs.x[((Itype*)bytes)->rs1] + sign_extend12(((Itype*)bytes)->imm)) & ~1) / 4;
				   regs.x[((Itype*)bytes)->rd] = t;
				   return 0;
				   /*
				   */
			   }
		case 0x1B:  {
				    regs.pc += sign_extend20(((Jtype*)bytes)->imm4 << 19
						    | ((Jtype*)bytes)->imm3 << 1
						    | ((Jtype*)bytes)->imm2 << 10
						    | ((Jtype*)bytes)->imm1 << 11);
				    dbg() << "jal to " << regs.pc;
				    index = regs.pc / 4;
				    return 0;

			    }
		case 0x1C:
			switch(((Itype*)bytes)->funct3) {
				case 0b000: {
						    dbg() << ((Instruction*)bytes)->data;
						    switch (((Instruction*)bytes)->data) {
							    case 0x20A000:
								    wait_for_interrupt();
								    break;
							    case 0x004000: // uret
							    case 0x204000: // sret
							    case 0x404000: // hret
								    while(1) {};
								    break;
							    case 0x604000: // mret
								    exception_return();
								    return 0;
							    case 0x002000: // ebreak
							    case 0:
								    exception(Exception::ECall_Machine_Mode);
								    return 1;
						    }
					    }
				case 0b001: {
						    //TODO: CHECK IF YOU'RE ALLOWED TO READ THE CSR
						    uint64_t t = csrs.get_csr(((Itype*)bytes)->imm);
						    csrs.set_csr(((Itype*)bytes)->imm, regs.x[((Itype*)bytes)->rs1]);
						    regs.x[((Itype*)bytes)->rd] = t;
						    dbg() << "csrrw csr[" << regs.x[((Itype*)bytes)->imm]
							    << "] = " << csrs.get_csr(((Itype*)bytes)->rd)
							    << " with rd = " << ((Itype*)bytes)->rd;
						    consume();
						    break;
					    }
				case 0b010: {
						    //TODO: CHECK IF YOU'RE ALLOWED TO READ THE CSR
						    uint64_t t = csrs.get_csr(((Itype*)bytes)->imm);
						    csrs.set_csr(((Itype*)bytes)->imm, t | regs.x[((Itype*)bytes)->rs1]);
						    regs.x[((Itype*)bytes)->rd] = t;
						    dbg() << "csrrs csr[" << regs.x[((Itype*)bytes)->imm]
							    << "] = " << csrs.get_csr(((Itype*)bytes)->rd)
							    << " with rd = " << ((Itype*)bytes)->rd;
						    consume();
						    break;
					    }
				case 0b011: {
						    //TODO: CHECK IF YOU'RE ALLOWED TO READ THE CSR
						    uint64_t t = csrs.get_csr(((Itype*)bytes)->imm);
						    csrs.set_csr(((Itype*)bytes)->imm, t & ~regs.x[((Itype*)bytes)->rs1]);
						    regs.x[((Itype*)bytes)->rd] = t;
						    dbg() << "csrrc csr[" << regs.x[((Itype*)bytes)->imm]
							    << "] = " << csrs.get_csr(((Itype*)bytes)->rd)
							    << " with rd = " << ((Itype*)bytes)->rd;
						    consume();
						    break;
					    }
				case 0b101: {
						    //TODO: CHECK IF YOU'RE ALLOWED TO READ THE CSR
						    csrs.set_csr(((Itype*)bytes)->imm, (uint64_t)((Itype*)bytes)->rs1);
						    regs.x[((Itype*)bytes)->rd] = csrs.get_csr(((Itype*)bytes)->imm);
						    dbg() << "csrrwi csr[" << regs.x[((Itype*)bytes)->imm]
							    << "] = " << csrs.get_csr(((Itype*)bytes)->rd)
							    << " with rd = " << ((Itype*)bytes)->rd;
						    consume();
						    break;
					    }
				case 0b110: {
						    //TODO: CHECK IF YOU'RE ALLOWED TO READ THE CSR
						    uint64_t t = csrs.get_csr(((Itype*)bytes)->imm);
						    csrs.set_csr(((Itype*)bytes)->imm, t | (uint64_t)((Itype*)bytes)->rs1);
						    regs.x[((Itype*)bytes)->rd] = t;
						    dbg() << "csrrsi csr[" << regs.x[((Itype*)bytes)->imm]
							    << "] = " << csrs.get_csr(((Itype*)bytes)->rd)
							    << " with rd = " << ((Itype*)bytes)->rd;
						    consume();
						    break;
					    }
				case 0b111: {
						    //TODO: CHECK IF YOU'RE ALLOWED TO READ THE CSR
						    uint64_t t = csrs.get_csr(((Itype*)bytes)->imm);
						    csrs.set_csr(((Itype*)bytes)->imm, t & ~(uint64_t)((Itype*)bytes)->rs1);
						    regs.x[((Itype*)bytes)->rd] = t;
						    dbg() << "csrrci csr[" << regs.x[((Itype*)bytes)->imm]
							    << "] = " << csrs.get_csr(((Itype*)bytes)->rd)
							    << " with rd = " << ((Itype*)bytes)->rd;
						    consume();
						    break;
					    }
				default:
					    exception(Exception::Illegal_Instruction);
					    return 1;
			}
			break;



		default:
			exception(Exception::Illegal_Instruction);
			return 1;
			consume();

	}
	return 0;
}
