/**
    Copyright 2010-2021, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#include <animray/maths/cross.hpp>
#include <felspar/test.hpp>


namespace {


    auto const suite = felspar::testsuite(__FILE__);


    auto const s = suite.test("simple", [](auto check) {
        check(animray::cross(
                animray::unit_vector<int>(0, 1, 0),
                animray::point3d<int>(1, 0, 0)))
                == animray::point3d<int>(0, 0, -1);
        check(animray::cross(
                animray::unit_vector<int>(1, 0, 0),
                animray::point3d<int>(0, 1, 0)))
                == animray::point3d<int>(0, 0, 1);
    });


    auto const i = suite.test("inverse", [](auto check) {
        animray::point3d<double> e1(5, 0, 0), e2(0, 3, 0);
        check(animray::cross(e1, e2)) == animray::point3d<double>(0, 0, 15);
        check(-animray::cross(e1, e2)) == animray::point3d<double>(0, 0, -15);
        check(animray::cross(e2, e1)) == animray::point3d<double>(0, 0, -15);
        check(-animray::cross(e2, e1)) == animray::point3d<double>(0, 0, 15);
    });


}
