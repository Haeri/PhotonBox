#!/usr/bin/env bash

old_path=$(pwd)
cd $(dirname "$0")

cd ..
root_path=$(pwd)

if [ ! -d $root_path"/external/vcpkg/scripts/buildsystems/vcpkg.cmake" ]; then
	echo "INFO: You forgot to download the submodules. I'll fix that for you."
	git submodule update --init
fi

if [ ! -d "build/" ]; then
	echo "INFO: First time setup will take longer as the dependencies need to be downloaded and compiled."
else
	rm -rf build
fi

mkdir "build"
cd build

cmake .. -DVCPKG_TARGET_TRIPLET=x64-linux -DCMAKE_TOOLCHAIN_FILE=$root_path"/external/vcpkg/scripts/buildsystems/vcpkg.cmake"
err=$?

if [ $err -ne 0 ]; then
	exit $err
fi

cd $old_path