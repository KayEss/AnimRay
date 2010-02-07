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


#include <fost/functor>
#include <animray/point2d.hpp>
#include <animray/detail/texture-policy.hpp>


namespace animray {


    /// Handles a texture by managing generating and mapping functions
    template<
        typename C, typename L,
        typename F = fostlib::functor::const_value< C >,
        typename P = texture_policy< C, L, F >
    > class texture {
        F function;
        typename P::color_conversion_functor_type color_converter;
        typename P::location_mapping_functor_type location_converter;
        public:
            /// The policy which controls the texture implementation
            typedef P policy_type;
            /// The colour type that is returned by the texture
            typedef typename P::color_type color_type;
            /// The location type used externally to the texture
            typedef typename P::location_type location_type;
            /// The type of the functor
            typedef typename P::functor_type functor_type;
            /// The type of color conversion functor
            typedef typename P::color_conversion_functor_type \
                color_conversion_functor_type;
            /// The type of the location conversion functor
            typedef typename P::location_mapping_functor_type \
                location_mapping_functor_type;
            /// The type of the first argument to the constructor (which creates the functor)
            typedef typename P::texture_constructor_arg1_type constructor_arg1_type;

            /// Construct a texture from a function
            texture( constructor_arg1_type f )
            : function( f ) {
            }

            /// Return the color at the specified location
            color_type operator () ( const location_type &location ) const {
                return color_converter(location_converter(function, location));
            }
    };


}


#endif // ANIMRAY_TEXTURE_HPP
