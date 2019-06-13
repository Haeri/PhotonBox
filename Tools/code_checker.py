#!/usr/bin/python

from shutil import copyfile
import os

header_path = "../PhotonBox/include/PhotonBox/"
cpp_path 	= "../PhotonBox/src/"

all_header 	= []
all_cpp 	= []

header_loc 	= 0
cpp_loc		= 0

MEMORY_MOD_ERR 	= 0
SYNC_MOD_ERR 	= 0

MEMORY_CHECK = "#ifdef PB_MEM_DEBUG\n#include \"PhotonBox/util/MEMDebug.h\"\n#define new DEBUG_NEW\n#endif"

def pathwalk(src, filter, function):
	for item in os.listdir(src):
		if os.path.isdir(src + item):
			#print("DIR: " + src + item + "/")
			pathwalk(src + item + "/", filter, function)
		elif item.lower().endswith(filter):
			#print("FILE: " + src + item)
			function(src + item)


# ----------- GENERAL INFO -----------------
def indexHeader(item):
	global header_loc
	all_header.append(item)
	header_loc += sum(1 for i in open(item, 'rb'))

def indexCpp(item):
	global cpp_loc
	all_cpp.append(item)
	cpp_loc += sum(1 for i in open(item, 'rb'))

def indexAll():
	pathwalk(header_path, (".h"), indexHeader)
	pathwalk(cpp_path, (".cpp"), indexCpp)

def countFiles():
	print("Total lines of code: \t\t\t" + str(header_loc + cpp_loc))
	print("Total lines of code in Headers:\t\t" + str(header_loc))
	print("Total lines of code in CPP files:\t" + str(cpp_loc))



# ----------- MEMORY CHECK -----------------
def checkMemory(item):
	global MEMORY_MOD_ERR
	data = ""
	with open(item, 'r') as myfile:
		data = myfile.read()
	if data.find(MEMORY_CHECK)  == -1:
		MEMORY_MOD_ERR += 1
		print("MEMORY CHECH MISSING: " + item)

def memoryCheckModule():
	pathwalk(cpp_path, (".cpp"), checkMemory)
	if MEMORY_MOD_ERR == 0:
		print("Memory Debug Check\t - OK")
	else:
		print("Memory Debug Check ended with " + str(MEMORY_MOD_ERR) + " errors!")	
		

# ----------- CPP-H SYNC CHECK -----------------
def syncCheck(item):
	global SYNC_MOD_ERR
	basename = os.path.basename(item).replace(".h", "")
	cpp_file = item.replace(header_path, cpp_path)
	cpp_file = cpp_file.replace(".h", ".cpp")
	if not os.path.isfile(cpp_file) and item.find("/shader/") == -1:
		for f in all_cpp:
			if f.find(basename) != -1:
				SYNC_MOD_ERR += 1
				print("CPP MISPLACED: " + item + " at " + f)
	

def syncCheckModule():
	pathwalk(header_path, (".h"), syncCheck)
	if SYNC_MOD_ERR == 0:
		print("Synchronization Check\t - OK")
	else:
		print("Synchronization Check ended with " + str(SYNC_MOD_ERR) + " errors!")	


print("------------------------------- START -------------------------------")

indexAll()
countFiles()

print("")

memoryCheckModule()
syncCheckModule()

print("------------------------------- DONE -------------------------------")