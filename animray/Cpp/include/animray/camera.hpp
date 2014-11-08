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


#ifndef ANIMRAY_CAMERA_HPP
#define ANIMRAY_CAMERA_HPP
#pragma once


#include <animray/point2d.hpp>
#include <random>
#include <animray/camera/flat.hpp>


namespace animray {


    /// Camera that introduces random 2d jitter on the sample locations
    template< typename E, typename C = flat_camera< E > >
    class flat_jitter_camera {
        /// The camera performing the base mapping
        C inner_camera;
        /// The entropy generator
        mutable std::default_random_engine generator;
        /// The distribution for the jitter
        mutable std::uniform_real_distribution<E> jitter;
    public:
        /// The type used to measure the height of the camera image
        typedef E extents_type;
        /// The type used to measure the physical resolution of the camera
        typedef typename C::resolution_type resolution_type;

        /// Constructs a camera whose film is a particular size
        flat_jitter_camera(extents_type w, extents_type h,
                resolution_type c, resolution_type r)
        : inner_camera(w, h, c, r),
                jitter(-inner_camera.half(), +inner_camera.half()) {
        }

        /// Map between pixel co-ordinates and world co-ordinates
        point2d< extents_type > operator() (
                resolution_type x, resolution_type y) const {
            return inner_camera(x, y) +
                point2d< extents_type >(
                    jitter(generator) * inner_camera.pixel_width(),
                    jitter(generator) * inner_camera.pixel_height());
        }
    };


}


#endif // ANIMRAY_CAMERA_HPP
