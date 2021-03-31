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


#include <animray/animation/procedural/rotate.hpp>
#include <animray/functional/reduce.hpp>
#include <animray/interpolation/linear.hpp>
#include <animray/maths/angles.hpp>
#include <animray/mixins/frame.hpp>
#include <animray/ray.hpp>
#include <animray/test.hpp>


namespace {


    auto const suite = felspar::testsuite(__FILE__);


    using base_ray = animray::ray<int>;
    using ray_type = animray::with_frame<base_ray, std::size_t>::type;


    auto const c = suite.test("constant", [](auto check) {
        ray_type ray;
        check(ray.frame) == 0u;
        check(animray::reduce(0, ray)) == 0;
    });


    auto const lf = suite.test("linear-frames", [](auto check) {
        const auto f{[](const ray_type &r) {
            return animray::interpolation::linear(
                    -5.0, 5.0, r.frame, std::size_t(10));
        }};
        ray_type ray;
        check(animray::reduce(f, ray)) == -5;
        ray.frame = 1;
        check(animray::reduce(f, ray)) == -4;
    });


    int linear_frames_function(const ray_type &r) {
        return animray::interpolation::linear(
                -5.0, 5.0, r.frame, std::size_t(10));
    }
    auto const lff = suite.test("linear_frames_function", [](auto check) {
        ray_type ray;
        check(animray::reduce(linear_frames_function, ray)) == -5;
        ray.frame = 1;
        check(animray::reduce(linear_frames_function, ray)) == -4;
    });


    auto const lfa = suite.test("linear_frames_auto", [](auto check) {
        const auto f = [](const ray_type &r) {
            return animray::interpolation::linear(
                    -5.0, 5.0, r.frame, std::size_t(10));
        };
        ray_type ray;
        check(animray::reduce(f, ray)) == -5;
        ray.frame = 5;
        check(animray::reduce(f, ray)) == 0;
    });


    auto const r = suite.test("rotate", [](auto check) {
        animray::animation::rotate_xy<animray::point3d<double>> rot{
                animray::point3d<double>(1, 1, 1), 2, 90_deg, 0_deg};
        animray::check_close(check, rot(0), animray::point3d<double>(3, 1, 1));
        animray::check_close(check, rot(1), animray::point3d<double>(1, 3, 1));
        animray::check_close(check, rot(2), animray::point3d<double>(-1, 1, 1));
        animray::check_close(check, rot(3), animray::point3d<double>(1, -1, 1));
        animray::check_close(check, rot(4), animray::point3d<double>(3, 1, 1));
        rot.phase = 90_deg;
        animray::check_close(check, rot(0), animray::point3d<double>(1, 3, 1));
        animray::check_close(check, rot(1), animray::point3d<double>(-1, 1, 1));
        animray::check_close(check, rot(2), animray::point3d<double>(1, -1, 1));
        animray::check_close(check, rot(3), animray::point3d<double>(3, 1, 1));
    });


}
