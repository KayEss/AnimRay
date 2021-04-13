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


#include <animray/color/rgb.hpp>
#include <animray/geometry/quadrics/sphere-unit-origin.hpp>
#include <animray/surface/matte.hpp>
#include <animray/test.hpp>
#include <felspar/test.hpp>


namespace {


    auto const suite = felspar::testsuite(__FILE__);


    auto const mrgb = suite.test("matte rgb", [](auto check) {
        animray::surface red_ball{
                animray::unit_sphere_at_origin<animray::ray<float>>{},
                animray::matte{animray::rgb<float>{1, 0, 0}}};

        // Following based on the light<point3d<W>> class
        decltype(red_ball)::intersection_type hit(
                animray::ray(
                        animray::point3d<float>(0, 0, 1),
                        animray::point3d<float>(0, 0, 2)),
                red_ball.surfaces);
        animray::ray illumination(
                animray::point3d<float>(0, 0, 1),
                animray::point3d<float>(0, 0, 5));
        animray::rgb<float> final(animray::shader(
                animray::ray<float>(), illumination, hit,
                animray::rgb<float>(255, 255, 255), red_ball));

        animray::check_close(check, final.red(), 255.f, 0.001f);
        check(final.green()) == 0;
        check(final.blue()) == 0;
    });


    auto const mg = suite.test("matte gray", [](auto check) {
        animray::surface gray_ball{
                animray::unit_sphere_at_origin<animray::ray<float>>{},
                animray::matte<float>{0.5f}};

        // Following based on the light<point3d<W>> class
        decltype(gray_ball)::intersection_type hit(
                animray::ray<float>(
                        animray::point3d<float>(0, 0, 1),
                        animray::point3d<float>(0, 0, 2)),
                gray_ball.surfaces);
        animray::ray<float> illumination(
                animray::point3d<float>(0, 0, 1),
                animray::point3d<float>(0, 0, 5));
        animray::rgb<float> final(animray::shader(
                animray::ray<float>(), illumination, hit,
                animray::rgb<float>(10, 10, 10), gray_ball));
        animray::check_close(check, final.red(), 5.f, 0.0001f);
        animray::check_close(check, final.green(), 5.f, 0.0001f);
        animray::check_close(check, final.blue(), 5.f, 0.0001f);
    });


}
