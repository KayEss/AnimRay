/**
    Copyright 2021, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#include <animray/numeric.hpp>

#include <fost/test.hpp>


namespace {


    auto const nbr = felspar::testsuite("numeric");

    struct nt {};
    using numeric = animray::number<nt, int>;

    auto const eq = nbr.test("<=>", [](auto check) {
        check(numeric{3}) == 3;
        check(3) == numeric{3};
        check(numeric{4}) != 3;
        check(4) != numeric{3};
        check(numeric{4}) > 3;
        check(numeric{3}) < 4;
    });

    auto const ops = nbr.test("+",
                              [](auto check) {
                                  check(numeric{2} + 3) == 5;
                                  check(2 + numeric{3}) == 5;
                                  check(numeric{2} + numeric{3}) == 5;
                              })
                             .test("-",
                                   [](auto check) {
                                       check(numeric{2} - 3) == -1;
                                       check(2 - numeric{3}) == -1;
                                       check(numeric{2} - numeric{3}) == -1;
                                   })
                             .test("*",
                                   [](auto check) {
                                       check(numeric{2} * 3) == 6;
                                       check(2 * numeric{3}) == 6;
                                       check(numeric{2} * numeric{3}) == 6;
                                   })
                             .test("/", [](auto check) {
                                 check(numeric{6} / 2) == 3;
                                 check(6 / numeric{2}) == 3;
                                 check(numeric{6} / numeric{2}) == 3;
                             });


}
