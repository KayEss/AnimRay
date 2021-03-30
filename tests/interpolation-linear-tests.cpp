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


#include <animray/interpolation/linear.hpp>
#include <chrono>
#include <felspar/test.hpp>


namespace {


    auto const suite = felspar::testsuite(__FILE__);


    auto const r = suite.test("rising", [](auto check) {
        check(animray::interpolation::linear(10, 20, 0, 2)) == 10;
        check(animray::interpolation::linear(10, 20, 2, 2)) == 20;
        check(animray::interpolation::linear(10, 20, 1, 2)) == 15;

        check(animray::interpolation::linear(10.0, 20.0, 0, 2)) == 10.0;
        check(animray::interpolation::linear(10.0, 20.0, 2, 2)) == 20.0;
        check(animray::interpolation::linear(10.0, 20.0, 1, 2)) == 15.0;

        check(animray::interpolation::linear(10, 20, 0.0, 2.0)) == 10;
        check(animray::interpolation::linear(10, 20, 2.0, 2.0)) == 20;
        check(animray::interpolation::linear(10, 20, 1.0, 2.0)) == 15;

        check(animray::interpolation::linear(
                10, 20, std::chrono::milliseconds(0),
                std::chrono::milliseconds(2)))
                == 10;
        check(animray::interpolation::linear(
                10, 20, std::chrono::milliseconds(2),
                std::chrono::milliseconds(2)))
                == 20;
        check(animray::interpolation::linear(
                10, 20, std::chrono::milliseconds(1),
                std::chrono::milliseconds(2)))
                == 15;

        check(animray::interpolation::linear(
                10.0, 20.0, std::chrono::milliseconds(0),
                std::chrono::milliseconds(2)))
                == 10;
        check(animray::interpolation::linear(
                10.0, 20.0, std::chrono::milliseconds(2),
                std::chrono::milliseconds(2)))
                == 20;
        check(animray::interpolation::linear(
                10.0, 20.0, std::chrono::milliseconds(1),
                std::chrono::milliseconds(2)))
                == 15;
    });


    auto const f = suite.test("falling", [](auto check) {
        check(animray::interpolation::linear(20, 10, 0, 2)) == 20;
        check(animray::interpolation::linear(20, 10, 2, 2)) == 10;
        check(animray::interpolation::linear(20, 10, 1, 2)) == 15;

        check(animray::interpolation::linear(
                20, 10, std::chrono::milliseconds(0),
                std::chrono::milliseconds(2)))
                == 20;
        check(animray::interpolation::linear(
                20, 10, std::chrono::milliseconds(2),
                std::chrono::milliseconds(2)))
                == 10;
        check(animray::interpolation::linear(
                20, 10, std::chrono::milliseconds(1),
                std::chrono::milliseconds(2)))
                == 15;
    });


}
