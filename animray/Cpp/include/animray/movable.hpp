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


#ifndef ANIMRAY_MOVABLE_HPP
#define ANIMRAY_MOVABLE_HPP
#pragma once


#include <animray/matrix.hpp>


namespace animray {


    /// Describe a type that allows artefacts to be transformed
    template< typename O, typename W, typename R >
    class movable;


    /// Abstract base class
    template<typename W, typename R>
    class movable<void, W, R> {
        public:
            /// The type of the local coordinate system
            typedef W local_coord_type;
            /// The type of the ray output by the instance
            typedef R ray_type;

            /// Allow this to be safely used as a superclass.
            virtual ~movable() = default;

            /// Ray intersection
            virtual fostlib::nullable< ray_type > intersection(
                const ray_type &by) const = 0;

            /// Occlusion check
            virtual bool occludes(const ray_type &by,
                const local_coord_type epsilon) const = 0;

        protected:
            matrix<W> forward, backward;
    };


    /// Concrete type for a given scene object
    template< typename O, typename W = typename O::local_coord_type,
        typename R = ray<W> >
    class movable : public movable<void, W, R> {
        O instance;
        typedef movable<void, W> superclass;
        public:
            /// The type of object that can be moved
            typedef O instance_type;
            /// The type of the local coordinate system
            typedef W local_coord_type;
            /// The type of the ray output by the instance
            typedef R ray_type;
            /// A transformation
            typedef std::pair<matrix<local_coord_type>,
                    matrix<local_coord_type>> transform_type;

            /// Allow the underlying instance to be constructed
            template<typename... A>
            explicit movable(A&&... args)
            : instance(std::forward<A>(args)...) {}

            /// Apply an affine transformation
            movable &operator () (
                const transform_type &t
            ) {
                // Swap forward and backward here because we're
                // going from world to local
                superclass::forward *= t.second;
                superclass::backward *= t.first;
                return *this;
            }

            /// Ray intersection
            fostlib::nullable< ray_type > intersection(const ray_type &by) const {
                return instance.intersection(by * superclass::forward)
                    * superclass::backward;
            }

            /// Occlusion check
            bool occludes(
                const ray_type &by,
                const local_coord_type epsilon = local_coord_type(0)
            ) const {
                return instance.occludes(by * superclass::forward, epsilon);
            }

            /// Allow the instance to be used as a camera
            template< typename F >
            ray_type operator() (F x, F y) const {
                return instance(x, y) * superclass::backward;
            }
    };


}


#endif // ANIMRAY_MOVABLE_HPP
