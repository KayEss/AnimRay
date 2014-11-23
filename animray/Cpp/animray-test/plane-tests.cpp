/*
    Copyright 2014, Kirit Saelensminde.
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


#include <animray/geometry/planar/plane.hpp>
#include <animray/ray.hpp>
#include <fost/test>


FSL_TEST_SUITE( plane );


FSL_TEST_FUNCTION(plane_constructor) {
    animray::plane<animray::ray<int>> board;
    FSL_CHECK_EQ(board.center(), animray::point3d<int>(0, 0, 0));
    FSL_CHECK_EQ(board.normal(),
        animray::unit_vector<int>(animray::point3d<int>(0, 0, 1)));
}
