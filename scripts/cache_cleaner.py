#!/usr/bin/python

import os

engine_res_path = "../res/"
game_res_path 	= "../examples/Sandbox/res/"

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