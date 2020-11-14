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
#include <fost/unicode>
#include <animray/library/lights/block.hpp>
#include <animray/camera/pinhole.hpp>
#include <animray/geometry/quadrics/sphere-unit-origin.hpp>
#include <animray/geometry/collection.hpp>
#include <animray/maths/angles.hpp>
#include <animray/movable.hpp>
#include <animray/intersection.hpp>
#include <animray/scene.hpp>
#include <animray/shader.hpp>
#include <animray/surface/matte.hpp>
#include <animray/surface/gloss.hpp>
#include <animray/targa.hpp>
#include <animray/affine.hpp>


FSL_MAIN("animray", "AnimRay. Copyright 2010-2020 Kirit Saelensminde")
(fostlib::ostream &, fostlib::arguments &args) {
    const int width = fostlib::coerce<int>(args[1].value_or("100"));
    const int height = fostlib::coerce<int>(args[2].value_or("150"));
    auto output_filename = fostlib::coerce<std::filesystem::path>(
            args[3].value_or("affine-transformations.tga"));

    typedef double world;
    const world aspect = double(width) / height;
    const world fw = width > height ? aspect * 0.024 : 0.024;
    const world fh = width > height ? 0.024 : 0.024 / aspect;

    using sphere_type = animray::movable<animray::surface<
            animray::unit_sphere_at_origin<animray::ray<world>>,
            animray::gloss<world>, animray::matte<animray::rgb<float>>>>;
    animray::collection<sphere_type> spheres;
    world const scale{200.0};
    spheres.insert(sphere_type(100.0f, animray::rgb<float>(1.0, 1.0, 1.0))(
            animray::translate<world>(0.0, 0.0, scale + 1.0))(
            animray::scale<world>(scale, scale, scale)));
    spheres.insert(sphere_type(200.0f, animray::rgb<float>(0, 1.0, 1.0))(
            animray::translate<world>(-1.0, -1.0, 0.0)));
    spheres.insert(sphere_type(10.0f, animray::rgb<float>(1.0, 0.25, 0.5))(
            animray::translate<world>(1.0, -1.0, 0.0)));
    spheres.insert(sphere_type(20.0f, animray::rgb<float>(0.25, 1.0, 0.5))(
            animray::translate<world>(-1.0, 1.0, 0.0)));
    spheres.insert(sphere_type(50.0f, animray::rgb<float>(0.25, 0.5, 1.0))(
            animray::translate<world>(1.0, 1.0, 0.0)));

    auto const scene = animray::scene{
            std::move(spheres), animray::library::lights::narrow_block<world>,
            animray::rgb<float>{20, 70, 100}};

    animray::movable<
            animray::pinhole_camera<animray::ray<world>>, animray::ray<world>>
            camera(fw, fh, width, height, 0.05);
    camera(animray::rotate_z<world>(25_deg))(animray::rotate_x<world>(-65_deg))(
            animray::translate<world>(0.0, 0.0, -8.5))(
            animray::rotate_x<world>(2_deg))(animray::rotate_y<world>(-1_deg))(
            animray::translate<world>(0.0, 0.0, -1.5));
    using film_type = animray::film<animray::rgb<uint8_t>>;
    film_type output(
            width, height,
            [&scene, &camera](
                    const film_type::size_type x, const film_type::size_type y) {
                animray::rgb<float> photons(scene(camera, x, y));
                const float exposure = 1.4f;
                photons /= exposure;
                return animray::rgb<uint8_t>(
                        uint8_t(photons.red() > 255 ? 255 : photons.red()),
                        uint8_t(photons.green() > 255 ? 255 : photons.green()),
                        uint8_t(photons.blue() > 255 ? 255 : photons.blue()));
            });
    animray::targa(output_filename, output);

    return 0;
}
