#!/usr/bin/env bash

oldpath=$(pwd)
cd $(dirname "$0")

chmod +x ./premake/mac/premake5
./premake/mac/premake5 xcode4 --file="../premake5.lua"

cd $oldpath