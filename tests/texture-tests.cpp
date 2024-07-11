/**
    Copyright 2010-2024, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#include <animray/texture.hpp>
#include <felspar/test.hpp>

#include <cstdint>


namespace {


    auto const suite = felspar::testsuite(__FILE__);


    auto const bc = suite.test("basic color", [](auto check) {
        using texture_type = animray::texture<std::uint8_t, animray::point2d<int>>;
        texture_type t(123);
        check(t(texture_type::location_type()))
                == texture_type::color_type(123);
    });


    auto const st = suite.test("square texture", [](auto check) {
        using location_type = animray::point2d<double>;
        using texture_type = animray::texture<
                int, location_type, std::function<int(double, double)>>;

        texture_type t([](auto const u, auto const v) {
            if (std::abs(u) <= 1 and std::abs(v) <= 1) {
                return 1;
            } else {
                return 0;
            }
        });
        check(t(location_type())) == 1;
        check(t(location_type(2, 2))) == 0;
    });


}
