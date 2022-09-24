#pragma once
#include <stdint.h>
#include <vector>
#include <thread>
#include "cpu/cpu.hpp"


class Threads {
	private:
		static Threads* t;
		struct CPUEntry {
			CPU* cpu;
			std::thread* thread;
		};
		std::vector<CPUEntry> cpus;
	public:
		static void start_all_cpus(Memory* m, uint64_t entry);
		static void join_all_cpus();
		static void pause_all_cpus();
		static void continue_all_cpus();

		static Threads& instance() {
			if (!t) t = new Threads();
			return *t;
		}

};
