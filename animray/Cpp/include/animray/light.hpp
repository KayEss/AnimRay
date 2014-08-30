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


#include <tuple>
#include <animray/shader.hpp>


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
        template< typename O, typename R, typename G >
        color_type operator () (const O &, const R &, const G &) const {
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
        template< typename O, typename I, typename G >
        typename superclass::color_type operator () (
            const O &observer, const I &intersection, const G &scene
        ) const {
            ray<typename O::local_coord_type> illumination(
                intersection.from(), geometry());
            if ( not scene.occludes(illumination, typename I::local_coord_type(1) /
                    typename I::local_coord_type(10000000000000) ) ) {
                return shader(observer, illumination, intersection, superclass::color(), scene);
            } else {
                return typename superclass::color_type();
            }
        }
    };


    /// Collection of lights of a single type
    template< typename C, typename L >
    class light<std::vector<L>, C> : public light<void, C > {
        typedef light<void, C> superclass;
    public:
        /// The container type
        typedef std::vector<L> container_type;
        /// The type of the light
        typedef L light_type;
        /// The colour model
        typedef C color_type;

        /// Add a light to this collection
        light<std::vector<L>, C> &push_back(const light_type &light) {
            _lights.push_back(light);
            return *this;
        }

        /// Calculate the illumination given by this light
        template< typename O, typename R, typename G >
        color_type operator () (
            const O &observer, const R &intersection, const G &scene
        ) const {
            color_type c(superclass::color());
            for ( const auto &i : _lights ) {
                c += i(observer, intersection, scene);
            }
            return c;
        }

    private:
        container_type _lights;
    };


    /// A collection of lights of differing types
    template<typename C, typename L1, typename... Ls>
    class light<std::tuple<L1, Ls...>, C>
            : public light<void, C>, public std::tuple<L1, Ls...> {
        typedef light<void, C> superclass;
        typedef std::tuple<L1, Ls...> tuple_type;

        template<typename O, typename R, typename G, std::size_t S>
        struct helper {
            typename std::tuple_element<S, tuple_type>::type::color_type lighting(
                const tuple_type &lights, const O &observer,
                const R &intersection, const G &scene
            ) const {
                return helper<O, R, G, S - 1>().lighting(lights, observer, intersection, scene) +
                    std::get<S>(lights)(observer, intersection, scene);
            }
        };
        template<typename O, typename R, typename G>
        struct helper<O, R, G, 0> {
            typename std::tuple_element<0, tuple_type>::type::color_type lighting(
                const tuple_type &lights, const O &observer,
                    const R &intersection, const G &scene
            ) const {
                return std::get<0>(lights)(observer, intersection, scene);
            }
        };

    public:
        /// The colour model
        typedef C color_type;

        /// Calculate the illumination given by this light
        template< typename O, typename R, typename G >
        color_type operator () (
            const O &observer, const R &intersection, const G &scene
        ) const {
            return superclass::color() +
                helper<O, R, G, std::tuple_size<tuple_type>::value - 1>().lighting(
                    *this, observer, intersection, scene);
        };
    };



}


#endif // ANIMRAY_LIGHT_HPP
