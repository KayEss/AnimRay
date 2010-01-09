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


#include <animray/point3d>
#include <fost/test>


FSL_TEST_SUITE( point3d );


FSL_TEST_FUNCTION( constructor_default_tests ) {
    using fostlib::test::default_copy_constructable;
    default_copy_constructable< animray::point3d< int > >();
    default_copy_constructable< animray::point3d< int64_t > >();
    default_copy_constructable< animray::point3d< float > >();
    default_copy_constructable< animray::point3d< double > >();
    default_copy_constructable< animray::point3d< long double > >();
}


namespace {
    template< typename D >
    inline void constructor_tests() {
        animray::point3d< D > h1;
        FSL_CHECK_EQ( h1.array()[0], 0.);
        FSL_CHECK_EQ( h1.array()[1], 0.);
        FSL_CHECK_EQ( h1.array()[2], 0.);
        FSL_CHECK_EQ( h1.array()[3], 1.);
        FSL_CHECK_EQ( h1.x(), 0.);
        FSL_CHECK_EQ( h1.y(), 0.);
        FSL_CHECK_EQ( h1.z(), 0.);

        animray::point3d< D > h2( 1, 2, 3 );
        FSL_CHECK_EQ( h2.x(), 1 );
        FSL_CHECK_EQ( h2.y(), 2 );
        FSL_CHECK_EQ( h2.z(), 3 );
    }
}
FSL_TEST_FUNCTION( constructors ) {
    constructor_tests< int >();
    constructor_tests< int64_t >();
    constructor_tests< float >();
    constructor_tests< double >();
    constructor_tests< long double >();
}


FSL_TEST_FUNCTION( json ) {
    FSL_CHECK_EQ(
        fostlib::coerce< fostlib::json >( animray::point3d< int64_t >() ),
        fostlib::json::parse(L"[0, 0, 0, 1]")
    );
}


FSL_TEST_FUNCTION( minus ) {
    animray::point3d< int >
        h0,
        h1( 1, 0, 0 ),
        h2( -1, 0, 0 )
    ;
    FSL_CHECK_EQ( h0 - h0, h0 );
    FSL_CHECK_EQ( h1 - h0, h1 );
    FSL_CHECK_EQ( h0 - h1, h2 );
    FSL_CHECK_EQ( h1 - h1, h0 );
}
