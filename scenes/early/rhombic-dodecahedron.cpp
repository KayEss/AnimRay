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
#include <animray/line.hpp>
#include <animray/maths/angles.hpp>
#include <animray/movable.hpp>
#include <animray/geometry/collection.hpp>
#include <animray/geometry/planar/triangle.hpp>
#include <animray/scene.hpp>
#include <animray/surface.hpp>
#include <animray/surface/gloss.hpp>
#include <animray/surface/reflective.hpp>
#include <animray/surface/transparent.hpp>


int main(int argc, char const *const argv[]) {
    /// Output handling

    auto const args = animray::cli::arguments{
            argc, argv, "rhombic-dodecahedron.tga", 96, 54};

    std::size_t const threads =
            args.switch_value('t', std::thread::hardware_concurrency());
    std::size_t const samples = args.switch_value('s', 2);
    std::size_t const frames = args.switch_value('l', 2);
    std::size_t const start_frame = args.switch_value('L', 0);
    std::size_t const depth = args.switch_value('d', 5);
    std::size_t const gloss = args.switch_value('g', 1000);
    float const exposure = args.switch_value('e', 1.4f);

    /// ## Set up the geometry
    using world = float;

    /// Screen aspect and pixel density
    world const aspect = world(args.width) / args.height;
    bool const high = (args.height > args.width);
    world const fw = high ? aspect * 0.036 : 0.036;
    world const fh = high ? 0.036 : 0.036 / aspect;

    /// Start with the apexes of the triangles for the shape.
    animray::point3d<world> constexpr top(0, 0, 2), bottom(0, 0, -2),
            east(2, 0, 0), west(-2, 0, 0), north(0, 2, 0), south(0, -2, 0),
            tne(1, 1, 1), tse(1, -1, 1), tsw(-1, -1, 1), tnw(-1, 1, 1),
            bne(1, 1, -1), bse(1, -1, -1), bsw(-1, -1, -1), bnw(-1, 1, -1);
    /// Then put them together into the triangles we require
    using triangle = animray::triangle<animray::ray<world>>;
    auto constexpr geometry = animray::collection{animray::make_array(
            triangle{tne, tse, top}, triangle{tsw, tnw, top},
            triangle{tnw, tne, top}, triangle{tse, tsw, top},
            triangle{bne, bse, bottom}, triangle{bsw, bnw, bottom},
            triangle{bnw, bne, bottom}, triangle{bse, bsw, bottom},
            triangle{tne, bne, east}, triangle{tse, bse, east},
            triangle{tne, tse, east}, triangle{bne, bse, east},
            triangle{tse, tsw, south}, triangle{bsw, bse, south},
            triangle{tse, bse, south}, triangle{bsw, tsw, south},
            triangle{tsw, bsw, west}, triangle{tnw, bnw, west},
            triangle{tsw, tnw, west}, triangle{bsw, bnw, west},
            triangle{tnw, bnw, north}, triangle{tne, bne, north},
            triangle{tnw, tne, north}, triangle{bne, bnw, north})};
    auto constexpr edges = animray::make_array(
            animray::line{top, tne}, animray::line{top, tse},
            animray::line{top, tnw}, animray::line{top, tsw},
            animray::line{north, tne}, animray::line{north, tnw},
            animray::line{north, bne}, animray::line{north, bnw},
            animray::line{south, tse}, animray::line{south, tsw},
            animray::line{south, bse}, animray::line{south, bsw},
            animray::line{east, tne}, animray::line{east, tse},
            animray::line{east, bne}, animray::line{east, bse},
            animray::line{west, tnw}, animray::line{west, tsw},
            animray::line{west, bnw}, animray::line{west, bsw},
            animray::line{bottom, bne}, animray::line{bottom, bse},
            animray::line{bottom, bnw}, animray::line{bottom, bsw});

    animray::rgb<float> constexpr red{0xa0, 0x40, 0x40},
            green{0x40, 0xa0, 0x40}, blue{0x40, 0x40, 0xa0};
    animray::light<
            std::vector<animray::library::lights::bulb<world>>,
            animray::rgb<float>>
            strips;
    for (auto const edge : edges) {
        strips.push_back({edge.proportion_along(0.25) * 0.99, green});
        strips.push_back({edge.proportion_along(0.5) * 0.99, red});
        strips.push_back({edge.proportion_along(0.75) * 0.99, blue});
    }
    auto lights = animray::light{
            std::move(strips), animray::light{animray::luma{0.f}}};

    auto const scene = animray::scene{
            animray::surface{
                    geometry, animray::transparent{world(0.5), depth},
                    animray::reflective{world(0.5), depth},
                    animray::gloss{gloss}},
            lights, animray::rgb<float>{0, 0, 0}};

    for (auto frame{start_frame}; frame != frames; ++frame) {
        animray::movable<
                animray::stacatto_movie<animray::pinhole_camera<
                        animray::ray<world>, animray::flat_jitter_camera<world>>>,
                typename animray::with_frame<
                        animray::ray<world>, std::size_t>::type>
                camera(fw, fh, args.width, args.height, 0.05);
        camera(animray::rotate_x<world>(frame * 360_deg / frames));
        camera(animray::rotate_y<world>(frame * 720_deg / frames));
        auto const orbit_position = std::acos(-1) * 2 * frame / frames;
        camera(animray::translate<world>(
                0.0, 0.0, -2.2 - (std::cos(orbit_position) + 1) * 3.9));
        camera.instance.frame = frame;

        using film_type = animray::film<animray::rgb<uint8_t>>;

        animray::cli_render_frame<film_type>(
                args, frame, threads,
                [samples, &scene, &camera, exposure](
                        const film_type::size_type x,
                        const film_type::size_type y) {
                    animray::rgb<float> photons;
                    for (std::size_t sample{}; sample != samples; ++sample) {
                        photons += scene(camera, x, y) /= samples;
                    }
                    return animray::to_srgb(photons, exposure * 255);
                });
    }

    return 0;
}
