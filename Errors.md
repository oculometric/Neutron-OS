When Neutron boots, it has various stages of verification to ensure it has the correct environment to run in. If when trying to boot you get a blank screen with ‘ERR’ highlighted in red in the top left corner, one of the boot checks has failed.

* **ERR 0** - no multiboot error. The OS must be loaded by a multiboot bootloader.
* **ERR 1** - no CPUID error. The CPUID instruction is provided by newer CPUs, the CPU you’re running on may be too old.
* **ERR 2** - no long mode. The CPU you’re using does not support long mode.