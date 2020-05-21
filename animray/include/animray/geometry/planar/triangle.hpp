/**
    Copyright 2014-2019, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#ifndef ANIMRAY_PLANAR_TRIANGLE_HPP
#define ANIMRAY_PLANAR_TRIANGLE_HPP
#pragma once


#include <animray/maths/cross.hpp>
#include <animray/maths/dot.hpp>


namespace animray {


    template<typename I, typename D = typename I::local_coord_type>
    class triangle : private detail::array_based<point3d<D>, 3> {
        typedef detail::array_based<point3d<D>, 3> superclass;

      public:
        /// The type of the local coordinates used
        typedef D local_coord_type;
        /// Type of intersection to be returned
        typedef I intersection_type;
        /// The type of the corners
        typedef point3d<local_coord_type> corner_type;

        /// Construct a triangle from three points
        constexpr triangle(
                corner_type one, corner_type two, corner_type three) noexcept {
            superclass::array[0] = std::move(one);
            superclass::array[1] = std::move(two);
            superclass::array[2] = std::move(three);
        }

        /// Calculate the intersection point
        template<typename R, typename E>
        fostlib::nullable<intersection_type>
                intersects(R by, const E epsilon) const {
            // Möller–Trumbore intersection algorithm
            const corner_type e1(superclass::array[1] - superclass::array[0]);
            const corner_type e2(superclass::array[2] - superclass::array[0]);

            const corner_type P(cross(by.direction(), e2));
            const local_coord_type determinant(dot(e1, P));
            if (determinant > -epsilon && determinant < epsilon) {
                return fostlib::null;
            }
            const local_coord_type inv_determinant(
                    local_coord_type(1) / determinant);

            const corner_type T(by.from() - superclass::array[0]);
            const local_coord_type u(dot(T, P) * inv_determinant);
            if (u < local_coord_type() || u > local_coord_type(1)) {
                return fostlib::null;
            }

            const corner_type Q(cross(T, e1));
            const local_coord_type v(dot(by.direction(), Q) * inv_determinant);
            if (v < local_coord_type() || u + v > local_coord_type(1)) {
                return fostlib::null;
            }

            const local_coord_type t(dot(e2, Q) * inv_determinant);
            if (t > epsilon) {
                typename intersection_type::direction_type normal(
                        cross(e2, e1));
                if (dot(normal, by.direction()) < local_coord_type()) {
                    return intersection_type(
                            by.from() + by.direction() * t, normal);
                } else {
                    return intersection_type(
                            by.from() + by.direction() * t, -normal);
                }
            } else {
                return fostlib::null;
            }
        }

        /// Returns true if the ray hits the triangle
        template<typename R, typename E>
        bool occludes(R by, const E epsilon) const {
            return intersects(by, epsilon).has_value();
        }
    };


}


#endif // ANIMRAY_PLANAR_TRIANGLE_HPP
