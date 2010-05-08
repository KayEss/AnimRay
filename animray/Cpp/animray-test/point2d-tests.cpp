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


#include <animray/point2d.hpp>
#include "test-json.hpp"


FSL_TEST_SUITE( point2d );


FSL_TEST_FUNCTION( constructor_default_tests ) {
    using fostlib::test::default_copy_constructable;
    default_copy_constructable< animray::point2d< int > >();
    default_copy_constructable< animray::point2d< int64_t > >();
    default_copy_constructable< animray::point2d< float > >();
    default_copy_constructable< animray::point2d< double > >();
    default_copy_constructable< animray::point2d< long double > >();
}


FSL_TEST_FUNCTION( json ) {
    json_roundtrip( animray::point2d< int64_t >(), "[0, 0]" );
    json_roundtrip( animray::point2d< int >(1,2), "[1,2]" );
    json_roundtrip( animray::point2d< int >(2,-5), "[2,-5]" );
}


FSL_TEST_FUNCTION( operator_add ) {
    animray::point2d<int> p1, p2 = animray::point2d<int>(1,2);
    FSL_CHECK_EQ(p1 += p2, animray::point2d<int>(1,2));
    FSL_CHECK_EQ(p1 += p2, 2 * animray::point2d<int>(1,2));
}
