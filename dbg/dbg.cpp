#include "../hpp/dbg/dbg.hpp"

Regs* dbg::regs = 0;

std::mutex dbg::mutex;

dbg::dbg() {
	mutex.lock();
	if (regs)
		std::cout << "[2;30;1m[[0;33;2m" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << regs->pc << "[30;1m][m ";
	else
		std::cout << "[2;30;1m[[0;33;2m" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << 0 << "[30;1m][m ";
}

dbg::~dbg() {
	mutex.unlock();
	std::cout << std::endl;
}

uint64_t dbg::binary(uint64_t integer) {
	uint8_t j = 0;
	for (uint8_t i = 0; i < 64; i++) {
		if(!(integer & (((uint64_t)1 << 63) >> i))) {
			std::cout << 0;
			j++;
		} else {
			std::cout << 1;
		}
	}
	std::cout << std::endl;
	return integer;
}

dbg::dbg(const char* str) {
	std::cout << str;
}

dbg& dbg::operator<< (const char c) {
	std::cout << c;
	return *this;
}

dbg& dbg::operator<< (const char* str) {
	std::cout << str;
	return *this;
}
#define PRINT(a)  							\
dbg& dbg::operator<< (a i) {						\
	std::cout << std::hex << "0x" << i;				\
	return *this;							\
}

#define PRINTI(a)  							\
dbg& dbg::operator<< (a i) {						\
	if ((uint64_t)i > ((uint64_t)0xFFFFFFFF << 31))			\
		std::cout << std::hex << "0x" << i			\
			<< '(' << std::dec << i << ')' << std::hex;	\
	else								\
		std::cout << std::hex << "0x" << i;			\
	return *this;							\
}

PRINT(uint8_t)
PRINTI(int8_t)
PRINT(uint16_t)
PRINTI(int16_t)
PRINT(uint32_t)
PRINTI(int32_t)
PRINT(uint64_t)
PRINTI(int64_t)
PRINT(uintmax_t)
PRINTI(intmax_t)

dbg& dbg::operator<< (bool i) {
	if (i)
		std::cout << "true";
	else
		std::cout << "false";
	return *this;
}

dbg& dbg::operator<< (void* ptr) {
	std::cout << ptr;
	return *this;
}
