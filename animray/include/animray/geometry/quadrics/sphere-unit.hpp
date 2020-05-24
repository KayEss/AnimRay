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


#ifndef ANIMRAY_GEOMETRY_QUADRICS_SPHERE_UNIT_HPP
#define ANIMRAY_GEOMETRY_QUADRICS_SPHERE_UNIT_HPP
#pragma once


#include <animray/functional/reduce.hpp>
#include <animray/geometry/quadrics/sphere-unit-origin.hpp>
#include <animray/ray.hpp>


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
        using local_coord_type = D;
        /// Type of intersection to return when the sphere is struck
        using intersection_type = I;

        /// Set the position of the sphere
        position_type position;

        /// Returns a ray giving the intersection point and surface normal or
        /// null if no intersection occurs
        template<typename R, typename E>
        fostlib::nullable<intersection_type>
                intersects(R by, const E epsilon) const {
            by.from = by.from - reduce(position, by);
            fostlib::nullable<intersection_type> hit(
                    origin.intersects(by, epsilon));
            if (hit) {
                hit->from = hit->from + reduce(position, by);
                return hit;
            } else {
                return fostlib::null;
            }
        }

        /// Returns true if the ray hits the sphere
        template<typename R, typename E>
        bool occludes(R by, const E epsilon) const {
            by.from = by.from - reduce(position, by);
            return origin.occludes(by, epsilon);
        }
    };


}


#endif // ANIMRAY_GEOMETRY_QUADRICS_SPHERE_UNIT_HPP
