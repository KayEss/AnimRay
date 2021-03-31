/**
    Copyright 2014-2021, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#ifndef ANIMRAY_TEST_HPP
#define ANIMRAY_TEST_HPP
#pragma once


#include <animray/point3d.hpp>
#include <felspar/test.hpp>


namespace animray {


    /// Check if two values are near each other
    template<typename C, typename D>
    inline void
            check_close(C check, D const &left, D const &right, D const &error) {
        check(left) >= right - error;
        check(right) <= right + error;
    }

    /// Check if two points are close enough to be considered the same
    template<typename C, typename D>
    inline void check_close(
            C check,
            animray::point3d<D> const &l,
            animray::point3d<D> const &r,
            D const error = 1e-3) {
        check_close(check, l.x(), r.x(), error);
        check_close(check, l.y(), r.y(), error);
        check_close(check, l.z(), r.z(), error);
    }


}


#endif // ANIMRAY_TEST_HPP
