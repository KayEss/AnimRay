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


#ifndef ANIMRAY_ANIMATION_PROCEDURAL_ROTATE_HPP
#define ANIMRAY_ANIMATION_PROCEDURAL_ROTATE_HPP
#pragma once


#include <animray/animation/animate.hpp>
#include <animray/interpolation/linear.hpp>


namespace animray {


    namespace animation {


        /// Spin an object
        template<
            typename W,
            std::pair<matrix<W>, matrix<W>> A(const W&),
            typename O
        >
        class affine {
            W start, end;
            std::size_t frames;
        public:
            /// The type of object that can be moved
            typedef O instance_type;
            /// Intersection type
            typedef typename O::intersection_type intersection_type;

            /// Store the instance
            fostlib::accessors<instance_type, fostlib::lvalue> instance;

            /// Store the animation parameters
            affine &operator() (const W &s, const W &e, const std::size_t f) {
                start = s; end = e; frames = f;
                return *this;
            }

            /// Calculate the transformation matrix
            template<typename R>
            std::pair<matrix<W>, matrix<W>> matrices(const R &ray) const {
                return A(interpolation::linear(start, end, ray.frame(), frames));
            }

            /// Ray intersection
            template<typename R, typename E>
            fostlib::nullable< intersection_type > intersects(
                const R &by, const E epsilon
            ) const {
                std::pair<matrix<W>, matrix<W>> transform(matrices(by));
                fostlib::nullable< intersection_type >
                    hit(instance().intersects(by * transform.first, epsilon));
                if ( hit.isnull() ) {
                    return fostlib::null;
                } else {
                    return hit.value() * transform.second;
                }
            }

            /// Occlusion check
            template<typename R, typename E>
            bool occludes(const R &by, const E epsilon) const {
                std::pair<matrix<W>, matrix<W>> transform(matrices(by));
                return instance().occludes(by * transform.first, epsilon);
            }

        private:
        };


    }


}


#endif // ANIMRAY_ANIMATION_PROCEDURAL_ROTATE_HPP
