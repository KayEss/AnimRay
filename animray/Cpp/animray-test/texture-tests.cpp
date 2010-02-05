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


#include <animray/texture.hpp>
#include <fost/test>


FSL_TEST_SUITE( texture );


namespace {
    int square( double u, double v ) {
        if ( std::abs(u) <= 1 && std::abs(v) <= 1 )
            return 1;
        else
            return 0;
    }
}


FSL_TEST_FUNCTION( basic_mapping ) {
    animray::texture<
        uint8_t, animray::texture_binop_wrapper< int, double >
    > t( square, 0 );
}
