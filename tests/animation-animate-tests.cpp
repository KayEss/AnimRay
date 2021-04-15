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


#include <animray/animation/animate.hpp>
#include <felspar/test.hpp>


namespace {


    auto const suite = felspar::testsuite(__FILE__);


    auto const c = suite.test("constructors", [](auto) {
        animray::animatable<int> v1;
        animray::animatable<int> v2{0};
    });


    auto const d = suite.test("degenerate", [](auto check) {
        check(animray::animatable<int>{}(0)) == 0;
        check(animray::animatable<int>{2}(0)) == 2;
    });


    auto const s = suite.test(
            "scalar", [](auto) { animray::animatable<int> value{2}; });


}
