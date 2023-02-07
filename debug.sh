#!/bin/bash

cd build
make
cd ..
gdb -q build/JSON
