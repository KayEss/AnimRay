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
    animray::compound<animray::movable<void, world>> scene;
    scene.insert(animray::movable<animray::sphere<world>>()(
        animray::translate(0.0, 0.0, 5.0)));
    scene.insert(animray::movable<animray::sphere<world>>()(
        animray::translate(-1.0, -1.0, 0.0)));
    scene.insert(animray::movable<animray::sphere<world>>()(
        animray::translate(1.0, -1.0, 0.0)));
    scene.insert(animray::movable<animray::sphere<world>>()(
        animray::translate(-1.0, 1.0, 0.0)));
    scene.insert(animray::movable<animray::sphere<world>>()(
        animray::translate(1.0, 1.0, 0.0)));
    typedef animray::ray<world> ray;
    animray::movable<animray::pinhole_camera<ray>> camera
        (fw, fh, width, height, 0.05);
    camera(animray::translate(0.0, 0.0, -8.5));
    typedef animray::film< animray::rgb< uint8_t > > film_type;
    film_type output(width, height,
        [&scene, &camera](const film_type::size_type x, const film_type::size_type y) {
            ray r(camera(x, y));
            fostlib::nullable<ray> intersection(scene.intersection(r));
            if ( !intersection.isnull() ) {
                ray light(intersection.value().from(), ray::end_type(5.0, 5.0, -5.0));
                if ( scene.occludes(light, 1e-9) ) {
                    return animray::rgb< uint8_t >(50);
                } else {
                    const double costheta = dot(light.direction(),
                        intersection.value().direction());
                    return animray::rgb< uint8_t >(50 + 205 * costheta);
                }
            } else {
                return animray::rgb< uint8_t >(0);
            }
        });
    animray::targa(output_filename, output);

    return 0;
}

