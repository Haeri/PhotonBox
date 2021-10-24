#!/usr/bin/env bash

old_path=$(pwd)
cd $(dirname "$0")

cd ..

mkdir -p pb_res
cp -R ./res ./pb_res
mkdir lib
cp ./build/Release/libPhotonBox*.a ./lib/

err=$?


if [ $err -ne 0 ]; then
	exit $err
fi

cd $old_path