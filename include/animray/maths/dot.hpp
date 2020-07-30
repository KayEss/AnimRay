/*
    Copyright 1995-2014, Kirit Saelensminde.
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


#ifndef ANIMRAY_MATHS_DOT_BASED
#define ANIMRAY_MATHS_DOT_BASED
#pragma once


#include <animray/unit-vector.hpp>


namespace animray {


    /// Dot product for two unit vectors
    template<typename D>
    D dot(const unit_vector<D> &d1, const unit_vector<D> &d2) {
        return d1.x() * d2.x() + d1.y() * d2.y() + d1.z() * d2.z();
    }
    /// Dot product for unit vectors and point
    template<typename D>
    D dot(const point3d<D> &d1, const unit_vector<D> &d2) {
        return d1.x() * d2.x() + d1.y() * d2.y() + d1.z() * d2.z();
    }
    /// Dot product for points and unit vectors
    template<typename D>
    D dot(const unit_vector<D> &d1, const point3d<D> &d2) {
        return d1.x() * d2.x() + d1.y() * d2.y() + d1.z() * d2.z();
    }
    /// Dot product for points
    template<typename D>
    D dot(const point3d<D> &d1, const point3d<D> &d2) {
        return d1.x() * d2.x() + d1.y() * d2.y() + d1.z() * d2.z();
    }


}


#endif // ANIMRAY_MATHS_DOT_BASED
