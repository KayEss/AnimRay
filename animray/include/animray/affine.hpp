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


#ifndef ANIMRAY_AFFINE_HPP
#define ANIMRAY_AFFINE_HPP
#pragma once


#include <animray/matrix.hpp>


namespace animray {


    /// Return matrices for moving the co-ordinate system by the requested amounts
    template<typename W>
    class translate {
        const W x, y, z;

      public:
        /// Construct a translation of the requested amount
        translate(const W &tx, const W &ty, const W &tz)
        : x(tx), y(ty), z(tz) {}

        point3d<W> operator()() const { return point3d<W>(x, y, z); }

        /// Return the forward matrix for the translation
        matrix<W> forward() const {
            matrix<W> f;
            f[0][3] = x;
            f[1][3] = y;
            f[2][3] = z;
            return f;
        }

        /// Return the backward matrix for the translation
        matrix<W> backward() const {
            matrix<W> b;
            b[0][3] = -x;
            b[1][3] = -y;
            b[2][3] = -z;
            return b;
        }
    };


    /// Return matrices for scaling along each axis.
    template<typename W>
    std::pair<matrix<W>, matrix<W>>
            scale(const W &sx, const W &sy, const W &sz) {
        matrix<W> f, b;
        f[0][0] = sx;
        b[0][0] = W(1) / sx;
        f[1][1] = sy;
        b[1][1] = W(1) / sy;
        f[2][2] = sz;
        b[2][2] = W(1) / sz;
        return std::make_pair(f, b);
    }


    /// Rotate about the x-axis
    template<typename W>
    std::pair<matrix<W>, matrix<W>> rotate_x(const W &radians) {
        matrix<W> f, b;
        f[0][0] = W(1);
        b[0][0] = W(1);
        f[1][1] = cos(radians);
        b[1][1] = cos(-radians);
        f[1][2] = -sin(radians);
        b[1][2] = -sin(-radians);
        f[2][1] = sin(radians);
        b[2][1] = sin(-radians);
        f[2][2] = cos(radians);
        b[2][2] = cos(-radians);
        f[3][3] = W(1);
        b[3][3] = W(1);
        return std::make_pair(f, b);
    }


    /// Rotate about the x-axis
    template<typename W>
    std::pair<matrix<W>, matrix<W>> rotate_y(const W &radians) {
        matrix<W> f, b;
        f[0][0] = cos(radians);
        b[0][0] = cos(-radians);
        f[0][2] = sin(radians);
        b[0][2] = sin(-radians);
        f[1][1] = W(1);
        b[1][1] = W(1);
        f[2][0] = -sin(radians);
        b[2][0] = -sin(-radians);
        f[2][2] = cos(radians);
        b[2][2] = cos(-radians);
        f[3][3] = W(1);
        b[3][3] = W(1);
        return std::make_pair(f, b);
    }


    /// Rotate about the x-axis
    template<typename W>
    std::pair<matrix<W>, matrix<W>> rotate_z(const W &radians) {
        matrix<W> f, b;
        f[0][0] = cos(radians);
        b[0][0] = cos(-radians);
        f[0][1] = -sin(radians);
        b[0][1] = -sin(-radians);
        f[1][0] = sin(radians);
        b[1][0] = sin(-radians);
        f[1][1] = cos(radians);
        b[1][1] = cos(-radians);
        f[2][2] = W(1);
        b[2][2] = W(1);
        f[3][3] = W(1);
        b[3][3] = W(1);
        return std::make_pair(f, b);
    }


}


#endif // ANIMRAY_AFFINE_HPP
