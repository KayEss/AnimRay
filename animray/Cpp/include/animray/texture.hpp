/*
    Copyright 1995-2010, Kirit Saelensminde.
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


#ifndef ANIMRAY_TEXTURE_HPP
#define ANIMRAY_TEXTURE_HPP
#pragma once


#include <fost/core>
#include <animray/point2d.hpp>


namespace animray {


    /** \brief Handles a texture by managing a binary function
    */
    template<
        typename C, typename F,
        typename L = point2d< typename F::argument_type >
    > class texture {
        F function;
        public:
            /// The type of the functor
            typedef F functor_type;
            /// The colour type that is returned by the texture
            typedef C color_type;
            /// Describes where on the texture the colour come from
            typedef L location_type;

            /// Construct a texture from a function
            texture( functor_type f, color_type base_color )
            : function( f ), base_color( base_color ) {
            }

            /// The base colour is used outside the function's range
            fostlib::accessors< color_type > base_color;

            /// Returns the colour for the specified location
            color_type operator () ( const location_type &location ) const {
                return base_color();
            }
    };


    /// A wrapper class for a function pointer that can be used as a texture
    template< typename R, typename A >
    class texture_binop_wrapper {
        R (*function)( A, A );
        public:
            typedef A argument_type;
            typedef R result_type;
            typedef result_type (*function_type)(
                argument_type,argument_type
            );

            texture_binop_wrapper( function_type f )
            : function( f ) {
            }
    };
}


#endif // ANIMRAY_TEXTURE_HPP
