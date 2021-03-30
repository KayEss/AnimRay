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


#pragma once


#include <type_traits>
#include <utility>


namespace animray {


    template<typename C>
    concept Spectrum = requires(C c) {
        std::is_trivially_destructible_v<C>;
    };

    template<typename C>
    concept PixelValue = requires(C c) {
        std::is_trivially_destructible_v<C>;
    };


    namespace detail {
        template<typename T, typename F>
        struct color_conversion;
    }
    /// Colour conversion between colour spaces
    template<Spectrum T, Spectrum F>
    T convert_to(F &&f) {
        return detail::color_conversion<T, F>{}.convert(std::forward<F>(f));
    }
    // template<PixelValue T, PixelValue F>
    // T convert_to(F &&f) {
    //     return detail::color_conversion<T, F>()(std::forward<F>(f));
    // }


}
