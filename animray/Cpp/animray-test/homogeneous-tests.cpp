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
    using fostlib::test::default_copy_constructable;
    default_copy_constructable< animray::homogeneous< int > >();
    default_copy_constructable< animray::homogeneous< int64_t > >();
    default_copy_constructable< animray::homogeneous< float > >();
    default_copy_constructable< animray::homogeneous< double > >();
    default_copy_constructable< animray::homogeneous< long double > >();
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


FSL_TEST_FUNCTION( json ) {
    FSL_CHECK_EQ(
        fostlib::coerce< fostlib::json >( animray::homogeneous< int64_t >() ),
        fostlib::json::parse(L"[0, 0, 0, 1]")
    );
}


FSL_TEST_FUNCTION( minus ) {
    animray::homogeneous< int >
        h0,
        h1( 1, 0, 0 ),
        h2( -1, 0, 0 )
    ;
    FSL_CHECK_EQ( h0 - h0, h0 );
    FSL_CHECK_EQ( h1 - h0, h1 );
    FSL_CHECK_EQ( h0 - h1, h2 );
    FSL_CHECK_EQ( h1 - h1, h0 );
}
