#!/usr/bin/env bash
#
# Copyright (c) 2019-present The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

export LC_ALL=C.UTF-8

export CONTAINER_NAME=ci_native_previous_releases
export CI_IMAGE_NAME_TAG="docker.io/debian:bullseye"
# Use minimum supported python3.9 and gcc-10, see doc/dependencies.md
export PACKAGES="gcc-10 g++-10 python3-zmq"
export DEP_OPTS="NO_UPNP=1 NO_NATPMP=1 DEBUG=1 CC=gcc-10 CXX=g++-10"
export TEST_RUNNER_EXTRA="--previous-releases --coverage --extended --exclude feature_dbcrash"  # Run extended tests so that coverage does not fail, but exclude the very slow dbcrash
export RUN_UNIT_TESTS_SEQUENTIAL="true"
export RUN_UNIT_TESTS="false"
export GOAL="install"
export DOWNLOAD_PREVIOUS_RELEASES="true"
export BITCOINII_CONFIG="--enable-zmq --with-libs=no --with-gui=qt5 --enable-reduce-exports --enable-debug \
CFLAGS=\"-g0 -O2 -funsigned-char\" CPPFLAGS='-DBOOST_MULTI_INDEX_ENABLE_SAFE_MODE' CXXFLAGS=\"-g0 -O2 -funsigned-char\""
