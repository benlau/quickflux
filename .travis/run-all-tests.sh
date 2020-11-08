#!/bin/bash

export DISPLAY=:99.0
set -v
set -e
export QT_QPA_PLATFORM=minimal
pushd tests/quickfluxunittests
qmake quickfluxunittests.pro
make
./quickfluxunittests
popd

pushd examples/todo
qmake
make
make clean
mkdir build
cd build
cmake ../
make
popd
