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


#ifndef ANIMRAY_MIXINS_TIME_HPP
#define ANIMRAY_MIXINS_TIME_HPP
#pragma once


#include <animray/mixins/mixin.hpp>

#include <fost/datetime>


namespace animray {


    namespace detail {
        struct at_time {
            std::chrono::system_clock::time_point time = {};
        };
    }


    /// Mixin for recording a time stamp
    template<typename T>
    struct with_time {
        typedef typename std::conditional<
                std::is_base_of<detail::at_time, T>::value,
                T,
                mixin<T, detail::at_time>>::type type;
    };


}


#endif // ANIMRAY_MIXINS_TIME_HPP
