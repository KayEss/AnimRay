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
#include <animray/line.hpp>
#include <felspar/test.hpp>


namespace {


    auto const suite = felspar::testsuite(__FILE__);


    static_assert(animray::Regular<animray::line<int>>);
    static_assert(animray::Regular<animray::line<float>>);


    auto const c = suite.test("constructor simple", [](auto) {
        animray::line l1{animray::point3d{0, 0, 1}, animray::point3d{0, 0, 10}};
    });


    auto const ls = suite.test("length squared", [](auto check) {
        check(animray::line(animray::point3d<int>(), animray::point3d(1, 0, 0))
                      .length_squared())
                == 1;
        check(animray::line(animray::point3d<int>(), animray::point3d(0, 2, 0))
                      .length_squared())
                == 4;
        check(animray::line(animray::point3d<int>(), animray::point3d(3, 0, 4))
                      .length_squared())
                == 25;
    });


    auto const p = suite.test("distance along", [](auto check) {
        auto constexpr line = animray::line{
                animray::point3d{0.0, 0.0, 0.0},
                animray::point3d{2.0, 2.0, 2.0}};
        check(line.proportion_along(1)) == animray::point3d{2.0, 2.0, 2.0};
        check(line.proportion_along(0.5)) == animray::point3d{1.0, 1.0, 1.0};
        check(line.proportion_along(3)) == animray::point3d{6.0, 6.0, 6.0};
    });


}
