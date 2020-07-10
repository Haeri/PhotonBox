#!/usr/bin/env bash

oldpath=$(pwd)
cd $(dirname "$0")

cd ..
rm -rf build
mkdir build
cd build

cmake .. -DVCPKG_TARGET_TRIPLET=x64-linux -DCMAKE_TOOLCHAIN_FILE="$VCPKG_INSTALLATION_ROOT/scripts/buildsystems/vcpkg.cmake"

cd $oldpath