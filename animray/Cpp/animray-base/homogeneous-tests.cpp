/*
    Copyright 2010, Kirit Saelensminde. http://www.kirit.com/AnimRay
    Distributed under the Boost Software License, Version 1.0.
    See accompanying file LICENSE_1_0.txt or copy at
        http://www.boost.org/LICENSE_1_0.txt
*/


#include <animray/homogeneous>
#include <fost/test>


FSL_TEST_SUITE( homogeneous );


FSL_TEST_FUNCTION( constructors ) {
    animray::homogeneous< int > h;
    FSL_CHECK_EQ( h.coords()[0], 0.);
    FSL_CHECK_EQ( h.coords()[1], 0.);
    FSL_CHECK_EQ( h.coords()[2], 0.);
    FSL_CHECK_EQ( h.coords()[3], 1.);
    FSL_CHECK_EQ( h.x(), 0.);
    FSL_CHECK_EQ( h.y(), 0.);
    FSL_CHECK_EQ( h.z(), 0.);
}

