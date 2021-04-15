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


#include <animray/extents2d.hpp>
#include <animray/functional/traits.hpp>
#include <felspar/test.hpp>


namespace {


    auto const suite = felspar::testsuite(__FILE__);


    static_assert(animray::Regular<animray::extents2d<char>>);
    static_assert(animray::Regular<animray::extents2d<int>>);
    static_assert(animray::Regular<animray::extents2d<double>>);


    auto const s = suite.test("sizes", [](auto check) {
        animray::extents2d<int> e1(-1, -1, 1, 1);
        check(e1.width()) == 3;
        check(e1.height()) == 3;
        check(e1.area()) == 9;

        animray::extents2d<double> e2(-1, -1, 1, 1);
        check(e2.width()) == 2;
        check(e2.height()) == 2;
        check(e2.area()) == 4;
    });


    auto const i = suite.test("intersection", [](auto check) {
        animray::extents2d<char> e1(0, 0, 10, 10), e2(20, 20, 30, 30),
                e3(5, 5, 15, 15), e4(5, 5, 10, 10);
        check(e1.intersection(e1).value()) == e1;
        check(e1.intersection(e2)).is_falsey();
        check(e1.intersection(e3)) == e4;
        check(e1.intersection(e3).value()) == e4;
    });


}
