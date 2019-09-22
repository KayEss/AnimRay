/**
    Copyright 2014-2019, [Kirit Saelensminde](https://kirit.com/AnimRay)

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


#ifndef ANIMRAY_MATHS_QUADRATIC_HPP
#define ANIMRAY_MATHS_QUADRATIC_HPP
#pragma once


#include <cmath>


namespace animray {


    /// Returns true if the quadratic has real solutions within the provided range
    template<typename D>
    bool quadratic_has_solution(const D a, const D b, const D c, const D range) {
        const D discriminant = b * b - D(4) * c;
        if (discriminant < D(0)) return false;
        const D disc_root = std::sqrt(discriminant);
        if (-b - disc_root >= range) return true;
        if (-b + disc_root >= range) return true;
        return false;
    }


    /// Returns the smallest real solution to the quadratic if it lies inside
    /// the range
    template<typename D>
    fostlib::nullable<D> first_positive_quadratic_solution(
            const D a, const D b, const D c, const D range) {
        const D discrim = b * b - D(4) * a * c;
        if (discrim < D(0)) { return fostlib::null; }
        using S = decltype(std::sqrt(discrim));
        S const root_discrim(std::sqrt(discrim));
        S const q(
                -(S{1} / S{2})
                * (b + (b < S(0) ? -root_discrim : +root_discrim)));
        S t0 = q / a, t1 = c / q;
        if (t1 < t0) { std::swap(t0, t1); }
        S t = t0 < S(0) ? t1 : t0;
        if (t < range) {
            return fostlib::null;
        } else {
            return t;
        }
    }


}


#endif // ANIMRAY_MATHS_QUADRATIC_HPP
