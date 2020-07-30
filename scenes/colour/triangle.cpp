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
    constexpr auto const top_left = colour{255, 0, 0};
    constexpr auto const top_right = colour{255, 0, 0};
    constexpr auto const bottom_left = colour{0, 255, 0};
    constexpr auto const bottom_right = colour{0, 0, 255};

    auto lerp(colour start, colour end, std::size_t pos, std::size_t limit) {
        return colour(
                animray::interpolation::linear(
                        start.red(), end.red(), pos, limit),
                animray::interpolation::linear(
                        start.green(), end.green(), pos, limit),
                animray::interpolation::linear(
                        start.blue(), end.blue(), pos, limit));
    }
    auto quantise(float channel) {
        return uint8_t(std::clamp(channel, 0.0f, 255.0f));
    }
}


FSL_MAIN("triangle", "AnimRay. Copyright 2010-2020 Kirit Saelensminde")
(fostlib::ostream &out, fostlib::arguments &args) {
    animray::film<animray::rgb<uint8_t>> image{
            width, height, [](auto x, auto y) {
                auto const left = lerp(top_left, bottom_left, y, height);
                auto const right = lerp(top_right, bottom_right, y, height);
                auto const pixel = lerp(left, right, x, width);
                return animray::rgb<uint8_t>(
                        quantise(pixel.red()), quantise(pixel.green()),
                        quantise(pixel.blue()));
            }};
    animray::targa("triangle-linear.tga", image);
    return 0;
}
