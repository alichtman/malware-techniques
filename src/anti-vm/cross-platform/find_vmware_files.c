#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <glob.h>
#include <errno.h>
#include "../../utils/print.h"

char* linux_vmware_files[] = {"run/vmware",
                              "etc/vmware-tools",
                              "var/lib/vmware",
                              "var/log/vmware*",
                              "usr/bin/vmware*",
                              "lib/udev/rules.d/99-vmware-scsi-udev.rules",
                              "etc/vmware-tools/scripts/vmware",
                              "usr/share/*/xserver-xorg-video-vmware-hwe-*",
                              "var/lib/dpkg/info/xserver-xorg-video-vmware-hwe-*",
                              "usr/share/grub-gfxpayload-lists/blacklist/10_vmware",
                              "usr/share/man/man4/vmware.4.gz",
                              "usr/share/man/man1/vmware-*",
                              "usr/share/apport/package-hooks/source_xserver-xorg-video-vmware.py",
                              "usr/lib/xorg/modules/drivers/vmware_drv.so",
                              "usr/share/open-vm-tools/messages/*/vmware-*",
                              "usr/src/linux-headers-*-generic/include/config/vmware",
                              "usr/src/linux-headers-*-generic/include/config/infiniband/vmware"
                             };

int glob_err(const char* path, int err_num) {
	fprintf(stderr, "%s: %s\n", path, strerror(err_num));
	return 0;
}

/**
 * Looks for files that would indicate we are inside a VMware Virtual Machine.
 * Some file paths have wildcards in them, others are absolute paths. Access the
 * absolute paths to check if they exist, and count the number of paths that are
 * returned from glob to see if the wildcard paths exist.
 * @return True if any files exist, False otherwise.
 */
bool find_vmware_files() {
	bool found_files = false;
	// Iterate through vmware files array.
	// If * in file name, glob and see if there are any results
	// If no *, just access file and see if it exists.
	char** files = linux_vmware_files;

	while (*files) {
		if (strchr(*files, '*')) { // Need to glob because wildcard is in list.
			// int glob(const char * restrict pattern, int flags, int (*errfunc)(const char *epath, int errno), glob_t * restrict pglob);
			glob_t globbed_paths;
			memset(&globbed_paths, 0, sizeof(glob_t));

			if (glob(*files, 0, glob_err, &globbed_paths) == 0) {
				if (globbed_paths.gl_pathc > 0) {
					found_files = true;
					break;
				}
			}
		} else { // No glob, just access.
			if (access(*files, F_OK) != - 1) { // file exists
				printf("FOUND: %s\n", *files);
				found_files = true;
				break;
			}
		}

		files += 1;
	}

	if (found_files) {
		print_found("Found VMware files.\n");
	} else {
		print_not_found("Found no VMware files.\n");
	}

	return found_files;
}

int main() {
	find_vmware_files();
	return 0;
}
