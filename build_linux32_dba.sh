#!/bin/bash

./build.sh -prefix "$(pwd)/eneboo-build-linux32-dba/" -platform "linux-g++-32" -dbadmin && ./clean_build.sh "eneboo-build-linux32-dba"
