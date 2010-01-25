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


#ifndef ANIMRAY_UNIT_VECTOR_HPP
#define ANIMRAY_UNIT_VECTOR_HPP
#pragma once


#include <animray/point3d.hpp>


namespace animray {


    /// A unit vector shows direction
    template < typename D >
    class unit_vector : private point3d< D > {
        typedef point3d< D > superclass_type;
        public:
            /// Constructs a unit vector pointing along the x axis.
            unit_vector()
            : superclass_type( 1, 0, 0 ) {
            }
            /// Constructs a unit vector from a point relative to the origin
            unit_vector( const point3d< D > &p )
            : superclass_type( p.unit() ) {
            }

            /// Compare for equality
            bool operator == ( const unit_vector &v ) const {
                return this->superclass_type::operator == ( v );
            }
            /// Compare for inequality
            bool operator !=( const unit_vector &v ) const {
                return this->superclass_type::operator != ( v );
            }

            using superclass_type::x;
            using superclass_type::y;
            using superclass_type::z;
    };


}


#endif // ANIMRAY_UNIT_VECTOR_HPP
