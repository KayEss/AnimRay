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


#include <animray/point2d.hpp>

#include <fost/test>


FSL_TEST_SUITE(point2d);


static_assert(std::regular<animray::point2d<int>>);
static_assert(std::regular<animray::point2d<float>>);


FSL_TEST_FUNCTION(operator_add) {
    animray::point2d<int> p1, p2 = animray::point2d<int>(1, 2);
    FSL_CHECK_EQ(p1 += p2, animray::point2d<int>(1, 2));
    FSL_CHECK_EQ(p1 += p2, 2 * animray::point2d<int>(1, 2));
}


FSL_TEST_FUNCTION(operator_mul) {
    animray::point2d<int> p1, p2 = animray::point2d<int>(1, 2);
    FSL_CHECK_EQ(p1 * 4, p1);
    FSL_CHECK_EQ(p2 *= 3, animray::point2d<int>(3, 6));
}
