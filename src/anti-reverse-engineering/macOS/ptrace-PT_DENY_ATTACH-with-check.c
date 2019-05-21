#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <unistd.h>

/**
 * This technique builds on top of the basic PT_DENY_ATTACH anti-debug
 * approach in macOS by adding a check to see if the PT_DENY_ATTACH call
 * was successful.
 *
 * This technique is explained well in Alex Mara's blog post, linked below.
 *
 * https://alexomara.com/blog/defeating-anti-debug-techniques-macos-ptrace-variants/
 */

bool deny_attach_successful = false;

void sigsegv_handler(int sig) {
    printf("SEGFAULT handler hit.\n");
    deny_attach_successful = true;
}

int main() {
	printf("Executing first PT_DENY_ATTACH.\n");
	// PT_DENY_ATTACH stops any process from being attached to this process
	ptrace(PT_DENY_ATTACH, 0, 0, 0);

	// Register SEGFAULT handler
	signal(SIGSEGV, sigsegv_handler);
	
	// Attempt to attach to our own process. If the first ptrace call was not
	// tampered with, this will cause a SEGFAULT, which will increment the
	// deny_attach_successful variable in the SEGFAULT handler.
	ptrace(PT_ATTACH, pid, 0, 0);

	if (deny_attach_successful) {
		printf("No debugger detected.\n");
		return -1;
	} else {
		printf("Debugger detected.\n");
		return 0;
	}
}
