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


#ifndef ANIMRAY_MIXINS_TIME_HPP
#define ANIMRAY_MIXINS_TIME_HPP
#pragma once


namespace animray {


    namespace detail {
        template<typename RI>
        class reflected_ray : public RI {
        public:
            reflected_ray(const reflected_ray &ray,
                    const typename RI::end_type &starts,
                    const typename RI::direction_type &dir)
            : RI(starts, dir), depth(ray.depth() + 1) {
            }
            reflected_ray(const RI &ray,
                    const typename RI::end_type &starts,
                    const typename RI::direction_type &dir)
            : RI(starts, dir), depth(1) {
            }

            fostlib::accessors<std::size_t> depth;
        };

        template<typename R>
        struct ref_type {
            typedef reflected_ray<R> type;
        };
        template<typename R>
        struct ref_type<reflected_ray<R>> {
            typedef reflected_ray<R> type;
        };
    }


}


#endif // ANIMRAY_MIXINS_TIME_HPP
