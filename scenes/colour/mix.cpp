/**
    Copyright 2020-2021, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#include <algorithm>
#include <animray/color/hsl.hpp>
#include <animray/color/yuv.hpp>
#include <animray/formats/targa.hpp>
#include <animray/interpolation/linear.hpp>
#include <cmath>
#include <fost/main>


namespace {

    constexpr std::size_t const width = 900, height = 900;

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
    template<typename C>
    auto pixel_colour(
            C top_left,
            C top_right,
            C bottom_left,
            C bottom_right,
            std::size_t x,
            std::size_t y) {
        auto const left = lerp(top_left, bottom_left, y, height);
        auto const right = lerp(top_right, bottom_right, y, height);
        return lerp(left, right, x, width);
    }
    uint8_t linear_clamp(float channel) {
        return std::clamp(channel, 0.0f, 255.0f);
    }
    uint8_t non_linear_clamp(float channel, float max) {
        auto const normalised = std::clamp(channel, 0.0f, max) / max;
        if (normalised < 0.0031308f) {
            return 255.0f * (normalised * 12.92f);
        } else {
            return 255.0f
                    * (std::pow(normalised * 1.055f, 1.0f / 2.4f) - 0.055f);
        }
    }
}


FSL_MAIN("mix", "AnimRay. Copyright 2010-2020 Kirit Saelensminde")
(fostlib::ostream &, fostlib::arguments &) {

    /// ## RGB
    using colour = animray::rgb<float>;
    constexpr auto const red = colour{255, 0, 0};
    constexpr auto const magenta = colour{255, 0, 255};
    constexpr auto const green = colour{0, 255, 0};
    constexpr auto const blue = colour{0, 0, 255};

    animray::film<animray::rgb<uint8_t>> linear{
            width, height, [&](auto x, auto y) {
                auto const pixel =
                        pixel_colour(red, magenta, green, blue, x, y);
                return animray::rgb<uint8_t>(
                        linear_clamp(pixel.red()), linear_clamp(pixel.green()),
                        linear_clamp(pixel.blue()));
            }};
    animray::targa("mix-linear.tga", linear);

    animray::film<animray::rgb<uint8_t>> non_linear{
            width, height, [&](auto x, auto y) {
                auto const pixel =
                        pixel_colour(red, magenta, green, blue, x, y);
                return animray::rgb<uint8_t>(
                        non_linear_clamp(pixel.red(), 255),
                        non_linear_clamp(pixel.green(), 255),
                        non_linear_clamp(pixel.blue(), 255));
            }};
    animray::targa("mix-non_linear.tga", non_linear);

    /// ## HSL
    using hslc = animray::hsl<float>;
    constexpr auto const hred = hslc{0.0f, 1.0f, 0.5f};
    constexpr auto const hmagenta = hslc{300.0f, 1.0f, 0.5f};
    constexpr auto const hgreen = hslc{120.0f, 1.0f, 0.5f};
    constexpr auto const hblue = hslc{240.0f, 1.0f, 0.5f};

    animray::film<animray::rgb<uint8_t>> hsl{
            width, height, [&](auto x, auto y) {
                auto const pixel = fostlib::coerce<colour>(
                        pixel_colour(hred, hmagenta, hgreen, hblue, x, y));
                return animray::rgb<uint8_t>(
                        non_linear_clamp(pixel.red(), 1),
                        non_linear_clamp(pixel.green(), 1),
                        non_linear_clamp(pixel.blue(), 1));
            }};
    animray::targa("mix-hsl.tga", hsl);

    /// YUV
    using yuvc = animray::yuv<float>;
    constexpr auto const yred = yuvc{0.0f, -0.5f, 0.5f};
    constexpr auto const ymagenta = yuvc{0.0f, 0.5f, 0.5f};
    constexpr auto const ygreen = yuvc{0.0f, -0.5f, -0.5f};
    constexpr auto const yblue = yuvc{0.0f, 0.5f, -0.5f};

    animray::film<animray::rgb<uint8_t>> yuv{
            width, height, [&](auto x, auto y) {
                auto const pixel = fostlib::coerce<colour>(
                        pixel_colour(yred, ymagenta, ygreen, yblue, x, y));
                return animray::rgb<uint8_t>(
                        non_linear_clamp(pixel.red(), 1),
                        non_linear_clamp(pixel.green(), 1),
                        non_linear_clamp(pixel.blue(), 1));
            }};
    animray::targa("mix-yuv.tga", yuv);

    return 0;
}
