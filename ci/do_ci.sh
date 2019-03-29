#!/bin/bash

set -e

[ -z "${SRC_DIR}" ] && export SRC_DIR="`pwd`"
[ -z "${BUILD_DIR}" ] && export BUILD_DIR=/build
mkdir -p "${BUILD_DIR}"

BAZEL_OPTIONS="--jobs 1"
BAZEL_TEST_OPTIONS="$BAZEL_OPTIONS --test_output=errors"

if [[ "$1" == "bazel.asan" ]]; then
  bazel build -c dbg \
        $BAZEL_OPTIONS \
        --copt=-fsanitize=address \
        --linkopt=-fsanitize=address \
        //...
  bazel test -c dbg \
        $BAZEL_TEST_OPTIONS \
        --copt=-fsanitize=address \
        --linkopt=-fsanitize=address \
        //...
  exit 0
elif [[ "$1" == "bazel.tsan" ]]; then
  bazel build -c dbg \
        $BAZEL_OPTIONS \
        --copt=-fsanitize=thread \
        --linkopt=-fsanitize=thread \
        //...
  bazel test -c dbg \
        $BAZEL_TEST_OPTIONS \
        --copt=-fsanitize=thread \
        --linkopt=-fsanitize=thread \
        //...
  exit 0
elif [[ "$1" == "cmake" ]]; then
  cd ${BUILD_DIR}
  cmake -DCMAKE_CXX_COMPILER=/usr/local/bin/clang++ \
        -DCMAKE_CXX_FLAGS="-stdlib=libc++" \
        ${SRC_DIR}
  make
  make install
fi
