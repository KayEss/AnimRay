/**
    Copyright 2014-2020, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#ifndef ANIMRAY_LIGHT_AMBIENT_HPP
#define ANIMRAY_LIGHT_AMBIENT_HPP
#pragma once


#include <animray/color/concept.hpp>
#include <animray/light/light.hpp>

#include <utility>


namespace animray {


    /// Void lights are ambient
    template<typename C>
    class light<void, C> {
      public:
        /// The colour type
        using color_type = C;

        /// The colour of the light
        color_type color{};

        /// Default construct a light with no illumination
        constexpr light() noexcept {}
        /// Construct with a given color
        explicit constexpr light(color_type c) noexcept : color{std::move(c)} {}

        /// Calculate the illumination given by this light
        template<typename O, typename R, typename G>
        color_type operator()(const O &, const R &, const G &) const {
            return color;
        }
    };
    template<Color C>
    light(C) -> light<void, C>;


}


#endif // ANIMRAY_LIGHT_AMBIENT_HPP
