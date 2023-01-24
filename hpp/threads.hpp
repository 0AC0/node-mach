#pragma once
#include <stdint.h>
#include <vector>
#include <thread>
#include "cpu/cpu.hpp"
#include "devices/device.hpp"

class Memory;
class CPU;

class Threads {
	private:
		static Threads* t;
	public:
		struct CPUEntry {
			CPU* cpu;
			std::thread* thread;
		};
		struct DeviceEntry {
			Device* device;
			std::thread* thread;
		};
		std::vector<CPUEntry> cpus;
		std::vector<DeviceEntry> devices;

		static void start_all_cpus(Memory* m, uint64_t entry);
		static void join_all_cpus();
		static void pause_all_cpus();
		static void continue_all_cpus();

		static void start_device(Device* d);

		static Threads& instance() {
			if (!t) t = new Threads();
			return *t;
		}

};
