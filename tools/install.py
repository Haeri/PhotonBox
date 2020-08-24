#!/usr/bin/python

import platform
import subprocess
import re
import os


fx = "\""
vcpkg_dir = "../extern/vcpkg/"
vcpkg_bootstraps = {
	"Windows": vcpkg_dir + "bootstrap-vcpkg.bat",
	"Darwin": vcpkg_dir + "bootstrap-vcpkg.sh",
	"Linux": vcpkg_dir + "bootstrap-vcpkg.sh",
}

project_generator = {
	"Windows": "./generate_project_win.bat",
	"Darwin": "./generate_project_mac.sh",
	"Linux": "./generate_project_linux.sh",
}

colors = {
	"white": "\x1B[0m",
	"red": "\x1B[31m",
	"green": "\x1B[32m",
	"blue": "\x1B[34m",
	"gray": "\x1B[90m"
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


def do_step(title, emoji, indent, cmd, regex, isFile = True, errfunc = None):
	print(" " * indent + emj(emoji) + " " + title + " " + clr(("." * (30 - len(title) - indent)), "gray") + " "+emj("🌀"), end='', flush=True)
	err, msg = subprocess.getstatusoutput(cmd)
	print(emj("\b\b \b"), end='')
	if err == 0:
		m = re.search(regex, msg)
		print(clr(emj("✔️ ") + "OK ", "green"), end="")
		if len(m.groups()) >= 1:
			print(m.group(1))
		else:
			print()
		return 0
	else:
		if isFile:
			print(clr(emj("❌ ") + "NOT FOUND!", "red"))
		else:
			print(clr(emj("❌ ") + "FAILED!", "red"))
			print(clr("--------------------", "gray") + " ERROR LOGS " + clr("--------------------", "gray"))
			print(clr(msg, "red"))
			print(clr("---------------------------------------------------", "gray"))
		if errfunc is not None:
			errfunc()
		else:
			exit(1)


#----------- Execution -----------

do_step("CMake check", 	"🛠️", 0, "cmake --version", 'cmake version (.*)')
do_step("Vcpkg setup", 	"🧰", 0, fx+vcpkg_bootstraps[platform.system()]+fx, '()', False, lambda :( 
	print(clr("Looks like you forgot to download the submodules. Let me fix that for you!","blue")), 
	do_step("Download submodules", 	"📦", 3, "git submodule update --init", '()', False),
	do_step("Vcpkg setup", 			"🧰", 3, fx+vcpkg_bootstraps[platform.system()]+fx, '()', False)
))
do_step("Install glfw3", 	"📚", 0, fx+vcpkg_dir + "vcpkg" + fx + " install glfw3:x64-" 	+ platforms[platform.system()], '()', False)
do_step("Install freetype", "📚", 0, fx+vcpkg_dir + "vcpkg" + fx + " install freetype:x64-" + platforms[platform.system()], '()', False)
do_step("Install imgui", 	"📚", 0, fx+vcpkg_dir + "vcpkg" + fx + " install imgui:x64-" 	+ platforms[platform.system()], '()', False)
do_step("Install PhysX", 	"📚", 0, fx+vcpkg_dir + "vcpkg" + fx + " install physx:x64-" 	+ platforms[platform.system()], '()', False)
do_step("Install zlib", 	"📚", 0, fx+vcpkg_dir + "vcpkg" + fx + " install zlib:x64-" 	+ platforms[platform.system()], '()', False)
do_step("Install stb", 		"📚", 0, fx+vcpkg_dir + "vcpkg" + fx + " install stb:x64-" 		+ platforms[platform.system()], '()', False)
do_step("Generate project", "🗂️", 0, fx+project_generator[platform.system()]+fx + " -s", '()', False)

print(colored(emj("✔️ ") + "Everything is ready!\n(The project was generated in the './build' directory)", "green"))	
exit(0)