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


    /// A texture which is a single solid colour
    template<
        typename C
    > class solid_color_texture {
        public:
            /// Colour type stored in the texture
            typedef C color_type;

            /// Construct a texture functor
            solid_color_texture( const color_type &c )
            : color( c ) {
            }

            /// Stores the colour
            fostlib::accessors< color_type > color;

            /// Return the color
            color_type operator () () const {
                return color();
            }
    };


    /// Colour conversion functor that forwards to coerce
    template< typename T, typename F >
    struct color_coercer {
        T operator () ( const F &f ) const {
            return fostlib::coerce< T >( f );
        }
    };


    /// Location mapping functor that throws away the location
    template< typename F, typename L >
    struct location_mapper_no_arguments {
        typedef typename F::color_type color_type;
        color_type operator() ( const F &f, const L & ) const {
            return f();
        }
    };
    /// A location mapper that splits the x and y for a binary functor
    template< typename F, typename L >
    struct location_mapper_binary_op {
        typename F::color_type operator () ( const F &f, const L &l ) const {
            return f( l.x(), l.y() );
        }
    };


    /** \brief Handles a texture by managing a binary function
    */
    template<
        typename C, typename L,
        typename F = solid_color_texture< C >,
        typename LM = location_mapper_no_arguments< F, L >,
        typename CC = color_coercer< C, typename F::color_type >
    > class texture {
        F function;
        CC color_converter;
        LM location_converter;
        public:
            /// The location type used externally to the texture
            typedef L location_type;
            /// The type of the functor
            typedef F functor_type;
            /// The colour type that is returned by the texture
            typedef C color_type;
            /// The type of color conversion functor
            typedef CC color_conversion_functor_type;
            /// The type of the location conversion functor
            typedef LM location_mapping_functor_type;

            /// Construct a texture from a function
            texture( functor_type f )
            : function( f ) {
            }

            /// Return the color at the specified location
            color_type operator () ( const location_type &location ) const {
                return color_converter(location_converter(function, location));
            }
    };


    /// A wrapper class for a function pointer that can be used as a texture
    template< typename R, typename A >
    class texture_binop_wrapper {
        R (*function)( A, A );
        public:
            typedef A argument_type;
            typedef R color_type;
            typedef color_type (*function_type)(
                argument_type,argument_type
            );

            texture_binop_wrapper( function_type f )
            : function( f ) {
            }

            R operator () ( A x, A y ) const {
                return function(x, y);
            }
    };
}


#endif // ANIMRAY_TEXTURE_HPP
