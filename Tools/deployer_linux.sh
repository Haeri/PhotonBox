#!/usr/bin/env bash

oldpath=$(pwd)
cd $(dirname "$0")

if [ -z "$1"]
then
	python deployer.py linux
else
	python deployer.py linux "$1"
fi

cd $oldpath