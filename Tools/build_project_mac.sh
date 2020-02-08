#!/usr/bin/env bash

oldpath=$(pwd)
cd $(dirname "$0")

if [ -z "$1" ] ; then
	make .. config=release
elif [ $1 == "Release" ] ; then
	make .. config=release
elif [ $1 == "Debug" ] ; then
	make ..
fi

cd $oldpath