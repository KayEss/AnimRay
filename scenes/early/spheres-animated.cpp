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


#include <animray/affine.hpp>
#include <animray/animation/procedural/rotate.hpp>
#include <animray/camera/flat-jitter.hpp>
#include <animray/camera/pinhole.hpp>
#include <animray/camera/movie.hpp>
#include <animray/cli/progress.hpp>
#include <animray/color/hsl.hpp>
#include <animray/geometry/planar/plane.hpp>
#include <animray/geometry/quadrics/sphere-unit.hpp>
#include <animray/geometry/collection.hpp>
#include <animray/compound.hpp>
#include <animray/maths/angles.hpp>
#include <animray/movable.hpp>
#include <animray/intersection.hpp>
#include <animray/scene.hpp>
#include <animray/shader.hpp>
#include <animray/surface/matte.hpp>
#include <animray/surface/gloss.hpp>
#include <animray/surface/reflective.hpp>
#include <animray/light/ambient.hpp>
#include <animray/light/collection.hpp>
#include <animray/light/point.hpp>


int main(int argc, char const *const argv[]) {
    auto const args =
            animray::cli::arguments{argc, argv, "spheres-animated.tga", 36, 27};

    std::size_t const threads =
            args.switch_value('t', std::thread::hardware_concurrency());
    std::size_t const samples = args.switch_value('s', 6);
    std::size_t const spheres = args.switch_value('c', 20);
    std::size_t const frames = args.switch_value('l', 12);
    std::size_t const start_frame = args.switch_value('L', 0);

    using world = double;
    const world aspect = world(args.width) / args.height;
    const world fw = args.width > args.height ? aspect * 0.024 : 0.024;
    const world fh = args.width > args.height ? 0.024 : 0.024 / aspect;

    using reflective_plane_type = animray::surface<
            animray::plane<animray::ray<world>>, animray::reflective<float>,
            animray::matte<animray::rgb<float>>>;
    using metallic_sphere_type = animray::surface<
            animray::unit_sphere<animray::animate<
                    animray::animation::rotate_xy<animray::point3d<world>>>>,
            animray::reflective<animray::rgb<float>>>;
    using gloss_sphere_type = animray::surface<
            animray::unit_sphere<animray::animate<
                    animray::animation::rotate_xy<animray::point3d<world>>>>,
            animray::gloss<world>, animray::matte<animray::rgb<float>>>;
    using scene_type = animray::scene<
            animray::compound<
                    reflective_plane_type,
                    animray::collection<metallic_sphere_type>,
                    animray::collection<gloss_sphere_type>>,
            animray::light<
                    std::tuple<
                            animray::light<void, float>,
                            animray::light<
                                    std::vector<animray::light<
                                            animray::point3d<world>,
                                            animray::rgb<float>>>,
                                    animray::rgb<float>>>,
                    animray::rgb<float>>,
            animray::rgb<float>>;
    scene_type scene;
    scene.background = animray::rgb<float>(20, 70, 100);

    std::get<0>(scene.geometry.instances) = reflective_plane_type(
            {animray::point3d<world>(0, 0, 4), {}}, 0.4f,
            animray::rgb<float>(0.3f));

    const std::vector<int> factors{1, 2, 3, 4, 6, 12, -12, -6, -4, -3, -2, -1};
    std::default_random_engine generator;
    std::uniform_int_distribution<int> surface(0, 2);
    std::uniform_int_distribution<int> factor(0, factors.size() - 1);
    std::uniform_real_distribution<world> hue(0, 360), radius(2, 10),
            phase(0_deg, 360_deg), x_position(-10, 10), y_position(-20, 20);
    for (std::size_t count{}; count != spheres; ++count) {
        animray::hsl<float> hsl_colour(hue(generator), 1.0f, 0.5f);
        auto colour(animray::convert_to<animray::rgb<float>>(hsl_colour));
        animray::animate<animray::animation::rotate_xy<animray::point3d<world>>>
                location(
                        animray::point3d<world>(
                                x_position(generator), y_position(generator), 0),
                        radius(generator),
                        360_deg * factors[factor(generator)] / frames,
                        phase(generator));
        switch (surface(generator) % 2) {
        case 0:
            std::get<1>(scene.geometry.instances)
                    .insert(metallic_sphere_type{location, colour});
            break;
        case 1:
        default:
            std::get<2>(scene.geometry.instances)
                    .insert(gloss_sphere_type{location, 10.0f, colour});
        }
    }

    std::get<0>(scene.light).color = 50;
    std::get<1>(scene.light)
            .push_back(
                    animray::light<animray::point3d<world>, animray::rgb<float>>(
                            animray::point3d<world>(-5.0, 5.0, -5.0),
                            animray::rgb<float>(0x40, 0xa0, 0x40)));
    std::get<1>(scene.light)
            .push_back(
                    animray::light<animray::point3d<world>, animray::rgb<float>>(
                            animray::point3d<world>(-5.0, -5.0, -5.0),
                            animray::rgb<float>(0xa0, 0x40, 0x40)));
    std::get<1>(scene.light)
            .push_back(
                    animray::light<animray::point3d<world>, animray::rgb<float>>(
                            animray::point3d<world>(5.0, -5.0, -5.0),
                            animray::rgb<float>(0x40, 0x40, 0xa0)));

    for (std::size_t frame{start_frame}; frame != frames; ++frame) {
        animray::movable<
                animray::stacatto_movie<animray::pinhole_camera<
                        animray::ray<world>, animray::flat_jitter_camera<world>>>,
                typename animray::with_frame<
                        animray::ray<world>, std::size_t>::type>
                camera(fw, fh, args.width, args.height, 0.05);
        camera(animray::rotate_x<world>(-65_deg));
        camera(animray::translate<world>(0.0, -4.0, -40));
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
                    return animray::rgb<std::uint8_t>(
                            std::uint8_t(
                                    photons.red() > 255 ? 255 : photons.red()),
                            std::uint8_t(
                                    photons.green() > 255 ? 255
                                                          : photons.green()),
                            std::uint8_t(
                                    photons.blue() > 255 ? 255
                                                         : photons.blue()));
                });
    }

    return 0;
}
