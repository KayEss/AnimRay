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


#include <animray/point3d.hpp>
#include <fost/test>


FSL_TEST_SUITE(point3d);


static_assert(std::regular<animray::point3d<int>>);
static_assert(std::regular<animray::point3d<float>>);


FSL_TEST_FUNCTION(basic_arithmatic) {
    animray::point3d<int> x(2, 0, 0, 2), y(0, 2, 0, 2), z(0, 0, 2, 2),
            xy(2, 2, 0, 2), xz(2, 0, 2, 2), yz(0, 2, 2, 2);
    FSL_CHECK_EQ(x + y, xy);
    FSL_CHECK_EQ(x + z, xz);
    FSL_CHECK_EQ(y + z, yz);
    FSL_CHECK_EQ(xy * xz, x);
    FSL_CHECK_EQ(xy * yz, y);
    FSL_CHECK_EQ(xz * yz, z);
    FSL_CHECK_EQ(x + x, x * 2);
    FSL_CHECK_EQ(y + y, y * 2);
    FSL_CHECK_EQ(z + z, z * 2);
}


FSL_TEST_FUNCTION(dot) {
    animray::point3d<int> h1(3, 4, 5), h2(-2, -2, -2);
    FSL_CHECK_EQ(h1.dot(), 50);
    FSL_CHECK_EQ(h2.dot(), 12);
    FSL_CHECK_EQ((h1 - h2).dot(), 110);
    FSL_CHECK_EQ((h2 - h1).dot(), 110);
    FSL_CHECK_EQ((h1 + h2).dot(), 14);
}
