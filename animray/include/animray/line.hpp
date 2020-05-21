/*
    Copyright 1995-2020, Kirit Saelensminde.
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
        typedef point3d<value_type> end_type;

        /// Construct a null line
        line() {}
        /// Construct a line between two locations
        line(const end_type &from, const end_type &to) : from(from), to(to) {}

        /// The start of the line
        fostlib::accessors<end_type> from;
        /// The end of the line
        fostlib::accessors<end_type> to;

        /// Compare for equality
        bool operator==(const line &r) const {
            return to() == r.to() && from() == r.from();
        }
        /// Compare for inequality
        bool operator!=(const line &r) const { return !(*this == r); }

        /// Returns the square of the length of the line
        value_type length_squared() const {
            value_type dx = to().x() - from().x(), dy = to().y() - from().y(),
                       dz = to().z() - from().z();
            return dx * dx + dy * dy + dz * dz;
        }
    };


}


namespace fostlib {
    /// Coerce a line to JSON
    template<typename D>
    struct coercer<json, animray::line<D>> {
        json coerce(const animray::line<D> &l) {
            json r;
            jcursor("from").insert(r, fostlib::coerce<json>(l.from()));
            jcursor("to").insert(r, fostlib::coerce<json>(l.to()));
            return r;
        }
    };
    /// Coerce a line from JSON
    template<typename D>
    struct coercer<animray::line<D>, json> {
        animray::line<D> coerce(const json &js) {
            return animray::line<D>(
                    fostlib::coerce<typename animray::line<D>::end_type>(
                            js["from"]),
                    fostlib::coerce<typename animray::line<D>::end_type>(
                            js["to"]));
        }
    };
}


namespace std {
    /// Allow the line to be displayed
    template<typename D>
    fostlib::ostream &
            operator<<(fostlib::ostream &o, const animray::line<D> &l) {
        return o << l.from() << " -> " << l.to();
    }
}


#endif // ANIMRAY_LINE_HPP
