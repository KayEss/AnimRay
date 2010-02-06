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


#include <animray/raster.hpp>
#include <animray/extents2d.hpp>
#include <fost/test>


FSL_TEST_SUITE( raster );


FSL_TEST_FUNCTION( constructors ) {
    animray::raster< animray::film< uint8_t > > r1(
        std::auto_ptr< animray::film< uint8_t > >(
            new animray::film< uint8_t >( 10, 6 )
        )
    );
    FSL_CHECK_EQ( r1.film().width(), 10u );
    FSL_CHECK_EQ( r1.film().height(), 6u );

    animray::raster< animray::film< unsigned int > > r2( 4, 3, 127 );
    animray::raster< animray::film< unsigned int > > r3( r2, 4, 3 );

    FSL_CHECK_EQ( r3.film().width(), 16u );
    FSL_CHECK_EQ( r3.film().height(), 9u );
    FSL_CHECK_EQ( r3.film()[12][4], 127u );
}

