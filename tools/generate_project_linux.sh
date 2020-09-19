#!/usr/bin/env bash

oldpath=$(pwd)
cd $(dirname "$0")

cd ..
rm -rf build
mkdir build
cd build

cmake .. -DVCPKG_TARGET_TRIPLET=x64-linux

err=$?

if [ $err -ne 0 ] ; then
	exit $err
fi

cd $oldpath