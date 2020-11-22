/**
    Copyright 2014-2020, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#include <fost/main>
#include <fost/progress-cli>
#include <fost/unicode>
#include <animray/camera/flat-jitter.hpp>
#include <animray/camera/pinhole.hpp>
#include <animray/geometry/quadrics/sphere-unit-origin.hpp>
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
#include <animray/targa.hpp>
#include <animray/affine.hpp>
#include <animray/threading/sub-panel.hpp>
#include <animray/color/hsl.hpp>
#include <thread>


FSL_MAIN("animray", "AnimRay. Copyright 2010-2020 Kirit Saelensminde")
(fostlib::ostream &out, fostlib::arguments &args) {
    const std::size_t threads(
            fostlib::coerce<fostlib::nullable<int>>(args.commandSwitch("t"))
                    .value_or(std::thread::hardware_concurrency()));
    const std::size_t samples(
            fostlib::coerce<int>(args.commandSwitch("ss").value_or("6")));
    const std::size_t spheres(
            fostlib::coerce<int>(args.commandSwitch("sp").value_or("10")));

    const int width = fostlib::coerce<int>(args[1].value_or("96"));
    const int height = fostlib::coerce<int>(args[2].value_or("54"));
    auto const output_filename = fostlib::coerce<std::filesystem::path>(
            args[3].value_or("spheres.tga"));

    typedef double world;
    const world aspect = double(width) / height;
    const world fw = width > height ? aspect * 0.024 : 0.024;
    const world fh = width > height ? 0.024 : 0.024 / aspect;

    using gloss_sphere_type = animray::movable<animray::surface<
            animray::unit_sphere_at_origin<animray::ray<world>>,
            animray::gloss<world>, animray::matte<animray::rgb<float>>>>;
    using reflective_sphere_type = animray::movable<animray::surface<
            animray::unit_sphere_at_origin<animray::ray<world>>,
            animray::reflective<float>, animray::matte<animray::rgb<float>>>>;
    using metallic_sphere_type = animray::movable<animray::surface<
            animray::unit_sphere_at_origin<animray::ray<world>>,
            animray::reflective<animray::rgb<float>>>>;
    using scene_type = animray::scene<
            animray::compound<
                    reflective_sphere_type,
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

    const world scale(200.0);
    std::get<0>(scene.geometry.instances) = reflective_sphere_type(
            0.4f, animray::rgb<float>(0.3f))(animray::translate<world>(
            0.0, 0.0, scale + 1.0))(animray::scale<world>(scale, scale, scale));

    std::default_random_engine generator;
    std::uniform_int_distribution<int> surface(1, 2);
    std::uniform_real_distribution<world> hue(0, 360), x_position(-20, 20),
            y_position(-20, 20);
    for (std::size_t count{}; count != spheres; ++count) {
        animray::hsl<float> hsl_colour(hue(generator), 1.0f, 0.5f);
        auto colour(fostlib::coerce<animray::rgb<float>>(hsl_colour));
        auto location(animray::translate<world>(
                x_position(generator), y_position(generator), 0.0));
        switch (surface(generator)) {
        case 1:
            std::get<1>(scene.geometry.instances)
                    .insert(metallic_sphere_type(colour)(location));
            break;
        case 2:
        default:
            std::get<2>(scene.geometry.instances)
                    .insert(gloss_sphere_type(10.0f, colour)(location));
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

    animray::movable<
            animray::pinhole_camera<
                    animray::ray<world>, animray::flat_jitter_camera<world>>,
            animray::ray<world>>
            camera(fw, fh, width, height, 0.05);
    camera(animray::rotate_x<world>(-65_deg))(
            animray::translate<world>(0.0, -4.0, -40));

    using film_type = animray::film<animray::rgb<uint8_t>>;

    fostlib::meter tracking;
    std::promise<film_type> promise;
    auto result = promise.get_future();
    std::thread{[threads, samples, width, height, &scene, &camera,
                 promise = std::move(promise)]() mutable {
        promise.set_value(animray::threading::sub_panel<film_type>(
                threads, width, height,
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
                }));
    }}.detach();
    fostlib::cli::monitor(
            out, tracking, result,
            [](const fostlib::meter::reading &current) {
                fostlib::stringstream out;
                out << "] " << current.done() << "/"
                    << current.work().value_or(0);
                if (current.meta().size() && not current.meta()[0].isnull()) {
                    fostlib::json meta(current.meta()[0]);
                    out << " ("
                        << fostlib::json::unparse(meta["panels"]["x"], false)
                        << "x"
                        << fostlib::json::unparse(meta["panels"]["y"], false)
                        << " of size "
                        << fostlib::json::unparse(meta["size"]["x"], false)
                        << "x"
                        << fostlib::json::unparse(meta["size"]["y"], false)
                        << ")";
                }
                return out.str();
            },
            [](const fostlib::meter::reading &) { return "["; });
    animray::targa(output_filename, result.get());

    return 0;
}
