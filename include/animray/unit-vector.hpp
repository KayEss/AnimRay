/**
    Copyright 1995-2021, [Kirit Saelensminde](https://kirit.com/AnimRay).

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
        using value_type = D;

        /// Constructs a unit vector pointing along the x axis.
        unit_vector() : superclass(0, 0, 1) {}
        /// Constructs a unit vector (already normalised)
        unit_vector(
                value_type const x,
                value_type const y,
                value_type const z,
                value_type const m = value_type{1})
        : superclass(x, y, z, m) {}
        /// Constructs a unit vector from a point relative to the origin
        explicit unit_vector(point3d<D> const &p) : superclass(p.unit()) {}

        /// Multiply by a scalar
        point3d<value_type> operator*(value_type scalar) const;
        /// Add a vector
        point3d<value_type> operator+(point3d<value_type> const &r) const {
            return point3d<value_type>(*this) + r;
        }

        /// Comparisons
        bool operator<=>(unit_vector const &) const = default;

        /// Unary minus
        unit_vector operator-() const {
            return unit_vector(
                    superclass::superclass::array[0],
                    superclass::superclass::array[1],
                    superclass::superclass::array[2],
                    -superclass::superclass::array[3]);
        }

        using superclass::print_on;
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


#endif // ANIMRAY_UNIT_VECTOR_HPP
