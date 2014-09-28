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


#ifndef ANIMRAY_EPSILON_HPP
#define ANIMRAY_EPSILON_HPP
#pragma once


namespace animray {


    /// In general we just want the epsilon value for some intersection type
    template<typename I>
    struct epsilon {
        static constexpr typename I::local_coord_type value =
            epsilon<typename I::local_coord_type>::value;
    };


    /// Specialisation for doubles
    template<>
    struct epsilon<double> {
        static constexpr double value = 1e-12;
    };


}


#endif // ANIMRAY_EPSILON_HPP
