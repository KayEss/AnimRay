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


#ifndef ANIMRAY_MATRIX_HPP
#define ANIMRAY_MATRIX_HPP
#pragma once


#include <animray/point3d.hpp>


namespace animray {


    /// This 4D matrix class represents transformations in 3D space
    template< typename D >
    class matrix : private detail::array_based< D, 16 > {
        typedef detail::array_based< D, 16 > superclass;
        public:
            typedef typename superclass::value_type value_type;
            typedef typename superclass::array_type array_type;
            typedef typename
                superclass::const_value_parameter_type
                const_value_parameter_type
            ;

            /// Construct a transform matrix that does nothing
            matrix() {
                // Set the values on the leading diagonal to 1
                superclass::array[0] = 1;
                superclass::array[5] = 1;
                superclass::array[10] = 1;
                superclass::array[15] = 1;
            }

            /// Compare for equality
            bool operator == ( const matrix &r ) const {
                return superclass::array == r.array;
            }
            /// Compare for inequality
            bool operator != ( const matrix &r ) const {
                return superclass::array != r.array;
            }

            fostlib::json to_json() const {
                fostlib::json ret, line;
                for ( std::size_t i = 0; i < 16; ++ i ) {
                    fostlib::jcursor().push_back( line, superclass::array[i] );
                    if ( ( i + 1 ) % 4 == 0 ) {
                        fostlib::jcursor().push_back(ret, line);
                        line = fostlib::json();
                    }
                }
                return ret;
            }
    };


}


#endif // ANIMRAY_MATRIX_HPP
