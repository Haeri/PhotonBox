from shutil import copyfile, rmtree
import os, sys

mode = "Debug"


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
binaries_path 	= "../Game/bin/x86_64/" + mode + "/Game.exe"

def copyFiles(src, dest, filters):
	if not os.path.exists(dest):
		os.makedirs(dest);
	for item in os.listdir(src):
		if os.path.isdir(src + item):
			copyFiles(src + item + "/", dest + item + "/", filters)
		elif item.lower().endswith(filters):
			print("Copy " + src + item + "\n  to " + dest + item)
			copyfile(src + item, dest + item)

print("------------------------------- START -------------------------------")

# Delete output directory if already exists
if os.path.exists(out_path) and os.path.isdir(out_path):
	rmtree(out_path)

# Create output directory
os.makedirs(out_path);

# Copy all engine resource files
copyFiles(engine_res_path, out_path + "PhotonBox/res/", ('.pbt', '.pbm', '.ttf', '.vs', '.fs'))
copyFiles(game_res_path, out_path + "Game/res/", ('.pbt', '.pbm', '.ttf', '.vs', '.fs'))
copyFiles(physx_dll_path, out_path + "Game/", ('.dll'))
copyfile(binaries_path, out_path + "Game/Game.exe")

print("------------------------------- DONE -------------------------------")