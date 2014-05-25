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


#ifndef ANIMRAY_HOMOGENEOUS_HPP
#define ANIMRAY_HOMOGENEOUS_HPP
#pragma once


#include <animray/detail/array_based.hpp>


#ifdef _MSC_VER
namespace std {
    template< typename I >
    typename boost::enable_if< boost::is_integral< I >, I >::type sqrt( I i ) {
        return I( sqrt( double(i) ) );
    }
}
#endif


namespace animray {


    /// Homogeneous coordinates store a location or a vector
    template < typename D >
    class point3d : private detail::array_based< D, 4 > {
        typedef detail::array_based< D, 4 > superclass;
        public:
            typedef typename superclass::value_type value_type;
            typedef typename superclass::array_type array_type;
            typedef typename
                superclass::const_value_parameter_type
                const_value_parameter_type
            ;
            static const std::size_t c_array_size = superclass::c_array_size;
            using superclass::to_json;
            using superclass::print_on;

            /// Return the 4 underlying coordinates
            const array_type &array() const {
                return superclass::array;
            }

            /// The x coordinate
            value_type x() const {
                return superclass::array[0] / superclass::array[3];
            }
            /// The y coordinate
            value_type y() const {
                return superclass::array[1] / superclass::array[3];
            }
            /// The z coordinate
            value_type z() const {
                return superclass::array[2] / superclass::array[3];
            }

            /// Constructor makes a point at the origin
            point3d() {
                superclass::array.c_array()[3] = 1;
            }
            /// Constructor for making a point at a given location
            point3d(
                const_value_parameter_type x,
                const_value_parameter_type y,
                const_value_parameter_type z,
                const_value_parameter_type h = 1
            ) {
                superclass::array.c_array()[0] = x;
                superclass::array.c_array()[1] = y;
                superclass::array.c_array()[2] = z;
                superclass::array.c_array()[3] = h;
            }

            /// Compare for equality
            bool operator == ( const point3d &r ) const {
                return superclass::array == r.superclass::array;
            }
            /// Compare for inequality
            bool operator != ( const point3d &r ) const {
                return superclass::array != r.superclass::array;
            }

            /// Binary subtraction
            point3d operator - ( const point3d &r ) const {
                return point3d( x() - r.x(), y() - r.y(), z() - r.z() );
            }

            /// Binary addition
            point3d operator + ( const point3d &r ) const {
                return point3d( x() + r.x(), y() + r.y(), z() + r.z() );
            }

            /// Return the homogeneous with unit length
            point3d unit() const {
                return point3d(
                    superclass::array[0], superclass::array[1], superclass::array[2],
                    std::sqrt(dot(*this)));
            }

            /// The dot product of two homogeneous co-ordinates
            D dot( const point3d &r ) const {
                return (
                    superclass::array[0] * r.superclass::array[0]
                    + superclass::array[1] * r.superclass::array[1]
                    + superclass::array[2] * r.superclass::array[2]
                ) / (superclass::array[3] * r.superclass::array[3]);
            }
    };


}


#endif // ANIMRAY_HOMOGENEOUS_HPP
