/**
    Copyright 2021-2025, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#pragma once


#if not defined(NO_CONCEPTS_HEADER) and __has_include(<concepts>)


#include <concepts>


namespace animray {


    template<typename C>
    concept Regular = requires { requires std::regular<C>; };


}


#else


namespace animray {


    template<typename C>
    concept Regular = requires { true; };


}


#endif
