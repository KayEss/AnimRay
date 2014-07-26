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
#include <animray/illumination.hpp>
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
        animray::lights<std::vector<
            animray::light<animray::point3d<world>, animray::rgb<uint8_t>>>>,
        animray::light<void, uint8_t>,
        animray::beam<animray::ray<world>, animray::rgb<uint8_t>>>
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

    scene.light().push_back(
        animray::light<animray::point3d<world>, animray::rgb<uint8_t>>(
            animray::point3d<world>(-5.0, 5.0, -5.0),
            animray::rgb<uint8_t>(0x20, 0x80, 0x20)));
    scene.light().push_back(
        animray::light<animray::point3d<world>, animray::rgb<uint8_t>>(
            animray::point3d<world>(-5.0, -5.0, -5.0),
            animray::rgb<uint8_t>(0x80, 0x20, 0x20)));
    scene.light().push_back(
        animray::light<animray::point3d<world>, animray::rgb<uint8_t>>(
            animray::point3d<world>(5.0, -5.0, -5.0),
            animray::rgb<uint8_t>(0x20, 0x20, 0x80)));
    scene.ambient().color(50);

    animray::movable<animray::pinhole_camera<scene_type::beam_type::ray_type>>
        camera(fw, fh, width, height, 0.05);
    camera(animray::translate(0.0, 0.0, -8.5));
    typedef animray::film<animray::rgb<uint8_t>> film_type;
    film_type output(width, height,
        [&scene, &camera](const film_type::size_type x, const film_type::size_type y) {
            return scene(camera, x, y);
        });
    animray::targa(output_filename, output);

    return 0;
}

