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


#ifndef ANIMRAY_SURFACE_HPP
#define ANIMRAY_SURFACE_HPP
#pragma once


#include <animray/ray.hpp>


namespace animray {


    /// Implements a simple shader
    template< typename O, typename C,
        typename I = typename O::intersection_type >
    class surface {
    public:
        /// The underlying object type
        typedef O instance_type;
        /// The colour type
        typedef C color_type;
        /// The type of the local coordinate system
        typedef typename instance_type::local_coord_type local_coord_type;
        /// The intersection type
        typedef I intersection_type;

        /// The geometry that is being shaded
        fostlib::accessors<instance_type, fostlib::lvalue> geometry;

        /// Calculate the intersection of the ray on the instance
        template<typename R>
        fostlib::nullable< intersection_type > intersects(const R &by) const {
            return geometry().intersects(by);
        }

        /// Calculate whether this object occludes the ray or not
        template< typename R >
        bool occludes(const R &by, const local_coord_type epsilon) const {
            return geometry().occludes(by, epsilon);
        }
    };


}


#endif // ANIMRAY_SURFACE_HPP
