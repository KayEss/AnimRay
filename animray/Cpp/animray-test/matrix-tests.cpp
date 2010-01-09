/*
    Copyright 2010, Kirit Saelensminde. http://www.kirit.com/AnimRay
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <animray/matrix>
#include <fost/test>


FSL_TEST_SUITE( matrix );


FSL_TEST_FUNCTION( constructor_default_tests ) {
    fostlib::test::default_copy_constructable< animray::matrix< int > >();
    fostlib::test::default_copy_constructable< animray::matrix< int64_t > >();
    fostlib::test::default_copy_constructable< animray::matrix< float > >();
    fostlib::test::default_copy_constructable< animray::matrix< double > >();
    fostlib::test::default_copy_constructable< animray::matrix< long double > >();
}


FSL_TEST_FUNCTION( json ) {
    FSL_CHECK_EQ(
        fostlib::coerce< fostlib::json >( animray::matrix< int64_t >() ),
        fostlib::json::parse(L"[[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]]")
    );
}

