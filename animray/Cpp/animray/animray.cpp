/*
    Copyright 2014, Kirit Saelensminde.
    http://www.kirit.com/AnimRay

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
#include <fost/unicode>
#include <animray/camera.hpp>
#include <animray/sphere.hpp>
#include <animray/compound.hpp>
#include <animray/movable.hpp>
#include <animray/scene.hpp>
#include <animray/light.hpp>
#include <animray/targa.hpp>
#include <animray/affine.hpp>


FSL_MAIN(
    "animray",
    "AnimRay. Copyright 2010-2014 Kirit Saelensminde"
)( fostlib::ostream &out, fostlib::arguments &args ) {
    boost::filesystem::wpath output_filename =
        fostlib::coerce< boost::filesystem::wpath >(args[1].value("out.tga"));
    const int width = fostlib::coerce< int >( args[2].value("100") );
    const int height = fostlib::coerce< int >( args[3].value("100") );
    const double aspect = double(width) / height;
    const double fw = width > height ? aspect * 0.024 : 0.024;
    const double fh = width > height ? 0.024 : 0.024 / aspect;

    typedef double world;
    typedef animray::scene<
        animray::compound<animray::movable<void, world>>,
        animray::light<
            std::tuple<
                animray::light<void, float>,
                animray::light<
                    std::vector<
                        animray::light<animray::point3d<world>, animray::rgb<float>>>,
                    animray::rgb<float>>
            >, animray::rgb<float>
        >,
        animray::rgb<float>>
            scene_type;
    scene_type scene;

    scene.geometry().insert(animray::movable<animray::sphere<world>>()(
        animray::translate(0.0, 0.0, 5.0)));
    scene.geometry().insert(animray::movable<animray::sphere<world>>()(
        animray::translate(-1.0, -1.0, 0.0)));
    scene.geometry().insert(animray::movable<animray::sphere<world>>()(
        animray::translate(1.0, -1.0, 0.0)));
    scene.geometry().insert(animray::movable<animray::sphere<world>>()(
        animray::translate(-1.0, 1.0, 0.0)));
    scene.geometry().insert(animray::movable<animray::sphere<world>>()(
        animray::translate(1.0, 1.0, 0.0)));

    std::get<0>(scene.light()).color(50);
    std::get<1>(scene.light()).push_back(
        animray::light<animray::point3d<world>, animray::rgb<float>>(
            animray::point3d<world>(-5.0, 5.0, -5.0),
            animray::rgb<float>(0x40, 0xa0, 0x40)));
    std::get<1>(scene.light()).push_back(
        animray::light<animray::point3d<world>, animray::rgb<float>>(
            animray::point3d<world>(-5.0, -5.0, -5.0),
            animray::rgb<float>(0xa0, 0x40, 0x40)));
    std::get<1>(scene.light()).push_back(
        animray::light<animray::point3d<world>, animray::rgb<float>>(
            animray::point3d<world>(5.0, -5.0, -5.0),
            animray::rgb<float>(0x40, 0x40, 0xa0)));

    animray::movable<animray::pinhole_camera<scene_type::ray_type>>
        camera(fw, fh, width, height, 0.05);
    camera(animray::translate(0.0, 0.0, -8.5));
    typedef animray::film<animray::rgb<uint8_t>> film_type;
    film_type output(width, height,
        [&scene, &camera](const film_type::size_type x, const film_type::size_type y) {
            animray::rgb<float> photons(scene(camera, x, y));
            const float exposure = 1.2f;
            return animray::rgb<uint8_t>(
                uint8_t(photons.red() / exposure > 255 ? 255 : photons.red() / exposure),
                uint8_t(photons.green() / exposure > 255 ? 255 : photons.green() / exposure),
                uint8_t(photons.blue() / exposure > 255 ? 255 : photons.blue() / exposure));
        });
    animray::targa(output_filename, output);

    return 0;
}

