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


#ifndef ANIMRAY_INTERPOLATION_LINEAR_HPP
#define ANIMRAY_INTERPOLATION_LINEAR_HPP
#pragma once


#include <fost/datetime>


namespace animray {


    namespace interpolation {


        /// Linear interpolation
        template<typename S, typename T>
        S linear(const S end, const T time, const T outof) {
            const S perunit(end / outof);
            return perunit * time;
        }
        template<typename S, boost::int64_t P>
        S linear(
            const S end,
            const boost::date_time::subsecond_duration<boost::posix_time::time_duration, P> time,
            const boost::date_time::subsecond_duration<boost::posix_time::time_duration, P> outof
        ) {
            const S perunit(end * P / outof.ticks());
            return perunit * time.ticks() / P;
        }


        /// Linear interpolator
        template<typename S, typename T>
        S linear(const S start, const S end, const T time, const T outof) {
            return start + linear(end - start, time, outof);
        }


    }


}


#endif // ANIMRAY_INTERPOLATION_LINEAR_HPP
