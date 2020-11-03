#!/bin/bash

export DISPLAY=:99.0
set -v
set -e

pushd tests/quickfluxunittests
qmake quickfluxunittests.pro
make
./quickfluxunittests
popd

pushd examples/todo
qmake
make
popd
