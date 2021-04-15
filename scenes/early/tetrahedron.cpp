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
#include <animray/intersection.hpp>
#include <animray/library/lights/block.hpp>
#include <animray/maths/angles.hpp>
#include <animray/movable.hpp>
#include <animray/geometry/collection.hpp>
#include <animray/geometry/planar/triangle.hpp>
#include <animray/scene.hpp>


int main(int argc, char const *const argv[]) {
    /// Output handling
    auto const args =
            animray::cli::arguments{argc, argv, "tetrahedron.tga", 96, 54};

    std::size_t const threads =
            args.switch_value('t', std::thread::hardware_concurrency());
    std::size_t const samples = args.switch_value('s', 1);
    /// Convert `frames` to an actual frame count instead of scaling value
    std::size_t const frames = args.switch_value('T', 2);

    /// Screen aspect and pixel density
    auto const aspect = static_cast<double>(args.width) / args.height;
    auto const fw = args.width > args.height ? aspect * 0.024 : 0.024;
    auto const fh = args.width > args.height ? 0.024 : 0.024 / aspect;

    /// ## Set up the geometry
    using world = float;
    std::size_t const angle{20};

    /// Start with the apexes of the triangles for the shape.
    animray::point3d<world> constexpr top(0, 0, 1), bottom(0, 0, -1),
            north(0, 1, 0), south(0, -1, 0), east(1, 0, 0), west(-1, 0, 0);
    /// Then put them together into the triangles we require
    using triangle = animray::triangle<animray::ray<world>>;
    auto const tetrahedron = animray::animation::affine{
            animray::rotate_z<world>, 40_deg, 1_deg * angle, frames,
            animray::animation::affine{
                    animray::rotate_y<world>, 0, 2_deg * angle, frames,
                    animray::collection{animray::make_array(
                            triangle{top, north, east},
                            triangle{top, east, south},
                            triangle{top, south, west},
                            triangle{top, west, north},
                            triangle{bottom, north, east},
                            triangle{bottom, east, south},
                            triangle{bottom, south, west},
                            triangle{bottom, west, north})}}};

    auto const scene = animray::scene{
            tetrahedron, animray::library::lights::narrow_block<world>,
            animray::rgb<float>{20, 70, 100}};

    for (std::size_t frame{}; frame != frames * 360 / angle; ++frame) {
        animray::movable<
                animray::stacatto_movie<animray::pinhole_camera<
                        animray::ray<world>, animray::flat_jitter_camera<world>>>,
                typename animray::with_frame<
                        animray::ray<world>, std::size_t>::type>
                camera(fw, fh, args.width, args.height, 0.05);
        camera(animray::rotate_x<world>(-15_deg));
        camera(animray::translate<world>(0.0, 0.0, -4));
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
                    const float exposure = 1.4f;
                    photons /= exposure;
                    return animray::rgb<uint8_t>(
                            uint8_t(photons.red() > 255 ? 255 : photons.red()),
                            uint8_t(photons.green() > 255 ? 255
                                                          : photons.green()),
                            uint8_t(photons.blue() > 255 ? 255
                                                         : photons.blue()));
                });
    }

    return 0;
}
