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


#include <fost/main>
#include <animray/interpolation/linear.hpp>
#include <animray/targa.hpp>
#include <algorithm>


namespace {
    using colour = animray::rgb<float>;

    constexpr std::size_t const width = 1920, height = 1080;

    constexpr auto const red = colour{255, 0, 0};
    constexpr auto const green = colour{0, 255, 0};
    constexpr auto const blue = colour{0, 0, 255};

    constexpr auto const top_left = red;
    constexpr auto const top_right = red;
    constexpr auto const bottom_left = green;
    constexpr auto const bottom_right = blue;

    template<typename C>
    auto lerp(C start, C end, std::size_t pos, std::size_t limit) {
        return C(
                animray::interpolation::linear(
                        start.array()[0], end.array()[0], pos, limit),
                animray::interpolation::linear(
                        start.array()[1], end.array()[1], pos, limit),
                animray::interpolation::linear(
                        start.array()[2], end.array()[2], pos, limit));
    }
    auto pixel_colour(std::size_t x, std::size_t y) {
        auto const left = lerp(top_left, bottom_left, y, height);
        auto const right = lerp(top_right, bottom_right, y, height);
        return lerp(left, right, x, width);
    }
    uint8_t linear_clamp(float channel) {
        return std::clamp(channel, 0.0f, 255.0f);
    }
    uint8_t non_linear_clamp(float channel) {
        auto const normalised = std::clamp(channel, 0.0f, 255.0f) / 255.0f;
        if (normalised < 0.0031308) {
            return 255.0f * (normalised * 12.92);
        } else {
            return 255.0f
                    * (std::pow(normalised * 1.055, 1.0f / 2.4f) - 0.055f);
        }
    }
}


FSL_MAIN("mix", "AnimRay. Copyright 2010-2020 Kirit Saelensminde")
(fostlib::ostream &out, fostlib::arguments &args) {
    animray::film<animray::rgb<uint8_t>> linear{
            width, height, [](auto x, auto y) {
                auto const pixel = pixel_colour(x, y);
                return animray::rgb<uint8_t>(
                        linear_clamp(pixel.red()), linear_clamp(pixel.green()),
                        linear_clamp(pixel.blue()));
            }};
    animray::targa("mix-linear.tga", linear);
    animray::film<animray::rgb<uint8_t>> non_linear{
            width, height, [](auto x, auto y) {
                auto const pixel = pixel_colour(x, y);
                return animray::rgb<uint8_t>(
                        non_linear_clamp(pixel.red()),
                        non_linear_clamp(pixel.green()),
                        non_linear_clamp(pixel.blue()));
            }};
    animray::targa("mix-non_linear.tga", non_linear);
    return 0;
}
