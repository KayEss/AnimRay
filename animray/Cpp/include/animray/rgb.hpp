/*
    Copyright 1995-2014, Kirit Saelensminde.
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
                const_value_parameter_type;
        static const std::size_t c_array_size = superclass::c_array_size;
        using superclass::print_on;
        using superclass::to_json;

        /// Default construct an RGB colour with all channels at zero
        rgb() {}
        rgb( value_type gray ) {
            superclass::array[0] = gray;
            superclass::array[1] = gray;
            superclass::array[2] = gray;
        }
        /// Construct an RGB colour with the specified channel values
        rgb( value_type r, value_type g, value_type b ) {
            superclass::array[0] = r;
            superclass::array[1] = g;
            superclass::array[2] = b;
        }
        /// Construct from a 3 valued array
        rgb(superclass &&s)
        : superclass(s) {
        }

        /// Return the channel values
        const array_type &array() const {
            return superclass::array;
        }

        /// Return the red channel value
        value_type red() const {
            return superclass::array[0];
        }
        /// Return the red channel value
        value_type green() const {
            return superclass::array[1];
        }
        /// Return the red channel value
        value_type blue() const {
            return superclass::array[2];
        }

        /// Compare for equality
        bool operator == ( const rgb &r ) const {
            return superclass::array == r.superclass::array;
        }
        /// Compare for inequality
        bool operator != ( const rgb &r ) const {
            return superclass::array != r.superclass::array;
        }

        /// Add the same value to each channel
        rgb operator + (value_type gray) const {
            return rgb(superclass::operator +(gray));
        }
        /// Add two colour values together
        rgb operator + (const rgb &r) {
            return rgb(red() + r.red(), green() + r.green(), blue() + r.blue());
        }

        /// Multiply the channel values by a scalar
        template<typename S>
        rgb operator * (const S weight) const {
            return rgb(superclass::operator *(weight));
        }
    };


    /// Add a value to each channel
    template<typename D> inline
    rgb<D> operator + (const D d, const rgb<D> &c) {
        return c + d;
    }


}


#endif // ANIMRAY_RGB_HPP
