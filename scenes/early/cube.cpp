/**
    Copyright 2014-2021, [Kirit Saelensminde](https://kirit.com/AnimRay).

    This file is part of AnimRay.

    AnimRay is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    AnimRay is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with AnimRay.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <animray/animation/procedural/affine.hpp>
#include <animray/camera/flat-jitter.hpp>
#include <animray/camera/pinhole.hpp>
#include <animray/camera/movie.hpp>
#include <animray/cli/progress.hpp>
#include <animray/color/srgb.hpp>
#include <animray/intersection.hpp>
#include <animray/library/lights/block.hpp>
#include <animray/maths/angles.hpp>
#include <animray/movable.hpp>
#include <animray/geometry/collection.hpp>
#include <animray/geometry/planar/triangle.hpp>
#include <animray/scene.hpp>


int main(int argc, char const *const argv[]) {
    /// Output handling

    auto const args = animray::cli::arguments{argc, argv, "cube.tga", 96, 54};

    std::size_t const threads =
            args.switch_value('t', std::thread::hardware_concurrency());
    std::size_t const samples = args.switch_value('s', 2);
    std::size_t const frames = args.switch_value('l', 2);

    /// ## Set up the geometry
    using world = float;

    /// Screen aspect and pixel density
    world const aspect = world(args.width) / args.height;
    bool const high = (args.height > args.width);
    world const fw = high ? aspect * 0.036 : 0.036;
    world const fh = high ? 0.036 : 0.036 / aspect;

    /// Start with the apexes of the triangles for the shape.
    animray::point3d<world> constexpr tne(1, 1, 1), tse(1, -1, 1),
            tsw(-1, -1, 1), tnw(-1, 1, 1), bne(1, 1, -1), bse(1, -1, -1),
            bsw(-1, -1, -1), bnw(-1, 1, -1);
    /// Then put them together into the triangles we require
    using triangle = animray::triangle<animray::ray<world>>;
    auto const cube = animray::collection{animray::make_array(
            triangle{tne, tse, tsw}, triangle{tsw, tnw, tne},
            triangle{bne, bse, bsw}, triangle{bsw, bnw, bne},
            triangle{tne, bne, tse}, triangle{tse, bse, bne},
            triangle{tse, tsw, bsw}, triangle{bsw, bse, tse},
            triangle{tsw, bsw, tnw}, triangle{tnw, bnw, bsw},
            triangle{tnw, bnw, tne}, triangle{tne, bne, bnw})};

    auto const scene = animray::scene{
            cube, animray::library::lights::narrow_block<world>,
            animray::rgb<float>{5, 18, 25}};

    for (std::size_t frame{}; frame != frames; ++frame) {
        animray::movable<
                animray::stacatto_movie<animray::pinhole_camera<
                        animray::ray<world>, animray::flat_jitter_camera<world>>>,
                typename animray::with_frame<
                        animray::ray<world>, std::size_t>::type>
                camera(fw, fh, args.width, args.height, 0.05);
        camera(animray::rotate_x<world>(frame * 360_deg / frames));
        camera(animray::rotate_y<world>(frame * 720_deg / frames));
        camera(animray::translate<world>(0.0, 0.0, -6));
        camera.instance.frame = frame;

        using film_type = animray::film<animray::rgb<uint8_t>>;

        animray::cli_render_frame<film_type>(
                args, frame, threads,
                [samples, &scene, &camera](
                        const film_type::size_type x,
                        const film_type::size_type y) {
                    animray::rgb<float> photons;
                    for (std::size_t sample{}; sample != samples; ++sample) {
                        photons += scene(camera, x, y) /= samples;
                    }
                    auto const exposure = 1.4f;
                    return animray::to_srgb(photons, exposure * 255);
                });
    }

    return 0;
}
