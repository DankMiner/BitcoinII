#!/usr/bin/env bash
#
# Copyright (c) 2019-present The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

export LC_ALL=C.UTF-8

export SDK_URL=${SDK_URL:-https://bitcoinIIcore.org/depends-sources/sdks}

export CONTAINER_NAME=ci_macos_cross
export CI_IMAGE_NAME_TAG="docker.io/ubuntu:22.04"
export HOST=x86_64-apple-darwin
export PACKAGES="zip"
export XCODE_VERSION=15.0
export XCODE_BUILD_ID=15A240d
export RUN_UNIT_TESTS=false
export RUN_FUNCTIONAL_TESTS=false
export GOAL="deploy"
export BITCOINII_CONFIG="--with-gui --enable-reduce-exports"
