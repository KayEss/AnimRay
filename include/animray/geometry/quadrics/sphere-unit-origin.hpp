/*
    Copyright 1995-2021, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#ifndef ANIMRAY_GEOMETRY_QUADRICS_UNIT_SPHERE_ORIGIN_HPP
#define ANIMRAY_GEOMETRY_QUADRICS_UNIT_SPHERE_ORIGIN_HPP
#pragma once


#include <animray/epsilon.hpp>
#include <animray/ray.hpp>
#include <animray/maths/dot.hpp>
#include <animray/maths/quadratic.hpp>


namespace animray {


    /// A simple 3D unit sphere primitive at the origin.
    template<typename I, typename D = typename I::local_coord_type>
    struct unit_sphere_at_origin {
        /// The type of the local coordinates used
        using local_coord_type = D;
        /// Type of intersection to return when the sphere is struck
        using intersection_type = I;

        /// Check for equality
        bool operator==(const unit_sphere_at_origin &) const { return true; }
        /// Check for inequality
        bool operator!=(const unit_sphere_at_origin &) const { return false; }

        /// Returns the b c values for the quadratic given a start and direction
        template<typename R>
        static std::pair<D, D> quadratic_b_c(const R &by) {
            return std::make_pair(
                    D{2} * dot(by.from, by.direction), by.from.dot() - D{1});
        }

        /// Returns a ray giving the intersection point and surface normal or
        /// null if no intersection occurs
        template<typename R>
        std::optional<intersection_type>
                intersects(const R &by, D const eps = epsilon<D>) const {
            const std::pair<D, D> bc(quadratic_b_c(by));
            const std::optional<D> t(first_positive_quadratic_solution(
                    D(1), bc.first, bc.second, eps));
            if (t) {
                using end_type = typename ray<D>::end_type;
                using direction_type = typename ray<D>::direction_type;
                direction_type normal(by.from + by.direction * *t);
                return intersection_type(end_type(normal), normal);
            } else {
                return {};
            }
        }

        /// Returns true if the ray hits the sphere
        template<typename R>
        bool occludes(const R &by, D const eps = epsilon<D>) const {
            const std::pair<D, D> bc(quadratic_b_c(by));
            return quadratic_has_solution(D(1), bc.first, bc.second, eps);
        }
    };


}


#endif // ANIMRAY_GEOMETRY_QUADRICS_UNIT_SPHERE_ORIGIN_HPP
