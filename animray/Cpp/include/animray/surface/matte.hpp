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


#ifndef ANIMRAY_SURFACE_MATTE_HPP
#define ANIMRAY_SURFACE_MATTE_HPP
#pragma once


#include <animray/surface.hpp>


namespace animray {


    /// The matte surface intersection type
    template<typename I, typename C>
    class matte : public I {
        typedef I superclass;
    public:
        class parameters {
        public:
            parameters(const C &c)
            : attenuation(c) {
            }

            /// Store the light attenuation
            fostlib::accessors<C> attenuation;
        };

        /// Default constructor (nothing struck)
        matte()
        : superclass(), m_struck(nullptr) {
        }
        /// Construct an intersection that also stores the struck geometry
        matte(
            const superclass &intersection, const parameters &struck
        ) : superclass(intersection), m_struck(&struck) {
        }

        /// The geometry that was hit by the intersection
        const parameters &struck() const {
            return *m_struck;
        }

        /// Multiply by something
        template< typename S >
        matte operator * ( const S &s ) {
            superclass r(superclass::operator * (s));
            return matte(r, *m_struck);
        }
    private:
        const parameters *m_struck;
    };

    template<typename C, typename I, typename IC, typename R, typename G>
    struct surface_interaction< C, matte<I, IC>, R, G > {
        surface_interaction() {}
        C operator() (
            const R &light,
            const matte<I, IC> &intersection,
            const C &incident,
            const G &
        ) const {
            const typename R::local_coord_type costheta =
                    dot(light.direction(), intersection.direction());
            return incident * intersection.struck().attenuation() * costheta;
        }
    };


}


#endif // ANIMRAY_SURFACE_MATTE_HPP
