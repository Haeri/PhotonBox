#!/usr/bin/env bash

oldpath=$(pwd)
cd $(dirname "$0")

python3 code_checker.py

cd $oldpath