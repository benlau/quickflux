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
export Qt5_DIR=/opt/Qt/5.9.9/usr/lib/cmake/Qt5
ls /opt/Qt
ls $Qt5_DIR
cmake ../
make
popd
