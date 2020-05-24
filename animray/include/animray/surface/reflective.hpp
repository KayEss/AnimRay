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


#ifndef ANIMRAY_SURFACE_REFLECTIVE_HPP
#define ANIMRAY_SURFACE_REFLECTIVE_HPP
#pragma once


#include <animray/epsilon.hpp>
#include <animray/surface.hpp>
#include <animray/mixins/depth-count.hpp>
#include <animray/unit-vector.hpp>


namespace animray {


    /// The matte surface intersection type
    template<typename C>
    class reflective {
      public:
        /// Default constructor
        reflective() = default;

        /// The absorption attenuation of the surface
        typedef C parameters;

        /// Calculate the light coming from the reflected ray
        template<typename RI, typename I, typename CI, typename G>
        CI reflected(
                const CI &,
                const RI &observer,
                const I &intersection,
                const G &scene) const {
            using accuracy = typename RI::local_coord_type;
            const accuracy ci =
                    -dot(observer.direction, intersection.direction);
            const unit_vector<accuracy> ri(
                    observer.direction
                    + intersection.direction * accuracy(2) * ci);
            typename animray::with_depth_count<RI>::type refray(observer);
            refray.add_count(observer);
            refray.from = intersection.from;
            refray.direction = ri;
            if (refray.depth_count > 5) {
                return scene.background;
            } else {
                return scene(refray);
            }
        }

        /// Calculate the light/surface interaction
        template<typename RI, typename RL, typename I, typename CI, typename G>
        CI operator()(
                const C &,
                const RI &,
                const RL &,
                const I &,
                const CI &,
                const G &) const {
            // There is never any light due to illumination
            return CI();
        }

        /// The specular light is emissive
        template<typename CI, typename RI, typename I, typename G>
        CI operator()(
                const C &attenuation,
                const CI &c,
                const RI &observer,
                const I &intersection,
                const G &scene) const {
            return reflected(c, observer, intersection, scene) * attenuation;
        }
    };


}


#endif // ANIMRAY_SURFACE_REFLECTIVE_HPP
