import inquirer
import platform
import subprocess as sp
from colorama import Fore, Style


def get_os_name():
	return platform.system().lower()


def run_cmd(command):
	"""
	Wrapper on subprocess.run to handle shell commands as either a list of args
	or a single string.
	"""
	if not isinstance(command, list):
		process = sp.run(command.split(), stdout=sp.PIPE, stderr=sp.DEVNULL)
	else:
		process = sp.run(command, stdout=sp.PIPE, stderr=sp.DEVNULL)
	return process


def choice(question, choices):
	"""
	Displays list of choices and returns the one that was selected.
	"""
	choice_prompt = [inquirer.List('choice',
                             message=Fore.GREEN + Style.BRIGHT + question + Fore.BLUE,
                             choices=choices)
	]
	return inquirer.prompt(choice_prompt).get('choice').strip()
