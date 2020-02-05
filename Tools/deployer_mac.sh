#!/usr/bin/env bash

oldpath=$(pwd)
cd $(dirname "$0")

if [ -z "$1" ]
then
	python3 deployer.py mac
else
	python3 deployer.py mac "$1"
fi

cd $oldpath
