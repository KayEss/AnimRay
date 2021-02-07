/**
    Copyright 2020, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#include <animray/detail/array_based.hpp>
#include <animray/color/rgb.hpp>


namespace animray {


    /// YUV colour space. The luma channel can take on any value, and
    /// chroma values are between -0.5 and + 0.5 for floating point types.
    /// For digital use this colour format is properly called YCrCb, but
    /// nobody uses the proper name.
    template<typename D>
    class yuv : private detail::array_based<D, 3> {
        typedef detail::array_based<D, 3> superclass;

      public:
        using value_type = typename superclass::value_type;
        using array_type = typename superclass::array_type;
        using const_value_parameter_type =
                typename superclass::const_value_parameter_type;
        static const std::size_t c_array_size = superclass::c_array_size;
        using superclass::print_on;

        /// Default construct an YUV colour with all channels at zero
        constexpr yuv() = default;
        /// Construct a colour from a luma signal
        explicit constexpr yuv(value_type gray) noexcept {
            superclass::array[0] = gray;
            superclass::array[1] = 0;
            superclass::array[2] = 0;
        }
        /// Construct an YUV colour with the specified channel values
        constexpr yuv(value_type y, value_type u, value_type v) noexcept {
            superclass::array[0] = y;
            superclass::array[1] = u;
            superclass::array[2] = v;
        }
        /// Construct from a 3 valued array
        yuv(superclass &&s) : superclass(s) {}

        /// Return the channel values
        const array_type &array() const noexcept { return superclass::array; }

        /// Return the red channel value
        value_type y() const noexcept { return superclass::array[0]; }
        /// Return the red channel value
        value_type u() const noexcept { return superclass::array[1]; }
        /// Return the red channel value
        value_type v() const noexcept { return superclass::array[2]; }

        /// Compare for equality
        bool operator==(const yuv &r) const {
            return superclass::array == r.superclass::array;
        }
    };


}


namespace fostlib {


    /// Allow conversion from YUV to RGB for float versions. Output RGB range
    /// is [0...1] for each channel, but is un-clamped, so some values may be
    /// out of range. Values are for the HDTV (BT.709) standard
    template<typename D>
    struct coercer<
            animray::rgb<D>,
            animray::yuv<D>,
            std::enable_if_t<std::is_floating_point_v<D>>> {
        animray::rgb<D> coerce(animray::yuv<D> const &yuv) {
            auto const r = yuv.y() + D{1.28033} * yuv.v();
            auto const g =
                    yuv.y() - D{0.21482} * yuv.u() - D{0.38059} * yuv.v();
            auto const b = yuv.y() + D{2.12798} * yuv.u();
            return animray::rgb<D>{r, g, b};
        }
    };


}
