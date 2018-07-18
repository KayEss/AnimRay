/*
    Copyright 2014-2018, Kirit Saelensminde.
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


#include <animray/affine.hpp>
#include <animray/ray.hpp>
#include <animray/matrix.hpp>


namespace animray {


    /// Handles forward and backward transformation between 3D co-ordinate systems
    template< typename M >
    class transformable {
    protected:
        M forward, backward;
    public:
        /// A transformation
        typedef std::pair<M, M> transform_type;

        /// Apply an affine transformation
        transformable &operator () (const transform_type &t) {
            // Swap forward and backward here because we're
            // going from world to local
            forward = t.second * forward;
            backward *= t.first;
            return *this;
        }

        /// Apply a transformation
        template<typename T>
        transformable &operator () (const T &t) {
            forward = t.backward() * forward;
            backward *= t.forward();
            return *this;
        }
    };


    /// Concrete type for a given scene object
    template< typename O,
            typename I = typename O::intersection_type,
            typename T = transformable< matrix< typename O::local_coord_type > >
    >
    class movable : private T {
        typedef T superclass;
    public:
        /// The type of object that can be moved
        typedef O instance_type;
        /// The type of the local coordinate system
        typedef typename O::local_coord_type local_coord_type;
        /// The type of the intersection of the instance
        typedef I intersection_type;
        /// The type of the transformation that needs to be applied
        typedef typename T::transform_type transform_type;

        /// Store the instance
        fostlib::accessors<instance_type, fostlib::lvalue> instance;

        /// Allow the underlying instance to be constructed
        template<typename... A>
        explicit movable(A&&... args)
        : instance(std::forward<A>(args)...) {}

        /// Apply a transformation
        movable &operator () (const transform_type &t) {
            superclass::operator()(t);
            return *this;
        }

        /// Apply a translation
        movable &operator () (const translate<local_coord_type> &t) {
            superclass::operator()(t);
            return *this;
        }

        /// Ray intersection
        template< typename R, typename E >
        std::optional<intersection_type> intersects(
            const R &by, const E epsilon
        ) const {
            const auto hit{instance().intersects(by * superclass::forward, epsilon)};
            if ( hit ) {
                return hit.value() * superclass::backward;
            } else {
                return {};
            }
        }

        /// Occlusion check
        template< typename R >
        bool occludes(const R &by, const local_coord_type epsilon) const {
            return instance().occludes(by * superclass::forward, epsilon);
        }

        /// Allow the instance to be used as a camera
        template< typename F >
        intersection_type operator () (F x, F y) const {
            return instance()(x, y) * superclass::backward;
        }
    };


}


#endif // ANIMRAY_MOVABLE_HPP
