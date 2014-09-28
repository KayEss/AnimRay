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


#ifndef ANIMRAY_SURFACE_REFLECTIVE_HPP
#define ANIMRAY_SURFACE_REFLECTIVE_HPP
#pragma once


#include <animray/epsilon.hpp>
#include <animray/surface.hpp>


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


    /// The matte surface intersection type
    template<typename C>
    class reflective {
    public:
        /// Default constructor
        reflective() {}

        /// The absorption attenuation of the surface
        typedef C parameters;

        /// Calculate the light coming from the reflected ray
        template< typename RI, typename RL, typename I,
            typename CI, typename G >
        CI reflected(
            const RI &observer, const RL &light,
            const I &intersection, const CI &, const G &scene
        ) const {
            typedef typename RI::local_coord_type accuracy;
            const accuracy ci = -dot(observer.direction(), intersection.direction());
            const unit_vector< accuracy > ri(
                observer.direction() +
                    intersection.direction() * accuracy(2) * ci);
            typename detail::ref_type<RI>::type refray(
                observer, intersection.from(), ri);
            if ( refray.depth() > 5 ) {
                return CI();
            }
            fostlib::nullable<typename G::intersection_type>
                reflected(scene.geometry().intersects(refray, epsilon<I>::value));
            if ( reflected.isnull() ) {
                return CI();
            } else {
                return scene.light()(refray, reflected.value(), scene);
            }
        }

        /// Calculate the light/surface interaction
        template< typename RI, typename RL, typename I,
            typename CI, typename G >
        CI operator () (
            const C &attenuation, const RI &observer, const RL &light,
            const I &intersection, const CI &incident, const G &scene
        ) const {
            return reflected(observer, light, intersection, incident, scene)
                * attenuation;
        }
    };


}


#endif // ANIMRAY_SURFACE_REFLECTIVE_HPP
