/**
    Copyright 2010-2020, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#include <animray/line.hpp>

#include <fost/test>
#include <concepts>


FSL_TEST_SUITE(line);


static_assert(std::regular<animray::line<int>>);
static_assert(std::regular<animray::line<float>>);


FSL_TEST_FUNCTION(constructor_simple) {
    animray::line<int> l1(
            animray::line<int>::end_type(0, 0, 1),
            animray::line<int>::end_type(0, 0, 10));
}


FSL_TEST_FUNCTION(length_squared) {
    FSL_CHECK_EQ(
            animray::line<int>(
                    animray::line<int>::end_type(),
                    animray::line<int>::end_type(1, 0, 0))
                    .length_squared(),
            1);
    FSL_CHECK_EQ(
            animray::line<int>(
                    animray::line<int>::end_type(),
                    animray::line<int>::end_type(0, 2, 0))
                    .length_squared(),
            4);
    FSL_CHECK_EQ(
            animray::line<int>(
                    animray::line<int>::end_type(),
                    animray::line<int>::end_type(3, 0, 4))
                    .length_squared(),
            25);
}
