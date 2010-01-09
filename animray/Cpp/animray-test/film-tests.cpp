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


#include <animray/film>
#include <fost/test>


FSL_TEST_SUITE( film );


FSL_TEST_FUNCTION( extents_construct ) {
    fostlib::test::default_copy_constructable< animray::point2d< char > >();
    fostlib::test::default_copy_constructable< animray::extents2d< char > >();
}


FSL_TEST_FUNCTION( film_construction ) {
    animray::film< uint8_t > film( 10, 100 );
    FSL_CHECK_EQ( film.size(), animray::film< uint8_t >::extents_type(0, 0, 9, 99) );
}
