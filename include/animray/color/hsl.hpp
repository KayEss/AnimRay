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


#ifndef ANIMRAY_HLS_HPP
#define ANIMRAY_HLS_HPP
#pragma once


#include <animray/color/concept.hpp>
#include <animray/color/rgb.hpp>


namespace animray {


    /// Represents the normal 3 channel HLS colour space
    template<typename D>
    class hsl : private detail::array_based<D, 3> {
        typedef detail::array_based<D, 3> superclass;

      public:
        /// The value type
        using value_type = typename superclass::value_type;
        /// The array type
        using array_type = typename superclass::array_type;
        /// The type of a value parameter
        using const_value_parameter_type =
                typename superclass::const_value_parameter_type;
        /// The size of the array
        static const std::size_t c_array_size = superclass::c_array_size;

        using superclass::print_on;

        /// Default construct an HLS colour with all channels at zero
        constexpr hsl() noexcept = default;
        /// Construct an HLS colour with the specified channel values
        constexpr hsl(value_type h, value_type s, value_type l) noexcept {
            superclass::array[0] = h;
            superclass::array[1] = s;
            superclass::array[2] = l;
        }

        /// Return the channel values
        constexpr const array_type &array() const noexcept {
            return superclass::array;
        }

        /// Compare for equality
        bool operator==(hsl const &r) const {
            return superclass::array == r.superclass::array;
        }
        /// Compare for inequality
        bool operator!=(hsl const &r) const {
            return superclass::array != r.superclass::array;
        }
    };


    /// Allow conversion from HLS to RGB for float versions
    template<typename D>
    struct detail::color_conversion<rgb<D>, hsl<D>> {
        auto convert(hsl<D> c) {
            /// Performs the coercion
            const D h = c.array()[0], s = c.array()[1], l = c.array()[2];
            const D C = l <= D(0.5) ? D(2) * l * s : (D(2) - D(2) * l) * s;
            const D H = h / D(60);
            D Hmod2 = H;
            while (Hmod2 > D(2)) Hmod2 -= D(2);
            const D X = C * (D(1) - std::abs(Hmod2 - D(1)));
            D r, g, b;
            if (H < D(1)) {
                r = C;
                g = X;
                b = D(0);
            } else if (H < D(2)) {
                r = X;
                g = C;
                b = D(0);
            } else if (H < D(3)) {
                r = D(0);
                g = C;
                b = X;
            } else if (H < D(4)) {
                r = D(0);
                g = X;
                b = C;
            } else if (H < D(5)) {
                r = X;
                g = D(0);
                b = C;
            } else {
                r = C;
                g = D(0);
                b = X;
            }
            D const m = l - D(0.5) * C;
            return rgb<D>(r + m, g + m, b + m);
        }
    };


}


#endif // ANIMRAY_HLS_HPP
