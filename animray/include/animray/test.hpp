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


#ifndef ANIMRAY_TEST_HPP
#define ANIMRAY_TEST_HPP
#pragma once


#include <fost/test>
#include <animray/point3d.hpp>


namespace animray {


    /// Check if two points are close enough to be considered the same
    template<typename D>
    void check_close(
            animray::point3d<D> const &l,
            animray::point3d<D> const &r,
            D const error = 1e-3) {
        try {
            FSL_CHECK_ERROR(l.x(), r.x(), error);
            FSL_CHECK_ERROR(l.y(), r.y(), error);
            FSL_CHECK_ERROR(l.z(), r.z(), error);
        } catch (fostlib::exceptions::exception &e) {
            fostlib::insert(e.data(), "check_close", "error", error);
            // fostlib::insert(e.data(), "check_close", "left", l);
            // fostlib::insert(e.data(), "check_close", "right", r);
            throw;
        }
    }


}


#endif // ANIMRAY_TEST_HPP
