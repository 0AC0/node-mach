#include "hpp/threads.hpp"

Threads* Threads::t = 0;

void Threads::start_all_cpus(Memory* m, uint64_t entry) {
	instance().cpus.reserve(2);
	instance().cpus[0].cpu = new CPU(m, 0);
	instance().cpus[0].thread = new std::thread(instance().cpus[0].cpu->run, instance().cpus[0].cpu, entry);
	instance().cpus[1].cpu = new CPU(m, 1);
	instance().cpus[1].thread = new std::thread(instance().cpus[1].cpu->run, instance().cpus[1].cpu, entry);
	//TODO: this is hacky but temporary until we implement multi cpu support
	dbg::regs = &(instance().cpus[0].cpu->regs);
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

void Threads::start_device(Device* d) {
	DeviceEntry* de = new DeviceEntry;
	de->device = d;
	//de->thread = new std::thread(de->device->run);

	instance().devices.push_back(*de);
}
