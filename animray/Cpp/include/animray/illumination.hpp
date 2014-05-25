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


    /// Unlit part of a scene
    template< typename M >
    class dark {
    };

    /// A light
    template< typename M >
    class light {
    };

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


}


#endif // ANIMRAY_ILLUMINATION_HPP
