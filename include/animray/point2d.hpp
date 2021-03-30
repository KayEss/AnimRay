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


#ifndef ANIMRAY_POINT2D_HPP
#define ANIMRAY_POINT2D_HPP
#pragma once


#include <ostream>


namespace animray {


    /// A 2d point
    template<typename C>
    class point2d {
      public:
        /// The type of the location values
        using value_type = C;

        /// The x location
        value_type x{};
        /// The y location
        value_type y{};

        /// Construct an empty point
        point2d() {}
        /// Construct a point from a pair of co-ordinates
        point2d(C x, C y) : x(x), y(y) {}

        /// Check for equality
        bool operator==(const point2d &r) const { return x == r.x && y == r.y; }
        /// Check for inequality
        bool operator!=(const point2d &r) const { return x != r.x || y != r.y; }

        /// Add another point to this one
        point2d &operator+=(const point2d &r) {
            x += r.x;
            y += r.y;
            return *this;
        }

        /// Scale a point by a scalar
        point2d &operator*=(const C &r) {
            x *= r;
            y *= r;
            return *this;
        }
    };


    /// Allow two points to be added together
    template<typename C>
    inline auto operator+(const point2d<C> &a, const point2d<C> &b) {
        return point2d<C>(a.x + b.x, a.y + b.y);
    }
    /// Allow two points to be subracted
    template<typename C>
    inline auto operator-(const point2d<C> &a, const point2d<C> &b) {
        return point2d<C>(a.x - b.x, a.y - b.y);
    }
    /// Allow us to scale a point location by a scalar
    template<typename C>
    inline auto operator*(const C &a, const point2d<C> &b) {
        return point2d<C>(a * b.x, a * b.y);
    }
    /// Allow us to scale a point location by a scalar
    template<typename C>
    inline auto operator*(const point2d<C> &a, const C &b) {
        return point2d<C>(a.x * b, a.y * b);
    }
    /// Allow us to divide a point location by a scalar
    template<typename C>
    inline auto operator/(const C &a, const point2d<C> &b) {
        return point2d<C>(a / b.x, a / b.y);
    }
    /// Allow us to divide a point location by a scalar
    template<typename C>
    inline auto operator/(const point2d<C> &a, const C &b) {
        return point2d<C>(a.x / b, a.y / b);
    }

    /// Allow 2d points to be displayed on a stream
    template<typename C>
    inline std::ostream &operator<<(std::ostream &o, point2d<C> const &p) {
        return o << "(" << p.x << ", " << p.y << ")";
    }

}


#endif // ANIMRAY_POINT2D_HPP
