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


#ifndef ANIMRAY_SCENE_HPP
#define ANIMRAY_SCENE_HPP
#pragma once


#include <animray/epsilon.hpp>
#include <animray/emission.hpp>

#include <optional>
#include <utility>


namespace animray {


    /// A scene featuring a light and a model
    template<typename G, typename L, typename C>
    class scene {
      public:
        /// The geometry type
        using geometry_type = G;
        /// The light type
        using light_type = L;
        /// The colour type
        using color_type = C;
        /// The type of the rays used
        using intersection_type = typename geometry_type::intersection_type;
        using local_coord_type = typename intersection_type::local_coord_type;

        /// Construct an empty scene
        constexpr scene() = default;
        constexpr scene(G g, L l, C c)
        : geometry{std::move(g)}, light{std::move(l)}, background{std::move(c)} {}

        /// Store the geometry
        geometry_type geometry;
        /// Store the light
        light_type light;
        /// Background colour
        color_type background;

        /// Given a position on the camera film, calculate the colour it should be
        template<typename M, typename S>
        color_type operator()(const M &camera, S x, S y) const {
            typename M::intersection_type observer(camera(x, y));
            return (*this)(observer);
        }

        /// Given a ray work out how much light is returned along it
        template<typename R>
        color_type operator()(const R &observer) const {
            std::optional<intersection_type> intersection(geometry.intersects(
                    observer,
                    epsilon<typename intersection_type::local_coord_type>));
            if (intersection) {
                return color_type(light(observer, intersection.value(), *this))
                        + emission<color_type>(
                                observer, intersection.value(), *this);
            } else {
                return background;
            }
        }
    };

    template<typename G, typename L, typename C>
    scene(G, L, C) -> scene<G, L, C>;


}


#endif // ANIMRAY_SCENE_HPP
