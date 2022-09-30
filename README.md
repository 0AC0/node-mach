![RISC-V](https://riscv.org/wp-content/uploads/2020/06/riscv-color.svg)

# NodeMach
NodeMach, the RISC-V 64 virtual environment for NodeNet

### NodeMach?
NodeMach is a virtual environment standard for running the operating system NodeNet. 
It acts as a layer between devices, like the CPU or the network adapter, that translates the access protocols into simpler, more versatile versions.
It is essentially a RISC-V emulator virtual machine used for running NodeNet.

### NodeNet?
A network based operating system based on the idea that everything is a node and can be connected to another node provided the nodes are compatible. (ex. /path/to/image -> /path/to/framebuffer)

### Current project status
![platform](https://img.shields.io/badge/Platform-MacOS-brightgreen)
![commit](https://img.shields.io/github/last-commit/0ac0/node-mach)
![build](https://img.shields.io/github/workflow/status/0AC0/node-mach/Make%20simple%20CI)
![lang](https://img.shields.io/github/languages/top/0ac0/node-mach)
![code](https://img.shields.io/github/languages/code-size/0ac0/node-mach)
![nodenet](https://img.shields.io/badge/NodeNet-Download%20Soon-red)
<p>

 - Complete rv64i implementation
 - Incomplete rv64m implementation
 - Incomplete rv64a implementation
 - Single hart CPU
 - Basic 39bit paging
 - Hacky device support
 - No PLIC
 - No CLINT
 - Bare bones CSR support

</p>

<details><summary>Building</summary>

Dependencies: `clang, make`

#### build with
```
make
```
#### run with
```
cp /path/to/your/payload.elf build/payload.elf 
make run
```
#### build and run with
```
cp /path/to/your/payload.elf build/payload.elf 
make run-update
```

</details>

<details><summary>Debugging</summary>

^C (Ctrl + C) or send the process a SIGINT to pull up the debugger.

| Command | Description |
| --- | --- |
| show | Show command buffer(used for debugging of the debugger) |
| step | Step one CPU cycle |
| stepto \<addr\> | Run the CPU until the program counter is equal to \<addr\> |
| read32 \<addr\> | Read 4x 32bit values from \<addr\> |
| read64 \<addr\> | Read 4x 64bit values from \<addr\> |
| ls | List CPU registers |
| resume | Exit debugger, continuing to run the payload |
| exit | Exit the debugger, exiting the program |

</details>


## This repo is part of the NodeNet project
Official NodeNet website: [NodeNet.work](http://nodenet.work)
