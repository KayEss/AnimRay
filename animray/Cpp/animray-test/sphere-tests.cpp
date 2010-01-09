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
        .occludes( animray::ray< double >(
            animray::line< double >::end_type( 0, 0, 10 ),
            animray::line< double >::end_type()
        ) )
    );
    FSL_CHECK( not animray::sphere< double >()
        .occludes( animray::ray< double >(
            animray::line< double >::end_type(),
            animray::line< double >::end_type( 0, 0, 10 )
        ) )
    );
    FSL_CHECK( not animray::sphere< double >()
        .occludes( animray::ray< double >(
            animray::line< double >::end_type( 0, 0, 5 ),
            animray::line< double >::end_type( 0, 0, 10 )
        ) )
    );
}
