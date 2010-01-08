/*
    Copyright 2010, Kirit Saelensminde. http://www.kirit.com/AnimRay
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <animray/ray>
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
        animray::ray< int >::end_type( 0, 0, 1 )
    );
}
