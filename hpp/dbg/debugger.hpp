#pragma once
#include <stdint.h>
#include <signal.h>
#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include "dbg.hpp"
#include "../threads.hpp"
#include "../cpu/cpu.hpp"

class debugger {
	private:
		static debugger* d;
	public:
		static debugger& instance() {
			if (!d) d = new debugger();
			return *d;
		}

		static void handler(int) {
			instance()(0);
		}

		debugger();
		void operator() (int);
};
