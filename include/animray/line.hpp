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


#ifndef ANIMRAY_LINE_HPP
#define ANIMRAY_LINE_HPP
#pragma once


#include <animray/point3d.hpp>


namespace animray {


    /// Represents part of a line through 3D space
    template<typename D>
    class line {
      public:
        /// The value type of the line
        typedef D value_type;
        /// The type of the end points
        using end_type = point3d<value_type>;

        /// Construct a null line
        constexpr line() = default;
        /// Construct a line between two locations
        constexpr line(const end_type &from, const end_type &to)
        : from(from), to(to) {}

        /// The start of the line
        end_type from;
        /// The end of the line
        end_type to;

        /// Compare for equality
        bool operator==(const line &r) const {
            return to == r.to && from == r.from;
        }
        /// Compare for inequality
        bool operator!=(const line &r) const { return !(*this == r); }

        /// Returns the square of the length of the line
        value_type length_squared() const {
            value_type dx = to.x() - from.x(), dy = to.y() - from.y(),
                       dz = to.z() - from.z();
            return dx * dx + dy * dy + dz * dz;
        }
    };


    template<typename D>
    std::ostream &operator<<(std::ostream &o, const line<D> &l) {
        return o << l.from << " -> " << l.to;
    }


}


#endif // ANIMRAY_LINE_HPP
