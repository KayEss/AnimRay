/**
    Copyright 1995-2021, [Kirit Saelensminde](https://kirit.com/AnimRay)

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

#include <cmath>


namespace animray {


    /// A unit vector shows direction
    template<typename D>
    class unit_vector;


    /// Homogeneous coordinates store a location or a vector
    template<typename D>
    class point3d : protected detail::array_based<D, 4> {
      protected:
        typedef detail::array_based<D, 4> superclass;

        /// Allow this class to construct from the superclass
        point3d(const superclass &sc) : superclass(sc) {}

      public:
        using value_type = typename superclass::value_type;
        using array_type = typename superclass::array_type;
        using const_value_parameter_type =
                typename superclass::const_value_parameter_type;
        static const std::size_t c_array_size = superclass::c_array_size;
        using superclass::print_on;

        /// Return the 4 underlying coordinates
        const array_type &array() const { return superclass::array; }

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
        constexpr point3d() { superclass::array[3] = 1; }
        /// Constructor for making a point at a given location
        constexpr point3d(
                const_value_parameter_type x,
                const_value_parameter_type y,
                const_value_parameter_type z,
                const_value_parameter_type h = 1) {
            superclass::array[0] = x;
            superclass::array[1] = y;
            superclass::array[2] = z;
            superclass::array[3] = h;
        }
        /// Constructor taking a unit_vector
        explicit point3d(const unit_vector<value_type> &);

        /// Compare for equality
        bool operator==(const point3d &r) const {
            return x() == r.x() && y() == r.y() && z() == r.z();
        }
        /// Compare for inequality
        bool operator!=(const point3d &r) const {
            return x() != r.x() || y() != r.y() || z() != r.z();
        }

        /// Binary subtraction
        point3d operator-(const point3d &r) const {
            return point3d(x() - r.x(), y() - r.y(), z() - r.z());
        }

        /// Binary addition
        point3d operator+(const point3d &r) const {
            return point3d(x() + r.x(), y() + r.y(), z() + r.z());
        }

        /// Multiply by a scalar
        point3d operator*(const value_type s) const {
            return point3d(
                    superclass::array[0], superclass::array[1],
                    superclass::array[2], superclass::array[3] / s);
        }
        point3d operator/(value_type const s) const {
            return point3d(
                    superclass::array[0], superclass::array[1],
                    superclass::array[2], superclass::array[3] * s);
        }
        /// Multiply by another point
        point3d operator*(const point3d &r) const {
            return point3d(superclass::operator*(r));
        }

        /// Unary minus
        point3d operator-() const { return (*this) * -1; }

        /// Return the homogeneous with unit length
        unit_vector<value_type> unit() const {
            return unit_vector<value_type>(
                    superclass::array[0], superclass::array[1],
                    superclass::array[2], magnitude());
        }

        /// The dot product of the location as vector with itself
        value_type dot() const {
            return (superclass::array[0] * superclass::array[0]
                    + superclass::array[1] * superclass::array[1]
                    + superclass::array[2] * superclass::array[2])
                    / (superclass::array[3] * superclass::array[3]);
        }
        /// The length of the location as vectro
        value_type magnitude() const { return std::sqrt(dot()); }
    };


    template<typename V>
    point3d(V, V, V) -> point3d<V>;
    template<typename V>
    point3d(V, V, V, V) -> point3d<V>;


}


#endif // ANIMRAY_HOMOGENEOUS_HPP
