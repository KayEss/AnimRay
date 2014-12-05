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


#include <animray/geometry/planar/triangle.hpp>
#include <animray/ray.hpp>
#include <fost/log>
#include <fost/test>


FSL_TEST_SUITE(triangle);


FSL_TEST_FUNCTION(occludes) {
    animray::triangle<animray::ray<double>> right(
        animray::point3d<double>(0, 0, 0),
        animray::point3d<double>(5.f, 0, 0),
        animray::point3d<double>(0, 3.f, 0));
    FSL_CHECK(right.occludes(
        animray::ray<double>(
            animray::point3d<double>(1, 1, 1),
            animray::unit_vector<double>(0, 0, -1)), 0));
    FSL_CHECK(not right.occludes(
        animray::ray<double>(
            animray::point3d<double>(-1, 1, -1),
            animray::unit_vector<double>(0, 0, -1)), 0));
}

