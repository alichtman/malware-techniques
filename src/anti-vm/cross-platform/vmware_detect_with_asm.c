#include <stdio.h>
#include <stdbool.h>
#include "../../../lib/SignalRecovery/signal_recovery.h"

/**
 * This technique is drawn from a Symantec paper titled: "Attacks on Virtual
 * Machine Emulators" and can be found at:
 * https://www.symantec.com/content/dam/symantec/docs/security-center/white-papers/attacks-on-virtual-machine-emulators-07-en.pdf
 *
 * In short, this technique relies on the differing behavior of the IN
 * instruction inside vs. outside of a VMware box. This assembly routine asks
 * the kernel for the VMware version. If we are inside of a VMware VM, the IN
 * instruction won't error out, since the information we are asking for does
 * actually exist. If we are not, that call will generate an exception, and this
 * program will be killed. This signal is hooked with the signal_recovery library
 * to allow the program to exit cleanly and prettify the output.
 */

/**
 * @return Returns 1 if VMware is detected, and throws an exception otherwise.
 */
bool detect_vmware_with_assembly_trick() {

	asm ("movl $0x564d5868, %%eax\n" // 'VMXh'
	     "movl $0xa, %%ecx\n" // Get VMware version
	     "movw $0x5658, %%dx\n" // 'VX'
	     "in  %%dx, %%eax\n" // This instruction will generate an exception if not running in VMware
	     "cmp $0x564d5868, %%ebx\n" // 'VMXh'
	     "pushf\n"      // Put FLAGS register on stack
	     "pop %%ax\n"   // Put FLAGS register into ax
	     "push %%ax\n"  // Copy onto stack for storage
	     "andl $0x40, %%eax\n"  // Isolate zero flag from FLAGS register in eax by masking with 0x40
	     "shrl $6, %%eax\n" // Set eax equal to either 0 or 1. 1 means it was detected.
	     "popf\n" // Restore FLAGS
	     : // No outputs
	     : // No inputs
	     :"eax", "ebx", "ax", "dx"); // clobbered registers

	// eax should be implicitly returned, and will be cast to a boolean.
}

int main() {
	signal_catch_init();

	bool detected = false;
	signal_try(detect) {
		detected = detect_vmware_with_assembly_trick();
	}
	signal_catch(detect) {
		printf("Did not detect VMware!\n");
	}
	signal_end(detect)

	if (detected) {
		printf("Detected VMware!\n");
	}

	return 0;
}
