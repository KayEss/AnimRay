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


#ifndef ANIMRAY_CAMERA_FLAT_HPP
#define ANIMRAY_CAMERA_FLAT_HPP
#pragma once


#include <animray/point2d.hpp>


namespace animray {


    /// Camera for 2d imaging
    template< typename E, typename R = std::size_t >
    class flat_camera {
    public:
        /// The type used to measure the height of the camera image
        typedef E extents_type;
        /// The type used to measure the physical resolution of the camera
        typedef R resolution_type;

        /// Constructs a camera whose film is a particular size
        flat_camera(extents_type w, extents_type h,
                resolution_type c, resolution_type r)
        : width(w), height(h), columns(c), rows(r) {
        }

        /// Give a value for the width of a pixel in world co-ordinates
        extents_type pixel_width() const {
            return width() / columns();
        }
        /// Give a value for the height of a pixel in world co-ordinates
        extents_type pixel_height() const {
            return height() / rows();
        }

        /// Convert from resolution co-ordinates to world co-ordinates
        point2d< extents_type > operator() (
                resolution_type x, resolution_type y) const {
            return point2d< extents_type >(
                width() * ((extents_type(x) + half()) / columns() - half()),
                -height() * ((extents_type(y) + half()) / rows() - half()));
        }

        /// The width of the camera
        fostlib::accessors<const extents_type> width;
        /// The height of the camera
        fostlib::accessors<const extents_type> height;
        /// The number of columns
        fostlib::accessors<const resolution_type> columns;
        /// The number of rows of resolution
        fostlib::accessors<const resolution_type> rows;

        /// Useful constant
        constexpr static extents_type half() {
            return extents_type(1) / extents_type(2);
        }
    };


}


#endif // ANIMRAY_CAMERA_FLAT_HPP
