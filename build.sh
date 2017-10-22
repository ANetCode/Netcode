#!/bin/bash

ROOT_DIR=`pwd`
BUILD_OUTPUT="${ROOT_DIR}/build/"
RELEASE_DIR="${ROOT_DIR}/release/"
echo ${BUILD_OUTPUT}

function do_configure() {
    autoreconf --install
    mkdir -p ${BUILD_OUTPUT}
    cd ${BUILD_OUTPUT}
    ../configure
    cd -
}

function do_build() {
    cd ${BUILD_OUTPUT}
    make && make test
}

function do_release() {
    # do release
    #mkdir -p ${RELEASE_DIR}/bin
    #ln -sf ${BUILD_OUTPUT}/netcode/netcode ${RELEASE_DIR}/bin/netcode
    # ln -sf ${ROOT_DIR}/scripts ${RELEASE_DIR}/scripts
    do_copy_ext
    cp -f ${BUILD_OUTPUT}/netcode/netcode ${RELEASE_DIR}/scripts/
}

function do_copy_ext() {
    out=${RELEASE_DIR}/scripts/ext/
    mkdir -p ${out}
    path=${BUILD_OUTPUT}/ext/
    cmd="find ${path} -name \"*.so\""
    rs=$(eval ${cmd})
    for p in $rs 
    do
        echo "${p}"
        cp -f ${p} ${out}
    done
}

op_code=$1
if [[ "$op_code" == "rebuild" ]]; then
    do_configure
    do_build
    do_release
elif [[ "$op_code" == "purge" ]]; then
    rm -rf ${BUILD_OUTPUT}
else
    do_build
    do_release
fi
