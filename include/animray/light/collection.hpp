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


#ifndef ANIMRAY_LIGHT_COLLECTION_HPP
#define ANIMRAY_LIGHT_COLLECTION_HPP
#pragma once


#include <animray/light/light.hpp>

#include <array>
#include <tuple>
#include <vector>


namespace animray {


    /// Collection of lights of a single type
    template<typename C, Light L, std::size_t N>
    class light<std::array<L, N>, C> {
      public:
        /// The container type
        using container_type = std::array<L, N>;
        /// The type of the light
        using light_type = L;
        /// The colour model
        using color_type = C;

        /// Place the lights
        explicit constexpr light(std::array<L, N> l) noexcept
        : lights{std::move(l)} {}

        /// Calculate the illumination given by this light
        template<typename O, typename R, typename G>
        color_type operator()(
                const O &observer, const R &intersection, const G &scene) const {
            color_type c{};
            for (auto const &i : lights) {
                c += i(observer, intersection, scene);
            }
            return c;
        }

      private:
        container_type lights;
    };
    template<Light L, std::size_t N>
    light(std::array<L, N>) -> light<std::array<L, N>, typename L::color_type>;


    /// Collection of lights of a single type
    template<typename C, typename L>
    class light<std::vector<L>, C> {
      public:
        /// The container type
        using container_type = std::vector<L>;
        /// The type of the light
        using light_type = L;
        /// The colour model
        using color_type = C;

        /// Add a light to this collection
        auto &push_back(const light_type &light) {
            lights.push_back(light);
            return *this;
        }

        /// Calculate the illumination given by this light
        template<typename O, typename R, typename G>
        color_type operator()(
                const O &observer, const R &intersection, const G &scene) const {
            color_type c{};
            for (const auto &i : lights) {
                c += i(observer, intersection, scene);
            }
            return c;
        }

      private:
        container_type lights;
    };


    /// A collection of lights of differing types
    template<typename C, Light L1, Light... Ls>
    class light<std::tuple<L1, Ls...>, C> : public std::tuple<L1, Ls...> {
        using tuple_type = std::tuple<L1, Ls...>;

      public:
        /// The colour model
        using color_type = C;

        template<typename... LL>
        constexpr light(LL... ls) noexcept
        : tuple_type{std::forward<LL>(ls)...} {}

        /// Calculate the illumination given by this light
        template<typename O, typename R, typename G>
        color_type operator()(
                const O &observer, const R &intersection, const G &scene) const {
            return std::apply(
                    [&](auto &&...light) -> color_type {
                        return (light(observer, intersection, scene) + ...);
                    },
                    static_cast<tuple_type const &>(*this));
        }
    };
    template<Light L1, Light L2, Light... Ls>
    light(L1, L2, Ls...)
            -> light<std::tuple<L1, L2, Ls...>, typename L1::color_type>;


}


#endif // ANIMRAY_LIGHT_COLLECTION_HPP
