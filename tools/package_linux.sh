#!/usr/bin/env bash

oldpath=$(pwd)
cd $(dirname "$0")

if [ -z "$1" ]
then
	python3 package.py linux
else
	python3 package.py linux "$1"
fi

err=$?

if [ $err -ne 0 ] ; then
	exit $err
fi

cd $oldpath
