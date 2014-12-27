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


#include <animray/value.hpp>
#include <animray/ray.hpp>
#include <animray/mixins/frame.hpp>
#include <animray/interpolation/linear.hpp>
#include <fost/test>


namespace {
    typedef animray::ray<int> base_ray;
    typedef animray::with_frame<base_ray>::type ray_type;
}


FSL_TEST_SUITE(animation_procedural);


FSL_TEST_FUNCTION(constant) {
    ray_type ray;
    FSL_CHECK_EQ(ray.frame(), 0);
    FSL_CHECK_EQ(animray::value(0, ray), 0);
}


FSL_TEST_FUNCTION(linear_frames_boost_function) {
    boost::function<int(const ray_type &)> f{[](const ray_type &r) {
        return animray::interpolation::linear(5.0, 15.0, r.frame(), std::size_t(10));
    }};
    ray_type ray;
    FSL_CHECK_EQ(animray::value(f, ray), 5);
    ray.frame(10);
    FSL_CHECK_EQ(ray.frame(), 10);
    FSL_CHECK_EQ(animray::value(f, ray), 15);
}


FSL_TEST_FUNCTION(linear_frames_std_function) {
    std::function<int(const ray_type &)> f{[](const ray_type &r) {
        return animray::interpolation::linear(-5.0, 5.0, r.frame(), std::size_t(10));
    }};
    ray_type ray;
    FSL_CHECK_EQ(animray::value(f, ray), -5);
    ray.frame(1);
    FSL_CHECK_EQ(animray::value(f, ray), -4);
}


namespace {
    int linear_frames_function(const ray_type &r) {
        return animray::interpolation::linear(-5.0, 5.0, r.frame(), std::size_t(10));
    }
}
FSL_TEST_FUNCTION(linear_frames_function) {
    ray_type ray;
    FSL_CHECK_EQ(animray::value(linear_frames_function, ray), -5);
    ray.frame(1);
    FSL_CHECK_EQ(animray::value(linear_frames_function, ray), -4);
}


FSL_TEST_FUNCTION(linear_frames_auto) {
    auto f = [](const ray_type &r) {
        return animray::interpolation::linear(-5.0, 5.0, r.frame(), std::size_t(10));
    };
    ray_type ray;
    FSL_CHECK_EQ(animray::value(f, ray), -5);
    ray.frame(5);
    FSL_CHECK_EQ(animray::value(f, ray), 0);
}

