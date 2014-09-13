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


#include <animray/rgb.hpp>
#include "test-json.hpp"


FSL_TEST_SUITE( rgb );


FSL_TEST_FUNCTION( constructor_default_tests ) {
    fostlib::test::default_copy_constructable< animray::rgb< int > >();
    fostlib::test::default_copy_constructable< animray::rgb< int64_t > >();
    fostlib::test::default_copy_constructable< animray::rgb< float > >();
    fostlib::test::default_copy_constructable< animray::rgb< double > >();
    fostlib::test::default_copy_constructable< animray::rgb< long double > >();
}


FSL_TEST_FUNCTION( json ) {
    json_roundtrip( animray::rgb< int64_t >(), "[0, 0, 0]" );
    json_roundtrip( animray::rgb< uint8_t >( 1, 2, 3 ), "[1,2,3]" );
}


FSL_TEST_FUNCTION(attenuation) {
    animray::rgb<int> f1(animray::rgb<int>(10, 10, 10) * 0.5f);
    FSL_CHECK_EQ(f1.red(), 5);
    FSL_CHECK_EQ(f1.green(), 5);
    FSL_CHECK_EQ(f1.blue(), 5);
}

