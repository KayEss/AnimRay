/*
    Copyright 2010, Kirit Saelensminde. http://www.kirit.com/AnimRay
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <animray/homogeneous>
#include <fost/test>


FSL_TEST_SUITE( homogeneous );


FSL_TEST_FUNCTION( constructor_default_tests ) {
    fostlib::test::default_copy_constructable< animray::homogeneous< int > >();
    fostlib::test::default_copy_constructable< animray::homogeneous< int64_t > >();
    fostlib::test::default_copy_constructable< animray::homogeneous< float > >();
    fostlib::test::default_copy_constructable< animray::homogeneous< double > >();
    fostlib::test::default_copy_constructable< animray::homogeneous< long double > >();
}


namespace {
    template< typename D >
    inline void constructor_tests() {
        animray::homogeneous< D > h1;
        FSL_CHECK_EQ( h1.coords()[0], 0.);
        FSL_CHECK_EQ( h1.coords()[1], 0.);
        FSL_CHECK_EQ( h1.coords()[2], 0.);
        FSL_CHECK_EQ( h1.coords()[3], 1.);
        FSL_CHECK_EQ( h1.x(), 0.);
        FSL_CHECK_EQ( h1.y(), 0.);
        FSL_CHECK_EQ( h1.z(), 0.);

        animray::homogeneous< D > h2( 1, 2, 3 );
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

