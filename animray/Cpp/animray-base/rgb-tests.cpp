/*
    Copyright 2010, Kirit Saelensminde. http://www.kirit.com/AnimRay
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <animray/rgb>
#include <fost/test>


FSL_TEST_SUITE( rgb );


FSL_TEST_FUNCTION( constructor_default_tests ) {
    fostlib::test::default_copy_constructable< animray::rgb< int > >();
    fostlib::test::default_copy_constructable< animray::rgb< int64_t > >();
    fostlib::test::default_copy_constructable< animray::rgb< float > >();
    fostlib::test::default_copy_constructable< animray::rgb< double > >();
    fostlib::test::default_copy_constructable< animray::rgb< long double > >();
}


FSL_TEST_FUNCTION( json ) {
    FSL_CHECK_EQ(
        fostlib::coerce< fostlib::json >( animray::rgb< int64_t >() ),
        fostlib::json::parse(L"[0, 0, 0]")
    );
}

