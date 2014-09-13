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


#include <animray/affine.hpp>
#include <animray/ray.hpp>
#include <fost/test>


FSL_TEST_SUITE( ray );


FSL_TEST_FUNCTION( constructor_default_tests ) {
    fostlib::test::default_copy_constructable< animray::ray< int > >();
    fostlib::test::default_copy_constructable< animray::ray< int64_t > >();
    fostlib::test::default_copy_constructable< animray::ray< float > >();
    fostlib::test::default_copy_constructable< animray::ray< double > >();
    fostlib::test::default_copy_constructable< animray::ray< long double > >();
}


FSL_TEST_FUNCTION( constructor_simple ) {
    animray::ray< int > r1(
        animray::ray< int >::end_type( 0, 0, 0 ),
        animray::ray< int >::end_type( 0, 0, 1 ));
}


FSL_TEST_FUNCTION( transformation ) {
    animray::ray< int > r(
        animray::ray< int >::end_type( 0, 0, 0 ),
        animray::ray< int >::end_type( 0, 0, 1 ));
    std::pair< animray::matrix< int >, animray::matrix< int > > aff
        (animray::translate(10, 23, 54));
    animray::ray<int> rf(r * aff.first);
    FSL_CHECK_EQ(rf.from(), animray::ray< int >::end_type(10, 23, 54));
    FSL_CHECK_EQ(rf.direction(), animray::ray< int >::end_type(0, 0, 1));
    animray::ray<int> rb(r * aff.second);
    FSL_CHECK_EQ(rb.from(), animray::ray< int >::end_type(-10, -23, -54));
    FSL_CHECK_EQ(rb.direction(), animray::ray< int >::end_type(0, 0, 1));

    FSL_CHECK_EQ((r * aff.first * aff.second).from(), r.from());
    FSL_CHECK_EQ((r * aff.first * aff.second).direction(), r.direction());
    FSL_CHECK_EQ((r * aff.second * aff.first).from(), r.from());
    FSL_CHECK_EQ((r * aff.second * aff.first).direction(), r.direction());
}

