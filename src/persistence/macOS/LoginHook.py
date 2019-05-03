# This approach is explained in Patrick Wardle's "Methods of Malware
# Persistence on macOS" paper on page 18.
#
# NOTE: Untested.


import os
import sys
sys.path.insert(0, "../..")
from utils.print import *
from utils.utils import choice


def main():
	action = choice("Establish persistence or remove persistence?", [" Establish", " Remove"])

	if action == "establish":
		print_blue("Establishing macOS persistence with LoginHook")
		command = "sudo defaults write com.apple.loginwindow LoginHook"
		path = input("Enter the path of a script you'd like to run on login.")
		if not os.path.isfile(path):
			print_red("ERROR: {} is not a file.".format(path))
			sys.exit(1)

		# TODO: Print stdout, stderr
		run_cmd("{} {}".format(command, path))
		print_green("Persistence established.")
	elif action == "remove":
		print_blue("Removing macOS persistence with LoginHook")
		command = "sudo defaults delete com.apple.loginwindow LoginHook"
		# TODO: Print stdout, stderr
		run_cmd("{} {}".format(command))
		print_green("Persistence removed.")


if __name__ == '__main__':
	main()
