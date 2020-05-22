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


#include <animray/geometry/planar/plane.hpp>
#include <animray/ray.hpp>
#include <fost/log>
#include <fost/test>


namespace {
    const fostlib::module c_mod{__FILE__};
}


FSL_TEST_SUITE(plane);


FSL_TEST_FUNCTION(plane_constructor) {
    animray::plane<animray::ray<int>> board;
    FSL_CHECK_EQ(board.center, animray::point3d<int>(0, 0, 0));
    FSL_CHECK_EQ(
            board.normal,
            animray::unit_vector<int>(animray::point3d<int>(0, 0, 1)));
}


namespace {
    void check_intersection(
            animray::ray<int> ray, fostlib::nullable<animray::ray<int>> hit) {
        fostlib::log::debug(c_mod)("ray", ray)("hit", hit.value());
        animray::plane<animray::ray<int>> board;
        fostlib::nullable<animray::ray<int>> intersection(
                board.intersects(ray, 0));
        if (hit) {
            FSL_CHECK_EQ(intersection.value(), hit.value());
        } else {
            FSL_CHECK(not intersection);
        }
    }
}
FSL_TEST_FUNCTION(plane_intersection) {
    // From above
    check_intersection(
            animray::ray<int>(
                    animray::point3d<int>(0, 0, 1),
                    animray::point3d<int>(0, 0, -1)),
            animray::ray<int>(
                    animray::point3d<int>(0, 0, 0),
                    animray::point3d<int>(0, 0, 1)));
    check_intersection(
            animray::ray<int>(
                    animray::point3d<int>(1, 1, 1),
                    animray::point3d<int>(1, 1, -1)),
            animray::ray<int>(
                    animray::point3d<int>(1, 1, 0),
                    animray::point3d<int>(1, 1, 1)));
    check_intersection(
            animray::ray<int>(
                    animray::point3d<int>(0, 0, 5),
                    animray::point3d<int>(0, 0, 4)),
            animray::ray<int>(
                    animray::point3d<int>(0, 0, 0),
                    animray::point3d<int>(0, 0, 1)));
    // From below
    check_intersection(
            animray::ray<int>(
                    animray::point3d<int>(0, 0, -1),
                    animray::point3d<int>(0, 0, 1)),
            animray::ray<int>(
                    animray::point3d<int>(0, 0, 0),
                    animray::point3d<int>(0, 0, -1)));
}


namespace {
    void check_occludes(animray::ray<int> ray, bool occludes, int epsilon = 0) {
        animray::plane<animray::ray<int>> board;
        FSL_CHECK_EQ(board.occludes(ray, epsilon), occludes);
    }
}
FSL_TEST_FUNCTION(plane_occludes) {
    check_occludes(
            animray::ray<int>(
                    animray::point3d<int>(0, 0, 1),
                    animray::point3d<int>(1, 1, 1)),
            false); // parallel to the XY plane
    check_occludes(
            animray::ray<int>(
                    animray::point3d<int>(0, 0, 1),
                    animray::point3d<int>(0, 0, -1)),
            true); // towards the XY plane from above
    check_occludes(
            animray::ray<int>(
                    animray::point3d<int>(0, 0, -1),
                    animray::point3d<int>(0, 0, -2)),
            false); // away from the XY plane from below
    check_occludes(
            animray::ray<int>(
                    animray::point3d<int>(1, 1, 1),
                    animray::point3d<int>(1, 1, -1)),
            true); // towards the XY plane from above
    check_occludes(
            animray::ray<int>(
                    animray::point3d<int>(0, 0, 5),
                    animray::point3d<int>(0, 0, 4)),
            true); // towards the XY plane from further above
    check_occludes(
            animray::ray<int>(
                    animray::point3d<int>(1, 1, 1),
                    animray::point3d<int>(1, 1, -1)),
            false, 2); // towards the XY plane from above, but within epsilon
}
