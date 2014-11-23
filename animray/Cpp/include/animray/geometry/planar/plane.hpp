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


#ifndef ANIMRAY_PLANAR_PLANE_HPP
#define ANIMRAY_PLANAR_PLANE_HPP
#pragma once


#include <animray/unit-vector.hpp>


namespace animray {


    template<typename I, typename D = typename I::local_coord_type>
    class plane {
    public:
        /// The type of the local coordinates used
        typedef D local_coord_type;
        /// Type of intersection to be returned
        typedef I intersection_type;

        /// The centre of the plane
        fostlib::accessors<point3d<local_coord_type>> center;
        /// Surface normal
        fostlib::accessors<unit_vector<local_coord_type>> normal;

        /// Calculate the intersection point
        template<typename R, typename E>
        fostlib::nullable< intersection_type > intersects(R by, const E epsilon) const {
            throw fostlib::exceptions::not_implemented("plane intersection");
        }

        /// Returns true if the ray hits the sphere
        template<typename R, typename E>
        bool occludes(R by, const E epsilon) const {
            throw fostlib::exceptions::not_implemented("plane occlusion");
        }
    };


}


#endif // ANIMRAY_PLANAR_PLANE_HPP
