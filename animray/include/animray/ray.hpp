/**
    Copyright 1995-2020, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


    /// Represents a ray starting at a point and going to infinity along a given
    /// vector
    template<typename D, typename F = point3d<D>, typename V = unit_vector<D>>
    class ray {
      public:
        /// The value type of the line
        typedef D local_coord_type;
        /// The type of the end point
        typedef F end_type;
        /// Unit vector type describing the direction
        typedef V direction_type;

        /// Construct a null ray
        ray() {}
        /// Construct a line between two locations
        ray(const end_type &from, const end_type &to)
        : from(from), direction(to - from) {}
        /// Construct a line from a location in the specified direction
        ray(const end_type &from, const direction_type &dir)
        : from(from), direction(dir) {}
        /// Construct a follow on ray between two locations
        ray(const ray &, const end_type &from, const end_type &to)
        : from(from), direction(to - from) {}
        /// Construct a follow on ray from a location in the specified direction
        ray(const ray &, const end_type &from, const direction_type &dir)
        : from(from), direction(dir) {}

        /// The start of the ray
        end_type from;
        /// A unit direction vector
        direction_type direction;

        /// Set a point the ray must go through
        void to(const end_type &t) { direction = direction_type{t - from}; }

        /// Return a point somewhere along the line
        end_type ends(local_coord_type distance = local_coord_type(1)) const {
            return from + direction * distance;
        }

        /// Compare for equality
        bool operator==(const ray &r) const {
            return direction == r.direction && from == r.from;
        }
        /// Compare for inequality
        bool operator!=(const ray &r) const {
            return direction != r.direction || from != r.from;
        }

        /// Transform a ray by a matrix
        template<typename MD>
        ray &operator*=(const matrix<MD> &right) {
            end_type f(right * from), e(right * ends());
            from = f;
            to(e);
            return *this;
        }

        /// Multiply
        template<typename MD>
        ray operator*(const matrix<MD> &right) const {
            ray res(*this);
            res *= right;
            return res;
        }
    };


}


namespace fostlib {
    /// Coerce a ray to JSON
    template<typename D>
    struct coercer<json, animray::ray<D>> {
        json coerce(const animray::ray<D> &l) {
            json r;
            jcursor("from").insert(r, fostlib::coerce<json>(l.from));
            jcursor("direction").insert(r, fostlib::coerce<json>(l.direction));
            return r;
        }
    };
}


namespace std {


    /// Output to a stream
    template<typename D>
    ostream &operator<<(ostream &o, const animray::ray<D> &r) {
        return o << r.from << " -> " << r.direction;
    }


}


#endif // ANIMRAY_RAY_HPP
