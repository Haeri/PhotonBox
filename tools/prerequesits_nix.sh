#!/usr/bin/env bash

oldpath=$(pwd)
cd $(dirname "$0")

python3 ./prerequesits.py

err=$?

if [ $err -ne 0 ] ; then
	exit $err
fi

cd $oldpath