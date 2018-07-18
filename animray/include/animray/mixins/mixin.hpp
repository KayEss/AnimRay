/*
    Copyright 2014-2018, Kirit Saelensminde.
    <https://kirit.com/AnimRay>

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


    /// # Mixins
    /**
     * These are used to annotate some type `R` with extra information and
     * APIs provided by `M`. In all cases where this is used the intention is
     * that the main ray tracer object may need to carry with it extra
     * information that is then used later on. For example, a ray emitted by
     * a camera may add a frame number used by later geometry that uses
     * the frame number to move its location -- this is how animation is
     * performed in AnimRay.
     */
    template<typename R, typename M>
    struct mixin : public R, public M {
        /// Default constructor
        mixin()
        : R{}, M{} {
        }

        /// Construct by forwarding all arguments to both bases
        template<typename... A>
        explicit mixin(A&&... args)
        : R{args...}, M{args...} {
        }

        /// These operations always go to `R` because that is where geometry etc.
        /// lives.
        template<typename B>
        auto operator * (const B &by) const {
            mixin r{*this};
            r.R::operator *= (by);
            return r;
        }
    };


}


#endif // ANIMRAY_MIXINS_MIXIN_HPP
