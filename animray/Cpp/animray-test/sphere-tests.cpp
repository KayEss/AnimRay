/*
    Copyright 2010, Kirit Saelensminde. http://www.kirit.com/AnimRay
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <animray/sphere>
#include <fost/test>


FSL_TEST_SUITE( sphere );


FSL_TEST_FUNCTION( constructor_default_tests ) {
    fostlib::test::default_copy_constructable< animray::sphere< int > >();
    fostlib::test::default_copy_constructable< animray::sphere< int64_t > >();
    fostlib::test::default_copy_constructable< animray::sphere< float > >();
    fostlib::test::default_copy_constructable< animray::sphere< double > >();
    fostlib::test::default_copy_constructable< animray::sphere< long double > >();
}


FSL_TEST_FUNCTION( intersection ) {
    FSL_CHECK( animray::sphere< double >()
        .occludes( animray::line< double >(
            animray::line< double >::end_type( 0, 0, 10 ),
            animray::line< double >::end_type()
        ) )
    );
}
