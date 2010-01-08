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
