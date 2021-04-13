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
#include <animray/point3d.hpp>
#include <felspar/test.hpp>


namespace {


    auto const suite = felspar::testsuite(__FILE__);


    static_assert(animray::Regular<animray::point3d<int>>);
    static_assert(animray::Regular<animray::point3d<float>>);


    auto const ba = suite.test("basic arithmatic", [](auto check) {
        animray::point3d x(2, 0, 0, 2), y(0, 2, 0, 2), z(0, 0, 2, 2),
                xy(2, 2, 0, 2), xz(2, 0, 2, 2), yz(0, 2, 2, 2);
        check(x + y) == xy;
        check(x + z) == xz;
        check(y + z) == yz;
        check(xy * xz) == x;
        check(xy * yz) == y;
        check(xz * yz) == z;
        check(x + x) == x * 2;
        check(y + y) == y * 2;
        check(z + z) == z * 2;
    });


    auto const d = suite.test("dot", [](auto check) {
        animray::point3d h1(3, 4, 5), h2(-2, -2, -2);
        check(h1.dot()) == 50;
        check(h2.dot()) == 12;
        check((h1 - h2).dot()) == 110;
        check((h2 - h1).dot()) == 110;
        check((h1 + h2).dot()) == 14;
    });


}
