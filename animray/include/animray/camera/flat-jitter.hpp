/*
    Copyright 2014-2018, Kirit Saelensminde.
    <https://kirit.com/AnimRay>

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


#include <animray/camera/flat.hpp>
#include <animray/threading/random-generator.hpp>


namespace animray {


    /// Camera that introduces random 2D jitter on the sample locations
    template<
        typename E,
        typename C = flat_camera<E>,
        typename J = random::jitter<std::uniform_real_distribution<E>>
    >
    class flat_jitter_camera {
        /// The camera performing the base mapping
        C inner_camera;
    public:
        /// The type used to measure the height of the camera image
        using extents_type = E;
        /// The type used to measure the physical resolution of the camera
        using resolution_type = typename C::resolution_type;

        /// Constructs a camera whose film is a particular size
        flat_jitter_camera(extents_type w, extents_type h,
                resolution_type c, resolution_type r)
        : inner_camera(w, h, c, r) {
        }

        /// Map between pixel co-ordinates and world co-ordinates
        auto operator () (resolution_type x, resolution_type y) const {
            return inner_camera(x, y) +
                point2d<extents_type>(
                    J::sample() * inner_camera.pixel_width(),
                    J::sample() * inner_camera.pixel_height());
        }
    };


}

