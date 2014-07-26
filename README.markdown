AnimRay was a 16 bit Windows 3.0 ray-tracer and animation program. This is a port of the concepts that it implemented to modern C++ code and modern platforms.

# Building AnimRay #

You'll need a recent Ubuntu, or at least a recent clang.

    git clone git@github.com:KayEss/AnimRay.git
    cd AnimRay
    git flow init -d
    ./build

You should now have a `dist/bin` folder which contains a simple renderer. You can generate an image using

    dist/bin/animray out.tga 600 400

You'll now have an image you can open in an image viewer.

# Concepts #

AnimRay is fully implemented as C++ templates in such a way as to make all of the key parts of a ray tracing algorithm pluggable (shortcomings in the implementation notwithstanding).

