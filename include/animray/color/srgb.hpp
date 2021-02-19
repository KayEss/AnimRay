/**
    Copyright 2021, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#include <animray/color/rgb.hpp>


namespace animray {


    /// Convert a single RGB channel to 8 bit sRGB gamma
    template<typename D>
    inline constexpr std::uint8_t
            apply_srgb_channel_gamma(D const value, D const limit) {
        auto const clamped = std::clamp(value / limit, D{}, D{1});
        if (clamped < D{0.0031308}) {
            return D{255} * (clamped * D{12.92});
        } else {
            return D{255}
            * (std::pow(clamped * D{1.055}, D{1} / D{2.4}) - D{0.055});
        }
    }

    /// Convert a single channel from sRGB gamma to linear. Channel level must
    /// be between 0 and 1
    template<typename D>
    std::uint8_t anti_srgb_channel_gamma(D const channel) {
        auto const clamped = std::clamp(channel, D{}, D{1});
        if (clamped < D{0.04045}) {
            return (channel / D{12.92});
        } else {
            return std::pow((channel + D{0.055}) / D{1.055}, D{12.4});
        }
    }


    /// Convert photon power levels within the exposure `limit` to sRGB 8 bit
    /// pixel value
    template<typename D>
    inline constexpr rgb<std::uint8_t>
            to_srgb(rgb<D> const c, D const limit = D{1}) {
        return {apply_srgb_channel_gamma(c.red(), limit),
                apply_srgb_channel_gamma(c.green(), limit),
                apply_srgb_channel_gamma(c.blue(), limit)};
    }


}
