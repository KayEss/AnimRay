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


#include <animray/surface/matte.hpp>
#include <animray/sphere.hpp>
#include <animray/color/rgb.hpp>
#include <fost/test>


FSL_TEST_SUITE( surface );


FSL_TEST_FUNCTION(matte_rgb) {
    animray::surface<
            animray::sphere<animray::ray<float>>,
            animray::matte< animray::rgb<float> >
        > red_ball(animray::rgb<float>(1, 0, 0));

    // Following based on the light<point3d<W>> class
    decltype(red_ball)::intersection_type
        hit(animray::ray<float>(
                animray::point3d<float>(0, 0, 1),
                animray::point3d<float>(0, 0, 2)),
            red_ball.surface_parameters());
    animray::ray<float> illumination(
        animray::point3d<float>(0, 0, 1),
        animray::point3d<float>(0, 0, 5));
    animray::rgb<float> final(animray::shader(
        animray::ray<float>(),
        illumination, hit, animray::rgb<float>(255, 255, 255), red_ball));
    FSL_CHECK_ERROR(final.red(), 81.169f, 0.001f);
    FSL_CHECK_EQ(final.green(), 0);
    FSL_CHECK_EQ(final.blue(), 0);
}


FSL_TEST_FUNCTION(matte_gray) {
    FSL_CHECK_EQ(10 * 0.5f, 5); // Check the maths will work out
    animray::surface<
            animray::sphere<animray::ray<float>>,
            animray::matte< float >
        > gray_ball(0.5);

    // Following based on the light<point3d<W>> class
    decltype(gray_ball)::intersection_type
        hit(animray::ray<float>(
                animray::point3d<float>(0, 0, 1),
                animray::point3d<float>(0, 0, 2)),
            gray_ball.surface_parameters());
    animray::ray<float> illumination(
        animray::point3d<float>(0, 0, 1),
        animray::point3d<float>(0, 0, 5));
    animray::rgb<float> final(animray::shader(
        animray::ray<float>(),
        illumination, hit, animray::rgb<float>(10, 10, 10), gray_ball));
    FSL_CHECK_ERROR(final.red(), 1.59155f, 0.0001f);
    FSL_CHECK_ERROR(final.green(), 1.59155f, 0.0001f);
    FSL_CHECK_ERROR(final.blue(), 1.59155f, 0.0001f);
}

