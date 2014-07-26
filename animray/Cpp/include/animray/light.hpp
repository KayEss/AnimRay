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


#ifndef ANIMRAY_LIGHT_HPP
#define ANIMRAY_LIGHT_HPP
#pragma once


namespace animray {


    /// Lights allow illumination of the scene
    template< typename L, typename C >
    class light;


    /// Void lights are ambient
    template< typename C >
    class light<void, C> {
    public:
        /// The colour type
        typedef C color_type;

        /// The colour of the light
        fostlib::accessors< color_type > color;

        /// Default construct a light with no illumincation
        light()
        : color() {
        }
        /// Construct with a given color
        explicit light(const color_type &c)
        : color(c) {
        }

        /// Calculate the illumination given by this light
        template< typename R, typename G >
        color_type operator () (const R &intersection, const G &scene) const {
            return color();
        }
    };


    /// Point lights
    template< typename C, typename W >
    class light<point3d<W>, C> : public light<void, C> {
        typedef light<void, C> superclass;
    public:
        /// The light geometry
        typedef point3d<W> geometry_type;

        /// The geometry of the light
        fostlib::accessors< geometry_type > geometry;

        /// Construct from a position and color
        light(const geometry_type &p, const typename superclass::color_type &c)
        : superclass(c), geometry(p) {
        }

        /// Calculate the illumination given by this light
        template< typename R, typename G >
        typename superclass::color_type operator () (
            const R &intersection, const G &scene
        ) const {
            R illumination(intersection.from(), geometry());
            if ( not scene.occludes(illumination, typename R::value_type(1) /
                    typename R::value_type(1000000000) ) ) {
                const typename R::value_type costheta =
                    dot(illumination.direction(), intersection.direction());
                return superclass::color() * costheta;
            } else {
                return typename superclass::color_type(0);
            }
        }
    };


    /// Collections of lights of a single type
    template< typename S >
    class lights {
    public:
        /// The container type
        typedef S container_type;
        /// The type of the light
        typedef typename S::value_type light_type;
        /// The colour model
        typedef typename light_type::color_type color_type;

        /// Add a light to this collection
        lights &push_back(const light_type &light) {
            _lights.push_back(light);
            return *this;
        }

        /// Calculate the illumination given by this light
        template< typename R, typename G >
        color_type operator () (const R &intersection, const G &scene) const {
            color_type c;
            for ( const auto &i : _lights ) {
                c += i(intersection, scene);
            }
            return c;
        }

    private:
        container_type _lights;
    };


}


#endif // ANIMRAY_LIGHT_HPP
