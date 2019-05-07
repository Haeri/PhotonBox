#!/usr/bin/env bash

oldpath=$(pwd)
cd $(dirname "$0")

if [ -z "$1"]
then
	python3 deployer.py linux
else
	python3 deployer.py linux "$1"
fi

cd $oldpath
