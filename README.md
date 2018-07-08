AnimRay was a 16 bit Windows 3.0 ray-tracer and animation program. This is a port of the concepts that it implemented to modern C++ code and modern platforms.

# Building AnimRay #

You'll need a recent Ubuntu, or at least a recent clang. You will probably need to make sure you have the following:

    sudo apt-get install clang freeglut3-dev libglew-dev

Then you can build a sample test scene:

    git clone git@github.com:KayEss/AnimRay.git
    cd AnimRay
    git flow init -d
    ./build

You should now have a `dist/bin` folder which contains a simple renderer. You can generate an image using

    dist/bin/animray out.tga 600 400

You'll now have an image you can open in an image viewer.


# Concepts #

AnimRay is fully implemented as C++ templates in such a way as to make all of the key parts of a ray tracing algorithm pluggable (shortcomings in the implementation notwithstanding).

The intention is to try to avoid all run-time polymorphism, replacing it with compile static types that are composable so that the scene can be described in code.

## Geometry ##

Geometry describes what the scene is comprised of in a very general way. Geometry needs to satisfy two method signatures, one for calculating the intersection of a ray and one used to check if the geometry blocks a light source. See the class `animray::sphere` for an example.

Geometry is described by a class that implements the required methods.

## Lights ##

Lights create illumination. They are responsible for working out how much light hits any part of the geometry.

## Surface physics ##

Surface physics handle the colour of objects and how this colour and the illumination from lights shows up.

