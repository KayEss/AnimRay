/*
    Copyright 1995-2010, Kirit Saelensminde.
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


#ifndef ANIMRAY_DETAIL_POINT2D_HPP
#define ANIMRAY_DETAIL_POINT2D_HPP
#pragma once


/// Allow two points to be added together
template< typename C > inline
animray::point2d< C > operator + (
    const animray::point2d< C > &a, const animray::point2d< C > &b
) {
    return animray::point2d< C >( a.x() + b.x(), a.y() + b.y() );
}
/// Allow two points to be subracted
template< typename C > inline
animray::point2d< C > operator - (
    const animray::point2d< C > &a, const animray::point2d< C > &b
) {
    return animray::point2d< C >( a.x() - b.x(), a.y() - b.y() );
}
/// Allow us to scale a point location by a scalar
template< typename C > inline
animray::point2d< C > operator * (
    const C &a, const animray::point2d< C > &b
) {
    return animray::point2d< C >( a * b.x(), a * b.y() );
}
/// Allow us to scale a point location by a scalar
template< typename C > inline
animray::point2d< C > operator * (
    const animray::point2d< C > &a, const C &b
) {
    return animray::point2d< C >( a.x() * b, a.y() * b );
}
/// Allow us to divide a point location by a scalar
template< typename C > inline
animray::point2d< C > operator / (
    const C &a, const animray::point2d< C > &b
) {
    return animray::point2d< C >( a / b.x(), a / b.y() );
}
/// Allow us to divide a point location by a scalar
template< typename C > inline
animray::point2d< C > operator / (
    const animray::point2d< C > &a, const C &b
) {
    return animray::point2d< C >( a.x() / b, a.y() / b );
}


#endif // ANIMRAY_DETAIL_POINT2D_HPP
