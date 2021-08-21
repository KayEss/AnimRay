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
#include <animray/camera/flat-jitter.hpp>
#include <animray/camera/pinhole.hpp>
#include <animray/cli/progress.hpp>
#include <animray/compound.hpp>
#include <animray/geometry/quadrics/sphere.hpp>
#include <animray/geometry/collection.hpp>
#include <animray/intersection.hpp>
#include <animray/light/ambient.hpp>
#include <animray/light/collection.hpp>
#include <animray/light/point.hpp>
#include <animray/maths/angles.hpp>
#include <animray/movable.hpp>
#include <animray/scene.hpp>
#include <animray/shader.hpp>
#include <animray/surface/matte.hpp>
#include <animray/surface/gloss.hpp>
#include <animray/surface/reflective.hpp>


int main(int argc, char const *const argv[]) {
    auto const args =
            animray::cli::arguments{argc, argv, "reflections.tga", 100, 150};

    std::size_t const threads =
            args.switch_value('t', std::thread::hardware_concurrency());
    std::size_t const samples = args.switch_value('s', 6);

    using world = double;
    world const aspect = double(args.width) / args.height;
    world const fw = args.width > args.height ? aspect * 0.024 : 0.024;
    world const fh = args.width > args.height ? 0.024 : 0.024 / aspect;

    using gloss_sphere_type = animray::surface<
            animray::sphere<world, animray::point3d<world>>,
            animray::gloss<world>, animray::matte<animray::rgb<float>>>;
    using reflective_sphere_type = animray::movable<animray::surface<
            animray::unit_sphere_at_origin<animray::ray<world>>,
            animray::reflective<float>, animray::matte<animray::rgb<float>>>>;
    using metallic_sphere_type = animray::movable<animray::surface<
            animray::unit_sphere_at_origin<animray::ray<world>>,
            animray::reflective<animray::rgb<float>>,
            animray::matte<animray::rgb<float>>>>;
    using scene_type = animray::scene<
            animray::compound<
                    reflective_sphere_type, metallic_sphere_type,
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

    const world scale(200.0);
    std::get<0>(scene.geometry.instances) =
            reflective_sphere_type{
                    animray::unit_sphere_at_origin<animray::ray<world>>{}, 0.4f,
                    animray::rgb<float>(0.5f)}(
                    animray::translate<world>(0.0, 0.0, scale + 1.0))(
                    animray::scale<world>(scale, scale, scale));
    std::get<1>(scene.geometry.instances) = metallic_sphere_type{
            animray::unit_sphere_at_origin<animray::ray<world>>{},
            animray::rgb<float>(0, 0.8f, 0.8f),
            animray::rgb<float>(
                    0, 0.9f, 0.9f)}(animray::translate<world>(-1.0, -1.0, 0.0));
    std::get<2>(scene.geometry.instances)
            .insert(gloss_sphere_type{
                    {{1.0, -1.0, 0.0}, 0.5},
                    10.0f,
                    animray::rgb<float>(1.0, 0.25, 0.5)});
    std::get<2>(scene.geometry.instances)
            .insert(gloss_sphere_type{
                    {{-2.0, 2.0, 0.0}, 2.0},
                    20.0f,
                    animray::rgb<float>(0.25, 1.0, 0.5)});
    std::get<2>(scene.geometry.instances)
            .insert(gloss_sphere_type{
                    {{1.0, 1.0, 0.0}, 1.0},
                    50.0f,
                    animray::rgb<float>(0.25, 0.5, 1.0)});

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

    animray::movable<
            animray::pinhole_camera<
                    animray::ray<world>, animray::flat_jitter_camera<world>>,
            animray::ray<world>>
            camera(fw, fh, args.width, args.height, 0.05);
    camera(animray::rotate_z<world>(25_deg))(animray::rotate_x<world>(-65_deg))(
            animray::translate<world>(0.0, 0.0, -8.5))(animray::rotate_x<world>(
            2_deg))(animray::
                            rotate_y<world>(-1_deg))(animray::translate<world>(
            0.0, 0.0, -1.5));

    using film_type = animray::film<animray::rgb<uint8_t>>;

    animray::cli_render<film_type>(
            args, threads,
            [samples, &scene, &camera](
                    const film_type::size_type x, const film_type::size_type y) {
                animray::rgb<float> photons;
                for (std::size_t sample{}; sample != samples; ++sample) {
                    photons += scene(camera, x, y) /= samples;
                }
                const float exposure = 1.4f;
                photons /= exposure;
                return animray::rgb<uint8_t>(
                        uint8_t(photons.red() > 255 ? 255 : photons.red()),
                        uint8_t(photons.green() > 255 ? 255 : photons.green()),
                        uint8_t(photons.blue() > 255 ? 255 : photons.blue()));
            });

    return 0;
}
