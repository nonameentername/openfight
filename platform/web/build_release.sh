#!/bin/bash
set -e

. /emsdk/emsdk_env.sh

dir=$(realpath .)

# configure godot-openfight

build_dir=$dir/addons/openfight/bin/web/release

mkdir -p $build_dir
cd $build_dir

cmake -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_VERBOSE_MAKEFILE=1 \
    -DCMAKE_SYSTEM_NAME=Emscripten \
    -DEMSDK=$EMSDK \
    -DEMSDK_NODE=$EMSDK_NODE \
    $dir

# build godot-openfight

make libopenfight

cd $dir
scons platform=web target=template_release
