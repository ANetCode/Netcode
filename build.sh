#!/bin/bash
ROOT_DIR=`pwd`
BUILD_OUTPUT="${ROOT_DIR}/build/"
RELEASE_DIR=${ROOT_DIR}/release/

echo ${BUILD_OUTPUT}

autoreconf --install
mkdir -p ${BUILD_OUTPUT} 
cd ${BUILD_OUTPUT} 
../configure
make && make test

# do release
mkdir -p ${RELEASE_DIR}/bin
ln -sf ${BUILD_OUTPUT}/netcode/netcode ${RELEASE_DIR}/bin/netcode
ln -sf ${ROOT_DIR}/scripts/ ${RELEASE_DIR}/scripts