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


#ifndef ANIMRAY_SURFACE_HPP
#define ANIMRAY_SURFACE_HPP
#pragma once


#include <animray/shader.hpp>


namespace animray {


    /// Stores a surface color
    template< typename O, typename C >
    class surface {
    public:
        /// The underlying object type
        typedef O instance_type;
        /// The colour type
        typedef C color_type;
        /// The type of the local coordinate system
        typedef typename instance_type::local_coord_type local_coord_type;
        /// The intersection type
        class intersection_type : public O::intersection_type {
            typedef typename O::intersection_type superclass;
            const surface *m_struck;
        public:
            /// Default constructor (nothing struck)
            intersection_type()
            : superclass(), m_struck(nullptr) {
            }
            /// Construct an intersection that also stores the struck geometry
            intersection_type(
                const superclass &intersection, const surface &struck
            ) : superclass(intersection), m_struck(&struck) {
            }

            /// The geometry that was hit by the intersection
            surface &struck() {
                return *m_struck;
            }

            /// Multiply by something
            template< typename S >
            intersection_type operator * ( const S &s ) {
                superclass r(superclass::operator * (s));
                return intersection_type(r, *m_struck);
            }
        };

        /// Constructor captures the color
        surface(const color_type &c)
        : attenuation(c) {
        }

        /// Capture the colour attenuation used by the surface
        fostlib::accessors< color_type > attenuation;

        /// The geometry that is being shaded
        fostlib::accessors<instance_type, fostlib::lvalue> geometry;

        /// Calculate the intersection of the ray on the instance
        template<typename R>
        fostlib::nullable< intersection_type > intersects(const R &by) const {
            fostlib::nullable< typename O::intersection_type >
                hit(geometry().intersects(by));
            if ( hit.isnull() ) {
                return fostlib::null;
            } else {
                return intersection_type(hit.value(), *this);
            }
        }

        /// Calculate whether this object occludes the ray or not
        template< typename R >
        bool occludes(const R &by, const local_coord_type epsilon) const {
            return geometry().occludes(by, epsilon);
        }
    };


    template<typename O, typename C, typename R, typename G>
    struct surface_interaction< C,
            typename surface<O, C>::intersection_type, R, G > {
        C operator() (
            const R &light,
            const typename surface<O, C>::intersection_type &intersection,
            const C &incident,
            const G &
        ) const {
            const typename R::local_coord_type costheta =
                    dot(light.direction(), intersection.direction());
            return incident * intersection.struck().attenuation() * costheta;
        }
    };


}


#endif // ANIMRAY_SURFACE_HPP
