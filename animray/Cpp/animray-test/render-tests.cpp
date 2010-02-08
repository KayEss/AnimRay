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


#include <animray/mandelbrot.hpp>
#include <animray/render.hpp>
#include <fost/test>


FSL_TEST_SUITE( render );


FSL_TEST_FUNCTION( constructors ) {
    animray::render<
        animray::texture< int, animray::point2d< std::size_t > >
    > r1( 127 );
    FSL_CHECK_EQ( r1.film()[0][0], 127 );
}


FSL_TEST_FUNCTION( mandelbrot ) {
    typedef animray::mandelbrot::iterations<
        animray::film< uint8_t, std::size_t >, double
    > mandelbrot_type;

    typedef animray::texture<
        int, animray::point2d< std::size_t >,
        mandelbrot_type
    > mandelbrot_texture_type;

    animray::render< mandelbrot_texture_type > rm(
        mandelbrot_type( 100, 100, 0, 0, 2, 6 )
    );
}

