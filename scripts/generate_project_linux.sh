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

build_type=""

if [ "$1" = "-static" ]; then
	build_type="-DBUILD_SHARED_LIBS=OFF"
fi

cmake .. -DVCPKG_TARGET_TRIPLET=x64-linux -DVCPKG_OVERLAY_PORTS=$root_path"/external/custom-ports" -DCMAKE_TOOLCHAIN_FILE=$root_path"/external/vcpkg/scripts/buildsystems/vcpkg.cmake" $build_type
err=$?

if [ $err -ne 0 ]; then
	exit $err
fi

cd $old_path