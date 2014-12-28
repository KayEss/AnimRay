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


#ifndef ANIMRAY_MIXINS_FRAME_HPP
#define ANIMRAY_MIXINS_FRAME_HPP
#pragma once


#include <animray/mixins/mixin.hpp>
#include <fost/core>


namespace animray {


    namespace detail {
        template<typename F>
        class at_frame {
        public:
            at_frame() {
            }
            fostlib::accessors<F> frame;
        };
    }


    /// Mixin for recording a frame number
    template<typename T, typename F = std::size_t>
    struct with_frame {
        typedef typename std::conditional<
                std::is_base_of<detail::at_frame<F>, T>::value,
                T,
                mixin<T, detail::at_frame<F>>
            >::type type;
    };


}


#endif // ANIMRAY_MIXINS_FRAME_HPP
