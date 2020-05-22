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


namespace animray {


    /// Collection of lights of a single type
    template<typename C, typename L>
    class light<std::vector<L>, C> : public light<void, C> {
        typedef light<void, C> superclass;

      public:
        /// The container type
        using container_type = std::vector<L>;
        /// The type of the light
        using light_type = L;
        /// The colour model
        using color_type = C;

        /// Add a light to this collection
        auto &push_back(const light_type &light) {
            _lights.push_back(light);
            return *this;
        }

        /// Calculate the illumination given by this light
        template<typename O, typename R, typename G>
        color_type operator()(
                const O &observer, const R &intersection, const G &scene) const {
            color_type c(superclass::color);
            for (const auto &i : _lights) {
                c += i(observer, intersection, scene);
            }
            return c;
        }

      private:
        container_type _lights;
    };


    /// A collection of lights of differing types
    template<typename C, typename L1, typename... Ls>
    class light<std::tuple<L1, Ls...>, C> :
    public light<void, C>,
            public std::tuple<L1, Ls...> {
        using superclass = light<void, C>;
        using tuple_type = std::tuple<L1, Ls...>;

      public:
        /// The colour model
        using color_type = C;

        /// Calculate the illumination given by this light
        template<typename O, typename R, typename G>
        color_type operator()(
                const O &observer, const R &intersection, const G &scene) const {
            return superclass::color
                    + std::apply(
                            [&observer, &intersection,
                             &scene](auto &&... light) -> color_type {
                                return (light(observer, intersection, scene)
                                        + ...);
                            },
                            static_cast<const tuple_type &>(*this));
        };
    };


}


#endif // ANIMRAY_LIGHT_COLLECTION_HPP
