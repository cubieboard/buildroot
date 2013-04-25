#!/bin/bash


MY_TARGET_DIR=$1

rm -rf ${MY_TARGET_DIR}/init
(cd ${MY_TARGET_DIR} && ln -s bin/busybox init)


