/*
    Copyright s2014, Kirit Saelensminde.
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


#include <animray/interpolation/linear.hpp>
#include <fost/test>


FSL_TEST_SUITE(interpolation_linear);


FSL_TEST_FUNCTION(rising) {
    FSL_CHECK_EQ(animray::interpolation::linear(10, 20, 0, 2), 10);
    FSL_CHECK_EQ(animray::interpolation::linear(10, 20, 2, 2), 20);
    FSL_CHECK_EQ(animray::interpolation::linear(10, 20, 1, 2), 15);

    FSL_CHECK_EQ(animray::interpolation::linear(10.0, 20.0, 0, 2), 10.0);
    FSL_CHECK_EQ(animray::interpolation::linear(10.0, 20.0, 2, 2), 20.0);
    FSL_CHECK_EQ(animray::interpolation::linear(10.0, 20.0, 1, 2), 15.0);

    FSL_CHECK_EQ(animray::interpolation::linear(10, 20, 0.0, 2.0), 10);
    FSL_CHECK_EQ(animray::interpolation::linear(10, 20, 2.0, 2.0), 20);
    FSL_CHECK_EQ(animray::interpolation::linear(10, 20, 1.0, 2.0), 15);

    FSL_CHECK_EQ(animray::interpolation::linear(10, 20,
        fostlib::milliseconds(0), fostlib::milliseconds(2)), 10);
    FSL_CHECK_EQ(animray::interpolation::linear(10, 20,
        fostlib::milliseconds(2), fostlib::milliseconds(2)), 20);
    FSL_CHECK_EQ(animray::interpolation::linear(10, 20,
        fostlib::milliseconds(1), fostlib::milliseconds(2)), 15);

    FSL_CHECK_EQ(animray::interpolation::linear(10.0, 20.0,
        fostlib::milliseconds(0), fostlib::milliseconds(2)), 10);
    FSL_CHECK_EQ(animray::interpolation::linear(10.0, 20.0,
        fostlib::milliseconds(2), fostlib::milliseconds(2)), 20);
    FSL_CHECK_EQ(animray::interpolation::linear(10.0, 20.0,
        fostlib::milliseconds(1), fostlib::milliseconds(2)), 15);
}


FSL_TEST_FUNCTION(falling) {
    FSL_CHECK_EQ(animray::interpolation::linear(20, 10, 0, 2), 20);
    FSL_CHECK_EQ(animray::interpolation::linear(20, 10, 2, 2), 10);
    FSL_CHECK_EQ(animray::interpolation::linear(20, 10, 1, 2), 15);

    FSL_CHECK_EQ(animray::interpolation::linear(20, 10,
        fostlib::milliseconds(0), fostlib::milliseconds(2)), 20);
    FSL_CHECK_EQ(animray::interpolation::linear(20, 10,
        fostlib::milliseconds(2), fostlib::milliseconds(2)), 10);
    FSL_CHECK_EQ(animray::interpolation::linear(20, 10,
        fostlib::milliseconds(1), fostlib::milliseconds(2)), 15);
}

