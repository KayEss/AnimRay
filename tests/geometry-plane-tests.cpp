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


#include <animray/geometry/planar/plane.hpp>
#include <animray/functional/traits.hpp>
#include <animray/ray.hpp>
#include <felspar/test.hpp>


namespace {


    auto const suite = felspar::testsuite(__FILE__);


    static_assert(animray::Regular<animray::plane<animray::ray<int>>>);


    auto const pc = suite.test("plane_constructor", [](auto check) {
        animray::plane<animray::ray<int>> board;
        check(board.center) == animray::point3d(0, 0, 0);
        check(board.normal) == animray::unit_vector(animray::point3d(0, 0, 1));
    });

    template<typename C>
    void check_intersection(
            C check, animray::ray<int> ray, animray::ray<int> hit) {
        animray::plane<animray::ray<int>> board;
        std::optional<animray::ray<int>> intersection(board.intersects(ray, 0));
        check(intersection.value()) == hit;
    }
    auto const pi = suite.test("plane_intersection", [](auto check) {
        // From above
        check_intersection(
                check, {animray::point3d(0, 0, 1), animray::point3d(0, 0, -1)},
                {animray::point3d(0, 0, 0), animray::point3d(0, 0, 1)});
        check_intersection(
                check, {animray::point3d(1, 1, 1), animray::point3d(1, 1, -1)},
                {animray::point3d(1, 1, 0), animray::point3d(1, 1, 1)});
        check_intersection(
                check, {animray::point3d(0, 0, 5), animray::point3d(0, 0, 4)},
                {animray::point3d(0, 0, 0), animray::point3d(0, 0, 1)});
        // From below
        check_intersection(
                check, {animray::point3d(0, 0, -1), animray::point3d(0, 0, 1)},
                {animray::point3d(0, 0, 0), animray::point3d(0, 0, -1)});
    });


    template<typename C>
    void check_occludes(
            C check, animray::ray<int> ray, bool occludes, int epsilon = 0) {
        animray::plane<animray::ray<int>> board;
        check(board.occludes(ray, epsilon)) == occludes;
    }
    auto const po = suite.test("plane_occludes", [](auto check) {
        check_occludes(
                check, {animray::point3d(0, 0, 1), animray::point3d(1, 1, 1)},
                false); // parallel to the XY plane
        check_occludes(
                check, {animray::point3d(0, 0, 1), animray::point3d(0, 0, -1)},
                true); // towards the XY plane from above
        check_occludes(
                check, {animray::point3d(0, 0, -1), animray::point3d(0, 0, -2)},
                false); // away from the XY plane from below
        check_occludes(
                check, {animray::point3d(1, 1, 1), animray::point3d(1, 1, -1)},
                true); // towards the XY plane from above
        check_occludes(
                check, {animray::point3d(0, 0, 5), animray::point3d(0, 0, 4)},
                true); // towards the XY plane from further above
        check_occludes(
                check, {animray::point3d(1, 1, 1), animray::point3d(1, 1, -1)},
                false,
                2); // towards the XY plane from above, but within epsilon
    });


}
