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


#ifndef ANIMRAY_RAY_HPP
#define ANIMRAY_RAY_HPP
#pragma once


#include <animray/matrix.hpp>
#include <animray/unit-vector.hpp>


namespace animray {


    /// Represents a ray starting at a point and going to infinity along a given vector
    template< typename D >
    class ray {
    public:
        /// The value type of the line
        typedef D local_coord_type;
        /// The type of the end point
        typedef point3d< local_coord_type > end_type;
        /// Unit vector type describing the direction
        typedef unit_vector< local_coord_type > direction_type;

        /// Construct a null ray
        ray() {
        }
        /// Construct a line between two locations
        ray( const end_type &from, const end_type &to )
        : from(from), direction(to - from) {
        }
        /// Construct a line from a location in the specified direction
        ray( const end_type &from, const direction_type &dir )
        : from(from), direction(dir) {
        }

        /// The start of the ray
        fostlib::accessors< end_type > from;
        /// A unit direction vector
        fostlib::accessors< direction_type > direction;

        /// Return a point somewhere along the line
        end_type ends(local_coord_type distance = local_coord_type(1)) const {
            return from() + direction() * distance;
        }

        /// Compare for equality
        bool operator == ( const ray &r ) const {
            return direction() == r.direction() && from() == r.from();
        }
        /// Compare for inequality
        bool operator != ( const ray &r ) const {
            return ! ( *this == r );
        }

        /// Transform a ray by a matrix
        template<typename MD>
        ray operator * (const matrix<MD> &right) const {
            return ray(right * from(), right * ends());
        }
    };


}


namespace std {


    /// Output to a stream
    template<typename D>
    ostream &operator << (ostream &o, const animray::ray<D> &r) {
        return o << r.from() << " -> " << r.direction();
    }


}


#endif // ANIMRAY_RAY_HPP
