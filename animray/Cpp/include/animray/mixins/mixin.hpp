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


#ifndef ANIMRAY_MIXINS_MIXIN_HPP
#define ANIMRAY_MIXINS_MIXIN_HPP
#pragma once


#include <utility>


namespace animray {


    /// Handle two way mixins
    template<typename R, typename M>
    struct mixin : public R, public M {
        /// Default constructor
        mixin()
        : R(), M() {
        }
        /// Construct by forwarding all arguments to both bases
        template<typename... A>
        explicit mixin(A&&... args)
        : R(std::forward<A>(args)...), M(std::forward<A>(args)...) {
        }
    };


}


#endif // ANIMRAY_MIXINS_MIXIN_HPP
