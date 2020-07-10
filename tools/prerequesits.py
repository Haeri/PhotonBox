#!/usr/bin/python

import platform
import subprocess
import re
import os

colors = {
	"white": "\x1B[0m",
	"red": "\x1B[31m",
	"green": "\x1B[32m",
	"blue": "\x1B[34m"
}

platforms = {
	"Windows": "windows",
	"Darwin": "osx",
	"Linux": "linux"
}

if platform.system() == "Windows" and os.getenv('WT_SESSION') is None:
	useEmoji = False
	useColor = False
else:
	useEmoji = True
	useColor = True

def emj(emoji):
	if useEmoji:
		return emoji
	else:
		return	""

def colored(text, color):
	if useColor:
		return colors[color] + text + colors["white"]
	else:
		return text


def do_step(title, emoji, indent, cmd, regex, isFile = True):
	print(" " * indent + emj(emoji) + " " + title + " " + colored(("." * (30 - len(title) - indent)), "blue") + " ", end='', flush=True)
	err, msg = subprocess.getstatusoutput(cmd)
	if err == 0:
		m = re.search(regex, msg)
		print(colored(emj("✔️ ") + "OK ", "green") + m.group(1))
		return 0
	else:
		if isFile:
			print(colored(emj("❌ ") + "NOT FOUND!", "red"))
		else:
			print(colored(emj("❌ ") + "FAILED!", "red"))
			print(colored(msg, "red"))
		return err

def do_custon_step(title, emoji, indent, fn, errMsg):
	print(" " * indent + emj(emoji) + " " + title + " " + colored(("." * (30 - len(title) - indent)), "blue") + " ", end='', flush=True)
	if fn():
		print(colored(emj("✔️ ") + "OK", "green"))
		return 0
	else:
		print(colored(emj("❌ ") + "FAILED!", "red"))
		print(colored(errMsg, "red"))
		return 1

err = 0
tab = "    "

if do_step("CMake", "🤖", 0, "cmake --version", 'cmake version (.*)') != 0:
	err = err + 1

if do_step("Vcpkg", "📦", 0, "vcpkg version", 'Vcpkg package management program version (.*)') != 0:
	err = err + 1
else:
	if do_step("Install glfw3", "📚", 4, "vcpkg install glfw3:x64-" + platforms[platform.system()], '()', False) != 0:
		err = err + 1
	if do_step("Install freetype", "📚", 4, "vcpkg install freetype:x64-" + platforms[platform.system()], '()', False) != 0:
		err = err + 1
	if do_step("Install imgui", "📚", 4, "vcpkg install imgui:x64-" + platforms[platform.system()], '()', False) != 0:
		err = err + 1
	if do_step("Install PhysX", "📚", 4, "vcpkg install physx:x64-" + platforms[platform.system()], '()', False) != 0:
		err = err + 1
	if do_step("Install STB", "📚", 4, "vcpkg install stb:x64-" + platforms[platform.system()], '()', False) != 0:
		err = err + 1
	if do_step("Install zlib", "📚", 4, "vcpkg install zlib:x64-" + platforms[platform.system()], '()', False) != 0:
		err = err + 1

	if do_custon_step("Vcpkg environment", "🔖", 4, lambda : os.getenv("VCPKG_INSTALLATION_ROOT"), "VCPKG_INSTALLATION_ROOT environment variable not set!") != 0:
			err = err + 1


if(err == 0):
	print(colored(emj("✔️ ") + "Everything ready for project steup!", "green"))	
else:
	print(colored(emj("❌ ") + "You need to resolve the issues!", "red"))	
exit(err)