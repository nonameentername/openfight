#!/usr/bin/env python
import os
import sys


def normalize_path(val, env):
    return val if os.path.isabs(val) else os.path.join(env.Dir("#").abspath, val)


def validate_parent_dir(key, val, env):
    if not os.path.isdir(normalize_path(os.path.dirname(val), env)):
        raise UserError("'%s' is not a directory: %s" % (key, os.path.dirname(val)))


def set_build_info(source, target):
    with open(source, 'r') as f:
        content = f.read()

    content = content.replace("${version}", os.environ.get('TAG_VERSION', '0.0.0'))
    content = content.replace("${build}", os.environ.get('BUILD_SHA', 'none'))

    with open(target, 'w') as f:
        f.write(content)


build_info_source_files = ["templates/godotopenfight.gdextension.in",
                           "templates/version_generated.gen.h.in"]

build_info_target_files = ["addons/openfight/bin/godotopenfight.gdextension",
                           "src/version_generated.gen.h"]


def create_build_info_files(target, source, env):
    for i in range(len(build_info_source_files)):
        set_build_info(build_info_source_files[i], build_info_target_files[i])


def create_build_info_strfunction(target, source, env):
    return f"Creating build info files: {', '.join(str(t) for t in target)}"


libname = "libopenfightgodot"
projectdir = "."

if sys.platform == "windows":
    localEnv = Environment(tools=["mingw"], PLATFORM="")
else:
    localEnv = Environment(tools=["default"], PLATFORM="")

customs = ["custom.py"]
customs = [os.path.abspath(path) for path in customs]

opts = Variables(customs, ARGUMENTS)
opts.Add(
    BoolVariable(
        key="compiledb",
        help="Generate compilation DB (`compile_commands.json`) for external tools",
        default=localEnv.get("compiledb", False),
    )
)
opts.Add(
    PathVariable(
        key="compiledb_file",
        help="Path to a custom `compile_commands.json` file",
        default=localEnv.get("compiledb_file", "compile_commands.json"),
        validator=validate_parent_dir,
    )
)
opts.Add(
    BoolVariable(
        key="asan",
        help="Enable AddressSanitizer for builds",
        default=localEnv.get("asan", False),
    )
)
opts.Update(localEnv)

Help(opts.GenerateHelpText(localEnv))

env = localEnv.Clone()
env["compiledb"] = False

env.Tool("compilation_db")
compilation_db = env.CompilationDatabase(
    normalize_path(localEnv["compiledb_file"], localEnv)
)
env.Alias("compiledb", compilation_db)

env = SConscript("godot-cpp/SConstruct", {"env": env, "customs": customs})

if env["platform"] == "windows":
    env.Append(CPPFLAGS=["-DMINGW"])

    if env["dev_build"]:
        env.Append(LIBS=["yaml-cppd"])
        env.Append(LIBPATH=["addons/openfight/bin/windows/debug/vcpkg_installed/x64-mingw-static/debug/lib"])
        env.Append(CPPPATH=["addons/openfight/bin/windows/debug/vcpkg_installed/x64-mingw-static/include"])
    else:
        env.Append(LIBS=["yaml-cpp"])
        env.Append(LIBPATH=["addons/openfight/bin/windows/release/vcpkg_installed/x64-mingw-static/lib"])
        env.Append(CPPPATH=["addons/openfight/bin/windows/release/vcpkg_installed/x64-mingw-static/include"])
elif env["platform"] == "web":
    env.Append(LIBS=["openfight", "yaml-cpp"])

    if env["dev_build"]:
        env.Append(LIBPATH=["addons/openfight/bin/web/debug", "addons/openfight/bin/web/debug/vcpkg_installed/wasm32-emscripten/lib"])
        env.Append(CPPPATH=["addons/openfight/bin/web/debug/vcpkg_installed/wasm32-emscripten/include"])

        env.Append(CPPFLAGS=["-g"])
        env.Append(LINKFLAGS=["-g", "-s", "ERROR_ON_UNDEFINED_SYMBOLS=1"])
    else:
        env.Append(LIBPATH=["addons/openfight/bin/web/release", "addons/openfight/bin/web/release/vcpkg_installed/wasm32-emscripten/lib"])
        env.Append(CPPPATH=["addons/openfight/bin/web/release/vcpkg_installed/wasm32-emscripten/include"])
elif env["platform"] == "macos":
    env.Append(LINKFLAGS=["-framework", "CoreAudio",
                          "-framework", "AudioToolbox",
                          "-framework", "CoreFoundation",
                          "-framework", "Foundation"
                          ])
    env.Append(LIBS=["yaml-cpp"])

    if env["dev_build"]:
        env.Append(LIBPATH=["addons/openfight/bin/macos/debug/vcpkg_installed/universal-osxcross/lib"])
        env.Append(CPPPATH=["addons/openfight/bin/macos/debug/vcpkg_installed/universal-osxcross/include"])
    else:
        env.Append(LIBPATH=["addons/openfight/bin/macos/release/vcpkg_installed/universal-osxcross/lib"])
        env.Append(CPPPATH=["addons/openfight/bin/macos/release/vcpkg_installed/universal-osxcross/include"])
elif env["platform"] == "linux":
    env.Append(LIBS=["yaml-cpp"])

    if env["dev_build"]:
        env.Append(LIBPATH=["addons/openfight/bin/linux/debug/vcpkg_installed/x64-linux/lib"])
        env.Append(CPPPATH=["addons/openfight/bin/linux/debug/vcpkg_installed/x64-linux/include"])
        #env.Append(RPATH=["", "."])
    else:
        env.Append(LIBPATH=["addons/openfight/bin/linux/release/vcpkg_installed/x64-linux/lib"])
        env.Append(CPPPATH=["addons/openfight/bin/linux/release/vcpkg_installed/x64-linux/include"])
        #env.Append(RPATH=["", "."])

if env["platform"] != "web":
    env.Append(CPPFLAGS=["-fexceptions"])

env.Append(CPPPATH=["src/", "src/libopenfight", "src/platform/godot"])

if env["platform"] == "web":
    sources = Glob("src/platform/godot/*.cpp")
else:
    sources = Glob("src/libopenfight/*.cpp") + Glob("src/platform/godot/*.cpp")

if env.get("asan", False):
    print("SCons: Building with AddressSanitizer instrumentation")
    env.Append(CPPFLAGS=["-fsanitize=address", "-fno-omit-frame-pointer", "-g"])
    env.Append(LINKFLAGS=["-fsanitize=address"])

if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

file = "{}{}{}".format(libname, env["suffix"], env["SHLIBSUFFIX"])

if env["platform"] == "macos":
    platlibname = "{}.{}.{}".format(libname, env["platform"], env["target"])
    file = "{}.framework/{}".format(env["platform"], platlibname, platlibname)

libraryfile = "addons/openfight/bin/{}/{}".format(env["platform"], file)
library = env.SharedLibrary(
    libraryfile,
    source=sources,
)

create_build_info_action = Action(create_build_info_files,
                                  strfunction=create_build_info_strfunction)

env.Command(
    build_info_target_files,
    build_info_source_files,
    create_build_info_action
)

env.AddPreAction(library, create_build_info_action)
env.Depends(library, build_info_target_files)

#copy = env.InstallAs("{}/bin/{}/lib{}".format(projectdir, env["platform"], file), library)

default_args = [library]
if localEnv.get("compiledb", False):
    default_args += [compilation_db]
Default(*default_args)
