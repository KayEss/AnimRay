/*
    Copyright 2018-2019, [Kirit Saelensminde](https://kirit.com/AnimRay).

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

#include <algorithm>


namespace animray {


    template<typename C = uint8_t>
    class luma {
        C _luma;

      public:
        using value_type = C;

        luma() : _luma{} {}
        explicit luma(value_type f) : _luma(f) {}
        template<typename V>
        explicit luma(V v)
        : _luma{C(std::clamp<V>(
                v,
                std::numeric_limits<C>::min(),
                std::numeric_limits<C>::max()))} {}

        operator C() const { return _luma; }

        /// Saturating addition which clamps at the numeric limit
        luma operator+(luma r) const {
            if (_luma > std::numeric_limits<C>::max() - r._luma) {
                return luma{std::numeric_limits<C>::max()};
            } else {
                return luma{_luma + r._luma};
            }
        }
    };


}
