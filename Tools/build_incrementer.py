#!/usr/bin/python

from shutil import copyfile
import os
import re

version_file = "../PhotonBox/include/PhotonBox/core/PhotonBoxVersion.h"
pattern = "#define PHOTON_BOX_VERSION_BUILD "

old_version = 0
new_version = 0

text = ""
replace_line = ""

with open(version_file, 'r') as f:
	for line in f:
		text += line
		if line.startswith(pattern):
			line = line.replace(pattern, "")
			line = line.strip()
			old_version = int(line)
			new_version = old_version + 1
			replace_line = pattern + str(new_version)


text = re.sub(pattern + "(\d*)", replace_line, text)

with open(version_file, "w") as f:
	f.write(text)

print("Bumping Build Version from: " + str(old_version) + " to " + str(new_version))