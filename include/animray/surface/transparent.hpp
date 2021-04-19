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


#include <animray/mixins/depth-count.hpp>


namespace animray {


    /// A surface that allows propagation of a ray through it without change.
    /// This is not the same as a refractive surface as the ray always passes
    /// through in the same direction it hits the surface
    template<typename C>
    class transparent {
      public:
        static bool const can_occlude = false;

        transparent() = default;
        transparent(C c) : transparency{std::move(c)} {}
        transparent(C c, std::size_t const md)
        : transparency{std::move(c)}, max_depth{md} {}

        /// The transparency of the material
        C transparency;
        /// The maximum depth the material is transparent to
        std::size_t max_depth = 5;

        /// Calculate the light/surface interaction
        template<typename RI, typename RL, typename I, typename CI, typename G>
        CI operator()(const RI &, const RL &, const I &, const CI &, const G &)
                const {
            // There is never any light due to illumination
            return CI();
        }

        /// The light coming through is treated as emissive
        template<typename CI, typename RI, typename I, typename G>
        CI operator()(
                CI const &,
                RI const &observer,
                I const &intersection,
                G const &scene) const {
            typename animray::with_depth_count<RI>::type transray(observer);
            transray.add_count(observer);
            if (transray.depth_count > max_depth) {
                return scene.background;
            } else {
                transray.from = intersection.from;
                transray.direction = observer.direction;
                return scene(transray) * transparency;
            }
        }
    };


}
