/**
    Copyright 2021, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#include <animray/geometry/quadrics/sphere-unit-origin.hpp>


namespace animray {


    template<
            typename S,
            typename P,
            typename I = animray::ray<typename P::value_type>,
            typename D = typename P::value_type>
    class sphere {
        unit_sphere_at_origin<I, D> origin;

      public:
        using size_type = S;
        using position_type = P;
        using local_coord_type = D;
        using intersection_type = I;

        /// Sphere location and radius
        position_type position;
        size_type radius;

        /// Construction
        sphere() : position{}, radius{1} {}
        sphere(position_type p, size_type const r)
        : position{std::move(p)}, radius{r} {}

        template<typename R, typename E>
        std::optional<intersection_type>
                intersects(R by, E const epsilon) const {
            by.from = by.from - position;
            by.from = by.from / radius;
            auto hit = origin.intersects(by, epsilon);
            if (hit) {
                hit->from = hit->from * radius;
                hit->from = hit->from + position;
                return hit;
            } else {
                return {};
            }
        }
        template<typename R, typename E>
        bool occludes(R by, E const epsilon) const {
            by.from = by.from - position;
            by.from = by.from / radius;
            return origin.occludes(by, epsilon);
        }
    };


}
