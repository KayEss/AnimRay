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


#include <animray/affine.hpp>
#include "test-json.hpp"


FSL_TEST_SUITE( matrix );


FSL_TEST_FUNCTION( constructor_default_tests ) {
    fostlib::test::default_copy_constructable< animray::matrix< int > >();
    fostlib::test::default_copy_constructable< animray::matrix< int64_t > >();
    fostlib::test::default_copy_constructable< animray::matrix< float > >();
    fostlib::test::default_copy_constructable< animray::matrix< double > >();
    fostlib::test::default_copy_constructable< animray::matrix< long double > >();
}


FSL_TEST_FUNCTION( matrix_multiply ) {
    animray::matrix< int > id;
    std::pair< animray::matrix< int >, animray::matrix< int > > aff
        (animray::translate(10, 23, 54));
    FSL_CHECK_EQ(aff.first * aff.second, id);
    FSL_CHECK_EQ(id * aff.first, aff.first);
    FSL_CHECK_EQ(id * aff.second, aff.second);
    animray::point3d< int > pf(aff.first * animray::point3d<int>());
    FSL_CHECK_EQ(pf.x(), 10);
    FSL_CHECK_EQ(pf.y(), 23);
    FSL_CHECK_EQ(pf.z(), 54);
    animray::point3d< int > ps(aff.second * animray::point3d<int>());
    FSL_CHECK_EQ(ps.x(), -10);
    FSL_CHECK_EQ(ps.y(), -23);
    FSL_CHECK_EQ(ps.z(), -54);
}


FSL_TEST_FUNCTION( json ) {
    json_roundtrip(
        animray::matrix< int64_t >(),
        L"[[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]]"
    );
    animray::matrix< int64_t > m( fostlib::json::parse(
        L"[[1, 2, 3, 4], [5, 6, 7, 8], [9, 10, 11, 12], [13, 14, 15, 16]]"
    ) );
    FSL_CHECK_EQ( m[0][0], 1 );
    FSL_CHECK_EQ( m[0][2], 3 );
    FSL_CHECK_EQ( m[3][3], 16 );
    FSL_CHECK_EXCEPTION( m[4][0], fostlib::exceptions::out_of_range<std::size_t>& );
}

