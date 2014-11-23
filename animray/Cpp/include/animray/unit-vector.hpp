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


#ifndef ANIMRAY_UNIT_VECTOR_HPP
#define ANIMRAY_UNIT_VECTOR_HPP
#pragma once


#include <animray/point3d.hpp>


namespace animray {


    /// A unit vector shows direction
    template < typename D >
    class unit_vector : private point3d< D > {
        typedef point3d< D > superclass;

        friend class point3d<D>;
    public:
        /// The value type
        typedef D value_type;

        /// Constructs a unit vector pointing along the x axis.
        unit_vector()
        : superclass( 1, 0, 0 ) {
        /// Constructs a unit vector (already normalised)
        unit_vector(value_type x, value_type y, value_type z)
        : superclass(x, y, z) {
        }
        /// Constructs a unit vector from a point relative to the origin
        unit_vector( const point3d< D > &p )
        : superclass( p.unit() ) {
        }

        /// Multiply by a scalar
        point3d<value_type> operator * (D scalar) const {
            return point3d<value_type>(*this) * scalar;
        }
        /// Add a vector
        point3d<value_type> operator + (const point3d<value_type> &r) const {
            return point3d<value_type>(*this) + r;
        }

        /// Compare for equality
        bool operator == ( const unit_vector &v ) const {
            return this->superclass::operator == ( v );
        }
        /// Compare for inequality
        bool operator !=( const unit_vector &v ) const {
            return this->superclass::operator != ( v );
        }

        using superclass::x;
        using superclass::y;
        using superclass::z;
        using superclass::print_on;
    };


    /// Dot product for two unit vectors
    template< typename D >
    D dot(const unit_vector<D> &d1, const unit_vector<D> &d2) {
        return d1.x() * d2.x() + d1.y() * d2.y() + d1.z() * d2.z();
    }
    /// Dot product for unit vectors and point
    template< typename D >
    D dot(const point3d<D> &d1, const unit_vector<D> &d2) {
        return d1.x() * d2.x() + d1.y() * d2.y() + d1.z() * d2.z();
    }


}


template<typename D>
animray::point3d<D>::point3d(const unit_vector<value_type> &v)
: superclass(v) {
}


#endif // ANIMRAY_UNIT_VECTOR_HPP
