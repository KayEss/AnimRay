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


#ifndef ANIMRAY_EXTENTS2D_HPP
#define ANIMRAY_EXTENTS2D_HPP
#pragma once


#include <animray/point2d.hpp>


namespace animray {


    /// The size for integral types -- includes both values in the range
    template<typename S>
    inline typename boost::enable_if<boost::is_integral<S>, S>::type
            size(S min, S max) {
        return max - min + 1;
    }
    /// The size for continuous types
    template<typename S>
    inline typename boost::enable_if<boost::is_floating_point<S>, S>::type
            size(S min, S max) {
        return max - min;
    }


    /// Extents that describe part of the area of a film
    template<typename S>
    class extents2d {
      public:
        /// The size type of the extents
        typedef S size_type;
        /// The corner type
        using corner_type = point2d<size_type>;

        /// A corner of the extents
        corner_type lower_left;
        /// A corner of the extents
        corner_type top_right;

        /// Construct an empty extents
        extents2d() : lower_left(), top_right() {}
        /// Construct an extents from the
        extents2d(size_type sx, size_type sy, size_type ex, size_type ey)
        : lower_left(corner_type(sx, sy)), top_right(corner_type(ex, ey)) {
            if (lower_left.x > top_right.x)
                throw fostlib::exceptions::out_of_range<size_type>(
                        "Top right for x is less than lower left for x",
                        lower_left.x, std::numeric_limits<size_type>::max(),
                        top_right.x);
            if (lower_left.y > top_right.y)
                throw fostlib::exceptions::out_of_range<size_type>(
                        "Top right for y is less than lower left for y",
                        lower_left.y, std::numeric_limits<size_type>::max(),
                        top_right.y);
        }

        /// Calculate the height of the extents
        size_type width() const {
            return animray::size<size_type>(lower_left.x, top_right.x);
        }
        /// Calculate the width of the extents
        size_type height() const {
            return animray::size<size_type>(lower_left.y, top_right.y);
        }
        /// Calculate the area of the extents
        size_type area() const { return width() * height(); }

        /// Check for equality
        bool operator==(const extents2d &r) const {
            return lower_left == r.lower_left && top_right == r.top_right;
        }
        /// Check for inequality
        bool operator!=(const extents2d &r) const {
            return lower_left != r.lower_left || top_right != r.top_right;
        }

        /// Return the intersection between this extents and another one
        fostlib::nullable<extents2d> intersection(const extents2d &r) {
            size_type lx = std::max(lower_left.x, r.lower_left.x);
            size_type ly = std::max(lower_left.y, r.lower_left.y);
            size_type ux = std::min(top_right.x, r.top_right.x);
            size_type uy = std::min(top_right.y, r.top_right.y);
            if (lx > ux || ly > uy) {
                return fostlib::null;
            } else {
                return extents2d(lx, ly, ux, uy);
            }
        }
    };


}


namespace fostlib {
    /// Allow extents to be coerced between different numeric types
    template<typename T, typename F>
    struct coercer<animray::extents2d<T>, animray::extents2d<F>> {
        animray::extents2d<T> coerce(const animray::extents2d<F> &e) {
            return animray::extents2d<T>(
                    fostlib::coerce<T>(e.lower_left.x),
                    fostlib::coerce<T>(e.lower_left.y),
                    fostlib::coerce<T>(e.top_right.x),
                    fostlib::coerce<T>(e.top_right.y));
        }
    };
}


namespace std {
    /// Output an extents to a stream
    template<typename S>
    inline fostlib::ostream &
            operator<<(fostlib::ostream &o, const animray::extents2d<S> &e) {
        return o << "[ " << e.lower_left << " -> " << e.top_right << " ]";
    }
}


#endif // ANIMRAY_EXTENTS2D_HPP
