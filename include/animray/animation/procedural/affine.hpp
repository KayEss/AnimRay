/**
    Copyright 2014-2021, [Kirit Saelensminde](https://kirit.com/AnimRay).

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


#ifndef ANIMRAY_ANIMATION_PROCEDURAL_AFFINE_HPP
#define ANIMRAY_ANIMATION_PROCEDURAL_AFFINE_HPP
#pragma once


#include <animray/matrix.hpp>
#include <animray/animation/animate.hpp>
#include <animray/interpolation/linear.hpp>


namespace animray::animation {


    /// Apply an animated affine transformation to the object
    template<typename W, typename T, typename O>
    class affine {
        std::add_pointer_t<T> lambda;
        typename W::value_type start{}, end{};
        std::size_t frames{};

      public:
        /// The type of object that can be moved
        using instance_type = O;
        /// Intersection type
        using intersection_type = typename O::intersection_type;
        /// Global co-ordinate base type
        using local_coord_type = typename O::local_coord_type;

        /// Store the instance
        instance_type instance;

        constexpr affine() noexcept {}
        constexpr affine(
                std::add_pointer_t<T> t,
                typename W::value_type const &s,
                typename W::value_type const &e,
                std::size_t const f,
                O &&o) noexcept
        : lambda{t}, start{s}, end{e}, frames{f}, instance{std::move(o)} {}

        /// Store the animation parameters
        affine &operator()(
                T l,
                const typename W::value_type &s,
                const typename W::value_type &e,
                const std::size_t f) {
            lambda = std::move(l);
            start = s;
            end = e;
            frames = f;
            return *this;
        }

        /// Calculate the transformation matrix
        template<typename R>
        std::pair<W, W> matrices(const R &ray) const {
            return lambda(interpolation::linear(start, end, ray.frame, frames));
        }

        /// Ray intersection
        template<typename R, typename E>
        std::optional<intersection_type>
                intersects(const R &by, const E epsilon) const {
            std::pair<W, W> transform(matrices(by));
            std::optional<intersection_type> hit(
                    instance.intersects(by * transform.first, epsilon));
            if (hit) {
                return hit.value() * transform.second;
            } else {
                return {};
            }
        }

        /// Occlusion check
        template<typename R, typename E>
        bool occludes(const R &by, const E epsilon) const {
            std::pair<W, W> transform(matrices(by));
            return instance.occludes(by * transform.first, epsilon);
        }

      private:
    };


    template<typename A, typename B, typename E, typename O>
    affine(A, B, E, std::size_t, O) -> affine<
            matrix<typename O::local_coord_type>,
            std::remove_pointer_t<A>,
            O>;


}


#endif // ANIMRAY_ANIMATION_PROCEDURAL_AFFINE_HPP
