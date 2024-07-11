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


#include <animray/film.hpp>
#include <animray/functional/traits.hpp>
#include <felspar/test.hpp>

#include <cstdint>


namespace {


    auto const suite = felspar::testsuite("film");


    static_assert(animray::Regular<animray::point2d<char>>);


    std::uint8_t white(std::size_t, std::size_t) { return 255; }
    auto const fcons = suite.test("film construction", [](auto check) {
        check(animray::film<std::uint8_t>{10, 100}.size())
                == animray::film<std::uint8_t>::extents_type(0, 0, 9, 99);

        check([]() {
            animray::film<std::uint8_t>{0, 10};
        }).throws(std::underflow_error{"Width can't be less than 1"});
        check([]() {
            animray::film<std::uint8_t>{10, 0};
        }).throws(std::underflow_error{"Height can't be less than 1"});

        check([]() {
            animray::film<std::uint8_t>{0, 10, white};
        }).throws(std::underflow_error{"Width can't be less than 1"});
        check([]() {
            animray::film<std::uint8_t>{10, 0, white};
        }).throws(std::underflow_error{"Height can't be less than 1"});
    });


    auto const fcomp = suite.test("film composition", [](auto check) {
        animray::film<uint8_t> background(10, 10, 3);
        animray::film<uint8_t> foreground(10, 10, 4);

        check(background[5][5]) == 3u;
        animray::film<uint8_t> composite(
                background.width(), background.height(),
                [&](animray::film<uint8_t>::size_type x,
                    animray::film<uint8_t>::size_type y) {
                    return background[x][y] + foreground[x][y];
                });
        check(composite[5][5]) == 7u;
    });


}
