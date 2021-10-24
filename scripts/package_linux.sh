#!/usr/bin/env bash

old_path=$(pwd)
cd $(dirname "$0")

cd ..

cp -R ./res ./pb_res
mkdir lib
cp ./build/libPhotonBox*.a ./lib/

err=$?


if [ $err -ne 0 ]; then
	exit $err
fi

cd $old_path