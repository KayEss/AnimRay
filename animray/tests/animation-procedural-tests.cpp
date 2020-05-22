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


#include <animray/animation/procedural/rotate.hpp>
#include <animray/functional/reduce.hpp>
#include <animray/interpolation/linear.hpp>
#include <animray/maths/angles.hpp>
#include <animray/mixins/frame.hpp>
#include <animray/ray.hpp>
#include <animray/test.hpp>


namespace {
    typedef animray::ray<int> base_ray;
    typedef animray::with_frame<base_ray, std::size_t>::type ray_type;
}


FSL_TEST_SUITE(animation_procedural);


FSL_TEST_FUNCTION(constant) {
    ray_type ray;
    FSL_CHECK_EQ(ray.frame, 0);
    FSL_CHECK_EQ(animray::reduce(0, ray), 0);
}


FSL_TEST_FUNCTION(linear_frames_boost_function) {
    const auto f{[](const ray_type &r) {
        return animray::interpolation::linear(
                5.0, 15.0, r.frame, std::size_t(10));
    }};
    ray_type ray;
    FSL_CHECK_EQ(animray::reduce(f, ray), 5);
    ray.frame = 10;
    FSL_CHECK_EQ(ray.frame, 10);
    FSL_CHECK_EQ(animray::reduce(f, ray), 15);
}


FSL_TEST_FUNCTION(linear_frames_std_function) {
    const auto f{[](const ray_type &r) {
        return animray::interpolation::linear(
                -5.0, 5.0, r.frame, std::size_t(10));
    }};
    ray_type ray;
    FSL_CHECK_EQ(animray::reduce(f, ray), -5);
    ray.frame = 1;
    FSL_CHECK_EQ(animray::reduce(f, ray), -4);
}


namespace {
    int linear_frames_function(const ray_type &r) {
        return animray::interpolation::linear(
                -5.0, 5.0, r.frame, std::size_t(10));
    }
}
FSL_TEST_FUNCTION(linear_frames_function) {
    ray_type ray;
    FSL_CHECK_EQ(animray::reduce(linear_frames_function, ray), -5);
    ray.frame = 1;
    FSL_CHECK_EQ(animray::reduce(linear_frames_function, ray), -4);
}


FSL_TEST_FUNCTION(linear_frames_auto) {
    const auto f = [](const ray_type &r) {
        return animray::interpolation::linear(
                -5.0, 5.0, r.frame, std::size_t(10));
    };
    ray_type ray;
    FSL_CHECK_EQ(animray::reduce(f, ray), -5);
    ray.frame = 5;
    FSL_CHECK_EQ(animray::reduce(f, ray), 0);
}


FSL_TEST_FUNCTION(rotate) {
    animray::animation::rotate_xy<animray::point3d<double>> rot(
            animray::point3d<double>(1, 1, 1), 2, 90_deg, 0_deg);
    check_close(rot(0), animray::point3d<double>(3, 1, 1));
    check_close(rot(1), animray::point3d<double>(1, 3, 1));
    check_close(rot(2), animray::point3d<double>(-1, 1, 1));
    check_close(rot(3), animray::point3d<double>(1, -1, 1));
    check_close(rot(4), animray::point3d<double>(3, 1, 1));
    rot.phase = 90_deg;
    check_close(rot(0), animray::point3d<double>(1, 3, 1));
    check_close(rot(1), animray::point3d<double>(-1, 1, 1));
    check_close(rot(2), animray::point3d<double>(1, -1, 1));
    check_close(rot(3), animray::point3d<double>(3, 1, 1));
}
