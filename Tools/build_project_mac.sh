#!/usr/bin/env bash

oldpath=$(pwd)
cd $(dirname "$0")

if [ -z "$1" ] ; then
	make -C .. config=release
elif [ $1 == "Release" ] ; then
	make -C .. config=release
elif [ $1 == "Debug" ] ; then
	make -C ..
fi

cd $oldpath