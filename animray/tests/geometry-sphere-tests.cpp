/*
    Copyright 2010-2018, Kirit Saelensminde.
    <https://kirit.com/AnimRay>

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


#include <animray/geometry/quadrics/sphere-unit-origin.hpp>
#include <animray/ray.hpp>
#include <fost/insert>
#include <fost/test>


FSL_TEST_SUITE( sphere );


FSL_TEST_FUNCTION( constructor_default_tests ) {
    fostlib::test::default_copy_constructable<
        animray::unit_sphere_at_origin< animray::ray< int>  > >();
    fostlib::test::default_copy_constructable<
        animray::unit_sphere_at_origin< animray::ray< int64_t > > >();
    fostlib::test::default_copy_constructable<
        animray::unit_sphere_at_origin< animray::ray<float > > >();
    fostlib::test::default_copy_constructable<
        animray::unit_sphere_at_origin< animray::ray<double > > >();
    fostlib::test::default_copy_constructable<
        animray::unit_sphere_at_origin< animray::ray<long double > > >();
}


namespace {
    template< typename D >
    void sphere_occlude() {
        try {
            typedef typename animray::ray< D >::end_type end_type;
            typedef animray::ray< D > ray;
            animray::unit_sphere_at_origin< animray::ray< D > > s;
            FSL_CHECK( s.occludes(
                ray( end_type( 0, 0, 10 ), end_type() ), 0
            ) );
            FSL_CHECK( s.occludes(
                ray( end_type(), end_type( 0, 0, 10 ) ), 0
            ) );
            FSL_CHECK( !s.occludes(
                ray( end_type( 0, 0, 5 ), end_type( 0, 0, 10 ) ), 0
            ) );
            FSL_CHECK( s.occludes(
                ray( end_type( 0, 0, 10 ), end_type( 0, 0, 5 ) ), 0
            ) );
        } catch ( fostlib::exceptions::exception &e ) {
            fostlib::insert(e.data(), "Type under test",
                fostlib::coerce< fostlib::string >(typeid(D).name()));;
            throw;
        }
    }
}
FSL_TEST_FUNCTION( occlusion ) {
    sphere_occlude< int >();
    sphere_occlude< int64_t >();
    sphere_occlude< float >();
    sphere_occlude< double >();
    sphere_occlude< long double >();
}
