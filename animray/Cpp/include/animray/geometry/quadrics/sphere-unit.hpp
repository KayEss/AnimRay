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


#ifndef ANIMRAY_GEOMETRY_QUADRICS_SPHERE_UNIT_HPP
#define ANIMRAY_GEOMETRY_QUADRICS_SPHERE_UNIT_HPP
#pragma once


#include <animray/affine.hpp>
#include <animray/ray.hpp>
#include <animray/geometry/quadrics/sphere-unit-origin.hpp>


namespace animray {


    template<
        typename P,
        typename I = animray::ray<typename P::value_type>,
        typename D = typename P::value_type>
    class unit_sphere {
        /// The sphere at the origin we want to map position to
        unit_sphere_at_origin<I, D> origin;
    public:
        /// Position type
        typedef P position_type;
        /// The type of the local coordinates used
        typedef D local_coord_type;
        /// Type of intersection to return when the sphere is struck
        typedef I intersection_type;

        /// Set the position of the sphere
        fostlib::accessors<position_type> position;

        /// Returns a ray giving the intersection point and surface normal or
        /// null if no intersection occurs
        template<typename R, typename E>
        fostlib::nullable< intersection_type > intersects(R by, const E epsilon) const {
            by.from(by.from() - position());
            fostlib::nullable< intersection_type > hit(origin.intersects(by, epsilon));
            if ( hit.isnull() ) {
                return fostlib::null;
            } else {
                hit.value().from(hit.value().from() + position());
                return hit;
            }
        }

        /// Returns true if the ray hits the sphere
        template<typename R, typename E>
        bool occludes(R by, const E epsilon) const {
            by.from(by.from() - position());
            return origin.occludes(by, epsilon);
        }
    };


}


#endif // ANIMRAY_GEOMETRY_QUADRICS_SPHERE_UNIT_HPP
