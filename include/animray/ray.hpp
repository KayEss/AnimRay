/**
    Copyright 1995-2025, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#pragma once


#include <animray/matrix.hpp>
#include <animray/unit-vector.hpp>


namespace animray {


    /// ## Ray
    /**
     * Represents a ray starting at a point and going to infinity along a given
     * vector
     */
    template<typename D, typename F = point3d<D>, typename V = unit_vector<D>>
    class ray {
      public:
        /// ### The value type of the line
        typedef D local_coord_type;
        /// ### The type of the end point
        using end_type = F;
        /// ### Unit vector type describing the direction
        using direction_type = V;

        /// ### Construct a null ray
        constexpr ray() = default;
        /// ### Construct a line between two locations
        constexpr ray(const end_type &from, const end_type &to)
        : from(from), direction(to - from) {}
        /// ### Construct a line from a location in the specified direction
        constexpr ray(const end_type &from, const direction_type &dir)
        : from(from), direction(dir) {}
        /// ### Construct a follow on ray between two locations
        constexpr ray(const ray &, const end_type &from, const end_type &to)
        : from(from), direction(to - from) {}
        /// ### Construct a follow on ray from a location in the specified
        /// direction
        constexpr ray(
                const ray &, const end_type &from, const direction_type &dir)
        : from(from), direction(dir) {}


        /// ### The start of the ray
        end_type from;
        /// ### A unit direction vector
        direction_type direction;


        /// ### Set a point the ray must go through
        void to(const end_type &t) { direction = direction_type{t - from}; }

        /// ### Return a point somewhere along the line
        end_type ends(local_coord_type distance = local_coord_type(1)) const {
            return from + direction * distance;
        }


        /// ### Compare for equality
        friend bool operator==(ray const &, ray const &) = default;

        /// ### Transform a ray by a matrix
        template<typename MD>
        ray &operator*=(const matrix<MD> &right) {
            end_type f(right * from), e(right * ends());
            from = f;
            to(e);
            return *this;
        }

        /// ### Multiply
        template<typename MD>
        ray operator*(const matrix<MD> &right) const {
            ray res(*this);
            res *= right;
            return res;
        }
    };


    template<typename V>
    ray(point3d<V>, point3d<V>) -> ray<V, point3d<V>>;


    /// ### Output to a stream
    template<typename D>
    std::ostream &operator<<(std::ostream &o, animray::ray<D> const &r) {
        return o << r.from << " -> " << r.direction;
    }


}
