#!/usr/bin/env bash

oldpath=$(pwd)
cd $(dirname "$0")

./premake/linux/premake5 gmake2 --file="../premake5.lua"

cd $oldpath