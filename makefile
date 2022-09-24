BUILDDIR  = build
SRCDIR    = .

SRCFILES  := $(shell find ${SRCDIR} -type f -name "*.cpp" -print)
OBJ       := $(SRCFILES:%.cpp=%.o)

OPTIMIZATIONLEVEL = 0
# TODO: sse and sse2 and mmx
CXXFLAGS     := -std=gnu++20 -Wall -Wextra -O${OPTIMIZATIONLEVEL}
LDFLAGS = -macosx_version_min 12.0 -lSystem -syslibroot `xcrun -sdk macosx --show-sdk-path` -arch arm64

TARGET     = ${BUILDDIR}/machine.bin

all: start $(TARGET)
	@echo ' '"Build successful.\n"

start:
	@echo ' '"\n\n\033[1;2;32m--------------- NEW BUILD ---------------\033[m\n\n"
	@mkdir -p ${BUILDDIR}

$(TARGET): $(OBJ)
	@echo ' '"[MSG]  -   OBJ build successful"
	@echo ' '[LNK] ' >  ' ${TARGET}
	@g++ $(shell ls ${BUILDDIR}/*.o) -o ${TARGET}
	@echo ' '"[MSG]  -   ""Final size: $$(du -sh ${TARGET} | cut -c -4)\n"

%.o: %.cpp
	@echo ' '"[CPP]  >  " $(@F)
	@${CXX} $(CXXFLAGS) -c $< -o ${BUILDDIR}/$(@F)

run:
	@echo ' '"[MSG]  -   ""Running...\n"
	@build/machine.bin build/payload.elf

test:
	riscv64-unknown-elf-g++ -ffreestanding -MD -fno-omit-frame-pointer -gdwarf-2 -march=rv64imad -fno-common -nostdlib -mno-relax -T ../machine-tests/common.ld ../machine-tests/test.cpp -o build/payload.elf
	riscv64-unknown-elf-objdump -S build/payload.elf > build/payload.asm
	

.PHONY: run-update
run-update: all run 

.PHONY: test-update
test-update: all test run

.PHONY: clean
clean:
	${RM} -r ${BUILDDIR}/*
