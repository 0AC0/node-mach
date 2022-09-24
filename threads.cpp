#include "hpp/threads.hpp"

Threads* Threads::t = 0;

void Threads::start_all_cpus(Memory* m, uint64_t entry) {
	instance().cpus.reserve(1);
	instance().cpus[0].cpu = new CPU(m);
	instance().cpus[0].thread = new std::thread(instance().cpus[0].cpu->run, instance().cpus[0].cpu, entry);
}

void Threads::pause_all_cpus() {
	instance().cpus[0].cpu->pause();
}

void Threads::continue_all_cpus() {
	instance().cpus[0].cpu->resume();
}

void Threads::join_all_cpus() {
	instance().cpus[0].thread->join();
}
