#include <mach/task.h>
#include <mach/mach_init.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * This technique was introduced in a Secuinside presentation in 2012
 * which can be found here: https://web.archive.org/web/20160513183512/https://reverse.put.as/wp-content/uploads/2012/07/Secuinside-2012-Presentation.pdf
 *
 * In macOS, debuggers listen on what are called "exception ports". We
 * can check if a program is being debugged by enumerating these ports
 * and looking for ones that aren't NULL.
 *
 * A fantastic tutorial for how to defeat this check can be found here:
 * https://alexomara.com/blog/defeating-anti-debug-techniques-macos-mach-exception-ports/
 *
 * @return  true if a debugger is detected, false otherwise.
 */
bool checkIfBeingDebugged(void) {
	exception_mask_t mask = EXC_MASK_BAD_ACCESS
	                        | EXC_MASK_BAD_INSTRUCTION
	                        | EXC_MASK_ARITHMETIC
	                        | EXC_MASK_EMULATION
	                        | EXC_MASK_SOFTWARE
	                        | EXC_MASK_BREAKPOINT
	                        | EXC_MASK_SYSCALL
	                        | EXC_MASK_MACH_SYSCALL
	                        | EXC_MASK_RPC_ALERT
	                        | EXC_MASK_CRASH;
	exception_mask_t masks[EXC_TYPES_COUNT];
	mach_msg_type_number_t count = 0;
	mach_port_t ports[EXC_TYPES_COUNT];
	exception_behavior_t behaviors[EXC_TYPES_COUNT];
	thread_state_flavor_t flavors[EXC_TYPES_COUNT];

	// Ask kernel for a bunch of information about this our own task.
	// We're really only interested in the ports array
	kern_return_t result = task_get_exception_ports(mach_task_self(), mask, masks, &count, ports, behaviors, flavors);

	if (result == KERN_SUCCESS) {
		// Iterate through each port and test if it's valid. If it is, we're being debugged.
		for (mach_msg_type_number_t idx = 0; idx < count; idx++) {
			if (MACH_PORT_VALID(ports[idx])) {
				return true;
			}
		}
	}

	// If we end up here, we didn't find a port that was valid, so we're not being debugged.
	return false;
}

int main() {
	if (checkIfBeingDebugged()) {
		printf("Debugger detected!\n");
	} else {
		printf("No debugger detected!\n");
	}

	return 0;
}
