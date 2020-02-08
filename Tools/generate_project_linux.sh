#!/usr/bin/env bash

oldpath=$(pwd)
cd $(dirname "$0")

./premake/linux/premake5 gmake2 --file="../premake5.lua"

err=$?

if [ $err -ne 0 ] ; then
	exit $err
fi

cd $oldpath