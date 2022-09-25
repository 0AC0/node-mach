#include "../hpp/dbg/debugger.hpp"

debugger* debugger::d = 0;

void debugger::operator()(int){
	Threads::pause_all_cpus();
	
	std::this_thread::sleep_for(std::chrono::seconds(1));

	char command[40] = {0};
	while (1) {
		memset(command, 0, 40);

		std::cout << "dbg > ";
		int ret = scanf("%39s", command);

		if (std::strncmp(command, "resume", 6) == 0) {
			Threads::continue_all_cpus();
			break;
		} else if (std::strncmp(command, "step", 4) == 0) {
			Threads::instance().cpus[0].cpu->step();
		} else if (std::strncmp(command, "ls", 2) == 0) {
			for (uint8_t j = 0; j < 32; j++) {
				dbg("[2;30;1m[[0;36;2m   LS   [30;1m][m ")
					<< Threads::instance().cpus[0].cpu->regs.name[j] << ": "
					<< Threads::instance().cpus[0].cpu->regs.x[j];
			}
		} else if (std::strncmp(command, "exit", 4) == 0) {
			exit(0);
		}
		if (ret == EOF) exit(0);
	}
}

debugger::debugger() {

	signal(SIGINT, &handler);
}
