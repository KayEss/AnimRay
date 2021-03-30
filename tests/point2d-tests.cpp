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


#include <animray/functional/traits.hpp>
#include <animray/point2d.hpp>
#include <felspar/test.hpp>


namespace {


    auto const suite = felspar::testsuite(__FILE__);


    static_assert(animray::Regular<animray::point2d<int>>);
    static_assert(animray::Regular<animray::point2d<float>>);


    auto const add = suite.test("add", [](auto check) {
        animray::point2d<int> p1, p2 = animray::point2d<int>(1, 2);
        check(p1 += p2) == animray::point2d<int>(1, 2);
        check(p1 += p2) == 2 * animray::point2d<int>(1, 2);
    });


    auto const mul = suite.test("mul", [](auto check) {
        animray::point2d<int> p1, p2 = animray::point2d<int>(1, 2);
        check(p1 * 4) == p1;
        check(p2 *= 3) == animray::point2d<int>(3, 6);
    });


}
