from shutil import copyfile, rmtree
import os, sys

engine_res_path = "../PhotonBox/res/"
game_res_path 	= "../Game/res/"

def delFiles(dest, filters):
	for item in os.listdir(dest):
		if os.path.isdir(dest + item):
			delFiles(dest + item + "/", filters)
		elif item.lower().endswith(filters):
			print("Delete " + dest + item)
			os.remove(dest + item)

print("------------------------------- START -------------------------------")

delFiles(engine_res_path, (".pbt", ".pbm"))
delFiles(game_res_path, (".pbt", ".pbm"))

print("------------------------------- DONE -------------------------------")