#!/bin/bash

$(eval /osxcross/tools/osxcross_conf.sh)

dir=$(realpath .)

$dir/scripts/lipo-dir.py  \
    $dir/addons/openfight/bin/osxcross-arm64/debug \
    $dir/addons/openfight/bin/osxcross-x86_64/debug \
    $dir/addons/openfight/bin/osxcross/debug

prefix=$dir/addons/openfight/bin/macos/debug
prefix_x64=$dir/addons/openfight/bin/osxcross-x86_64/debug
prefix_arm64=$dir/addons/openfight/bin/osxcross-arm64/debug

$dir/scripts/lipo-dir.py $prefix_arm64 $prefix_x64 $prefix

export OSXCROSS_ROOT=$OSXCROSS_BASE_DIR

cd $dir
scons platform=macos target=template_debug dev_build=yes debug_symbols=yes osxcross_sdk=$OSXCROSS_TARGET

x86_64-apple-${OSXCROSS_TARGET}-dsymutil addons/openfight/bin/macos/macos.framework/libopenfightgodot.macos.template_debug

chmod -R a+rX addons/openfight/bin/macos/macos.framework/libopenfightgodot.macos.template_debug.dSYM
