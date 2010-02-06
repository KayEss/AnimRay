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
    typedef animray::extents2d< double > extents_type;
    typedef uint8_t color_type;

    animray::raster< color_type, extents_type > r1(
        extents_type(-1, -1, 1, 1), 4.0 / 10000.0 // 100 pixels in each dimension
    );
    animray::raster< color_type, extents_type > r2(
        extents_type(-1, -1, 1, 1), 100, 100
    );
    FSL_CHECK_EQ( r1.pixel_area(), r2.pixel_area() );
    FSL_CHECK_EQ( r1.pixels(), r2.pixels() );
}


FSL_TEST_FUNCTION( film_api ) {
    typedef animray::extents2d< double > extents_type;
    typedef uint8_t color_type;
    animray::raster< color_type, extents_type > raster(
        extents_type(-1, -1, 1, 1), 2, 2
    );
    FSL_CHECK_EQ( raster.pixel_area(), 1 );
    FSL_CHECK_EQ( raster.pixels(), 4u );
}

