#!/bin/bash
export CROSS='i686-apple-darwin8-'
export PATH=$PATH:/opt/mac/bin

./build.sh -platform "macx-g++-cross-64" -quick && ./clean_build.sh "eneboo-build-mac_i686-quick"
