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
    template<typename D>
    class unit_vector : protected point3d<D> {
        typedef point3d<D> superclass;
        friend class point3d<D>;

      public:
        /// The value type
        typedef D value_type;

        /// Constructs a unit vector pointing along the x axis.
        unit_vector() : superclass(0, 0, 1) {}
        /// Constructs a unit vector (already normalised)
        unit_vector(
                const value_type x,
                const value_type y,
                const value_type z,
                const value_type m = value_type(1))
        : superclass(x, y, z, m) {}
        /// Constructs a unit vector from a point relative to the origin
        unit_vector(const point3d<D> &p) : superclass(p.unit()) {}

        /// Multiply by a scalar
        point3d<value_type> operator*(const value_type scalar) const;
        /// Add a vector
        point3d<value_type> operator+(const point3d<value_type> &r) const {
            return point3d<value_type>(*this) + r;
        }

        /// Compare for equality
        bool operator==(const unit_vector &v) const {
            return this->superclass::operator==(v);
        }
        /// Compare for inequality
        bool operator!=(const unit_vector &v) const {
            return this->superclass::operator!=(v);
        }

        /// Unary minus
        unit_vector operator-() const {
            return unit_vector(
                    superclass::superclass::array[0],
                    superclass::superclass::array[1],
                    superclass::superclass::array[2],
                    -superclass::superclass::array[3]);
        }

        using superclass::print_on;
        using superclass::to_json;
        using superclass::x;
        using superclass::y;
        using superclass::z;
    };


}


template<typename D>
animray::point3d<D>::point3d(const unit_vector<value_type> &v)
: superclass(v) {}


template<typename D>
animray::point3d<D> animray::unit_vector<D>::operator*(const D scalar) const {
    return point3d<D>(
            superclass::superclass::array[0] * scalar,
            superclass::superclass::array[1] * scalar,
            superclass::superclass::array[2] * scalar,
            superclass::superclass::array[3]);
}

// template<typename D>
// typename std::enable_if<std::is_floating_point<D>::value,
// animray::point3d<D>>::type
//         animray::unit_vector<D>::operator * (const D scalar) const {
//     return point3d<D>(
//         superclass::superclass::array[0],
//         superclass::superclass::array[1],
//         superclass::superclass::array[2],
//         superclass::superclass::array[3] / scalar);
// }
// template<typename D>
// typename std::enable_if<std::is_integral<D>::value, animray::point3d<D>>::type
//         animray::unit_vector<D>::operator * (const D scalar) const {
//     return point3d<D>(
//         superclass::superclass::array[0] * scalar,
//         superclass::superclass::array[1] * scalar,
//         superclass::superclass::array[2] * scalar,
//         superclass::superclass::array[3]);
// }


#endif // ANIMRAY_UNIT_VECTOR_HPP
