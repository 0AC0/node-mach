![RISC-V](https://riscv.org/wp-content/uploads/2020/06/riscv-color.svg)

## This repo is part of the NodeNet project
Official NodeNet website: [NodeNet.work](http://nodenet.work)

# NodeMach
NodeMach, the RISC-V 64 virtual environment for NodeNet

### NodeMach?
NodeMach is a virtual environment standard for running the operating system NodeNet. 
It acts as a layer between devices, like the CPU or the network adapter, that translates the access protocols into simpler, more versatile versions.
It is enetially a RISC-V emulator virtual machine used for running NodeNet.

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
 - No paging
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
