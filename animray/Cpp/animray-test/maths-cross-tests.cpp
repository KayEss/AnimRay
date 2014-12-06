/*
    Copyright 2010-2014, Kirit Saelensminde.
    http://www.kirit.com/AnimRay

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


#include <animray/maths/cross.hpp>
#include <fost/test>


FSL_TEST_SUITE(cross_product);


FSL_TEST_FUNCTION(simple) {
    FSL_CHECK_EQ(cross(
        animray::unit_vector<int>(0, 1, 0),
        animray::point3d<int>(1, 0, 0)),
        animray::point3d<int>(0, 0, -1));
    FSL_CHECK_EQ(cross(
        animray::unit_vector<int>(1, 0, 0),
        animray::point3d<int>(0, 1, 0)),
        animray::point3d<int>(0, 0, 1));
}


FSL_TEST_FUNCTION(inverse) {
    animray::point3d<double> e1(5, 0, 0), e2(0, 3, 0);
    FSL_CHECK_EQ(cross(e1, e2), animray::point3d<double>(0, 0, 15));
    FSL_CHECK_EQ(-cross(e1, e2), animray::point3d<double>(0, 0, -15));
    FSL_CHECK_EQ(cross(e2, e1), animray::point3d<double>(0, 0, -15));
    FSL_CHECK_EQ(-cross(e2, e1), animray::point3d<double>(0, 0, 15));
}

