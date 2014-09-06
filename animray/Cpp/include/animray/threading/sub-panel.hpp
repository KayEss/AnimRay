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


#ifndef ANIMRAY_THREADING_SUB_PANEL_HPP
#define ANIMRAY_THREADING_SUB_PANEL_HPP
#pragma once


#include <animray/panel.hpp>


namespace animray {


    namespace threading {


        namespace detail {
            /// Calculate the greatest common denominator
            template< typename S >
            S gcd(S a, S b) {
                while ( b != 0 ) {
                    S t = b;
                    b = a % b;
                    a = t;
                }
                return a;
            }
            /// Calculate the next power of 2
            template< typename S >
            S bigestodd(S n) {
                while ( (n & 1) == 0 ) {
                    n /= 2;
                }
                return n;
            }
        }


        /// A mechanism whereby the frame is rendered in a number of sub-panels
        template<typename F, typename S, typename Fn>
        F sub_panel(S w, S h, Fn fn) {
            return F(w, h, fn);
        }


    }


}


#endif // ANIMRAY_THREADING_SUB_PANEL_HPP
