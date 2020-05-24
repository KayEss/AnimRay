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


#include <animray/maths/angles.hpp>
#include <animray/affine.hpp>
#include <animray/ray.hpp>

#include <fost/test>


FSL_TEST_SUITE(matrix);


static_assert(std::regular<animray::matrix<int>>);
static_assert(std::regular<animray::matrix<float>>);


FSL_TEST_FUNCTION(matrix_multiply) {
    animray::matrix<int> id;
    auto aff(animray::translate<int>(10, 23, 54));
    FSL_CHECK_EQ(aff.forward() * aff.backward(), id);
    FSL_CHECK_EQ(id * aff.forward(), aff.forward());
    FSL_CHECK_EQ(id * aff.backward(), aff.backward());
    animray::point3d<int> pf(aff.forward() * animray::point3d<int>());
    FSL_CHECK_EQ(pf.x(), 10);
    FSL_CHECK_EQ(pf.y(), 23);
    FSL_CHECK_EQ(pf.z(), 54);
    animray::point3d<int> ps(aff.backward() * animray::point3d<int>());
    FSL_CHECK_EQ(ps.x(), -10);
    FSL_CHECK_EQ(ps.y(), -23);
    FSL_CHECK_EQ(ps.z(), -54);
}


FSL_TEST_FUNCTION(ray_rotate_forward) {
    animray::ray<double> z(
            animray::point3d<double>(0, 0, 0),
            animray::unit_vector<double>(0, 0, 1));
    FSL_CHECK_EQ(z.direction, (z.ends() - z.from).unit());

    animray::matrix<double> rx45(animray::rotate_x<double>(-45_deg).first);
    animray::ray<double> r45(z * rx45);
    FSL_CHECK_EQ(r45.direction, (r45.ends() - r45.from).unit());

    animray::ray<double> r90(r45 * rx45);
    FSL_CHECK_EQ(r90.direction, (r90.ends() - r90.from).unit());
    FSL_CHECK_ERROR(r90.direction.x(), 0, 1e-10);
    FSL_CHECK_ERROR(r90.direction.y(), 1, 1e-10);
    FSL_CHECK(r90.direction.z() < 1e-10);
}


FSL_TEST_FUNCTION(ray_rotate_backward) {
    animray::ray<double> z(
            animray::point3d<double>(0, 0, 0),
            animray::unit_vector<double>(0, 0, 1));
    FSL_CHECK_EQ(z.direction, (z.ends() - z.from).unit());

    animray::matrix<double> rx45(animray::rotate_x<double>(45_deg).second);
    animray::ray<double> r45(z * rx45);
    FSL_CHECK_EQ(r45.direction, (r45.ends() - r45.from).unit());

    animray::ray<double> r90(r45 * rx45);
    FSL_CHECK_EQ(r90.direction, (r90.ends() - r90.from).unit());
    FSL_CHECK_ERROR(r90.direction.x(), 0, 1e-10);
    FSL_CHECK_ERROR(r90.direction.y(), 1, 1e-10);
    FSL_CHECK(r90.direction.z() < 1e-10);
}


FSL_TEST_FUNCTION(ray_scale) {
    animray::ray<double> all(
            animray::point3d<double>(0, 0, 0),
            animray::point3d<double>(1, 1, 1));
    FSL_CHECK_EQ(all.direction, (all.ends() - all.from).unit());

    animray::matrix<double> halve(animray::scale<double>(0.5, 0.5, 0.5).first);
    animray::ray<double> havled(all * halve);
    FSL_CHECK_EQ(all, havled);
}
