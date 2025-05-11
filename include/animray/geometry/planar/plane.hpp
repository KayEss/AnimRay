/**
    Copyright 2014-2025, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#include <animray/maths/dot.hpp>
#include <optional>


namespace animray {


    template<typename I, typename D = typename I::local_coord_type>
    struct plane {
        /// ### The type of the local coordinates used
        using local_coord_type = D;
        /// ### Type of intersection to be returned
        using intersection_type = I;


        /// ### The centre of the plane
        point3d<local_coord_type> center;
        /// ### Surface normal
        unit_vector<local_coord_type> normal;


        /// ### Calculate the intersection point
        template<typename R, typename E>
        std::optional<intersection_type>
                intersects(R by, const E epsilon) const {
            const local_coord_type dot_normal(
                    animray::dot(by.direction, normal));
            if (dot_normal == local_coord_type()) { return {}; }
            const local_coord_type numerator(
                    animray::dot(normal, center - by.from));
            const local_coord_type t(numerator / dot_normal);
            if (t > epsilon) {
                return intersection_type(
                        by.from + by.direction * t,
                        dot_normal < 0 ? normal : -normal);
            } else {
                return {};
            }
        }

        /// ### Returns true if the ray hits the plane
        template<typename R, typename E>
        bool occludes(R by, const E epsilon) const {
            return intersects(by, epsilon).has_value();
        }

        /// ### Comparison
        friend bool operator==(plane const &, plane const &) = default;
    };


}
