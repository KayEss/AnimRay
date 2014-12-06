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


#ifndef ANIMRAY_MATHS_CROSS_BASED
#define ANIMRAY_MATHS_CROSS_BASED
#pragma once


#include <animray/unit-vector.hpp>


namespace animray {


    /// Cross product for unit vector and point
    template<typename D>
    point3d<D> cross(const unit_vector<D> &b, const point3d<D> &c) {
        return point3d<D>(
            b.y() * c.z() - b.z() * c.y(),
            b.z() * c.x() - b.x() * c.z(),
            b.x() * c.y() - b.y() * c.x());
    }
    /// Cross product for points
    template<typename D>
    point3d<D> cross(const point3d<D> &b, const point3d<D> &c) {
        return point3d<D>(
            b.y() * c.z() - b.z() * c.y(),
            b.z() * c.x() - b.x() * c.z(),
            b.x() * c.y() - b.y() * c.x());
    }


}


#endif // ANIMRAY_MATHS_CROSS_BASED
