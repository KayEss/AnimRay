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


#ifndef ANIMRAY_MIXINS_DEPTH_COUNT_HPP
#define ANIMRAY_MIXINS_DEPTH_COUNT_HPP
#pragma once


#include <fost/core>
#include <animray/mixins/mixin.hpp>


namespace animray {


    namespace detail {
        class depth_counted {
        public:
            depth_counted()
            : depth_count(1) {
            }
            template<
                typename R, typename... A,
                typename std::enable_if<
                        not std::is_base_of<depth_counted, R>::value
                    >::type *E = nullptr
            >
            depth_counted(const R &, A&&...)
            : depth_count(1) {
            }
            template<
                typename R, typename... A,
                typename std::enable_if<
                        std::is_base_of<depth_counted, R>::value
                    >::type *E = nullptr
            >
            depth_counted(const R &item, A&&...)
            : depth_count(item.depth_count() + 1) {
            }

            fostlib::accessors<std::size_t> depth_count;
        };
    }


    template<typename T>
    struct with_depth_count {
        typedef typename std::conditional<
                std::is_base_of<detail::depth_counted, T>::value,
                T,
                mixin<T, detail::depth_counted>
            >::type type;
    };


}


#endif // ANIMRAY_MIXINS_DEPTH_COUNT_HPP
