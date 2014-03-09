AnimRay was a 16 bit Windows 3.0 ray-tracer and animation program. This is a port of the concepts that it implemented to modern C++ code and modern platforms.

# Building AnimRay #

You'll need a recent Ubuntu, or at least a recent clang.

    git clone git@github.com:KayEss/AnimRay.git
    cd AnimRay
    git flow init -d
    ./build

You should now have a `dist/bin` folder which contains the viewer program, `ar-view-d` (the `-d` means it's the debug build).

    dist/bin/ar-view-d

Currently this just shows a small white window.
