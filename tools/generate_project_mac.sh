#!/usr/bin/env bash

oldpath=$(pwd)
cd $(dirname "$0")

cd ..
rm -rf build
mkdir build
cd build

cmake .. -DVCPKG_TARGET_TRIPLET=x64-osx -DCMAKE_TOOLCHAIN_FILE="../extern/vcpkg/scripts/buildsystems/vcpkg.cmake"

err=$?

if [ $err -ne 0 ] ; then
	exit $err
fi

cd $oldpath