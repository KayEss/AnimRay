/*
    Copyright 2014, Kirit Saelensminde.
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


#ifndef ANIMRAY_PANEL_HPP
#define ANIMRAY_PANEL_HPP
#pragma once


namespace animray {


    /// A panel is part of a film
    template< typename F >
    class panel {
        F inner_film;
    public:
        /// The extents size type
        typedef typename F::size_type size_type;

        /// Default constructor
        panel() {}

        /// Construct a panel from a size, offset and lambda
        template< typename Fn >
        panel(size_type x, size_type y, size_type ox, size_type oy, Fn fn)
        : inner_film(x, y, [ox, oy, &fn] (size_type x, size_type y) {
                return fn(x + ox, y + oy);
            }) {}

        /// Return a non-mutable row from the inner film
        const typename F::column_type &operator [] (const size_type c) const {
            return inner_film[c];
        }
    };


}


#endif // ANIMRAY_PANEL_HPP
