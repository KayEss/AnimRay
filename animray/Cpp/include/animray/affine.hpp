/*
    Copyright 2014, Kirit Saelensminde.
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


#ifndef ANIMRAY_AFFINE_HPP
#define ANIMRAY_AFFINE_HPP
#pragma once


#include <animray/matrix.hpp>
#include <boost/math/constants/constants.hpp>


namespace animray {


    /// Return matrices for moving the co-ordinate system by the requested amounts
    template<typename W>
    std::pair<matrix<W>, matrix<W>> translate(
            const W &tx, const W &ty, const W &tz
    ) {
        matrix<W> f, b;
        f[0][3] = tx; b[0][3] = -tx;
        f[1][3] = ty; b[1][3] = -ty;
        f[2][3] = tz; b[2][3] = -tz;
        return std::make_pair(f, b);
    }


    /// Return matrices for scaling along each axis.
    template<typename W>
    std::pair<matrix<W>, matrix<W>> scale(
        const W &sx, const W &sy, const W &sz
    ) {
        matrix<W> f, b;
        f[0][0] = sx; b[0][0] = W(1) / sx;
        f[1][1] = sy; b[1][1] = W(1) / sy;
        f[2][2] = sz; b[2][2] = W(1) / sz;
        return std::make_pair(f, b);
    }


    /// Rotate about the x-axis
    template<typename W>
    std::pair<matrix<W>, matrix<W>> rotate_x(const W &radians) {
        matrix<W> f, b;
        f[0][0] = W(1); b[0][0] = W(1);
        f[1][1] = cos(radians); b[1][1] = cos(-radians);
        f[1][2] = -sin(radians); b[1][2] = -sin(-radians);
        f[2][1] = sin(radians); b[2][1] = sin(-radians);
        f[2][2] = cos(radians); b[2][2] = cos(-radians);
        f[3][3] = W(1); b[3][3] = W(1);
        return std::make_pair(f, b);
    }


    /// Rotate about the x-axis
    template<typename W>
    std::pair<matrix<W>, matrix<W>> rotate_y(const W &radians) {
        matrix<W> f, b;
        f[0][0] = cos(radians); b[0][0] = cos(-radians);
        f[0][2] = sin(radians); b[0][2] = sin(-radians);
        f[1][1] = W(1); b[1][1] = W(1);
        f[2][0] = -sin(radians); b[2][0] = -sin(-radians);
        f[2][2] = cos(radians); b[2][2] = cos(-radians);
        f[3][3] = W(1); b[3][3] = W(1);
        return std::make_pair(f, b);
    }


    /// Rotate about the x-axis
    template<typename W>
    std::pair<matrix<W>, matrix<W>> rotate_z(const W &radians) {
        matrix<W> f, b;
        f[0][0] = cos(radians); b[0][0] = cos(-radians);
        f[0][1] = -sin(radians); b[0][1] = -sin(-radians);
        f[1][0] = sin(radians); b[1][0] = sin(-radians);
        f[1][1] = cos(radians); b[1][1] = cos(-radians);
        f[2][2] = W(1); b[2][2] = W(1);
        f[3][3] = W(1); b[3][3] = W(1);
        return std::make_pair(f, b);
    }


}


/// Define a way to use degree literals (converted to radians)
constexpr long double operator "" _deg(long double d) {
    return boost::math::constants::pi<long double>()
        / (long double)(180.0) * d;
}
/// Define a way to use degree literals (converted to radians)
constexpr long double operator ""_deg(unsigned long long d) {
    return boost::math::constants::pi<long double>()
        / (long double)(180.0) * (long double)(d);
}


#endif // ANIMRAY_AFFINE_HPP
