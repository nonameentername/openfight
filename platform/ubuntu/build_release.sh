#!/bin/bash

echo TAG_VERSION=$TAG_VERSION
echo BUILD_SHA=$BUILD_SHA

dir=$(realpath .)

# configure godot-openfight

build_dir=$dir/addons/openfight/bin/linux/release

mkdir -p $build_dir
cd $build_dir

cmake -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_VERBOSE_MAKEFILE=1 \
    $dir

# build godot-openfight

make

# build godot-openfight (gdextension)

cd $dir
scons platform=linux target=template_release
