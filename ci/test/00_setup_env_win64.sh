#!/usr/bin/env bash
#
# Copyright (c) 2019-present The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.

export LC_ALL=C.UTF-8

export CONTAINER_NAME=ci_win64
export CI_IMAGE_NAME_TAG="docker.io/amd64/ubuntu:22.04"  # Check that Jammy can cross-compile to win64
export HOST=x86_64-w64-mingw32
export DPKG_ADD_ARCH="i386"
export PACKAGES="nsis g++-mingw-w64-x86-64-posix wine-binfmt wine64 wine32 file"
export RUN_FUNCTIONAL_TESTS=false
export GOAL="deploy"
# Prior to 11.0.0, the mingw-w64 headers were missing noreturn attributes, causing warnings when
# cross-compiling for Windows. https://sourceforge.net/p/mingw-w64/bugs/306/
# https://github.com/mingw-w64/mingw-w64/commit/1690994f515910a31b9fb7c7bd3a52d4ba987abe
export BITCOINII_CONFIG="--enable-reduce-exports --disable-gui-tests CXXFLAGS=-Wno-return-type"
