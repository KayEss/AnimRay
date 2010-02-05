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


#ifndef ANIMRAY_RGB_HPP
#define ANIMRAY_RGB_HPP
#pragma once


#include <animray/detail/array_based.hpp>


namespace animray {


    /// Represents the normal 3 channel RGB colour space
    template< typename D >
    class rgb : private detail::array_based< D, 3 > {
        typedef detail::array_based< D, 3 > superclass;
        public:
            typedef typename superclass::value_type value_type;
            typedef typename superclass::array_type array_type;
            typedef typename
                superclass::const_value_parameter_type
                const_value_parameter_type
            ;
            using superclass::c_array_size;
            using superclass::print_on;
            using superclass::to_json;

            /// Default construct an RGB colour with all channels at zero
            rgb() {
            }
            /// Construct an RGB colour with the specified channel values
            rgb( value_type r, value_type g, value_type b ) {
                superclass::array[0] = r;
                superclass::array[1] = g;
                superclass::array[2] = b;
            }

            /// Return the channel values
            const array_type &array() const {
                return superclass::array;
            }

            /// Compare for equality
            bool operator == ( const rgb &r ) const {
                return superclass::array == r.superclass::array;
            }
            /// Compare for inequality
            bool operator != ( const rgb &r ) const {
                return superclass::array != r.superclass::array;
            }
    };


}


#endif // ANIMRAY_RGB_HPP
