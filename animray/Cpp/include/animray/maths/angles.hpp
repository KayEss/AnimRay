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


#ifndef ANIMRAY_MATHS_ANGLES_BASED
#define ANIMRAY_MATHS_ANGLES_BASED
#pragma once


#include <boost/math/constants/constants.hpp>


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


#endif // ANIMRAY_MATHS_ANGLES_BASED
