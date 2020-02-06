#!/usr/bin/python
# -*- coding: utf-8 -*-

from shutil import copyfile, rmtree
import os, sys


mode = "NO_SET"
os_mode = "NO_SET"

dynamic_Lib_ext = ""
executable_ext = ""


if(len(sys.argv) > 1):
	os_mode = sys.argv[1]
	if os_mode == "win":
		dynamic_Lib_ext = ".dll"
		executable_ext = ".exe"
	elif os_mode == "linux":
		dynamic_Lib_ext = ".so"
		executable_ext = ""
	elif os_mode == "mac":
		dynamic_Lib_ext = ".so"
		executable_ext = ""

if(len(sys.argv) > 2):
	mode = sys.argv[2]

if mode == "NO_SET":
	while True:
		imp = input("DEBUG or RELEASE (d, r)\n")
		imp = imp.strip()
		if imp == "d":
			mode = "Debug"
			break
		elif imp == "r":
			mode = "Release"
			break
		else:
			print("Command not recognized")


out_path 		= "../Build/" + mode + "/"
engine_res_path = "../PhotonBox/res/"
game_res_path 	= "../Game/res/"
physx_dll_path	= "../PhotonBox/vendor/PhysX/bin/" + mode.lower() + "/"
binaries_path 	= "../Game/bin/x86_64/" + mode + "/Game" + executable_ext

def copyFiles(src, dest, filters):
	print(src)
	if not os.path.exists(dest):
		os.makedirs(dest)
	for item in os.listdir(src):
		if os.path.isdir(src + item):
			copyFiles(src + item + "/", dest + item + "/", filters)
		elif item.lower().endswith(filters):
			print("  <-[" + src + item + "]\n  ->[" + dest + item + "]\n")
			copyfile(src + item, dest + item)

print("------------------------------- START -------------------------------")

# Delete output directory if already exists
if os.path.exists(out_path) and os.path.isdir(out_path):
	rmtree(out_path)

# Create output directory
os.makedirs(out_path)

# Copy all engine resource files
# copyFiles(engine_res_path, out_path + "PhotonBox/res/", ('.pbt', '.pbm', '.ttf', '.vs', '.fs'))
# copyFiles(game_res_path, out_path + "Game/res/", ('.pbt', '.pbm', '.ttf', '.vs', '.fs'))

copyFiles(engine_res_path, out_path + "PhotonBox/res/", ('.png', '.jpg', '.jpeg', '.obj', '.ttf', '.vs', '.fs'))
copyFiles(game_res_path, out_path + "Game/res/", ('.png', '.jpg', '.jpeg', '.obj', '.ttf', '.vs', '.fs'))

copyFiles(physx_dll_path, out_path + "Game/", (dynamic_Lib_ext))
copyfile(binaries_path, out_path + "Game/Game" + executable_ext)
#copyfile(engine_res_path + "logo.png", out_path + "PhotonBox/res/logo.png")

# Temporary
copyFiles(game_res_path + "enviroment/", out_path + "Game/res/enviroment/", ('.jpg', '.jpeg', '.png'))

print("------------------------------- DONE -------------------------------")