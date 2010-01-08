/*
    Copyright 2010, Kirit Saelensminde. http://www.kirit.com/AnimRay
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <animray/line>
#include <fost/test>


FSL_TEST_SUITE( line );


FSL_TEST_FUNCTION( constructor_default_tests ) {
    fostlib::test::default_copy_constructable< animray::line< int > >();
    fostlib::test::default_copy_constructable< animray::line< int64_t > >();
    fostlib::test::default_copy_constructable< animray::line< float > >();
    fostlib::test::default_copy_constructable< animray::line< double > >();
    fostlib::test::default_copy_constructable< animray::line< long double > >();
}


FSL_TEST_FUNCTION( constructor_simple ) {
    animray::line< int > l1(
        animray::line< int >::end_type( 0, 0, 1 ),
        animray::line< int >::end_type( 0, 0, 10 )
    );
}


FSL_TEST_FUNCTION( length_squared ) {
    FSL_CHECK_EQ(
        animray::line< int >(
            animray::line< int >::end_type(),
            animray::line< int >::end_type( 1, 0, 0 )
        ).length_squared(), 1
    );
    FSL_CHECK_EQ(
        animray::line< int >(
            animray::line< int >::end_type(),
            animray::line< int >::end_type( 0, 2, 0 )
        ).length_squared(), 4
    );
}
