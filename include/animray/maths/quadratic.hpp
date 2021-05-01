/**
    Copyright 2014-2021, [Kirit Saelensminde](https://kirit.com/AnimRay)

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
#include <optional>


namespace animray {


    /// Returns true if the quadratic has real solutions within the provided range
    template<typename D>
    inline bool quadratic_has_solution(
            D const, D const b, D const c, D const range) {
        D const discriminant = b * b - D(4) * c;
        if (discriminant < D(0)) { return false; }
        D const disc_root = std::sqrt(discriminant);
        return ((-b - disc_root >= range) or (-b + disc_root >= range));
    }


    /// Returns the smallest real solution to the quadratic if it lies inside
    /// the range
    template<typename D>
    inline std::optional<D> first_positive_quadratic_solution(
            D const a, D const b, D const c, D const range) {
        D const discriminant = b * b - D{4} * a * c;
        if (discriminant < D{}) { return {}; }
        auto const root_discrim = std::sqrt(discriminant);
        using S = std::remove_cv_t<decltype(root_discrim)>;
        S const q{
                -(S{1} / S{2})
                * (b + (b < S{0} ? -root_discrim : +root_discrim))};
        S t0 = q / a, t1 = c / q;
        if (t1 < t0) { std::swap(t0, t1); }
        S t = t0 < S(0) ? t1 : t0;
        if (t < range) {
            return {};
        } else {
            return t;
        }
    }


}


#endif // ANIMRAY_MATHS_QUADRATIC_HPP
