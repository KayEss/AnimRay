/*
    Copyright 2010, Kirit Saelensminde.
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


#include <animray/unit-vector.hpp>
#include <fost/test>


using animray::unit_vector;


FSL_TEST_SUITE(unit_vector);


FSL_TEST_FUNCTION(constructor_default_tests) {
    fostlib::test::default_copy_constructable< unit_vector< int > >();
    fostlib::test::default_copy_constructable< unit_vector< int64_t > >();
    fostlib::test::default_copy_constructable< unit_vector< float > >();
    fostlib::test::default_copy_constructable< unit_vector< double > >();
    fostlib::test::default_copy_constructable< unit_vector< long double > >();
}


FSL_TEST_FUNCTION(comparison) {
    FSL_CHECK_EQ(unit_vector<int>(), unit_vector<int>(0, 0, 1));
}

