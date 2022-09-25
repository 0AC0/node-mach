#include "../hpp/dbg/debugger.hpp"

debugger* debugger::d = 0;

void debugger::operator()(int){
	Threads::pause_all_cpus();
	
	// wait 1 second for all the threads to acknowledge
	std::this_thread::sleep_for(std::chrono::seconds(1));

	char command[40] = {0};
	while (1) {
		memset(command, 0, 40);

		std::cout << "dbg > ";
		char* ret = fgets(command, 39, stdin);

		if (ret == nullptr) exit(0);
		if (std::strncmp(command, "resume", 6) == 0) {
			Threads::continue_all_cpus();
			break;
		} else if (std::strncmp(command, "stepto", 6) == 0) {
			try {
				uint64_t addr = std::stoi(command + 7, nullptr, 16);
				if (addr < Memory::MEM_START) addr += Memory::MEM_START;

				Threads::instance().cpus[0].cpu->step();
				while (Threads::instance().cpus[0].cpu->regs.pc != addr) {
					if (Threads::instance().cpus[0].cpu->regs.pc % 4) {
						dbg() << "Misaligned addr";
						break; 
					}
					Threads::instance().cpus[0].cpu->step();
				}
			} catch (std::exception &err) {
				dbg() << "Invalid addr: " << command + 5;
			}
		} else if (std::strncmp(command, "step", 4) == 0) {
			Threads::instance().cpus[0].cpu->step();
		} else if (std::strncmp(command, "show", 4) == 0) {
			for (uint8_t i = 0; i < 40; i++)
				std::cout << command[i];
		} else if (std::strncmp(command, "read64", 6) == 0) {
			try {
				uint64_t addr = std::stoi(command + 7, nullptr, 16);
				if (addr < Memory::MEM_START) addr += Memory::MEM_START;
				for (uint8_t i = 0; i < 32; i += 8) {
					std::cout << addr + i << ':';
					std::cout
						<< std::hex
						<< std::uppercase
						<< std::setfill('0')
						<< std::setw(16)
						<< Threads::instance().cpus[0].cpu->memory->read64(addr + i)
						<< '\n';
				}
			} catch (std::exception &err) {
				dbg() << "Invalid addr: " << command + 5;
			}
		} else if (std::strncmp(command, "read32", 6) == 0) {
			try {
				uint64_t addr = std::stoi(command + 7, nullptr, 16);
				if (addr < Memory::MEM_START) addr += Memory::MEM_START;
				for (uint8_t i = 0; i < 16; i += 4) {
					std::cout << addr + i << ':';
					std::cout
						<< std::hex
						<< std::uppercase
						<< std::setfill('0')
						<< std::setw(8)
						<< Threads::instance().cpus[0].cpu->memory->read32(addr + i)
						<< '\n';
				}
			} catch (std::exception &err) {
				dbg() << "Invalid addr: " << command + 5;
			}
		} else if (std::strncmp(command, "ls", 2) == 0) {
			for (uint8_t j = 0; j < 32; j++) {
				dbg("[2;30;1m[[0;36;2m   LS   [30;1m][m ")
					<< Threads::instance().cpus[0].cpu->regs.name[j] << ": "
					<< Threads::instance().cpus[0].cpu->regs.x[j];
			}
		} else if (std::strncmp(command, "exit", 4) == 0) {
			exit(0);
		}
	}
}

debugger::debugger() {

	signal(SIGINT, &handler);
}
