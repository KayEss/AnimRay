/*
    Copyright 2014-2019, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#ifndef ANIMRAY_EPSILON_HPP
#define ANIMRAY_EPSILON_HPP
#pragma once


namespace animray {

    /**
     * `epsilon` is used to manage near zero results in a predictable
     * manner.
     */

    /// The default epsilon is zero
    template<typename T>
    constexpr T epsilon = T{};

    /// Specialisation for doubles
    template<>
    constexpr double epsilon<double> = 1e-12;


}


#endif // ANIMRAY_EPSILON_HPP
