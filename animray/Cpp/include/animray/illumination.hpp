/*
    Copyright 2014, Kirit Saelensminde.
    http://www.kirit.com/AnimRay

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


#ifndef ANIMRAY_ILLUMINATION_HPP
#define ANIMRAY_ILLUMINATION_HPP
#pragma once


#include <animray/ray.hpp>


namespace animray {


    /// Like a ray, but carries with it an illumination model
    template< typename R, typename C, typename S >
    class beam {
        R ray;
        const S &scene;
        public:
            /// The ray type for specifying the direction of the beam
            typedef R ray_type;
            /// The colour model for the beam
            typedef C color_type;
            /// The scene type for the beam
            typedef S scene_type;

            /// Construct a beam
            beam(const ray_type &ray, const scene_type &scene)
            : ray(ray), scene(scene) {
            }
    };

    /// A scene featuring a light and a model
    template< typename S, typename L,
            typename B = typename S::beam_type >
    class scene {
        public:
            typedef S geometry_type;
            typedef L light_type;
            typedef B beam_type;

            scene &insert(const geometry_type &g) {
                geometry.push_back(g);
                return *this;
            }

        private:
            geometry_type geometry;
            light_type light;
    };


}


#endif // ANIMRAY_ILLUMINATION_HPP
