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


#ifndef ANIMRAY_VALUE_HPP
#define ANIMRAY_VALUE_HPP
#pragma once


#include <functional>


namespace animray {


    /// Allow parameters to be applied such as to calculate some value
    template<typename V, typename... P>
    V value(const V &v, const P &...) {
        return v;
    }


    /// Apply the parameters to the function
    template<typename V, typename... P>
    V value(const std::function<V(const P &...)> &f, const P &...ps) {
        return f(ps...);
    }


}


#endif // ANIMRAY_VALUE_HPP
